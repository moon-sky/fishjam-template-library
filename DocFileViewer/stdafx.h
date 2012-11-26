// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__FB3A4DAA_B384_4223_AC8B_650FCA7463C4__INCLUDED_)
#define AFX_STDAFX_H__FB3A4DAA_B384_4223_AC8B_650FCA7463C4__INCLUDED_

// Change these values to use different versions
#define WINVER		0x0500
#define _WIN32_WINNT	0x0501
#define _WIN32_IE	0x0600
#define _RICHEDIT_VER	0x0200

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlcom.h>
#include <atlhost.h>
#include <atlwin.h>
#include <atlctl.h>

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlctrlw.h> 

#include <atlcrack.h> 
#include <atlmisc.h> 
#include <atlsplit.h> 
#include <atlctrlx.h> 
#include <atlddx.h>


#include <stack> 
#include <map> 
#include <vector> 

#include "atlcomsafearray.h" 
#include "atlcustom.h" 
#include "errorstr.h" 
#include "strcvt.h" 
#include "timecvt.h" 

//#import "E:\\Projects\\SuperPad\\HexEditAx\\Debug\\HexEditAx.dll" \
//    raw_interfaces_only, raw_native_types, no_auto_exclude, no_smart_pointers, rename_namespace("HexEditor") 
#include "hexeditax.h" 

//#import "C:\\WINDOWS\\system32\\Macromed\\Flash\\Flash9d.ocx" \
//    raw_interfaces_only, raw_native_types, no_auto_exclude, no_smart_pointers, rename_namespace("Flash") 
//#include "Flash9d.h" 

#include "DocFileViewer.h" 
#include "viewhexedit.h" 

LPCTSTR AdjustFileName(LPTSTR lpszName); 
HRESULT WriteStreamToFile(LPCTSTR lpszFileName, IStream *pStrm); 

#define WM_CUSTOM_POSTCREATE    WM_USER + 10 
#define WM_CUSTOM_ACTIVE_ITEM   WM_USER + 11 

static UINT    MRU_MAX_ENTRIES = 16; 
static LPCTSTR APP_SETTINGS_KEY = _T("software\\FreeFlyStudio\\DocFileViewerer"); 


#endif // !defined(AFX_STDAFX_H__FB3A4DAA_B384_4223_AC8B_650FCA7463C4__INCLUDED_)
