#ifndef F_DRIVER_HOOK_API_H
#define F_DRIVER_HOOK_API_H
#pragma once

//#include "WindowsTypes.h"

//TODO: Depends 查看 ntkrnlpa.exe + win32k.sys 并比较索引值
//SoftICE for Windows NT/2000的ntcall命令可以将这些System Service显示出 -- http://dev.21tx.com/2005/03/14/12553.html
//Hook NtGdiBitBlt  -- http://www.osronline.com/showthread.cfm?link=187950
//http://bbs.pediy.com/showthread.php?t=56955

/******************************************************************************************************************
* 在Windows操作系统中，消息钩子（通过SetWindowsHookEx设置）只会当前的桌面上的窗口有效，
* 所以可以建立一个安全桌面，用于运行需要严密保护的进程，这样非本桌面上运行的程序无法通过消息钩子的方式来
* 获取需要保护进程窗口的信息，达到了保护目标进程窗口的目的(360保险箱和金山密保 的 安全桌面功能 )
******************************************************************************************************************/

/******************************************************************************************************************
* 驱动级的 HOOK API( 通过更改 SSDP 中的函数地址来 Hook) -- 
*   注意先看该作者的其他下载。http://download.csdn.net/detail/jyw1111/5275135
*   缺点：可能被反病毒软件作为 rootkit 提醒
*
* 软中断：
*   Win2K -- int 2eh
*   WinXp -- sysenter
*
* Native API：
*   ntdll.dll -- Native API 的接口，通过软中断方式将调用传到内核来调用系统服务，但本身属于Ring 3。
*     函数名一般是对应Win32API前加 Nt 两个字母。
*     1.把对应的功能编号存入eax寄存器,
*     2.使用 SYSENTER 指令(早期使用触发 Int 2e 中断的方式)引发"自陷"
*     3.KiSystemService 内核处理程序运行
*     4.在SDT里找到功能编号的对应函数位置，并调用
*       SDT在系统中一共有4个()：
*       0.SSDT -- 位于 ntoskrnl.exe 或 ntkrnlpa.exe 中，记录了系统关键功能的位置,涉及文件操作,注册表操作,进程操作等。TODO：为什么有两个？区别？
*       1.Shadow SSDT -- 位于 win32k.sys 中， 记录了和用户与图形界面有关的功能。
*       2.3 -- 预留的SDT，即有人工创造SDT的可能，不过基本上没有意义
*     5.Object Hook -- 更底层的接口，不过已经很难控制了
*
* ntkrnlpa.exe (SSDT?) -- Kernel32.Dll 的内核实现
* ntoskrnl.exe -- Windows执行体组件
* win32k.sys (Shadow SSDT?) -- User32.dll + Gdi32.DLL 的内核实现
*   将GUI的实现放入内核模式，会增大系统不稳定的几率。但会大大提高图形处理的运行效率。
*
* KeUserModeCallBack -- 从R0调用位于Ring3的函数(系统所有的消息钩子回调都是利用该函数完成的)
* 
* NtGdiBitBlt
NtGdiOpenDCW 
NtGdiDeleteObjectApp
NtGdiBitBlt
NtGdiStretchBlt
NtUserPrintWindow
NtGdiDdLock
NtGdiPlgBlt
NtGdiMaskBlt
NtGdiTransparentBlt
NtGdiCreateDIBSection
NtGdiAlphaBlend
NtGdiGetPixel
NtUserSendInput
NtOpenSection
NtGdiBitBlt，NtGdiMaskBlt，NtGdiPlgBlt，NtGdiStretchBlt。NtUserBuildHwndList，NtUserFindWindowEx
******************************************************************************************************************/

