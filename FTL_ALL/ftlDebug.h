#ifndef FTL_DEBUG_H
#define FTL_DEBUG_H

#pragma once

namespace FTL
{
	/*********************************************************************************************************
	* Windbg
	* 命令
	*   dds 地址 -- 可显示指定位置的函数名?
	*   lmm -- list module match
	*   u 地址 -- 反汇编
	*********************************************************************************************************/

    /*********************************************************************************************************
    * 在Release版本中设置断点: _asm int 3
	*   
	* 使用 TRACE/ATLTRACE 打印带有中日韩的文字时，可能会报" _CrtDbgReport: String too long or IO Error "的错误，而无法打出日志：
	*   原因: wprintf_s 不能正确输出中日韩的 UNICODE 文字(似乎VS2010后修复了这个Bug？)
	*   补充资料(尚未仔细研究)：http://www.cnblogs.com/hnrainll/archive/2011/05/07/2039700.html
	*   解决方法：
	*     1.使用FTLTRACE(内部使用 StringCchVPrintfEx + OutputDebugString )
	*     2.#include <locale.h>
	*       char* old_locale = _strdup( setlocale(LC_ALL,NULL) );
	*       setlocale( LC_ALL, "chs" );    或 "", "korean"
	*       TRACE(XXXX);
	*       setlocale( LC_ALL, old_locale); 
	*       free(old_locale); 
    *********************************************************************************************************/

    //printf 的 Ctrl Code -- printf("\033[30m" "其他控制符" "\033[0m" , 参数列表)
    #define CTRL_COLOR_RESET        TEXT("\033[0m")

    #define CTRL_COLOR_BOLD         TEXT("\033[1m")
    #define CTRL_COLOR_GREY         TEXT("\033[5m")
    #define CTRL_COLOR_UNDERLINE    TEXT("\033[4m")
    #define CTRL_COLOR_BLINK        TEXT("\033[5m")
    #define CTRL_COLOR_BLACKBLOCK   TEXT("\033[7m")
    #define CTRL_COLOR_DELETELINE   TEXT("\033[9m")

    #define CTRL_COLOR_BLACK        TEXT("\033[30m")
    #define CTRL_COLOR_RED          TEXT("\033[31m")
    #define CTRL_COLOR_GREEN        TEXT("\033[32m")
    #define CTRL_COLOR_YELLOW       TEXT("\033[33m")
    #define CTRL_COLOR_BLUE         TEXT("\033[34m")
    #define CTRL_COLOR_MAGENTA      TEXT("\033[35m")
    #define CTRL_COLOR_CYAN         TEXT("\033[36m")


	FTLEXPORT class CFRectDumpInfo : public CFConvertInfoT<CFRectDumpInfo, const RECT&, 64>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFRectDumpInfo);
	public:
		FTLINLINE explicit CFRectDumpInfo(const RECT& rect);
		FTLINLINE virtual LPCTSTR ConvertInfo();
	};

	FTLEXPORT class CFFileTimeDumpInfo : public CFConvertInfoT<CFFileTimeDumpInfo, const FILETIME&, 64>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFFileTimeDumpInfo);
	public:
		FTLINLINE explicit CFFileTimeDumpInfo(const FILETIME& fileTime);
		FTLINLINE virtual LPCTSTR ConvertInfo();
	};

	//双显示器时: dwFlags = 01(MONITORINFOF_PRIMARY) 表主显
	//  rcMonitor 显示器的显示区域
	//  rcWork    排除任务栏的区域
	//程序在左边的主显(下方有任务栏)：cbSize= 40,dwFlags=0x1, rcMonitor=(0,0)-(1440,900), 1440x900, rcWork=(0,0)-(1440,869), 1440x869
	//程序在右边的次显：              cbSize= 40,dwFlags=0x0, rcMonitor=(1440,-124)-(2720,900), 1280x1024, rcWork=(1440,-124)-(2720,900), 1280x1024

	FTLEXPORT class CFMonitorInfoDumpInfo: public CFConvertInfoT<CFMonitorInfoDumpInfo, const MONITORINFO &>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFMonitorInfoDumpInfo);
	public:
		FTLINLINE explicit CFMonitorInfoDumpInfo(const MONITORINFO& monitorInfo);
		FTLINLINE virtual LPCTSTR ConvertInfo();
	};
}

#endif //FTL_DEBUG_H

#ifndef USE_EXPORT
#  include "ftlDebug.hpp"
#endif
