#ifndef F_DRIVER_HOOK_API_H
#define F_DRIVER_HOOK_API_H

/******************************************************************************************************************
* 驱动级的 HOOK API( 通过 SSDP 来Hook) -- 注意先看该作者的其他下载。http://download.csdn.net/detail/jyw1111/5275135
*   缺点：可能被反病毒软件作为 rootkit 提醒
*
*
* Native API：
*   ntdll.dll -- Native API 的接口，将调用传到内核，属于Ring 3
*     1.把对应的功能编号存入eax寄存器,
*     2.使用 SYSENTER 指令(早期使用触发 Int 2e 中断的方式)引发"自陷"
*     3.KiSystemService 内核处理程序运行
*     4.在SDT里找到功能编号的对应函数位置，并调用
*       SDT在系统中一共有4个()：
*       a.SSDT -- 位于 ntoskrnl.exe 或 ntkrnlpa.exe 中，记录了系统关键功能的位置,涉及文件操作,注册表操作,进程操作等。TODO：为什么有两个？区别？
*       b.Shadow SSDT -- 位于 win32k.sys 中， 记录了和用户与图形界面有关的功能。
*       c.d -- 预留的SDT，即有人工创造SDT的可能，不过基本上没有意义
*     5.Object Hook -- 更底层的接口，不过已经很难控制了
*
* ntkrnlpa.exe (SSDT?) -- 
* win32k.sys (Shadow SSDT?) -- 
*
* NtGdiBitBlt
******************************************************************************************************************/

/******************************************************************************************************************
* SSDT(System Services Descriptor Table) -- 系统服务描述符表，把ring3的Win32 API和ring0的内核API联系起来？
*   通过修改此表的函数地址可以对常用windows函数及API进行hook，从而实现对一些关心的系统动作进行过滤、监控的目的
*   TODO: 怎么得到一个函数的SSDT索引号
*   系统内部有个名为 KeServiceDescriptorTable 的变量，保存了相关信息，可通过 extern 后访问
*     如 WinDbg 中: dd KeServiceDescriptorTable
******************************************************************************************************************/

#if 0
//SSDT表结构
typedef struct _ServiceDescriptorTable {
	PULONG ServiceTableBase;			//System Service Dispatch Table 的基地址  
	PULONG ServiceCounterTableBase; 
	//包含着 SSDT 中每个服务被调用次数的计数器。这个计数器一般由sysenter 更新
	ULONG NumberOfServices;				//由 ServiceTableBase 描述的服务的数目
	PUCHAR ParamTableBase;				//包含每个系统服务参数字节数表的基地址-系统服务参数表  
}*PServiceDescriptorTable;
//由SSDT索引号获取当前函数地址
//NtOpenProcess  [[KeServiceDescriptorTable]+0x7A*4]
extern "C" PServiceDescriptorTable KeServiceDescriptorTable;

#endif //F_DRIVER_HOOK_API_H

