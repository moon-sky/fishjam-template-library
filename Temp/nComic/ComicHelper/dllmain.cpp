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
            LPCTSTR pszFileName = PathFindFileName(szModuleName);
            if (pszFileName)
            {
                //if (StrStrI(pszFileName, TEXT("NaverCapture.exe")) == NULL
                //    //|| StrStrI(szModuleName, TEXT("NComic.exe")) != NULL
                //    )
                //{
                //    ATLTRACE(TEXT(">>> Will Skip Hook %s\n"), pszFileName);
                //    return FALSE;
                //}
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

