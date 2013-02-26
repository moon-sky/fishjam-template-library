///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   ftlthread.h
/// @brief  Functional Template Library Base Header File.
/// @author fujie
/// @version 0.6 
/// @date 03/30/2008
/// @defgroup ftlthread ftl thread function and class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef FTL_THREADPOOL_H
#define FTL_THREADPOOL_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlThreadPool.h requires ftlbase.h to be included first
#endif

#include "ftlThread.h"

namespace FTL
{
	/*********************************************************************************************
    * 线程池 -- 基于预创建技术，将线程创建和销毁本身所带来的开销分摊到了各个具体的任务上，
    *           执行次数越多，每个任务所分担到的线程本身开销则越小
    *   适用范围：
    *     1.单位时间内处理任务频繁而且任务处理时间短 -- 如不适合 FTP 服务器；
    *     2.对实时性要求较高。如果接受到任务后在创建线程，可能满足不了实时要求，因此必须采用线程池进行预创建。 
    *     注意：使用线程池所产生的开销较大,相当多的线程和内部数据结构变成了你的进程的一个组成部分,不要盲目使用
	*
	* 微软提供的线程池(ms-help://MS.MSDNQTR.v90.en/dllproc/base/thread_pools.htm) -- Vista之后才有?
	*	线程池中有四个组件，每个组件由一组线程组成
	*     1. 非 I/O 组件 ( WT_EXECUTEDEFAULT )，由于线程与完成端口相关联，因此可以同时运行的线程数量限制为 CPU 数量的两倍。
	*        非 I/O 组件中的线程是根据工作量来创建或撤消的。
	*     2. I/O 组件( WT_EXECUTEINIOTHREAD ) －－ 运行发出异步I/O请求的代码
	*        CreateThreadpoolIo -> StartThreadpoolIo -> 
	*     3. 定时器组件 ( WT_EXECUTEINTIMERTHREAD ) -- 使定时器组件的线程能够执行该工作项目,注意:代码应该迅速执行，不应该中断
	*     4. 等待组件 (WT_EXECUTEINWAITTHREAD) -- 可以使等待组件的线程之一运行工作项目函数本身，速率更高(工作项目不必排队放入 I/O 组件中)，
	*      但有一定危险(正在执行工作项目的等待组件函数的线程无法等待其他对象得到通知)
	* 
	*   能够执行下列操作
	*     a.异步调用函数(服务器启动新线程服务每一个客户)
	*       QueueUserWorkItem(异步调用函数) -- 将一个工作项目(回调函数的动作)排队放入线程池中的一个线程中并且立即返回，
	*         线程池中的某个线程将处理该工作项目，导致函数被调用。线程执行完后并不立即被撤消，而是返回线程池。
	*         不必为每个客户机请求创建和撤消线程。内部使用 I/O 完成端口
	*     b.按照规定的时间间隔调用函数 -- 等待定时器内核对象,方便地获得基于时间的通知.线程池的定时器组件创建等待定时器,给APC项目排队
	*       CreateTimerQueue -- 创建定时器队列,对一组定时器进行组织安排
	*       CreateTimerQueueTimer -- 在定时器队列中创建定时器,可以指定采用系统默认的定时器队列(NULL)
	*       DeleteTimerQueueTimer/DeleteTimerQueue -- 不使用时需要删除 定时器/定时器队列 ,注意:不要在工作项目线程中删除,否则可能造成死锁
	*       ChangeTimerQueueTimer -- 改变定时器到期时间和到期周期,可用于在单步定时器项目结束时,更改时间
	*         可用于倒计时关闭由 MessageBox 创建的对话框 --WM_TIMER 也可以,关键是显示信息的 ID 为0x0000FFFF(Spy++)
	*     c.当单个内核对象变为已通知状态时调用函数 -- 传送给等待组件(当工作项目准备执行时，它被默认排队放入非 I/O 组件的线程中)
	*       RegisterWaitForSingleObject -- 内部使用 WaitForMultipleObjects 来等待已经注册的对象。
	*         限制：1.不能多次等待单个句柄(DuplicateHandle)
	*               2.每隔 63 个对象后，就要将另一个线程添加给该组件
	*       UnregisterWait -- 取消注册状态.在取消等待组件的注册状态之前，不要关闭内核对象的句柄，会导致整个等待组件将无法正常工作
	*     d.当异步 I/O 请求完成时调用函数
	*       服务器应用程序发出某些异步 I/O 请求，当这些请求完成时，需要让一个线程池准备好来处理已完成的I/O请求(打开设备，将它与线程池的非I/O组件关联起来)。
	*       BindIoCompletionCallback -- 关联异步IO句柄和线程池中的函数指针，在后续的 ReadFileEx/WriteFileEx 等函数完成时，调用指定的函数。
	*         该函数在内部调用CreateIoCompletionPort，可以保证至少有一个线程始终在非I/O组件中.
	*         如果要将更多的上下文信息放入回调函数，应该使用将上下文信息放入 OVERLAPPED 结构的结尾处的传统方法(即以 OVERLAPPED 为第一个成员变量定义新的结构体)
	*         注意：关闭设备会导致它的所有待处理的I/O请求立即完成，并产生一个错误代码。在回调函数中注意处理这种情况。
	*   参数说明
	*     WT_EXECUTEINPERSISTENTTHREAD  －－ 要求调用线程不能终止运行,使用永久线程池的线程，应该保证工作项目迅速执行(放到定时器组件线程中执行)
	*     WT_EXECUTELONGFUNCTION －－ 工作项目需要花费很长的时间来运行，帮助线程池决定是否要将新线程添加给线程池。
	*     WT_EXECUTEONLYONCE －－工作项目只执行一次就停止
	*     WT_TRANSFER_IMPERSONATION
	* 
	*
    * ATL7开始提供基于完成端口的线程池(CThreadPool)模版类
    * Sample：http://msdn.microsoft.com/en-us/library/39f8fee2(VS.80).aspx
	*   可能的实现：
	*     创建一个 I/O 完成端口，并创建一个等待该端口的线程池，打开多个 I/O 设备，将它们的句柄与完成端口关联起来。
	*     当异步 I/O 请求完成时，设备驱动程序就将“工作项目”排队列入该完成端口。
    *   缺点：
    *     要求调用的请求函数参数是相同的--在它的生存期它只能安排一模一样的请求任务(不如 QueueUserWorkItem 灵活)
    *     
	* 
    * 使用方式：参见 test_CFThreadPool
    *********************************************************************************************/
#pragma TODO(CreateThreadpoolIo)
	//目前的线程池 SubmitJob 的实现方式可能会照成 Job 结束的时候，这个函数还没有返回的Bug ?
	//http://m.blog.csdn.net/blog/ithzhang/8373243

