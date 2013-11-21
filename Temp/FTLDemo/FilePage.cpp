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
    m_strCopySrcDir = _T("C:\\Program Files (x86)\\Common Files");
    m_strCopyDstDir = _T("E:\\testOutput");

    m_strCopySrcFile = TEXT("E:\\ServerFileList.txt");

    m_DirCopier.SetCallback(this);
    m_nTotalSize = 0LL;
    m_nCopiedSize = 0LL;
    m_nCopyFileCount = 5;
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
    ON_BN_CLICKED(IDC_BTN_FILE_API_COPYFILE, &CFilePage::OnBnClickedBtnApiCopyFile)
    ON_BN_CLICKED(IDC_BTN_FILE_API_COPYFILEEX, &CFilePage::OnBnClickedBtnApiCopyFileEx)
    ON_BN_CLICKED(IDC_BTN_FILE_READ_WRITE_COPY, &CFilePage::OnBnClickedBtnReadWriteCopy)

    ON_BN_CLICKED(IDC_BTN_FILE_FILLDUMP, &CFilePage::OnBnClickedBtnFillDump)

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

//class CFIocpFileReadTask : public CFFile
//    ,public CFIocpBaseTask
//{
//public:
//    virtual INT GetIocpHandleCount() const 
//    {
//        return 1;
//    }
//
//    virtual HANDLE GetIocpHandle(INT nIndex) const
//    {
//        return m_hFile;
//    }
//
//    virtual BOOL OnInitialize(CFIocpMgr* pIocpMgr,  CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred )
//    {
//        BOOL bRet = FALSE;
//        pIoBuffer->m_operType = IORead;
//        API_VERIFY(Read(pIoBuffer->m_pBuffer, pIoBuffer->m_dwSize, NULL, &pIoBuffer->m_overLapped));
//        return bRet;
//    }
//
//    virtual BOOL AfterReadCompleted(CFIocpMgr* pIocpMgr,  CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred )
//    {
//        BOOL bRet = FALSE;
//        FTLASSERT(IORead == pIoBuffer->m_operType);
//        API_VERIFY(Read(pIoBuffer->m_pBuffer, pIoBuffer->m_dwSize, NULL, &pIoBuffer->m_overLapped));
//        return bRet;
//    }
//    virtual BOOL AfterWriteCompleted(CFIocpMgr* pIocpMgr,  CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred )
//    {
//        BOOL bRet = FALSE;
//        FTLASSERT(IOWrite == pIoBuffer->m_operType);
//        API_VERIFY(Write(pIoBuffer->m_pBuffer, pIoBuffer->m_dwSize, NULL, &pIoBuffer->m_overLapped));
//        return bRet;
//    }
//    virtual BOOL OnUninitialize(CFIocpMgr* pIocpMgr, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred)
//    {
//        BOOL bRet = FALSE;
//        this->Close();
//        return bRet;
//    }
//};

class CFIocpCopyFileTask : public CFIocpBaseTask
{
public:
    static LONG s_Count;

    CFIocpCopyFileTask()
    {
        if (0 == s_Count)
        {
            m_ElapseCount.Start();
        }
        InterlockedIncrement(&s_Count);
    }
    ~CFIocpCopyFileTask()
    {
        LONG nCount = InterlockedDecrement(&s_Count);
        if (0 == nCount)
        {
            m_ElapseCount.Stop();
            CFStringFormater formater;
            formater.Format(TEXT("CFIocpCopyFileTask Copy File Elapse %d(ms)\n"),
                m_ElapseCount.GetElapseTime() / NANOSECOND_PER_MILLISECOND);
            OutputDebugString(formater.GetString());
        }
    }
    FTL::CFElapseCounter m_ElapseCount;

    BOOL OpenFiles(LPCTSTR pszSrcFile, LPCTSTR pszDstFile)
    {
        BOOL bRet = FALSE;

        API_VERIFY(m_SrcFile.Create(pszSrcFile, GENERIC_READ, FILE_SHARE_READ, NULL, 
            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED));
        API_VERIFY(m_DstFile.Create(pszDstFile, GENERIC_WRITE, 0, NULL,
            CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED));
        return bRet;
    }
    

    virtual INT GetIocpHandleCount() const 
    {
        return 2;
    }

