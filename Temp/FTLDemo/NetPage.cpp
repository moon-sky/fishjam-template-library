// NetPage.cpp : 实现文件
//

#include "stdafx.h"
#include "FTLDemo.h"
#include "NetPage.h"
#include <ftlNet.h>
#include <ftlSocket.h>
#include <ftlFile.h>

#define USER_AGENT	TEXT("FTLDemoAgent")


// CNetPage 对话框

IMPLEMENT_DYNAMIC(CNetPage, CPropertyPage)

CNetPage::CNetPage()
	: CPropertyPage(CNetPage::IDD)
	, m_strServerHost(_T("www.baidu.com"))
	, m_pMyServer(NULL)
	, m_hSession(NULL)
	, m_hConnect(NULL)
	, m_hRequest(NULL)
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
    }

	m_pInternetTranser = NULL;
	m_nJobIndex = 0;
}

CNetPage::~CNetPage()
{
	SAFE_DELETE(m_pInternetTranser);
    WSACleanup();
}

void CNetPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NET_SERVER, m_strServerHost);
}


BEGIN_MESSAGE_MAP(CNetPage, CPropertyPage)
    ON_BN_CLICKED(IDC_BTN_START_SERVER, &CNetPage::OnBnClickedBtnStartServer)
    ON_BN_CLICKED(IDC_BTN_STOP_SERVER, &CNetPage::OnBnClickedBtnStopServer)
	ON_BN_CLICKED(IDC_BTN_WININET_CONNECT, &CNetPage::OnBnClickedBtnWininetConnect)
	ON_BN_CLICKED(IDC_BTN_WINHTTP_CONNECT, &CNetPage::OnBnClickedBtnWinhttpConnect)

	ON_BN_CLICKED(IDC_BTN_INTERNET_TRANSFER_ADD_DOWNLOAD, &CNetPage::OnBnClickedBtnInternetTransferAddDownload)
	ON_BN_CLICKED(IDC_BTN_INTERNET_TRANSFER_ADD_UPLOAD, &CNetPage::OnBnClickedBtnInternetTransferAddUpload)
	ON_BN_CLICKED(IDC_BTN_INTERNET_TRANSFER_CANCEL_JOB, &CNetPage::OnBnClickedBtnInternetTransferCancelJob)
	ON_BN_CLICKED(IDC_BTN_INTERNET_TRANSFER_STOP, &CNetPage::OnBnClickedBtnInternetTransferStop)

    ON_BN_CLICKED(IDC_BTN_SOCKET_DUMP_OPTION, &CNetPage::OnBnClickedBtnSocketDumpOption)
    ON_BN_CLICKED(IDC_BTN_NET_TRANSMITFILE, &CNetPage::OnBnClickedBtnTransmitFile)
END_MESSAGE_MAP()


// CNetPage 消息处理程序

void CNetPage::SetButtonStatus(BOOL bServerEnabled)
{
    GetDlgItem(IDC_BTN_START_SERVER)->EnableWindow(!bServerEnabled);
    GetDlgItem(IDC_BTN_STOP_SERVER)->EnableWindow(bServerEnabled);
}

void CNetPage::OnBnClickedBtnStartServer()
{
    int rc = NO_ERROR;
    m_pMyServer = new CMyNetServer(FTL::stTCP);
    NET_VERIFY(m_pMyServer->Create(5555,INADDR_ANY));
    if (NO_ERROR == rc)
    {
        NET_VERIFY(m_pMyServer->Start(5,10));
        //SetButtonStatus(TRUE);
    }
}

void CNetPage::OnBnClickedBtnStopServer()
{
    int rc = NO_ERROR;
	if (m_pMyServer)
	{
		NET_VERIFY(m_pMyServer->Stop());
		NET_VERIFY(m_pMyServer->Destroy());
		SAFE_DELETE(m_pMyServer);
	}
    //SetButtonStatus(FALSE);
}

