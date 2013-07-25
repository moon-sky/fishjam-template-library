#ifndef F_DRIVER_HOOK_API_H
#define F_DRIVER_HOOK_API_H
#pragma once

//#include "WindowsTypes.h"

//TODO: Depends 查看 ntkrnlpa.exe + win32k.sys 并比较索引值
//SoftICE for Windows NT/2000的ntcall命令可以将这些System Service显示出 -- http://dev.21tx.com/2005/03/14/12553.html
//Hook NtGdiBitBlt  -- http://www.osronline.com/showthread.cfm?link=187950

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
* NtGdiBitBlt
* 
******************************************************************************************************************/

/******************************************************************************************************************
* SSDT(System Service Descriptor Table) -- 系统服务描述表，是各个系统服务号的 索引 => 对应系统服务函数的地址。
*   把ring3的Win32 API和ring0的内核API联系起来？
*   通过修改此表的函数地址可以对常用windows函数及API进行hook，从而实现对一些关心的系统动作进行过滤、监控的目的
*   TODO: 怎么得到一个函数的SSDT索引号(dumpbin 或 depends ?)
*         http://www.cnblogs.com/nlsoft/archive/2013/04/02/2994628.html
*
*   系统内部有个名为 KeServiceDescriptorTable 的变量，保存了相关信息，可通过 extern 后访问
*     如 WinDbg 中: dd KeServiceDescriptorTable
******************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#define   CR0_WP_MASK     0x0FFFEFFFF

#define SSDT_API_CALL_ENTER(x) (InterlockedIncrement(&x))
#define SSDT_API_CALL_LEAVE(x) (InterlockedDecrement(&x))

typedef struct _SYS_SERVICE_TABLE { 
	void **ServiceTable; 
	unsigned long CounterTable; 
	unsigned long ServiceLimit; 
	PUCHAR ArgumentsTable; 
} SYS_SERVICE_TABLE, *PSYS_SERVICE_TABLE;

typedef void *HWND;
typedef void *HDC;
typedef void *HRGN;

typedef unsigned int UINT;
//typedef short unsigned int WORD;
typedef unsigned char BYTE, *PBYTE;

#pragma push(1)
typedef struct _RECT { 
	LONG left; 
	LONG top; 
	LONG right; 
	LONG bottom; 
} RECT, *PRECT;
typedef RECT *LPRECT;

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
//int GetScrollDCIndex(SYS_SERVICE_TABLE *p);

void InstallCopyProtectHook(HANDLE hProcess);
void UnInstallScrollHook(void);

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

