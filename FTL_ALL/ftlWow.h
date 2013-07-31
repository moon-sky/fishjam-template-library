#ifndef FTL_WOW_H
#define FTL_WOW_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlWow.h requires ftlbase.h to be included first
#endif

namespace FTL
{
	/*****************************************************************************************************
	* 开始进行64位 Windows系统编程之前需要了解的信息 -- http://www.microsoft.com/china/MSDN/library/Windev/64bit/issuesx64.mspx?mfr=true
    * WOW/WOW64 -- Windows On Windows /64-bit 
    *   64位编码 -- 分为 AMD64, Intel的EM64T, IA64， 有什么区别?
    *   需要尽可能迅速而方便地将现有的 32 位应用程序移植到 64 位环境中去，当时在许多源代码中，
    *   指针被视为32位值。如果简单地重新编写应用程序，就会造成指针被截断的错误和不正确的内存访问。
    *   默认情况下，由系统确保不对 0x000000007FFFFFFF 以上的内存地址进行分配（若要使6 4位应用程序能够访问
    *   它的全部 4TB 用户方式分区，该应用程序必须使用 /LARGEADDRESSAWARE 链接开关来创建），
    *   当系统创建进程的 64 位地址空间时，要检查一个可执行文件的 LARGEADDRESSAWARE 标志，但如果是DLL，那么系统将忽略该标志。
    * 
    * 定义变量时，使用 UINT_PTR、DWORD_PTR 等自动支持 32/64 的变量
	*
	* 64位程序使用注册表时，需要使用 KEY_WOW64_32KEY 参数?
	*   CRegKey reg;
	*   #if _WIN64
	*   if ( ERROR_SUCCESS == reg.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\Microsoft\VisualStudio"),	KEY_READ | KEY_WOW64_32KEY) )
	*   #else
	*   if ( ERROR_SUCCESS == reg.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\Microsoft\VisualStudio"),	KEY_READ) )
	*   #endif
	*
	* 64位OS上注册DLL时(是不是反了?)：
	*   32-bit DLL -- syswow64\regsvr32.exe
	*   64-bit DLL -- system32\regsvr32.exe
    *
    * 文件  ：64位系统中32位库在C:Windows\SysWow64，32位程序试图从System32中加载库时会被WOW64重定向。
    * 注册表：WoW64重定向HKLM\Software到HKLM\Software\Wow6432Node
	*****************************************************************************************************/
    class CFWow
    {
    public:
        FTLINLINE CFWow(void);
        FTLINLINE virtual ~CFWow(void);
    };

}

#endif //FTL_WOW_H

#ifndef USE_EXPORT
#  include "ftlWow.hpp"
#endif