    virtual HANDLE GetIocpHandle(INT nIndex) const
    {
        HANDLE hResult = INVALID_HANDLE_VALUE;
        switch (nIndex)
        {
        case 0:
            hResult = m_SrcFile.m_hFile;
            break;
        case 1:
            hResult = m_DstFile.m_hFile;
            break;
        default:
            FTLASSERT(FALSE);
            break;
        }
        return hResult;
    }

    virtual BOOL OnInitialize(CFIocpMgr* pIocpMgr,  CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred )
    {
        BOOL bRet = FALSE;
        pIoBuffer->m_operType = iotRead;
        API_VERIFY_EXCEPT1(m_SrcFile.Read(pIoBuffer->m_pBuffer, pIoBuffer->m_dwSize, NULL, &pIoBuffer->m_overLapped),
            ERROR_IO_PENDING);
        return bRet;
    }

    virtual BOOL AfterReadCompleted(CFIocpMgr* pIocpMgr, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred )
    {
        BOOL bRet = FALSE;
        FTLASSERT(iotRead == pIoBuffer->m_operType);
        pIoBuffer->m_operType = iotWrite;
        API_VERIFY_EXCEPT1(m_DstFile.Write(pIoBuffer->m_pBuffer, dwBytesTransferred, NULL, &pIoBuffer->m_overLapped),
            ERROR_IO_PENDING);

        return bRet;
    }
    virtual BOOL AfterWriteCompleted(CFIocpMgr* pIocpMgr, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred )
    {
        BOOL bRet = FALSE;
        FTLASSERT(iotWrite == pIoBuffer->m_operType);
        pIoBuffer->m_operType = iotRead;

        AddOverLappedOffset(&pIoBuffer->m_overLapped, dwBytesTransferred);

        API_VERIFY_EXCEPT1(m_SrcFile.Read(pIoBuffer->m_pBuffer, pIoBuffer->m_dwSize, NULL, &pIoBuffer->m_overLapped),
            ERROR_IO_PENDING);
        return bRet;
    }
    virtual BOOL OnUninitialize(CFIocpMgr* pIocpMgr, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred)
    {
        BOOL bRet = FALSE;
        switch(pIoBuffer->m_operType)
        {
        case iotRead:
            //read over
            API_VERIFY(m_SrcFile.Close());
            API_VERIFY(m_DstFile.Write(pIoBuffer->m_pBuffer, dwBytesTransferred, NULL, NULL));
            m_DstFile.Close();
            break;
        default:
            FTLASSERT(FALSE);
            break;
        //case IOWrite:
        //    API_VERIFY(m_DstFile.Close());
        //    break;
        }
        delete this;
        return bRet;
    }
private:
    CFFile  m_SrcFile;
    CFFile  m_DstFile;
};
LONG CFIocpCopyFileTask::s_Count = 0;

void CFilePage::OnBnClickedBtnIocpTest()
{
    //BOOL bRet = FALSE;
    BOOL bRet = FALSE;

    //CFIocpFileReadTask  fileReadTask;
    //API_VERIFY(fileReadTask.Create(TEXT("D:\\test.jpg"), 
    //    GENERIC_WRITE | GENERIC_READ, 
    //    FILE_SHARE_READ, NULL,
    //    OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED));

    CFElapseCounter counter;
    for (INT i = 0; i < m_nCopyFileCount; i++)
    {
        CFIocpCopyFileTask* pFileCopyTask = new CFIocpCopyFileTask();
        CAtlString strTagetFile;
        strTagetFile.Format(TEXT("E:\\TargetCopy_%d.txt"), counter.GetElapseTime() % LONG_MAX);
        pFileCopyTask->OpenFiles(m_strCopySrcFile, strTagetFile);
        m_iocpMgr.AssociateTask(pFileCopyTask);
    }


    //fileIocpTask.m_OverLapped.overLapped.Offset = 0;

    //fileIocpTask.WriteFileHeader((LPOVERLAPPED)&fileIocpTask.m_OverLapped);

    //fileReadTask.Close();

    //iocpMgr.Stop();
    //iocpMgr.Close();
}

