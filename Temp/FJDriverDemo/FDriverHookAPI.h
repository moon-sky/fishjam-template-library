#ifndef F_DRIVER_HOOK_API_H
#define F_DRIVER_HOOK_API_H

/******************************************************************************************************************
* 驱动级的 HOOK API( 通过 SSDP 来Hook) -- 注意先看该作者的其他下载。http://download.csdn.net/detail/jyw1111/5275135
*   缺点：可能被反病毒软件作为 rootkit 提醒
*
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

