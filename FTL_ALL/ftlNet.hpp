#ifndef FTL_NET_HPP
#define FTL_NET_HPP
#pragma once

#include "ftlBase.h"
#ifdef USE_EXPORT
#  include "ftlNet.h"
#endif

#include <WSipx.h>
#include <AtalkWsh.h>
#include <WS2atm.h>
#include <Af_irda.h>
#include <io.h>

#pragma comment(lib, "Wininet.lib")
//#include <WinSock2.h>

#pragma warning( push )
#pragma warning(disable: 4996) //C4996: 'read': The POSIX name for this item is deprecated.

namespace FTL
{
    CFNetErrorInfo::CFNetErrorInfo(int err) : CFConvertInfoT<CFNetErrorInfo,int>(err)
    {

    }

    LPCTSTR CFNetErrorInfo::ConvertInfo()
    {
        if (NULL == m_bufInfo[0])
        {
            switch (m_Info)
            {
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAEINTR);  //10004
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAEACCES); //10013
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAEFAULT); //10014
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAEINVAL); //10022
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAEMFILE); //10024
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAEWOULDBLOCK);  //10035
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAEINPROGRESS);  //10036
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAEALREADY); //10037
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAENOTSOCK); //10038
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAEDESTADDRREQ); //10039
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAEMSGSIZE);  //10040
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAEPROTOTYPE);  //10041
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAENOPROTOOPT);  //10042
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAEPROTONOSUPPORT);  //10043
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAESOCKTNOSUPPORT);  //10044
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAEOPNOTSUPP);  //10045
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAEPFNOSUPPORT);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAEAFNOSUPPORT);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAEADDRINUSE);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAEADDRNOTAVAIL);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAENETDOWN);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAENETUNREACH);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAENETRESET);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAECONNABORTED);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAENOBUFS);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAEISCONN);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAENOTCONN);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAESHUTDOWN);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAETIMEDOUT);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAECONNREFUSED);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAEHOSTDOWN);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAEHOSTUNREACH);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAEPROCLIM);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSASYSNOTREADY);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAVERNOTSUPPORTED);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSANOTINITIALISED);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAEDISCON);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSATYPE_NOT_FOUND);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAHOST_NOT_FOUND);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSATRY_AGAIN);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSANO_RECOVERY);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSANO_DATA);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSA_INVALID_HANDLE);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSA_INVALID_PARAMETER);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSA_IO_INCOMPLETE);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSA_IO_PENDING);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSA_NOT_ENOUGH_MEMORY);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSA_OPERATION_ABORTED);
                //HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAINVALIDPROCTABLE);
                //HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAINVALIDPROVIDER);
                //HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSAPROVIDERFAILEDINIT);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), WSASYSCALLFAILURE);
            default:
                CFAPIErrorInfo errorInfo(m_Info);
                StringCchCopy(m_bufInfo, _countof(m_bufInfo), errorInfo.GetConvertedInfo());
                break;
            }
        }
        return m_bufInfo;
    }

    namespace NetInfo
    {
        LPCTSTR GetAddressFamily(int iAddressFamily)
        {
            switch(iAddressFamily)
            {
                HANDLE_CASE_RETURN_STRING( AF_UNIX );
                HANDLE_CASE_RETURN_STRING( AF_INET );
                HANDLE_CASE_RETURN_STRING( AF_IMPLINK );
                HANDLE_CASE_RETURN_STRING( AF_PUP );
                HANDLE_CASE_RETURN_STRING( AF_CHAOS );
                HANDLE_CASE_RETURN_STRING( AF_NS ); //也是 AF_IPX
                HANDLE_CASE_RETURN_STRING( AF_ISO );//也是 AF_OSI
                HANDLE_CASE_RETURN_STRING( AF_ECMA );
                HANDLE_CASE_RETURN_STRING( AF_DATAKIT );
                HANDLE_CASE_RETURN_STRING( AF_CCITT );
                HANDLE_CASE_RETURN_STRING( AF_SNA );
                HANDLE_CASE_RETURN_STRING( AF_DECnet );
                HANDLE_CASE_RETURN_STRING( AF_DLI );
                HANDLE_CASE_RETURN_STRING( AF_LAT );
                HANDLE_CASE_RETURN_STRING( AF_HYLINK );
                HANDLE_CASE_RETURN_STRING( AF_APPLETALK );
                HANDLE_CASE_RETURN_STRING( AF_NETBIOS );
                HANDLE_CASE_RETURN_STRING( AF_VOICEVIEW );
                HANDLE_CASE_RETURN_STRING( AF_FIREFOX );
                HANDLE_CASE_RETURN_STRING( AF_UNKNOWN1 );
                HANDLE_CASE_RETURN_STRING( AF_BAN );
                HANDLE_CASE_RETURN_STRING( AF_ATM );
                HANDLE_CASE_RETURN_STRING( AF_INET6 );
                HANDLE_CASE_RETURN_STRING( AF_CLUSTER );
                HANDLE_CASE_RETURN_STRING( AF_12844 );
                HANDLE_CASE_RETURN_STRING( AF_IRDA );
                HANDLE_CASE_RETURN_STRING( AF_NETDES );
                HANDLE_CASE_RETURN_STRING( AF_TCNPROCESS );
                HANDLE_CASE_RETURN_STRING( AF_TCNMESSAGE );
                HANDLE_CASE_RETURN_STRING( AF_ICLFXBM );
                HANDLE_CASE_RETURN_STRING( AF_MAX );
            default:
                FTLASSERT(FALSE);
                break;
            }
            return _T("UnknowFamily");
        }

        LPCTSTR GetProtocolType(int iAddressFamily,int iProtocol)
        {
            switch(iAddressFamily)
            {
            case AF_INET:
            case AF_INET6:
                switch(iProtocol)
                {
                    //AF_INET/AF_INET6 家族的协议
                    HANDLE_CASE_RETURN_STRING( IPPROTO_IP ); //也是 IPPROTO_HOPOPTS
                    HANDLE_CASE_RETURN_STRING( IPPROTO_ICMP );
                    HANDLE_CASE_RETURN_STRING( IPPROTO_IGMP );
                    HANDLE_CASE_RETURN_STRING( IPPROTO_GGP );
                    HANDLE_CASE_RETURN_STRING( IPPROTO_IPV4 );
                    HANDLE_CASE_RETURN_STRING( IPPROTO_TCP );
                    HANDLE_CASE_RETURN_STRING( IPPROTO_PUP );
                    HANDLE_CASE_RETURN_STRING( IPPROTO_UDP );
                    HANDLE_CASE_RETURN_STRING( IPPROTO_IDP );
                    HANDLE_CASE_RETURN_STRING( IPPROTO_IPV6 );
                    HANDLE_CASE_RETURN_STRING( IPPROTO_ROUTING );
                    HANDLE_CASE_RETURN_STRING( IPPROTO_FRAGMENT );
                    HANDLE_CASE_RETURN_STRING( IPPROTO_ESP );
                    HANDLE_CASE_RETURN_STRING( IPPROTO_AH );
                    HANDLE_CASE_RETURN_STRING( IPPROTO_ICMPV6 );
                    HANDLE_CASE_RETURN_STRING( IPPROTO_NONE );
                    HANDLE_CASE_RETURN_STRING( IPPROTO_DSTOPTS );
                    HANDLE_CASE_RETURN_STRING( IPPROTO_ND );
                    HANDLE_CASE_RETURN_STRING( IPPROTO_ICLFXBM );
                    HANDLE_CASE_RETURN_STRING( IPPROTO_RAW );
                    HANDLE_CASE_RETURN_STRING( IPPROTO_MAX );
                }
                break;
            case AF_NS:
                switch(iProtocol)
                {
                    //AF_NS 家族的协议
                    HANDLE_CASE_RETURN_STRING( NSPROTO_IPX);
                    HANDLE_CASE_RETURN_STRING( NSPROTO_SPX);
                    HANDLE_CASE_RETURN_STRING( NSPROTO_SPXII);
                }
                break;
            case AF_APPLETALK:
                switch(iProtocol)
                {
                    //AF_APPLETALK 家族的协议
                    HANDLE_CASE_RETURN_STRING( DDPPROTO_RTMP);
                    HANDLE_CASE_RETURN_STRING( DDPPROTO_NBP);
                    HANDLE_CASE_RETURN_STRING( DDPPROTO_ATP);
                    HANDLE_CASE_RETURN_STRING( DDPPROTO_AEP);
                    HANDLE_CASE_RETURN_STRING( DDPPROTO_RTMPRQ);
                    HANDLE_CASE_RETURN_STRING( DDPPROTO_ZIP);
                    HANDLE_CASE_RETURN_STRING( DDPPROTO_ADSP);
                    HANDLE_CASE_RETURN_STRING( ATPROTO_ADSP);
                    HANDLE_CASE_RETURN_STRING( ATPROTO_ATP );
                    HANDLE_CASE_RETURN_STRING( ATPROTO_ASP );
                    HANDLE_CASE_RETURN_STRING( ATPROTO_PAP );
                }
                break;
            case AF_ATM:
                switch(iProtocol)
                {
                    //AF_ATM 家族的协议
                    HANDLE_CASE_RETURN_STRING( ATMPROTO_AALUSER );
                    HANDLE_CASE_RETURN_STRING( ATMPROTO_AAL1 );
                    HANDLE_CASE_RETURN_STRING( ATMPROTO_AAL2 );
                    HANDLE_CASE_RETURN_STRING( ATMPROTO_AAL34 );
                    HANDLE_CASE_RETURN_STRING( ATMPROTO_AAL5 );
                }
                break;
            case AF_IRDA:
                switch(iProtocol)
                {
                    //AF_IRDA 家族的协议
                    HANDLE_CASE_RETURN_STRING( IRDA_PROTO_SOCK_STREAM );
                }
            default:
                FTLASSERT(FALSE);
                break;
            }
            return _T("UnknowProtocolType");
        }

        LPCTSTR GetSocketType(int iSocketType)
        {
            switch(iSocketType)
            {
                HANDLE_CASE_RETURN_STRING( SOCK_STREAM );
                HANDLE_CASE_RETURN_STRING( SOCK_DGRAM );
                HANDLE_CASE_RETURN_STRING( SOCK_RAW );
                HANDLE_CASE_RETURN_STRING( SOCK_RDM );
                HANDLE_CASE_RETURN_STRING( SOCK_SEQPACKET );
            default:
                FTLASSERT(FALSE);
                break;
            }
            return _T("UnknowSocketType");
        }

        //LPCTSTR GetServiceFlagsType(DWORD dwServiceFlags);
        //LPCTSTR GetProviderFlagsType(DWORD dwProviderFlags);

        LONG GetLocalIPAddress()
        {
            LONG localIP = 0;
            char host[MAX_BUFFER_LENGTH];
            ::gethostname(host, sizeof(host));
            hostent	*ent = ::gethostbyname(host);
            if (ent)
            {
                localIP = *(ULONG *)ent->h_addr_list[0];
            }
            //后续可以使用 inet_ntoa 等函数转换为 IP 的字符串
            return localIP;
        }
    }//namespace .NetInfo 


    //////////////////////////////////////////////////////////////////////////
    template<typename T>
    CFSocketT<T>::CFSocketT()
    {
        m_socket = INVALID_SOCKET;
        m_socketType = stTCP;
        m_bASync = TRUE;
        m_nSession = (UINT)(-1);
    }

    template<typename T>
    CFSocketT<T>::~CFSocketT()
    {
        FTLASSERT(INVALID_SOCKET == m_socket && TEXT("Forget Close The Socket"));
        Close();
    }

    template<typename T>
    int CFSocketT<T>::Open(FSocketType st, BOOL bAsync)
    {
        FTLASSERT(INVALID_SOCKET == m_socket && TEXT("Can not Create Socket Twice"));

        int rc = NO_ERROR;
        m_socketType = st;
        m_bASync = bAsync;
        DWORD dwFlags = m_bASync ? WSA_FLAG_OVERLAPPED : 0;
        switch(m_socketType)
        {
        case stTCP:
            m_socket = WSASocket(AF_INET, SOCK_STREAM, 0/*IPPROTO_TCP*/, NULL, 0, dwFlags);
            break;
        case stUDP:
            m_socket = WSASocket(AF_INET, SOCK_DGRAM, 0/*IPPROTO_UDP*/, NULL, 0, dwFlags);
            break;
        default:
            FTLASSERT(FALSE);
            break;
        }
        if (INVALID_SOCKET == m_socket)
        {
            NET_VERIFY(SOCKET_ERROR);
            rc = SOCKET_ERROR;
        }
        return rc;
    }
    
    template<typename T>
    int CFSocketT<T>::Close()
    {
        int rc = NO_ERROR;
        SAFE_CLOSE_SOCKET(m_socket);
        //if (INVALID_SOCKET != m_socket)
        //{ 
        //    NET_VERIFY(closesocket(m_socket));
        //    m_socket = INVALID_SOCKET;
        //}
        m_nSession = (UINT)(-1);
        return rc;
    }

    template<typename T>
    int CFSocketT<T>::Shutdown(INT how)
    {
        FTLASSERT(INVALID_SOCKET != m_socket);
        int rc = NO_ERROR;
        rc = shutdown(m_socket, how);
        return rc;
    }

    //////////////////////////////////////////////////////////////////////////
    template<typename T>
    int CFClientSocketT<T>::Connect(LPCTSTR pszAddr, INT nSocketPort)
    {
        FTLASSERT(pszAddr);
        FTLASSERT(nSocketPort);

        int rc = NO_ERROR;
        int nLength = sizeof(sockaddr_in);
        sockaddr_in addrConnect = {0};

        addrConnect.sin_family = AF_INET;
        addrConnect.sin_port = htons(nSocketPort);
        addrConnect.sin_addr.S_un.S_addr = inet_addr(CT2A(pszAddr));

        NET_VERIFY(WSAConnect(m_socket,(sockaddr*)&addrConnect,nLength,NULL,NULL,NULL,NULL));

        return rc;
    }

    template<typename T>
    int CFClientSocketT<T>::Send(const BYTE* pBuf, INT len, DWORD flags)
    {
        int rc = NO_ERROR;
        WSABUF sendBuf = {0};
        sendBuf.len = (ULONG)len;
        sendBuf.buf = (char*)pBuf;
        DWORD dwNumberOfSend = 0;
        NET_VERIFY(WSASend(m_socket,&sendBuf, 1,&dwNumberOfSend,flags, NULL, NULL));
        return rc;
    }

    template<typename T>
    int CFClientSocketT<T>::Recv(BYTE* pBuf, INT len, DWORD flags)
    {
        int rc = NO_ERROR;

        if (m_socketType == stUDP)
        {
            //WSAEMSGSIZE
        }

        WSABUF recvBuf = {0};
        recvBuf.len = len;
        recvBuf.buf = (char*)pBuf;
        DWORD dwNumberOfRecv = 0;

        NET_VERIFY(WSARecv(m_socket,&recvBuf, 1, &dwNumberOfRecv, &flags ,NULL,NULL));
        return rc;
    }

    template<typename T>
    int CFClientSocketT<T>::Associate(SOCKET socket, struct sockaddr_in *pForeignAddr)
    {
        FTLASSERT(m_socket == INVALID_SOCKET);
        int rc = NO_ERROR;
        this->m_socket = socket;
        memcpy(&m_foreignAddr, pForeignAddr, sizeof(struct sockaddr_in));
        return rc;
    }

    //////////////////////////////////////////////////////////////////////////
    template<typename T>
    CFServerSocketT<T>::CFServerSocketT()
    {
        m_pClientSocketPool = NULL;
    }

    template<typename T>
    CFServerSocketT<T>::~CFServerSocketT()
    {
        SAFE_DELETE(m_pClientSocketPool);
    }

    template<typename T>
    int CFServerSocketT<T>::StartListen(INT backlog, INT nMaxClients)
    {
        int rc = NO_ERROR;
        m_pClientSocketPool = new CFMemCacheT<CFClientSocketT <T> >(0,nMaxClients);
        
        FTLASSERT( m_socket != INVALID_SOCKET );
        NET_VERIFY(listen(m_socket, backlog));
        return rc;
    }

    template<typename T>
    int CFServerSocketT<T>::Bind(USHORT listenPort, LPCTSTR pszBindAddr  = NULL )
    {
        FTLASSERT(m_socket != INVALID_SOCKET);
        int rc = NO_ERROR;
        SOCKADDR_IN bindAddr_in = {0};
        bindAddr_in.sin_family = AF_INET;
        bindAddr_in.sin_port = htons(listenPort);

        if (pszBindAddr)
        {
            bindAddr_in.sin_addr.S_un.S_addr = inet_addr(CT2A(pszBindAddr));
        }
        else
        {
            bindAddr_in.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
        }
        
        NET_VERIFY(bind(m_socket,(SOCKADDR*)&bindAddr_in, sizeof(bindAddr_in)));
        return rc;
    }

    template<typename T>
    CFClientSocketT<T>*  CFServerSocketT<T>::Accept()
    {
        SOCKET soAccept = INVALID_SOCKET;
        struct sockaddr_in addrClient = {0};
        int nLength = sizeof(struct sockaddr_in);
        CFClientSocketT<T> *pRetClient = NULL;

        // Check if there is something in the listen queue.
        soAccept = WSAAccept(m_socket, (struct sockaddr *) &addrClient, 
            &nLength, NULL, NULL);

        // Check for errors.
        if (soAccept != INVALID_SOCKET) 
        {
            // Get a pointer to the free ClientSocket 
            // instance from the pool.
            pRetClient = m_pClientSocketPool->Get();
            if (pRetClient == NULL) 
            {
                // There are no free instances in the pool, maximum 
                // number of accepted client connections is reached.
                ::shutdown(soAccept,2);
                ::closesocket(soAccept);
            }
            else 
            {
                // Everything is fine, so associate the instance 
                // with the client socket.
                pRetClient->Associate(soAccept,&addrClient);
            }
        }
        
        return pRetClient;
    }

    //////////////////////////////////////////////////////////////////////////
    ///                     CFNetClientT                                   ///
    //////////////////////////////////////////////////////////////////////////

    template< typename T >
    CFNetClientT<T>::CFNetClientT(FSocketType st)
    {
        m_pClientSocket = NULL;
        m_socketType = st;
    }

    template< typename T >
    CFNetClientT<T>::~CFNetClientT()
    {

    }

    template< typename T >
    int CFNetClientT<T>::Create()
    {
        int rc = NO_ERROR;
        m_pClientSocket = new CFSocket(m_socketType);
        m_pClientSocket->Open();
    }

    template< typename T >
    int CFNetClientT<T>::Destroy()
    {

    }

    template< typename T >
    int CFNetClientT<T>::Connect(LPCTSTR pszAddr)
    {
        int rc = NO_ERROR;
        SOCKADDR_IN conAddr = {0};
        conAddr.sin_family = AF_INET;
        conAddr.sin_port = 0;
        conAddr.sin_addr.S_un.S_addr = 0; 

        //WSAECONNREFUSED -- 想连接的计算机没有监听指定端口
        //WSAETIMEDOUT -- 
        NET_VERIFY(WSAConnect(m_pClientSocket->m_socket,SOCKADDR*(&conAddr), sizeof(conAddr),NULL,NULL,NULL,NULL));
        return rc;
    }

    
    //////////////////////////////////////////////////////////////////////////
    ///                     CFNetServerT                                   ///
    //////////////////////////////////////////////////////////////////////////

    template< typename T >
    DWORD CFNetServerT<T>::getNumberOfConcurrentThreads()
    {
        //用于 CreateIoCompletionPort 的参数和创建线程。
        //0表示系统内安装了多少个处理器，便允许同时运行多少个线程操作IO完成端口,但通常返回 CPU个数 * 2 + 2;
        SYSTEM_INFO sysInfo = {0};
        GetSystemInfo(&sysInfo);

#pragma TODO(超线程或多核时获取的线程个数)
        DWORD numberOfConcurrentThreads = sysInfo.dwNumberOfProcessors * 2 + 2;
        return numberOfConcurrentThreads;
    }

    template< typename T >
    unsigned int CFNetServerT<T>::serverThreadProc( LPVOID lpThreadParameter )
    {
        FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
        unsigned int ret = 0;
        CFNetServerT<T>* pThis = static_cast< CFNetServerT<T>* >(lpThreadParameter);
        ret = pThis->doServerLoop();
        return ret;
    }

    template< typename T >
    unsigned int CFNetServerT<T>::doServerLoop()
    {
        FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
        //int rc = 0;

        //将Accept的客户端Socket绑定到IO完成端口，返回的将是之前创建的IO端口，而不创建新的
        //SOCKET acceptClient = 0;
        
        for (;;)
        {
            CFClientSocketT<T>* pClient = m_pServerSocket->Accept();
            if (pClient)
            {
                HANDLE hbindIoPort = CreateIoCompletionPort((HANDLE)(pClient->m_socket),m_hIoCompletionPort,
                    (ULONG_PTR)(pClient), 0);
                FTLASSERT( hbindIoPort == m_hIoCompletionPort);
                //::PostQueuedCompletionStatus(m_hIoCompletionPort,1,pClient,)
            }
            //FOVERLAPPED*     pKey = new FOVERLAPPED();
            //pKey->socket = acceptClient;
            //pKey->overLapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

            //(m_pServerSocket->Accept())
            //break;
        }
        return 0;
    }

    template< typename T >
    unsigned int CFNetServerT<T>::workerThreadProc( LPVOID lpThreadParameter )
    {
        unsigned int ret = 0;
        CFNetServerT<T>* pThis = static_cast< CFNetServerT<T>* >(lpThreadParameter);
        ret = pThis->doWorkerLoop();
        return ret;
    }

    template< typename T >
    unsigned int CFNetServerT<T>::doWorkerLoop()
    {
        DWORD dwNumberOfBytesTransferred = 0;
        CFClientSocketT<T>* pClientSocket;//用于获得第二次CreateIoCompletionPort时传入的参数
        LPFOVERLAPPED    pOverLapped = NULL;
        BOOL bRet = FALSE;
        for (;;)
        {
            //分三种情况：
            //1.成功取出一个Completion Packet，返回TRUE，并填写各个参数;
            //2.返回FALSE，但取出Completion Packet，pOverLapped指向失败的操作，具体GetLastError
            //3.返回FALSE，并且没有取出 Completion Packet, pOverLapped 为NULL
            API_VERIFY(GetQueuedCompletionStatus(m_hIoCompletionPort,&dwNumberOfBytesTransferred,
                (PULONG_PTR)&pClientSocket, (LPOVERLAPPED*)(&pOverLapped), INFINITE));

            if (FALSE == bRet)
            {
                if (NULL == pOverLapped)
                {
                    break;
                }
                else
                {
                    //Happend instead of end-of-file
                }
            }
            else if(0 == dwNumberOfBytesTransferred)
            {
                //socket close
            }
            else
            {
                //FTLTRACEEX(FTL::tlInfo, TEXT("ClientSocket[%d,Session=%d],OverLapped[mode=%d,len=%d,dataBuf=%s]\n"),
                //    pClientSocket->m_socket,pClientSocket->m_nSession, 
                //    pOverLapped->socketMode,pOverLapped->dataBuf.len, (TCHAR*)(pOverLapped->dataBuf.buf));
                //Got a valid data block
            }
        }

        return 0;
    }

    template< typename T >
    CFNetServerT<T>::CFNetServerT(FSocketType st)
    {
        m_socketType = st;
        m_pServerSocket = NULL;
        m_hIoCompletionPort = NULL;
        //m_pIoServerThreadPool = NULL;
        //m_pServerThread = NULL;
    }

    template< typename T >
    CFNetServerT<T>::~CFNetServerT()
    {
        Destroy();
    }

    template< typename T >
    int CFNetServerT<T>::Create(USHORT listenPort, LPCTSTR pszBindAddr /* = NULL */)
    {
        FTLASSERT(NULL == m_pServerSocket);
        //if (NULL != m_pServerSocket)
        //{
        //    Destroy(); 
        //}
        int rc = NO_ERROR;
        //BOOL bRet = FALSE;
        m_pServerSocket = new CFServerSocketT<T>();
        NET_VERIFY(m_pServerSocket->Open(m_socketType, TRUE));

        do 
        {
            //int nZero = 0;
            //Winsock便会在重叠I/O调用中直接使用应用程序的缓冲，避免拷贝
            //setsockopt(m_pServerSocket->m_socket,SOL_SOCKET, SO_SNDBUF,(char*)&nZero, sizeof(nZero));

            if (stTCP == m_socketType)
            {
                NET_VERIFY(m_pServerSocket->Bind(listenPort,pszBindAddr));
                if (NO_ERROR != rc)
                {
                    break;
                }
            }


            //2.使用线程池创建为IO完成端口服务的工作者线程
            //m_pIoServerThreadPool = new CFThreadPool<DWORD>(0,0);

            //3.将Accept后的套接字句柄同完成端口关联到一起
            //CreateIoCompletionPort((HANLDE)acceptSocket,m_hIoCompletionPort,(ULONG_PTR)this,0);


            //4.使用Overlapped方式进行IO处理，并依赖于完成端口，进行异步
            //工作者线程中在完成端口上等待 GetQueuedCompletionStatus(m_hIoCompletionPort,)

            //m_pServerThread = new CFThread<DefaultThreadTraits>();
            //FTLASSERT(m_pServerThread);

            //if (m_pServerThread)
            //{
            //    m_pServerThread->Start((LPTHREAD_START_ROUTINE)(&CFNetServerT<T>::ServerThreadProc), this, TRUE);
            //}
        } while (FALSE);
        
        if (NO_ERROR != rc)
        {
            Destroy();
        }
        return rc;
    }

    template< typename T >
    int CFNetServerT<T>::Destroy()
    {
        int rc = NO_ERROR;
        BOOL bRet = FALSE;
        //在完成端口上， 向每个工作者线程都发送一个特殊的完成数据包,终止所有工作者线程的运行
        //PostQueuedCompletionStatus(m_hIoCompletionPort)
        SAFE_CLOSE_HANDLE(m_hIoCompletionPort, NULL);

        //if (m_pServerThread)
        //{
        //    m_pServerThread->StopAndWait(INFINITE,TRUE);
        //    SAFE_DELETE(m_pServerThread);
        //}
        if (m_pServerSocket)
        {
            NET_VERIFY(m_pServerSocket->Close());
            SAFE_DELETE(m_pServerSocket);
        }
        return rc;
    }


    template< typename T >
    int CFNetServerT<T>::Start(INT backlog, INT nMaxClients)
    {
        int rc = NO_ERROR;
        BOOL bRet = FALSE;



        DWORD dwNumberOfConcurrentThreads = getNumberOfConcurrentThreads();
        FTLASSERT(dwNumberOfConcurrentThreads > 0);

        //1.创建IO完成端口
        m_hIoCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 
            NULL, 
            0, 
            dwNumberOfConcurrentThreads
            );
        API_VERIFY(NULL != m_hIoCompletionPort);

        m_pWorkerThreads = new HANDLE[dwNumberOfConcurrentThreads];
        ZeroMemory(m_pWorkerThreads, sizeof(HANDLE) * dwNumberOfConcurrentThreads );
        unsigned int threadId = 0;

        for (DWORD threadIndex = 0; threadIndex < dwNumberOfConcurrentThreads; threadIndex++)
        {
            m_pWorkerThreads[threadIndex] = (HANDLE)_beginthreadex(NULL, 0, workerThreadProc, this, 0, &threadId);
        }

        m_hServerThread = (HANDLE) _beginthreadex(NULL,0,serverThreadProc, this, 0, &threadId);
        
        NET_VERIFY(m_pServerSocket->StartListen(backlog,nMaxClients));
        return rc;
    }

    template< typename T >
    int CFNetServerT<T>::Stop()
    {
        int rc = NO_ERROR;
        BOOL bRet = FALSE;
        if (m_pWorkerThreads)
        {
            DWORD dwNumberOfConcurrentThreads = getNumberOfConcurrentThreads();
            for (DWORD threadIndex = 0; threadIndex < dwNumberOfConcurrentThreads; threadIndex++)
            {
                SAFE_CLOSE_HANDLE(m_pWorkerThreads[threadIndex], NULL);
            }
            SAFE_DELETE_ARRAY(m_pWorkerThreads);
        }
        SAFE_CLOSE_HANDLE(m_hServerThread, NULL);
        return rc;
    }

    size_t CFSocketUtils::readn(int fd, void* vptr, size_t n)
    {
        size_t nleft = n;
        size_t nread = 0;
        char * ptr = (char*)vptr;
        while(nleft > 0)
        {
            if((nread = read(fd,ptr,nleft)) < 0)
            {
                if(EINTR == errno)
                {
                    nread = 0; //call read again
                }
                else
                {
                    return (size_t)(-1);
                }
            }
            else if(0 == nread)
            {
                break;   //EOF
            }
            nleft -= nread;
            ptr += nread;
        }
        return (n-nleft); 
    }

    size_t CFSocketUtils::writen(int fd, const void* vptr, size_t n)
    {
        size_t nleft = n;
        size_t nwritten = 0;
        const char* ptr = (const char*)vptr;
        while (nleft > 0)
        {
            if((nwritten = write(fd, ptr, nleft)) <= 0)
            {
                if(EINTR == errno)
                {
                    nwritten = 0; //call write again-- reset了 ?
                }
                else
                {
                    return (size_t)(-1);
                }
            }
            nleft -= nwritten;
            ptr += nwritten;
        } 
        return n;
    }


	LPCTSTR CFNetUtil::GetCookieInfo(CFStringFormater& formater, LPCTSTR lpszUrl, LPCTSTR lpszCookieName)
	{
		BOOL bRet = FALSE;
		DWORD dwLength = 0;

		//InternetGetCookie只读取COOKIES目录下生成的COOKIE，遇到HTTPONLY属性则不生成本地COOKIES中的文件，而是直接通过HTTP头来传输
		//有生存周期的保存在磁盘，否则保存在内存
		API_VERIFY(InternetGetCookie(lpszUrl, lpszCookieName, NULL, &dwLength));
		if (bRet)
		{
			dwLength += 1;
			CFMemAllocator<TCHAR>	memAlloc(dwLength);
			API_VERIFY(InternetGetCookie(lpszUrl, lpszCookieName, (TCHAR*)memAlloc, &dwLength));
			if (bRet)
			{
				formater.Format(TEXT("%s"), (TCHAR*)memAlloc);
			}
		}
		return formater.GetString();
	}

	DWORD CFNetUtil::GetCookieInfoMap(LPCTSTR pszCookies, CookiKeyValueMap& cookieMap)
	{
		return 0;
	}

	CUrlComponents::CUrlComponents()
	{
		this->dwStructSize = sizeof(URL_COMPONENTS);

		this->lpszScheme = m_szScheme;
		this->dwSchemeLength = _countof(m_szScheme);
		this->nScheme = INTERNET_SCHEME_UNKNOWN;
		this->lpszHostName = m_szHostName;
		this->dwHostNameLength = _countof(m_szHostName);
		this->lpszUserName = m_szUserName;
		this->dwUserNameLength = _countof(m_szUserName);
		this->lpszPassword = m_szPassword;
		this->dwPasswordLength = _countof(m_szPassword);
		this->lpszUrlPath = m_szUrlPath;
		this->dwUrlPathLength = _countof(m_szUrlPath);
		this->lpszExtraInfo = m_szExtraInfo;
		this->dwExtraInfoLength = _countof(m_szExtraInfo);

		ZeroMemory(m_szScheme, _countof(m_szScheme));
		ZeroMemory(m_szHostName, _countof(m_szHostName));
		ZeroMemory(m_szUserName, _countof(m_szUserName));
		ZeroMemory(m_szPassword, _countof(m_szPassword));
		ZeroMemory(m_szUrlPath, _countof(m_szUrlPath));
		ZeroMemory(m_szExtraInfo, _countof(m_szExtraInfo));
	}

	BOOL CUrlComponents::ParseUrl( LPCTSTR pstrURL, DWORD& dwServiceType, WORD& nPort, DWORD dwFlags )
	{
		//未测试 -- 用 WinHttpCrackUrl 有什么问题？
		BOOL bRet = FALSE;
		LPTSTR pstrCanonicalizedURL = NULL;
		TCHAR szCanonicalizedURL[INTERNET_MAX_URL_LENGTH] = {0};
		DWORD dwNeededLength = INTERNET_MAX_URL_LENGTH;
		
		// Decoding is done in InternetCrackUrl/UrlUnescape
		// so we don't need the ICU_DECODE flag here.

		DWORD dwCanonicalizeFlags = dwFlags & ( ICU_NO_ENCODE | ICU_NO_META | ICU_ENCODE_SPACES_ONLY | ICU_BROWSER_MODE );

		DWORD dwCrackFlags = 0;

		BOOL bUnescape = FALSE;

		if ( ( dwFlags & ( ICU_ESCAPE | ICU_DECODE ) ) && ( dwUrlPathLength != 0 ) )
		{
			// We use only the ICU_ESCAPE flag for decoding even if
			// ICU_DECODE is passed.
			// Also, if ICU_BROWSER_MODE is passed we do the unescaping
			// manually because InternetCrackUrl doesn't do
			// Browser mode unescaping
			if ( dwFlags & ICU_BROWSER_MODE )
			{
				bUnescape = TRUE;
			}
			else
			{
				dwCrackFlags = ICU_ESCAPE;
			}
		}
		API_VERIFY(InternetCanonicalizeUrl( pstrURL, szCanonicalizedURL,
			&dwNeededLength, dwCanonicalizeFlags ));
		pstrCanonicalizedURL = szCanonicalizedURL;

		API_VERIFY( InternetCrackUrl( pstrCanonicalizedURL, 0, dwCrackFlags, this ));
		API_VERIFY(UrlUnescape( lpszUrlPath, NULL, NULL, URL_UNESCAPE_INPLACE | URL_DONT_UNESCAPE_EXTRA_INFO ));

		return bRet;
	}
	//////////////////////////////////////////////////////////////////////////
