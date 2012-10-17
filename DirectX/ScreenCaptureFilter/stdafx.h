// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define INCLUDE_DETECT_CONTROL  1
//#define INCLUDE_DETECT_DDRAW    1
#define INCLUDE_DETECT_DOCOBJ   1
#define INCLUDE_DETECT_KSPROXY  1
#define INCLUDE_DETECT_MEDIAOBJ 1
#define INCLUDE_DETECT_STRMIF   1
#define INCLUDE_DETECT_WMF      1

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#include <atlbase.h>
#include <atlstr.h>

#include <Streams.h>
#include <ftlBase.h>
#include <ftlCom.h>
#include <ftlComDetect.h>
#include <ftlDirectX.h>
#include <ftlThread.h>


//#include <ddraw.h>
//#pragma comment(lib, "ddraw.lib")

// TODO: reference additional headers your program requires here
