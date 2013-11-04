// ThreadPage.cpp : 实现文件
//

#include "stdafx.h"
#include "FTLDemo.h"
#include "ThreadPage.h"

// CThreadPage 对话框
#define WM_THREAD_UPDATE_PROGRESS      (WM_USER + 0x1)

IMPLEMENT_DYNAMIC(CThreadPage, CPropertyPage)

CThreadPage::CThreadPage()
	: CPropertyPage(CThreadPage::IDD)
    , m_pRWLocker(NULL)
{

}

CThreadPage::~CThreadPage()
{
}

void CThreadPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PROGRESS_OF_THREAD, m_ProgressOfThread);
}


BEGIN_MESSAGE_MAP(CThreadPage, CPropertyPage)
    ON_WM_DESTROY()
    ON_MESSAGE(WM_THREAD_UPDATE_PROGRESS,OnThreadUpdateProgress)

    ON_BN_CLICKED(IDC_BTN_THREAD_START, &CThreadPage::OnBnClickedBtnThreadStart)
    ON_BN_CLICKED(IDC_BTN_THREAD_STOP, &CThreadPage::OnBnClickedBtnThreadStop)
    ON_BN_CLICKED(IDC_BTN_THREAD_PAUSE_RESUME, &CThreadPage::OnBnClickedBtnThreadPauseResume)
    ON_BN_CLICKED(IDC_BTN_THREAD_GETWAITTYPEEX, &CThreadPage::OnBnClickedBtnThreadGetWaitTypeEx)
    ON_BN_CLICKED(IDC_BTN_THREAD_STOP_WAITTYPEEX_THREAD, &CThreadPage::OnBnClickedBtnThreadStopWaitTypeExThread)

    ON_BN_CLICKED(IDC_BTN_RWLOCK_START, &CThreadPage::OnBnClickedBtnRwlockStart)
    ON_BN_CLICKED(IDC_BTN_RWLOCK_ADD_READ, &CThreadPage::OnBnClickedBtnRwlockAddRead)
    ON_BN_CLICKED(IDC_BTN_RWLOCK_ADD_WRITE, &CThreadPage::OnBnClickedBtnRwlockAddWrite)
    ON_BN_CLICKED(IDC_BTN_RWLOCK_STOP, &CThreadPage::OnBnClickedBtnRwlockStop)
END_MESSAGE_MAP()

// CThreadPage 消息处理程序

LRESULT AFX_MSG_CALL CThreadPage::OnThreadUpdateProgress(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    int nLower = 0;
    int nUpper = 0;
    m_ProgressOfThread.GetRange(nLower,nUpper);
    if (m_ProgressOfThread.GetPos() >= nUpper)
    {
        m_ProgressOfThread.SetPos(nLower);
    }
    m_ProgressOfThread.StepIt();
    return 0;
}

void CThreadPage::SetThreadButtonStatus(BOOL bStarted, BOOL bPaused)
{
    GetDlgItem(IDC_BTN_THREAD_START)->EnableWindow(!bStarted);
    GetDlgItem(IDC_BTN_THREAD_PAUSE_RESUME)->EnableWindow(bStarted);
    GetDlgItem(IDC_BTN_THREAD_STOP)->EnableWindow(bStarted);

    GetDlgItem(IDC_BTN_THREAD_PAUSE_RESUME)->SetWindowText( bPaused ? TEXT("Resume") : TEXT("Pause"));
}

DWORD CThreadPage::TestThreadProc(void* pParam)
{
    CThreadPage* pThis = static_cast<CThreadPage*>(pParam);
    ASSERT(pThis);

    while (ftwtTimeOut == pThis->m_TestThread.SleepAndCheckStop(100))
    {
        if (ftwtContinue != pThis->m_TestThread.GetThreadWaitType(INFINITE))
        {
            break;
        }
        pThis->PostMessage(WM_THREAD_UPDATE_PROGRESS);
    }
    return 0;
}

void CThreadPage::OnBnClickedBtnThreadStart()
{
    m_TestThread.Start(&CThreadPage::TestThreadProc,this);
    SetThreadButtonStatus(TRUE,FALSE);
}


void CThreadPage::OnBnClickedBtnThreadPauseResume()
{
    if (m_TestThread.HadRequestPause())
    {
        m_TestThread.Resume();
    }
    else
    {
        m_TestThread.Pause();
    }
    SetThreadButtonStatus(TRUE,m_TestThread.HadRequestPause());
}

void CThreadPage::OnBnClickedBtnThreadStop()
{
    m_TestThread.StopAndWait();
    m_ProgressOfThread.SetPos(0);
    SetThreadButtonStatus(FALSE,FALSE);
}

DWORD CThreadPage::ExecProgressThreadProc(void* pParam)
{
    BOOL bRet = FALSE;
    TCHAR szCmdPath[MAX_PATH] = {0};
    GetSystemDirectory(szCmdPath, _countof(szCmdPath));
    PathAppend(szCmdPath, TEXT("cmd.exe"));

    STARTUPINFO startInfo = {0};
    PROCESS_INFORMATION processInfo = { 0 };
    API_VERIFY(CreateProcess(szCmdPath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, 
        &startInfo, &processInfo));
    if (bRet)
    {
        CThreadPage* pThis = reinterpret_cast<CThreadPage*>(pParam);
        DWORD dwResultIndex = 0;

        FTL::FTLThreadWaitType waitType = ftwtContinue;
        while (ftwtContinue == waitType)
        {
           waitType = pThis->m_threadExecProcess.GetThreadWaitTypeEx(&processInfo.hProcess, 1, &dwResultIndex, TRUE);
           Sleep(100);
        }

        if (waitType == ftwtUserHandle)
        {
            FTLASSERT(dwResultIndex == 0);
            FTLTRACE(TEXT("The Cmd.exe Process Exit\n"));
        }
        else
        {
            FTLASSERT(waitType == ftwtStop);
            FTLTRACE(TEXT("The Wait Thread Stop\n"));
        }
        SAFE_CLOSE_HANDLE(processInfo.hProcess, NULL);
        SAFE_CLOSE_HANDLE(processInfo.hThread, NULL)
    }

    return 0;
}

