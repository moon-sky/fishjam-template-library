#include "stdafx.h"
#include "DebugInfoFilter.h"
#include "DebugInfoFilterProperty.h"
#include "GraphInfoPropertyPage.h"

//关联OLE的入口函数和对象创建方式，由类工厂调用 static CreateInstance
CFactoryTemplate g_Templates[] = 
{
    { 
        L"Debug Info Filter", 
        &CLSID_DebugInfoFilter, 
        CDebugInfoFilter::CreateInstance,
        NULL, 
        &sudDebugInfoFilter 
    },
    { 
        L"Debug Info Filter Property", 
        &CLSID_FilterDebugInfoProperty, 
		CDebugInfoFilterProperty::CreateInstance,
        NULL, 
        NULL 
    },
	{ 
		L"Graph Info Property", 
		&CLSID_GraphInfoProperty, 
		CGraphInfoPropertyPage::CreateInstance,
		NULL, 
		NULL 
	},

};
int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);


//
// DllRegisterServer
//
STDAPI DllRegisterServer()
{ 
	LPCTSTR pszCommandLine = GetCommandLine();
    return AMovieDllRegisterServer2( TRUE );

} // DllRegisterServer


//
// DllUnregisterServer
//
STDAPI DllUnregisterServer()
{
	LPCTSTR pszCommandLine = GetCommandLine();
    return AMovieDllRegisterServer2( FALSE );

} // DllUnregisterServer

//
// DllEntryPoint
//

extern "C" BOOL WINAPI DllEntryPoint(HINSTANCE, ULONG, LPVOID);
BOOL APIENTRY DllMain(HANDLE hModule, 
                      DWORD  dwReason, 
                      LPVOID lpReserved)
{
    return DllEntryPoint((HINSTANCE)(hModule), dwReason, lpReserved);
}
