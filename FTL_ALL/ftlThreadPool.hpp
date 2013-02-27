#ifndef FTL_THREADPOOL_HPP
#define FTL_THREADPOOL_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlThreadPool.h"
#endif

namespace FTL
{
	///////////////////////////////////////////// CFJobBase ///////////////////////////////////////////////////
	template <typename T>
	CFJobBase<T>::CFJobBase()
	{
		m_nJobIndex = 0;
		m_pThreadPool = NULL;
		m_hEventJobStop = NULL;
	}
	template <typename T>
	CFJobBase<T>::~CFJobBase()
	{
		Finalize();
	}

	template <typename T>
	LONG CFJobBase<T>::GetJobIndex() const
	{
		return m_nJobIndex;
	}

	template <typename T>
	BOOL CFJobBase<T>::NotifyStop()
	{
		BOOL bRet = FALSE;
		API_VERIFY(SetEvent(m_hEventJobStop));
		return bRet;
	}

	template <typename T>
	void CFJobBase<T>::OnCancelJob(T param)
	{
		UNREFERENCED_PARAMETER(param);
		//delete pParam; //注意先转换
		//delete this; 
		//注意：此处是个空实现
	}

	template <typename T>
	BOOL CFJobBase<T>::Initialize()
	{
		FTLASSERT(NULL == m_hEventJobStop);
		BOOL bRet = FALSE;
		API_VERIFY(NULL != (m_hEventJobStop = CreateEvent(NULL, TRUE, FALSE, NULL))); 
		return bRet;
	}

	template <typename T>
	BOOL CFJobBase<T>::Finalize()
	{
		BOOL bRet = TRUE;
		SAFE_CLOSE_HANDLE(m_hEventJobStop, NULL);
		return bRet;
	}

	template <typename T>
	FTLThreadWaitType CFJobBase<T>::GetJobWaitType(DWORD dwMilliseconds /* = INFINITE*/) const
	{
		FUNCTION_BLOCK_TRACE(0);
		FTLThreadWaitType waitType = ftwtError;
		HANDLE waitEvent[] = 
		{
			m_hEventJobStop,
			m_pThreadPool->m_hEventStop,
			m_pThreadPool->m_hEventContinue
		};
		DWORD dwResult = ::WaitForMultipleObjects(_countof(waitEvent),waitEvent,FALSE,dwMilliseconds);
		switch (dwResult)
		{
		case WAIT_OBJECT_0:
			waitType = ftwtStop;		//Job Stop Event
			break;
		case WAIT_OBJECT_0 + 1:
			waitType = ftwtStop;		//Job Thread Stop Event
			break;
		case WAIT_OBJECT_0 + 2:
			waitType = ftwtContinue;	//Job Thread Continue Event
			break;
		case WAIT_TIMEOUT:
			waitType = ftwtTimeOut;
			break;
		default:
			FTLASSERT(FALSE);
			waitType = ftwtError;
			break;
		}
		return waitType;
	}

	//////////////////////////////////////    CFThreadPool    ///////////////////////////////////////////////////
	template <typename T>  
	CFThreadPool<T>::CFThreadPool(IFThreadPoolCallBack<T>* pCallBack /* = NULL*/)
		:m_pCallBack(pCallBack)
	{
		FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);

		m_nMinNumThreads = 0;
		m_nMaxNumThreads = 1;

		m_nJobIndex = 0;
		m_nCurNumThreads = 0;
		m_nRunningThreadNum = 0;

		m_pJobThreadHandles = NULL;
		m_pJobThreadIds = NULL;

		m_hEventStop = CreateEvent(NULL, TRUE, FALSE, NULL);
		FTLASSERT(NULL != m_hEventStop);

		m_hEventAllThreadComplete = ::CreateEvent(NULL, TRUE, TRUE, NULL);
		FTLASSERT(NULL != m_hEventAllThreadComplete);

		m_hEventContinue = ::CreateEvent(NULL, TRUE, TRUE, NULL);
		FTLASSERT(NULL != m_hEventContinue);

