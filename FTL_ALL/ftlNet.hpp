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
//#include <WS2tcpip.h>

#include <ftlFunctional.h>
#include <ftlDebug.h>
//#include <ftlMultiMedia.h>
#include <ftlNLS.h>

#pragma comment(lib, "Wininet.lib")
#ifdef __WINCRYPT_H__
#  pragma comment(lib, "Crypt32.lib")
#endif 

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

    namespace FNetInfo
    {
		//LPCTSTR GetSockAddrString(CFStringFormater& formater, SOCKADDR *sa, int len)
		//{
		//	ATLASSERT(sa);
		//	if (NULL == sa)
		//	{
		//		return NULL;
		//	}
		//	int rc = SOCKET_ERROR;
		//	TCHAR    host[NI_MAXHOST] = {0}, serv[NI_MAXSERV] = {0};
		//	int     hostlen = NI_MAXHOST, servlen = NI_MAXSERV;
		//	NET_VERIFY(GetNameInfo(sa, len, host, hostlen, serv, servlen, 
		//		NI_NUMERICHOST | NI_NUMERICSERV )); //Convert to numeric representation
		//	if (0 == rc)
		//	{
		//		if (sa->sa_family == AF_INET6)
		//		{
		//			formater.Format(TEXT("[%s]:%s"), host, serv)
		//		}
		//		else
		//		{
		//			formater.Format(TEXT("%s:%s"), host, serv)
		//		}
		//	}
		//	else
		//	{
		//		ATLASSERT(FALSE);
		//	}
		//	return formater.GetString();
		//}


		unsigned short CheckSum(USHORT *pBuffer, int size)
		{
			unsigned long cksum = 0;

			while (size > 1) 
			{
				cksum += *pBuffer++;
				size -= sizeof(USHORT);
			}
			if (size) 
			{
				cksum += *(UCHAR*)pBuffer;
			}
			cksum = (cksum >> 16) + (cksum & 0xffff);
			cksum += (cksum >>16);
			return (USHORT)(~cksum);
		}

		LPCTSTR GetCookieInfo(CFStringFormater& formater, LPCTSTR lpszUrl, LPCTSTR lpszCookieName)
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

		DWORD GetCookieInfoMap(LPCTSTR pszCookies, CookieKeyValueMap& cookieMap)
		{
			FTLASSERT(pszCookies);
			if (pszCookies)
			{
				typedef std::list<tstring>	ListCookies;
				ListCookies lstCookies;
#pragma TODO(Cookie Split Delimiter)
				//按照标准应该是分号分隔，但实测(baidu)似乎也有冒号分隔?
				FTL::Split(pszCookies, TEXT(";:"), false, lstCookies);
				for (ListCookies::iterator iter = lstCookies.begin();
					iter != lstCookies.end();
					++iter)
				{
					const tstring& strCookie = *iter;
					ListCookies	lstCookieKeyValue;
					FTL::Split(strCookie, TEXT("="), false, lstCookieKeyValue);
					FTLASSERT(lstCookieKeyValue.size() == 2);
					if (lstCookieKeyValue.size() == 2)
					{
						const tstring& strKey = lstCookieKeyValue.front();
						const tstring& strValue = lstCookieKeyValue.back();
						cookieMap[strKey] = strValue;
					}
				}
			}
			return cookieMap.size();
		}

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
                HANDLE_CASE_RETURN_STRING( SOCK_RAW );	//原生Socket，可以制作 Ping 等(参见 Samples\netds\winsock\ping)
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

		LPCTSTR GetPerConnOptionListInfo(CFStringFormater& formater, const INTERNET_PER_CONN_OPTION_LIST& optList)
		{
			formater.AppendFormat(TEXT("dwSize=%d, pszConnection=%s, dwOptionCount=%d, dwOptionError=%d,"),
				optList.dwSize,
				optList.pszConnection,
				optList.dwOptionCount,
				optList.dwOptionError
				);
#pragma TODO(LPINTERNET_PER_CONN_OPTIONA  pOptions)

			return formater.GetString();
		}

		LPCTSTR GetHInternetHandleType(CFStringFormater& formater, const ULONG& HandleType)
		{
			UNREFERENCED_PARAMETER(formater);
			LPCTSTR pszType = NULL;
			switch (HandleType)
			{
			case INTERNET_HANDLE_TYPE_INTERNET:			pszType = TEXT("Internet");		break;
			case INTERNET_HANDLE_TYPE_CONNECT_FTP:		pszType = TEXT("ConnectFtp");	break;
			case INTERNET_HANDLE_TYPE_CONNECT_GOPHER:	pszType = TEXT("ConnectGopher");break;
			case INTERNET_HANDLE_TYPE_CONNECT_HTTP:		pszType = TEXT("ConnectHttp");	break;
			case INTERNET_HANDLE_TYPE_FTP_FIND:			pszType = TEXT("FtpFind");		break;
			case INTERNET_HANDLE_TYPE_FTP_FIND_HTML:	pszType = TEXT("FtpFindHtml");	break;
			case INTERNET_HANDLE_TYPE_FTP_FILE:			pszType = TEXT("FtpFile");		break;
			case INTERNET_HANDLE_TYPE_FTP_FILE_HTML:	pszType = TEXT("FtpFileHtml");	break;
			case INTERNET_HANDLE_TYPE_GOPHER_FIND:		pszType = TEXT("GhopherFind");	break;
			case INTERNET_HANDLE_TYPE_GOPHER_FIND_HTML: pszType = TEXT("GhopherFindHtml"); break;
			case INTERNET_HANDLE_TYPE_GOPHER_FILE:		pszType = TEXT("GopherFile");	break;
			case INTERNET_HANDLE_TYPE_GOPHER_FILE_HTML: pszType = TEXT("GopherFileHtml"); break;
			case INTERNET_HANDLE_TYPE_HTTP_REQUEST:		pszType = TEXT("HttpRequest");	break;
			case INTERNET_HANDLE_TYPE_FILE_REQUEST:		pszType = TEXT("FileRequest");	break;
			default:
				FTLASSERT(FALSE);
				break;
			}
			return pszType;
		}

		LPCTSTR GetProxyInfoString(CFStringFormater& formater, const INTERNET_PROXY_INFO& proxyInfo)
		{
			formater.Format(TEXT("AccessType=%d, Proxy=%s, proxyBypass=%s"),
				proxyInfo.dwAccessType,
				proxyInfo.lpszProxy,
				proxyInfo.lpszProxyBypass);
			return formater.GetString();
			
		}

		LPCTSTR GetDiagnosticSocketInfoString(CFStringFormater& formater, const INTERNET_DIAGNOSTIC_SOCKET_INFO& diagSocketInfo)
		{
			formater.AppendFormat(TEXT("Socket=%d, SourcePort=%d, DestPort=%d, Flags=%d"),
				diagSocketInfo.Socket,
				diagSocketInfo.SourcePort,
				diagSocketInfo.DestPort,
				diagSocketInfo.Flags);
			return formater.GetString();
		}

		LPCTSTR GetCacheTimeStampsString(CFStringFormater& formater, const INTERNET_CACHE_TIMESTAMPS& cacheTimeStamps)
		{
			formater.AppendFormat(TEXT("Expires=%s, LastModified=%s"),
				CFFILETIMEDumpInfo(cacheTimeStamps.ftExpires).GetConvertedInfo(),
				CFFILETIMEDumpInfo(cacheTimeStamps.ftLastModified).GetConvertedInfo()
				);
			return formater.GetString();
		}

#ifdef __WINCRYPT_H__
		LPCTSTR GetCertChainContextString(CFStringFormater& formater, const PCCERT_CHAIN_CONTEXT& certChainContext)
		{
			FTLASSERT(FALSE);
			return formater.GetString();
		}
#endif 

		LPCTSTR GetReqestFlagString(CFStringFormater& formater, DWORD dwRequestFlags)
		{
			formater.Format(TEXT("0x%x, "), dwRequestFlags);

			DWORD dwOldRequestFlags = dwRequestFlags;
			UNREFERENCED_PARAMETER(dwOldRequestFlags);

			HANDLE_COMBINATION_VALUE_TO_STRING_EX(formater, dwRequestFlags, INTERNET_REQFLAG_FROM_CACHE, TEXT("FROM_CACHE"), TEXT("|"));
			HANDLE_COMBINATION_VALUE_TO_STRING_EX(formater, dwRequestFlags, INTERNET_REQFLAG_ASYNC, TEXT("ASYNC"), TEXT("|"));
			HANDLE_COMBINATION_VALUE_TO_STRING_EX(formater, dwRequestFlags, INTERNET_REQFLAG_VIA_PROXY, TEXT("VIA_PROXY"), TEXT("|"));
			HANDLE_COMBINATION_VALUE_TO_STRING_EX(formater, dwRequestFlags, INTERNET_REQFLAG_NO_HEADERS, TEXT("NO_HEADERS"), TEXT("|"));
			HANDLE_COMBINATION_VALUE_TO_STRING_EX(formater, dwRequestFlags, INTERNET_REQFLAG_PASSIVE, TEXT("PASSIVE"), TEXT("|"));
			HANDLE_COMBINATION_VALUE_TO_STRING_EX(formater, dwRequestFlags, INTERNET_REQFLAG_CACHE_WRITE_DISABLED, TEXT("CACHE_WRITE_DISABLED"), TEXT("|"));
			HANDLE_COMBINATION_VALUE_TO_STRING_EX(formater, dwRequestFlags, INTERNET_REQFLAG_NET_TIMEOUT, TEXT("NET_TIMEOUT"), TEXT("|"));

			FTLASSERT(0 == dwRequestFlags);
			return formater.GetString();
		}
		
		LPCTSTR GetSecurityFlagsString(CFStringFormater& formater, DWORD dwSecurityFlags)
		{
			formater.Format(TEXT("0x%x, "), dwSecurityFlags);

			DWORD dwOldSecurityFlags = dwSecurityFlags;
			UNREFERENCED_PARAMETER(dwOldSecurityFlags);

			HANDLE_COMBINATION_VALUE_TO_STRING_EX(formater, dwSecurityFlags, SECURITY_FLAG_SECURE, TEXT("Secure"), TEXT("|"));
			HANDLE_COMBINATION_VALUE_TO_STRING_EX(formater, dwSecurityFlags, SECURITY_FLAG_SSL, TEXT("SSL"), TEXT("|"));
			HANDLE_COMBINATION_VALUE_TO_STRING_EX(formater, dwSecurityFlags, SECURITY_FLAG_SSL3, TEXT("SSL3"), TEXT("|"));
			HANDLE_COMBINATION_VALUE_TO_STRING_EX(formater, dwSecurityFlags, SECURITY_FLAG_PCT, TEXT("PCT"), TEXT("|"));
			HANDLE_COMBINATION_VALUE_TO_STRING_EX(formater, dwSecurityFlags, SECURITY_FLAG_PCT4, TEXT("PCT4"), TEXT("|"));
			HANDLE_COMBINATION_VALUE_TO_STRING_EX(formater, dwSecurityFlags, SECURITY_FLAG_IETFSSL4, TEXT("IETFSSL4"), TEXT("|"));
			HANDLE_COMBINATION_VALUE_TO_STRING_EX(formater, dwSecurityFlags, SECURITY_FLAG_IGNORE_REVOCATION, TEXT("IgnoreRevocation"), TEXT("|"));
			HANDLE_COMBINATION_VALUE_TO_STRING_EX(formater, dwSecurityFlags, SECURITY_FLAG_IGNORE_UNKNOWN_CA, TEXT("IgnoreUnknownCA"), TEXT("|"));
			HANDLE_COMBINATION_VALUE_TO_STRING_EX(formater, dwSecurityFlags, SECURITY_FLAG_IGNORE_WRONG_USAGE, TEXT("IgnoreWrongUsage"), TEXT("|"));
			HANDLE_COMBINATION_VALUE_TO_STRING_EX(formater, dwSecurityFlags, SECURITY_FLAG_IGNORE_CERT_CN_INVALID, TEXT("IgnoreCertCNInvalid"), TEXT("|"));
			HANDLE_COMBINATION_VALUE_TO_STRING_EX(formater, dwSecurityFlags, SECURITY_FLAG_IGNORE_CERT_DATE_INVALID, TEXT("IgnoreCertDateInvalid"), TEXT("|"));
			HANDLE_COMBINATION_VALUE_TO_STRING_EX(formater, dwSecurityFlags, SECURITY_FLAG_IGNORE_REDIRECT_TO_HTTPS, TEXT("IgnoreRedirectToHttps"), TEXT("|"));
			HANDLE_COMBINATION_VALUE_TO_STRING_EX(formater, dwSecurityFlags, SECURITY_FLAG_IGNORE_REDIRECT_TO_HTTP, TEXT("IgnoreRedirectToHttp"), TEXT("|"));
			HANDLE_COMBINATION_VALUE_TO_STRING_EX(formater, dwSecurityFlags, SECURITY_FLAG_FORTEZZA, TEXT("Fortezza"), TEXT("|"));
			HANDLE_COMBINATION_VALUE_TO_STRING_EX(formater, dwSecurityFlags, SECURITY_FLAG_STRENGTH_WEAK, TEXT("StrengthWeak"), TEXT("|"));
			HANDLE_COMBINATION_VALUE_TO_STRING_EX(formater, dwSecurityFlags, SECURITY_FLAG_STRENGTH_STRONG, TEXT("StrengthStrong"), TEXT("|"));
			HANDLE_COMBINATION_VALUE_TO_STRING_EX(formater, dwSecurityFlags, SECURITY_FLAG_STRENGTH_MEDIUM, TEXT("StrengthMedium"), TEXT("|"));
			HANDLE_COMBINATION_VALUE_TO_STRING_EX(formater, dwSecurityFlags, SECURITY_FLAG_UNKNOWNBIT, TEXT("UnknownBit"), TEXT("|"));

			FTLASSERT(0 == dwSecurityFlags);
			return formater.GetString();
		}

		LPCTSTR GetHInternetOption(CFStringFormater& formater, HINTERNET hInternet, DWORD dwOption /* = -1 */)
		{
			BOOL bRet = FALSE;
			//DWORD dwValOption = 0;
			ULONG ulValOption = 0;
			//BOOL  bValOption = FALSE;
			DWORD dwSize = sizeof(ulValOption);
			//TCHAR szDefaultFileStringBuf[MAX_PATH] = {0};


#define GET_HINTERNET_OPTION_STRING_EX(opt, pVal, ex1, ex2, info)\
		dwSize = sizeof(*pVal);\
		ZeroMemory(pVal, dwSize);\
		API_VERIFY_EXCEPT2(InternetQueryOption(hInternet, opt, pVal, &dwSize), ex1, ex2);\
		if(bRet)\
		{\
			formater.AppendFormat(info, *pVal);\
		}

#define GET_HINTERNET_OPTION_STRING(opt, pVal, info)	GET_HINTERNET_OPTION_STRING_EX(opt, pVal, 0, 0, info)

#define GET_HINTERNET_OPTION_STRING_TRANSLATE_EX(opt, pVal, ex1, ex2, info, tran)\
		dwSize = sizeof(*pVal);\
		ZeroMemory(pVal, dwSize);\
		API_VERIFY_EXCEPT2(InternetQueryOption(hInternet, opt, pVal, &dwSize), ex1, ex2);\
		if(bRet)\
		{\
			CFStringFormater localFormater;\
			formater.AppendFormat(info, tran(localFormater, *pVal));\
		}


//获取字符串型的参数
#define GET_HINTERNET_STRING_OPTION_STRING_EX(opt, ex1, ex2, info)\
		dwSize = 0;\
		bRet = InternetQueryOption(hInternet, opt, NULL, &dwSize);\
		if(!bRet && GetLastError() == ERROR_INSUFFICIENT_BUFFER)\
		{\
			LPTSTR pszData = new TCHAR[dwSize + 1];\
			ZeroMemory(pszData, dwSize);\
			API_VERIFY_EXCEPT2(InternetQueryOption(hInternet, opt, pszData, &dwSize), ex1, ex2);\
			if(bRet)\
			{\
				pszData[dwSize] = NULL;\
				formater.AppendFormat(info, pszData);\
			}\
			delete [] pszData;\
		}

			if (-1 == dwOption)
			{
				GET_HINTERNET_OPTION_STRING(INTERNET_OPTION_CALLBACK, &ulValOption, TEXT("Callback=0x%x\n"));

				//连接超时，默认60m
				GET_HINTERNET_OPTION_STRING(INTERNET_OPTION_CONNECT_TIMEOUT, &ulValOption, TEXT("ConnectTimeout:%d(ms)\n"));

				//连接重试次数，默认
				GET_HINTERNET_OPTION_STRING(INTERNET_OPTION_CONNECT_RETRIES, &ulValOption, TEXT("ConnectRetries:%d\n"));

				GET_HINTERNET_OPTION_STRING(INTERNET_OPTION_CONNECT_BACKOFF, &ulValOption, TEXT("ConnectBackOff:%d\n"));

				//发送超时，默认30m
				GET_HINTERNET_OPTION_STRING(INTERNET_OPTION_SEND_TIMEOUT, &ulValOption, TEXT("SendTimeOut:%d(ms)\n"));//INTERNET_OPTION_CONTROL_SEND_TIMEOUT
				//接收超时，默认30m
				GET_HINTERNET_OPTION_STRING(INTERNET_OPTION_RECEIVE_TIMEOUT, &ulValOption, TEXT("ReceiveTimeOut:%d(ms)\n"));//INTERNET_OPTION_CONTROL_RECEIVE_TIMEOUT
				//数据发送超时，默认30m
				GET_HINTERNET_OPTION_STRING(INTERNET_OPTION_DATA_SEND_TIMEOUT, &ulValOption, TEXT("DataSendTimeOut:%d(ms)\n"));
				//数据接收超时，默认30m
				GET_HINTERNET_OPTION_STRING(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, &ulValOption, TEXT("DataSendReceiveTimeOut:%d(ms)\n"));

				//获取指定 HINTERNET 句柄的连接类型
				GET_HINTERNET_OPTION_STRING_TRANSLATE_EX(INTERNET_OPTION_HANDLE_TYPE, &ulValOption, 0, 0, TEXT("HandleType:%s\n"), GetHInternetHandleType);

				//10
				//msdn上说是 ERROR_NOT_SUPPORTED，但实测是 ERROR_INVALID_PARAMETER
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_LISTEN_TIMEOUT, &ulValOption, ERROR_INVALID_PARAMETER, 0, TEXT("ListenTimeOut:%d\n"));
				
				//本地读数据缓冲区大小，默认 4K(4096)
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_READ_BUFFER_SIZE, &ulValOption, ERROR_INTERNET_INCORRECT_HANDLE_TYPE, 0, TEXT("ReadBufferSize:%d\n"));
				//本地发数据缓冲区大小，默认 4K(4096)
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_WRITE_BUFFER_SIZE, &ulValOption, ERROR_INTERNET_INCORRECT_HANDLE_TYPE, 0, TEXT("WriteBufferSize:%d\n"));
				//14
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_ASYNC_ID, &ulValOption, ERROR_INVALID_PARAMETER, 
					ERROR_CALL_NOT_IMPLEMENTED, TEXT("AsyncId:%d\n"));
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_ASYNC_PRIORITY, &ulValOption, 
					ERROR_INVALID_PARAMETER, ERROR_CALL_NOT_IMPLEMENTED, TEXT("AsyncPriority:%d\n"));
				//17~20

				//
				GET_HINTERNET_OPTION_STRING(INTERNET_OPTION_PARENT_HANDLE, &ulValOption, TEXT("ParentHandle:0x%x\n"));

				//msdn中写的是未实现，但返回TRUE，其值为乱码
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_KEEP_CONNECTION, &ulValOption, ERROR_INTERNET_INCORRECT_HANDLE_TYPE, 0, TEXT("KeepConnection:0x%x\n"));
				
				GET_HINTERNET_OPTION_STRING_TRANSLATE_EX(INTERNET_OPTION_REQUEST_FLAGS, &ulValOption, 
					ERROR_INTERNET_INCORRECT_HANDLE_TYPE, 0, TEXT("RequestFlags:%s\n"), GetReqestFlagString);

				GET_HINTERNET_OPTION_STRING(INTERNET_OPTION_EXTENDED_ERROR, &ulValOption, TEXT("ExtendedError:%d\n"));
				//25
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_OFFLINE_MODE, &ulValOption, ERROR_CALL_NOT_IMPLEMENTED, 0, TEXT("OfflineMode:%d\n"));
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_CACHE_STREAM_HANDLE, &ulValOption,
					ERROR_INVALID_FUNCTION, ERROR_INTERNET_INCORRECT_HANDLE_TYPE, TEXT("CacheStreamHandle:%d\n"));

				//InternetConnect 时设置的用户名和密码(明文)
				GET_HINTERNET_STRING_OPTION_STRING_EX(INTERNET_OPTION_USERNAME, ERROR_INTERNET_INCORRECT_HANDLE_TYPE, 0, TEXT("UserName:%s\n"));
				GET_HINTERNET_STRING_OPTION_STRING_EX(INTERNET_OPTION_PASSWORD, ERROR_INTERNET_INCORRECT_HANDLE_TYPE, 0, TEXT("Password:%s\n"));

				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_ASYNC, &ulValOption, ERROR_CALL_NOT_IMPLEMENTED, 0, TEXT("Async:%d\n"));
				GET_HINTERNET_OPTION_STRING_TRANSLATE_EX(INTERNET_OPTION_SECURITY_FLAGS, &ulValOption,
					ERROR_INTERNET_INCORRECT_HANDLE_TYPE, 0, TEXT("SecurityFlags:%s\n"), GetSecurityFlagsString);

				INTERNET_CERTIFICATE_INFO certInfo = {0};
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_SECURITY_CERTIFICATE_STRUCT, &certInfo,
					ERROR_INTERNET_INVALID_OPERATION, ERROR_INTERNET_INCORRECT_HANDLE_TYPE, TEXT("SecurityCertificateStruct:%d\n"));

				GET_HINTERNET_STRING_OPTION_STRING_EX(INTERNET_OPTION_DATAFILE_NAME, ERROR_INTERNET_INCORRECT_HANDLE_TYPE,
					ERROR_INTERNET_ITEM_NOT_FOUND, TEXT("DataFileName:%s\n"));

				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_URL, &ulValOption, ERROR_INTERNET_INCORRECT_HANDLE_TYPE, ERROR_INTERNET_INVALID_URL, TEXT("Url:%d\n"));
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_SECURITY_CERTIFICATE, &ulValOption, 
					ERROR_INTERNET_INVALID_OPERATION, ERROR_INTERNET_INCORRECT_HANDLE_TYPE, TEXT("SecurityCertificate:%d\n"));

				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_SECURITY_KEY_BITNESS, &ulValOption, 
					ERROR_INTERNET_INVALID_OPERATION, ERROR_INTERNET_INCORRECT_HANDLE_TYPE, TEXT("SecurityKeyBitness:%d\n"));
				
