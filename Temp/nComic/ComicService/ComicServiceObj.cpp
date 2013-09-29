// ComicServiceObj.cpp : Implementation of CComicServiceObj

#include "stdafx.h"
#include "ComicServiceObj.h"
#include "../ComicHelper/ComicHelper.h"
#include "../ComicHelperProxy/ComicHelperProxy.h"
#include <ftlService.h>
#include <WtsApi32.h>
#include <ftlSystem.h>

//#define JUST_WIN32

// CComicServiceObj

CComicServiceObj::CComicServiceObj()
{
    FTLTRACE(TEXT("Enter CComicServiceObj::CComicServiceObj\n"));
    HRESULT hr = E_FAIL;
    m_HeartBeatTicket = 0;
#ifdef JUST_WIN32
    m_bIsWindows64 = FALSE; 
#else
    m_bIsWindows64 = FTL::CFSystemUtil::Is64BitWindows();
#endif 
    
    m_bUserStopProcess = FALSE;
    m_hEventStop = ::CreateEvent(NULL, TRUE, FALSE, NULL);

    m_hHelperProxyProcess32 = NULL;
    m_hEventProtectNotify32 = NULL;

    m_hHelperProxyProcess64 = NULL;
    m_hEventProtectNotify64 = NULL;
    m_pProtectWndInfoMap = NULL;
    m_pHelperProxyProcessMonitorThread = NULL;
}

CComicServiceObj::~CComicServiceObj()
{
    BOOL bRet = FALSE;
    FTLASSERT(NULL == m_hEventProtectNotify32);
    FTLASSERT(NULL == m_hEventProtectNotify64);
    SAFE_CLOSE_HANDLE(m_hEventStop, NULL);
}

HRESULT CComicServiceObj::FinalConstruct()
{
    BOOL bRet = FALSE;
    HRESULT hr = E_FAIL;

    SECURITY_DESCRIPTOR secutityDese;
    API_VERIFY(::InitializeSecurityDescriptor(&secutityDese, SECURITY_DESCRIPTOR_REVISION));
    API_VERIFY(::SetSecurityDescriptorDacl(&secutityDese, TRUE, NULL, FALSE));
    SECURITY_ATTRIBUTES securityAttr;
    securityAttr.nLength = sizeof( SECURITY_ATTRIBUTES);
    securityAttr.bInheritHandle = FALSE;
    securityAttr.lpSecurityDescriptor = &secutityDese;

    COM_VERIFY(m_FileMap.MapSharedMem(sizeof(ProtectWndInfoFileMap32), COMIC_PROTECT_WND_FILE_MAP_NAME,
        NULL, 
        &securityAttr, 
        PAGE_READWRITE, FILE_MAP_ALL_ACCESS));
    if (SUCCEEDED(hr))
    {
        m_pProtectWndInfoMap = (ProtectWndInfoFileMap32*)m_FileMap;
    }

    m_hEventProtectNotify32 = ::CreateEvent(&securityAttr, FALSE, FALSE, COMIC_PROTECT_NOTIFY_EVENT_NAME32);
    API_ASSERT(m_hEventProtectNotify32 != NULL);

    API_VERIFY(_CreateProxyProcess(FALSE));
    if (m_bIsWindows64)
    {
        m_hEventProtectNotify64 = ::CreateEvent(&securityAttr, FALSE, FALSE, COMIC_PROTECT_NOTIFY_EVENT_NAME64);
        API_VERIFY(_CreateProxyProcess(TRUE));
    }

    m_pHelperProxyProcessMonitorThread = new FTL::CFThread<DefaultThreadTraits>(m_hEventStop, NULL);
    if (m_pHelperProxyProcessMonitorThread)
    {
        API_VERIFY(m_pHelperProxyProcessMonitorThread->Start(_ProcessMonitorProc, this));
    }
    return hr;
}

