// FilePage.cpp : implementation file
//

#include "stdafx.h"
#include "FTLDemo.h"
#include "FilePage.h"

#include "ftlFile.h"
#include "ftlShell.h"
// CFilePage dialog

IMPLEMENT_DYNAMIC(CFilePage, CPropertyPage)

CFilePage::CFilePage()
	: CPropertyPage(CFilePage::IDD)
{
    m_strCopySrcDir = _T("C:\\Program Files (x86)\\Common Files\\microsoft shared\\ink\\");
    m_strCopyDstDir = m_strCopySrcDir;    
}

CFilePage::~CFilePage()
{
}

void CFilePage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_FILE_COPY_SRC_DIR, m_strCopySrcDir);
    DDX_Text(pDX, IDC_EDIT_FILE_COPY_DST_DIR, m_strCopyDstDir);
}


BEGIN_MESSAGE_MAP(CFilePage, CPropertyPage)
    ON_BN_CLICKED(IDC_BTN_FILE_CHOOSE_COPY_SRC_DIR, &CFilePage::OnBnClickedBtnFileChooseCopySrcDir)
    ON_BN_CLICKED(IDC_BTN_FILE_CHOOSE_COPY_DST_DIR, &CFilePage::OnBnClickedBtnFileChooseCopyDstDir)
    ON_BN_CLICKED(IDC_BTN_FILE_COPY_DIR, &CFilePage::OnBnClickedBtnFileCopyDir)
END_MESSAGE_MAP()


// CFilePage message handlers


void CFilePage::OnBnClickedBtnFileChooseCopySrcDir()
{
    UpdateData(TRUE);
    CFDirBrowser browser(TEXT("Select Source Folder"), m_hWnd, m_strCopySrcDir,
        BIF_STATUSTEXT | BIF_RETURNONLYFSDIRS);
    if(browser.DoModal())
    {
        m_strCopySrcDir = browser.m_szPath;
        UpdateData(FALSE);
    }
}

void CFilePage::OnBnClickedBtnFileChooseCopyDstDir()
{
    UpdateData(TRUE);
    CFDirBrowser browser(TEXT("Select Dest Folder"), m_hWnd, m_strCopyDstDir,
        BIF_NEWDIALOGSTYLE | BIF_STATUSTEXT | BIF_RETURNONLYFSDIRS | BIF_VALIDATE);
    if(browser.DoModal())
    {
        m_strCopyDstDir = browser.m_szPath;
        UpdateData(FALSE);
    }
}

void CFilePage::OnBnClickedBtnFileCopyDir()
{
    //CFStringFormater formater;
    //FTL::CFShellUtil::BrowserDirectory(formater, TEXT("Choose Copy Src Dir"), )
    CFileDialog dlg(TRUE, NULL, m_strCopyDstDir, OFN_EXPLORER , _T("*..*"));
    if (dlg.DoModal() == IDOK)
    {
    }
}

BOOL CFilePage::OnInitDialog()
{
    CPropertyPage::OnInitDialog();


    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
