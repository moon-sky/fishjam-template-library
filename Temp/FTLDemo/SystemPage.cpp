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
    API_VERIFY(FTL::CFService::CreateServiceUIProcess(TEXT("C:\\Windows\\System32\\calc.exe"), TRUE));
}

