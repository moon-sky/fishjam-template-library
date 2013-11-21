#ifndef FTL_IOCP_H
#define FTL_IOCP_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlIocp.h requires ftlbase.h to be included first
#endif

#include "ftlThread.h"
#include "ftlMem.h"
#include "ftlSharePtr.h"
#include "ftlSocket.h"

/*************************************************************************************************************************
// http://www.cnblogs.com/lancidie/archive/2011/12/19/2293773.html
* IOCP(IO Completion Port -- 完成端口) -- 一个通知队列，和Overlapped I/O协同工作，可以在一个“管理众多的句柄且受制于I/O的程序(如Web服务器)”中获得最佳性能。
*   没有64个HANDLE的限制，使用一堆线程(通常可设置为 CPU个数*2+2 )服务一堆Events的性质，自动支持 scalable。
*   操作系统把已经完成的重叠I/O请求的通知放入完成端口的通知队列(一个线程将一个请求暂时保存下来)，等待另一个线程为它做实际服务。
*   线程池的所有线程 = 目前正在执行的 + 被阻塞的 + 在完成端口上等待的。
*   本质是一个调用异步IO操作完成时的回调函数的线程池,目前Windows平台上性能最好的输入输出模型。
*   微软例子：web\Wininet\Async
*   使用流程：
*     1.产生一个I/O completion port -- hPort=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL...)。产生一个没有和任何文件Handle有关系的port
*     2.产生一堆线程 -- 一般是 for(CPU*2+2){ _beginthreadex }
*     3.让完成端口和文件handle、Socket句柄等产生关联 -- CreateIoCompletionPort(hFile,hPort...)，为每一个欲附加的文件Handle进行关联
*     4.让每一个线程在Completion Port上等待，返回后根据情况进行处理
*       GetQueuedCompletionStatus(,INFINITE)，可通过 CONTAINING_RECORD 宏取出其中指向扩展结构的指针
*     5.开始对着那个文件handle发出一些overlapped I/O请求 -- 如 ReadFile、WriteFile、DeviceIoControl等，
*     6.其他线程如果需要交互(如通知程序结束)，需要通过 PostQueuedCompletionStatus 向完成端口放入事件通知，从而唤醒各个线程,
*       然后 WaitForSingleObject(hPort, 超时时间)
*   注意：
*     1.如果重叠操作调用失败时(返回值错误且错误原因不是ERROR_IO_PENDING)，那么完成端口将不会收到任何完成通知。
*       如果重叠操作调用成功，或者错误原因是ERROR_IO_PENDING的错误时，完成端口将总是能够收到完成通知。
*
*   BindIoCompletionCallback -- 内部封装了IOCP的功能，通过设置的回调函数处理IO，
*     通常在其回调中需要调用 RtlNtStatusToDosError 将内部的NTStatus码转换成Windows的错误码(Ring0 -> Ring3)
*     ?最佳实践(避免阻塞IOCP)?--在回调函数中根据操作类型(如接收完成或发送完成)调用 QueueUserWorkItem 
*        将对应操作的WorkItem放入线程池
*************************************************************************************************************************/

namespace FTL
{
    //前向声明
    class CFIocpBaseTask;
    class CFIocpMgr;

    FTLINLINE LONGLONG AddOverLappedOffset(LPOVERLAPPED pOverlapped, DWORD dwTransfered)
    {
        LARGE_INTEGER* pLargeInteger = reinterpret_cast<LARGE_INTEGER*>(&pOverlapped->Offset);
        pLargeInteger->QuadPart += dwTransfered;
        return pLargeInteger->QuadPart;
    }

    enum IocpOperationType 
    {
        //iotUnknown = -1,

	    iotInitialize,   // The client just connected
	    iotRead,         // Read from the client. 
	    //IOReadCompleted,// Read completed
	    iotWrite,        // Write to the Client
	    //IOWriteCompleted, // Write Completed.
	    //IOZeroByteRead, // Read zero Byte from client (dummy for avoiding The System Blocking error) 
	    //IOZeroReadCompleted, // Read Zero Byte  completed. (se IOZeroByteRead)
	    //IOTransmitFileCompleted,
    };

    class CFIocpBuffer
    {
    public:
        OVERLAPPED          m_overLapped;
        //HANDLE              m_hIoHandle;
        IocpOperationType   m_operType;
        LONG            m_nSequenceNumber;
        PBYTE	        m_pBuffer;
        DWORD           m_dwSize;
        //DWORD           m_dwUsed;
    public:
        FTLINLINE CFIocpBuffer();
        FTLINLINE ~CFIocpBuffer();
        FTLINLINE VOID Reset(DWORD dwSize);
    };
    typedef CFMemoryPoolObjectHelper<CFIocpBuffer> CFIoBufferPoolObject;
    typedef CFSharePtr<CFIoBufferPoolObject>    CFIoBufferPoolObjectPtr;

    class CFIocpBaseTask
    {
        friend class CFIocpMgr;
    public:
        typedef std::map<LONG, CFIoBufferPoolObjectPtr> IocpBufferMap;  //可以在上面同时投递多个IO请求
        FTLINLINE CFIocpBaseTask();