/******************************************************************************************************************
*      System Services Descriptor Table
* SSDT(System Service Descriptor Table) -- 系统服务描述表，是各个系统服务号的 索引 => 对应系统服务函数的地址。
*   把Ring3的Win32 API和Ring0的内核API联系起来。
*   通过修改此表的函数地址可以对常用windows函数及API进行hook，从而实现对一些关心的系统动作进行过滤、监控的目的.
*   TODO: 怎么得到一个函数的SSDT索引号(dumpbin 或 depends ?)
*         http://www.cnblogs.com/nlsoft/archive/2013/04/02/2994628.html
* Shadow SSDT(Shadow System Services Descriptor Table) -- 管图形、用户相关的函数(gdi32.dll,user32.dll)
*
*   系统内部有个名为 KeServiceDescriptorTable 的变量，保存了相关信息，可通过 extern 后访问
*     如 WinDbg 中: dd KeServiceDescriptorTable
*
* 对SSDT进行Hook的过程
*   1.关闭CR0写保护(改变CR0寄存器的WP位)；
*   2.1.Hook Shadow SSDT 之前，必须将进程上下文切换到CSRSS进程
*   2.2.用新的函数地址替换原来SSDT中的函数地址
*   3.恢复CR0写保护
*
* 窗口保护 NtUserFindWindowEx、NtUserGetForegroundWindow、NtUserBuildHwndList、NtUserQueryWindow、
*          NtUserWindowFromPoint、NtUserSetParent、NtUserPostMessage、NtUserMessageCall、
*          NtUserSetWindowLong、NtUserShowWindow、NtUserDestroyWindow、NtUserCallHwndParamLock
* 模拟按键 NtUserSendInput
* 获取键盘按键状态 NtUserGetAsyncKeyState
* 打开安全桌面 NtUserOpenDesktop
* 将虚假按键还原成真实的按键 NtUserTranslateMessage
* 截屏保护 NtGdiBitBlt、NtGdiStretchBlt
******************************************************************************************************************/

/******************************************************************************************************************
* 系统进程分析
*   csrss.exe -- 会保存所有进程的句柄，
*     Console窗体的 ScrollDC 等API需要Hook这个进程才行？
*     win32!RawInputThread 通过 GUID_CLASS_KEYBOARD 获得键盘设备栈中PDO的符号连接名
*   System -- 系统进程，在系统启动时创建。
*     当驱动加载的时候，系统进程启动新的线程，调用对象管理器，创建一个驱动对象(DRIVER_OBJECT)，然后调用其 DriverEntry 
******************************************************************************************************************/