#if 0
				//强制刷新的选项，只用于Set
				GET_HINTERNET_OPTION_STRING(INTERNET_OPTION_REFRESH, &ulValOption, TEXT("Refresh:%d\n"));
#endif 			
				{
					INTERNET_PROXY_INFO  internetProxyInfo = {0};
					GET_HINTERNET_OPTION_STRING_TRANSLATE_EX(INTERNET_OPTION_PROXY, &internetProxyInfo, ERROR_INTERNET_INCORRECT_HANDLE_TYPE, 0, 
						TEXT("Proxy:%s\n"), GetProxyInfoString);

				}
#if 0
				//通知系统设置改变，只用于Set
				GET_HINTERNET_OPTION_STRING(INTERNET_OPTION_SETTINGS_CHANGED, &ulValOption, TEXT("SettingsChanged:%d\n"));
#endif 
				{
					//Win7 下的版本是 1.2
					INTERNET_VERSION_INFO internetVersionInfo = {0};
					dwSize = sizeof(internetVersionInfo);
					API_VERIFY(InternetQueryOption(hInternet, INTERNET_OPTION_VERSION, &internetVersionInfo, &dwSize));
					if (bRet)
					{
						formater.AppendFormat(TEXT("InternetVersion: %d.%d\n"), internetVersionInfo.dwMajorVersion, internetVersionInfo.dwMinorVersion);
					}
				}

				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_USER_AGENT, &ulValOption, ERROR_INTERNET_INCORRECT_HANDLE_TYPE, 0, TEXT("UserAgent:%d\n"));
#if 0
				//只用于Set
				GET_HINTERNET_OPTION_STRING(INTERNET_OPTION_END_BROWSER_SESSION, &ulValOption, TEXT("EndBrowserSession:%d\n"));
