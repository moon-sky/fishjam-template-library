// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

extern "C" BOOL WINAPI DllEntryPoint(HINSTANCE, ULONG, LPVOID);

BOOL APIENTRY DllMain(HANDLE hModule, 
					  DWORD  dwReason, 
					  LPVOID lpReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
		{
			//DbgSetModuleLevel(LOG_TRACE|LOG_MEMORY , 5); 
			FUNCTION_BLOCK_INIT();
		}
        break;
    case DLL_PROCESS_DETACH:
		{
			FUNCTION_BLOCK_UNINIT();
		}
        break;
    }
	return DllEntryPoint((HINSTANCE)(hModule), dwReason, lpReserved);
}
