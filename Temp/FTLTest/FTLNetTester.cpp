#include "StdAfx.h"
#include "FTLNetTester.h"
#include <ftlNet.h>
#include <ftlSocket.h>
#include <ftlConversion.h>
#include <atlpath.h>

//#include <tuple>
#pragma comment( lib, "Iphlpapi.lib" )
void CFTLNetTester::dump_allAdaptersInfo()
{
    //显示所有 Enable 的网络适配器的地址信息(注意:Disable的无法获取)
    BOOL bRet = FALSE;
    IP_ADAPTER_INFO AdapterInfo[16] = { NULL };
    DWORD dwBufLen = sizeof(AdapterInfo);

    DWORD adpIndex = 0;
    DWORD dwStatus = 0;
    API_VERIFY(ERROR_SUCCESS == (dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen)));
    if (bRet)
    {
        PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
        while (pAdapterInfo)
        {
            FTLASSERT(pAdapterInfo->ComboIndex == pAdapterInfo->Index); //TODO:实测这两个值一样, 有什么区别?

            adpIndex++;
            //将16进制的Mac地址转换为易读的方式
            LONG nStrBinaryCount = 0;
            API_VERIFY(CFConvUtil::HexFromBinary(pAdapterInfo->Address, pAdapterInfo->AddressLength, NULL, &nStrBinaryCount, _T('-')));
            CFMemAllocator<TCHAR> AddressBuf(nStrBinaryCount);
            API_VERIFY(CFConvUtil::HexFromBinary(pAdapterInfo->Address, pAdapterInfo->AddressLength, AddressBuf.GetMemory(), &nStrBinaryCount, _T('-')));
            
            FTLTRACE(TEXT("[%d]:ComboIdx=%d, Idx=%d,Type=%d Desc=%s, AdapterName=%s, AddressLength=%d, Address=%s\n"), 
                adpIndex,
                pAdapterInfo->ComboIndex,
                pAdapterInfo->Index,
                pAdapterInfo->Type, //目前实测都是 6, TODO: 具体对应的枚举
                CFConversion().MBCS_TO_TCHAR(pAdapterInfo->Description),    //描述信息, 如: Realtek PCIe GBE Family Controller
                CFConversion().MBCS_TO_TCHAR(pAdapterInfo->AdapterName),    //GUID, 如 {6CF21E5D-0708-4F63-903E-47208B4F51EB}
                pAdapterInfo->AddressLength, AddressBuf.GetMemory());       //MAC地址(Address)的16位表示, 如: 8c-89-a5-3d-6d-d7

            //其他可以获取到IP地址等
            pAdapterInfo = pAdapterInfo->Next;
        }
    }
}

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
    //要Vista才默认支持？
    inet_pton(AF_INET6, "fishjam", &addr.sin6_addr);
    char strName[INET6_ADDRSTRLEN] = {0};
    char* pStr = inet_ntop(AF_INET6, &addr.sin6_addr, strName, sizeof(strName));
#endif 
}

void CFTLNetTester::test_UrlEscape()
{
	HRESULT hr = E_FAIL;
	//char unsafe;=> %hex(asc(unsafe))
	//MSDN中的例子
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
		
		//重新初始化Buffer大小
		dwBufferSize = INTERNET_MAX_URL_LENGTH;
	}

	//中文
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

void CFTLNetTester::test_UrlMonFunctions()
{
    HRESULT hr = E_FAIL;
    CHAR szUserAgent[4096] = {0};
    DWORD dwSize = _countof(szUserAgent) - 1;
    COM_VERIFY(ObtainUserAgentString(0, szUserAgent, &dwSize));
    //FTLTRACEA("userAgent=%s\n", szUserAgent);
}

void CFTLNetTester::test_DownloadMethods()
{
    BOOL bRet = FALSE;
    HRESULT hr = E_FAIL;
    CFElapseCounter counter;

    LPCTSTR STR_DOWNLOAD_FILE_URL = TEXT("http://vaccine.dn.naver.com/naver/vaccine/NaverSecurity64/toolbar/NaverTBVaccine64.exe");
    
    TCHAR szLocalPath[MAX_PATH] = {0};
    API_VERIFY( 0 < GetModuleFileName(NULL, szLocalPath, _countof(szLocalPath)));
    CPath   path(szLocalPath);
    API_VERIFY(path.RemoveFileSpec());
    CAtlString strLocalFolderPath = path;

    API_VERIFY(path.Append(TEXT("UrlDownloadFile_NaverTBVaccine64.exe")));

    API_VERIFY(DeleteUrlCacheEntry(STR_DOWNLOAD_FILE_URL));     //如果不调用该函数, 已经下载过的话,不会重新下载

    API_VERIFY(counter.Start());
    COM_VERIFY(URLDownloadToFile(NULL, STR_DOWNLOAD_FILE_URL, path, 0, NULL));
    API_VERIFY(counter.Stop());
    FTLTRACE(TEXT("URLDownloadToFile Elapse %d(ms)\n"), counter.GetElapseTime() / NANOSECOND_PER_MILLISECOND);
}

void CFTLNetTester::test_FireWallInterface()
{
    HRESULT hr = E_FAIL;

    COM_VERIFY(CoInitialize(NULL));

    CComPtr<INetFwMgr>      spNetFwMgr;
    CComPtr<INetFwPolicy>   spNetFwPolicy;
    CComPtr<INetFwProfile>  spNetFwProfile;
    CComPtr<INetFwAuthorizedApplications>   spNetFwAuthorizedApplications;

    COM_VERIFY(spNetFwMgr.CoCreateInstance(__uuidof(NetFwMgr), NULL));
    if (spNetFwMgr)
    {
        COM_VERIFY(spNetFwMgr->get_LocalPolicy(&spNetFwPolicy));
        if (spNetFwPolicy)
        {
            COM_VERIFY(spNetFwPolicy->get_CurrentProfile(&spNetFwProfile));
            if (spNetFwProfile)
            {
                COM_VERIFY(spNetFwProfile->get_AuthorizedApplications(&spNetFwAuthorizedApplications));
                if (spNetFwAuthorizedApplications)
                {
                    long nAuthorizedApplicationCount = 0;
                    COM_VERIFY(spNetFwAuthorizedApplications->get_Count(&nAuthorizedApplicationCount));
                    FTLTRACE(TEXT("nAuthorizedApplicationCount=%d\n"), nAuthorizedApplicationCount);
                    CComPtr<IUnknown> spEnum;
                    COM_VERIFY(spNetFwAuthorizedApplications->get__NewEnum(&spEnum));
                    CComQIPtr<IEnumVARIANT>     spEnumAuthorizedApp = spEnum;
                    if (spEnumAuthorizedApp)
                    {
                        //COM_DETECT_INTERFACE_FROM_LIST(spEnumAuthorizedApp);
                    }
                }
            }
        }

    }

    CoUninitialize();
}