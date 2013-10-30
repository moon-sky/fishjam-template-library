// BasePage.cpp : 实现文件
//

#include "stdafx.h"
#include "FTLDemo.h"
#include "BasePage.h"


// CBasePage 对话框

IMPLEMENT_DYNAMIC(CBasePage, CPropertyPage)

CBasePage::CBasePage()
	: CPropertyPage(CBasePage::IDD)
    , m_performanceSpinCountNumber(4000)
    , m_nPerformanceType(FALSE)
{
}

CBasePage::~CBasePage()
{
}

void CBasePage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_PERFORMANCE_SPINCOUNT_NUMBER, m_performanceSpinCountNumber);
    DDX_Radio(pDX, IDC_RADIO_PERFORMANCE_CS, m_nPerformanceType);
}


BEGIN_MESSAGE_MAP(CBasePage, CPropertyPage)
    ON_BN_CLICKED(IDC_BTN_ELAPSE_START, &CBasePage::OnBnClickedBtnStart)
    ON_BN_CLICKED(IDC_BTN_ELAPSE_PAUSE_RESUME, &CBasePage::OnBnClickedBtnPauseResume)
    ON_BN_CLICKED(IDC_BTN_ELAPSE_GET_ELAPSETIME, &CBasePage::OnBnClickedBtnGetElapsetime)
    ON_BN_CLICKED(IDC_BTN_ELAPSE_STOP, &CBasePage::OnBnClickedBtnStop)
    ON_BN_CLICKED(IDC_BTN_ELAPSE_EXE_COUNTER, &CBasePage::OnBnClickedBtnExeCounter)
    ON_BN_CLICKED(IDC_BTN_PERFORMANCE_START, &CBasePage::OnBnClickedBtnPerformanceStart)
    ON_BN_CLICKED(IDC_CHECK_PERFORMANCE_CS_SPINCOUNT, &CBasePage::OnBnClickedCheckCsSpincount)
    ON_BN_CLICKED(IDC_RADIO_PERFORMANCE_CS, &CBasePage::OnBnClickedRadioPerformanceCs)
    ON_BN_CLICKED(IDC_RADIO_PERFORMANCE_MUTEX, &CBasePage::OnBnClickedRadioPerformanceMutex)
	ON_BN_CLICKED(IDC_BTN_API_VERIFY, &CBasePage::OnBnClickedBtnApiVerify)
END_MESSAGE_MAP()


// CBasePage 消息处理程序

void CBasePage::SetElapseButtonStatus(FTL::RunningStatus status, BOOL bEnabled)
{
    switch(status)
    {
    case rsRunning:
        SetDlgItemText(IDC_BTN_ELAPSE_PAUSE_RESUME,TEXT("Pause"));
        break;
    case rsPaused:
        SetDlgItemText(IDC_BTN_ELAPSE_PAUSE_RESUME,TEXT("Resume"));
        break;
    case rsStopped:
        break;
    }
    GetDlgItem(IDC_BTN_ELAPSE_PAUSE_RESUME)->EnableWindow(bEnabled);
}

#include <ftlFile.h>
class CMyFindCallback : public IFileFindCallback
{
public:
    virtual FileFindResultHandle OnFindFile(LPCTSTR pszFilePath, const WIN32_FIND_DATA& findData)
    {
        LARGE_INTEGER fileSize;
        fileSize.HighPart = findData.nFileSizeHigh;
        fileSize.LowPart = findData.nFileSizeLow;

        FTLTRACE(TEXT("OnFindFile, %s, dwFileAttributes=%d(0x%x), size=%ld\n"), 
            pszFilePath, findData.dwFileAttributes, findData.dwFileAttributes, fileSize.QuadPart);
        return rhContinue; 
    }
};


