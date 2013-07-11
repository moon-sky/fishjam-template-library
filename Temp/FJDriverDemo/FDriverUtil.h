#ifndef F_DRIVER_UTIL_H
#define F_DRIVER_UTIL_H

/******************************************************************************************************************
* NT_SUCCESS  -- 判断返回值是否成功
* 
* 宽字符串(UNICODE_STRING) = RTL_CONSTANT_STRING(L"xxx")
*   字符串操作时，Rtl-函数(如 RtlInitUnicodeString、)
* 
*
* DbgPrint
*   %wZ -- UNICODE_STRING 变量的地址
*
*
* PMDL -- 内存描述符链表指针？通常描述一个缓冲区
*
* IRP -- 请求(如主功能号为 IRP_MJ_CREATE 的IRP)
*
* 分发函数Hook技术 -- 
******************************************************************************************************************/


//#if DBG
//# define REPORT_ERROR_INFO(s, x)   \
//    DbgPrint("ERROR:%d, %s", s, #x);
//
//# define FNT_VERIFY(x)   \
//    status = (x);\
//    if(STATUS_SUCCESS != status)\
//    {\
//        REPORT_ERROR_INFO(status, x);\
//    }
//
//#else
//
//# define FNT_VERIFY(x)   \
//    nRet = (x);\
//
//#endif 


#endif //F_DRIVER_UTIL_H