void CThreadPage::OnBnClickedBtnThreadGetWaitTypeEx()
{
    m_threadExecProcess.Start(ExecProgressThreadProc, this);
}

void CThreadPage::OnBnClickedBtnThreadStopWaitTypeExThread()
{
    m_threadExecProcess.Stop();
    m_threadExecProcess.Wait(INFINITE);
}
void CThreadPage::OnDestroy()
{
    m_TestThread.StopAndWait();
    if (m_pRWLocker)
    {
        m_pRWLocker->Stop();
        delete(m_pRWLocker);
        m_pRWLocker = NULL;
    }
    CPropertyPage::OnDestroy();
}

unsigned int CThreadPage::RWLockReadThreadProc(void* pParam)
{
    CFRWLocker* pRWGuard = static_cast<CFRWLocker*>(pParam);
    ASSERT(pRWGuard);
    DWORD dwThreadId = GetCurrentThreadId();
    for (int i = 0; i < 5; i++)
    {
        FTLTRACE(TEXT("%d[%s] Want to Read...\n"), dwThreadId, GetCurrentTimeString());
        FTLThreadWaitType waitType = pRWGuard->WaitToRead(INFINITE);
        if (ftwtContinue == waitType)
        {
            FTLTRACE(TEXT("%d[%s] Succeed Get Read Lock, will sleep 2s\n"), dwThreadId, GetCurrentTimeString());
            Sleep(3000);
            pRWGuard->Done();
            FTLTRACE(TEXT("%d[%s] Release Read Lock\n"), dwThreadId, GetCurrentTimeString());
        }
        else
        {
            FTLTRACE(TEXT("%d[%s] Read Thread Detect User Stop...\n"),dwThreadId, GetCurrentTimeString());
            break;
        }
    }
    return 0;
}

unsigned int CThreadPage::RWLockWriteThreadProc(void* pParam)
{
    CFRWLocker* pRWGuard = static_cast<CFRWLocker*>(pParam);
    ASSERT(pRWGuard);
    DWORD dwThreadId = GetCurrentThreadId();
    for (int i = 0; i < 3; i++)
    {
        FTLTRACE(TEXT("%d[%s] Want to Write...\n"), dwThreadId, GetCurrentTimeString());
        FTLThreadWaitType waitType = pRWGuard->WaitToWrite(INFINITE);
        if (ftwtContinue == waitType)
        {
            FTLTRACE(TEXT("%d[%s] Succeed Get Write Lock, will sleep 5s\n"), dwThreadId, GetCurrentTimeString());
            Sleep(5000);
            pRWGuard->Done();
            FTLTRACE(TEXT("%d[%s] Release Write Lock\n"), dwThreadId, GetCurrentTimeString());
        }
        else
        {
            FTLTRACE(TEXT("%d[%s] Write Thread Detect User Stop...\n"),dwThreadId,GetCurrentTimeString());
            break;
        }
    }
    return 0;
}


void CThreadPage::SetReadWriteLockButtonStatus(BOOL bStarted)
{
    GetDlgItem(IDC_BTN_RWLOCK_START)->EnableWindow(!bStarted);
    GetDlgItem(IDC_BTN_RWLOCK_ADD_READ)->EnableWindow(bStarted);
    GetDlgItem(IDC_BTN_RWLOCK_ADD_WRITE)->EnableWindow(bStarted);
    GetDlgItem(IDC_BTN_RWLOCK_STOP)->EnableWindow(bStarted);
}

void CThreadPage::OnBnClickedBtnRwlockStart()
{
    m_pRWLocker = new CFRWLocker();
    m_pRWLocker->Start();
    SetReadWriteLockButtonStatus(TRUE);
}


void CThreadPage::OnBnClickedBtnRwlockAddRead()
{
    HANDLE hThreadRead = (HANDLE)_beginthreadex(NULL,0, RWLockReadThreadProc, 
        m_pRWLocker, 0, NULL);
    m_hReadThreads.push_back(hThreadRead);
}

void CThreadPage::OnBnClickedBtnRwlockAddWrite()
{
    HANDLE hThreadWrite = (HANDLE)_beginthreadex(NULL,0, RWLockWriteThreadProc, 
        m_pRWLocker, 0, NULL);
    m_hWriteThreads.push_back(hThreadWrite);
}

void CThreadPage::OnBnClickedBtnRwlockStop()
{
    m_pRWLocker->Stop();

    FTLASSERT(m_hReadThreads.size() < 64);
    FTLASSERT(m_hWriteThreads.size() < 64);

    if (!m_hReadThreads.empty())
    {
        WaitForMultipleObjects((DWORD)m_hReadThreads.size(),&m_hReadThreads[0], TRUE, INFINITE);
        m_hReadThreads.clear();
    }
    if (!m_hWriteThreads.empty())
    {
        WaitForMultipleObjects((DWORD)m_hWriteThreads.size(), &m_hWriteThreads[0], TRUE, INFINITE);
        m_hWriteThreads.clear();
    }

    SAFE_DELETE(m_pRWLocker);
    SetReadWriteLockButtonStatus(FALSE);
}
