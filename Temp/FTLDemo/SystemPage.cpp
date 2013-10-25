// SystemPage.cpp : 实现文件
//

#include "stdafx.h"
#include "FTLDemo.h"
#include "SystemPage.h"
#include <ftlsystem.h>
#include <ftlService.h>
#pragma comment(lib, "Userenv.lib")
#pragma comment(lib, "WtsApi32.lib")

// CSystemPage 对话框

IMPLEMENT_DYNAMIC(CSystemPage, CPropertyPage)

CSystemPage::CSystemPage()
	: CPropertyPage(CSystemPage::IDD)
{

}

CSystemPage::~CSystemPage()
{
}

void CSystemPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSystemPage, CPropertyPage)
    ON_BN_CLICKED(IDC_BTN_TEMP_FPU_RC_RESET, &CSystemPage::OnBnClickedBtnTempFpuRcReset)
	ON_BN_CLICKED(IDC_BTN_SYSTEM_METRICS, &CSystemPage::OnBnClickedBtnSystemMetrics)
    ON_BN_CLICKED(IDC_BTN_SYSTEM_CREATE_PROCESS_AS_USER, &CSystemPage::OnBnClickedBtnCreateProcessAsUser)
    ON_BN_CLICKED(IDC_BTN_SYSTEM_WAIT_ALL_CHILD_PROCESS, &CSystemPage::OnBnClickedBtnCreateProcessAndWaitAllChild)
    ON_BN_CLICKED(IDC_BTN_SYSTEM_GET_REG_VALUE_EXPORT_STRING, &CSystemPage::OnBnClickedBtnGetRegValueExportString)
    
END_MESSAGE_MAP()


// CSystemPage 消息处理程序

BOOL CSystemPage::OnInitDialog()
{
    BOOL bIs64Bit = CFSystemUtil::Is64BitWindows();
    SetDlgItemText(IDC_STATIC_IS64, bIs64Bit ? TEXT("TRUE") : TEXT("FALSE"));
    return TRUE;  // return TRUE unless you set the focus to a control
}

void CSystemPage::OnBnClickedBtnTempFpuRcReset()
{
    double f = 1.0 / 13.0;
    TRACE(TEXT("f = %f\n"),f);

    CFTempFpuRcReset rcReset;
    f = 1.0 / 13.0;
    TRACE(TEXT("f = %f\n"),f);
}

void CSystemPage::OnBnClickedBtnSystemMetrics()
{
	CFSystemMetricsProperty	systemMetricsProperty;
	FTLTRACE(TEXT("SystemMetricsProperty = %s\n"), systemMetricsProperty.GetPropertyString());
}

void CSystemPage::OnBnClickedBtnCreateProcessAsUser()
{
    BOOL bRet = FALSE;
    HANDLE hCurProcess = GetCurrentProcess();
    API_VERIFY(FTL::CFSystemUtil::EnableProcessPrivilege(hCurProcess, SE_CHANGE_NOTIFY_NAME, TRUE));
    API_VERIFY(FTL::CFSystemUtil::EnableProcessPrivilege(hCurProcess, SE_INCREASE_QUOTA_NAME, TRUE));
    API_VERIFY(FTL::CFSystemUtil::EnableProcessPrivilege(hCurProcess, SE_TCB_NAME, TRUE)); 
    API_VERIFY(FTL::CFSystemUtil::EnableProcessPrivilege(hCurProcess, SE_ASSIGNPRIMARYTOKEN_NAME, TRUE));
    API_VERIFY(FTL::CFService::CreateServiceUIProcess(TEXT("C:\\Windows\\System32\\calc.exe"), TRUE, NULL));
}

