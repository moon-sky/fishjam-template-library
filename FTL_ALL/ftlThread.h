///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   ftlthread.h
/// @brief  Functional Template Library Base Header File.
/// @author fujie
/// @version 0.6 
/// @date 03/30/2008
/// @defgroup ftlthread ftl thread function and class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef FTL_THREAD_H
#define FTL_THREAD_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlthread.h requires ftlbase.h to be included first
#endif

//#ifdef _DEBUG  //__AFX_H__ , _MFC_VER
//#  define new DEBUG_NEW
//#endif

#if defined(_MT)
#  include <process.h>    //for _beginthreadex
#endif

#include <queue>
#include <vector>
#include <map>
#include <list>
#include <algorithm>

/******************************************************************************************************
* Intel Hyper-Threading Technology -- Intel的超线程技术
* 
* APC(Asynchronous Procedure Call) -- 异步过程调用。APC只有当程序处于 "Aalertable态下时，才会被调用。
*   目前有五个函数：SleepEx、WaitForXXXEx(,,TRUE)、SignalObjectAndWait等，函数会返回 WAIT_IO_COMPLETION。
*   此时OVERLAPPED中的hEvent不必放置EventHandle，而是自由使用。
*
* 临界区(Critial Section)--对临界资源实施操作的程序段。对互斥临界区管理的原则是：
*   有空则进，无空则等，有限等待，让权等待(当不能进入临界区时，应立即释放CPU，以免陷入忙状态)
*   更精细的锁机制：读多写少锁(RCU)，读写平均锁(RW)，读少写多锁(SEQ)，
*     短时间的锁可考虑用spin_lock锁进行忙等。
*     注意：1.spin_lock的使用要求很高，上锁后不允许调用任何引起调度的函数；
*           2.不是 pthread_spin_lock - 该方法只是模拟
* 
*   互斥区问题的解决思路只有一个 -- 减少互斥区
*   一般会把加锁的机制分为两种：
*     业务逻辑锁
*     数据锁
* 
* 信号量：S>=0表示某资源的可用数，S<0时其绝对值表示阻塞队列中等待该资源的进程(线程)数。
*   原子操作PV:P操作表示申请一个资源，S=S-1; if(S<0){Wait(S);加入阻塞队列}
*              V操作表示释放一个资源，S=S+1; if(S<=0){Resume(S);从阻塞队列总唤醒一个}
*
* 产生死锁的4个必要条件：
*   1.互斥条件：线程对其所要求的资源进行排他性控制，即一次只允许一个线程使用；
*   2.请求保持条件：零星地请求支援，即已获得部分资源又请求资源被阻塞；
*   3.不可剥夺条件：线程已获得的资源在未使用完之前，不能被剥夺，只能在使用完时由自己释放；
*   4.环路条件：当发生死锁时，在线程资源有向图中必构成环路，每个线程占有了下一个线程申请的一个或多个资源。
*
* 核心对象的名称(是否正确？)：
*   \\Global\
*   \\Local\
*
* 核心对象的安全属性(LPSECURITY_ATTRIBUTES)
*   NULL -- 默认属性，如果是Admin，则其他用户无法打开
*   
* 不要在线程之间共享GDI对象 -- 为什么？通常应该只有主线程才操作GDI对象
* MFC各对象和Win32 handle之间的映射关系记录在线程局部存储中，因此没有办法在线程间传递MFC对象或对象指针，
*   替代方案是传递对象的Handle(通过GetSafeXXX获取)，目的线程可以利用FromHandle产生一个临时对象，
*   使用Attach附着到一个永久对象(退出之前应Detach)，

* TerminateThread 的危害
*   1.被结束线程没有机会在结束前进行清理，堆栈不会被释放(内存泄露)；
*   2.相关DLL不会获得 Thread Detach 的通知；
*   3.如果被结束线程正在CS中，则该CS将永远处于锁定状态；Mutex将会被Abandoned
* 
* 如果在一个worker线程中调用GetMessage，该线程就会产生消息队列，纵然它并没有窗口，
*   这时就可以调用PostThreadMessage给该worker线程发送消息。
*
* TLS -- 线程局部存储
*   C runtime library把errno和strtok指针放在TLS中
*   MFC通过TLS来追踪每一个线程所使用的GDI对象和USER对象
*   _declspec(thread) DWORD 变量名 -- 该变量对每一个线程是独一无二的，在可执行文件中生成一个特殊的节区，内含所有的线程局部变量。
*   但一个DLL如果使用了_declspec(thread)，就没有办法被LoadLibrary载入(KB118816)
*
* 线程调试：
*   1.多线程环境下，如果没有做好同步，如一个线程正在析构对象，但另外一个线程调用了该对象的重载函数，可能产生纯虚函数调用的错误
*   2.线程死锁时用 ProcessExplorer 能很容易地定位死锁的位置和原因。
*   3.使用Window Object Explorer(WinObjEx)查看对象，可了解具体的使用状态
*   
* 性能调优：
*   1.无关系的数据不要为了方便使用同一把锁
*   2.并不是说线程越多切换就越多 -- 应该降低系统切换频率，有线程“没有用完自己的时间片”就发生切换时消耗很大，如等待IO、互斥等待、短时互斥等。
*   3.软件的负反馈机制(如流水线？)的基本处理原则是：必须反馈到第一级，否则会造成整个系统的抖动
*   4.进行数据散列，可根据入口的数据直接散列岛不同的数据结构上，使同时访问这个数据的机会表小(glibc把内存分为多个区，每个区用单独的锁)
*     linux 下有个 PER_CPU 机制，可以为每个CPU创建一份单独的数据
*   5.无锁算法 -- 依赖于指针赋值得原子性，其本质是数据散列
*   6.
******************************************************************************************************/

namespace FTL
{
    //自动同步锁的基类
    class CFLockObject
    {
    public:
        virtual BOOL Lock(DWORD dwTimeout = INFINITE) = 0;
        virtual BOOL UnLock() = 0;
    };

    FTLEXPORT class CFNullLockObject : public CFLockObject
    {
    public:
        FTLINLINE virtual BOOL Lock(DWORD dwTimeout = INFINITE);
        FTLINLINE virtual BOOL UnLock();
    };

#ifdef FTL_DEBUG
#  define CRITICAL_SECTION_LOCK_TRACE(pCS, bTrace)  { pCS->SetTrace(bTrace);}
#else
#  define CRITICAL_SECTION_LOCK_TRACE(pCS, bTrace)
#endif

