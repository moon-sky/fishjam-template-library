// FilePage.cpp : implementation file
//

#include "stdafx.h"
#include "FTLDemo.h"
#include "FilePage.h"

#include "ftlShell.h"
#include "ftlIocp.h"
// CFilePage dialog

IMPLEMENT_DYNAMIC(CFilePage, CPropertyPage)

CFilePage::CFilePage()
	: CPropertyPage(CFilePage::IDD)
{
    m_strCopySrcDir = _T("C:\\Program Files (x86)\\Common Files");
    m_strCopyDstDir = _T("E:\\testOutput");
    m_DirCopier.SetCallback(this);
    m_nTotalSize = 0LL;
    m_nCopiedSize = 0LL;
}

CFilePage::~CFilePage()
{
}

void CFilePage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_FILE_COPY_SRC_DIR, m_strCopySrcDir);
    DDX_Text(pDX, IDC_EDIT_FILE_COPY_DST_DIR, m_strCopyDstDir);
    DDX_Control(pDX, IDC_PROGRESS_FILE_COPY_DIR, m_progressCopyDir);
}


BEGIN_MESSAGE_MAP(CFilePage, CPropertyPage)
    ON_MESSAGE(UM_UPDATE_COPY_DIR_PROGRESS,OnCopyDirProgress)
    ON_MESSAGE(UM_UPDATE_COPY_DIR_ERROR,OnCopyDirError)

    ON_BN_CLICKED(IDC_BTN_FILE_CHOOSE_COPY_SRC_DIR, &CFilePage::OnBnClickedBtnFileChooseCopySrcDir)
    ON_BN_CLICKED(IDC_BTN_FILE_CHOOSE_COPY_DST_DIR, &CFilePage::OnBnClickedBtnFileChooseCopyDstDir)
    ON_BN_CLICKED(IDC_BTN_FILE_FIND_SOURCE_DIR, &CFilePage::OnBnClickedBtnFileFindSourceDir)
    ON_BN_CLICKED(IDC_BTN_FILE_START_COPY_DIR, &CFilePage::OnBnClickedBtnFileStartCopyDir)
    ON_BN_CLICKED(IDC_BTN_FILE_PAUSE_RESUME_COPY_DIR, &CFilePage::OnBnClickedBtnFilePauseResumeCopyDir)
    ON_BN_CLICKED(IDC_BTN_FILE_STOP_COPY_DIR, &CFilePage::OnBnClickedBtnFileStopCopyDir)

    ON_BN_CLICKED(IDC_BTN_FILE_IOCP, &CFilePage::OnBnClickedBtnIocpTest)

END_MESSAGE_MAP()


// CFilePage message handlers

LRESULT AFX_MSG_CALL CFilePage::OnCopyDirProgress(WPARAM wParam, LPARAM /*lParam*/)
{
    m_progressCopyDir.SetPos((INT)wParam);

    return 0;
}

LRESULT AFX_MSG_CALL CFilePage::OnCopyDirError(WPARAM wParam, LPARAM /*lParam*/)
{
    DWORD dwError = (DWORD)wParam;
    FTL::CFAPIErrorInfo apiErrorInfo(dwError);
    FormatMessageBox(m_hWnd, TEXT("OnError"), MB_OK, TEXT("Err:%d\n%s"), 
        dwError, apiErrorInfo.GetConvertedInfo());
    return 0;
}

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
    BOOL bRet = FALSE;
    UpdateData(TRUE);

#ifdef _DEBUG
    m_DirCopier.Stop();
    m_DirCopier.WaitToEnd(INFINITE);
#endif 
    m_nTotalSize = 0LL;
    m_nCopiedSize = 0LL;

    API_VERIFY(m_DirCopier.Start(m_strCopySrcDir, m_strCopyDstDir, _T("*.*"), FALSE, TRUE));
}

void CFilePage::OnBnClickedBtnFilePauseResumeCopyDir()
{
    m_DirCopier.PauseOrResume();
}

void CFilePage::OnBnClickedBtnFileStopCopyDir()
{
    m_DirCopier.Stop();
    m_DirCopier.WaitToEnd(INFINITE);
}