void CFilePage::OnBnClickedBtnApiCopyFile()
{
    BOOL bRet = FALSE;
    m_strCopyDstFile.Format(TEXT("E:\\TargetCopy_%d.txt"), GetTickCount());
    API_VERIFY_EXCEPT1(DeleteFile(m_strCopyDstFile), ERROR_FILE_NOT_FOUND);

    CFElapseCounter counter;
    counter.Start();
    API_VERIFY(CopyFile(m_strCopySrcFile, m_strCopyDstFile, FALSE));
    counter.Stop();

    CFStringFormater formater;
    formater.Format(TEXT("API CopyFile Elapse %d(ms)\n"), counter.GetElapseTime() / NANOSECOND_PER_MILLISECOND);
    OutputDebugString(formater.GetString());
}

static DWORD CALLBACK CopyProgressRoutine(LARGE_INTEGER TotalFileSize,
   LARGE_INTEGER TotalBytesTransferred,
   LARGE_INTEGER StreamSize,
   LARGE_INTEGER StreamBytesTransferred,
   DWORD dwStreamNumber,
   DWORD dwCallbackReason,
   HANDLE hSourceFile,
   HANDLE hDestinationFile,
   LPVOID lpData
)
{
    return PROGRESS_CONTINUE;
}


void CFilePage::OnBnClickedBtnApiCopyFileEx()
{
    BOOL bRet = FALSE;
    m_strCopyDstFile.Format(TEXT("E:\\TargetCopy_%d.txt"), GetTickCount());
    API_VERIFY_EXCEPT1(DeleteFile(m_strCopyDstFile), ERROR_FILE_NOT_FOUND);

    CFElapseCounter counter;
    counter.Start();
    BOOL bCancel = FALSE;
    API_VERIFY(CopyFileEx(m_strCopySrcFile, m_strCopyDstFile, CopyProgressRoutine, this, &bCancel, 0));
    counter.Stop();

    CFStringFormater formater;
    formater.Format(TEXT("API CopyFileEx Elapse %d(ms)\n"), counter.GetElapseTime() / NANOSECOND_PER_MILLISECOND);
    OutputDebugString(formater.GetString());
}

void CFilePage::OnBnClickedBtnReadWriteCopy()
{
    BOOL bRet = FALSE;
    m_strCopyDstFile.Format(TEXT("E:\\TargetCopy_%d.txt"), GetTickCount());
    API_VERIFY_EXCEPT1(DeleteFile(m_strCopyDstFile), ERROR_FILE_NOT_FOUND);

    CFFile srcFile;
    CFFile dstFile;
    CFMemAllocator<BYTE> buf(4096);

    CFElapseCounter counter;
    counter.Start();

    API_VERIFY(srcFile.Create(m_strCopySrcFile, GENERIC_READ, FILE_SHARE_READ, NULL,
        OPEN_EXISTING));
    API_VERIFY(dstFile.Create(m_strCopyDstFile, GENERIC_WRITE, 0, NULL,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL));

    DWORD dwRead = 0;
    DWORD dwWrite = 0;
    while (srcFile.Read(buf.GetMemory(), buf.GetCount(), &dwRead, NULL) 
        && dwRead > 0)
    {
        API_VERIFY(dstFile.Write(buf.GetMemory(), dwRead, &dwWrite, NULL));
    }
    srcFile.Close();
    dstFile.Close();

    counter.Stop();

    CFStringFormater formater;
    formater.Format(TEXT("Read + Write Copy Elapse %d(ms)\n"), counter.GetElapseTime() / NANOSECOND_PER_MILLISECOND);
    OutputDebugString(formater.GetString());
}

void CFilePage::OnBnClickedBtnFillDump()
{
    BOOL bRet = FALSE;
    CFileDialog dlg(FALSE, TEXT(".fil"), TEXT("FileDump.fil"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        TEXT("*.fil\0*.fil\0All Files(*.*)\0*.*\0\0"), this);
    if (dlg.DoModal() == IDOK)
    {
        CString strFilePath = dlg.GetPathName();

        CFFile dmpFile;
        API_VERIFY(dmpFile.Create(strFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS));
        if (bRet)
        {
            API_VERIFY(dmpFile.SetLength(100 * 1024 * 1024));
            API_VERIFY(dmpFile.Close());
        }
    }
}

BOOL CFilePage::OnInitDialog()
{
    CPropertyPage::OnInitDialog();
    m_progressCopyDir.SetRange32(0, 100);
    m_iocpMgr.Start();

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