    //! class CFCriticalSection
    //! 用于线程同步的同步对象(临界区)
    FTLEXPORT class CFCriticalSection : public CFLockObject
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFCriticalSection);
    public:
        FTLINLINE CFCriticalSection();
        FTLINLINE ~CFCriticalSection();

        //一旦一个线程进入一个CS，它就能够一再地重复进入该CS,但需要保证 对应的UnLock
        FTLINLINE BOOL Lock(DWORD dwTimeout = INFINITE);
        FTLINLINE BOOL UnLock();
        FTLINLINE BOOL TryLock();
#ifdef FTL_DEBUG
        FTLINLINE BOOL IsLocked() const;
        FTLINLINE BOOL SetTrace(BOOL bTrace);
#endif
    private:
        CRITICAL_SECTION m_CritSec;
#ifdef FTL_DEBUG
        DWORD   m_currentOwner;     //指明是哪个线程锁定了该临界区
        BOOL    m_fTrace;           //指明是否要打印日志信息
        DWORD   m_lockCount;        //用于跟踪线程进入关键代码段的次数,可以用于调试
#endif //FTL_DEBUG
    };

    //Mutex -- 锁住一个未被拥有的Mutex，比锁住一个未被拥有的CS，需要花费几乎100倍的时间(实测Xp时是30倍左右)
    //   CS不需要进入系统内核，直接在ring3级就可以进行操作
    
    FTLEXPORT class CFMutex : public CFLockObject
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFMutex);
    public:
        FTLINLINE CFMutex(BOOL bInitialOwner = FALSE, LPCTSTR lpName = NULL,
            LPSECURITY_ATTRIBUTES lpMutexAttributes = NULL);
        FTLINLINE ~CFMutex();
        FTLINLINE BOOL UnLock();

        //拥有mutex的线程不论再调用多少次wait函数，也不会被阻塞住
        FTLINLINE BOOL Lock(DWORD dwTimeout = INFINITE);
        FTLINLINE operator const HANDLE() const { return m_hMutex;}
    protected:
        HANDLE  m_hMutex;
#ifdef FTL_DEBUG
        DWORD   m_currentOwner;     //指明是哪个线程锁定了该互斥量
        DWORD   m_lockCount;        //用于跟踪线程进入关键代码段的次数,可以用于调试
#endif //FTL_DEBUG
    };
    
    //! 事件对象
    FTLEXPORT class CFEvent : public CFLockObject
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFEvent);
    public:
        FTLINLINE CFEvent(BOOL bInitialState = TRUE, BOOL bManualReset = TRUE,
            LPCTSTR pszName = NULL, LPSECURITY_ATTRIBUTES lpsaAttribute = NULL);
        FTLINLINE ~CFEvent();
        FTLINLINE operator const HANDLE () const { return m_hEvent; }

        FTLINLINE BOOL Wait(DWORD dwTimeout = INFINITE);
        //允许在等待时处理 SEND 发送的消息(从DirectShow中拷贝的代码) --
        //http://support.microsoft.com/kb/136885/en-us
        FTLINLINE BOOL WaitMsg(DWORD dwTimeout = INFINITE);

        FTLINLINE BOOL Reset();
        FTLINLINE BOOL UnLock();
        FTLINLINE BOOL Lock(DWORD dwTimeout = INFINITE);
    private:
        HANDLE m_hEvent;
    };

    //! 自动锁定同步对象
    FTLEXPORT template<typename T = CFLockObject>
    class CFAutoLock
    {
    public:
        explicit FTLINLINE CFAutoLock<T>(T* pLockObj);
        FTLINLINE ~CFAutoLock();
    private:
        T*   m_pLockObj;
    };

    //一个将非线程安全的类包装成线程安全的类 -- 具体使用方法见 test_threadSafeWrapper
    //  利用了 operator-> 的特性：如果返回的不是指针类型，会继续调用返回值的operator->()方法，直到最终解析出一个指针类型
    //  第一次返回非指针类型的 proxy，于是会生成一个临时的 proxy 对象，对 m_ptr 加锁，然后通过 proxy::-> 进行调用，
    //  最后释放临时的 proxy 对象来解锁。http://cunsh.ycool.com/post.1785495.html
    template <typename T, typename Lock>
    class CFThreadSafeWrapper
    {
    public:
        //typedef CFThreadSafeWrapper self_type;
        //typedef Pointer pointer_type;
        //typedef Lock lock_type;
    private:
        struct proxy{
            //嵌套类proxy的构造函数的隐式转换(即没有加explicit)是必要的，它可以消除额外的copy ctor的需求
            proxy(CFThreadSafeWrapper* host)   
                : m_host(host)
                , m_locker(&host->m_lockObj)
            {
                //FTLTRACE(_T("proxy construct\n"));
            };
            ~proxy()
            {
                //FTLTRACE(_T("proxy destructor\n"));
            }
            T* operator->(){
                return m_host->m_ptr;
            }
        private:
            CFThreadSafeWrapper* m_host;  //self_type* m_host;
            CFAutoLock<Lock> m_locker;
        private:
            //禁止拷贝构造和赋值操作符
            proxy(const proxy&);
            proxy& operator=(const proxy&);
        };
        friend struct proxy;
    public:
        CFThreadSafeWrapper(T* ptr, BOOL bManage)
            :m_ptr(ptr)
            ,m_bManage(bManage)
        {
        }
        ~CFThreadSafeWrapper()
        {
            if(m_bManage)
            {
                SAFE_DELETE(m_ptr);
            }
        }
        proxy operator->() {
            return this;
        }
        void reset(T* ptr ,BOOL bManage)
        {
            if (m_bManage)
            {
                SAFE_DELETE(m_ptr);
            }
            m_ptr = ptr;
            m_bManage = bManage;
        }
        T* release()
        {
            T* pRet = m_ptr;
            m_ptr = NULL;
            m_bManage = FALSE;
            return pRet;
        }

        //原本是想临时获取内部对象，提供一个大范围加锁的机制来增加性能，但实现上有问题
#if 0
        friend T* getImpObject(const CFThreadSafeWrapper<T, Lock> & self);
        friend Lock& getLockObject(const CFThreadSafeWrapper<T, Lock> & self);
#endif
    private:
        T*          m_ptr;
        BOOL        m_bManage;
        Lock        m_lockObj;
    };