	template <typename T> class CFThreadPool;  //前向声明
	//! 线程池类 -- 具有模板参数，通常可以方便参数传递，
	//! 如果想实现多个具有不同参数类型的Job，可以将模板设为 DWORD_PTR 等可以转换为指针的类型即可
	FTLEXPORT template <typename T>
	class CFJobBase
	{
		friend class CFThreadPool<T>;   //允许Threadpool设置 m_pThreadPool 的值
	public:
		CFJobBase();
		virtual ~CFJobBase();
		INT	 GetJobIndex() const;
		//如果Job正在运行过程中被取消，会调用这个方法
		BOOL NotifyStop();

		//! 如果是new出来的，一定要在结束时调用 delete this（包括参数 pParam） -- 是否可以增加 m_bAutoDelete？
		// 在这个Run中通常需要循环 调用 GetJobWaitType 方法检测
		virtual void Run(T param) = 0;

		//如果线程池停止了，剩下的Job将不会执行，会调用该函数，用于清除内存等资源, 如 delete param/this 等
		virtual void OnCancelJob(T param);
	protected:
		//BOOL m_bCancel;
		//! 通过该函数，获取线程池的状态(Stop/Pause)，以及Job自己的Stop, 用法同 CFThread:GetThreadWaitType:
		FTLINLINE FTLThreadWaitType GetJobWaitType(DWORD dwMilliseconds = INFINITE) const;
	private:
		//设置为私有的变量和方法，即使是子类也不要直接更改，由Pool调用进行控制
		INT m_nJobIndex;
		HANDLE m_hEventJobStop;
		CFThreadPool<T>* m_pThreadPool;
		BOOL Initialize();
		BOOL Finalize();
	};


