#ifndef FTL_SOCKET_H
#define FTL_SOCKET_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlSocket.h requires ftlbase.h to be included first
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>        //包含 SO_CONNECT_TIME 等宏定义，可能会造成其他错误?

#include "ftlNet.h"
#include "ftlIocp.h"

/*

怎么划分？
             CFClientSocket/CFServerSocket
            / 
CFSocketBase
            \CFTCPSocket/CFUDPSocket
*/

/*************************************************************************************************************************
* 1.是否能通过 select 等异步Accept?
* 2.长连接中Server需要保存客户端的socket列表，如果是多个的话怎么处理?
* 3.客户端调用 bind 的好处是什么，UDP 客户端如果调用过 connect 的话，可以直接通过 send/recv 交互，但只能和一个对端交互了。
* 4.UDP 服务器只需要bind，不需要listen，可以通过 recvfrom/sendto 直接交互。但是否需要accept?
* 5.AF_INET6 宏在linux下定义为 10, 在 Windows下定义为 23? 这样的话怎么兼容？可以将 ai_family 设置为 AF_UNSPEC(0)，然后通过其他参数来区分？
* 6.(ipv6.google.com 为什么不能访问？) -- 用IPV6建立服务器的话，即使客户端仍使用IPV4的socket连接也可以正常通讯，IPV4的地址会被转换成类似 ::ffff:192.168.1.1 的地址
*   IPV6的服务器上如果有IPV4的地址(主机有双重协议栈)，则对IPV4客户端是兼容的
* 7.getaddrinfo 中如果指定 hints.family 为 AF_UNSPEC，则解析时会返回可用的 IPV4/IPV6 地址链表，则可以自动兼容？
* 
*
* IN6_IS_ADDR_V4MAPPED -- 判断是否是IPV4的地址映射成的IPV6
*
* 
* 地址结构
*   基础:sockaddr, family(2) + data(14) = 16
*   IPV4:sockaddr_in, family(2) + port(2) + sin_addr(4) + zero(8) = 16
*   IPV6:sockaddr_in6, family(2) + port(2) + flowinfo(4) + sin6_addr(16) [+ scope_id(4)] = 24[28]
*   Storage:sockaddr_storage -- family(2) + data(126?)，可以兼容所有的地址结构(IPV4/IPV6/Unix 等)
*     能提供严格的结构对齐，能容纳系统支持的更大的地址结构、容器，
*     使用时需要尽量强制转换成 sockaddr_in/sockaddr_in6 后操作?
*     bind 函数时 sizeof 也支持 storage
*   ??      sockaddr_storage_xp
* 
* addrinfo -- 通用的地址信息?
* 
* 各协议的区别
*     项目                             IPV4                                   IPV6                             兼容
*   地址结构                  sockaddr_in/SOCKADDR_IN                 sockaddr_in6/SOCKADDR_IN6         sockaddr_storage
*   地址长度                            4字节
*   地址长度宏                   INET_ADDRSTRLEN(22)                   INET6_ADDRSTRLEN(65)
*   通配地址                      htonl(INADDR_ANY)                        in6addr_any 
*   地址转换                  inet_aton/inet_ntoa/inet_addr                                              WSAAddressToString/InetNtop(Vista!!)
*   主机名和地址转换           gethostbyname/gethostbyaddr                                         getaddrinfo/freeaddrinfo/getnameinfo
*   包头长度(字节)                      20                                     40 
*   DNS中的记录(什么意思?)             A记录                                 AAAA记录
*************************************************************************************************************************/


namespace FTL
{
	class CFWinsockEnvInit
	{
	public:
		FTLINLINE CFWinsockEnvInit(WORD wVersionRequested = MAKEWORD(2, 2));
		FTLINLINE ~CFWinsockEnvInit();
	public:
		int		m_initResult;
		WSADATA m_wsaData;
	};

	//兼容 IPV4/IPV6 的地址管理类，套接字参数: 协议(AF_XXX) + IP地址 + 端口号
	//template <typename SOCKADDR_BASE = sockaddr_in>
	//可以尝试通过 union 兼容，判断 family 后 强制转换成对应的结构并处理

#if 0
	class CFSockAddrIn :  public SOCKADDR_IN //  : public addrinfo// 
	{
	public:
		FTLINLINE static USHORT GetPortNumber(LPCTSTR strServiceName);

