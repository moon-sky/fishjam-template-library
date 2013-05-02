///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   ftlthread.h
/// @brief  Functional Template Library Base Header File.
/// @author fujie
/// @version 0.6 
/// @date 03/30/2008
/// @defgroup ftlThreadPool ftl thread pool function and class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*************************************************************************************************************************
* 已读例子
*   C++\ATL\Advanced\CThreadPool -- 使用ATL中的 CThreadPool 的例子
*************************************************************************************************************************/

#ifndef FTL_THREADPOOL_H
#define FTL_THREADPOOL_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlThreadPool.h requires ftlbase.h to be included first
#endif

#include "ftlThread.h"
//#include "ftlSharePtr.h"
#include "ftlFunctional.h"

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
	*       QueueUserWorkItem(异步调用函数) -- 将一个工作项目(回调函数的动作)排队放入线程池中的一个线程中并且立即返回，放入后就不能取消
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
    *     不能动态改变大小、不能很好的暂停、停止 等
	* 
    * 使用方式：参见 test_CFThreadPool
    *********************************************************************************************/
	//WaitForThreadpoolWorkCallbacks -- 可以取消尚未运行的任务，但无法取消已经运行的任务
	//http://www.cnblogs.com/wz19860913/articles/1274214.html --  Windows 线程池

	//目前的线程池 SubmitJob 的实现方式可能会造成 Job 结束的时候，这个函数还没有返回的Bug ?
	//可以在回调函数(如清除调用方的管理容器) 和 调用 SubmitJob 的函数(向调用方的管理容器中加入信息) 同步即可预防这个Bug

	//http://m.blog.csdn.net/blog/ithzhang/8373243

	//TODO:
	//  1.更改最大线程个数的实现(目前采用创建最大个线程数的数组方式，无法支持很大的值)
	//  2.支持设置和调整Job最大个数;
	//  3.支持动态设置线程个数;
	//  4.支持动态调整Job优先级
	//  5.将整个Pool暂停改为支持单个Job暂停，并封装整个Pool暂停的方法;
	//  6.
	//前向声明
	//! 具有模板参数的线程池类，除能可以方便的进行参数传递外，还拥有以下特点：
	//  1.能自动根据任务和线程的多少在 最小/最大 线程个数之间调整(Vista后的系统有 SetThreadpoolThreadMaximum 等函数有类似功能)
	//  2.能方便的对任一任务进行取消，如任务尚未运行则由框架代码处理，如任务已经运行，则需要 JobBase 的子类根据 GetJobWaitType 的返回值进行处理
	//    (相比较而言，WaitForThreadpoolWorkCallbacks 函数只能取消尚未运行的任务)
	//  3.能对整个线程池进行 暂停、继续、停止 处理 -- 需要 JobBase 的子类根据 GetJobWaitType 的返回值进行处理
	//  4.支持回调方式的反馈通知( Progress/Error 等)
	//  5.使用模版方式实现，能方便的进行参数传递
	//  6.在加入任务时可以设置优先级(目前尚不支持动态调整)
	//  7.使用的是微软的基本API，能支持WinXP、Vista、Win7等各种操作系统(CreateThreadpoolWork 等只能在Vista后才能使用)
	//  8.可以有多个实例，方便进行控制(QueueUserWorkItem 等是单实例)
	//  9.采用的是对称模式的线程池(没有管理线程)，运行成本低
	template <typename T> class CFThreadPool;  

	//enum FJobStatus
	//{
	//	jsWaiting,
	//	jsDoing,
	//	jsCancel,	//不会主动设置为cancel, 如果Job子类支持Cancel的话，自行在 GetJobWaitType 调用后处理并设置
	//	jsDone,
	//	jsError,	
	//};

	//! 如果想实现多个具有不同参数类型的Job，可以将模板设为 DWORD_PTR 等可以转换为指针的类型即可
	FTLEXPORT template <typename T>
	class CFJobBase
	{
		friend class CFThreadPool<T>;   //允许Threadpool设置 m_pThreadPool/m_nJobIndex 的值
	public:
		FTLINLINE CFJobBase();
		FTLINLINE CFJobBase(T& rJobParam);
		FTLINLINE virtual ~CFJobBase();

		//! 比较Job的大小，用于确定在 Waiting 容器中的队列， 排序依据为 Priority -> Index
		bool operator < (const CFJobBase & other) const;

		//! 获取或设置Job的优先级, 数字越小，优先级越高(在等待队列中拍在越前面)，缺省值是 0
		//  注意：优先级必须在放入 Pool 前设置，放入后就不能再调整了
		FTLINLINE LONG GetJobPriority() const { return m_nJobPriority; }
		FTLINLINE LONG SetJobPriority(LONG nNewPriority);

		FTLINLINE LONG GetJobIndex() const;
		FTLINLINE DWORD GetErrorStatus() const;
		FTLINLINE LPCTSTR GetErrorInfo() const;
		//FTLINLINE FJobStatus GetJobStatus() const;

		T		m_JobParam;			//! Job会使用的参数，此处为了简化，直接采用公有变量的方式
		//FTLINLINE T& GetJobParam();
		//FTLINLINE const T& GetJobParam() const;

		//如果Job正在运行过程中被取消，会调用这个方法
		FTLINLINE BOOL RequestCancel();
	protected:
		//这个三个函数一组, 用于运行起来的Job： if( OnInitialize ){ Run -> OnFinalize }，即使Run中出现错误
		virtual BOOL OnInitialize();
		// 在这个Run中通常需要循环 调用 GetJobWaitType 方法检测
		virtual BOOL Run() = 0;
		//! 如果是new出来的，通常需要在 OnFinalize 中调用 delete this (除非又有另外的生存期管理容器)
		virtual VOID OnFinalize() = 0;

		//这个函数用于未运行的Job(直接取消或线程池停止), 用于清除内存等资源, 如 delete this 等
		FTLINLINE virtual void OnCancelJob() = 0;
	protected:
		FTLINLINE void _SetErrorStatus(DWORD dwErrorStatus, LPCTSTR pszErrorInfo);
		FTLINLINE void _NotifyProgress(LONGLONG nCurPos, LONGLONG nTotalSize);
		FTLINLINE void _NotifyError();
		FTLINLINE void _NotifyError(DWORD dwError, LPCTSTR pszDescription);
		FTLINLINE void _NotifyCancel();

		//! 通过该函数，获取线程池的状态(Stop/Pause)，以及Job自己的Stop, 用法同 CFThread:GetThreadWaitType:
		//! 如果想支持暂停，参数是 INFINITE；如不想支持暂停(如网络传输)，则参数传 0
		FTLINLINE FTLThreadWaitType GetJobWaitType(DWORD dwMilliseconds = INFINITE) const;
	private:
		//设置为私有的变量和方法，即使是子类也不要直接更改，由Pool调用进行控制
		LONG		m_nJobPriority;
		LONG		m_nJobIndex;
		DWORD				m_dwErrorStatus;	//GetLastError
		CFStringFormater	m_strFormatErrorInfo;		
		HANDLE		m_hEventJobStop;					//停止Job的事件，该变量将由Pool创建和释放(TODO:Pool中缓存?)
		//FJobStatus	m_JobStatus;
		CFThreadPool<T>* m_pThreadPool;
	};

	typedef enum tagGetJobType
	{
		typeStop,
		typeSubtractThread,
		typeGetJob,
		typeError,		//发生未知错误(目前尚不清楚什么情况下会发生)
	}GetJobType;

	//回调函数 -- 通过 pJob->m_JobParam 可以访问类型为 T 的 参数
	FTLEXPORT template <typename T>
	class IFThreadPoolCallBack
	{
	public:
		//当Job运行起来以后，会由 Pool 激发 Begin 和 End 两个函数
		FTLINLINE virtual void OnJobBegin(LONG nJobIndex, CFJobBase<T>* pJob )
		{
			UNREFERENCED_PARAMETER(nJobIndex);
			UNREFERENCED_PARAMETER(pJob);
			FTLTRACEEX(FTL::tlInfo, TEXT("IFThreadPoolCallBack[0x%x]::OnJobBegin[%d]\n"), this, nJobIndex);
		} 
		FTLINLINE virtual void OnJobEnd(LONG nJobIndex, CFJobBase<T>* pJob)
		{
			UNREFERENCED_PARAMETER(nJobIndex);
			UNREFERENCED_PARAMETER(pJob);
			FTLTRACEEX(FTL::tlInfo, TEXT("IFThreadPoolCallBack[0x%x]::OnJobEnd[%d]\n"), this, nJobIndex);
		}

		//如果尚未到达运行状态就被取消的Job，会由Pool调用这个函数
		FTLINLINE virtual void OnJobCancel(LONG nJobIndex, CFJobBase<T>* pJob)
		{
			UNREFERENCED_PARAMETER(nJobIndex);
			UNREFERENCED_PARAMETER(pJob);
			FTLTRACEEX(FTL::tlInfo, TEXT("IFThreadPoolCallBack[0x%x]::OnJobCancel[%d]\n"), this, nJobIndex);
		}

		//Progress 和 Error 由 JobBase 的子类激发
		FTLINLINE virtual void OnJobProgress(LONG nJobIndex , CFJobBase<T>* pJob, LONGLONG nCurPos, LONGLONG nTotalSize)
		{
			UNREFERENCED_PARAMETER(nJobIndex);
			UNREFERENCED_PARAMETER(pJob);
			UNREFERENCED_PARAMETER(nCurPos);
			UNREFERENCED_PARAMETER(nTotalSize);
		}
		FTLINLINE virtual void OnJobError(LONG nJobIndex , CFJobBase<T>* pJob, DWORD dwError, LPCTSTR pszDescription)
		{
			UNREFERENCED_PARAMETER(nJobIndex);
			UNREFERENCED_PARAMETER(pJob);
			UNREFERENCED_PARAMETER(dwError);
			UNREFERENCED_PARAMETER(pszDescription);
			FTLTRACEEX(FTL::tlError, TEXT("IFThreadPoolCallBack[0x%x]::OnJobError[%d], dwError=%d, pszDesc=%s\n"), 
				this, nJobIndex, dwError, pszDescription);
		}
	};

	FTLEXPORT template <typename T>  
	class CFThreadPool
	{
		//typedef CFSharePtr<CFJobBase< T> > CFJobBasePtr;
		//friend class CFJobBasePtr;
		friend class CFJobBase<T>;  //允许Job在 GetJobWaitType 中获取 m_hEventStop/m_hEventContinue
		DISABLE_COPY_AND_ASSIGNMENT(CFThreadPool);
	public:
		FTLINLINE CFThreadPool(IFThreadPoolCallBack<T>* pCallBack = NULL, LONG nMaxWaitingJobs = LONG_MAX);
		FTLINLINE virtual ~CFThreadPool(void);

		//! 开始线程池,此时会创建 nMinNumThreads 个线程，然后会根据任务数在 nMinNumThreads -- nMaxNumThreads 之间自行调节线程的个数
		FTLINLINE BOOL Start(LONG nMinNumThreads, LONG nMaxNumThreads);

		//! 请求停止线程池
		//! 注意：
		//!   1.只是设置StopEvent，需要Job根据GetJobWaitType处理 
		//!   2.不会清除当前注册的但尚未进行的工作，如果需要删除，需要调用ClearUndoWork
		FTLINLINE BOOL Stop();

		FTLINLINE BOOL StopAndWait(DWORD dwTimeOut = FTL_MAX_THREAD_DEADLINE_CHECK);

		//! 等待所有线程都结束并释放Start中分配的线程资源
		FTLINLINE BOOL Wait(DWORD dwTimeOut = FTL_MAX_THREAD_DEADLINE_CHECK);

		//! 清除当前未完成的工作，
		FTLINLINE BOOL ClearUndoWork();

		//! 向线程池中注册工作 -- 如果当前没有空闲的线程，并且当前线程数小于最大线程数，则会自动创建新的线程，
		//! 成功后会通过 outJobIndex 返回Job的索引号，可通过该索引定位、取消特定的Job
		FTLINLINE BOOL SubmitJob(CFJobBase<T>* pJob, LONG* pOutJobIndex, DWORD dwMilliseconds = INFINITE);

		//! 取消指定的Job,
		//! TODO:如果取出Job给客户，可能调用者得到指针时，Job执行完毕 delete this，会照成野指针异常
		FTLINLINE BOOL CancelJob(LONG nJobIndex);

		//FTLINLINE BOOL PauseJob(LONG nJobIndex);
		//FTLINLINE BOOL ResumeJob(LONG nJobIndex);

		//! 请求暂停线程池的操作
		FTLINLINE BOOL Pause();

		//! 请求继续线程池的操作
		FTLINLINE BOOL Resume();

		//! 是否已经请求了暂停线程池
		FTLINLINE BOOL HadRequestPause() const;

		//! 是否已经请求了停止线程池
		FTLINLINE BOOL HadRequestStop() const;
	protected:
		//! 增加运行的线程,如果 当前线程数 + nThreadNum <= m_nMaxNumThreads 时 会成功执行
		FTLINLINE BOOL _AddJobThread(LONG nThreadNum);
		FTLINLINE void _DestroyPool();
		FTLINLINE void _DoJobs();

		FTLINLINE GetJobType _GetJob(CFJobBase<T>** ppJob);

		FTLINLINE void _NotifyJobBegin(CFJobBase<T>* pJob);
		FTLINLINE void _NotifyJobEnd(CFJobBase<T>* pJob);
		FTLINLINE void _NotifyJobCancel(CFJobBase<T>* pJob);

		FTLINLINE void _NotifyJobProgress(CFJobBase<T>* pJob, LONGLONG nCurPos, LONGLONG nTotalSize);
		FTLINLINE void _NotifyJobError(CFJobBase<T>* pJob, DWORD dwError, LPCTSTR pszDescription); 

	protected:
		LONG m_nMinNumThreads;					//! 线程池中最少的线程个数
		LONG m_nMaxNumThreads;					//! 线程池中最大的线程个数
        LONG m_nMaxWaitingJobs;                 //! 等待队列中的最大线程个数
		IFThreadPoolCallBack<T>* m_pCallBack;	//! 回调接口
		LONG m_nJobIndex;						//! Job的索引，每 SubmitJob 一次，则递增1

		LONG m_nRunningJobNumber;				//! 当前正在运行的Job个数
		LONG m_nRunningThreadNum;				//! 当前运行着的线程个数(用来在所有的线程结束时激发 Complete 事件)

		typedef std::map<DWORD, HANDLE>   TaskThreadContrainer;
		TaskThreadContrainer m_TaskThreads;
		//TODO: 调整成 list，这样可以方便动态调整最大、最小线程的个数 ?
		//HANDLE* m_pJobThreadHandles;            //! 保存线程句柄的数组
		//DWORD*  m_pJobThreadIds;                //! 保存线程 Id 的数组(为了在线程结束后调整数组中的位置)

		//HANDLE	m_hMgrThread;					//! Pool管理线程的句柄

		//! 保存等待Job的信息，由于有优先级的问题，而且一般是从最前面开始取，因此保存成 set
		typedef typename UnreferenceLess< CFJobBase<T> * >	JobBaseUnreferenceLess;
		typedef std::set<CFJobBase<T>*, JobBaseUnreferenceLess > WaitingJobContainer;
		WaitingJobContainer		m_WaitingJobs;	//! 等待运行的Job

		//! 保存运行Job的信息， 由于会频繁加入、删除，且需要按照JobIndex查找，因此保存成 map
		typedef std::map<LONG, CFJobBase<T>* >	DoingJobContainer;
		DoingJobContainer		m_DoingJobs;	//! 正在运行的Job

		HANDLE m_hEventStop;                    //! 停止Pool的事件
		HANDLE m_hEventAllThreadComplete;		//! 所有的线程都结束时激发这个事件
		HANDLE m_hEventContinue;				//! 整个Pool继续运行的事件
        HANDLE m_hSemaphoreWaitingPos;          //! 保存等待容器中还可以放的Job个数，每取出一个Job就增加1，每Submit一个进去就减1
		HANDLE m_hSemaphoreJobToDo;             //! 保存还有多少个Job的信号量,每Submit一个Job,就增加一个
		HANDLE m_hSemaphoreSubtractThread;      //! 用于减少线程个数时的信号量,初始时个数为0,每要释放一个，就增加一个，

		CFCriticalSection m_lockDoingJobs;		//访问 m_DoingJobs 时互斥
		CFCriticalSection m_lockWaitingJobs;    //访问 m_WaitingJobs 时互斥
		CFCriticalSection m_lockThreads;        //访问 m_pJobThreadHandles/m_pJobThreadIds 时互斥

		static unsigned int CALLBACK JobThreadProc(void *pParam);    //! 工作线程的执行函数
		//static unsigned int CALLBACK MgrThreadProc(void* pParam);	 //! 管理线程的执行函数
	};
}

#endif //FTL_THREADPOOL_H

#ifndef USE_EXPORT
#  include "ftlThreadPool.hpp"
#endif