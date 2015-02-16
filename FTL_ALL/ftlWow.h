#ifndef FTL_WOW_H
#define FTL_WOW_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlWow.h requires ftlbase.h to be included first
#endif

namespace FTL
{
    /*****************************************************************************************************
    * __int64 i;   // 编译器在处理1<<31的时候是按照32位整型来处理
    * 错误写法
    *   i = 1 << 31;            // i == 0xffffffff80000000
    *   i = 1 << 32;            // i == 0x0000000000000000
    * 正确写法
    *   i = __int64(1) << 31;   // i == 0x0000000080000000
    *   i = __int64(1) << 32;   // i == 0x0000000100000000
    *
    *****************************************************************************************************/

    /*****************************************************************************************************
    * 兼容X86指令的64( _M_X64 )
    *   EM64T(Extended Memory 64 Technology) -- Intel的
    *   AMD64(Advanced Micro Devices 64),目前实现中只用了64位的48位来表示地址空间(第128T的UserMode + 高128T的KernelModel)
    * IA64(_M_IA64) -- 不兼容X86,真正的64位CPU，指令集有很大变化,微软有专门的OS对应版本(不常见)
    *   采用软件模拟方式兼容IA32和X86指令，效率不高(其上也能运行32位程序？)
    *   
    *****************************************************************************************************/

    /*****************************************************************************************************
    * 编译选项
    *   1.C++ -> General -> Detect 64-bit Portability Issues(/Wp64) -- 可以帮助发现大多数的64位问题
    *   2.如果设置 IMAGE_FILE_LARGE_ADDRESS_AWARE标志 (/LARGEADDRESSAWARE)，则32位应用程序在Wow64上
    *     可以使用全部的4G地址空间(否则默认只能使用2G -- 另2G是系统使用)
    *****************************************************************************************************/

	/*****************************************************************************************************
	* 开始进行64位 Windows系统编程之前需要了解的信息 -- http://www.microsoft.com/china/MSDN/library/Windev/64bit/issuesx64.mspx?mfr=true
    * WOW/WOW64 -- Windows On Windows /64-bit 
    *   需要尽可能迅速而方便地将现有的 32 位应用程序移植到 64 位环境中去，当时在许多源代码中，
    *   指针被视为32位值。如果简单地重新编写应用程序，就会造成指针被截断的错误和不正确的内存访问。
    *   默认情况下，由系统确保不对 0x000000007FFFFFFF 以上的内存地址进行分配（若要使6 4位应用程序能够访问
    *   它的全部 4TB 用户方式分区，该应用程序必须使用 /LARGEADDRESSAWARE 链接开关来创建），
    *   当系统创建进程的 64 位地址空间时，要检查一个可执行文件的 LARGEADDRESSAWARE 标志，但如果是DLL，那么系统将忽略该标志。
    * 
    * 定义变量时，使用 UINT_PTR、DWORD_PTR 等自动支持 32/64 的变量(WPARAM,LPARAM,LRESULT,各种HANDLE本质都是指针)
	*
	* 64位程序使用注册表时，可以使用 KEY_WOW64_32KEY 参数指定访问32位的注册表
	*   CRegKey reg;
	*   #if _WIN64
	*   if ( ERROR_SUCCESS == reg.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\Microsoft\VisualStudio"),	KEY_READ | KEY_WOW64_32KEY) )
	*   #else
	*   if ( ERROR_SUCCESS == reg.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\Microsoft\VisualStudio"),	KEY_READ) )
	*   #endif
	*
	* 64位OS上注册DLL时(TODO:运行 .js 等时，也需要使用 %systemroot%\SysWoW64\wscript.exe 等)：
	*   32-bit DLL -- %systemroot%\SysWoW64\regsvr32.exe
	*   64-bit DLL -- %systemroot%\System32\regsvr32.exe
    *
    * 注册表编辑器(regedit)
    *   32-bit版本 -- %windir%\SysWOW64\regedit.exe
    *   64-bit版本 -- %windir%\regedit.exe
    *
    * 64位Task Explorer查看32位程序，可以几个特殊的DLL
    *   wow64cpu -- 管理wow里每个正在运行的线程的32位CPU上下文，
    *               并且控制CPU在32位模式和64位模式之间的切换(wow64cpu!X86SwitchTo64BitMode)
    *   wow64.dll -- 管理进程和线程的创建，截获异常分发处理(Exception Dispatching)和系统调用(ntolkrnl.exe所暴露出来的系统调用).
    *                实现了文件系统重定向，注册表重定向，和注册表反射。
    *   wow64win -- 截获GUI系统调用，这些调用是Win32k.sys暴露出来的
    *   ntdll.dll(32位) -- 32位进程调用的接口，其内部通过 wow64cpu.dll+wow64.dll+wow64win.dll 调用64位的ntdll.dll
    *   ntdll.dll(64位) -- 64位系统的调用接口
	*****************************************************************************************************/

    /*****************************************************************************************************
    * 重定向
    *   文件系统重定向(File System Redirector)
    *     64位系统中32位库在C:Windows\SysWow64，32位程序试图从System32中加载库时会被WOW64重定向。
    *     例外：%windir%\system32\ 下的 catroot, drivers\etc, logfiles, spool 等
    *   注册表重定向(Registry Redirector)
    *     WoW64重定向几个注册表路径，如：HKLM\Software 到 HKLM\Software\Wow6432Node，
    *     RegXxxEx 的函数中可以使用 KEY_WOW64_64KEY(访问64位注册表) /KEY_WOW64_32KEY(访问32位注册表) /
    *       KEY_WOW64_RES(从定义来看是同时 32KEY 和 64KEY，具体意义?) 参数
    *     例外：HKLM\Software 下的 Classes、Microsoft\EventSystem、Microsoft\Ole 和 HKEY_USERS\*\Software\Classes 等
    *            HKLM\Software\Classes\CLSID -- 特例中的特例(注册的32位COM是local Server，则会映射到64；如是In-Proc则不映射)
    *   注册表反射(Registry Reflection)
    *     用于处理注册表重定向时的例外情况(如程序在 HKLM\Software\Wow6432Node\Classes 下新建值会立刻复制到 HKLM\Software\Classes  下 )
    * 
    * Wow64DisableWow64FsRedirection-- 禁止文件系统重定向
    * Wow64EnableWow64FsRedirection -- 允许文件系统重定向
    * Wow64RevertWow64FsRedirection -- 恢复文件系统重定向
    * RegEnableReflectionKey  -- 打开注册表反射功能
    * RegDisableReflectionKey -- 禁用注册表反射功能
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