		FTLINLINE CFSockAddrIn() { Clear(); }
		FTLINLINE CFSockAddrIn(const CFSockAddrIn& sin) { Copy( sin ); }
		FTLINLINE ~CFSockAddrIn() { }
		FTLINLINE CFSockAddrIn& Copy(const CFSockAddrIn& sin);
		FTLINLINE void    Clear() { memset(this, 0, sizeof(SOCKADDR_IN)); }
		FTLINLINE bool    IsEqual(const CFSockAddrIn& sin) const;
		FTLINLINE bool    IsGreater(const CFSockAddrIn& sin) const;
		FTLINLINE bool    IsLower(const CFSockAddrIn& pm) const;
		FTLINLINE bool    IsNull() const { return ((sin_addr.s_addr==0L)&&(sin_port==0)); }
		FTLINLINE ULONG   GetIPAddr() const { return sin_addr.s_addr; }
		FTLINLINE short   GetPort() const { return sin_port; }
		FTLINLINE bool    CreateFrom(LPCTSTR sAddr, LPCTSTR sService, int nFamily = AF_INET);
		FTLINLINE CFSockAddrIn& operator=(const CFSockAddrIn& sin) { return Copy( sin ); }
		FTLINLINE bool    operator==(const CFSockAddrIn& sin) { return IsEqual( sin ); }
		FTLINLINE bool    operator!=(const CFSockAddrIn& sin) { return !IsEqual( sin ); }
		FTLINLINE bool    operator<(const CFSockAddrIn& sin)  { return IsLower( sin ); }
		FTLINLINE bool    operator>(const CFSockAddrIn& sin)  { return IsGreater( sin ); }
		FTLINLINE bool    operator<=(const CFSockAddrIn& sin) { return !IsGreater( sin ); }
		FTLINLINE bool    operator>=(const CFSockAddrIn& sin) { return !IsLower( sin ); }
		FTLINLINE operator LPSOCKADDR() { return (LPSOCKADDR)(this); }
		FTLINLINE size_t  Size() const { return sizeof(SOCKADDR_IN); }
		FTLINLINE void    SetAddr(SOCKADDR_IN* psin) { CopyMemory(this, psin, Size()); }
	};
#endif 

    enum FSocketType
    {
        stTCP,
        stUDP,
    };

    enum FSocketMode
    {
        smClose,
        smAccept,
        smRead,
        smWrite,
        smPending
    };

    typedef struct tagFOVERLAPPED
    {
        OVERLAPPED				overLapped;
        WSABUF					dataBuf;
        FSocketMode             socketMode;
        volatile UINT	        nSession;
    }FOVERLAPPED, *LPFOVERLAPPED;

    class CFSocket
    {
    public:
        SOCKET              m_socket;
		FTLINLINE CFSocket* CreateSocketByAddr(CFSocketAddress& /*addr*/)
		{
			//服务器端：通过hints指定过滤条件，解析出本地的地址，然后可以直接用于创建socket并绑定
			/*
			struct addrinfo hints = { 0 };
			struct addrinfo *res = NULL;
			hints.ai_family = AF_UNSPEC;
			hints.ai_socktype = SOCK_DGRAM;

			//AI_PASSIVE -- 被动的，用于bind，通常用于 server socket
			//AI_CANONNAME -- 解析域名
			//AI_NUMERICHOST -- 地址为数字串
			hints.ai_flags = AI_PASSIVE;

			rc = getaddrinfo(NULL, "8080", &hints, &res);  //res 返回地址链表，一般来说使用第一个bind成功就可以了
			socket s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
			bind(s, res->ai_addr, res->ai_addrlen);
			freeaddrinfo(res);
			*/
			
			//客户端
			struct addrinfo	hints = {0};
			struct addrinfo * res = NULL;
			hints.ai_flags = AF_UNSPEC;
			hints.ai_socktype = SOCK_DGRAM;
			hints.ai_flags = AI_CANONNAME;
			int rc = SOCKET_ERROR;
            NET_VERIFY(getaddrinfo("www.baidu.com", "http", &hints, &res));  //得到指定域名的连接地址(可能会同时返回 IPV4/IPV6 的地址)
			SOCKET socketfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
			connect(socketfd, res->ai_addr, res->ai_addrlen);
		}

        FTLINLINE CFSocket();
        FTLINLINE virtual ~CFSocket();
		FTLINLINE BOOL IsOpen();
        FTLINLINE int Open(FSocketType st, BOOL bAsync);
        FTLINLINE int Close();
        FTLINLINE int Shutdown(INT how);

		//Connection
		FTLINLINE int Connect(const CFSocketAddress& addr);// LPCTSTR pszAddr, INT nSocketPort);

		//Data
		FTLINLINE int Send(const BYTE* pBuf, INT len, DWORD flags);
		FTLINLINE int Recv(BYTE* pBuf, INT len, DWORD flags);

        FTLINLINE int IoCtl(long cmd, u_long* argp);
		//FTLINLINE int SetOpt(xxxx); //setsocketopt 的封装


		FTLINLINE CFSocket* Accept();
		FTLINLINE int Associate(SOCKET socket, SOCKADDR_IN* pForeignAddr);
		FTLINLINE SOCKADDR_IN& GetForeignAddr() { return m_foreignAddr; }
	protected:
		FTLINLINE virtual void OnOpen() {}
		FTLINLINE virtual void OnClose() {}
    protected:
        FSocketType         m_socketType;
        BOOL                m_bASync;
        CFCriticalSection   m_lockObject;
		SOCKADDR_IN		m_foreignAddr;
        volatile UINT       m_nSession;
    };

    class CFServerSocket : public CFSocket
    {
    public:
        FTLINLINE CFServerSocket();
        FTLINLINE ~CFServerSocket();
        //Server
		FTLINLINE int Bind(USHORT listenPort, LPCTSTR pszBindAddr);
        FTLINLINE int StartListen(INT backlog, INT nMaxClients);
		//FTLINLINE void ReleaseClient(CFClientSocketT<T>* pClient);
	protected:
		FTLINLINE virtual void OnClose();
    protected:
        INT     m_nMaxClients;
    };