class CFIocpFileReadTask : public CFFile
    ,public CFIocpBaseTask
{
public:
    HANDLE GetIocpHandle() const
    {
        return m_hFile;
    }

    virtual BOOL OnInitialize( CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred )
    {
        BOOL bRet = __super::OnInitialize(pIoBuffer, dwBytesTransferred);
        pIoBuffer->m_ioType = IORead;
        Read(pIoBuffer->m_pBuffer, pIoBuffer->m_dwSize, &pIoBuffer->m_overLapped);
        return bRet;
    }

    virtual BOOL OnRead( CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred )
    {
        BOOL bRet = __super::OnRead(pIoBuffer, dwBytesTransferred);
        pIoBuffer->m_ioType = IORead;

        Read(pIoBuffer->m_pBuffer, pIoBuffer->m_dwSize, &pIoBuffer->m_overLapped);
        return bRet;
    }
};

void CFilePage::OnBnClickedBtnIocpTest()
{
    //BOOL bRet = FALSE;
    CFIocpMgr iocpMgr;
    iocpMgr.Start(1);
    BOOL bRet = FALSE;

    CFIocpFileReadTask  fileReadTask;
    API_VERIFY(fileReadTask.Create(TEXT("D:\\test.jpg"), 
        GENERIC_WRITE | GENERIC_READ, 
        FILE_SHARE_READ, NULL,
        OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED));
    iocpMgr.AddTask(&fileReadTask);

    //fileIocpTask.m_OverLapped.overLapped.Offset = 0;

    //fileIocpTask.WriteFileHeader((LPOVERLAPPED)&fileIocpTask.m_OverLapped);

    Sleep(10000000);
    //fileReadTask.Close();

    iocpMgr.Stop();
    iocpMgr.Close();
}

BOOL CFilePage::OnInitDialog()
{
    CPropertyPage::OnInitDialog();
    m_progressCopyDir.SetRange32(0, 100);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CFilePage::OnBeginPrepareSourceFiles(LPCTSTR /*pszSrcDir*/, LPCTSTR /*pszDstDir*/)
{
}

VOID CFilePage::OnBegin(LONGLONG nTotalSize, LONG nFileCount)
{
    m_nTotalSize = nTotalSize;
    SetWindowText(TEXT("Begin Copy"));
    FTLTRACE(TEXT("Begin Copy, TotalSize=%lld, nFileCount=%d\n"), nTotalSize, nFileCount);
}

VOID CFilePage::OnCopyFile(LPCTSTR pszSrcFile, LPCTSTR pszTargetFile, LONG nIndex, LONGLONG nFileSize, LONGLONG nCopiedSize)
{
    FTLASSERT(nCopiedSize >= m_nCopiedSize);
    FTLASSERT(nCopiedSize <= m_nTotalSize);

    FTLTRACE(TEXT("OnCopyFile, %s->%s, index=%d, nFileSize=%lld, nCopiedSize/nTotalSize=%lld/%lld\n"),
        pszSrcFile, pszTargetFile, nIndex, nFileSize, nCopiedSize, m_nTotalSize);

    m_nCopiedSize = nCopiedSize;
    if (m_nTotalSize > 0)
    {
        INT nPercent = (INT)((double)m_nCopiedSize * 100 / m_nTotalSize);
        PostMessage(UM_UPDATE_COPY_DIR_PROGRESS, nPercent, 0);
    }
    else
    {
        PostMessage(UM_UPDATE_COPY_DIR_PROGRESS, 100, 0);
    }
    
}
VOID CFilePage::OnEnd(BOOL bSuccess, LONGLONG nTotalCopiedSize, LONG nCopiedFileCount)
{
    SetWindowText(TEXT("Copy Dir End"));
    FTLTRACE(TEXT("Copy Dir End, bSuccess=%d, nTotalCopiedSize=%lld, nCopiedFileCount=%d\n"),
        bSuccess, nTotalCopiedSize, nCopiedFileCount);
}

VOID CFilePage::OnError(LPCTSTR pszSrcFile, LPCTSTR pszTargetFile, DWORD dwError)
{
    FTLTRACE(TEXT("OnError, pszSrcFile=%s, pszTargetFile=%s, dwError=%d\n"),
        pszSrcFile, pszTargetFile, dwError);
    SendMessage(UM_UPDATE_COPY_DIR_ERROR, dwError, 0);
}
