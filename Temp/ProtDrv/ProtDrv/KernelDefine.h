#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <ntddk.h>
#include <windef.h>

#define SSDT_API_CALL_ENTER(x) (InterlockedIncrement(&x))
#define SSDT_API_CALL_LEAVE(x) (InterlockedDecrement(&x))


#pragma push(0)

//SSDT表结构
typedef struct _SYSTEM_SERVICE_TABLE { 
    // PNTPROC ServiceTable;
    PVOID* ServiceTableBase;					//SSDT在内存中的基地址(数组，其每一个的元素就是各个Rin0的函数)
    PULONG CounterTable;					// array of usage counters, 包含着 SSDT 中每个服务被调用次数计数器的数组。这个计数器一般由sysenter 更新
    //ULONG NumberOfServices;				    //SSDT项的个数
    SIZE_T NumberOfServices;				    //SSDT项的个数
    PBYTE ArgumentsTable;					//TODO:实际上是参数个数(ParamTableBase)? 包含每个系统服务参数字节数表的基地址-系统服务参数表
} SYSTEM_SERVICE_TABLE, *PSYSTEM_SERVICE_TABLE;

//系统中的4个SSDT表
typedef struct _SERVICE_DESCRIPTOR_TABLE
{
    SYSTEM_SERVICE_TABLE ntoskrnl;  // ntoskrnl.exe(native api，即 SSDT, KeServiceDescriptorTable )
    SYSTEM_SERVICE_TABLE win32k;    // win32k.sys (gdi/user support, shadow SSDT)
    SYSTEM_SERVICE_TABLE Table3;    // not used
    SYSTEM_SERVICE_TABLE Table4;    // not used
}SYSTEM_DESCRIPTOR_TABLE, *PSYSTEM_DESCRIPTOR_TABLE;


#ifdef __cplusplus
}
#endif