void CComicServiceObj::FinalRelease()
{
    FTLTRACE(TEXT("CComicServiceObj::FinalRelease\n"));
    HRESULT hr = E_FAIL;
    BOOL bRet = FALSE;
    API_VERIFY(SetEvent(m_hEventStop));
    if (m_pHelperProxyProcessMonitorThread)
    {
        m_pHelperProxyProcessMonitorThread->Wait();
        delete m_pHelperProxyProcessMonitorThread;
        m_pHelperProxyProcessMonitorThread = NULL;
    }
    API_VERIFY(_StopHelperProxyProcess());

    COM_VERIFY(m_FileMap.Unmap());
    SAFE_CLOSE_HANDLE(m_hEventProtectNotify32, NULL);
    SAFE_CLOSE_HANDLE(m_hEventProtectNotify64, NULL);
}

BOOL CComicServiceObj::_CreateProxyProcess(BOOL isWin64)
{
    BOOL bRet = FALSE;
    TCHAR szHelperProxyPath[MAX_PATH] = {0};
    GetModuleFileName(NULL, szHelperProxyPath, _countof(szHelperProxyPath));
    API_VERIFY(PathRemoveFileSpec(szHelperProxyPath));
    HANDLE* pHelperProcess = NULL;
    if (isWin64)
    {
        pHelperProcess = &m_hHelperProxyProcess64;
        API_VERIFY(PathAppend(szHelperProxyPath, TEXT("ComicHelperProxy64.exe")));
    }
    else
    {
        pHelperProcess = &m_hHelperProxyProcess32;
        API_VERIFY(PathAppend(szHelperProxyPath, TEXT("ComicHelperProxy32.exe")));
    }

    if (*pHelperProcess)
    {
        DWORD dwWaitResult = WaitForSingleObject(*pHelperProcess, 0);
        if (WAIT_OBJECT_0 == dwWaitResult)
        {
            //the process already end, so 
            *pHelperProcess = NULL;
        }
    }
    if (NULL == *pHelperProcess)
    {
        PROCESS_INFORMATION processInfo = {0};
        FTLTRACE(TEXT("ProxyProcess is %s\n"), szHelperProxyPath);
        API_VERIFY(FTL::CFService::CreateServiceUIProcess(szHelperProxyPath, TRUE, &processInfo, WTSGetActiveConsoleSessionId()));
        if (bRet)
        {
            *pHelperProcess = processInfo.hProcess;
            CloseHandle(processInfo.hThread);
        }
    }
    return bRet;
}

BOOL CComicServiceObj::_StopHelperProxyProcess()
{
    BOOL bRet = TRUE;
    m_bUserStopProcess = TRUE;

    if (m_pProtectWndInfoMap)
    {
        m_pProtectWndInfoMap->dwCommand = COMMAND_NOTIFY_END_HELPER_PROXY;
        API_VERIFY(SetEvent(m_hEventProtectNotify32));
        if (m_bIsWindows64)
        {
            API_VERIFY(SetEvent(m_hEventProtectNotify64));
            API_VERIFY(WAIT_OBJECT_0 == WaitForSingleObject(m_hHelperProxyProcess64, INFINITE)); //FTL_MAX_THREAD_DEADLINE_CHECK
            if (!bRet)
            {
                API_VERIFY(TerminateProcess(m_hHelperProxyProcess64, GetLastError()));
            }
        }
        API_VERIFY(WAIT_OBJECT_0 == WaitForSingleObject(m_hHelperProxyProcess32, INFINITE)); //FTL_MAX_THREAD_DEADLINE_CHECK
        if (!bRet)
        {
            API_VERIFY(TerminateProcess(m_hHelperProxyProcess32, GetLastError()));
        }
    }

    m_hHelperProxyProcess32 = NULL;
    m_hHelperProxyProcess64 = NULL;
    return bRet;
}

DWORD WINAPI CComicServiceObj::_ProcessMonitorProc(LPVOID lpThreadParameter)
{
    CComicServiceObj* pThis = static_cast<CComicServiceObj*>(lpThreadParameter);
    return pThis->_InnerProcessMonitorProc();
}

