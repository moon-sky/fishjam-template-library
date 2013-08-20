#pragma once

#ifdef FDRIVER_DEMO_PROJECT
#  include "FDriverDemoDefine.h"
#else
#  include "nComicDrvDefine.h"
#endif

#include "InlineHook.h"

#define SSDT_API_CALL_ENTER(x) (InterlockedIncrement(&x))
#define SSDT_API_CALL_LEAVE(x) (InterlockedDecrement(&x))


//SSDT表结构 -- 32/64 通用， func = (PBYTE)Base+Base[index];
typedef struct _SYSTEM_SERVICE_TABLE { 
	// PNTPROC ServiceTable;
	PVOID* ServiceTableBase;				//SSDT在内存中的基地址(数组，其每一个的元素就是各个Rin0的函数)
	PULONG CounterTable;					// array of usage counters, 包含着 SSDT 中每个服务被调用次数计数器的数组。这个计数器一般由sysenter 更新
	SIZE_T NumberOfServices;			    //SSDT项的个数，不论是32(直接保存地址)还是64(前 3.5 个字节保存地址的偏移)为系统，每项都是4个字节，其个数 * 4 就是整个地址表的大小
	PBYTE ArgumentsTable;					//TODO:实际上是参数个数(ParamTableBase)? 包含每个系统服务参数字节数表的基地址-系统服务参数表
} SYSTEM_SERVICE_TABLE, *PSYSTEM_SERVICE_TABLE;

//系统中的4个SSDT表
typedef struct _SERVICE_DESCRIPTOR_TABLE
{
	SYSTEM_SERVICE_TABLE ntoskrnl;  // ntoskrnl.exe(native api，即 SSDT, KeServiceDescriptorTable )
	SYSTEM_SERVICE_TABLE win32k;    // win32k.sys (gdi/user support, shadow SSDT) -- GUI 进程中才有，CUI 进程中没有？
	SYSTEM_SERVICE_TABLE Table3;    // not used
	SYSTEM_SERVICE_TABLE Table4;    // not used
}SYSTEM_DESCRIPTOR_TABLE, *PSYSTEM_DESCRIPTOR_TABLE;

//#pragma pack(push, 8)
typedef struct _HOOK_API_INFO
{
	//在Hook函数执行过程中pOrig可能被替换,需要先将 pOrigApiAddress 保存成临时变量，判断不为空后使用临时变量
	LPWSTR  pwzApiName;
	PVOID   pTargetAddress;			
	PVOID   pNewApiAddress;
	PVOID	pOrigApiAddress;		//x86是Shadow SSDT中的函数，amd64是原始函数
	LONG	nIndexInSSDT;
	LONG    nParamCount;
	BOOL	bEnableHook;

	//KSPIN_LOCK	spinLock;		//LONG	nAPICallCount;
	//LONG    nPatchSize;
	PINLINE_HOOK_INFO	pInlineHookInfo;
}HOOK_API_INFO, *PHOOK_API_INFO;
//#pragma pack(pop)


#ifdef __cplusplus
extern "C" {
#endif

NTSTATUS InstallCopyProtectHook(HANDLE hTargetProcess);
NTSTATUS UpdateCopyProtectWndInfo(PPROTECT_WND_INFO pProtectWndInfo);
NTSTATUS UnInstallCopyProtectHook(void);

#ifdef __cplusplus
}
#endif
