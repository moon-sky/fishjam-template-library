#include "StdAfx.h"
#include "MakerChooseSetupFilePage.h"

CMakerChooseSetupFilePage::CMakerChooseSetupFilePage(_U_STRINGorID title /* = (LPCTSTR)NULL*/ )
:baseClass(title)
{
}

BOOL CMakerChooseSetupFilePage::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
{
    this->InitializeControls();
    this->InitializeValues();
    return TRUE;
}

void CMakerChooseSetupFilePage::InitializeControls(void)
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

void CMakerChooseSetupFilePage::InitializeValues(void)
{
    //bool showWelcome = m_pMakerWizardInfo->GetShowWelcome();
    //m_buttonSkipWelcome.SetCheck(showWelcome ? BST_UNCHECKED : BST_CHECKED);
}

bool CMakerChooseSetupFilePage::StoreValues(void)
{
    m_pMakerWizardInfo->SetSetupFilePath(m_strSetupFilePath);
    //m_pMakerWizardInfo->SetShowWelcome(m_buttonSkipWelcome.GetCheck() == BST_UNCHECKED);
    return true;
}

// Overrides from base class
int CMakerChooseSetupFilePage::OnSetActive()
{
    this->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);

    // 0 = allow activate
    // -1 = go back to page that was active
    // page ID = jump to page
    int result = 0;

    return result;
}

int CMakerChooseSetupFilePage::OnWizardNext()
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

void CMakerChooseSetupFilePage::OnHelp()
{
    m_pMakerWizardInfo->ShowHelp(IDD);
}

void CMakerChooseSetupFilePage::OnBtnChooseSetupFile(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    CFileDialog dlg(TRUE, NULL, NULL,
        OFN_EXPLORER | OFN_ENABLESIZING | OFN_NOCHANGEDIR,
        _T("Setup Files(*.exe;*.msi)\0*.exe;*.msi\0All Files (*.*)\0*.*\0"), m_hWnd);
    dlg.m_ofn.lpstrTitle = _T("Select the setup file to execute");
    if (dlg.DoModal() == IDOK)
    {
        m_strSetupFilePath = dlg.m_szFileName;
        DoDataExchange(FALSE);
    }
}