DWORD CComicServiceObj::_InnerProcessMonitorProc()
{
    BOOL bRet = FALSE;

    DWORD dwWaitCount = 2;
    if (m_bIsWindows64 && NULL != m_hHelperProxyProcess64)
    {
        dwWaitCount = 3;
    }
    BOOL bQuit = FALSE;
    while (!bQuit)
    {
        HANDLE hWaitHandles[3] = {
            m_hEventStop,
            m_hHelperProxyProcess32,
            m_hHelperProxyProcess64
        };

        DWORD dwWaitResult = WaitForMultipleObjects(dwWaitCount, hWaitHandles, FALSE, INFINITE);
        switch (dwWaitResult)
        {
        case WAIT_OBJECT_0:
            //Stop Event
            bQuit = TRUE;
            break;
        case WAIT_OBJECT_0 + 1:
            //32 Bit Process is closed
            if (!m_bUserStopProcess)
            {
                API_VERIFY(_CreateProxyProcess(FALSE));
            }
            break;
        case WAIT_OBJECT_0 + 2:
            //64 Bit Process is closed
            if(!m_bUserStopProcess)
            {
                API_VERIFY(_CreateProxyProcess(TRUE));
            }
            break;
        default:
            FTLASSERT(FALSE);
            break;
        }
    }

    return 0;
}

STDMETHODIMP CComicServiceObj::ProtectWnd(OLE_HANDLE hWnd, OLE_COLOR clrDisabled)
{
    DWORD dwResult = 0;
    BOOL bRet = FALSE;
    FTLTRACE(TEXT("CComicServiceObj::ProtectWnd, hWnd=0x%x\n"), hWnd);
    
    FTLASSERT(m_pProtectWndInfoMap);
    if (m_pProtectWndInfoMap)
    {
        m_pProtectWndInfoMap->hWndProtect = (ULONG)hWnd;
        m_pProtectWndInfoMap->clrDisabled = COLORREF(clrDisabled);
        m_pProtectWndInfoMap->dwCommand = COMMAND_BEGIN_PROTECT_WND;
        m_pProtectWndInfoMap->dwProtectProcessId = GetCurrentProcessId();
    }

    API_VERIFY(SetEvent(m_hEventProtectNotify32));
    if (m_bIsWindows64)
    {
        API_VERIFY(SetEvent(m_hEventProtectNotify64));
    }

    FTLTRACE(TEXT("CComicServiceObj::ProtectWnd, hWnd=0x%x\n"), hWnd);
    return S_OK;
}

STDMETHODIMP CComicServiceObj::UnProtectWnd(OLE_HANDLE hWnd)
{
    FTLTRACE(TEXT("CComicServiceObj::UnProtectWnd, hWnd=0x%x\n"), hWnd);
    BOOL bRet = FALSE;

    FTLASSERT(m_pProtectWndInfoMap);
    if (m_pProtectWndInfoMap)
    {
        m_pProtectWndInfoMap->dwCommand = COMMAND_END_PROTECT_WND;
    }
    API_VERIFY(SetEvent(m_hEventProtectNotify32));
    if (m_bIsWindows64)
    {
        API_VERIFY(SetEvent(m_hEventProtectNotify64));
    }

    return S_OK;
}

STDMETHODIMP CComicServiceObj::HeartBeat(LONG curTicket)
{
    //do nothing, just remain current ticket
    m_HeartBeatTicket = curTicket;
    return S_OK;
}

STDMETHODIMP CComicServiceObj::UpdateSetupFileInfo(LONG nSetupFilePid)
{
    FTLTRACE(TEXT("CComicServiceObj::UpdateSetupFileInfo, nSetupFilePid=%d\n"), nSetupFilePid);
    m_pProtectWndInfoMap->dwSetupFilePID = (DWORD)nSetupFilePid;
    return S_OK;
}