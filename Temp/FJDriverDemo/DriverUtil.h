#ifndef DRIVER_UTIL_H
#define DRIVER_UTIL_H

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
* 内核对象
*   DRIVER_OBJECT(驱动对象) -- 需要填写一组回调函数来让Windows调用，插件模式
*     快速IO分发函数 -- FAST_IO_DISPATCH
*     普通分发函数   -- DRIVER_DISPATCH
*   DEVICE_OBJECT(设备对象) -- 可以接受请求(IRP)
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
//# define NT_VERIFY(x)   \
//    status = (x);\
//    if(STATUS_SUCCESS != status)\
//    {\
//        REPORT_ERROR_INFO(status, x);\
//    }
//
//#else
//
//# define NT_VERIFY(x)   \
//    nRet = (x);\
//
//#endif 


#endif //DRIVER_UTIL_H