#if 0
	CFGenericHTTPClient::CFGenericHTTPClient()
	{
		m_hOpen = NULL;
		m_hConnection = NULL;
		m_hRequest=NULL;

		//ZeroMemory(m_szHTTPResponseHeader, sizeof(m_szHTTPResponseHeader));
		//ZeroMemory(m_szHTTPResponseHTML, sizeof(m_szHTTPResponseHTML));

		m_dwError = 0;
		m_dwResultSize = 0;
		//m_szHost = NULL;
		//m_dwPort = 0;
	}

	CFGenericHTTPClient::~CFGenericHTTPClient()
	{
		Close();
	}

	BOOL CFGenericHTTPClient::Connect(
		LPCTSTR szAddress, 
		WORD nPort /* = INTERNET_DEFAULT_HTTP_PORT */, 
		LPCTSTR szAgent /* = NULL */,
		LPCTSTR pszCookie /* = NULL */, 
		LPCTSTR szUserAccount /* = NULL */, 
		LPCTSTR szPassword /* = NULL */)
	{
		BOOL bRet = FALSE;

		m_hOpen =::InternetOpen(szAgent,	// agent name
			INTERNET_OPEN_TYPE_PRECONFIG,	// proxy option
			NULL,   // proxy
			NULL,	// proxy bypass
			0);		// flags

		API_ASSERT(m_hOpen);

		if(!m_hOpen)
		{
			m_dwError=::GetLastError();
			return FALSE;
		}	

		m_hConnection=::InternetConnect(m_hOpen,	// internet opened handle
			szAddress, // server name
			nPort, // ports
			szUserAccount, // user name
			szPassword, // password 
			INTERNET_SERVICE_HTTP, // service type
			0, //	INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_NO_CACHE_WRITE,	// service option
			0);	// context call-back option

		API_ASSERT(m_hConnection);
		if(!m_hConnection){		
			m_dwError=::GetLastError();
			SAFE_CLOSE_INTERNET_HANDLE(m_hOpen);
			return FALSE;
		}
#if 0
		if(::InternetAttemptConnect(NULL)!=ERROR_SUCCESS){		
			API_ASSERT(FALSE);
			m_dwError=::GetLastError();
			SAFE_CLOSE_INTERNET_HANDLE(m_hConnection);
			SAFE_CLOSE_INTERNET_HANDLE(m_hOpen);
			return FALSE;
		}
#endif 

#if 1
		if ( pszCookie != NULL && lstrcmp ( pszCookie, _T ( "INVALID_COOKIE" ) ) != 0 )
		{
			CAtlString szURL = _T ( "http://" );
			szURL += szAddress;
			API_VERIFY(::InternetSetCookie ( szURL, NULL, pszCookie ));
		}
#endif 
		return TRUE;															
	}

	BOOL CFGenericHTTPClient::Close()
	{
		BOOL bRet = FALSE;
		SAFE_CLOSE_INTERNET_HANDLE(m_hRequest);
		SAFE_CLOSE_INTERNET_HANDLE(m_hConnection);
		SAFE_CLOSE_INTERNET_HANDLE(m_hOpen);

		return bRet;
	}

	BOOL CFGenericHTTPClient::Request(LPCTSTR szURL, 
		LPCTSTR szAgent, 
		RequestMethod nMethod)
	{

		BOOL bReturn=TRUE;
		WORD wPort=0;
		TCHAR szProtocol[__SIZE_BUFFER]= { 0 };
		TCHAR szAddress[__SIZE_BUFFER]= { 0 };	
		TCHAR szURI[__SIZE_BUFFER]= {0};
		//DWORD dwSize=0;
		m_dwResultSize = 0;

		//ParseURL(szURL, szProtocol, szAddress, wPort, szURI);

		if(Connect(szURL, wPort, szAgent ))
		{
			//if(!RequestOfURI(szURI, nMethod))
			//{
			//	bReturn=FALSE;
			//}
			//else
			//{
			//	if (	!Response (
			//		( PWORD ) _szHTTPResponseHeader, __SIZE_HTTP_BUFFER,
			//		( PWORD ) _szHTTPResponseHTML, __SIZE_HTTP_BUFFER, _dwResultSize
			//		) )
			//	{
			//		bReturn=FALSE;		
			//	}
			//}
			Close();
		}
		else
		{
			bReturn=FALSE;
		}

		return bReturn;
	}


	BOOL CFGenericHTTPClient::RequestGet(LPCTSTR szURI)
	{
		BOOL bRet = FALSE;

		
		CONST TCHAR *szAcceptType=__HTTP_ACCEPT_TYPE;
		const TCHAR *HTTP_MIME[]    = { _T ( "*/*" ), _T ( "text/*" ), NULL } ;

		m_hRequest =::HttpOpenRequest(	m_hConnection,
			__HTTP_VERB_GET, // HTTP Verb
			szURI, // Object Name
			HTTP_VERSION, // Version
			/*""*/NULL, // Reference
			&HTTP_MIME[1], // Accept Type
			INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_NO_CACHE_WRITE,
			0); // context call-back point

		API_ASSERT(!!m_hRequest);
		if(!m_hRequest)
		{
			m_dwError=::GetLastError();
			return FALSE;
		}

		// REPLACE HEADER
		API_VERIFY(::HttpAddRequestHeaders( m_hRequest, __HTTP_ACCEPT, 
			-1, HTTP_ADDREQ_FLAG_ADD_IF_NEW));
		if (!bRet)
		{
			m_dwError=::GetLastError();
			return FALSE;
		}

		// SEND REQUEST
		API_VERIFY(::HttpSendRequest( m_hRequest,	// handle by returned HttpOpenRequest
			NULL, // additional HTTP header
			0, // additional HTTP header length
			NULL, // additional data in HTTP Post or HTTP Put
			0)); // additional data length
		//if(!bRet)
		//{
		//	m_dwError=::GetLastError();
		//	return FALSE;
		//}

		return TRUE;
	}
