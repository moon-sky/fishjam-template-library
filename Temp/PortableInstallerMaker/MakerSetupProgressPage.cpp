#include "StdAfx.h"
#include "MakerSetupProgressPage.h"
#include "SetupMakerHelper/SetupMakerHelper.h"

CMakerSetupProgressPage::CMakerSetupProgressPage(_U_STRINGorID title /* = (LPCTSTR)NULL*/ )
:baseClass(title)
{
}

BOOL CMakerSetupProgressPage::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
{
    this->InitializeControls();
    this->InitializeValues();

    BOOL bRet = FALSE;
    API_VERIFY(m_ThreadSetup.Start(_RunSetupProc, this, TRUE));
    return TRUE;
}

BOOL CMakerSetupProgressPage::OnCopyData(CWindow wnd, PCOPYDATASTRUCT pCopyDataStruct)
{
    if (pCopyDataStruct)
    {
        FTLASSERT(pCopyDataStruct->cbData == sizeof(SETUP_MONITOR_INFO));
        if (pCopyDataStruct->cbData == sizeof(SETUP_MONITOR_INFO))
        {
            SETUP_MONITOR_INFO* pSetupMonitorInfo = (SETUP_MONITOR_INFO*)pCopyDataStruct->lpData;
            if (pSetupMonitorInfo)
            {
                m_pMakerWizardInfo->AddSetupMonitorInfo(pSetupMonitorInfo->dwInfoType, pSetupMonitorInfo->szPath);
                FTLTRACE(TEXT("OnCopyData, %d, %s\n"), pSetupMonitorInfo->dwInfoType, pSetupMonitorInfo->szPath);
            }
        }
        
    }
    return TRUE;
}

void CMakerSetupProgressPage::InitializeControls(void)
{
    CFontHandle fontExteriorPageTitleFont(baseClass::GetExteriorPageTitleFont());
    CFontHandle fontBulletFont(baseClass::GetBulletFont());

    //CWindow title = this->GetDlgItem(IDC_WIZ97_EXTERIOR_TITLE);
    //CWindow bullet1 = this->GetDlgItem(IDC_WIZ97_BULLET1);
    //CWindow bullet2 = this->GetDlgItem(IDC_WIZ97_BULLET2);
    //CWindow bullet3 = this->GetDlgItem(IDC_WIZ97_BULLET3);
    //CWindow bullet4 = this->GetDlgItem(IDC_WIZ97_BULLET4);
    //m_buttonSkipWelcome = this->GetDlgItem(IDC_WIZ97_WELCOME_NOTAGAIN);

    //title.SetFont(fontExteriorPageTitleFont);
    //bullet1.SetFont(fontBulletFont);
    //bullet2.SetFont(fontBulletFont);
    //bullet3.SetFont(fontBulletFont);
    //bullet4.SetFont(fontBulletFont);
}

void CMakerSetupProgressPage::InitializeValues(void)
{
    //bool showWelcome = m_pMakerWizardInfo->GetShowWelcome();
    //m_buttonSkipWelcome.SetCheck(showWelcome ? BST_UNCHECKED : BST_CHECKED);
}

bool CMakerSetupProgressPage::StoreValues(void)
{
    //m_pMakerWizardInfo->SetShowWelcome(m_buttonSkipWelcome.GetCheck() == BST_UNCHECKED);
    return true;
}

// Overrides from base class
int CMakerSetupProgressPage::OnSetActive()
{
    this->SetWizardButtons(0); //PSWIZB_BACK | PSWIZB_NEXT); //PSWIZB_BACK

    // 0 = allow activate
    // -1 = go back to page that was active
    // page ID = jump to page
    int result = 0;

    return result;
}

int CMakerSetupProgressPage::OnWizardNext()
{
    bool success = this->StoreValues();
    if(!success)
    {
        // Any errors are already reported, and if appropriate,
        // the control that needs attention has been given focus.
        return -1;
    }

    // 0  = goto next page
    // -1 = prevent page change
    // >0 = jump to page by dlg ID

    return m_pMakerWizardInfo->FindNextPage(IDD);
}

void CMakerSetupProgressPage::OnHelp()
{
    m_pMakerWizardInfo->ShowHelp(IDD);
}


DWORD __stdcall CMakerSetupProgressPage::_RunSetupProc(LPVOID pParam)
{
    CMakerSetupProgressPage* pThis = static_cast<CMakerSetupProgressPage*>(pParam);
    DWORD dwResult = pThis->_InnerRunSetupProc();
    return dwResult;
}
DWORD CMakerSetupProgressPage::_InnerRunSetupProc()
{
    BOOL bRet = FALSE;
    STARTUPINFO startupInfo = {0};
    PROCESS_INFORMATION processInfo = {0};

    API_VERIFY(CreateProcess(NULL, (LPTSTR)m_pMakerWizardInfo->GetSetupFilePath(), NULL, NULL, FALSE, 
        /*CREATE_NEW_PROCESS_GROUP |*/ CREATE_SUSPENDED, NULL, NULL, 
        &startupInfo, &processInfo));

    FTLTRACE(TEXT("After CreateProcess, bRet=%d, PID=%d, TID=%d\n"), 
        bRet, processInfo.dwProcessId, processInfo.dwThreadId);

    if (bRet)
    {
        API_VERIFY(EnableSetupMonitor(GetCurrentProcessId(), processInfo.dwProcessId, m_hWnd));

        ResumeThread(processInfo.hThread);
        FTLTRACE(TEXT("Before WaitFor processInfo.hThread\n"));

        WaitForSingleObject(processInfo.hThread, INFINITE);
        API_VERIFY(DisableSetupMonitor(processInfo.dwProcessId));

        FTLTRACE(TEXT("After WaitFor processInfo.hThread\n"));
        PostMessage(UM_SETUP_MAKER_PROCESS_FINISHED, 0, 0);
        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);
    }
    return 0;
}

LRESULT CMakerSetupProgressPage::OnSetupMakerProcessFinished(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    this->SetWizardButtons(PSWIZB_NEXT); //PSWIZB_BACK

    return 0;
}
