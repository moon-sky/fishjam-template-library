#pragma once

#include <WinSvc.h>

//NT 式的驱动可以通过服务方式加载
//WDM 式的驱动需要通过 INF 文件的方式来加载(Pnp会找到硬件设备的 VenderID 和 ProductID,会组合成注册表中的 HardwareID)

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
