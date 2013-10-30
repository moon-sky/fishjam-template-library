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

}

CFilePage::~CFilePage()
{
}

void CFilePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFilePage, CPropertyPage)
    ON_BN_CLICKED(IDC_BTN_FILE_CHOOSE_COPY_SRC_DIR, &CFilePage::OnBnClickedBtnFileChooseCopySrcDir)
    ON_BN_CLICKED(IDC_BTN_FILE_CHOOSE_COPY_DST_DIR, &CFilePage::OnBnClickedBtnFileChooseCopyDstDir)
    ON_BN_CLICKED(IDC_BTN_FILE_COPY_DIR, &CFilePage::OnBnClickedBtnFileCopyDir)
END_MESSAGE_MAP()


// CFilePage message handlers


void CFilePage::OnBnClickedBtnFileChooseCopySrcDir()
{

}

void CFilePage::OnBnClickedBtnFileChooseCopyDstDir()
{

}

void CFilePage::OnBnClickedBtnFileCopyDir()
{
    //CFStringFormater formater;
    //FTL::CFShellUtil::BrowserDirectory(formater, TEXT("Choose Copy Src Dir"), )

    CFDirBrowser browser(TEXT("Select Folder"), m_hWnd, TEXT("C:\\WINDOWS"));
    if(browser.DoModal())
    {
        MessageBox(browser.m_szPath, TEXT("Browser"), MB_OK);
    }
}