#if 0
    template <typename T, typename Lock>
    T* getImpObject(const CFThreadSafeWrapper<T, Lock> & self){ return self.m_ptr;}

    template <typename T, typename Lock>
    Lock& getLockObject(const CFThreadSafeWrapper<T, Lock> & self){ return self.m_lockObj; }
#endif

    //线程创建对象 -- TODO: MFC ?
    class CFCRTThreadTraits
    {
    public:
        static HANDLE CreateThread(LPSECURITY_ATTRIBUTES lpsa, DWORD dwStackSize, LPTHREAD_START_ROUTINE pfnThreadProc, void *pvParam, DWORD dwCreationFlags, DWORD *pdwThreadId) throw()
        {
            FTLASSERT(sizeof(DWORD) == sizeof(unsigned int)); // sanity check for pdwThreadId

            // _beginthreadex calls CreateThread which will set the last error value before it returns.
            return (HANDLE) _beginthreadex(lpsa, dwStackSize, (unsigned int (__stdcall *)(void *)) pfnThreadProc, pvParam, dwCreationFlags, (unsigned int *) pdwThreadId);
        }
    };

    class CFWin32ThreadTraits
    {
    public:
        static HANDLE CreateThread(LPSECURITY_ATTRIBUTES lpsa, DWORD dwStackSize, LPTHREAD_START_ROUTINE pfnThreadProc, void *pvParam, DWORD dwCreationFlags, DWORD *pdwThreadId) throw()
        {
            //CreateThread时C运行时库没有初始化
            return ::CreateThread(lpsa, dwStackSize, pfnThreadProc, pvParam, dwCreationFlags, pdwThreadId);
        }
    };
    //TODO:FTL中怎么判断默认创建者应该用谁？
#if defined(_MT)  //!defined(_ATL_MIN_CRT) && 
    typedef CFCRTThreadTraits DefaultThreadTraits;
#else
    typedef CFWin32ThreadTraits DefaultThreadTraits;
