#pragma once

#include <WinSvc.h>

class CFDriverDemoController
{
public:
    CFDriverDemoController(void);
    ~CFDriverDemoController(void);

    BOOL InstallService();
    BOOL UnInstallService();

    //BOOL StartService();
    //BOOL StopService();

private:
    SC_HANDLE m_hSCManager;
    SC_HANDLE m_hDriverDemoService;

};