BOOL CreateProcessAndWaitAllChild(LPCTSTR pszPath)
{
    BOOL bRet = FALSE;

    CHandle Job(CreateJobObject(NULL, NULL));
    CHandle IOPort(CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 1));

    if(!IOPort) {
        return FALSE;
    }

    JOBOBJECT_ASSOCIATE_COMPLETION_PORT Port;
    Port.CompletionKey = Job;
    Port.CompletionPort = IOPort;
    API_VERIFY(SetInformationJobObject(Job, 
        JobObjectAssociateCompletionPortInformation, &Port, sizeof(Port)));
    PROCESS_INFORMATION ProcessInformation;
    STARTUPINFO StartupInfo = { sizeof(StartupInfo) };

    API_VERIFY(CreateProcess(pszPath, NULL, NULL, NULL, FALSE, 
        NORMAL_PRIORITY_CLASS | CREATE_SUSPENDED | CREATE_BREAKAWAY_FROM_JOB, 
        NULL, NULL, &StartupInfo, &ProcessInformation));
    BOOL bInJob = FALSE;
    API_VERIFY(IsProcessInJob(ProcessInformation.hProcess, Job, &bInJob));
    //CHandle handCopyProcess(OpenProcess(PROCESS_ALL_ACCESS | PROCESS_SET_QUOTA|PROCESS_TERMINATE, FALSE, ProcessInformation.dwProcessId));

    API_VERIFY(AssignProcessToJobObject(Job, ProcessInformation.hProcess)); 

    ResumeThread(ProcessInformation.hThread);
    CloseHandle(ProcessInformation.hThread);
    CloseHandle(ProcessInformation.hProcess);

    DWORD CompletionCode;
    ULONG_PTR CompletionKey;
    LPOVERLAPPED Overlapped;
    while (GetQueuedCompletionStatus(IOPort, &CompletionCode,&CompletionKey, 
        &Overlapped, INFINITE) 
        && CompletionCode != JOB_OBJECT_MSG_ACTIVE_PROCESS_ZERO) {
        FTLTRACE(L"Still waiting...\n");
    }
    FTLTRACE(L"All done\n");


    /*

    DWORD dwWait = 0;
    HANDLE hJobObject = NULL;
    HANDLE hCopyProcess = NULL;
    API_VERIFY((hJobObject = CreateJobObject(NULL, NULL)) != NULL);
    if(bRet)
    {
        STARTUPINFO startupInfo = {0};
        startupInfo.cb = sizeof(startupInfo);
        PROCESS_INFORMATION processInfo = {0};

        API_VERIFY(CreateProcess(pszPath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo));
        if (bRet)
        {
            API_VERIFY(NULL != (hCopyProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, processInfo.dwProcessId)));
            API_VERIFY(AssignProcessToJobObject(hJobObject, hCopyProcess));

        }
        dwWait = WaitForSingleObject(hJobObject, INFINITE);
        switch (dwWait)
        {
        case WAIT_OBJECT_0:
            break;
        default:
            API_ASSERT(FALSE);
        }
        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);
        CloseHandle(hCopyProcess);

        CloseHandle(hJobObject);
    }
    */
    return bRet;
}

void CSystemPage::OnBnClickedBtnCreateProcessAndWaitAllChild()
{
    CreateProcessAndWaitAllChild(TEXT("C:\\Windows\\System32\\cmd.exe"));
}

void CSystemPage::OnBnClickedBtnGetRegValueExportString()
{
    //BOOL bRet = FALSE;
    //LONG nBufStringCount = 0;
    //BYTE bufs[] = {0x12, 0x34, 0x56, 0x78};
    //LONG nBytesCount = sizeof(bufs);

    //API_VERIFY(CFConvUtil::HexFromBinary(bufs, nBytesCount, NULL, &nBufStringCount, _T(',')));
    //CFMemAllocator<TCHAR> bufString(nBufStringCount);
    //API_VERIFY(CFConvUtil::HexFromBinary(bufs, nBytesCount, bufString.GetMemory(), &nBufStringCount, _T(',')));
    //FTLTRACE(TEXT("string is %s\n"), bufString.GetMemory());
    //return;


    LONG lRet = ERROR_SUCCESS;
    CAtlString strResult;
    DWORD dwRegType = REG_NONE;

    CRegKey reg;
    REG_VERIFY(reg.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), KEY_READ | KEY_WOW64_64KEY ));
    if (ERROR_SUCCESS == lRet)
    {
        LPCTSTR pszValueNames[] = {
            _T("PathName"),
            //_T("LogFileDirectory"),
        };

        for (int i = 0; i < _countof(pszValueNames); i++)
        {
            REG_VERIFY(FTL::CFRegUtil::GetRegValueExportString(reg, pszValueNames[i], strResult, &dwRegType));
            FTLTRACE(TEXT("%s(%d) export value is %s\n"), pszValueNames[i], dwRegType, strResult);
        }
        
    }
    
}