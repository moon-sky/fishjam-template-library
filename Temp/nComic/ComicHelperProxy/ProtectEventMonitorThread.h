#pragma once

#include <ftlThread.h>
#include <atlfile.h>

#include "ComicHelperProxy.h"

class CMainFrame;

class CProtectEventMonitorThread : private FTL::CFThread<DefaultThreadTraits>
{
public:
    CProtectEventMonitorThread(CMainFrame* pMainFrame);
    ~CProtectEventMonitorThread(void);
    BOOL Start();
    BOOL Stop();

    HWND GetProtectWnd() const;       //TODO:Debug
protected:
    static DWORD WINAPI MonitorThreadProc(LPVOID lpThreadParameter);

    DWORD _InnerMonitorThreadProc();
    DWORD _HandleProtectEventLoop(HANDLE hProtectEvent);
    FTL::FTLThreadWaitType _HandleSingleProtectEvent();

private:
    CMainFrame*                 m_pMainFrame;
    ProtectWndInfoFileMap32*      m_pProtectWndInfoFileMap;
    CAtlFileMapping<ProtectWndInfoFileMap32> m_FileMap;
};