void CBasePage::OnBnClickedBtnStart()
{
    //CMyFindCallback callback;
    //FTL::CFFileFinder finder;
    //finder.SetCallback(&callback);
    //finder.Find(TEXT("E:\\Code\\cafesolution\\src\\bin\\Debug\\PackageTemplate"));

    //return;

    //FTL::CFDirectoryCopier copier;
    //copier.Start(TEXT("E:\\Code\\cafesolution\\src\\bin\\Debug\\PackageTemplate"), TEXT("E:\\CopyTarget"));
    //copier.WaitToEnd();

    FUNCTION_BLOCK_NAME_TRACE_EX(TEXT("OnBnClickedBtnStart"), FTL::TraceDetailNone, 1);
    m_ElapseCounter.Start();
    SetElapseButtonStatus(m_ElapseCounter.GetStatus(), TRUE);
}


void CBasePage::OnBnClickedBtnPauseResume()
{
    BOOL bRet = FALSE;
    switch(m_ElapseCounter.GetStatus())
    {
    case rsRunning:
        API_VERIFY(m_ElapseCounter.Pause());
        break;
    case rsPaused:
        API_VERIFY(m_ElapseCounter.Resume());
        break;
    default:
        break;
    }
    SetElapseButtonStatus(m_ElapseCounter.GetStatus(), TRUE);
}

void CBasePage::OnBnClickedBtnGetElapsetime()
{
    LONGLONG llElapseTime = m_ElapseCounter.GetElapseTime();
    CString strElapseTime;
    switch (m_ElapseCounter.GetStatus())
    {
    case rsRunning:
        strElapseTime.Format(TEXT("%s"),TEXT("Running :"));
        break;
    case rsPaused:
        strElapseTime.Format(TEXT("%s"),TEXT("Paused :"));
        break;
    case rsStopped:
        strElapseTime.Format(TEXT("%s"),TEXT("Stopped :"));
        break;
    default:
        break;
    }
    
    strElapseTime.AppendFormat(TEXT("Elapse %I64d ns, %I64d ms"),llElapseTime,llElapseTime / 1000 / 1000);
    TRACE(TEXT("%s\n"),strElapseTime);
    SetDlgItemText(IDC_ELAPSE_TIME,strElapseTime);
}

void CBasePage::OnBnClickedBtnStop()
{
    m_ElapseCounter.Stop();
    SetElapseButtonStatus(m_ElapseCounter.GetStatus(), FALSE);
}

void CBasePage::OnBnClickedBtnExeCounter()
{
    CFElapseCounter counter;
    counter.Start();
    counter.Stop();
    FormatMessageBox(NULL,TEXT("ExecCounter"),MB_OK,TEXT("Exe Counter is %I64d"),counter.GetElapseTime());
}

void CBasePage::loopForCS(LPCRITICAL_SECTION pCS)
{
    for (int i = 0; i < 1000; i++)
    {
        for (int j = 0; j < 1000; j++)
        {
            EnterCriticalSection(pCS);
            LeaveCriticalSection(pCS);
        }
    }
}

void CBasePage::loopForMutex(HANDLE hMutex)
{
    for (int i = 0; i < 1000; i++)
    {
        for (int j = 0; j < 1000; j++)
        {
            WaitForSingleObject(hMutex, INFINITE);
            ReleaseMutex(hMutex);
        }
    }
}

DWORD CBasePage::TestPerformanceProc(LPVOID pParam)
{
    CBasePage* pThis = static_cast<CBasePage*>(pParam);
    FTLTRACE(TEXT("CBasePage::TestPerformanceProc, type = %s\n"),
        0 == pThis->m_nPerformanceType ? TEXT("CS") : TEXT("Mutex"));
    switch(pThis->m_nPerformanceType)
    {
    case 0: //CS
        loopForCS(&pThis->m_perCS);
        break;
    case 1: //Mutex
    default:
        loopForMutex(pThis->m_perMutex);
        break;
    }
    return 0;
}