        virtual INT    GetIocpHandleCount() const { return 1; }
        virtual HANDLE GetIocpHandle(INT index) const = 0;
        //virtual BOOL   OnIoComplete(CFIocpMgr* pIocpMgr, CFIoBufferPoolObjectPtr& pIoBuffer, DWORD dwBytesTransferred) = 0;
        virtual BOOL OnInitialize(CFIocpMgr* pIocpMgr, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred) { return TRUE; }
        virtual BOOL AfterReadCompleted(CFIocpMgr* pIocpMgr, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred) { return TRUE; }
        virtual BOOL AfterWriteCompleted(CFIocpMgr* pIocpMgr, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred) { return TRUE; }
        virtual BOOL OnUninitialize(CFIocpMgr* pIocpMgr, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred) { return TRUE; }

    private:
        LONG            m_nCurReadSequence;
        LONG            m_nCurWriteSequence;

        IocpBufferMap   m_ReadBufferMap;
        IocpBufferMap   m_WriteBufferMap;
    };

    //class CFSocketIocpTask : public CFIocpBaseTask
    //{
    //public:
    //    FTLINLINE virtual HANDLE GetIocpHandle() const;
    //protected:
    //    SOCKET  m_nSocket;
    //};

    //class CFFileIocpTask : public CFIocpBaseTask
    //{
    //public:
    //    FTLINLINE virtual HANDLE GetIocpHandle() const;
    //protected:
    //    HANDLE m_hHandle;
    //};

    //template <typename T>
    class CFIocpMgr
    {
    public:
        enum 
        {
            DEFAULT_THREAD_COUNT = (DWORD)(-1),
        };

        FTLINLINE CFIocpMgr(DWORD dwBufferSize = 4096);
        FTLINLINE ~CFIocpMgr();

        //0 is default, will create as 2 * CPU + 2
        FTLINLINE BOOL Start(DWORD NumberOfConcurrentThreads = DEFAULT_THREAD_COUNT);
        FTLINLINE BOOL Stop();
        FTLINLINE VOID Close(DWORD dwMilliseconds = INFINITE);

        //关联需要等待完成的重叠IO操作的句柄(如 File、SOCKET、Pipe 等)
        FTLINLINE BOOL AssociateTask(CFIocpBaseTask* pTask);

    protected:
        //工作线程初始化、终止化，可以进行COM初始化等
        FTLINLINE virtual BOOL OnIocpStart() { return TRUE; }
        FTLINLINE virtual BOOL OnIocpStop() { return TRUE; }

        FTLINLINE virtual BOOL OnWorkThreadInit();
        FTLINLINE virtual VOID OnWorkThreadFina();
    private:
        HANDLE  m_hIoCompletionPort;
        HANDLE* m_pWorkThreads;
        DWORD    m_dwBufferSize;
        volatile BOOL m_bStop;

        CFMemoryPoolT<CFIocpBuffer> m_IocpBufferPool;

        DWORD   m_nConcurrentThreadCount;

        FTLINLINE BOOL  _InitlizeIocp(DWORD NumberOfConcurrentThreads);
 
        FTLINLINE DWORD _GetDefaultConcurrentThreadCount();
        FTLINLINE DWORD _InnerWorkThreadProc();
        FTLINLINE BOOL  _HandleError(CFIocpBaseTask* pIocpTask, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred, DWORD dwErr);

        FTLINLINE static DWORD __stdcall _WorkThreadProc(LPVOID pParam);
        FTLINLINE BOOL _ProcessPackage(CFIocpBaseTask* pTask, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred);
    };

    class CFIospServerSocket : public CFIocpBaseTask
        , public CFSocket
    {
        virtual INT GetIocpHandleCount() const
        {
            return 1;
        }

        virtual HANDLE GetIocpHandle( INT index ) const
        {
            return (HANDLE)m_socket;
        }

    };

    class CFIocpNetServer;

    class CFIocpListenTask : public CFIocpBaseTask
    {
    public:
        CFIocpListenTask(SOCKET socketListen)
        {
            m_socketListen = socketListen;
        }
        FTLINLINE virtual INT GetIocpHandleCount() const
        {
            return 1;
        }

        FTLINLINE virtual HANDLE GetIocpHandle( INT index ) const
        {
            FTLASSERT(index < 1);
            return (HANDLE)m_socketListen;
        }

        FTLINLINE virtual BOOL OnInitialize(CFIocpMgr* pIocpMgr, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred )
        {
            FTLASSERT(pIoBuffer->m_operType == iotInitialize);
            
            CFIospServerSocket* pSocket = new CFIospServerSocket();
            pIocpMgr->AssociateTask(pSocket);

            return TRUE;
        }
    private:
        SOCKET m_socketListen;
    };
    class CFIocpNetServer : public CFIocpMgr
    {
    public:
        FTLINLINE virtual BOOL OnIocpStart();
        FTLINLINE virtual BOOL OnIocpStop();
    protected:
        SOCKET      m_nSocketListen;
    };
}
#endif //FTL_IOCP_H

#ifndef USE_EXPORT
#  include "ftlIocp.hpp"
#endif 