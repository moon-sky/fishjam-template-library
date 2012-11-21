// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//  are changed infrequently
//

#pragma once

// Change these values to use different versions
#define WINVER _WIN32_WINNT_WIN7
#define _WIN32_WINNT	0x0501
#define _WIN32_IE	0x0501
#define _RICHEDIT_VER	0x0200

#define _WTL_NO_CSTRING		 // 不使用WTL的CString (使用ATL的)
#define _WTL_NO_WTYPES
#define _WTL_NO_UNION_CLASSES

#include <atlbase.h>
#include <atlstr.h>         // 包含ATL的CString
#include <atltypes.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>

#include <atlframe.h>       // WTL frame window classes
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlctrls.h>
#include <atlctrlw.h>
#include <atlctrlx.h>
#include <atlcrack.h>       // WTL 增强消息映射宏(message cracker),消息映射宏为 MSG_WM_XXX
#include <atlddx.h>

#include <mfapi.h>
#include <mfidl.h>

#include <ftlBase.h>
#include <ftlCom.h>
#include <ftlComDetect.h>

#if defined _M_IX86
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
