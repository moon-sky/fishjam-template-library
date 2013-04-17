#ifndef FTL_SOCKET_H
#define FTL_SOCKET_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlSocket.h requires ftlbase.h to be included first
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <ftlNet.h>

/*

怎么划分？
             CFClientSocket/CFServerSocket
            / 
CFSocketBase
            \CFTCPSocket/CFUDPSocket
*/

/*************************************************************************************************************************
*   
*************************************************************************************************************************/

namespace FTL
{
	class CFWinsockEnvInit
	{
	public:
		FTLINLINE CFWinsockEnvInit(WORD wVersionRequested = MAKEWORD(2, 2))
		{
			ZeroMemory(&m_wsaData, sizeof(m_wsaData));
			m_initResult = WSAStartup(wVersionRequested, &m_wsaData);

			//Success
			if (0 != m_initResult)
			{
				FTLTRACEEX(tlError, TEXT("WSAStartup Error: %d(%s)"), 
					m_initResult, CFNetErrorInfo(m_initResult).GetConvertedInfo());
			}
			FTLASSERT(m_initResult == 0);
			FTLASSERT(wVersionRequested == m_wsaData.wVersion);
		}
		FTLINLINE ~CFWinsockEnvInit()
		{
			if (0 == m_initResult)
			{
				WSACleanup();
			}
		}
	public:
		int		m_initResult;
		WSADATA m_wsaData;
	};

	//兼容 IPV4/IPV6 的地址管理类
	class CFSockAddrIn : public SOCKADDR_IN 
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

    FTLEXPORT template <typename T>
    class CFSocketT
    {
    public:
        FTLINLINE CFSocketT();
        FTLINLINE virtual ~CFSocketT();
		FTLINLINE BOOL IsOpen();
        FTLINLINE int Open(FSocketType st, BOOL bAsync);
        FTLINLINE int Close();
        FTLINLINE int Shutdown(INT how);

		//Connection
		FTLINLINE int Connect(const CFSockAddrIn& addr);// LPCTSTR pszAddr, INT nSocketPort);

		//Data
		FTLINLINE int Send(const BYTE* pBuf, INT len, DWORD flags);
		FTLINLINE int Recv(BYTE* pBuf, INT len, DWORD flags);

        FTLINLINE int IoCtl(long cmd, u_long* argp);
	protected:
		FTLINLINE virtual void OnOpen() {}
		FTLINLINE virtual void OnClose() {}
    protected:
        SOCKET              m_socket;
        FSocketType         m_socketType;
        BOOL                m_bASync;
        CFCriticalSection   m_lockObject;
        volatile UINT       m_nSession;
    };

    FTLEXPORT template <typename T>
    class CFClientSocketT : public CFSocketT<T>
    {
    public:
        FTLINLINE int Associate(SOCKET socket, PSOCKADDR_IN pForeignAddr);
		FTLINLINE SOCKADDR_IN& GetForeignAddr() { return m_foreignAddr; }
    protected:
        SOCKADDR_IN		m_foreignAddr;
    };

    FTLEXPORT template <typename T>
    class CFServerSocketT : public CFSocketT< T >
    {
    public:
        FTLINLINE CFServerSocketT();
        FTLINLINE ~CFServerSocketT();
        //Server
		FTLINLINE int Bind(USHORT listenPort, LPCTSTR pszBindAddr);
        FTLINLINE int StartListen(INT backlog, INT nMaxClients);
        FTLINLINE CFClientSocketT<T>* Accept();
		FTLINLINE void ReleaseClient(CFClientSocketT<T>* pClient);
	protected:
		FTLINLINE virtual void OnClose();
    protected:
        CFMemCacheT<CFClientSocketT <T> >*    m_pClientSocketPool;
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
        CFSocketT<T>*    m_pClientSocket;
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
        //CFSocketT<T>* Accept();
    protected:
        FSocketType                 m_socketType;
        CFServerSocketT<T>*         m_pServerSocket;

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

#if 0
    class CFSocketUtils
    {
    public:
        static FTLINLINE size_t readn(int fd, void* vptr, size_t n);
        static FTLINLINE size_t writen(int fd, const void* vptr, size_t n);
    };
#endif 
}
#endif //FTL_SOCKET_H

#ifndef USE_EXPORT
#  include "ftlSocket.hpp"
#endif 