#pragma once

// Insert your headers here
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 从 Windows 标头中排除不常使用的资料
#endif

// 如果您必须使用下列所指定的平台之前的平台，则修改下面的定义。
// 有关不同平台的相应值的最新信息，请参考 MSDN。

#ifndef WINVER
#define WINVER 0x0501
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif						

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0501 
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0600
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将是显式的

#  pragma warning(disable: 4995)  //'%s': name was marked as #pragma deprecated
#  pragma warning(disable: 4996)  //'%s': This function or variable may be unsafe.

#include <Windows.h>
#include <ShellAPI.h>

//使用ATL
#include <atlbase.h>
#include <atlstr.h>
#include <atltypes.h>

//使用WTL
#include <atlapp.h>
#include <atldlgs.h>
#include <atlcrack.h>
#include <atlddx.h>
#include <atlgdi.h>
#include <atlctrls.h>
#include <atlctrlw.h>
#include <atlctrlx.h>

#include <streams.h>

//使用FTL
//#define INCLUDE_DETECT_DDRAW	1
#define INCLUDE_DETECT_INTEL_MEDIA_SDK	1
#define INCLUDE_DETECT_MEDIAOBJ	1
#define INCLUDE_DETECT_MEDIA_FOUNDATION	1
//#define INCLUDE_DETECT_KSPROXY	1
//#define	INCLUDE_DETECT_QEDIT	1
//#define INCLUDE_DETECT_SHLOBJ	1
#define INCLUDE_DETECT_STRMIF	1
#define	INCLUDE_DETECT_WM_CODEC_DSP	1
#define INCLUDE_DETECT_WMF		1

#include <ftlbase.h>
#include <ftlcom.h>
#include <ftlComDetect.h>
#include <ftlDirectx.h>
#include <ftlControls.h>

//#include <ftlhardware.h>
using namespace FTL;