void CNetPage::OnBnClickedBtnWininetConnect()
{
	BOOL bRet = FALSE;
	API_VERIFY(UpdateData(TRUE));

	m_hSession = InternetOpen(USER_AGENT, INTERNET_OPEN_TYPE_PRECONFIG, NULL, 0, 0);
	API_VERIFY(NULL != m_hSession);

	m_hConnect = InternetConnect(m_hSession, m_strServerHost, 0, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	API_VERIFY(NULL != m_hConnect);

	//可以设置 用户(INTERNET_OPTION_USERNAME) 和 密码(INTERNET_OPTION_PASSWORD)
	//InternetSetOption(m_hConnect,  INTERNET_OPTION_PASSWORD, pszPass, lstrlen(pszPass) + 1);
  
	DWORD dwFlags = INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_RELOAD;  // flags: keep-alive, bypass cache
	m_hRequest = HttpOpenRequest(m_hConnect, TEXT("GET"), TEXT("/"), NULL, NULL, NULL, dwFlags, NULL);
	API_VERIFY(NULL != m_hRequest);

	API_VERIFY(HttpSendRequest(m_hRequest, NULL, 0,	NULL, 0));

	BYTE buf[INTERNET_BUFFER_SIZE] = {0};
	DWORD dwRead = 0;
	while (InternetReadFile(m_hRequest, buf, sizeof(buf) - 1, &dwRead))
	{
		if (dwRead == 0)
		{
			break;
		}
		buf[dwRead] = NULL;
		OutputDebugStringA((LPCSTR)buf);
	}
	SAFE_CLOSE_INTERNET_HANDLE(m_hRequest);
	SAFE_CLOSE_INTERNET_HANDLE(m_hConnect);
	SAFE_CLOSE_INTERNET_HANDLE(m_hSession);
}

void CNetPage::OnBnClickedBtnWinhttpConnect()
{
#ifdef USE_WIN_HTTP
	BOOL bRet = FALSE;
	UpdateData(TRUE);

	//使用 NTLM 进行HTTP 认证的例子(MS 的 AuthDefaultCred)
	
	// Use WinHttpOpen to obtain a session handle and specify no proxy  
	m_hSession = WinHttpOpen(USER_AGENT, WINHTTP_ACCESS_TYPE_NO_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

	// Use WinHttpConnect to specify target server and port
	m_hConnect = WinHttpConnect(m_hSession, m_strServerHost, INTERNET_DEFAULT_HTTP_PORT, 0);

	// Use WinHttpOpenRequest to open a GET request and specify target path
	m_hRequest = WinHttpOpenRequest(m_hConnect, TEXT("GET"), TEXT("/"), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES,	0);

	DWORD dwAutoLogonPolicy = WINHTTP_AUTOLOGON_SECURITY_LEVEL_LOW;
	// Use WinHttpSetOption to set auto logon policy to low level to include the default credentials in a request
	API_VERIFY(WinHttpSetOption(m_hRequest,	WINHTTP_OPTION_AUTOLOGON_POLICY, &dwAutoLogonPolicy, sizeof(dwAutoLogonPolicy)));

	// Use WinHttpSetCredentials with NULL username and password to use default credentials
	API_VERIFY(WinHttpSetCredentials(m_hRequest, WINHTTP_AUTH_TARGET_SERVER, WINHTTP_AUTH_SCHEME_NTLM, NULL, NULL,NULL));


	// Use WinHttpSendRequest to send the request and specify no additional headers and request data
	API_VERIFY(WinHttpSendRequest(m_hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0));

	// Use WinHttpReceiveResponse to receive the response
	API_VERIFY(WinHttpReceiveResponse(m_hRequest, NULL));

	DWORD dwStatusCode = 0;
	DWORD dwSize = sizeof(dwStatusCode);
	// Use WinHttpQueryHeaders to retrieve the status code
	API_VERIFY(WinHttpQueryHeaders(m_hRequest, WINHTTP_QUERY_FLAG_NUMBER | WINHTTP_QUERY_STATUS_CODE, NULL,	&dwStatusCode, &dwSize, NULL));

	// Expect to get status code 200 -- 正确
	FTLTRACE(TEXT("Got expected status code=%d\n"), dwStatusCode);

	SAFE_CLOSE_INTERNET_HANDLE(m_hRequest);
	SAFE_CLOSE_INTERNET_HANDLE(m_hConnect);
	SAFE_CLOSE_INTERNET_HANDLE(m_hSession);

#endif


}

BOOL CNetPage::OnSetActive()
{
	BOOL bRet = __super::OnSetActive();
	if (bRet)
	{
		API_VERIFY(UpdateData(FALSE));
	}
	return bRet;
}

void CNetPage::OnJobEnd(LONG nJobIndex, CFJobBase<FTransferJobInfoPtr>* pJob)
{
	UNREFERENCED_PARAMETER(nJobIndex);
	UNREFERENCED_PARAMETER(pJob);
	FTLTRACE(TEXT("CNetPage::OnJobEnd, nJobIndex=%d, Response=%s\n"),
		nJobIndex,  FTL::CFConversion().UTF8_TO_TCHAR(pJob->m_JobParam->m_strResponseData));
}
void CNetPage::OnJobProgress(LONG nJobIndex, CFJobBase<FTransferJobInfoPtr>* pJob, LONGLONG nCurPos, LONGLONG nTotalSize)
{
	UNREFERENCED_PARAMETER(nJobIndex);
	UNREFERENCED_PARAMETER(pJob);
	UNREFERENCED_PARAMETER(nCurPos);
	UNREFERENCED_PARAMETER(nTotalSize);

	FTLTRACE(TEXT("CNetPage::OnJobProgress, nJobIndex=%d, nCurPos=%lld, nTotalSize=%lld\n"),
		nJobIndex, nCurPos, nTotalSize);
}

void CNetPage::OnBnClickedBtnInternetTransferAddDownload()
{
	if (!m_pInternetTranser)
	{
		m_pInternetTranser = new FTL::CFInternetTransfer();
		m_pInternetTranser->Start(this, 1, 4, TEXT("InternetTransfer"));
	}

	CString strFileName;
	strFileName.Format(TEXT("newsreader-wyyy_news_%d.exe"), GetTickCount());
	CFileDialog dlg(FALSE, NULL, strFileName);
	if (dlg.DoModal() == IDOK)
	{
		//http://download.microsoft.com/download/5/D/8/5D8C65CB-C849-4025-8E95-C3966CAFD8AE/vcredist_x86.exe

		//http://file.m.163.com/app/free/201303/01/newsreader-wyyy_news.apk
		FTL::FTransferJobInfoPtr pDownloadJobInfo(new FTL::FTransferJobInfo());
		pDownloadJobInfo->m_strServerName = TEXT("file.m.163.com");
		//pDownloadJobInfo->m_bUploadJob = FALSE;
		pDownloadJobInfo->m_strObjectName = TEXT("/app/free/201303/01/newsreader-wyyy_news.apk");
		pDownloadJobInfo->m_nPort = 80;
		pDownloadJobInfo->AddTransferParam(tptLocalFile, TEXT(""), dlg.GetPathName());

		m_nJobIndex = m_pInternetTranser->AddDownloadTask(pDownloadJobInfo);
	}
}

void CNetPage::OnBnClickedBtnInternetTransferAddUpload()
{
	if (!m_pInternetTranser)
	{
		m_pInternetTranser = new FTL::CFInternetTransfer();
		m_pInternetTranser->Start(this, 1, 4, TEXT("InternetTransfer"));
	}
	
	CFileDialog	dlgOpen(TRUE);
	if (dlgOpen.DoModal() == IDOK)
	{

		FTL::FTransferJobInfoPtr pUploadJobInfo(new FTL::FTransferJobInfo());
		pUploadJobInfo->m_strServerName = TEXT("u.download.csdn.net");
		pUploadJobInfo->m_strObjectName = TEXT("/upload");
		pUploadJobInfo->m_nPort = 80;

		pUploadJobInfo->AddTransferParam(FTL::tptPostArgument, TEXT("txt_userfile"), TEXT("myTxtUserFile"));
		pUploadJobInfo->AddTransferParam(FTL::tptPostArgument, TEXT("txt_title"), TEXT("myTxtTitle"));
		pUploadJobInfo->AddTransferParam(FTL::tptLocalFile, TEXT("file"), dlgOpen.GetPathName());
		m_nJobIndex = m_pInternetTranser->AddUploadTask(pUploadJobInfo);
	}
}

void CNetPage::OnBnClickedBtnInternetTransferCancelJob()
{
	if (m_pInternetTranser)
	{
		m_pInternetTranser->CancelTask(m_nJobIndex--);
	}
}

void CNetPage::OnBnClickedBtnInternetTransferStop()
{
	BOOL bRet = FALSE;
	if (m_pInternetTranser)
	{
		API_VERIFY(m_pInternetTranser->Stop());
		SAFE_DELETE(m_pInternetTranser);
	}
}


void CNetPage::OnBnClickedBtnSocketDumpOption()
{
    int rc = SOCKET_ERROR;

    FTL::CFSocket tcpSocket;
    NET_VERIFY(tcpSocket.Open(stTCP, TRUE));
    CFSocketAddress tcpAddress(TEXT("www.baidu.com:80"), FALSE);
    NET_VERIFY(tcpSocket.Connect(tcpAddress));
    
    NET_VERIFY(FTL::CFSocketUtils::DumpSocketOption(tcpSocket.m_socket));
    NET_VERIFY(tcpSocket.Close());


    FTL::CFSocket udpSocket;
    NET_VERIFY(udpSocket.Open(stUDP, FALSE));
    NET_VERIFY(FTL::CFSocketUtils::DumpSocketOption(udpSocket.m_socket));
    NET_VERIFY(udpSocket.Close());
}

void CNetPage::OnBnClickedBtnTransmitFile()
{
    BOOL bRet = FALSE;
    int rc = SOCKET_ERROR;

    CFSocketAddress tcpAddress(TEXT("127.0.0.1:999"), FALSE);// TEXT("www.baidu.com"), 80);
    
    CFileDialog dlg(TRUE);
    if (dlg.DoModal() == IDOK)
    {
        FTL::CFFile fileSrc;
        API_VERIFY(fileSrc.Create(dlg.GetPathName(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING));

        if (tcpAddress.IsValid())
        {
            FTL::CFStringFormater formater;
            FTLTRACE(TEXT("SockAddress=%s\n"), tcpAddress.ToString(formater));

            FTL::CFSocket tcpSocket;
            NET_VERIFY(tcpSocket.Open(stTCP, TRUE));
            if (SOCKET_ERROR != rc)
            {
                NET_VERIFY(tcpSocket.Connect(tcpAddress));
                NET_VERIFY(tcpSocket.Send((const BYTE*)"GET /", 6, 0));

                DWORD dwLength = (DWORD)fileSrc.GetLength();
                //API_VERIFY(TransmitFile(tcpSocket.m_socket, fileSrc.m_hFile, 
                //    dwLength, 0, NULL, NULL, 0));

                CFMemAllocator<BYTE> resultBuffer(4096);
                int nResult = tcpSocket.Recv(resultBuffer.GetMemory(), resultBuffer.GetCount(), 0);

                FTLTRACE(TEXT("Result(%d) is \n"), nResult);

                tcpSocket.Close();
            }
        }
    }
}