#endif 
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_PROXY_USERNAME, &ulValOption, ERROR_INTERNET_INCORRECT_HANDLE_TYPE, 0, TEXT("ProxyUserName:%d\n"));
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_PROXY_PASSWORD, &ulValOption, ERROR_INTERNET_INCORRECT_HANDLE_TYPE, 0, TEXT("ProxyPassword:%d\n"));
				GET_HINTERNET_OPTION_STRING(INTERNET_OPTION_CONTEXT_VALUE, &ulValOption, TEXT("ConnectValue:%d\n"));

				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_CONNECT_LIMIT, &ulValOption, ERROR_INVALID_PARAMETER, 0, TEXT("ConnectLimit:%d\n"));
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_SECURITY_SELECT_CLIENT_CERT, &ulValOption, ERROR_INVALID_PARAMETER, 0, TEXT("SecuritySelectClientCert:%d\n"));
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_POLICY, &ulValOption, ERROR_INVALID_PARAMETER, 0, TEXT("Policy:%d\n"));
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_DISCONNECTED_TIMEOUT, &ulValOption, ERROR_INVALID_PARAMETER, 0, TEXT("DisconnectedTimeout:%d\n"));

				GET_HINTERNET_OPTION_STRING(INTERNET_OPTION_CONNECTED_STATE, &ulValOption, TEXT("ConnectedState:%d\n"));
				GET_HINTERNET_OPTION_STRING(INTERNET_OPTION_IDLE_STATE, &ulValOption, TEXT("IdleState:%d\n"));
				GET_HINTERNET_OPTION_STRING(INTERNET_OPTION_OFFLINE_SEMANTICS, &ulValOption, TEXT("OfflineSemantics:%d\n"));

				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_SECONDARY_CACHE_KEY, &ulValOption, ERROR_INTERNET_INCORRECT_HANDLE_TYPE, 0, TEXT("SecondaryCacheKey:%d\n"));
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_CALLBACK_FILTER, &ulValOption, ERROR_INTERNET_INCORRECT_HANDLE_TYPE, ERROR_NOT_SUPPORTED, TEXT("CallbackFilter:%d\n"));
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_CONNECT_TIME, &ulValOption, ERROR_NOT_SUPPORTED, 0, TEXT("ConnectTime:%d\n"));
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_SEND_THROUGHPUT, &ulValOption, ERROR_NOT_SUPPORTED, 0, TEXT("SendThroughput:%d\n"));
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_RECEIVE_THROUGHPUT, &ulValOption, ERROR_NOT_SUPPORTED, 0, TEXT("ReceiveThroughput:%d\n"));

				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_REQUEST_PRIORITY, &ulValOption, ERROR_INTERNET_INCORRECT_HANDLE_TYPE, 0, TEXT("RequestPriority:%d\n"));

				{
					//Win7下的版本是 1.1 
					HTTP_VERSION_INFO httpVersionInfo = {0};
					dwSize = sizeof(httpVersionInfo);
					API_VERIFY(InternetQueryOption(hInternet, INTERNET_OPTION_HTTP_VERSION, &httpVersionInfo, &dwSize));
					if (bRet)
					{
						formater.AppendFormat(TEXT("HttpVersion: %d.%d\n"), httpVersionInfo.dwMajorVersion, httpVersionInfo.dwMinorVersion);
					}
				}

				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_RESET_URLCACHE_SESSION, &ulValOption, ERROR_INVALID_PARAMETER, 0, TEXT("ResetUrlCacheSession:%d\n"));
				//61
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_ERROR_MASK, &ulValOption, ERROR_INVALID_PARAMETER, 0, TEXT("ErrorMask:%d\n"));
				GET_HINTERNET_OPTION_STRING(INTERNET_OPTION_FROM_CACHE_TIMEOUT, &ulValOption, TEXT("FromCacheTimeout:%d\n"));
				GET_HINTERNET_OPTION_STRING(INTERNET_OPTION_BYPASS_EDITED_ENTRY, &ulValOption, TEXT("BypassEditedEntry:%d\n"));
				GET_HINTERNET_OPTION_STRING(INTERNET_OPTION_HTTP_DECODING, &ulValOption, TEXT("HttpDecoding:%d\n"));
				//66
				{
					INTERNET_DIAGNOSTIC_SOCKET_INFO diagSocketInfo = {0};
					GET_HINTERNET_OPTION_STRING_TRANSLATE_EX(INTERNET_OPTION_DIAGNOSTIC_SOCKET_INFO, &diagSocketInfo, 
						ERROR_INTERNET_INCORRECT_HANDLE_TYPE, 0, TEXT("DiagnosticSocketInfo:%s\n"), GetDiagnosticSocketInfoString);
				}
				GET_HINTERNET_OPTION_STRING(INTERNET_OPTION_CODEPAGE, &ulValOption, TEXT("CodePage:%d\n"));
				{
					INTERNET_CACHE_TIMESTAMPS cacheTimeStamps = {0};
					GET_HINTERNET_OPTION_STRING_TRANSLATE_EX(INTERNET_OPTION_CACHE_TIMESTAMPS, &cacheTimeStamps, 
						ERROR_INTERNET_INCORRECT_HANDLE_TYPE, 0, TEXT("CacheTimeStamps:%s\n"), GetCacheTimeStampsString);
				}
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_DISABLE_AUTODIAL, &ulValOption, ERROR_NOT_SUPPORTED, 0, TEXT("DisableAutoDial:%d\n"));
				//71~73

				//每服务器允许的最大连接数， P2P破解的就是这个? 默认值是 10
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_MAX_CONNS_PER_SERVER, &ulValOption, 
					ERROR_INTERNET_INCORRECT_HANDLE_TYPE, ERROR_INTERNET_INVALID_OPERATION, TEXT("MaxConnsPerServer:%d\n"));

				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_MAX_CONNS_PER_1_0_SERVER, &ulValOption, 
					ERROR_INTERNET_INVALID_OPERATION, 0, TEXT("MaxConnsPer10Server:%d\n"));

				{
					INTERNET_PER_CONN_OPTION_LIST connOptionList = {0};
					GET_HINTERNET_OPTION_STRING_TRANSLATE_EX(INTERNET_OPTION_PER_CONNECTION_OPTION, &connOptionList, 0, 0, TEXT("PerConnectionOption:%s\n"),
						GetPerConnOptionListInfo);
				}
				
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_DIGEST_AUTH_UNLOAD, &ulValOption, ERROR_INVALID_PARAMETER, 0, TEXT("DigestAuthUnload:%d\n"));
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_IGNORE_OFFLINE, &ulValOption, ERROR_INVALID_PARAMETER, 0, TEXT("IgnoreOffline:%d\n"));
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_IDENTITY, &ulValOption, ERROR_CALL_NOT_IMPLEMENTED, 0, TEXT("Identity:%d\n"));
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_REMOVE_IDENTITY, &ulValOption, ERROR_INVALID_PARAMETER, 0, TEXT("RemoveIdentity:%d\n"));
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_ALTER_IDENTITY, &ulValOption, ERROR_INVALID_PARAMETER, 0, TEXT("AlterIdentity:%d\n"));
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_SUPPRESS_BEHAVIOR, &ulValOption, ERROR_INVALID_PARAMETER, 0, TEXT("SuppressBehavior:%d\n"));
				GET_HINTERNET_OPTION_STRING(INTERNET_OPTION_AUTODIAL_MODE, &ulValOption, TEXT("AutoDialMode:%d\n"));
				GET_HINTERNET_OPTION_STRING(INTERNET_OPTION_AUTODIAL_CONNECTION, &ulValOption, TEXT("AutoDialConnection:%d\n"));
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_CLIENT_CERT_CONTEXT, &ulValOption, ERROR_INVALID_PARAMETER, 0, TEXT("ClientCertContext:%d\n"));
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_AUTH_FLAGS, &ulValOption, ERROR_INVALID_PARAMETER, 0, TEXT("AuthFlags:%d\n"));
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_COOKIES_3RD_PARTY, &ulValOption, ERROR_INTERNET_INCORRECT_HANDLE_TYPE, 0, TEXT("Cookies3rdParty:%d\n"));
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_DISABLE_PASSPORT_AUTH, &ulValOption, ERROR_INVALID_PARAMETER, 0, TEXT("DisablePassportAuth:%d\n"));
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_SEND_UTF8_SERVERNAME_TO_PROXY, &ulValOption, ERROR_INVALID_PARAMETER, 0, TEXT("SendUTF8ServerNameToProxy:%d\n"));
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_EXEMPT_CONNECTION_LIMIT, &ulValOption, ERROR_INVALID_PARAMETER, 0, TEXT("ExemptConnectionLimit:%d\n"));
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_ENABLE_PASSPORT_AUTH, &ulValOption, ERROR_INVALID_PARAMETER, 0, TEXT("EnablePassportAuth:%d\n"));
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_HIBERNATE_INACTIVE_WORKER_THREADS, &ulValOption, ERROR_INVALID_PARAMETER, 0, TEXT("HibernateInactiveWorkerThreads:%d\n"));
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_ACTIVATE_WORKER_THREADS, &ulValOption, ERROR_INVALID_PARAMETER, 0, TEXT("ActivateWorkerThreads:%d\n"));
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_RESTORE_WORKER_THREAD_DEFAULTS, &ulValOption, ERROR_INVALID_PARAMETER, 0, TEXT("RestoreWorkerThreadDefaults:%d\n"));
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_SOCKET_SEND_BUFFER_LENGTH, &ulValOption, ERROR_INVALID_PARAMETER, 0, TEXT("SocketSendBufferLength:%d\n"));
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_PROXY_SETTINGS_CHANGED, &ulValOption, ERROR_INVALID_PARAMETER, 0, TEXT("ProxySettingChanged:%d\n"));
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_DATAFILE_EXT, &ulValOption, ERROR_INVALID_PARAMETER, 0, TEXT("DataFileExt:%d\n"));
				
				//97~99
				//返回的是-1，用字符串会出错 ?
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_CODEPAGE_PATH, &ulValOption, 0, 0, TEXT("CodePagePath:%d\n"));
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_CODEPAGE_EXTRA, &ulValOption, 0, 0, TEXT("CodePageExtra:%d\n"));
				GET_HINTERNET_OPTION_STRING(INTERNET_OPTION_IDN, &ulValOption, TEXT("IDN:%d\n"));
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_MAX_CONNS_PER_PROXY, &ulValOption, ERROR_INTERNET_INVALID_OPERATION, 0, TEXT("MaxConnsPerProxy:%d\n"));
				GET_HINTERNET_OPTION_STRING_EX(INTERNET_OPTION_SUPPRESS_SERVER_AUTH, &ulValOption, ERROR_INVALID_PARAMETER, 0, TEXT("SuppressServerAuth:%d\n"));
				{
#ifdef __WINCRYPT_H__				
					PCCERT_CHAIN_CONTEXT pCertChainContext= NULL;
					GET_HINTERNET_OPTION_STRING_TRANSLATE_EX(INTERNET_OPTION_SERVER_CERT_CHAIN_CONTEXT, &pCertChainContext, 
						ERROR_INTERNET_INCORRECT_HANDLE_TYPE, ERROR_INTERNET_INCORRECT_HANDLE_STATE, TEXT("ServerCertChainContext:%s\n"), GetCertChainContextString);
					if (pCertChainContext)
					{
						 CertFreeCertificateChain(pCertChainContext);
						 pCertChainContext = NULL;
					}
#endif 
				}
			}
			return formater.GetString();
		}

		LPCTSTR GetHttpQueryInfoString(CFStringFormater& formater, HINTERNET hInternet, DWORD dwInfoLevel /* = DWORD */)
		{
			BOOL bRet = FALSE;
			//DWORD dwValOption = 0;
			ULONG ulValOption = 0;
			//BOOL  bValOption = FALSE;
			DWORD dwIndex = 0;
			TCHAR szDefaultFileStringBuf[MAX_PATH] = {0};
			DWORD dwSize = sizeof(szDefaultFileStringBuf);

		#define GET_HTTP_QUERY_INFO_STRING_EX(lv, pVal, ex1, ex2, info)\
			dwSize = sizeof(*pVal);\
			FTLTRACE(TEXT("Level=%d, dwSize=%d\n"), lv, dwSize);\
			ZeroMemory(pVal, dwSize);\
			API_VERIFY_EXCEPT2(HttpQueryInfo(hInternet, lv, pVal, &dwSize, &dwIndex), ex1, ex2);\
			if(bRet)\
			{\
				formater.AppendFormat(info, *pVal);\
			}

		#define GET_HTTP_QUERY_INFO_STRING(lv, pVal, info)	GET_HTTP_QUERY_INFO_STRING_EX(lv, pVal, 0, 0, info)

		#define GET_HTTP_QUERY_INFO_STRING_TRANSLATE_EX(lv, pVal, ex1, ex2, info, tran)\
			dwSize = sizeof(*pVal);\
			ZeroMemory(pVal, dwSize);\
			API_VERIFY_EXCEPT2(HttpQueryInfo(hInternet, lv, pVal, &dwSize, &dwIndex), ex1, ex2);\
			if(bRet)\
			{\
				CFStringFormater localFormater;\
				formater.AppendFormat(info, tran(localFormater, *pVal));\
			}


			//获取字符串型的参数
		#define GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(lv, ex1, ex2, info)\
			dwSize =  sizeof(szDefaultFileStringBuf);\
			szDefaultFileStringBuf[0] = NULL;\
			bRet = HttpQueryInfo(hInternet, lv, szDefaultFileStringBuf, &dwSize, &dwIndex);\
			if(!bRet && GetLastError() == ERROR_INSUFFICIENT_BUFFER)\
			{\
				LPTSTR pszData = new TCHAR[dwSize + 1];\
				ZeroMemory(pszData, dwSize);\
				API_VERIFY_EXCEPT2(HttpQueryInfo(hInternet, lv, pszData, &dwSize, &dwIndex), ex1, ex2);\
				if(bRet)\
				{\
					pszData[dwSize] = NULL;\
					formater.AppendFormat(info, pszData);\
				}\
				delete [] pszData;\
			}\
			else if(bRet && NULL != szDefaultFileStringBuf[0])\
			{\
				formater.AppendFormat(info, szDefaultFileStringBuf);\
			}

			if (-1 == dwInfoLevel)
			{
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_MIME_VERSION, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("MimeVersion:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_CONTENT_TYPE, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("ContentType:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_CONTENT_TRANSFER_ENCODING, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("ContentTransferEncoding:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_CONTENT_ID, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("ContentID:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_CONTENT_DESCRIPTION, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("ContentDescription:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_CONTENT_LENGTH, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("ContentLength:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_CONTENT_LANGUAGE, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("ContentLanguage:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_ALLOW, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("Allow:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_PUBLIC, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("Public:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_DATE, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("Date:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_EXPIRES, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("Expires:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_LAST_MODIFIED, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("LastModified:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_MESSAGE_ID, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("MessageId:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_URI, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("URI:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_DERIVED_FROM, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("DerivedFrom:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_COST, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("Cost:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_LINK, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("Link:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_PRAGMA, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("Pragma:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_VERSION, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("Version:%s\n"));

				//状态码
				GET_HTTP_QUERY_INFO_STRING_EX(HTTP_QUERY_FLAG_NUMBER | HTTP_QUERY_STATUS_CODE, &ulValOption, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("StatusCode:%d\n"));
				//GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_STATUS_CODE, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("StatusCode:%s\n"));

				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_STATUS_TEXT, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("StatusText:%s\n"));

				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_RAW_HEADERS, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("RawHeaders:%s\n"));

				//获取带CRLF的原始头数据?
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_RAW_HEADERS_CRLF, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("RawHeadersCRLF:%s\n"));

				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_CONNECTION, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("Connection:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_ACCEPT, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("Accept:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_ACCEPT_CHARSET, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("AcceptCharset:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_ACCEPT_ENCODING, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("AcceptEncoding:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_ACCEPT_LANGUAGE, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("AcceptLanguage:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_AUTHORIZATION, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("Authorization:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_CONTENT_ENCODING, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("ContentEncoding:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_FORWARDED, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("Forwarded:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_FROM, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("From:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_IF_MODIFIED_SINCE, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("IfModifiedSince:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_LOCATION, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("Location:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_ORIG_URI, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("OrigUri:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_REFERER, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("Buffer:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_RETRY_AFTER, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("RetryAfter:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_SERVER, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("Server:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_TITLE, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("Title:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_USER_AGENT, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("UserAgent:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_WWW_AUTHENTICATE, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("WwwAuthenticate:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_PROXY_AUTHENTICATE, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("ProxyAuthenticate:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_ACCEPT_RANGES, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("AcceptRanges:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_SET_COOKIE, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("SetCookie:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_COOKIE, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("Cookie:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_REQUEST_METHOD, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("RequestMethod:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_REFRESH, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("Refresh:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_CONTENT_DISPOSITION, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("ContentDisposition:%s\n"));

				// HTTP 1.1 defined headers
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_AGE, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("Age:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_CACHE_CONTROL, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("CacheControl:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_CONTENT_BASE, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("ContentBase:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_CONTENT_LOCATION, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("ContentLocation:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_CONTENT_MD5, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("ContentMD5:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_CONTENT_RANGE, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("ContentRange:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_ETAG, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("ETag:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_HOST, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("Host:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_IF_MATCH, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("IfMatch:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_IF_NONE_MATCH, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("IfNoneMatch:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_IF_RANGE, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("IfRange:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_IF_UNMODIFIED_SINCE, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("IfUnmodified:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_MAX_FORWARDS, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("MaxForwards:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_PROXY_AUTHORIZATION, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("ProxyAuthorization:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_RANGE, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("Range:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_TRANSFER_ENCODING, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("TransferEncoding:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_UPGRADE, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("Upgrade:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_VARY, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("Vary:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_VIA, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("Via:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_WARNING, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("Warning:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_EXPECT, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("Expect:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_PROXY_CONNECTION, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("ProxyConnection:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_UNLESS_MODIFIED_SINCE, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("UnlessModifiedSince:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_ECHO_REQUEST, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("EchoRequest:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_ECHO_REPLY, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("EchoReply:%s\n"));

				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_ECHO_HEADERS, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("EchoHeaders:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_ECHO_HEADERS_CRLF, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("EchoHeadersCRLF:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_PROXY_SUPPORT, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("ProxySupport:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_AUTHENTICATION_INFO, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("AuthenticationInfo:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_PASSPORT_URLS, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("PassportUrls:%s\n"));
				GET_HTTP_QUERY_INFO_STRING_OPTION_STRING_EX(HTTP_QUERY_PASSPORT_CONFIG, ERROR_HTTP_HEADER_NOT_FOUND, 0, TEXT("PassportConfig:%s\n"));

			}

			return formater.GetString();
		}


        LONG GetLocalIPAddress()
        {
            LONG localIP = 0;
            char host[MAX_BUFFER_LENGTH];
            ::gethostname(host, sizeof(host));
            hostent	*ent = ::gethostbyname(host);
            if (ent)
            {
                localIP = *(ULONG *)ent->h_addr_list[0];
				//sprintf(szIP, "%d.%d.%d.%d", (BYTE)ent->h_addr_list[0][0], (BYTE)ent->h_addr_list[0][1], 
				//(BYTE)ent->h_addr_list[0][2], (BYTE)ent->h_addr_list[0][3]);
            }

            //后续可以使用 inet_ntoa 等函数转换为 IP 的字符串
            return localIP;
        }
    }//namespace .NetInfo 

	CFInternetStatusCallbackImpl::CFInternetStatusCallbackImpl()
	{
		m_param = NULL;
		m_hInternet = NULL;
		m_pOldCallBack = NULL;	
	}

	CFInternetStatusCallbackImpl::~CFInternetStatusCallbackImpl()
	{
		Detach();
	}
	
	VOID CFInternetStatusCallbackImpl::SetParam(DWORD_PTR param)
	{
		m_param = param;
	}

	BOOL CFInternetStatusCallbackImpl::Attach(HINTERNET hInternet)
	{
		ATLASSERT(NULL == m_pOldCallBack);  //just call once

		BOOL bRet = TRUE;
		INTERNET_STATUS_CALLBACK pOldCallback = NULL;
		pOldCallback = InternetSetStatusCallback(hInternet, _StatusCallbackProc);
		API_ASSERT(INTERNET_INVALID_STATUS_CALLBACK  != pOldCallback );

		if (INTERNET_INVALID_STATUS_CALLBACK != pOldCallback)
		{
			m_pOldCallBack = pOldCallback;
			m_hInternet = hInternet;
		}
		return bRet;
	}

	BOOL CFInternetStatusCallbackImpl::Detach()
	{
		if (m_pOldCallBack && m_hInternet)
		{
			InternetSetStatusCallback(m_hInternet, m_pOldCallBack);
			m_pOldCallBack = NULL;
			m_hInternet = NULL;
		}
		return TRUE;
	}

	VOID CFInternetStatusCallbackImpl::_InnerStatusCallbackProc(
		HINTERNET hInternet, 
		DWORD dwInternetStatus, 
		LPVOID lpvStatusInformation, 
		DWORD dwStatusInformationLength)
	{
		FTLTRACE(TEXT("_InnerStatusCallbackProc, hInternet=0x%x, dwInternetStatus=%d, lpvStatusInformation=0x%p, length=%d\n"),
			hInternet, dwInternetStatus, lpvStatusInformation, dwStatusInformationLength);

		switch (dwInternetStatus)
		{
		case INTERNET_STATUS_RESOLVING_NAME: 
			OnResolvingName(hInternet, m_param, (LPCTSTR)lpvStatusInformation, dwStatusInformationLength);
			break;
		case INTERNET_STATUS_NAME_RESOLVED: 
			OnNameResolved(hInternet, m_param, CA2T((LPCSTR)lpvStatusInformation), dwStatusInformationLength); 
			break;
		case INTERNET_STATUS_CONNECTING_TO_SERVER: 
			OnConnectingToServer(hInternet, m_param, (SOCKADDR*)lpvStatusInformation, dwStatusInformationLength); 
			break;
		case INTERNET_STATUS_CONNECTED_TO_SERVER: OnConnectedToServer(hInternet, m_param); break;
		case INTERNET_STATUS_SENDING_REQUEST: OnSendingRequest(hInternet, m_param); break;
		case INTERNET_STATUS_REQUEST_SENT: 
			OnRequestSent(hInternet, m_param, (DWORD*)lpvStatusInformation, dwStatusInformationLength); 
			break;
		case INTERNET_STATUS_RECEIVING_RESPONSE: OnReceivingResponse(hInternet, m_param); break;
		case INTERNET_STATUS_RESPONSE_RECEIVED: 
			OnResponseReceived(hInternet, m_param, (DWORD*)lpvStatusInformation, dwStatusInformationLength); 
			break;
		case INTERNET_STATUS_CTL_RESPONSE_RECEIVED: OnCtlResponseReceived(hInternet, m_param); break;
		case INTERNET_STATUS_PREFETCH: OnPrefetch(hInternet, m_param); break;
		case INTERNET_STATUS_CLOSING_CONNECTION: OnClosingConnection(hInternet, m_param); break;
		case INTERNET_STATUS_CONNECTION_CLOSED: OnConnectionClosed(hInternet, m_param); break;
		case INTERNET_STATUS_HANDLE_CREATED: 
			OnHandleCreated(hInternet, m_param, (INTERNET_ASYNC_RESULT*)lpvStatusInformation, dwStatusInformationLength); 
			break;
		case INTERNET_STATUS_HANDLE_CLOSING: 
			OnHandleClosing(hInternet, m_param, (DWORD*)lpvStatusInformation, dwStatusInformationLength); 
			break;
		case INTERNET_STATUS_DETECTING_PROXY: OnDetectingProxy(hInternet, m_param); break;
		case INTERNET_STATUS_REQUEST_COMPLETE: 
			OnRequestComplete(hInternet, m_param, (INTERNET_ASYNC_RESULT*)lpvStatusInformation, dwStatusInformationLength); 
			break;
		case INTERNET_STATUS_REDIRECT: OnRedirect(hInternet, m_param); break;
		case INTERNET_STATUS_INTERMEDIATE_RESPONSE: OnIntermediateResponse(hInternet, m_param); break;
		case INTERNET_STATUS_USER_INPUT_REQUIRED: OnUserInputRequired(hInternet, m_param); break;
		case INTERNET_STATUS_STATE_CHANGE: OnStateChange(hInternet, m_param); break;
		case INTERNET_STATUS_COOKIE_SENT: 
			OnCookieSent(hInternet, m_param, (DWORD*)lpvStatusInformation, dwStatusInformationLength); 
			break;
		case INTERNET_STATUS_COOKIE_RECEIVED: OnCookieReceived(hInternet, m_param); break;
		case INTERNET_STATUS_PRIVACY_IMPACTED: OnPrivacyImpacted(hInternet, m_param); break;
		case INTERNET_STATUS_P3P_HEADER: OnP3pHeader(hInternet, m_param); break;
		case INTERNET_STATUS_P3P_POLICYREF: OnP3pPolicyRef(hInternet, m_param); break;
		case INTERNET_STATUS_COOKIE_HISTORY: 
			OnCookieHistory(hInternet, m_param, (InternetCookieHistory*)lpvStatusInformation, dwStatusInformationLength) ; break;
			//case INTERNET_STATUS_XXXXXXXXXXXX: OnXXXXXXXXXXXXX(dwContext); break;
		default:
			FTLTRACEEX(FTL::tlWarning, TEXT("%s: Unknown Internet Status %d(0x%x)\n"), 
				__FILE__LINE__, dwInternetStatus, dwInternetStatus);
			break;
		}
	}

	VOID CFInternetStatusCallbackImpl::_StatusCallbackProc( 
		HINTERNET hInternet, 
		DWORD_PTR dwContext, 
		DWORD dwInternetStatus, 
		LPVOID lpvStatusInformation, 
		DWORD dwStatusInformationLength )
	{
		//ATLASSERT(m_hInternet == hInternet);
		ATLASSERT(NULL != dwContext);
		if (NULL != dwContext)
		{
			CFInternetStatusCallbackImpl* pThis = (CFInternetStatusCallbackImpl*)dwContext;

			ATLASSERT(pThis);

			pThis->_InnerStatusCallbackProc(hInternet, dwInternetStatus, lpvStatusInformation, dwStatusInformationLength);
		}
	}

	void CFInternetStatusCallbackImpl::OnResolvingName(HINTERNET hInternet, DWORD_PTR dwContext, LPCTSTR lpszName, DWORD dwLength)
	{
		UNREFERENCED_PARAMETER(hInternet);
		UNREFERENCED_PARAMETER(dwContext);
		UNREFERENCED_PARAMETER(lpszName);
		UNREFERENCED_PARAMETER(dwLength);

		FTLTRACE(TEXT("CFInternetStatusCallbackImpl::OnResolvingName, dwLength=%d, Name=%s\n"), 
			dwLength, lpszName);
	}
	void CFInternetStatusCallbackImpl::OnNameResolved(HINTERNET hInternet, DWORD_PTR dwContext, LPCTSTR lpszName, DWORD dwLength)
	{
		UNREFERENCED_PARAMETER(hInternet);
		UNREFERENCED_PARAMETER(dwContext);
		UNREFERENCED_PARAMETER(lpszName);
		UNREFERENCED_PARAMETER(dwLength);

		FTLTRACE(TEXT("CFInternetStatusCallbackImpl::OnNameResolved, dwLength=%d, Name=%s\n"),
			dwLength, lpszName);
	}
	void CFInternetStatusCallbackImpl::OnConnectingToServer(HINTERNET hInternet, DWORD_PTR dwContext, SOCKADDR* pSockAddr, DWORD dwLength)
	{
		UNREFERENCED_PARAMETER(hInternet);
		UNREFERENCED_PARAMETER(dwContext);
		UNREFERENCED_PARAMETER(pSockAddr);
		UNREFERENCED_PARAMETER(dwLength);

		FTLTRACE(TEXT("CFInternetStatusCallbackImpl::OnConnectingToServer, dwLength=%d, SockAddr=%s\n"),
			dwLength, CA2T(pSockAddr->sa_data));
	}
	void CFInternetStatusCallbackImpl::OnConnectedToServer(HINTERNET hInternet, DWORD_PTR dwContext)
	{
		UNREFERENCED_PARAMETER(hInternet);
		UNREFERENCED_PARAMETER(dwContext);
		FTLTRACE(TEXT("CFInternetStatusCallbackImpl::OnConnectedToServer\n"));
	}
	void CFInternetStatusCallbackImpl::OnSendingRequest(HINTERNET hInternet, DWORD_PTR dwContext)
	{
		UNREFERENCED_PARAMETER(hInternet);
		UNREFERENCED_PARAMETER(dwContext);
		FTLTRACE(TEXT("CFInternetStatusCallbackImpl::OnSendingRequest\n"));
	}
	void CFInternetStatusCallbackImpl::OnRequestSent(HINTERNET hInternet, DWORD_PTR dwContext, DWORD* pdwSend, DWORD dwLength)
	{
		UNREFERENCED_PARAMETER(hInternet);
		UNREFERENCED_PARAMETER(dwContext);
		UNREFERENCED_PARAMETER(pdwSend);
		UNREFERENCED_PARAMETER(dwLength);

		FTLTRACE(TEXT("CFInternetStatusCallbackImpl::OnRequestSent, Send %d Bytes to Server\n"),
			*pdwSend);
	}
	void CFInternetStatusCallbackImpl::OnReceivingResponse(HINTERNET hInternet, DWORD_PTR dwContext)
	{
		UNREFERENCED_PARAMETER(hInternet);
		UNREFERENCED_PARAMETER(dwContext);
		FTLTRACE(TEXT("CFInternetStatusCallbackImpl::OnReceivingResponse\n"));
	}
	void CFInternetStatusCallbackImpl::OnResponseReceived(HINTERNET hInternet, DWORD_PTR dwContext, DWORD* pdwResponse, DWORD dwLength)
	{
		UNREFERENCED_PARAMETER(hInternet);
		UNREFERENCED_PARAMETER(dwContext);
		UNREFERENCED_PARAMETER(pdwResponse);
		UNREFERENCED_PARAMETER(dwLength);
		FTLTRACE(TEXT("CFInternetStatusCallbackImpl::OnResponseReceived, Received %d Bytes\n"),
			*pdwResponse);
	}
	void CFInternetStatusCallbackImpl::OnCtlResponseReceived(HINTERNET hInternet, DWORD_PTR dwContext)
	{
		UNREFERENCED_PARAMETER(hInternet);
		UNREFERENCED_PARAMETER(dwContext);
		FTLTRACE(TEXT("CFInternetStatusCallbackImpl::OnCtlResponseReceived\n"));
	}
	void CFInternetStatusCallbackImpl::OnPrefetch(HINTERNET hInternet, DWORD_PTR dwContext)
	{
		UNREFERENCED_PARAMETER(hInternet);
		UNREFERENCED_PARAMETER(dwContext);
		FTLTRACE(TEXT("CFInternetStatusCallbackImpl::OnPrefetch\n"));
	}
	void CFInternetStatusCallbackImpl::OnClosingConnection(HINTERNET hInternet, DWORD_PTR dwContext)
	{
		UNREFERENCED_PARAMETER(hInternet);
		UNREFERENCED_PARAMETER(dwContext);
		FTLTRACE(TEXT("CFInternetStatusCallbackImpl::OnClosingConnection\n"));
	}
	void CFInternetStatusCallbackImpl::OnConnectionClosed(HINTERNET hInternet, DWORD_PTR dwContext)
	{
		UNREFERENCED_PARAMETER(hInternet);
		UNREFERENCED_PARAMETER(dwContext);
		FTLTRACE(TEXT("CFInternetStatusCallbackImpl::OnConnectionClosed\n"));
	}
	void CFInternetStatusCallbackImpl::OnHandleCreated(HINTERNET hInternet, DWORD_PTR dwContext, 
		INTERNET_ASYNC_RESULT* pAsyncResult, DWORD dwLength)
	{
		UNREFERENCED_PARAMETER(hInternet);
		UNREFERENCED_PARAMETER(dwContext);
		UNREFERENCED_PARAMETER(pAsyncResult);
		UNREFERENCED_PARAMETER(dwLength);
		//dwResult 是 HINTERNET 句柄
		FTLASSERT(sizeof(INTERNET_ASYNC_RESULT) == dwLength);
		FTLTRACE(TEXT("CFInternetStatusCallbackImpl::OnHandleCreated, AsyncResult, dwResult=0x%x, dwError=%d\n"),
			pAsyncResult->dwResult, pAsyncResult->dwError);
	}

	void CFInternetStatusCallbackImpl::OnHandleClosing(HINTERNET hInternet, DWORD_PTR dwContext, DWORD* pdwTemp, DWORD dwLength)
	{
		//最后一个事件?
		UNREFERENCED_PARAMETER(hInternet);
		UNREFERENCED_PARAMETER(dwContext);
		UNREFERENCED_PARAMETER(pdwTemp);
		UNREFERENCED_PARAMETER(dwLength);

		FTLTRACE(TEXT("CFInternetStatusCallbackImpl::OnHandleClosing, dwLength=%d, tmpValue=%d,\n"),
			dwLength, *pdwTemp);
	}
	void CFInternetStatusCallbackImpl::OnDetectingProxy(HINTERNET hInternet, DWORD_PTR dwContext)
	{
		UNREFERENCED_PARAMETER(hInternet);
		UNREFERENCED_PARAMETER(dwContext);
		FTLTRACE(TEXT("CFInternetStatusCallbackImpl::OnDetectingProxy\n"));
	}
	void CFInternetStatusCallbackImpl::OnRequestComplete(HINTERNET hInternet, DWORD_PTR dwContext, INTERNET_ASYNC_RESULT* pAsyncResult, DWORD dwLength)
	{
		UNREFERENCED_PARAMETER(hInternet);
		UNREFERENCED_PARAMETER(dwContext);
		UNREFERENCED_PARAMETER(pAsyncResult);
		UNREFERENCED_PARAMETER(dwLength);
		FTLASSERT(sizeof(INTERNET_ASYNC_RESULT) == dwLength);
		FTLTRACE(TEXT("CFInternetStatusCallbackImpl::OnRequestComplete, AsyncResult, dwResult=0x%x, dwError=%d\n"),
			pAsyncResult->dwResult, pAsyncResult->dwError);
	}
	void CFInternetStatusCallbackImpl::OnRedirect(HINTERNET hInternet, DWORD_PTR dwContext)
	{
		UNREFERENCED_PARAMETER(hInternet);
		UNREFERENCED_PARAMETER(dwContext);
		FTLTRACE(TEXT("CFInternetStatusCallbackImpl::OnRedirect\n"));
	}
	void CFInternetStatusCallbackImpl::OnIntermediateResponse(HINTERNET hInternet, DWORD_PTR dwContext)
	{
		UNREFERENCED_PARAMETER(hInternet);
		UNREFERENCED_PARAMETER(dwContext);
		FTLTRACE(TEXT("CFInternetStatusCallbackImpl::OnIntermediateResponse\n"));
	}
	void CFInternetStatusCallbackImpl::OnUserInputRequired(HINTERNET hInternet, DWORD_PTR dwContext)
	{
		UNREFERENCED_PARAMETER(hInternet);
		UNREFERENCED_PARAMETER(dwContext);
		FTLTRACE(TEXT("CFInternetStatusCallbackImpl::OnUserInputRequired\n"));
	}
	void CFInternetStatusCallbackImpl::OnStateChange(HINTERNET hInternet, DWORD_PTR dwContext)
	{
		UNREFERENCED_PARAMETER(hInternet);
		UNREFERENCED_PARAMETER(dwContext);
		FTLTRACE(TEXT("CFInternetStatusCallbackImpl::OnStateChange\n"));
	}
	void CFInternetStatusCallbackImpl::OnCookieSent(HINTERNET hInternet, DWORD_PTR dwContext, DWORD* pTmpValue, DWORD dwLength)
	{
		UNREFERENCED_PARAMETER(hInternet);
		UNREFERENCED_PARAMETER(dwContext);
		UNREFERENCED_PARAMETER(pTmpValue);
		UNREFERENCED_PARAMETER(dwLength);
		FTLTRACE(TEXT("CFInternetStatusCallbackImpl::OnCookieSent\n"));
	}
	void CFInternetStatusCallbackImpl::OnCookieReceived(HINTERNET hInternet, DWORD_PTR dwContext)
	{
		UNREFERENCED_PARAMETER(hInternet);
		UNREFERENCED_PARAMETER(dwContext);
		FTLTRACE(TEXT("CFInternetStatusCallbackImpl::OnCookieReceived\n"));
	}
	void CFInternetStatusCallbackImpl::OnPrivacyImpacted(HINTERNET hInternet, DWORD_PTR dwContext)
	{
		UNREFERENCED_PARAMETER(hInternet);
		UNREFERENCED_PARAMETER(dwContext);
		FTLTRACE(TEXT("CFInternetStatusCallbackImpl::OnPrivacyImpacted\n"));
	}
	void CFInternetStatusCallbackImpl::OnP3pHeader(HINTERNET hInternet, DWORD_PTR dwContext)
	{
		UNREFERENCED_PARAMETER(hInternet);
		UNREFERENCED_PARAMETER(dwContext);
		FTLTRACE(TEXT("CFInternetStatusCallbackImpl::OnP3pHeader\n"));
	}
	void CFInternetStatusCallbackImpl::OnP3pPolicyRef(HINTERNET hInternet, DWORD_PTR dwContext)
	{
		UNREFERENCED_PARAMETER(hInternet);
		UNREFERENCED_PARAMETER(dwContext);
		FTLTRACE(TEXT("CFInternetStatusCallbackImpl::OnP3pPolicyRef\n"));
	}
	void CFInternetStatusCallbackImpl::OnCookieHistory(HINTERNET hInternet, DWORD_PTR dwContext, 
		InternetCookieHistory* pCookieHistory, DWORD dwLength)
	{
		UNREFERENCED_PARAMETER(hInternet);
		UNREFERENCED_PARAMETER(dwContext);
		UNREFERENCED_PARAMETER(pCookieHistory);
		UNREFERENCED_PARAMETER(dwLength);
		FTLASSERT(sizeof(InternetCookieHistory) == dwLength);
		FTLTRACE(TEXT("CFInternetStatusCallbackImpl::OnCookieHistory, CookieInfo = Accepted =%d, Leashed=%d, Downgraded=%d, Rejected=%d\n"),
			pCookieHistory->fAccepted, pCookieHistory->fLeashed, pCookieHistory->fDowngraded, pCookieHistory->fRejected);
	}

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

	template<typename T>
	int CFSocketT<T>::Send(const BYTE* pBuf, INT len, DWORD flags)
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
	int CFSocketT<T>::Recv(BYTE* pBuf, INT len, DWORD flags)
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
            //分三种情况(注意对 PostQueuedCompletionStatus 的调用也会唤醒，需要判断并处理)：
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

	//////////////////////////////////////////////////////////////////////////
	//template<typename PACKET_T>
	CFTCPReceiver::CFTCPReceiver(IReceiveAdapter* pReceiveAdapter, IPacketParserAdapter* pParserAdapter, 
		INT nBufferSize /* = INTERNET_BUFFER_SIZE */)
	{
		m_pReceiveAdapter = pReceiveAdapter;
		m_pParserAdapter = pParserAdapter;

		m_pBufferHeader = new BYTE[nBufferSize];
		if (m_pBufferHeader)
		{
			ZeroMemory(m_pBufferHeader, nBufferSize);

			m_nBufferSize = nBufferSize;
			m_pRead = m_pBufferHeader;
			m_pWrite = m_pBufferHeader;
		}
		else
		{
			m_nBufferSize = 0;
			m_pRead = NULL;
			m_pWrite = NULL;
		}
	}

	CFTCPReceiver::~CFTCPReceiver()
	{
		m_pRead = NULL;
		m_pWrite = NULL;
		SAFE_DELETE_ARRAY(m_pBufferHeader);
	}

	INT CFTCPReceiver::ReceiveData()
	{
		FTLASSERT(m_pReceiveAdapter);
		FTLASSERT(m_pBufferHeader);

		CFAutoLock<CFLockObject> locker(&m_LockObject);
		INT nLength = 0;

		if (m_pReceiveAdapter && m_pBufferHeader)
		{
			INT nAvailableWriteLength = m_nBufferSize - INT(m_pWrite - m_pBufferHeader);
			FTLASSERT(nAvailableWriteLength >= 0);
			if (nAvailableWriteLength > 0)
			{
				nLength = m_pReceiveAdapter->ReceiveData(m_pWrite,  nAvailableWriteLength);
			}
			if (nLength > 0)
			{
				m_pWrite += nLength;
			}
		}
		return nLength;
	}

	INT CFTCPReceiver::ParsePacket()
	{
		FTLASSERT(m_pParserAdapter);
		FTLASSERT(m_pRead <= m_pWrite);

		CFAutoLock<CFLockObject> locker(&m_LockObject);

		INT nLength = 0;
		INT nTotalParseLength = 0;

		INT dwAvailableReadLength = INT(m_pWrite - m_pRead);

		while (dwAvailableReadLength > 0 )
		{
			nLength = m_pParserAdapter->ParsePacket(m_pRead, dwAvailableReadLength);
			if (nLength > 0)
			{
				nTotalParseLength += nLength;
				m_pRead += nLength;
				dwAvailableReadLength -= nLength;

				FTLASSERT(m_pRead <= m_pWrite);
				FTLASSERT(dwAvailableReadLength >= 0);
			}
			else
			{
				//nLength <= 0
				break;
			}
		}

		FTLASSERT(m_pRead <= m_pWrite);
		
		if (m_pRead != m_pBufferHeader)
		{
			DWORD dwRemainLength = (DWORD)(m_pWrite - m_pRead);
			BYTE* pOldRead = m_pRead;
			m_pRead = m_pBufferHeader;

			if (dwRemainLength > 0)
			{
				CopyMemory(m_pRead, pOldRead, dwRemainLength);
			}
			m_pWrite = m_pRead + dwRemainLength;
		}
		return nTotalParseLength;
	}
	//////////////////////////////////////////////////////////////////////////

	FTransferJobInfo::FTransferJobInfo()
	{
		m_strServerName.Empty();
		m_strObjectName.Empty();
		m_nPort = INTERNET_DEFAULT_HTTP_PORT;
		//m_bUploadJob = FALSE;
		m_transferParams.clear();
		m_strResponseData = "";
	}
	FTransferJobInfo::FTransferJobInfo(LPCTSTR pszServerName, LPCTSTR pszObjectName, 
		USHORT nPort /* = INTERNET_DEFAULT_HTTP_PORT */)
	{
		FTLASSERT(pszServerName);
		FTLASSERT(pszObjectName);
		
		if (pszServerName)
		{
			m_strServerName = pszServerName;
		}
		if (pszObjectName)
		{
			m_strObjectName = pszObjectName;
		}
		m_nPort = nPort;
		
		//m_bUploadJob = FALSE;
		m_transferParams.clear();
		m_strResponseData = "";
	}
	FTransferJobInfo::~FTransferJobInfo()
	{

	}

	CFTransferJobBase::CFTransferJobBase(const CAtlString& strAgent)
	{
		m_strAgent = strAgent;
		m_nTotalSize = (LONGLONG)(-1);
		m_nCurPos = 0;

		m_hSession = NULL;
		m_hConnection = NULL;
		m_hRequest = NULL;
	}

	CFTransferJobBase::~CFTransferJobBase()
	{
		FTLASSERT(NULL == m_hRequest);
		FTLASSERT(NULL == m_hConnection);
		FTLASSERT(NULL == m_hSession);
	}

	BOOL CFTransferJobBase::_Connect()
	{
		FTLASSERT(NULL == m_hSession);
		BOOL bRet = FALSE;

		API_VERIFY(NULL != (m_hSession = ::InternetOpen(m_strAgent,
			INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0)));
		if (bRet)
		{
			API_VERIFY(NULL !=(m_hConnection = ::InternetConnect(m_hSession, 
				m_JobParam->m_strServerName, 
				m_JobParam->m_nPort, 
				NULL, NULL, //username and password
				INTERNET_SERVICE_HTTP, 0, NULL)));
		}
		return bRet;
	}

	void CFTransferJobBase::_Close()
	{
		BOOL bRet = FALSE;
		SAFE_CLOSE_INTERNET_HANDLE(m_hRequest);
		SAFE_CLOSE_INTERNET_HANDLE(m_hConnection);
		SAFE_CLOSE_INTERNET_HANDLE(m_hSession);
	}

	BOOL CFTransferJobBase::_SendN(PBYTE pBuffer, DWORD nCount, DWORD* pSend)
	{
		FTLASSERT(m_hRequest);
		FTLASSERT(pBuffer);
		FTLASSERT(nCount > 0);

		BOOL bRet = FALSE;

		DWORD nLeft = nCount;
		DWORD nSend = 0;
		PBYTE pSendData = pBuffer;
		while (nLeft > 0)
		{
			API_VERIFY(::InternetWriteFile(m_hRequest, pSendData, nLeft, &nSend));
			if (!bRet)
			{
				//TODO:
				break;
			}
			FTLASSERT(nLeft == nSend);   //什么情况会出现不等 ?
			nLeft -= nSend;
			pSendData += nSend;
		} 
		if (pSend)
		{
			*pSend = nSend;
		}
		return bRet;
	}


	BOOL CFTransferJobBase::Initialize()
	{
		return TRUE;
	}

	BOOL CFTransferJobBase::Run()
	{
		BOOL bRet = FALSE;
		DWORD dwError = 0;
		CAtlString strErrInfo = TEXT("");

		do 
		{
			API_VERIFY(_CheckParams());
			if (!bRet)
			{
				dwError = ERROR_INVALID_PARAMETER;
				strErrInfo = TEXT("CheckParam");
				break;
			}
			API_VERIFY(_Connect());
			if (!bRet)
			{
				dwError = GetLastError();
				strErrInfo = TEXT("Connect");
				break;
			}
			API_VERIFY_EXCEPT1(_SendRequest(), ERROR_CANCELLED);
			if (!bRet)
			{
				dwError = GetLastError();
				strErrInfo = TEXT("SendRequest");
				break;
			}
			API_VERIFY_EXCEPT1(_ReceiveResponse(), ERROR_CANCELLED);
			if (!bRet)
			{
				dwError = GetLastError();
				strErrInfo = TEXT("ReceiveResponse");
				break;
			}
		} while (FALSE);

		//error handle, notify callback			
		if (dwError)
		{
			if (dwError == ERROR_CANCELLED || GetJobWaitType(0) == ftwtStop)
			{
				_NotifyCancel();
			}
			else
			{
				_NotifyError(dwError, strErrInfo);
			}
		}

		_Close();
		return TRUE;
	}

	void CFTransferJobBase::Finalize()
	{
		delete this;
	}

	void CFTransferJobBase::OnCancelJob()
	{
		delete this;
	}

	CFUploadJob::CFUploadJob(const CAtlString& strAgent)
		:CFTransferJobBase(strAgent)
	{
		m_pEndBoundaryPostParam = NULL;
	}

	LPCTSTR CFUploadJob::_GetMultiPartBoundary(BOOL bEnd) const
	{
		if (!bEnd)
		{
			return TEXT("--MULTI-PARTS-FORM-DATA-BOUNDARY");
			
		}
		return TEXT("--MULTI-PARTS-FORM-DATA-BOUNDARY--");
	}

	BOOL CFUploadJob::_CheckParams()
	{
		BOOL bRet = TRUE;
		m_nTotalSize = 0;

		for (TransferParamContainer::iterator iter = m_JobParam->m_transferParams.begin();
			iter != m_JobParam->m_transferParams.end();
			++iter)
		{
			FTransferParam& transParam = *iter;
			UNREFERENCED_PARAMETER(transParam);
		}

		//转换Post的参数（1.LPCTSTR -> LPSTR; 2. localFile)，并计算总共需要的长度
		API_VERIFY(_TranslatePostParams());

		LONG64 nTotalSize = _CalcContentLength();
		if (bRet && nTotalSize > 0)
		{
			m_nTotalSize = nTotalSize;
			
			//创建RequestHeader 的说明信息 -- 说明是 "multipart/form-data, 且指明 Boundary
			CAtlString strContentTypeValue;
			strContentTypeValue.Format(TEXT("multipart/form-data; boundary=%s"), _GetMultiPartBoundary(FALSE));
			_AddRequestHeader(TEXT("Content-Type"), strContentTypeValue);

			//增加其他可能需要的Header
			_AddRequestHeader(TEXT("Accept"), TEXT("*/*"));

			CAtlString strContentLength;
			strContentLength.Format(TEXT("%d"), (LONG32)nTotalSize);		//最大4G?
			_AddRequestHeader(TEXT("Content-Length"), strContentLength);

			_AddRequestHeader(TEXT("Connection"), TEXT("Keep-Alive"));
			_AddRequestHeader(TEXT("Cache-Control"), TEXT("no-cache"));
		}
		return bRet;
	}

	//LPSTR CFUploadJob::_AllocMultiCharBuffer(LPCWSTR pwszInfo, UINT nCodePage)
	//{
	//	LPSTR pszBuffer = NULL;
	//	int nSize = WideCharToMultiByte( nCodePage, 0, pwszInfo, -1, NULL, 0, NULL, NULL );
	//	if (nSize > 0)
	//	{
	//		LPSTR pszBuffer = new CHAR[nSize];
	//		ZeroMemory(pszBuffer, nSize);
	//		WideCharToMultiByte( nCodePage, 0, pwszInfo, -1, pszBuffer, nSize, 0, 0 );
	//	}
	//	return pszBuffer;
	//}
	BOOL CFUploadJob::_TranslatePostParams()
	{
		//first clear old param
		_ClearPostParams();

		BOOL bRet = TRUE;
		for (TransferParamContainer::iterator iter = m_JobParam->m_transferParams.begin();
			iter != m_JobParam->m_transferParams.end();
			++iter)
		{
			FTransferParam& transParam = *iter;
			switch(transParam.paramType)
			{
			case tptPostArgument:
				{
					CAtlStringA strParam;
					strParam.Format(
						"--%s\r\n"
						"Content-Disposition: form-data; name=\"%s\"\r\n"
						"\r\n"
						"%s\r\n", 
						CFConversion(transParam.CodePage).TCHAR_TO_UTF8(_GetMultiPartBoundary(FALSE)),
						CFConversion(transParam.CodePage).TCHAR_TO_UTF8(transParam.strName),
						CFConversion(transParam.CodePage).TCHAR_TO_UTF8(transParam.strValue));
					PostArgumentParam* pPostArgument = new PostArgumentParam();
					if (pPostArgument)
					{
						pPostArgument->nBufferSize = strParam.GetLength();
						int nLength = strParam.GetLength() + sizeof(CHAR);
						pPostArgument->pBuffer = new CHAR[ nLength ];
						StringCchCopyA(pPostArgument->pBuffer, nLength, strParam);

						m_postArgumentParams.push_back(pPostArgument);
					}
					break;
				}
			case tptLocalFile:
				{
					HANDLE hFile=::CreateFile(transParam.strValue, 
						GENERIC_READ, 
						FILE_SHARE_READ, // share mode
						NULL, // security attribute
						OPEN_EXISTING, // create disposition
						FILE_ATTRIBUTE_NORMAL, // flag and attributes
						NULL); // template file handle
					API_ASSERT(INVALID_HANDLE_VALUE != hFile);

					if (INVALID_HANDLE_VALUE != hFile)
					{
						PostFileParam* pFileParam = new PostFileParam();
						pFileParam->dwFileSize= ::GetFileSize(hFile, NULL);
						CloseHandle(hFile);

						//TCHAR szContentType[MAX_PATH] = {0};
						//API_VERIFY(CFMultiMediaUtil::GetContentType(transParam.strValue, szContentType, _countof(szContentType)));

						CAtlStringA strParam;
						strParam.Format(
							"--%s\r\n"
							"Content-Disposition: form-data; name=\"%s\"; filename=\"%s\"\r\n"
							//"Content-Type: %s\r\n"
							"\r\n",
							CFConversion(transParam.CodePage).TCHAR_TO_UTF8(_GetMultiPartBoundary(FALSE)),
							CFConversion(transParam.CodePage).TCHAR_TO_UTF8(transParam.strName),  //一般来说是 "file[1]"
							CFConversion(transParam.CodePage).TCHAR_TO_UTF8(transParam.strValue)
							//CFConversion(transParam.CodePage).TCHAR_TO_UTF8(szContentType)
							//CFConversion(transParam.CodePage).TCHAR_TO_UTF8(TEXT("image/jpeg"))
							);

						pFileParam->strFilePath = transParam.strValue;
						pFileParam->nBufferSize = strParam.GetLength();
						int nLength = strParam.GetLength() + sizeof(CHAR);
						pFileParam->pBuffer = new CHAR[ nLength ];
						StringCchCopyA(pFileParam->pBuffer, nLength, strParam);

						m_postFileParams.push_back(pFileParam);
					}

					break;
				}
			}
		}

		if (!m_postArgumentParams.empty() || !m_postFileParams.empty())
		{
			CAtlStringA strEndBoundary;
			strEndBoundary.Format("--%s\r\n", 
				CFConversion(CP_UTF8).TCHAR_TO_UTF8(_GetMultiPartBoundary(TRUE)));

			m_pEndBoundaryPostParam = new PostArgumentParam();
			m_pEndBoundaryPostParam->nBufferSize = strEndBoundary.GetLength();
			int nLength = strEndBoundary.GetLength() + sizeof(CHAR);
			m_pEndBoundaryPostParam->pBuffer = new CHAR[ nLength ];
			StringCchCopyA(m_pEndBoundaryPostParam->pBuffer, nLength, strEndBoundary);
		}
		return bRet;
	}

	void CFUploadJob::_ClearPostParams()
	{
		SAFE_DELETE(m_pEndBoundaryPostParam);

		for (std::list<PostArgumentParam*>::iterator iterArgument = m_postArgumentParams.begin();
			iterArgument != m_postArgumentParams.end();
			++iterArgument)
		{
			PostArgumentParam* pArgumentParam = *iterArgument;
			SAFE_DELETE_ARRAY(pArgumentParam->pBuffer);
			SAFE_DELETE(pArgumentParam);
		}
		m_postArgumentParams.clear();

		for (std::list<PostFileParam*>::iterator iterFile = m_postFileParams.begin();
			iterFile != m_postFileParams.end();
			++iterFile)
		{
			PostFileParam* pFileParam = *iterFile;
			SAFE_DELETE_ARRAY(pFileParam->pBuffer);
			SAFE_DELETE(pFileParam);
		}
		m_postFileParams.clear();
	}

	void CFUploadJob::_AddRequestHeader(LPCTSTR pszName, LPCTSTR pszValue)
	{
		FTransferParam param;
		param.CodePage = CP_UTF8;
		param.paramType = tptRequestHeader;
		param.strName = pszName;
		param.strValue = pszValue;
		m_JobParam->m_transferParams.push_back(param);
	}

	BOOL CFUploadJob::_SendPostArgument(PBYTE pBuffer, DWORD dwBufferSize)
	{
		FUNCTION_BLOCK_TRACE(100);
		BOOL bRet = TRUE;
		//DWORD dwPostSize = 0;
		
		PBYTE pDestBuffer = pBuffer;
		DWORD dwDestSize = 0;

		for (std::list<PostArgumentParam*>::iterator iterArgument = m_postArgumentParams.begin();
			iterArgument != m_postArgumentParams.end();
			++iterArgument)
		{
			if (GetJobWaitType(0) == ftwtStop)
			{
				break;
			}
			PostArgumentParam* pArgumentParam = *iterArgument;
			
			FTLASSERT(pArgumentParam);
			ATLASSERT((LONG)dwBufferSize - (LONG)dwDestSize > pArgumentParam->nBufferSize);

			CopyMemory(pDestBuffer, pArgumentParam->pBuffer, pArgumentParam->nBufferSize);
			pDestBuffer += pArgumentParam->nBufferSize;
			dwDestSize += pArgumentParam->nBufferSize;

			FTLTRACEEX(tlTrace, TEXT("_SendPostArgument:Len=%d, Buffer=%s\n"), 
				pArgumentParam->nBufferSize, 
				CFConversion(CP_UTF8).UTF8_TO_TCHAR(pArgumentParam->pBuffer));
		}
		if (dwDestSize > 0)
		{
			API_VERIFY(_SendN((PBYTE)pBuffer, dwDestSize, NULL));
			m_nCurPos += dwDestSize;
			_NotifyProgress(m_nCurPos, m_nTotalSize);
		}

		return bRet;
	}

	BOOL CFUploadJob::_SendLocalFile(PBYTE pBuffer, DWORD dwBufferSize)
	{
		FUNCTION_BLOCK_TRACE(100);
		BOOL bRet = TRUE;
		for (std::list<PostFileParam*>::iterator iterFile = m_postFileParams.begin();
			iterFile != m_postFileParams.end();
			++iterFile)
		{
			if (GetJobWaitType(0) == ftwtStop)
			{
				SetLastError(ERROR_CANCELLED);
				break;
			}

			PostFileParam* pFileParam = *iterFile;
			FTLASSERT(pFileParam);
			//DWORD dwOutPostBufferLength = 0;

			FTLTRACEEX(tlTrace, TEXT("_SendLocalFile:FileSize=%d, Len=%d, Buffer=%s\n"), 
				pFileParam->dwFileSize,
				pFileParam->nBufferSize, 
				CFConversion(CP_UTF8).UTF8_TO_TCHAR(pFileParam->pBuffer));

			API_VERIFY(_SendN((PBYTE)pFileParam->pBuffer, pFileParam->nBufferSize, NULL));
			//API_VERIFY(InternetWriteFile(m_hRequest, pFileParam->pBuffer, pFileParam->nBufferSize, &dwOutPostBufferLength));
			if (!bRet)
			{
				break;
			}

			HANDLE hFile=::CreateFile(pFileParam->strFilePath, 
				GENERIC_READ, 
				FILE_SHARE_READ, // share mode
				NULL, // security attribute
				OPEN_EXISTING, // create disposition
				FILE_ATTRIBUTE_NORMAL, // flag and attributes
				NULL); // template file handle
			API_ASSERT(INVALID_HANDLE_VALUE != hFile);
			if (INVALID_HANDLE_VALUE != hFile)
			{
				DWORD dwReadSize = 0;
				DWORD dwRemainBytes = ::GetFileSize(hFile, NULL);
				while (ReadFile(hFile, pBuffer, dwBufferSize, &dwReadSize, NULL) 
					&& dwReadSize > 0
					&& dwRemainBytes > 0)
				{
					if (GetJobWaitType(0) == ftwtStop)
					{
						SetLastError(ERROR_CANCELLED);
						break;
					}
					API_VERIFY(_SendN(pBuffer, dwReadSize, NULL));

					if (!bRet)
					{
						break;
					}
					dwRemainBytes -= dwReadSize;
					m_nCurPos += dwReadSize;

					_NotifyProgress(m_nCurPos, m_nTotalSize);
				}

				//发送最后的 \r\n
				API_VERIFY(_SendN((PBYTE)"\r\n", 2, NULL));
				CloseHandle(hFile);
			}
		}
		return bRet;
	}

	BOOL  CFUploadJob::_SendEndBoundary(PBYTE pBuffer, DWORD dwBufferSize)
	{
		BOOL bRet = TRUE;
		if (m_pEndBoundaryPostParam)
		{
			UNREFERENCED_PARAMETER(pBuffer);
			UNREFERENCED_PARAMETER(dwBufferSize);
			API_VERIFY(_SendN((PBYTE)m_pEndBoundaryPostParam->pBuffer, m_pEndBoundaryPostParam->nBufferSize, NULL));
		}
		return bRet;
	}

	BOOL CFUploadJob::_SendUploadData()
	{
		BOOL bRet = FALSE;
		DWORD dwBufferSize = INTERNET_BUFFER_SIZE;

		PBYTE pBuffer = new BYTE[dwBufferSize];
		ZeroMemory(pBuffer, dwBufferSize);

		INTERNET_BUFFERS InternetBufferIn = {0};
		InternetBufferIn.dwStructSize = sizeof ( INTERNET_BUFFERS );
		InternetBufferIn.Next = NULL;
		
		//TODO:如果使用 HttpSendRequestEx 来发送所有的数据，则可以一次全部发送？
		//InternetBufferIn.dwBufferTotal = GetFileSize() + GetPostParamSize();

		API_VERIFY(HttpSendRequestEx(m_hRequest, &InternetBufferIn, NULL, HSR_INITIATE, 0));
		if (bRet)
		{
		    API_VERIFY(_SendPostArgument(pBuffer, dwBufferSize) 
				&& _SendLocalFile(pBuffer, dwBufferSize)
				&& _SendEndBoundary(pBuffer, dwBufferSize));
		}
		if (bRet)
		{
			API_VERIFY(HttpEndRequest( m_hRequest, NULL, HSR_INITIATE, 0 ));
		}

		SAFE_DELETE_ARRAY(pBuffer);
		return bRet;
	}

	BOOL CFUploadJob::_SendRequest()
	{
		BOOL bRet = FALSE;
		DWORD dwFlags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE; //INTERNET_FLAG_DONT_CACHE

		API_VERIFY(NULL != (m_hRequest = ::HttpOpenRequest(m_hConnection,
			_T("POST"), 
			m_JobParam->m_strObjectName, 
			NULL, NULL, NULL, dwFlags, NULL)));
		if (bRet)
		{
			API_VERIFY(_SetRequestHeader());
		}
		if (bRet)
		{
			API_VERIFY(_SendUploadData());
		}

		return bRet;
	}

	BOOL CFUploadJob::_ReceiveResponse()
	{
		BOOL bRet = FALSE;
		DWORD nContentLength = 0; //4G  -- HTTP_QUERY_FLAG_NUMBER64
		DWORD dwInfoSize = sizeof(nContentLength);

		API_VERIFY(::HttpQueryInfo(m_hRequest, HTTP_QUERY_FLAG_NUMBER | HTTP_QUERY_CONTENT_LENGTH, 
			&nContentLength, &dwInfoSize, NULL));
		//if (bRet)
		{
			DWORD dwRead = 0;
			DWORD dwBufferSize = INTERNET_BUFFER_SIZE;
			CHAR* pBuffer = new CHAR[dwBufferSize];
			if (pBuffer)
			{
				CAtlStringA strResponse;
				do 
				{
#ifdef FTL_DEBUG
					ZeroMemory(pBuffer, dwBufferSize);
#endif
					dwBufferSize = INTERNET_BUFFER_SIZE;
					API_VERIFY(::InternetReadFile(m_hRequest, pBuffer, dwBufferSize - 1, &dwRead));
					if (bRet && dwRead > 0)
					{
						pBuffer[dwRead] = NULL;
						strResponse.AppendFormat("%s", pBuffer);
						//m_JobParam->m_strResponseData += std::string((char)pBuffer, dwRead);
						//m_strResponse += std::string((char)pBuffer, dwRead);
					}
				} while (dwRead >0 && (GetJobWaitType(0) != ftwtStop));
				SAFE_DELETE_ARRAY(pBuffer);
				m_JobParam-> m_strResponseData = strResponse;
			}
			else
			{
				SetLastError(ERROR_OUTOFMEMORY);
				bRet = FALSE;
			}
		}
		return bRet;
	}

	void CFUploadJob::_OnClose()
	{
		_ClearPostParams();
	}

	LONG64 CFUploadJob::_CalcContentLength()
	{
		//计算总共的长度：post参数长度 + 文件长度(注意文件内容分割的长度) -- 和nTalk等其他项目的代码不一样(依赖于Post参数的创建方式中是否包含多余的CRLF等)

		LONG64 nContentLength = 0;

		for (std::list<PostArgumentParam*>::iterator iterArgument = m_postArgumentParams.begin();
			iterArgument != m_postArgumentParams.end();
			++iterArgument)
		{
			PostArgumentParam* pArgumentParam = *iterArgument;
			nContentLength += pArgumentParam->nBufferSize;
		}

		for (std::list<PostFileParam*>::iterator iterFile = m_postFileParams.begin();
			iterFile != m_postFileParams.end();
			++iterFile)
		{
			PostFileParam* pFileParam = *iterFile;
			nContentLength += (pFileParam->nBufferSize + pFileParam->dwFileSize + 2); //每个文件内容分割处有 \r\n
		}
		
		if (m_pEndBoundaryPostParam)
		{
			nContentLength += m_pEndBoundaryPostParam->nBufferSize;
		}

		FTLTRACEEX(tlTrace, TEXT("CFUploadJob::_CalcContentLength nJobId=%d, ContentLength=%ld\n"), 
			GetJobIndex(), nContentLength);
		return nContentLength;
	}

	BOOL CFUploadJob::_SetRequestHeader()
	{
		BOOL bRet = TRUE;

		for (TransferParamContainer::iterator iter = m_JobParam->m_transferParams.begin();
			iter != m_JobParam->m_transferParams.end();
			++iter)
		{
			FTransferParam& transParam = *iter;
			switch (transParam.paramType)
			{
			case tptRequestHeader:
				{
					if (!transParam.strName.IsEmpty())
					{
						CAtlString strHeader;
						strHeader.Format(TEXT("%s: %s\r\n"), transParam.strName, transParam.strValue);
						FTLTRACE(TEXT("_SetRequestHeader >>>>  %s\n"), strHeader);

						API_VERIFY(::HttpAddRequestHeaders(m_hRequest, strHeader, 
							strHeader.GetLength(), HTTP_ADDREQ_FLAG_ADD_IF_NEW));
						if (!bRet)
						{
							break;
						}
					}
				}
				break;
			default:
				break;
			}
		}
		return bRet;
	}

	CFDownloadJob::CFDownloadJob(const CAtlString& strAgent)
	:CFTransferJobBase(strAgent)
	{
	}

	BOOL CFDownloadJob::_CheckParams()
	{
		BOOL bHaveLocalFile = FALSE;
		for (TransferParamContainer::iterator iter = m_JobParam->m_transferParams.begin();
			iter != m_JobParam->m_transferParams.end();
			++iter)
		{
			FTransferParam& transParam = *iter;
			if (transParam.paramType == tptLocalFile)
			{
				bHaveLocalFile = TRUE;
				m_strLocalFilePath = transParam.strValue;
				break;
			}
		}
		if(!bHaveLocalFile)
		{
			SetLastError(ERROR_INVALID_PARAMETER);
		}
		return bHaveLocalFile;
	}

	BOOL CFDownloadJob::_SendRequest()
	{
		BOOL bRet = FALSE;

		DWORD dwFlags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE; //INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_KEEP_CONNECTION
		API_VERIFY(NULL != (m_hRequest = ::HttpOpenRequest(m_hConnection, _T("GET"), m_JobParam->m_strObjectName, 
			NULL, NULL, NULL, dwFlags, NULL)));
		if (!bRet)
		{
			return bRet;
		}

		API_VERIFY(::HttpSendRequest(m_hRequest, NULL, 0, NULL, 0));
		if (bRet)
		{
			DWORD dwStatusCode = 0;
			DWORD dwInfoSize = sizeof(dwStatusCode);
			API_VERIFY(::HttpQueryInfo(m_hRequest, HTTP_QUERY_FLAG_NUMBER | HTTP_QUERY_STATUS_CODE, &dwStatusCode,
				&dwInfoSize, NULL));
			if (bRet)
			{
				FTLTRACEEX(tlError, TEXT("CFDownloadJob::_SendRequest, StatusCode is %d\n"), dwStatusCode);
				bRet = FALSE;
				DWORD dwError = ERROR_SUCCESS;
				switch (dwStatusCode)
				{
				case HTTP_STATUS_OK:				//200
				case HTTP_STATUS_NOT_MODIFIED:		//304
					bRet = TRUE;
					break;
				case HTTP_STATUS_NOT_FOUND:			//404
					dwError = ERROR_NOT_FOUND;
					break;
				//需要身份验证(代理、服务器等) -- 需要提供给子类处理的机制
				case HTTP_STATUS_DENIED:			//401
				case HTTP_STATUS_FORBIDDEN:			//403
				case HTTP_STATUS_PROXY_AUTH_REQ:	//407
				default:
					dwError = ERROR_NOT_AUTHENTICATED;
					break;
				}
				if (!bRet)
				{
					FTLASSERT(FALSE);
					//_NotifyResult(jobInfo, IFInternetCallback::ecERROR, dwStatusCode, TEXT("HttpOpenRequest"));
				}
			}
		}
		return bRet;
	}

	BOOL CFDownloadJob::_ReceiveResponse()
	{
		BOOL bRet = FALSE;
		DWORD nContentLength = 0; //4G  -- HTTP_QUERY_FLAG_NUMBER64
		DWORD dwInfoSize = sizeof(nContentLength);

		API_VERIFY(::HttpQueryInfo(m_hRequest, HTTP_QUERY_FLAG_NUMBER | HTTP_QUERY_CONTENT_LENGTH, 
			&nContentLength, &dwInfoSize, NULL));
		if (bRet)
		{
#pragma TODO(考虑本地文件重复需要重命名或删除等)

			m_nTotalSize = nContentLength;
			HANDLE hLocalFile = ::CreateFile(m_strLocalFilePath, GENERIC_WRITE, 0, NULL,
				CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			API_VERIFY(INVALID_HANDLE_VALUE != hLocalFile);

			if (INVALID_HANDLE_VALUE != hLocalFile)
			{
				_NotifyProgress(m_nCurPos, m_nTotalSize);
					
				DWORD dwSize = 0, dwRead = 0;
				DWORD dwWriteToLocal = 0;
				DWORD dwBufferSize = INTERNET_BUFFER_SIZE;
				//BOOL  bFirstBuffer = TRUE;
				BYTE* pBuffer = new BYTE[dwBufferSize];
				do 
				{
#ifdef FTL_DEBUG
					ZeroMemory(pBuffer, dwBufferSize);
#endif
					//TODO: 是否需要调用这个函数，及其作用和返回值? 如果真取得 dwRead 大小，还要和 INTERNET_BUFFER_SIZE 比较
					//API_VERIFY(InternetQueryDataAvailable(m_hRequest, &dwRead, 0, 0));

					dwSize = INTERNET_BUFFER_SIZE;
					API_VERIFY(::InternetReadFile(m_hRequest, pBuffer, dwSize, &dwRead));
					if (bRet && dwRead > 0)
					{
						//if (_CheckInternetBuffer(pBuffer, dwRead))
						{
#pragma TODO(_CheckInternetBuffer)
							//对网络上读取到的数据进行检查, 防止各个流程都正确，但文件内容错误(比如：没有进行身份验证时往往会被重定向到登录网站)
							API_VERIFY(WriteFile(hLocalFile, pBuffer, dwRead, &dwWriteToLocal, NULL));
							if (!bRet || dwRead != dwWriteToLocal)
							{
								_NotifyError(GetLastError(), TEXT("WriteFile"));
								break;
							}
							m_nCurPos += dwWriteToLocal;
							_NotifyProgress(m_nCurPos, m_nTotalSize);
						}
						//else
						//{
						//}
					}
				} while (dwRead != 0 && (GetJobWaitType(0) != ftwtStop));
				SAFE_DELETE_ARRAY(pBuffer);

				if (dwRead == 0 && (GetJobWaitType(0) != ftwtStop))
				{
					_NotifyProgress(m_nCurPos, m_nTotalSize);
					bRet = TRUE;
				}
				else if(GetJobWaitType(0) == ftwtStop)
				{
					SetLastError(ERROR_CANCELLED);
					bRet = FALSE;
				}
				CloseHandle(hLocalFile);
			}
			if (GetJobWaitType(0) == ftwtStop)
			{
				BOOL bTempRet = ::DeleteFile(m_strLocalFilePath);
				API_ASSERT(bTempRet);
			}
		}
		return bRet;
	}


	CFInternetTransfer::CFInternetTransfer(  )
	{
		m_pThreadPool = NULL;
		m_strAgent = TEXT("InternetTransfer");
	}

	CFInternetTransfer::~CFInternetTransfer()
	{
		Stop();
		Close();
	}
	
	BOOL CFInternetTransfer::IsStarted()
	{
		BOOL bRet = ((NULL != m_pThreadPool));
		return bRet;
	}

	BOOL CFInternetTransfer::Start(IFThreadPoolCallBack<FTransferJobInfoPtr>* pCallBack /* = NULL */, 
		LONG nMinParallelCount /* = 1 */, 
		LONG nMaxParallelCount /* = 4 */, 
		LPCTSTR pszAgent /* = NULL*/)
	{
		BOOL bRet = FALSE;
		
		FTLASSERT( NULL == m_pThreadPool );
		FTLASSERT(nMinParallelCount >= 1);
		FTLASSERT(nMinParallelCount <= nMaxParallelCount);
		m_pThreadPool = new CFThreadPool<FTransferJobInfoPtr>(pCallBack);
		if (m_pThreadPool)
		{
			API_VERIFY(m_pThreadPool->Start(nMinParallelCount, nMaxParallelCount));
			if (pszAgent)
			{
				m_strAgent = pszAgent;
			}
		}
		else
		{
			SetLastError(ERROR_OUTOFMEMORY);
			bRet = FALSE;
		}

		return bRet;
	}

	BOOL CFInternetTransfer::Stop()
	{
		BOOL bRet = TRUE;
		if (m_pThreadPool)
		{
			API_VERIFY(m_pThreadPool->StopAndWait());
		}
		return bRet;
	}

	void CFInternetTransfer::Close()
	{
		SAFE_DELETE(m_pThreadPool);
	}

	LONG CFInternetTransfer::AddUploadTask(FTransferJobInfoPtr pUploadJobInfo)
	{
		FTLASSERT(m_pThreadPool);
		FTLASSERT(pUploadJobInfo);
		//FTLASSERT(pUploadJobInfo->m_bUploadJob);

		BOOL bRet = FALSE;
		LONG nJobIndex = 0;
		if (m_pThreadPool)
		{
			CFUploadJob* pJob = new CFUploadJob(m_strAgent);
			pJob->m_JobParam = pUploadJobInfo;
			API_VERIFY(m_pThreadPool->SubmitJob(pJob, &nJobIndex));
		}
		return nJobIndex;
	}

	LONG CFInternetTransfer::AddDownloadTask(FTransferJobInfoPtr pDownloadJobInfo)
	{
		FTLASSERT(m_pThreadPool);
		FTLASSERT(pDownloadJobInfo);
		//FTLASSERT(!pDownloadJobInfo->m_bUploadJob);

		BOOL bRet = FALSE;
		LONG nJobIndex = 0;
		if (m_pThreadPool)
		{
			CFDownloadJob* pDownloadJob = new CFDownloadJob(m_strAgent);
			pDownloadJob->m_JobParam = pDownloadJobInfo;
			API_VERIFY(m_pThreadPool->SubmitJob(pDownloadJob, &nJobIndex));
		}
		return nJobIndex;

	}

	BOOL CFInternetTransfer::CancelTask(LONG nJobIndex)
	{
		FTLASSERT(m_pThreadPool);
		BOOL bRet = FALSE;
		if (m_pThreadPool)
		{
			API_VERIFY(m_pThreadPool->CancelJob(nJobIndex));
		}
		return bRet;
	}

	//////////////////////////////////////////////////////////////////////////

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

		//bRet = ::InternetGetCookie(IDS_LOGIN_COOKIE_DOMAIN, NULL, szCookie, &len); 
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
		//获取参数列表(比如 Content-Type、Accept、Content-Length、 User-Agent、Connection、Cookie 等)，循环发送出去
		//while(xxxx)
		{
			API_VERIFY(::HttpAddRequestHeaders( m_hRequest, 
				strHeader.GetBuffer(), strHeader.GetLength(), //__HTTP_ACCEPT, -1, 
				HTTP_ADDREQ_FLAG_ADD_IF_NEW));
			if (!bRet)
			{
				m_dwError=::GetLastError();
				//return FALSE;
			}
		}

		// SEND REQUEST
		INTERNET_BUFFERS InternetBufferIn = {0};
		InternetBufferIn.dwStructSize = sizeof(INTERNET_BUFFERS);
		InternetBufferIn.Next = NULL;
		API_VERIFY(::HttpSendRequestEx( m_hRequest,	// handle by returned HttpOpenRequest
			NULL, // additional HTTP header
			0, // additional HTTP header length
			HSR_INITIATE, // additional data in HTTP Post or HTTP Put
			0)); // additional data length
		//if(!bRet)
		//{
		//	m_dwError=::GetLastError();
		//	return FALSE;
		//}

		//循环 发送数据
		while(...)
		{
			DWORD dwOutPostBufferLength = 0;
			bRet = ::InternetWriteFile(m_hRequest, buffToPost.GetCurrentBuffer(), nBufferSize, &dwOutPostBufferLength);
		}

		bRet = ::HttpEndRequest(m_hRequest, NULL, HSR_INITIATE, 0);

		//获取结果
		const int RESPONSE_MAX_LENGTH = 1024;
		char byData[RESPONSE_MAX_LENGTH + 1];
		DWORD dwReadByte = 0;
		bRet = ::InternetReadFile(m_hRequest, byData, RESPONSE_MAX_LENGTH, &dwReadByte) ;

		return bRet;
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
				COM_VERIFY(spWebBrowser2->get_Offline(&bOffline));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("Offline"), bOffline));
			}
		}
		return hr;
	}

}//namespace FTL

#pragma warning( pop )

#endif //FTL_NET_HPP