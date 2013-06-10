// dllmain.cpp : Implementation of DllMain.

#include "stdafx.h"
#include "resource.h"
#include "FanXianIEPlugin_i.h"
#include "dllmain.h"
#include "dlldatax.h"

CFanXianIEPluginModule _AtlModule;
//CComModule _Module;

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
#ifdef _MERGE_PROXYSTUB
	if (!PrxDllMain(hInstance, dwReason, lpReserved))
		return FALSE;
#endif
	hInstance;

	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		{
			//过滤资源管理器(explorer.exe), 只处理IE(iexplore.exe)
			if (CFSystemUtil::IsSpecialProcessName(TEXT("explorer.exe"), NULL))
			{
				FTLTRACE(TEXT("IsSpecialProcessName return for explorer.exe\n"));
				return FALSE;
			}
			_AtlModule.m_hModuleInstance = hInstance;
		}
		break;
	case DLL_PROCESS_DETACH:
		break;
	}

	return _AtlModule.DllMain(dwReason, lpReserved); 
	//return TRUE; //_Module.DllMain(hInstance, dwReason, lpReserved)
}