	typedef enum tagGetJobType
	{
		typeStop,
		typeSubtractThread,
		typeGetJob,
		typeError,		//发生未知错误(目前尚不清楚什么情况下会发生)
	}GetJobType;

	//觉得使用指针似乎更好？使用模板的话，保存时应该使用引用还是指针？或直接用值？ -- 用值，不要过分优化
	FTLEXPORT template <typename T>  
	class CFThreadPool
	{
		//typedef CFSharePtr<CFJobBase< T> > CFJobBasePtr;
		//friend class CFJobBasePtr;
		friend class CFJobBase<T>;  //允许Job获取StopEventHandle -- 两边互为friend，是否合适？
	public:
		struct JobInfo
		{
			JobInfo()
			{
				nJobPriority = 0;
				nJobIndex = 0;
				pJob = NULL;
				param = T();
			}
			bool operator < (const JobInfo & other) const
			{
				COMPARE_MEM_LESS(nJobPriority, other);
				COMPARE_MEM_LESS(nJobIndex, other);
				return true;
			}

			//容器改成map后，优先级已经不起作用了
			INT nJobPriority;		//优先级，值越小，优先级越高，越早被调用，默认是0，可在 OnSubmitJob 中调整(尚不支持动态调整)
			INT nJobIndex;
			CFJobBase<T> *pJob;
			//CFJobBasePtr *pJob;
			T param;
		};
		//! 会在 nMinNumThreads -- nMaxNumThreads 之间自行调节线程的个数
		//! 注意：nMaxNumThreads 必须小于 MAXIMUM_WAIT_OBJECTS
		FTLINLINE CFThreadPool(LONG nMinNumThreads,LONG nMaxNumThreads);
		FTLINLINE virtual ~CFThreadPool(void);

		//! 开始线程池,此时会创建 nMinNumThreads 个线程
		FTLINLINE BOOL Start();

		//! 停止线程池 -- 每一个线程结束时会调用 OnSubtractThread
		//! 注意：不会删除当前注册的但尚未进行的工作，如果需要删除，需要调用ClearUndoWork
		FTLINLINE BOOL Stop();

		FTLINLINE BOOL StopAndWait(DWORD dwTimeOut = FTL_MAX_THREAD_DEADLINE_CHECK ,
			BOOL bCloseHandle = TRUE );

		//! 等待所有线程都结束
		FTLINLINE BOOL Wait(DWORD dwTimeOut,BOOL bCloseHandle);

		//! 清除当前未完成的工作，
		FTLINLINE BOOL ClearUndoWork();

		//! 向线程池中注册工作 -- 如果当前没有空闲的线程，并且当前线程数小于最大线程数，则会自动创建新的线程，
		//! 每一个线程创建是会调用 OnAddThread
		//! 成功后会通过 outJobIndex 返回Job的索引号，可通过该索引定位、取消特定的Job
		FTLINLINE BOOL SubmitJob(CFJobBase<T>* pJob,const T& param, INT* pOutJobIndex);
		//FTLINLINE BOOL SubmitJob(void (*pJobProc)(T& param), T& param, INT* pOutJobIndex);

		//取消指定的Job, TODO:如果取出Job给客户，可能调用者得到指针时，Job执行完毕 delete this，会照成野指针异常
		FTLINLINE BOOL CancelJob(INT nJobIndex);
		//FTLINLINE BOOL PauseJob(INT nJobIndex);
		//FTLINLINE BOOL ResumeJob(INT nJobIndex);

