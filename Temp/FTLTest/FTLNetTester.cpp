#include "StdAfx.h"
#include "FTLNetTester.h"
#include <ftlNet.h>
#include <ftlSocket.h>
//#include <tuple>

void CFTLNetTester::test_FNetServerT()
{
}

void CFTLNetTester::test_FSocketAddress()
{
    FTL::CFWinsockEnvInit sockEnvInit;

    FTL::CFStringFormater   formater;
    FTL::CFSocketAddress addrV4String(TEXT("192.168.0.1:80"), 80);
    addrV4String.ToString(formater);
    CPPUNIT_ASSERT(lstrcmp(TEXT("192.168.0.1:80"), formater.GetString()) == 0);


}

void CFTLNetTester::test_GetNetInfo()
{
    FTL::FNetInfo::GetAddressFamily(AF_INET);
    FTL::FNetInfo::GetProtocolType(AF_INET,IPPROTO_IP);
    FTL::FNetInfo::GetSocketType(SOCK_STREAM);

    //int rc = NO_ERROR;
    WSASetLastError(WSA_INVALID_HANDLE);
    //NET_VERIFY(SOCKET_ERROR);
    CFNetErrorInfo netError(WSAGetLastError());
    CPPUNIT_ASSERT(_tcscmp(netError.GetConvertedInfo(),TEXT("WSA_INVALID_HANDLE")) == 0) ;
}

void CFTLNetTester::test_IpV6()
{
#if 0
    sockaddr_in6 addr = {0};
    addr.sin6_family = AF_INET6;
    addr.sin6_port = htons(21);
    //ҪVista��Ĭ��֧�֣�
    inet_pton(AF_INET6, "fishjam", &addr.sin6_addr);
    char strName[INET6_ADDRSTRLEN] = {0};
    char* pStr = inet_ntop(AF_INET6, &addr.sin6_addr, strName, sizeof(strName));
#endif 
}

void CFTLNetTester::test_UrlEscape()
{
	HRESULT hr = E_FAIL;
	//char unsafe;=> %hex(asc(unsafe))
	//MSDN�е�����
	CString strSourceURL = TEXT("test/t%e<s t.asp?url=/{ex% ample</abc.asp?frame=true#fr%agment");
	typedef std::pair<DWORD, CString> UrlEscapeCheckValueType;

	UrlEscapeCheckValueType checkValues[] = 
	{
		std::make_pair(URL_ESCAPE_SPACES_ONLY, 
			TEXT("test/t%e<s%20t.asp?url=/{ex%%20ample</abc.asp?frame=true#fr%agment")),
		std::make_pair(URL_ESCAPE_SPACES_ONLY | URL_DONT_ESCAPE_EXTRA_INFO, 
			TEXT("test/t%e<s%20t.asp?url=/{ex% ample</abc.asp?frame=true#fr%agment")),
		std::make_pair(URL_ESCAPE_PERCENT , 
			TEXT("test/t%25e%3Cs%20t.asp?url=/{ex% ample</abc.asp?frame=true#fr%agment")),
		std::make_pair(URL_ESCAPE_SEGMENT_ONLY, 
			TEXT("test%2Ft%e%3Cs%20t.asp%3Furl=%2F%7Bex%%20ample%3C%2Fabc.asp%3Fframe=true%23fr%agment")),
	};
	
	DWORD dwBufferSize = INTERNET_MAX_URL_LENGTH;
	FTL::CFMemAllocator<TCHAR> szTargetURL(dwBufferSize);
	for (int i = 0; i < _countof(checkValues); i++)
	{
		COM_VERIFY(UrlEscape(strSourceURL, (TCHAR*)szTargetURL, &dwBufferSize, checkValues[i].first));
		CPPUNIT_ASSERT(checkValues[i].second.Compare((TCHAR*)szTargetURL) == 0);
		
		//���³�ʼ��Buffer��С
		dwBufferSize = INTERNET_MAX_URL_LENGTH;
	}

	//����
}

void CFTLNetTester::test_WinSocket()
{
    WORD wVersionRequested = MAKEWORD( 2, 2 );
    WSADATA wsaData = {0};
    int rc = NO_ERROR;
    NET_VERIFY(WSAStartup(wVersionRequested,&wsaData));
    if (NO_ERROR == rc)
    {
        FTLTRACEEX(FTL::tlTrace, 
            TEXT("WSAStartup Success : wVersion=0x%04x, wHighVersion=0x%04x, iMaxSockets=%d, iMaxUdpDg=%d, ")
            TEXT("szDescription=\"%s\", szSystemStatus=\"%s\", lpVendorInfo=\"%s\"\n"),
            wsaData.wVersion, wsaData.wHighVersion,wsaData.iMaxSockets, wsaData.iMaxUdpDg,
            wsaData.szDescription ? CA2T(wsaData.szDescription) : TEXT("No Description"),
            wsaData.szSystemStatus? CA2T(wsaData.szSystemStatus): TEXT("No SystemStatus"), 
            wsaData.lpVendorInfo ?  CA2T(wsaData.lpVendorInfo)  : TEXT("No VendorInfo"));
        WSACleanup();
    }
}