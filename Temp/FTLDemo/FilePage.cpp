// FilePage.cpp : implementation file
//

#include "stdafx.h"
#include "FTLDemo.h"
#include "FilePage.h"

#include "ftlShell.h"
// CFilePage dialog

IMPLEMENT_DYNAMIC(CFilePage, CPropertyPage)

CFilePage::CFilePage()
	: CPropertyPage(CFilePage::IDD)
{
    m_strCopySrcDir = _T("C:\\Program Files (x86)\\Common Files\\microsoft shared\\ink\\");
    m_strCopyDstDir = m_strCopySrcDir;
    m_DirCopier.SetCallback(this);
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
    ON_BN_CLICKED(IDC_BTN_FILE_FIND_SOURCE_DIR, &CFilePage::OnBnClickedBtnFileFindSourceDir)
    ON_BN_CLICKED(IDC_BTN_FILE_START_COPY_DIR, &CFilePage::OnBnClickedBtnFileStartCopyDir)
    ON_BN_CLICKED(IDC_BTN_FILE_STOP_COPY_DIR, &CFilePage::OnBnClickedBtnFileStopCopyDir)
END_MESSAGE_MAP()


// CFilePage message handlers


void CFilePage::OnBnClickedBtnFileChooseCopySrcDir()
{
    UpdateData(TRUE);
    CFDirBrowser browser(TEXT("Select Source Folder"), m_hWnd, m_strCopySrcDir,
        BIF_STATUSTEXT | BIF_RETURNONLYFSDIRS);
    if(browser.DoModal())
    {
        m_strCopySrcDir = browser.GetSelectPath();
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
        m_strCopyDstDir = browser.GetSelectPath();
        UpdateData(FALSE);
    }
}

class CMyFindCallback : public IFileFindCallback
{
public:
    virtual FileFindResultHandle OnFindFile(LPCTSTR pszFilePath, const WIN32_FIND_DATA& findData, LPVOID pParam)
    {
        LARGE_INTEGER fileSize;
        fileSize.HighPart = findData.nFileSizeHigh;
        fileSize.LowPart = findData.nFileSizeLow;
        DWORD* pCount = reinterpret_cast<DWORD*>(pParam);
        (*pCount)++;
        FTLTRACE(TEXT("OnFindFile, %s, dwFileAttributes=%d(0x%x), size=%ld\n"), 
            pszFilePath, findData.dwFileAttributes, findData.dwFileAttributes, fileSize.QuadPart);
        return rhContinue; 
    }
};
void CFilePage::OnBnClickedBtnFileFindSourceDir()
{
    CMyFindCallback callback;
    FTL::CFFileFinder finder;
    DWORD dwCount = 0;
    finder.SetCallback(&callback, LPVOID(&dwCount));
    finder.Find(m_strCopySrcDir);

    FTLTRACE(TEXT("Find %d Files\n"), dwCount);
}

void CFilePage::OnBnClickedBtnFileStartCopyDir()
{
    m_DirCopier.Start(m_strCopySrcDir, m_strCopyDstDir, _T("*.exe"), FALSE, TRUE);
}

void CFilePage::OnBnClickedBtnFileStopCopyDir()
{

}

BOOL CFilePage::OnInitDialog()
{
    CPropertyPage::OnInitDialog();


    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CFilePage::OnBeginPrepareSourceFiles(LPCTSTR pszSrcDir, LPCTSTR pszDstDir)
{

}
VOID CFilePage::OnBegin(LONGLONG nTotalSize, LONG nFileCount)
{

}
VOID CFilePage::OnCopyFile(LPCTSTR pszSrcFile, LPCTSTR pszTargetFile, LONG nIndex, LONGLONG nFileSize, LONGLONG nCopiedSize)
{

}
VOID CFilePage::OnEnd(BOOL bSuccess, LONGLONG nTotalCopiedSize, LONG nCopiedFileCount)
{

}
VOID CFilePage::OnError(LPCTSTR pszSrcFile, LPCTSTR pszTargetFile, DWORD dwError)
{

}