#endif

    #define FTL_MAX_THREAD_DEADLINE_CHECK   5000

    //! 模版线程类
    typedef enum tagFTLThreadWaitType
    {
        ftwtStop, 
        ftwtContinue,
        ftwtTimeOut,
        ftwtError,
    }FTLThreadWaitType;
    
    /************************************************************************
    * 单个写入程序/多个阅读程序的保护--涉及到试图访问共享资源的任意数量的线程
    * 有些线程（写入程序）需要修改数据的内容，而有些线程（阅读程序）则需要读取数据
    * 4个原则:
    *   1) 当一个线程正在写入数据时，其他任何线程不能写入数据。
    *   2) 当一个线程正在写入数据时，其他任何线程不能读取数据。
    *   3) 当一个线程正在读取数据时，其他任何线程不能写入数据。
    *   4) 当一个线程正在读取数据时，其他线程也能够读取数据。            
    ************************************************************************/
    FTLEXPORT
    class CFRWLocker
    {
    public:
        FTLINLINE CFRWLocker();        // Constructor
        FTLINLINE virtual ~CFRWLocker();                // Destructor

        FTLINLINE BOOL Start();
        FTLINLINE BOOL Stop();

        FTLINLINE FTLThreadWaitType WaitToRead(DWORD dwTimeOut);        // Call this to gain shared read access
        FTLINLINE FTLThreadWaitType WaitToWrite(DWORD dwTimeOut);       // Call this to gain exclusive write access
        FTLINLINE void Done();              // Call this when done accessing the resource
    private:
        //用于保护所有的其他成员变量，这样，对它们的操作就能够以原子操作方式来完成
        CFLockObject*   m_pLockObject;

        //当许多线程调用WaitToRead，但是由于m_nActive是- 1而被拒绝访问时，所有阅读线程均等待该信标。
        //当最后一个正在等待的阅读线程调用Done时，该信标被释放，其数量是m_nWaitingReaders，从而唤醒所有正在等待的阅读线程
        HANDLE m_hSemReaders;     // Readers wait on this if a writer has access

        //当线程调用WaitToWrite，但是由于m_nActive大于0而被拒绝访问时，所有写入线程均等待该信标。
        //当一个线程正在等待时，新阅读线程将被拒绝访问该资源。这可以防止阅读线程垄断该资源。
        //当最后一个拥有资源访问权的阅读线程调用Done时，该信标就被释放，其数量是1，从而唤醒一个正在等待的写入线程
        HANDLE m_hSemWriters;

        HANDLE m_hStopEvent;        //Event to stop

        int    m_nWaitingReaders; // 表示想要访问资源的阅读线程的数量。
        int    m_nWaitingWriters; // 表示想要访问资源的写入线程的数量。

        //用于反映共享资源的当前状态(0=no threads, >0=# of readers, -1=1 writer)
        //1.如果该值是0，那么没有线程在访问资源。
        //2.如果该值大于0，这个值用于表示当前读取该资源的线程的数量。
        //3.如果这个数量是负值，那么写入程序正在将数据写入该资源。唯一有效的负值是- 1
        int    m_nActive;         
    };


    FTLEXPORT 
    class CFEventChecker
    {
		DISABLE_COPY_AND_ASSIGNMENT(CFEventChecker);
    public:
        FTLINLINE CFEventChecker(HANDLE hEventStop,HANDLE hEventContinue);
        FTLINLINE ~CFEventChecker();
        FTLINLINE FTLThreadWaitType GetWaitType(DWORD dwTimeOut = INFINITE);
        FTLINLINE FTLThreadWaitType SleepAndCheckStop(DWORD dwTimeOut);
    private:
        HANDLE		    m_hEventStop;
        HANDLE          m_hEventContinue;
    };

    FTLEXPORT template <typename T>
    class CFSyncEventUtility
    {
    public:
        FTLINLINE CFSyncEventUtility(void);
        FTLINLINE ~CFSyncEventUtility(void);

        FTLINLINE void ClearAllEvent();
        FTLINLINE void AddEvent(T t, HANDLE hEvent = NULL);
        FTLINLINE void SetEvent(T t);
        FTLINLINE void ResetEvent(T t);
        FTLINLINE void SetAllEvent();
        FTLINLINE void ResetAllEvent();
        FTLINLINE BOOL WaitAllEvent(DWORD dwMilliseconds = INFINITE);
        FTLINLINE size_t GetSyncEventCount();
        T WaitOneEvent(DWORD dwMilliseconds /* = INFINITE */);
    private:
        mutable CFCriticalSection   m_LockObject;
        struct SyncEventInfo
        {
            BOOL            bCreateEvent;
            HANDLE          hEvent;
        };
        typedef std::map<T, SyncEventInfo> SYNC_EVENT_MAP;
        SYNC_EVENT_MAP  m_AllSyncEventMap;
    };

    /*********************************************************************************************************
    * TIB -- Thread Information Block
    *********************************************************************************************************/
    FTLEXPORT
    class CFThreadUtils
    {
    public:
        // if dwThreadId = -1, then current thread
        //VC6中,显示线程名字采用在 Watch 窗口中输入 (char*)(dw(@TIB+0x14)),s
        FTLINLINE static BOOL SetThreadName( DWORD dwThreadID, LPTSTR szThreadName);
        FTLINLINE static LPCSTR GetThreadName(DWORD dwThreadID);
    };

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// 一个支持安全的 暂停、继续、停止 功能的线程框架
    FTLEXPORT template <typename ThreadTraits = DefaultThreadTraits>
    class CFThread
    {
		DISABLE_COPY_AND_ASSIGNMENT(CFThread);
    public:
        /// 构造函数，创建线程类实例
        /// @param [in] hEventStop 外部传入的在线程停止时激发的事件，如果为NULL，线程内部会自己创建一个。
        /// @param [in] hEventContinue 外部传入的保证线程能力运行的事件(如果该事件未被激发，则线程会等待--暂停)，
        ///                            如果为NULL，线程内部会自己创建一个
        /// @note  hEventStop 和 hEventContinue 都必须是手动重置事件
        FTLINLINE CFThread(HANDLE hEventStop = NULL,HANDLE hEventContinue = NULL);

        FTLINLINE virtual ~CFThread();
    public:
        FTLINLINE virtual FTLThreadWaitType GetThreadWaitType(DWORD dwTimeOut = INFINITE);
        FTLINLINE virtual FTLThreadWaitType SleepAndCheckStop(DWORD dwTimeOut);
        FTLINLINE BOOL Start(LPTHREAD_START_ROUTINE pfnThreadProc, void *pvParam, BOOL resetEvent = TRUE);
        FTLINLINE BOOL Wait(DWORD dwTimeOut = INFINITE, BOOL bCloseHandle = TRUE, BOOL bTerminateIfTimeOut = TRUE);
        FTLINLINE BOOL Stop();
        FTLINLINE BOOL StopAndWait(DWORD dwTimeOut = FTL_MAX_THREAD_DEADLINE_CHECK, BOOL bCloseHandle = TRUE,
			BOOL bTerminateIfTimeOut = TRUE );
        FTLINLINE BOOL Pause();
        FTLINLINE BOOL Resume();
        FTLINLINE BOOL LowerPriority();
        FTLINLINE BOOL RaisePriority();
        FTLINLINE int  GetPriority() const;
        FTLINLINE BOOL SetPriority(int priority);
        FTLINLINE BOOL IsThreadRunning() const;
        FTLINLINE BOOL HadRequestPause() const;
    protected:
        //unsigned int	m_Id;			//!< Thread ID
        HANDLE		    m_hEventStop;
        HANDLE          m_hEventContinue;
        HANDLE		    m_hThread;		//!< Thread Handle
        CFEventChecker* m_pEventChecker;
    private:
        BOOL            m_bCreateEventStop;
        BOOL            m_bCreateEventContinue;
    };

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
        //! 如果是new出来的，一定要在结束时调用 delete this（包括参数 pParam） -- 是否可以增加 m_bAutoDelete？
        virtual void Run(T param) = 0;
        virtual void OnCancelJob(T param)
        {
            UNREFERENCED_PARAMETER(param);
            //如果线程池停止了，剩下的Job将不会执行，会调用该函数，用于清除内存等资源，如
            //delete pParam; //注意先转换
            //delete this; 
            //注意：此处是个空实现
        }
    protected:
        //! 通过该函数，获取线程池的状态 -- Stop/Pause，用法同 CFThread:GetThreadWaitType:
        FTLINLINE FTLThreadWaitType GetThreadPoolWaitType(DWORD dwMilliseconds = INFINITE) const;
    private:
        CFThreadPool<T>* m_pThreadPool;     //设置为私有变量，即使是子类也不要直接更改
    };

    /*********************************************************************************************
    * 线程池 -- 基于预创建技术，将线程创建和销毁本身所带来的开销分摊到了各个具体的任务上，
    *           执行次数越多，每个任务所分担到的线程本身开销则越小
    *   适用范围：
    *     1.单位时间内处理任务频繁而且任务处理时间短 -- 如不适合 FTP 服务器；
    *     2.对实时性要求较高。如果接受到任务后在创建线程，可能满足不了实时要求，因此必须采用线程池进行预创建。 
    *     注意：使用线程池所产生的开销较大,相当多的线程和内部数据结构变成了你的进程的一个组成部分,不要盲目使用
	*
	* 微软提供的线程池(ms-help://MS.MSDNQTR.v90.en/dllproc/base/thread_pools.htm)
	*	线程池中有四个组件，每个组件由一组线程组成
	*     1. 非 I/O 组件 ( WT_EXECUTEDEFAULT )，由于线程与完成端口相关联，因此可以同时运行的线程数量限制为 CPU 数量的两倍。
	*        非 I/O 组件中的线程是根据工作量来创建或撤消的。
	*     2. I/O 组件( WT_EXECUTEINIOTHREAD ) －－ 运行发出异步I/O请求的代码
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
    * TODO: 增加动态收缩功能 -- 根据负载自动调整线程池中线程的数量
    *********************************************************************************************/
    //觉得使用指针似乎更好？使用模板的话，保存时应该使用引用还是指针？或直接用值？ -- 用值，不要过分优化
    typedef enum tagGetJobType
    {
        typeStop,
        typeSubtractThread,
        typeGetJob
    }GetJobType;

    FTLEXPORT template <typename T>  
    class CFThreadPool
    {
        friend class CFJobBase<T>;  //允许Job获取StopEventHandle -- 两边互为friend，是否合适？
    public:
        struct JobInfo
        {
            CFJobBase<T> *pJob;
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
        FTLINLINE BOOL SubmitJob(CFJobBase<T>* pJob,const T& param);
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
        LONG m_nCurNumThreads;                  //! 当前运行着的线程个数
        //LONG m_nCanSubtractThreadNum;
        HANDLE* m_pThreadHandles;               //! 保存线程句柄的数组(为了方便Wait)
        DWORD*  m_pThreadIds;                   //! 保存线程 Id 的数组(为了在线程结束后调整数组中的位置)
        std::queue<JobInfo*>    m_JobsQueue;    //! 保存Job的信息,由于会频繁加入、删除，因此使用queue
        HANDLE m_hSemaphoreJobToDo;             //! 保存还有多少个Job的信号量，每Submit一个Job，就增加一个
        HANDLE m_hEventStop;                    
        HANDLE m_hEventContinue;
        HANDLE m_hSemaphoreSubtractThread;      //! 用于减少线程个数时的信标,初始时个数为0，每要释放一个，就增加一个，
                                                //! 最大个数为 m_nMaxNumThreads - m_nMinNumThreads
        CFCriticalSection m_lockJobs;             //访问 m_JobsQueue 时互斥
        CFCriticalSection m_lockThreads;          //访问 m_pThreadHandles/m_pThreadIds 时互斥
        
        static unsigned int CALLBACK ThreadExecute(void *pThis);    //线程池中线程的执行函数
        //static unsigned int CALLBACK StopAsyncProc(void *pAsyncProxyParam);
        //static unsigned int CALLBACK SubtractThreadAsyncProc(void * pAsyncProxyParam);
    };

    //前向声明
    template<typename ELEMENT> class CFAutoQueueOperator;
    /*****************************************************************************************************************************
    * @brief
    *  使用 信标内核对象(Semaphore)控制的 生产者/消费者 多线程队列
    *     *********************************************************
    *     |                      Capability                       |	//最大可用数量 -- 构造函数中传入，不可更改)
    *     |    Queue Max Size         |   nReserveSlot            | //Queue最多能放入多少个Element，可通过 ReserveSlot/ReleaseSlot 更改
    *     |  m_hSemNumElements  |      m_hSemNumSlots             | //当前Queue中有多少个Element，通过 Append/Remove 增加/删除
    *     *********************************************************
    *     | <= Remove() , Append() => |                           | //默认时，Append在队列尾部追加新的Element，Remove从头取
    *     |    ReleaseSlot() =>       |     <= ReserveSlot()      |
    *     *********************************************************
    * 
    * @note
    *   1.当前活动Element的个数为：生产者线程(正在处理) + Queue中的个数 + 消费者线程(正在处理)，
    *     因此，个数可能会比构造中传入的多，如果要完全相同，需要用 CFAutoQueueOperator 进行锁定并计数--更改后未仔细测试，可能有Bug
    *   2.ReserveSlot/ReleaseSlot 功能尚未仔细测试
    *****************************************************************************************************************************/
    FTLEXPORT template<typename ELEMENT>
    class CFProducerResumerQueue
    {
        friend class CFAutoQueueOperator<ELEMENT>;
    public:
        //利用nReserveSlot来实现动态改变队列大小，但尚未仔细考虑和测试，可能出错
        //如果nReserveSlot为0，即和以前一样（不要调用 RelaseSlot 和 ReserveSlot）
        FTLINLINE CFProducerResumerQueue(LONG nCapability, LONG nReserveSlot = 0, HANDLE hEventStop = NULL);
        FTLINLINE virtual ~CFProducerResumerQueue();
        //! 允许队列追加数据 -- 刚构造好时是允许加入数据的
        FTLINLINE BOOL Start();
        //! 停止队列，会唤醒所有等待的线程，之后将不能再 Append & Remove
        FTLINLINE BOOL Stop();
        
        //! 判断Queue是否已经Stop
        FTLINLINE BOOL IsStopped();

        //! 生产者调用，在队列中增加元素，
        //! @return 
        //!   ftwtStop 在增加Element的时候被停止
        //!   ftwtContinue 成功增加
        //!   ftwtTimeOut 超时
        //!   ftwtError   出现错误，详情GetLastError(什么时候会出现这种情况？)
        FTLINLINE FTLThreadWaitType Append(const ELEMENT& element, DWORD dwTimeOut);
        //! 消费者端调用,从队列中取出元素
        //! @return -- 同 Append
        FTLINLINE FTLThreadWaitType Remove(ELEMENT& element, DWORD dwTimeOut);

        //! 在停止后调用该方法取出元素，用于保证释放Queue中保留的Element
        //! 临时措施，是否有更好的方法 ？
        //! 当Queue中没有剩下的Element后，返回False
        FTLINLINE BOOL RemoveAfterStop(ELEMENT& element);

        //! 减少Queue的可用大小(获取一些Slot用于保留)，可以指定超时值(需要等待占用Slot的element被取走) -- 引入异步通知机制？
        FTLINLINE FTLThreadWaitType ReserveSlot(LONG nCount,DWORD dwMilliseconds);
        //! 增加Queue的可用大小(释放一些保留的Slot)
        FTLINLINE BOOL ReleaseSlot(LONG nCount);

        //! 获取Queue最大可以增加到多大 -- 由构造函数传入
        FTLINLINE LONG GetCapability() const;
        //! 返回当前Queue中的Element个数
        FTLINLINE LONG GetElementCount() const;
        //! 返回保留下来的Slot数
        FTLINLINE LONG GetReserveSlotCount() const;

    protected:
        //! 如果想实现带优先级的队列，可以重载该函数，将 element 插入合适的位置(越前面的优先级越高)，并且返回 TRUE
        FTLINLINE virtual BOOL OnAppendElement(const ELEMENT &element);

        //!作为保护成员，由友元类 CFAutoQueueOperator 访问，可以完全的控制Element的个数 
        FTLINLINE BOOL RequestAppend(DWORD dwTimeOut, const ELEMENT& element);
        FTLINLINE BOOL CommitAppend();
        FTLINLINE BOOL RequestRemove(DWORD dwTimeOut, ELEMENT& element);
        FTLINLINE BOOL CommitRemove();
    protected:
        std::deque<ELEMENT> m_AllElement;
        const LONG m_nCapability;                   //! 最大可以放入的元素个数
        LONG m_nReserveSlot;                        //! Slot的个数 
        BOOL m_bCreateEventStop;                    //! 判断是否在本类中创建的 m_hEventStop
        mutable CFMutex             m_lockElements; //! 访问 m_AllElement 的临界区
        mutable CFCriticalSection   m_lockSlot;     //! 访问 m_nReserveSlot 的临界区
        HANDLE              m_hSemNumElements;      //! 用于控制当前已经有的元素
        HANDLE              m_hSemNumSlots;         //! 用于控制还能往队列中放入多少元素
        HANDLE              m_hEventStop;           //! 用于停止的事件,停止后将不能再添加Element
    };

    //! 用于帮助控制生产者、消费者队列的辅助类，可以锁定队列
    FTLEXPORT template<typename ELEMENT>
    class CFAutoQueueOperator
    {
    public:
        FTLINLINE CFAutoQueueOperator(
            CFProducerResumerQueue<ELEMENT>* pQueue,
            ELEMENT &refOperationElement, 
            BOOL bIsQuestAppend = TRUE,
            DWORD dwTimeOut = INFINITE);
          FTLINLINE ~CFAutoQueueOperator();
          FTLINLINE BOOL HaveGotQueue() const;
    private:
        CFProducerResumerQueue<ELEMENT>* m_pQueue;
        BOOL m_bIsQuestAppend;
        BOOL m_bHaveGotQueue;
        ELEMENT& m_refOperationElement;
    };

    //用于对生产者生产出来的数据进行同步，按照顺序提供给消费者(一般只有一个)的队列。
    //如果之前序号的数据没有来，消费者必须等待
    //CFProducerResumerQueue 中的 OnAppendElement 没有太大的作用 -- 别的线程也可以先取出来？
    //由于和 CFProducerResumerQueue 的接口和实现差别比较大，使用继承还是重新写？
    //通常可以用于线程池处理数据，但处理后的数据需要同步的情况 -- 流水线更好？
    FTLEXPORT template<typename ELEMENT>
    class CFSyncProducerResumerQueue // : public CFProducerResumerQueue<ELEMENT>
    {
    private:
        //< 每个线程一个PRQueue，这样可以很好的控制每个线程的执行情况
        std::map<DWORD, CFProducerResumerQueue<ELEMENT> >   m_threadsPRQueue; 
    };


    //后台线程绘制
    struct BackDrawInfo
    {
        //RECT    rect;
        HWND      hWnd;
        HDC       hDC;
    };

	//后台线程绘制，然后Render到HDC中，注意加锁 -- 例子？codeproject 上的gdiplusspeed
    template <typename T>
    FTLEXPORT class CFBackDrawer
    {
    public:
        CFBackDrawer(HWND hWnd, FTL::CFCriticalSection& refLockObj, DWORD dwInterval = 0)
            :m_hWnd(hWnd)
            ,m_refLockObj(refLockObj)
            ,m_ThreadDraw(NULL,NULL)
            ,m_dwLastUpdateTime(0)
        {
        }
        BOOL Start()
        {
            BOOL bRet = FALSE;

            T* pThis = static_cast<T*>(this);
            pThis->OnInit();
            bRet = m_ThreadDraw.Start(DrawProc, this, TRUE);
            return bRet;
        }
        BOOL Stop()
        {
            BOOL bRet = FALSE;
            API_VERIFY(m_ThreadDraw.StopAndWait());
            T* pThis = static_cast<T*>(this);
            pThis->OnFina();
            return bRet;
        }
        BOOL Pause()
        {
            BOOL bRet = FALSE;
            API_VERIFY(m_ThreadDraw.Pause());
            return bRet;
        }
        BOOL Resume()
        {
            BOOL bRet = FALSE;
            API_VERIFY(m_ThreadDraw.Resume());
            return bRet;
        }
        //BOOL AddDrawJob(HDC hSrcDC, const BackDrawInfo* pDrawInfo);
    public:
        BOOL OnInit()
        {
            return TRUE;
        }
        BOOL OnSingleStep()
        {
            //FTLASSERT(FALSE);
            return FALSE;
        }
        BOOL OnFina()
        {
            return TRUE;
        }
    protected:
        static DWORD WINAPI DrawProc(LPVOID pParam)
        {
            T* pThis = static_cast<T*>(pParam);
            DWORD dwResult = pThis->InnerDrawProc();
            return dwResult;
        }
        DWORD InnerDrawProc()
        {
            T* pThis = static_cast<T*>(this);
            while (ftwtContinue == m_ThreadDraw.GetThreadWaitType(INFINITE))
            {
                pThis->OnSingleStep();
            }
            return 0;
        }
    protected:
        HWND                        m_hWnd;
        FTL::CFCriticalSection&     m_refLockObj;
        FTL::CFThread<>             m_ThreadDraw;
        DWORD                       m_dwLastUpdateTime;
    };


    FTLEXPORT template<typename KEY, typename VALUE, typename CHILDCLASS>
    class CFItemCacheT
    {
    public:
        CFItemCacheT(DWORD nMaxCacheSize,DWORD clearPercent = 30);
        virtual ~CFItemCacheT();
    public:
        enum ClearMode
        {
            //clearByOptimal,     //最佳置换算法(需要“估计”不使用或很长时间内不使用的的项，很难实现)
            //clearByLFU,         //Least Frequently Used(最不经常使用), TODO -- 实现
            clearByLRU,         //Least recently used(最近最少使用算法，通常只置换一个)
            clearByMRU,         //Most recently used(最近最常使用算法，性能非常差) 
            //clearByNUR,         //Not Used Recently(最近未用置换算法,是LRU的近似算法，置换所有从上次计算后未使用过的)
            clearByFIFO,        //First In First Out(先入先出算法，最直观，但性能最差)
            clearByRandom,      //随机清除，不排序(可能是按照 map 中 KEY 的大小顺序排序)
        };
        BOOL GetItemByKey(const KEY key,VALUE& value);
        void SetClearMode(ClearMode mode);
        BOOL SetClearPercent(DWORD clearPercent);    //使用百分比进行清除
    protected:
        //virtual BOOL GetRealItem(KEY key,VALUE& value) = 0;
        //virtual void FreeItem(KEY key,VALUE& value) = 0;
    protected:
        struct CItemData
        {
            KEY key;
            VALUE value;
            LARGE_INTEGER lastAccess;   //上一次访问的时间
            DWORD index;
        };
        typedef std::map<KEY,CItemData*>  CACHE_MAP;
        CACHE_MAP m_ItemCacheMaps;
        static bool LastAccessGreater(CItemData* p, CItemData* q);
        static bool LastAccessSmaller(CItemData* p, CItemData* q);
        static bool IndexGreater(CItemData* p, CItemData* q);
        static bool IndexSmaller(CItemData* p, CItemData* q);
        void ClearCache(DWORD percent);     
    private:
        void Init(DWORD nMaxCacheSize,DWORD clearPercent);

        CFCriticalSection   m_LockObject;
        ClearMode           m_ClearMode;
        DWORD               m_MaxCacheSize;
        DWORD               m_ClearPercent;
        DWORD               m_CurrentIndex;
    };

    //模板内存池
    //  注意：Get后返回的指针将是上一次的值，需要自行进行初始化
    //        Get和Release都从list前部进行，减少page交换的次数
    template <typename T>
    class CFMemCacheT
    {
    public:
        CFMemCacheT( INT nInitSize = 0 , INT nMaxSize = MAXLONG );
        ~CFMemCacheT();
        T* Get();
        void Release( T* t );
    private:
        CFCriticalSection   m_lockObject;
        std::list< T* >     m_allBlocks;
        std::list< T* >     m_freeBlocks;
        INT                 m_nMaxSize;
        INT                 m_nCurCount;
    };

//Function
    /*******************************************************************************************************
    * 解决复杂的线程同步问题（由于每次调用都需要创建线程，资源消耗较大－－注意使用的地方和频率）
    * 可以暂停调用线程的运行，直到单组句柄中的所有对象均已同时得到通知为止。
    *
    * 使用方式:
    *   HANDLE hWaitObjes[] = {hEventCopy,hSemFile,NULL,hEventContinue,NULL,hEventStop};
    *   DWORD dwWait = WaitForMultipleExpressions(_countof(hWaitObjes),hWaitObjes,INFINITE);
    *
    * 返回值：
    *	WAIT_OBJECT_0至(WAIT_OBJECT_0 +表达式－1的号码) ---  用于指明哪个表达式被选定了
    *   WAIT_TIMEOUT  在指定的时间内没有选定表达式
    *	WAIT_FAILED   产生一个错误。若要了解详细信息，调用GetLastError。
    *                 ERROR_TOO_MANY_SECRETS -- 设定的表达式超过了64个
    *                 ERROR_SECRET_TOO_LONG  --至少有一个表达式设定的对象超过了63个
    *
    *	@param[in] nExpObjects		用于指明参数phExpObjects指向的数组中的项目数量
    *	@param[in] phExpObjects	    包含多组内核对象句柄，每组句柄之间用一个NULL句柄项分开,
    *                               将单组句柄中的对象视为用AND组合起来的对象组，
    *                               而各个句柄组则是用OR组合起来的句柄组
    *	@param[in] dwMilliseconds	
    *
    *   @attention 句柄数组的项目可以大大超过64。然而不得拥有64个以上的表达式，并且每个表达式可以包含63个以上的句柄。
    *              不支持互斥对象(出错症状？)－－原因是“互斥对象可以被线程所拥有
    *              如果AND线程之一获得对互斥对象的所有权，那么当线程终止运行时，它就会放弃该互斥对象”
    * 
    * 实现说明：
    *   WaitForMultipleObjectsEx使得线程可以等待单个 AND 表达式。
    *   为了扩展该函数，使之包含使用OR的表达式，必须生成多个线程：每个OR表达式需要一个线程。
    *   这些线程中的每一个都使用 WaitForMultipleObjectsEx 来等待AND表达式，
    *   当其中的一个表达式被选定时，生成的线程中就有一个被唤醒并终止运行。
    *   调用 WaitForMultipleExpressions 函数的线程(它与产生所有OR线程的线程相同)必须等待，
    *   直到其中的一个OR表达式得以实现。生成的线程(OR表达式)的数量被传递给dwObjects参数，phObjects参数则指向一个数组，该数组
    *   包含生成的线程句柄的列表。对于fWaitAll参数，则传递FALSE ，这样，一旦任何一个表达式得以实现，主线程就立即醒来。
    *   实现注意：
    *     1.不希望多个OR线程在同时醒来，因为成功地等待某个内核对象会导致该对象改变其状态（如信标数量） 
    *       -- 创建一个自己的信标对象，初始数量是1，每个OR线程都进行等待(每组句柄可以设定的句柄不得超过64－1个(一个自定义的信标))
    *     2.如何强制正在等待的线程停止等待，以便正确地撤消－－使用 QueueUserAPC 来强制等待线程醒来（当一个项目进入
    *       异步过程调用（APC）队列时，如果等待线程处于待命状态，那么它们就会被强制唤醒）
    *     3.如何正确处理超时 --如果线程在等待时没有任何表达式等待实现，那么主线程调用的WaitForMultipleObjects就返回一个
    *       WAIT_TIMEOUT值。如果出现这种情况，我就想要防止任何表达式得以实现，否则可能导致对象改变它们的状态
    *       －－使用等待信标的办法来防止其他表达式的实现。这将使信标的数量递减为0，同时所有的OR线程都不会醒来，如果在中间某个
    *       操作间隙，某个表达式实现，则再次等待线程结束。
    ********************************************************************************************************/

    FTLINLINE DWORD WINAPI WaitForMultipleExpressions(DWORD nExpObjects,CONST HANDLE * phExpObjects,DWORD dwMilliseconds);

    //使用线程池技术减少频繁创建、销毁线程的消耗
    //FTLINLINE DWORD WINAPI WaitForMultipleExpressions(CFThreadPool<EXPRESSION>* pWaitThreadPool,DWORD nExpObjects,
    //    CONST HANDLE * phExpObjects,DWORD dwMilliseconds);

}//namespace FTL




