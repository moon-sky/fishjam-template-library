// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "ComicHelper.h"
extern HMODULE g_hModule;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
        {
            TCHAR szModuleName[MAX_PATH] = {0};
            GetModuleFileName(NULL, szModuleName, _countof(szModuleName));
            if (StrStr(szModuleName, TEXT("Thunder")) != NULL
                || StrStr(szModuleName, TEXT("HostProcess.exe")) != NULL)
            {
                ATLTRACE(TEXT(">>> Will Skip Hook Thunder\n"));
                return FALSE;
            }
        }
		g_hModule = hModule;
		break;
	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		break;

	case DLL_PROCESS_DETACH:
		UnHookApi();
		break;
	}
	return TRUE;
}