		//! 获取当前运行着的线程个数 -- 是否需要增加获取 最小、最大线程的函数？不需要
		FTLINLINE LONG GetActiveThreadCount() const;

		//! 请求暂停线程池的操作
		FTLINLINE BOOL Pause();
		//! 请求继续线程池的操作
		FTLINLINE BOOL Resume();
		//! 是否已经请求了暂停
		FTLINLINE BOOL HadRequestPause() const;
		FTLINLINE BOOL HadRequestStop() const;
	protected:
		virtual BOOL OnAddThread();
		virtual BOOL OnSubtractThread();
		//! 在注册Job时调用，如果要实现带优先级的Job，可以重载该函数，将Job加入适当的位置，然后返回TRUE
		virtual BOOL OnSubmitJob(const JobInfo* pJobInfo);
	protected:
		//! 增加运行的线程,如果 当前线程数 + nThreadNum <= m_nMaxNumThreads 时 会成功执行
		FTLINLINE BOOL AddThread(LONG nThreadNum);
		FTLINLINE void DestroyPool();
		FTLINLINE GetJobType GetJob(CFJobBase<T>** ppJob,T* pParam);
		FTLINLINE void DoJobs();
		//FTLINLINE BOOL InternalStop(DWORD dwMilliseconds);
		//FTLINLINE BOOL InternalSubtractThread(LONG nThreadNum,DWORD dwMilliseconds);
	protected:
		const LONG m_nMinNumThreads;            //! 线程池中最少的线程个数
		const LONG m_nMaxNumThreads;            //! 线程池中最大的线程个数

		//TODO: 两个最好统一？
		LONG m_nCurNumThreads;                  //! 当前的线程个数(主要用来维护 m_pThreadHandles 数组)
		LONG m_nRunningThreadNum;				//! 当前运行着的线程个数(用来在所有的线程结束时激发 Complete 事件)

		//LONG m_nCanSubtractThreadNum;
		HANDLE* m_pThreadHandles;               //! 保存线程句柄的数组(为了方便Wait)
		DWORD*  m_pThreadIds;                   //! 保存线程 Id 的数组(为了在线程结束后调整数组中的位置)
		//! 保存Job的信息,由于会频繁加入、删除，且需要按照JobIndex查找，因此保存成 map/set/queue ?
		typedef std::map<INT, JobInfo* >	JobInfoContainer;
		JobInfoContainer		  m_WaitingJobs;//! 等待运行的Job
		JobInfoContainer		  m_DoingJobs;  //! 正在运行的Job
		//std::queue<JobInfo*>    m_JobsQueue;    
		HANDLE m_hEventStop;                    
		HANDLE m_hEventContinue;
		HANDLE m_hEventAllThreadComplete;		//! 所有的线程都结束时激发这个事件
		HANDLE m_hSemaphoreJobToDo;             //! 保存还有多少个Job的信号量，每Submit一个Job，就增加一个
		HANDLE m_hSemaphoreSubtractThread;      //! 用于减少线程个数时的信标,初始时个数为0，每要释放一个，就增加一个，
		//! 最大个数为 m_nMaxNumThreads - m_nMinNumThreads
		INT               m_nJobIndex;
		CFCriticalSection m_lockDoingJobs;		//访问 m_DoingJobs 时互斥
		CFCriticalSection m_lockWaitingJobs;    //访问 m_WaitingJobs 时互斥
		CFCriticalSection m_lockThreads;        //访问 m_pThreadHandles/m_pThreadIds 时互斥

		static unsigned int CALLBACK JobThreadExecute(void *pThis);    //线程池中线程的执行函数
		//static unsigned int CALLBACK StopAsyncProc(void *pAsyncProxyParam);
		//static unsigned int CALLBACK SubtractThreadAsyncProc(void * pAsyncProxyParam);
	};

}

#endif //FTL_THREADPOOL_H

#ifndef USE_EXPORT
#  include "ftlThreadPool.hpp"
#endif