#ifndef F_DRIVER_UTIL_H
#define F_DRIVER_UTIL_H

/******************************************************************************************************************
* NT_SUCCESS  -- 判断返回值是否成功
* 
* 
*
* PMDL -- 内存描述符链表指针？通常描述一个缓冲区
*
* IRP -- 请求(如主功能号为 IRP_MJ_CREATE 的IRP)
*
* 分发函数Hook技术 -- 
******************************************************************************************************************/
//NT_ERROR宏不等同于!NT_SUCCESS，最好使用!NT_SUCCESS，因为除了错误外，它还截获警告信息
//if(!NT_SUCCESS(status))return status;

#ifdef __cplusplus
extern "C" {
#endif

#include "ntddk.h"
#include "wdm.h"

void DumpDeviceFlags(USHORT nFlags);
#pragma PAGEDCODE 
LPSTR GetNtStatusString(NTSTATUS status);


#ifdef __cplusplus
}
#endif

//#if DBG
# define REPORT_ERROR_INFO(s, x)   \
    DbgPrint( "%s(%d) : ERROR: 0x%x(%s) for call %s\n", __FILE__, __LINE__, s, GetNtStatusString(s), #x);

# define FNT_VERIFY(x)   \
	status = (x);\
	if(STATUS_SUCCESS != status)\
{\
	REPORT_ERROR_INFO(status, x);\
}

//#else
//
//# define FNT_VERIFY(x)   \
//	status = (x);\
//
//#endif 

#endif //F_DRIVER_UTIL_H