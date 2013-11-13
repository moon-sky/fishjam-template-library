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
    ON_BN_CLICKED(IDC_BTN_SYSTEM_TEST_OPENREG, &CSystemPage::OnBnClickedBtnTestOpenReg)

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

void CSystemPage::OnBnClickedBtnCreateProcessAndWaitAllChild()
{
    FTL::CFSystemUtil::CreateProcessAndWaitAllChild(TEXT("C:\\Windows\\System32\\cmd.exe"));
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


    //LONG lRet = ERROR_SUCCESS;
    //CAtlString strResult;
    //DWORD dwRegType = REG_NONE;

    //CRegKey reg;
    //REG_VERIFY(reg.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), KEY_READ | KEY_WOW64_64KEY ));
    //if (ERROR_SUCCESS == lRet)
    //{
    //    LPCTSTR pszValueNames[] = {
    //        _T("PathName"),
    //        //_T("LogFileDirectory"),
    //    };

    //    for (int i = 0; i < _countof(pszValueNames); i++)
    //    {
    //        REG_VERIFY(FTL::CFRegUtil::GetRegValueExportString(reg, pszValueNames[i], strResult, &dwRegType));
    //        FTLTRACE(TEXT("%s(%d) export value is %s\n"), pszValueNames[i], dwRegType, strResult);
    //    }
    //    
    //}
}

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

void CSystemPage::OnBnClickedBtnTestOpenReg()
{
    LONG lRet = ERROR_SUCCESS;
    CAtlString strSubKey = TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer");
    CRegKey regLocalMachine;
    REG_VERIFY(regLocalMachine.Open(HKEY_LOCAL_MACHINE, strSubKey, KEY_ALL_ACCESS));


    TCHAR    achKey[MAX_VALUE_NAME] = {0};   // buffer for subkey name
    DWORD    cbName = _countof(achKey);                   // size of name string 
    TCHAR    achClass[MAX_PATH] = {0};  // buffer for class name 
    DWORD    cchClassName = MAX_PATH;  // size of class string 
    DWORD    cSubKeys=0;               // number of subkeys 
    DWORD    cbMaxSubKey;              // longest subkey size 
    DWORD    cchMaxClass;              // longest class string 
    DWORD    cValues;              // number of values for key 
    DWORD    cchMaxValue;          // longest value name 
    DWORD    cbMaxValueData;       // longest value data 
    DWORD    cbSecurityDescriptor; // size of security descriptor 
    FILETIME ftLastWriteTime;      // last write time 

    DWORD i, retCode; 

    TCHAR  achValue[MAX_VALUE_NAME]; 
    DWORD cchValue = MAX_VALUE_NAME; 

    // Get the class name and the value count. 
    REG_VERIFY(RegQueryInfoKey(
        regLocalMachine.m_hKey,                    // key handle 
        achClass,                // buffer for class name 
        &cchClassName,           // size of class string 
        NULL,                    // reserved 
        &cSubKeys,               // number of subkeys 
        &cbMaxSubKey,            // longest subkey size 
        &cchMaxClass,            // longest class string 
        &cValues,                // number of values for this key 
        &cchMaxValue,            // longest value name 
        &cbMaxValueData,         // longest value data 
        &cbSecurityDescriptor,   // security descriptor 
        &ftLastWriteTime));       // last write time 



    CRegKey regCurrentUser;
    REG_VERIFY(regCurrentUser.Open(HKEY_CURRENT_USER, strSubKey, KEY_QUERY_VALUE));

    REG_VERIFY(RegQueryInfoKey(
        regCurrentUser.m_hKey,                    // key handle 
        achClass,                // buffer for class name 
        &cchClassName,           // size of class string 
        NULL,                    // reserved 
        &cSubKeys,               // number of subkeys 
        &cbMaxSubKey,            // longest subkey size 
        &cchMaxClass,            // longest class string 
        &cValues,                // number of values for this key 
        &cchMaxValue,            // longest value name 
        &cbMaxValueData,         // longest value data 
        &cbSecurityDescriptor,   // security descriptor 
        &ftLastWriteTime));       // last write time 

    //FTLASSERT(regLocalMachine.m_hKey == regCurrentUser.m_hKey);

}