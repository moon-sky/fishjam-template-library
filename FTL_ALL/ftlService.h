#ifndef FTL_SERVICE_H
#define FTL_SERVICE_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlService.h requires ftlbase.h to be included first
#endif

/*************************************************************************************************************************
* SCM(Service Control Manager)--服务控制管理程序
* 
* Windows服务分成两种类型：interactive service (交互式服务)和non-interactive service（非交互式服务）。
* 1.如果一个服务以"LocalSystem Account"运行，并且设置了"SERVICE_INTERACTIVE_PROCESS"属性。
*   那么它就被认为是一个interactive service。交互式服务在非Vista系统下（XP,NT），
*   使用"interactive window station"(WinSta0)，显示UI接受用户输入，和用户进行交互。
*   设置"SERVICE_INTERACTIVE_PROCESS"属性：
*     服务属性页->["log on"属性页]->[local system account] ->[Allow service to interact with desktop]
* 2.如果一个服务以"LocalSystem Account"运行，但是没有设置"SERVICE_INTERACTIVE_PROCESS"属性,则使用名为
*   "Service-0x0-3e7$\default" 的"window station",非交互的,XP/2003/Vista 都相同;
* 3.如果服务以"User Account"运行，它所使用的"window station"的名字基于用户的SID:
*   Service-0xZ1-Z2$, Z1, Z2分别代表SID的高位和低位 -- 非交互的.
*
* Interactive service存在的安全问题( WinXP中 服务程序和第一个登录用户的应用程序 在同一个Session 0中运行)。
*   在Vista下，将所有的系统程序和服务程序都隔离到一个Session 0的空间中,其他应用程序则通过分离的Session运行。
*   由于用户程序和服务程序运行在完全分离的Session中,他们使用不同的window station和桌面,这种空间的分离，
*   使得用户进程没有机会访问到服务程序的窗口句柄，这样就有效的避免了不良用户程序的攻击。
*   注意：可通过ProcessExplorer 的 Security 属性页查看进程所在Session
*
* Session分离带来的影响 -- 
*  在分离前，服务进程如果想和用户进程通信，只需要跨越进程边界，
*  而分离后，不仅仅是跨越进程边界，还需要跨越Session边界才能进行。
*  1>.不同Session内的进程无法互相访问各自的窗口句柄,不能使用 PostMessage/SendMessage 等函数;
*  2>.利用内核对象进行用户进程和服务进程的同步时，在分离Session模式下，需要指定内核对象的名字空间(如 "Global\\Event1")
* 
* Services将UI显示到用户桌面上:
*  1> WTSSendMessage(WTS_CURRENT_SERVER_HANDLE, WTSGetActiveConsoleSessionId(), ) -- 显示简单的Message
*  2> 复杂的UI -- 使用 CreateProcessAsUser()在用户的Session内创建一个进程，让这个进程来显示UI.
*     WTSQueryUserToken(WTSGetActiveConsoleSessionId(), &hToken) -> DuplicateTokenEx -> CreateEnvironmentBlock -> CreateProcessAsUser
*  3>.任何支持跨越主机的通信方式都支持跨越Session进行通信,简单适用的方式有RPC(远程过程调用)Named Pipes(命名管道)，WCF 等。
*
* 服务的安全控制
*   1.不接受停止控制(如 Fasoo) -- AcceptStop 和 AcceptPause 设置成 FALSE， 可通过禁用的方式？
*     可通过命令行查看属性确认：wmic service where name="服务名" get /value
*     问题：如果是独立进程的服务可以中止进程，但是如果处于svchost宿主后面就没辙了，因为你强行停掉它也会把同一宿主下的其它服务也退出了
*   2.禁止 禁用 -- 
*   3.DesktopInteract = FALSE
*************************************************************************************************************************/

/*************************************************************************************************************************
* ATL 服务程序Service(Exe)
*   安装服务(服务的注册, Register as Service，在SCM中查看)：XXX /Service, 
*   TODO: /RegServer 是本地服务器注册(注册COM？)
*   卸载服务：XXX /UnRegServer
*   启动|停止 服务： net start|stop 服务名称
* 
* 更改
*   服务名称：IDS_SERVICENAME
*   服务配置和描述等：重载 CAtlServiceModuleT::RegisterAppId(bool bService = false){ ChangeServiceConfig + ChangeServiceConfig2(SERVICE_CONFIG_DESCRIPTION) }
*   修改安全设置(否则客户端调用时可能会 E_ACCESSDENIED)：InitializeSecurity 中
*    CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_NONE, RPC_C_IMP_LEVEL_IDENTIFY, NULL, EOAC_NONE, NULL);
*
* 扩展实现
*   1.通过增加 ATL Simple Object 的方式增加功能，
*     接口所能使用的参数必须是OLE兼容的，否则在QueryInterface时可能会报 0x80020008(DISP_E_BADVARTYPE) 的错误
*   2.需要在对应的 rgs 文件中增加 val AppID = s '%APPID%'，否则客户端创建对象的时候会超时并报错误：0x80080005(CO_E_SERVER_EXEC_FAILURE)
*   3.在Win7下或 XP跨用户调用时需要 注册 xxxPS 工程生成的DLL(跨Session交互)
*
* 交互服务(WinXP以前)
*   GetProcessWindowStation() -- 获取当前进程的 Window Station
*   OpenDesktop("default", xxx)
*   OpenWindowStation("winsta0", xxx)  
* 
*
*
*************************************************************************************************************************/

/*************************************************************************************************************************
* C#开发服务
*   从 ServiceBase 继承，有可重载的 OnStart/OnStop 等方法，
*   程序编译后通过Installutil 将其加载到系统服务中
*************************************************************************************************************************/

#pragma comment(lib, "wtsapi32.lib")
#pragma comment(lib, "Userenv.lib")

namespace FTL
{
    class CFService
    {
    public:
        //判断服务是否安装
        FTLINLINE static BOOL IsServiceInstalled(LPCTSTR lpServiceName);
        FTLINLINE static BOOL InstallService(LPCTSTR lpBinaryPathName,LPCTSTR lpServiceName, LPCTSTR lpDependencies);
        FTLINLINE static BOOL UninstallService(LPCTSTR lpServiceName);

        //模拟Service 进程显示UI的步骤
        //http://blog.csdn.net/sonsie007/article/details/8835830
        FTLINLINE static BOOL CreateServiceUIProcess(LPCTSTR pszProcessPath, BOOL bAsSystem, ULONG SessionId = WTSGetActiveConsoleSessionId());
    };

}
#endif //FTL_SERVICE_H

#ifndef USE_EXPORT
#  include "ftlService.hpp"
#endif 