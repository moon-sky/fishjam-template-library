#include "StdAfx.h"
#include "MakerSetupResultPage.h"

CMakerSetupResultPage::CMakerSetupResultPage(_U_STRINGorID title /* = (LPCTSTR)NULL*/ )
:baseClass(title)
{
}

BOOL CMakerSetupResultPage::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
{
    DoDataExchange(FALSE);

    this->InitializeControls();
    this->InitializeValues();
    return TRUE;
}

void CMakerSetupResultPage::InitializeControls(void)
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

    CTreeItem tiFileRoot, tiRegRoot;

    tiFileRoot  = m_treeSetupChangeResult.InsertItem ( TEXT("File"), TVI_ROOT, TVI_LAST );
    //TreeView_SetCheckState(m_treeSetupChangeResult.m_hWnd, tiFileRoot.m_hTreeItem, TRUE);
    m_treeSetupChangeResult.SetCheckState(tiFileRoot, TRUE);

    for (SetupMonitorInfoContainer::iterator iterFile = m_pMakerWizardInfo->m_allSetupFileInfos.begin();
        iterFile != m_pMakerWizardInfo->m_allSetupFileInfos.end();
        ++iterFile)
    {
        SetupMonitorInfo& fileInfo = *iterFile;
        if (!_IsFilterFile(fileInfo.strPath))
        {
            CTreeItem tiFileItem = tiFileRoot.AddTail(fileInfo.strPath, 0);
            m_treeSetupChangeResult.SetCheckState(tiFileItem, TRUE);
            //TreeView_SetCheckState(m_treeSetupChangeResult.m_hWnd, tiFileItem.m_hTreeItem, TRUE);
        }
    }
    
    tiRegRoot  = m_treeSetupChangeResult.InsertItem ( TEXT("Reg"), TVI_ROOT, TVI_LAST );
    //TreeView_SetCheckState(m_treeSetupChangeResult.m_hWnd, tiRegRoot.m_hTreeItem, TRUE);
    m_treeSetupChangeResult.SetCheckState(tiRegRoot, TRUE);

    for (SetupMonitorInfoContainer::iterator iterReg = m_pMakerWizardInfo->m_allSetupRegInfos.begin();
        iterReg != m_pMakerWizardInfo->m_allSetupRegInfos.end();
        ++iterReg)
    {
        SetupMonitorInfo& regInfo = *iterReg;
        CTreeItem tiRegItem = tiRegRoot.AddTail(regInfo.strPath, 0);
        m_treeSetupChangeResult.SetCheckState(tiRegItem, TRUE);

        //TreeView_SetCheckState(m_treeSetupChangeResult.m_hWnd, tiRegItem.m_hTreeItem, TRUE);
    }
}

void CMakerSetupResultPage::InitializeValues(void)
{
    //bool showWelcome = m_pMakerWizardInfo->GetShowWelcome();
    //m_buttonSkipWelcome.SetCheck(showWelcome ? BST_UNCHECKED : BST_CHECKED);
}

bool CMakerSetupResultPage::StoreValues(void)
{
    //m_pMakerWizardInfo->SetShowWelcome(m_buttonSkipWelcome.GetCheck() == BST_UNCHECKED);
    return true;
}

// Overrides from base class
int CMakerSetupResultPage::OnSetActive()
{
    this->SetWizardButtons(PSWIZB_NEXT);

    // 0 = allow activate
    // -1 = go back to page that was active
    // page ID = jump to page
    int result = 0;

    return result;
}

int CMakerSetupResultPage::OnWizardNext()
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

void CMakerSetupResultPage::OnHelp()
{
    m_pMakerWizardInfo->ShowHelp(IDD);
}


BOOL CMakerSetupResultPage::_IsFilterFile(LPCTSTR pszFilePath)
{
    BOOL bRet = FALSE;
    if (!bRet)
    {
        bRet = PathIsRoot(pszFilePath);
    }
    if (!bRet)
    {
        bRet = PathIsNetworkPath(pszFilePath);
    }
    if (!bRet)
    {
        bRet = PathIsTempPath(pszFilePath);
    }
    return bRet;
}
BOOL CMakerSetupResultPage::PathIsTempPath(LPCTSTR pszFilePath)
{
    BOOL bRet = FALSE;
    TCHAR szTmpPath[MAX_PATH] = {0};
    if (!bRet)
    {
        GetTempPath(_countof(szTmpPath), szTmpPath);
        if (StrStrI(pszFilePath, szTmpPath) != NULL)
        {
            bRet = TRUE;
        }
    }
    if (!bRet)
    {

    }

    return bRet;
}