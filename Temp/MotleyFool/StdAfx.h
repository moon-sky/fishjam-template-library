// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__47022C0C_6BB7_44FF_B8E6_3C8ADDC7E3FC__INCLUDED_)
#define AFX_STDAFX_H__47022C0C_6BB7_44FF_B8E6_3C8ADDC7E3FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#define _WIN32_IE 0x0501

#define _ATL_APARTMENT_THREADED

#define _WTL_NO_CSTRING
#define _WTL_NO_WTYPES
#define _WTL_NO_UNION_CLASSES

#include <atlbase.h>
#include <atlstr.h>
#include <atltypes.h>
#include <atlapp.h>

//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>
#include <atlwin.h>

#include <atlcrack.h>
#include <atlctrls.h>
#include <atlctrlw.h>
#include <atlctrlx.h>
#include <atlddx.h>

//
// These are needed for IDeskBand
//

#include <shlguid.h>
#include <shlobj.h>

// needed for IInputObject and _bstr_t
#include <comdef.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#define USE_MESSAGEBOX_ERROR 1
#define INCLUDE_DETECT_SHLOBJ 1

#include <ftlbase.h>
#include <ftlCom.h>
#include <ftlComDetect.h>
#include <ftlWindow.h>
#include <ftlDebug.h>
#include <ftlFile.h>
#include <ftlIEHelper.h>
#include <ftlWebInterfaceDetect.h>
using namespace FTL;

#if defined _M_IX86
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#endif // !defined(AFX_STDAFX_H__47022C0C_6BB7_44FF_B8E6_3C8ADDC7E3FC__INCLUDED)
