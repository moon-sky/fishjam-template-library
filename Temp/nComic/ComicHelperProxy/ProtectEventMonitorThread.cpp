#include "StdAfx.h"
#include "resource.h"
#include "ProtectEventMonitorThread.h"
#include "../ComicHelper/ComicHelper.h"
#include "ComicHelperProxy.h"
#include "MainFrm.h"

CProtectEventMonitorThread::CProtectEventMonitorThread(CMainFrame* pMainFrame)
{
    m_pMainFrame = pMainFrame;
    m_pProtectWndInfoFileMap = NULL;
}

CProtectEventMonitorThread::~CProtectEventMonitorThread(void)
{
}

BOOL CProtectEventMonitorThread::Start()
{
    BOOL bRet = FALSE;
    API_VERIFY(__super::Start(MonitorThreadProc, this, FALSE));
    return bRet;
}

BOOL CProtectEventMonitorThread::Stop()
{
    BOOL bRet = FALSE;
    API_VERIFY(__super::StopAndWait());
    return bRet;
}

HWND CProtectEventMonitorThread::GetProtectWnd() const
{
    HWND hWndProtect = NULL;
    if (m_pProtectWndInfoFileMap)
    {
        hWndProtect = (HWND)m_pProtectWndInfoFileMap->hWndProtect;
    }
    return hWndProtect;
}

DWORD CProtectEventMonitorThread::MonitorThreadProc(LPVOID lpThreadParameter)
{
    DWORD dwResult = 0;
    CProtectEventMonitorThread* pThis = static_cast<CProtectEventMonitorThread*>(lpThreadParameter);
    dwResult = pThis->_InnerMonitorThreadProc();
    return dwResult;
}

DWORD CProtectEventMonitorThread::_InnerMonitorThreadProc()
{
    BOOL bRet = FALSE;
    DWORD dwResult = 0;

    FTLTRACE(TEXT("Enter CProtectEventMonitorThread::_InnerMonitorThreadProc\n"));
    FTL::FTLThreadWaitType waitType = FTL::ftwtError;
    
    HRESULT hr = E_FAIL;
    COM_VERIFY(m_FileMap.OpenMapping(COMIC_PROTECT_WND_FILE_MAP_NAME, sizeof(ProtectWndInfoFileMap32)));
    if (SUCCEEDED(hr))
    {
        m_pProtectWndInfoFileMap = (ProtectWndInfoFileMap32*)m_FileMap.GetData();
        FTLASSERT(m_pProtectWndInfoFileMap);
        if (m_pProtectWndInfoFileMap)
        {
            //add for Proxy restart
            if (m_pProtectWndInfoFileMap->hWndProtect)
            {
                API_VERIFY(EnableWindowProtected(GetCurrentProcessId(), (HWND)m_pProtectWndInfoFileMap->hWndProtect));
                m_pMainFrame->Invalidate(TRUE);
                m_pMainFrame->UpdateWindow();
            }
#if defined _M_IX86
            HANDLE hProtectEvent = ::OpenEvent(EVENT_ALL_ACCESS, FALSE, COMIC_PROTECT_NOTIFY_EVENT_NAME32);
#else
            HANDLE hProtectEvent = ::OpenEvent(EVENT_ALL_ACCESS, FALSE, COMIC_PROTECT_NOTIFY_EVENT_NAME64);
#endif 

            API_ASSERT(NULL != hProtectEvent);
            if (NULL != hProtectEvent)
            {
                _HandleProtectEventLoop(hProtectEvent);
                API_VERIFY(CloseHandle(hProtectEvent));
            }
        }
        
        COM_VERIFY(m_FileMap.Unmap());
    }

    FTLTRACE(TEXT("Leave CProtectEventMonitorThread::_InnerMonitorThreadProc\n"));
    return 0;
}

FTL::FTLThreadWaitType CProtectEventMonitorThread::_HandleSingleProtectEvent()
{
    FTLASSERT(m_pProtectWndInfoFileMap);
    BOOL bRet = FALSE;
    FTL::FTLThreadWaitType waitType = FTL::ftwtContinue;

    if (m_pProtectWndInfoFileMap)
    {
        FTLTRACE(TEXT("CProtectEventMonitorThread::_HandleSingleProtectEvent, Command=%d\n"), m_pProtectWndInfoFileMap->dwCommand);

        switch (m_pProtectWndInfoFileMap->dwCommand)
        {
        case COMMAND_BEGIN_PROTECT_WND:
            {
                HWND hWndProtect = (HWND)m_pProtectWndInfoFileMap->hWndProtect;
                if (hWndProtect)
                {
                    API_VERIFY(EnableWindowProtected(GetCurrentProcessId(), hWndProtect));
                    m_pMainFrame->Invalidate(TRUE);
                    m_pMainFrame->UpdateWindow();
                }
            }
            break;
        case COMMAND_END_PROTECT_WND:
            {
                HWND hWndProtect = (HWND)m_pProtectWndInfoFileMap->hWndProtect;
                m_pProtectWndInfoFileMap->hWndProtect = NULL;
                API_VERIFY(DisableWindowProtected(hWndProtect));
            }
            m_pMainFrame->Invalidate(TRUE);
            m_pMainFrame->UpdateWindow();
            break;
        case COMMAND_NOTIFY_END_HELPER_PROXY:
            if (m_pMainFrame)
            {
                m_pMainFrame->PostMessage(WM_CLOSE, 0, 0);
                waitType = FTL::ftwtStop;
            }
            break;
        default:
            FTLASSERT(FALSE && TEXT("Not Handled dwCommand in _HandleSingleProtectEvent"));
            break;
        }
    }
    return waitType;
}

DWORD CProtectEventMonitorThread::_HandleProtectEventLoop(HANDLE hProtectEvent)
{
    FTL::FTLThreadWaitType waitType = FTL::ftwtContinue;
    
    HANDLE waitHandles[] = {
        m_hEventStop,
        hProtectEvent
    };

    while (FTL::ftwtContinue == waitType)
    {
        DWORD dwResult  = ::WaitForMultipleObjects(_countof(waitHandles), waitHandles, FALSE, INFINITE);
        switch (dwResult)
        {
        case WAIT_OBJECT_0:
            //stop event
            waitType = FTL::ftwtStop;
            break;
        case WAIT_OBJECT_0 + 1: //Protect Event
            waitType = _HandleSingleProtectEvent();
            break;
        default:
            FTLASSERT(FALSE);   //will not come here
            waitType = FTL::ftwtError;
            break;
        }
    }
    return 0;
}