namespace FTL
{

    namespace ThreadUtil
    {
#if 0
        class CFWaitForMultipleExpressionsJob : public CFJobBase<EXPRESSION>
        {
        public:
            virtual unsigned int Run(EXPRESSION express)
            {
                DWORD dwResult = WaitForMultipleObjectsEx(
                    express.m_nExpObjects, express.m_phExpObjects, 
                    TRUE, INFINITE, TRUE);
                delete this;
                return dwResult;
            }
            virtual void OnCancelJob(EXPRESSION express)
            {
                delete this;
            }
        };

        inline DWORD WINAPI WaitForMultipleExpressions(CFThreadPool<EXPRESSION>* pWaitThreadPool,DWORD nExpObjects,
            CONST HANDLE * phExpObjects,DWORD dwMilliseconds)
        {
            FTLASSERT(FALSE);//还没有更改完成

            FTLASSERT(pWaitThreadPool->IsThreadPoolRunning());
            

            BOOL bRet = FALSE;
            //Allocate a temporary array because we modify the passed array and 
            //we need to add a handle at the end for the hsemOnlyOne semaphore.
            PHANDLE phExpObjectsTemp = (PHANDLE) _alloca(sizeof(HANDLE) * (nExpObjects+1));
            CopyMemory(phExpObjectsTemp,phExpObjects,sizeof(HANDLE) * nExpObjects);
            phExpObjectsTemp[nExpObjects] = NULL; //put sentinel at end

            //Semaphore to guarantee that only one expression gets satisfied
            HANDLE hsemOnlyOne = CreateSemaphore(NULL,1,1,NULL);

            //Expression information: 1 per possible thread
            EXPRESSION Expression[MAXIMUM_WAIT_OBJECTS] = {0};

            DWORD dwExpNum      = 0; //Current expression number
            DWORD dwNumExps     = 0; //Total number of expressions

            DWORD dwObjBegin    = 0; //First index of a set
            DWORD dwObjCur      = 0; //Current index of object in a set

            DWORD dwWaitRet = 0;
            unsigned int ThreadId = 0;
            //Array of thread handles for threads : 1 per expression
            HANDLE ahThreads[MAXIMUM_WAIT_OBJECTS] = {0};

            //Parse the callers handle list by initializing a structure for each expression
            //and adding hsemOnlyOne to each expression.

            while ((dwWaitRet != WAIT_FAILED) && (dwObjCur <= nExpObjects))
            {
                //while no errors,and object handles are in the caller's list...
                //Find next expression (OR-expressions are separated by NULL handles)
                while (phExpObjectsTemp[dwObjCur] != NULL)
                {
                    dwObjCur++;
                }

                //Initialize Expression structure which an OR-thread waits on
                phExpObjectsTemp[dwObjCur] = hsemOnlyOne;
                Expression[dwNumExps].m_phExpObjects = &phExpObjectsTemp[dwObjBegin];
                Expression[dwNumExps].m_nExpObjects =  (dwObjCur + 1 - dwObjBegin);

                if (Expression[dwNumExps].m_nExpObjects > MAXIMUM_WAIT_OBJECTS)
                {
                    //Error: Too many handles in single expression
                    dwWaitRet = WAIT_FAILED;
                    SetLastError(ERROR_SECRET_TOO_LONG);
                }

                //Advance to the next expression
                dwObjBegin = ++dwObjCur;
                if (++dwNumExps == MAXIMUM_WAIT_OBJECTS)
                {
                    //Error: Too many expressions
                    dwWaitRet = WAIT_FAILED;
                    SetLastError(ERROR_TOO_MANY_SECRETS);
                }
            }

            if (dwWaitRet != WAIT_FAILED)
            {
                //No errors occurred while parsing the handle list

                //Spawn thread to wait on each expression
                for (dwExpNum = 0; dwExpNum < dwNumExps; dwExpNum++)
                {
                    ahThreads[dwExpNum] = (HANDLE)_beginthreadex(NULL,
                        1, //we only require a small stack
                        WFME_ThreadExpression,&Expression[dwExpNum],
                        0,&ThreadId);
                }

                //wait for an expression to come TRUE or for a timeout
                dwWaitRet = WaitForMultipleObjects(dwExpNum,ahThreads,FALSE,dwMilliseconds);
                if (WAIT_TIMEOUT == dwWaitRet)
                {
                    //we timed-out, check if any expressions were satisfied by checking the state of the hsemOnlyOne semaphore
                    dwWaitRet = WaitForSingleObject(hsemOnlyOne,0);
                    if (WAIT_TIMEOUT == dwWaitRet)
                    {
                        //if the semaphore was not signaled, some thread expressions
                        //was satisfied; we need to determine which expression.
                        dwWaitRet = WaitForMultipleObjects(dwExpNum,ahThreads,FALSE,INFINITE);
                    }
                    else
                    {
                        //No expression was satisfied and WaitForSingleObject just gave us the semaphore
                        //so we know that no expression can ever be satisfied now -- waiting for an expression has tiemd-out.
                        dwWaitRet = WAIT_TIMEOUT;
                    }
                }
                //Break all the waiting expression threads out of their wait state so that they can terminate cleanly
                for (dwExpNum = 0; dwExpNum < dwNumExps ; dwExpNum++)
                {
                    if ((WAIT_TIMEOUT == dwWaitRet) || (dwExpNum != (dwWaitRet - WAIT_OBJECT_0)))
                    {
                        QueueUserAPC(WFME_ExpressionAPC,ahThreads[dwExpNum],0);
                    }
                }
#ifdef _DEBUG
                //In debug builds,wait for all of expression threads to terminate to make sure that we are forcing
                //the threads to wake up.
                //In non-debug builds,we'll assume that this works and not keep this thread waiting any longer.
                WaitForMultipleObjects(dwExpNum,ahThreads,TRUE,INFINITE);
#endif
                //Close our handles to all the expression threads
                for (dwExpNum = 0; dwExpNum < dwNumExps; dwExpNum++)
                {
                    SAFE_CLOSE_HANDLE(ahThreads[dwExpNum],NULL);
                }
            }//error occurred while parsing

            SAFE_CLOSE_HANDLE(hsemOnlyOne,NULL);
            return dwWaitRet;
        }
#endif
    }
}

#endif //FTL_THREAD_H

#ifndef USE_EXPORT
#  include "ftlthread.hpp"
#endif
