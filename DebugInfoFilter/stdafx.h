// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#  pragma warning(disable: 4995)  //'%s': name was marked as #pragma deprecated
#  pragma warning(disable: 4996)  //'%s': This function or variable may be unsafe.
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
#define INCLUDE_DETECT_INTEL_MEDIA_SDK	0
#define INCLUDE_DETECT_MEDIAOBJ	1
#define INCLUDE_DETECT_MEDIA_FOUNDATION	1
//#define INCLUDE_DETECT_KSPROXY	1
//#define	INCLUDE_DETECT_QEDIT	1
//#define INCLUDE_DETECT_SHLOBJ	1
#define INCLUDE_DETECT_STRMIF	1
#define INCLUDE_DETECT_VIDEOACC	1
#define	INCLUDE_DETECT_WM_CODEC_DSP	1
#define INCLUDE_DETECT_WMF		1

#include <ftlbase.h>
#include <ftlcom.h>
#include <ftlComDetect.h>
#include <ftlDirectx.h>
#include <ftlControls.h>

using namespace FTL;