    FTLEXPORT template < typename T>
    class CFNetClientT
    {
    public:
        CFNetClientT(FSocketType st = stTCP);
        virtual ~CFNetClientT();
        int Create();
        int Destroy();
        int Connect(LPCTSTR pszAddr);
    protected:
        CFSocket*		 m_pClientSocket;
        FSocketType      m_socketType;
    };

    FTLEXPORT template< typename T >
    class CFNetServerT
    {
    public:
        CFNetServerT(FSocketType st = stTCP);
        virtual ~CFNetServerT();
    public:

        /*
        * param [in] listenPort 
        * param [in] backlog 正在等待连接的最大队列长度
        */
        int Create(USHORT listenPort, LPCTSTR pszBindAddr = NULL );
        //int Close();
        int Destroy();
        int Start(INT backlog, INT nMaxClients);
        int Stop();
        //CFSocket<T>* Accept();
    protected:
        FSocketType                 m_socketType;
        CFServerSocket*			m_pServerSocket;

        //CFThreadPool<DWORD>*    m_pIoServerThreadPool;
        HANDLE                  m_hIoCompletionPort;
        HANDLE                  m_hServerThread;
        HANDLE                  *m_pWorkerThreads;

        DWORD                   getNumberOfConcurrentThreads();
        static unsigned int __stdcall serverThreadProc( LPVOID lpThreadParameter );
        static unsigned int __stdcall workerThreadProc( LPVOID lpThreadParameter );
        unsigned int     doServerLoop();
        unsigned int     doWorkerLoop();
    };
    class CFSocketUtils
    {
    public:
#if 0
        static FTLINLINE size_t readn(int fd, void* vptr, size_t n);
        static FTLINLINE size_t writen(int fd, const void* vptr, size_t n);
#endif 
        static FTLINLINE int DumpSocketOption(SOCKET s);
    };


    class CFIocpClientSocket : public CFIocpBaseTask
        , public CFSocket
    {
    public:
        FTLINLINE CFIocpClientSocket(CFIocpMgr* pIocpMgr);
        FTLINLINE virtual INT GetIocpHandleCount() const ;
        FTLINLINE virtual HANDLE GetIocpHandle( INT index ) const;
        FTLINLINE virtual BOOL OnInitialize( CFIocpMgr* pIocpMgr, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred );
        FTLINLINE virtual BOOL AfterReadCompleted( CFIocpMgr* pIocpMgr, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred );
        FTLINLINE virtual BOOL AfterWriteCompleted( CFIocpMgr* pIocpMgr, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred );
        FTLINLINE virtual BOOL OnUninitialize( CFIocpMgr* pIocpMgr, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred );
    };

    class CFIocpNetServer;

    class CFIocpListenTask : public CFIocpBaseTask
    {
    public:
        FTLINLINE CFIocpListenTask(CFIocpMgr* pIocpMgr, SOCKET socketListen);
        FTLINLINE ~CFIocpListenTask();

        FTLINLINE virtual INT GetIocpHandleCount() const;
        FTLINLINE virtual HANDLE GetIocpHandle( INT index ) const;

        FTLINLINE virtual BOOL OnInitialize(CFIocpMgr* pIocpMgr, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred );
        FTLINLINE virtual BOOL AfterReadCompleted( CFIocpMgr* pIocpMgr, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred );
        FTLINLINE virtual BOOL AfterWriteCompleted( CFIocpMgr* pIocpMgr, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred );
        FTLINLINE virtual BOOL OnUninitialize( CFIocpMgr* pIocpMgr, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred );
    public:
        SOCKET m_socketListen;
    };

    // 同时投递的Accept请求的数量(这个要根据实际的情况灵活设置)
#define MAX_POST_ACCEPT              1
    class CFIocpNetServer : public CFIocpMgr
    {
    public:
        FTLINLINE CFIocpNetServer();
        FTLINLINE ~CFIocpNetServer();
        FTLINLINE virtual BOOL OnIocpStart();
        FTLINLINE virtual BOOL OnIocpStop();
    protected:
        //SOCKET      m_nSocketListen;
        USHORT      m_nListenPort;
        CFIocpListenTask* m_pListenTask;
        // AcceptEx 和 GetAcceptExSockaddrs 的函数指针，用于调用这两个扩展函数
    public:
        LPFN_ACCEPTEX                m_lpfnAcceptEx;
        LPFN_GETACCEPTEXSOCKADDRS    m_lpfnGetAcceptExSockAddrs;

    //protected:
    public:
        FTLINLINE BOOL _PostAccept(CFIocpBuffer* pioBuffer);
        FTLINLINE BOOL _PostRecv(CFIocpBuffer* pioBuffer);
        FTLINLINE BOOL _PostSend(CFIocpBuffer* pioBuffer);
    };
}
#endif //FTL_SOCKET_H

#ifndef USE_EXPORT
#  include "ftlSocket.hpp"
#endif 