#endif

	HRESULT CFWebBrowserDumper::GetObjInfo(IInformationOutput* pInfoOutput)
	{
		HRESULT hr = E_POINTER;
		COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("WebBrowser")));
		if (m_pObj)
		{
			//能QI到
			CComQIPtr<IWebBrowser>     spWebBrowser(m_pObj);
			if (spWebBrowser)
			{
				CComBSTR bstrLocationName;//当前显示的资源名称的字符串(如 HTML标题、文件名等)
				COM_VERIFY(spWebBrowser->get_LocationName(&bstrLocationName));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("LocationName"), &bstrLocationName));

				CComBSTR bstrLocationURL; //WebBrowser正在显示的资源的URL
				COM_VERIFY(spWebBrowser->get_LocationURL(&bstrLocationURL));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("LocationURL"), &bstrLocationURL));
			}
		}
		return hr;
	}
	
	HRESULT CFWebBrowserAppDumper::GetObjInfo(IInformationOutput* pInfoOutput)
	{
		HRESULT hr = E_POINTER;
		COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("WebBrowserApp")));
		if (m_pObj)
		{
			//能QI到
			CComQIPtr<IWebBrowserApp>     spWebBrowserApp(m_pObj);
			if (spWebBrowserApp)
			{
				CComBSTR bstrName;
				COM_VERIFY(spWebBrowserApp->get_Name(&bstrName));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("Name"), &bstrName));
			}
		}
		return hr;
	}

	HRESULT CFWebBrowser2Dumper::GetObjInfo(IInformationOutput* pInfoOutput)
	{
		HRESULT hr = E_POINTER;
		COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("WebBrowser2")));
		if (m_pObj)
		{
			//能QI到
			CComQIPtr<IWebBrowser2>     spWebBrowser2(m_pObj);
			if (spWebBrowser2)
			{
				VARIANT_BOOL bOffline = VARIANT_FALSE;
				COM_VERIFY(spWebBrowser2->get_Offline(&)bOffline);
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("Offline"), bOffline));
			}
		}
		return hr;
	}

}//namespace FTL

#pragma warning( pop )

#endif //FTL_NET_HPP