		//将可以同时做的工作数设置为 MAXLONG -- 目前暂时不考虑队列中的个数
		m_hSemaphoreJobToDo = ::CreateSemaphore(NULL, 0, MAXLONG, NULL);
		FTLASSERT(NULL != m_hSemaphoreJobToDo);

		//创建调整线程个数的信号量(尝试支持动态调整线程池最小、最大个数)
		m_hSemaphoreSubtractThread = CreateSemaphore(NULL, 0, MAXLONG, NULL);
		FTLASSERT(NULL != m_hSemaphoreSubtractThread);
	}

	template <typename T>  
	CFThreadPool<T>::~CFThreadPool(void)
	{
		FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
		BOOL bRet = FALSE;
		API_VERIFY(StopAndWait(FTL_MAX_THREAD_DEADLINE_CHECK));
		DestroyPool();

		FTLASSERT(0 == m_nRunningThreadNum);  //析构时所有的线程都要结束
	}

	template <typename T>  
	BOOL CFThreadPool<T>::Start(LONG nMinNumThreads, LONG nMaxNumThreads)
	{
		FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
		FTLASSERT( 0 <= nMinNumThreads );
		FTLASSERT( nMinNumThreads <= nMaxNumThreads );       

		BOOL bRet = TRUE;
		FTLTRACE(TEXT("CFThreadPool::Start, this=%p, ThreadNum is [%d-%d]\n"), this, nMinNumThreads, nMaxNumThreads);
		m_nMinNumThreads = nMinNumThreads;
		m_nMaxNumThreads = nMaxNumThreads;

		API_VERIFY(ResetEvent(m_hEventStop));
		API_VERIFY(ResetEvent(m_hEventAllThreadComplete));
		API_VERIFY(SetEvent(m_hEventContinue));				//设置继续事件，保证各个工作线程能运行
		
		{
			CFAutoLock<CFLockObject>   locker(&m_lockThreads);
			if(NULL == m_pJobThreadHandles)    //防止多次调用Start
			{
				m_pJobThreadHandles = new HANDLE[m_nMaxNumThreads];  //分配m_nMaxNumThreads个线程的空间
				ZeroMemory(m_pJobThreadHandles,sizeof(HANDLE) * m_nMaxNumThreads);

				m_pJobThreadIds = new DWORD[m_nMaxNumThreads];
				ZeroMemory(m_pJobThreadIds,sizeof(DWORD) * m_nMaxNumThreads);

				AddJobThread(m_nMinNumThreads);		//开始时只创建 m_nMinNumThreads 个线程
				FTLASSERT(m_nCurNumThreads == m_nMinNumThreads);
			}
		}

		return bRet;
	}

	template <typename T>  
	BOOL CFThreadPool<T>::Stop()
	{
		FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
		FTLTRACE(TEXT("CFThreadPool::Stop, this=%p\n"), this);

		BOOL bRet = TRUE;
		API_VERIFY(SetEvent(m_hEventStop));
		return bRet;
	}

	template <typename T>  
	BOOL CFThreadPool<T>::StopAndWait(DWORD dwTimeOut /* = FTL_MAX_THREAD_DEADLINE_CHECK */)
	{
		FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
		BOOL bRet = TRUE;
		API_VERIFY(Stop());
		API_VERIFY(Wait(dwTimeOut));
		return bRet;
	}

	template <typename T>
	BOOL CFThreadPool<T>::Wait(DWORD dwTimeOut /* = FTL_MAX_THREAD_DEADLINE_CHECK */)
	{
		FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
		FTLTRACE(TEXT("CFThreadPool::Wait, this=%p, dwTimeOut=%d\n"), this, dwTimeOut);

		BOOL bRet = TRUE;
		DWORD dwResult = WaitForSingleObject(m_hEventAllThreadComplete, dwTimeOut);
		switch (dwResult)
		{
		case WAIT_OBJECT_0: //所有的线程都结束了
			bRet = TRUE;
			break;
		case WAIT_TIMEOUT:
			FTLTRACEEX(tlError,TEXT("!!!CFThreadPool::Wait, Not all thread over in %d millisec\n"),dwTimeOut);
			SetLastError(ERROR_TIMEOUT);
			bRet = FALSE;
			break;
		default:
			FTLASSERT(FALSE);
			bRet = FALSE;
			break;
		}

		{
			CFAutoLock<CFLockObject> locker(&m_lockThreads);
			for (LONG i = 0; i < m_nCurNumThreads; i++)
			{
				SAFE_CLOSE_HANDLE(m_pJobThreadHandles[i],NULL);
			}
			SAFE_DELETE_ARRAY(m_pJobThreadHandles);
			SAFE_DELETE_ARRAY(m_pJobThreadIds);
			m_nCurNumThreads = 0;
		}

		return bRet;
	}

	template <typename T>
	BOOL CFThreadPool<T>::Pause()
	{
		FTLTRACE(TEXT("CFThreadPool::Pause, this=%p\n"), this);
		BOOL bRet = FALSE;
		API_VERIFY(::ResetEvent(m_hEventContinue));
		return bRet;
	}

	template <typename T>
	BOOL CFThreadPool<T>::Resume()
	{
		FTLTRACE(TEXT("CFThreadPool::Resume, this=%p\n"), this);
		BOOL bRet = FALSE;
		API_VERIFY(::SetEvent(m_hEventContinue));
		return bRet;
	}

	template <typename T>  
	BOOL CFThreadPool<T>::ClearUndoWork()
	{
		FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);

		BOOL bRet = TRUE;
		{
			CFAutoLock<CFLockObject> locker(&m_lockWaitingJobs);
			FTLTRACE(TEXT("CFThreadPool::ClearUndoWork, waitingJob Number is %d\n"), m_WaitingJobs.size());
			while (!m_WaitingJobs.empty())
			{
				//释放对应的信标对象，其个数和 m_WaitingJobs 的个数是一致的
				DWORD dwResult = WaitForSingleObject(m_hSemaphoreJobToDo, FTL_MAX_THREAD_DEADLINE_CHECK); 
				API_VERIFY(dwResult == WAIT_OBJECT_0);

				JobInfoContainer::iterator iterBegin = m_WaitingJobs.begin();
				JobInfo* pInfo = iterBegin->second;
				FTLASSERT(pInfo);
				pInfo->pJob->OnCancelJob(pInfo->param);
				delete pInfo;

				m_WaitingJobs.erase(iterBegin);
			}
		}
		return bRet;
	}

	template <typename T>  
	BOOL CFThreadPool<T>::AddJobThread(LONG nThreadNum)
	{
		FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
		BOOL bRet = TRUE;
		{
			CFAutoLock<CFLockObject> locker(&m_lockThreads);
			if (m_nCurNumThreads + nThreadNum > m_nMaxNumThreads)
			{
				//超过最大个数，不能再加了
				SetLastError(ERROR_INVALID_PARAMETER);
				bRet = FALSE;
			}
			else
			{
				unsigned int threadId = 0;
				for(LONG i = 0;i < nThreadNum; i++)
				{
					FTLASSERT(NULL == m_pJobThreadHandles[m_nCurNumThreads]);
					m_pJobThreadHandles[m_nCurNumThreads] = (HANDLE) _beginthreadex( NULL, 0, JobThreadProc, this, 0, &threadId);
					FTLASSERT(NULL != m_pJobThreadHandles[m_nCurNumThreads]);

					m_pJobThreadIds[m_nCurNumThreads] = threadId;
					m_nCurNumThreads++;

					FTLTRACEEX(tlTrace,TEXT("CFThreadPool<T>::AddThread,Thread id = %d,CurNumThreads = %d\n"),
						threadId, m_nCurNumThreads);
				}
				bRet = TRUE;
			}
		}
		return bRet;
	}

	template <typename T>
	BOOL CFThreadPool<T>::OnSubmitJob(const JobInfo* pJobInfo)
	{
		UNREFERENCED_PARAMETER(pJobInfo);
		//TODO: 目前已经无效 -- 默认实现直接返回FALSE，在JobQueue的最后增加
		return FALSE;
	}

	template <typename T>  
	BOOL CFThreadPool<T>::SubmitJob(CFJobBase<T>* pJob, const T& param, LONG* pOutJobIndex)
	{
		FTLASSERT(NULL != m_hEventStop); //如果调用DestroyPool后，就不能再次调用该函数
		FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
		BOOL bRet = FALSE;
		{
			//加入Job并且唤醒一个等待线程
			{
				CFAutoLock<CFLockObject> locker(&m_lockWaitingJobs);
				m_nJobIndex++;
				pJob->m_pThreadPool = this;         //访问私有变量，并将自己赋值过去
				pJob->m_nJobIndex = m_nJobIndex;	//访问私有变量，设置JobIndex

				JobInfo *pInfo = new JobInfo;
				pInfo->pJob = pJob;
				pInfo->param = param;
				if (pOutJobIndex)
				{
					*pOutJobIndex = m_nJobIndex;
				}
				
				if (!OnSubmitJob(pInfo))
				{
					m_WaitingJobs.insert(JobInfoContainer::value_type(pJob->m_nJobIndex, pInfo));
					//m_WaitingJobs[m_nJobIndex] = pInfo;
				}
			}

			API_VERIFY(ReleaseSemaphore(m_hSemaphoreJobToDo,1L,NULL));
			//SwitchToThread();
			Sleep(1);	//唤醒等待的线程，使得其他线程可以获取Job -- 注意 CFAutoLock 的范围

			{
				//检查是否需要增加线程
				CFAutoLock<CFLockObject> locker(&m_lockWaitingJobs);
				BOOL bNeedMoreThread = (!m_WaitingJobs.empty() && (m_nCurNumThreads < m_nMaxNumThreads));
				{
					if (bNeedMoreThread)
					{
						API_VERIFY(AddJobThread(1L));      //每次增加一个线程
					}
				}
			}
		}
		return bRet;	
	}

	template <typename T>  
	BOOL CFThreadPool<T>::CancelJob(LONG nJobIndex)
	{
		BOOL bRet = TRUE;
		BOOL bFoundWaiting = FALSE;
		BOOL bFoundDoing = FALSE;
		{
			//首先查找未启动的任务
			CFAutoLock<CFLockObject> locker(&m_lockWaitingJobs);
			JobInfoContainer::iterator iterWaiting = m_WaitingJobs.find(nJobIndex);
			if (iterWaiting != m_WaitingJobs.end())
			{
				DWORD dwResult = WaitForSingleObject(m_hSemaphoreJobToDo, INFINITE); //释放对应的信标对象，避免再次找到
				API_ASSERT(dwResult == WAIT_OBJECT_0);

				//找到,说明这个Job还没有启动
				JobInfo* pInfo = iterWaiting->second;
				FTLASSERT(pInfo);
				FTLASSERT(pInfo->pJob->GetJobIndex() == nJobIndex);
				pInfo->pJob->OnCancelJob(pInfo->param);
				delete pInfo;
				m_WaitingJobs.erase(iterWaiting);

				bFoundWaiting = TRUE;
			}
		}
		if (!bFoundWaiting)
		{
			//查找正在运行的任务
			CFAutoLock<CFLockObject> locker(&m_lockDoingJobs);
			JobInfoContainer::iterator iterDoing = m_DoingJobs.find(nJobIndex);
			if (iterDoing != m_DoingJobs.end())
			{
				JobInfo* pInfo = iterDoing->second;
				FTLASSERT(pInfo);
				FTLASSERT(pInfo->pJob->GetJobIndex() == nJobIndex);
				pInfo->pJob->NotifyStop();
				//注意：这里只是请求Cancel，实际上任务是否能真正Cancel，需要依赖Job的实现，

				//不要 delete pInfo 和 m_DoingJobs.erase -- Job 结束后再取消
				////delete pInfo;
				////m_DoingJobs.erase(iterDoing);

				bFoundDoing = TRUE;
			}
		}
		if (!bFoundWaiting && !bFoundDoing)
		{
			//Waiting 和 Doing 中都没有找到，已经执行完毕
			//do nothing
		}

		return bRet; //(bFoundDoing || bFoundWaiting);
	}

	//template <typename T>  
	//BOOL CFThreadPool<T>::PauseJob(INT nJobIndex)
	//{
	//	SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
	//	return FALSE;
	//}

	//template <typename T>  
	//BOOL CFThreadPool<T>::ResumeJob(INT nJobIndex)
	//{
	//	SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
	//	return FALSE;
	//}

	template <typename T>  
	BOOL CFThreadPool<T>::HadRequestStop() const
	{
		_ASSERT(NULL != m_hEventStop);
		BOOL bRet = (WaitForSingleObject(m_hEventStop,0) == WAIT_OBJECT_0);
		return bRet;
	}

	template <typename T>
	BOOL CFThreadPool<T>::HadRequestPause() const
	{
		BOOL bRet = (WaitForSingleObject(m_hEventContinue,0) == WAIT_TIMEOUT);
		return bRet;
	}

	template <typename T>  
	void CFThreadPool<T>::DestroyPool()
	{
		FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
		BOOL bRet = FALSE;
		API_VERIFY(ClearUndoWork());
		
		SAFE_CLOSE_HANDLE(m_hSemaphoreJobToDo,NULL);
		SAFE_CLOSE_HANDLE(m_hSemaphoreSubtractThread,NULL);
		SAFE_CLOSE_HANDLE(m_hEventAllThreadComplete, NULL);
		SAFE_CLOSE_HANDLE(m_hEventContinue,NULL);
		SAFE_CLOSE_HANDLE(m_hEventStop,NULL);
	}

	template <typename T>  
	GetJobType CFThreadPool<T>::GetJob(CFJobBase<T>** ppJob, T* pParam)
	{
		FUNCTION_BLOCK_TRACE(0);
		HANDLE hWaitHandles[] = 
		{
			m_hEventStop,                 //用户选择了停止
			m_hSemaphoreJobToDo,          //还有未作的Job
			m_hSemaphoreSubtractThread,   //需要减少线程
		};

		DWORD dwResult = WaitForMultipleObjects(_countof(hWaitHandles),hWaitHandles,FALSE,INFINITE);
		switch(dwResult)
		{
		case WAIT_OBJECT_0: //Stop Event
			return typeStop;
		case WAIT_OBJECT_0 + 1: //Get Job
			break;
		case WAIT_OBJECT_0 + 2://减少线程
			return typeSubtractThread;
		default:
			FTLASSERT(FALSE);
			break;
		}
		{
			//从等待容器中获取用户作业
			CFAutoLock<CFLockObject> lockerWating(&m_lockWaitingJobs);
			FTLASSERT(!m_WaitingJobs.empty());
			JobInfoContainer::iterator iterBegin = m_WaitingJobs.begin();
			INT nJobIndex = iterBegin->first;
			JobInfo* pInfo = iterBegin->second;
			FTLASSERT(pInfo);

			*ppJob = pInfo->pJob;
			*pParam = pInfo->param;
			m_WaitingJobs.erase(iterBegin);
			{
				//放到进行作业的容器中
				CFAutoLock<CFLockObject> lockerDoing(&m_lockDoingJobs);
				m_DoingJobs.insert(JobInfoContainer::value_type(nJobIndex, pInfo));			
			}
		}
		return typeGetJob;	
	}

	template <typename T>  
	void CFThreadPool<T>::DoJobs()
	{
		BOOL bRet = FALSE;
		FUNCTION_BLOCK_TRACE(0);
		CFJobBase<T>* pJob = NULL;
		T param = T(); //需要缺省构造
		GetJobType getJobType = typeStop;
		while(typeGetJob == (getJobType = GetJob(&pJob, &param)))
		{
			INT nJobIndex = pJob->GetJobIndex();
			FTLTRACEEX(tlInfo, TEXT("CFThreadPool Begin Run Job %d\n"), nJobIndex);
			API_VERIFY(pJob->Initialize());
			pJob->Run(param); //执行Job，这个函数返回后, pJob 一般已经被 delete this 了, 不能再调用其方法

			FTLTRACEEX(tlInfo, TEXT("CFThreadPool End Run Job %d\n"), nJobIndex);

			//退出Run说明一个Job结束
			{
				//首先从运行列表中删除
				CFAutoLock<CFLockObject> lockerDoing(&m_lockDoingJobs);
				JobInfoContainer::iterator iter = m_DoingJobs.find(nJobIndex);
				FTLASSERT(m_DoingJobs.end() != iter);
				if (m_DoingJobs.end() != iter)
				{
					JobInfo* pInfo = iter->second;
					m_DoingJobs.erase(iter);
					delete pInfo;
				}
			}

			//检查一下是否需要减少线程
			BOOL bNeedSubtractThread = FALSE;
			{
				CFAutoLock<CFLockObject> locker(&m_lockWaitingJobs);
				//当队列中没有Job，并且当前线程数大于最小线程数时
				bNeedSubtractThread = (m_WaitingJobs.empty() && (m_nCurNumThreads > m_nMinNumThreads) && !HadRequestStop());
				if (bNeedSubtractThread)
				{
					//通知减少一个线程
					ReleaseSemaphore(m_hSemaphoreSubtractThread,1L,NULL);
				}
			}
		}
		if (typeSubtractThread == getJobType)  //需要减少线程,应该把自己退出 -- 注意：通知退出的线程和实际退出的线程可能不是同一个
		{
			FUNCTION_BLOCK_NAME_TRACE(TEXT("typeSubtractThread == getJobType,will remove self thread"),
				DEFAULT_BLOCK_TRACE_THRESHOLD);
			CFAutoLock<CFLockObject> locker(&m_lockThreads);
			LONG index = 0;
			DWORD dwCurrentThreadId = GetCurrentThreadId();
			for (; index < m_nCurNumThreads; index++)
			{
				if (m_pJobThreadIds[index] == dwCurrentThreadId)  //找到自己
				{
					break;
				}
			}
			FTLASSERT(index < m_nCurNumThreads);
			if (index < m_nCurNumThreads)
			{
				//把最后一个线程的信息移到退出的线程位置 -- 如果退出的线程就是最后一个时也正确
				HANDLE hOldTemp = m_pJobThreadHandles[index];
				m_pJobThreadHandles[index] = m_pJobThreadHandles[m_nCurNumThreads - 1];
				m_pJobThreadHandles[m_nCurNumThreads-1] = NULL;
				m_pJobThreadIds[index] = m_pJobThreadIds[m_nCurNumThreads - 1];
				m_pJobThreadIds[m_nCurNumThreads - 1] = 0;
				m_nCurNumThreads--;
				CloseHandle(hOldTemp);

				FTLTRACEEX(tlTrace,TEXT("CFThreadPool Subtract a thread, thread id = %d, curThreadNum = %d\n"),
					dwCurrentThreadId,m_nCurNumThreads);
			}
		}
		else //typeStop
		{
			//Do Nothing
		}
	}

	template <typename T>  
	unsigned int CFThreadPool<T>::JobThreadProc(void *pThis)
	{
		FUNCTION_BLOCK_TRACE(0);
		CFThreadPool<T>* pThreadPool = (CFThreadPool<T>*)pThis;
		LONG nRunningNumber = InterlockedIncrement(&pThreadPool->m_nRunningThreadNum);
		FTLTRACE(TEXT("ThreadId[%d] Increment, %ld\n"), GetCurrentThreadId(), nRunningNumber);

		pThreadPool->DoJobs();

		nRunningNumber = InterlockedDecrement(&pThreadPool->m_nRunningThreadNum);
		FTLTRACE(TEXT("ThreadId[%d] Decrement, %ld\n"), GetCurrentThreadId(), nRunningNumber);
		if (0 == nRunningNumber)
		{
			//线程结束后判断是否是最后一个线程，如果是，激发事件
			SetEvent(pThreadPool->m_hEventAllThreadComplete);
		}
		return(0);
	}
}

#endif //FTL_THREADPOOL_HPP