void CBasePage::OnBnClickedBtnPerformanceStart()
{
    BOOL bRet = FALSE;
    HANDLE  hThread = NULL;
    CFElapseCounter elapse;

    API_VERIFY(UpdateData(TRUE));
    switch(m_nPerformanceType)
    {
    case 0:  //CS
        {
            if(IsDlgButtonChecked(IDC_CHECK_PERFORMANCE_CS_SPINCOUNT))
            {
                API_VERIFY(InitializeCriticalSectionAndSpinCount(&m_perCS, m_performanceSpinCountNumber));
            }
            else
            {
                InitializeCriticalSection(&m_perCS);
            }
        }
        break;
    case 1:  //Mutex
            m_perMutex = CreateMutex(NULL,FALSE, NULL);
            API_VERIFY(NULL != m_perMutex);
            break;
    default:
        FTLASSERT(FALSE);
        break;
    }

    if (IsDlgButtonChecked(IDC_CHECK_PERFORMANCE_USE_THREAD))
    {
        hThread = CreateThread(NULL,0,&CBasePage::TestPerformanceProc, (LPVOID)this, 
            CREATE_SUSPENDED, NULL);
        API_VERIFY(NULL != hThread);
    }

    elapse.Start();
    if (hThread) //Use Thread
    {
        ResumeThread(hThread);
    }
    switch(m_nPerformanceType)
    {
    case 0: //CS
        loopForCS(&m_perCS);
        break;
    case 1:
        loopForMutex(m_perMutex);
        break;
    default:
        FTLASSERT(FALSE);
        break;
    }
    
    if (hThread)
    {
        WaitForSingleObject(hThread,INFINITE);
    }
    elapse.Stop();

    //清除资源
    if (hThread)
    {
        SAFE_CLOSE_HANDLE(hThread, NULL);
    }
    
    switch(m_nPerformanceType)
    {
    case 0:
        //CS
        DeleteCriticalSection(&m_perCS);
        break;
    case 1:
        SAFE_CLOSE_HANDLE(m_perMutex, NULL);
        break;
    default:
        FTLASSERT(FALSE);
        break;
    }

    CString strElapseTime;
    strElapseTime.Format(TEXT("10^6 %s Elapse %I64d ns, %I64d ms"),
        m_nPerformanceType == 0 ? TEXT("CS") : TEXT("Mutex"),
        elapse.GetElapseTime(),
        elapse.GetElapseTime()/NANOSECOND_PER_MILLISECOND);
    SetDlgItemText(IDC_PERFORMANCE_ELAPSE_TIME, strElapseTime);
}
void CBasePage::OnBnClickedCheckCsSpincount()
{
   GetDlgItem(IDC_EDIT_PERFORMANCE_SPINCOUNT_NUMBER)->EnableWindow(
       IsDlgButtonChecked(IDC_CHECK_PERFORMANCE_CS_SPINCOUNT));
}

void CBasePage::OnBnClickedRadioPerformanceCs()
{
    GetDlgItem(IDC_CHECK_PERFORMANCE_CS_SPINCOUNT)->EnableWindow(TRUE);
}

void CBasePage::OnBnClickedRadioPerformanceMutex()
{
    GetDlgItem(IDC_CHECK_PERFORMANCE_CS_SPINCOUNT)->EnableWindow(FALSE);
}

BOOL SomeFun1()
{
	FUNCTION_BLOCK_TRACE(1);
	SetLastError(ERROR_INVALID_PARAMETER);
	return FALSE;
}

BOOL SomeFun2()
{
	FUNCTION_BLOCK_TRACE(1);
	BOOL bRet = FALSE;
	API_VERIFY_EXCEPT1(SomeFun1(), ERROR_INVALID_PARAMETER);
	return bRet;
}

BOOL SomeFun3()
{
	FUNCTION_BLOCK_TRACE(1);
	BOOL bRet = FALSE;
	API_VERIFY_EXCEPT1(SomeFun2(), ERROR_INVALID_PARAMETER);
	return bRet;
}

void CBasePage::OnBnClickedBtnApiVerify()
{
	FUNCTION_BLOCK_TRACE(1);
	BOOL bRet = FALSE;
	API_VERIFY_EXCEPT1(SomeFun3(), ERROR_INVALID_PARAMETER);
	FTLASSERT(GetLastError() == ERROR_INVALID_PARAMETER);
}