/******************************************************************************************************************
*             Ring3(kernel32)                    |   Native API(ntdll!)     |       Ring0(nt!)          |
* OpenProcess                                    | NtOpenProcess            | NtOpenProcess             |
* NtDuplicateObject                              |                          |                           |
* CreateThread/CreateRemoteThread                | NtCreateThread           | CreateThread              |
* OpenThread                                     |                          | NtOpenThread              |
* WriteProcessMemory                             | NtWriteVirtualMemory     | NtWriteVirtualMemory      |

* ------------------------------------------------ Shadow SSDT -----------------------------------------|
* FindWindow                                     |                          | NtUserFindWindowEx        |
* GetForegroundWindow                            |                          | NtUserGetForegroundWindow |
* EnumWindows                                    |                          | NtUserBuildHwndList       |
* GetWindowThreadProcessId                       |                          | NtUserQueryWindow         |
* WindowFromPoint                                |                          | NtUserWindowFromPoint     |
* SetParent                                      |                          | NtUserSetParent           |
* PostMessage                                    |                          | NtUserPostMessage         |
* SendMessage                                    |                          | NtUserMessageCall         |
* SetWindowLong                                  |                          | NtUserSetWindowLong       |
* ShowWindow                                     |                          | NtUserShowWindow          |
* DestroyWindow                                  |                          | NtUserDestroyWindow       |
* EnableWindow                                   |                          | NtUserCallHwndParamLock   |
* BitBlt                                         |                          | NtGdiBitBlt               |
* StretchBlt                                     |                          | NtGdiStretchBlt           | 
******************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#define SSDT_API_CALL_ENTER(x) (InterlockedIncrement(&x))
#define SSDT_API_CALL_LEAVE(x) (InterlockedDecrement(&x))

//SSDT表结构
typedef struct _SYS_SERVICE_TABLE { 
	void **ServiceTable;					//SSDT在内存中的基地址(其每一个的元素就是各个Rin0的函数？)
	unsigned long CounterTable; 
											//包含着 SSDT 中每个服务被调用次数的计数器。这个计数器一般由sysenter 更新
	unsigned long uNumberOfServices;		//SSDT项的个数？由 CounterTable 描述的服务的数目
	PUCHAR ArgumentsTable;					//TODO:实际上是参数个数? 包含每个系统服务参数字节数表的基地址-系统服务参数表
} SYS_SERVICE_TABLE, *PSYS_SERVICE_TABLE;

typedef struct _SCROLL_HOOK_TARGET
{
	HWND hTargetWindow;
	HANDLE hSelfProcess;
	HANDLE hTargetProcess;
} SCROLL_HOOK_TARGET, *PSCROLL_HOOK_TARGET;

typedef struct _SCROLL_DATA
{
	int dy;
	RECT rcScroll;
} SCROLL_DATA, *PSCROLL_DATA;
#pragma push(0)

SYS_SERVICE_TABLE *GetServiceDescriptorShadowTableAddress();
//int GetNtGdiBitBltIndex(SYS_SERVICE_TABLE *p);

void InstallCopyProtectHook(HANDLE hProcess);
void UnInstallScrollHook(void);

typedef struct _SYSTEM_HANDLE_INFORMATION{
	ULONG ProcessID;                //进程的标识ID
	UCHAR ObjectTypeNumber;         //对象类型
	UCHAR Flags;                    //0x01 = PROTECT_FROM_CLOSE,0x02 = INHERIT
	USHORT Handle;                  //对象句柄的数值
	PVOID  Object;                  //对象句柄所指的内核对象地址
	ACCESS_MASK GrantedAccess;      //创建句柄时所准许的对象的访问权
}SYSTEM_HANDLE_INFORMATION, * PSYSTEM_HANDLE_INFORMATION;

typedef struct _SYSTEM_HANDLE_INFORMATION_EX
{
	ULONG NumberOfHandles;
	SYSTEM_HANDLE_INFORMATION Information[1];
}SYSTEM_HANDLE_INFORMATION_EX, *PSYSTEM_HANDLE_INFORMATION_EX;

//typedef enum _OBJECT_INFORMATION_CLASS{
//	ObjectBasicInformation,
//	ObjectNameInformation,
//	ObjectTypeInformation,
//	ObjectAllInformation,
//	ObjectDataInformation
//}OBJECT_INFORMATION_CLASS, *POBJECT_INFORMATION_CLASS;

#ifdef __cplusplus
}
#endif


#if 0
//SSDT表结构
typedef struct _ServiceDescriptorTable {
	PULONG ServiceTableBase;			//System Service Dispatch Table 的基地址  
	PULONG ServiceCounterTableBase; 
	//包含着 SSDT 中每个服务被调用次数的计数器。这个计数器一般由sysenter 更新
	ULONG NumberOfServices;				//由 ServiceTableBase 描述的服务的数目
	PBYTE ParamTableBase;				//TODO:实际上是参数个数? 包含每个系统服务参数字节数表的基地址-系统服务参数表  
}*PServiceDescriptorTable;
//由SSDT索引号获取当前函数地址
//NtOpenProcess  [[KeServiceDescriptorTable]+0x7A*4]
extern "C" PServiceDescriptorTable KeServiceDescriptorTable;


或者(似乎没有看到 _SERVICE_DESCRIPTOR_TABLE 的定义)
typedef struct _SERVICE_DESCRIPTOR_TABLE *PSERVICE_DESCRIPTOR_TABLE;
xtern PSERVICE_DESCRIPTOR_TABLE KeServiceDescriptorTable;
#endif

#endif //F_DRIVER_HOOK_API_H

