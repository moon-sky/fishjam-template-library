#include "stdafx.h"
#include "HookApi.h"

HOOK_API_INFO g_HookApiInfo;

HOOK_API_INFO::HOOK_API_INFO()
{
    bHooked = FALSE;
    HookedAPICallCount = 0;
    hCreateDesktopDC = NULL;

    hDCMemory = NULL;
    hBmpBackground = NULL;
    hOldBitmap = NULL;
    pBackgroundBuffer = NULL;
    nImageWidth = 0;
    nImageHeight = 0;
    hProtectProcessInCurProcess = NULL;

    ZeroMemory(HookApiInfos, sizeof(HookApiInfos));
}


BOOL HookApiFromModule(HMODULE hModule, LPCSTR lpProcName, PVOID pDetour, PINLINE_HOOK_INFO* ppOutHookInfo)
{
    BOOL bRet = FALSE;
    FTLASSERT(ppOutHookInfo &&  NULL == *ppOutHookInfo);

    TCHAR szProcessName[MAX_PATH] = {0};
    GetModuleFileName(NULL, szProcessName, _countof(szProcessName));
    PVOID pTarget = (PVOID)GetProcAddress(hModule, lpProcName);
    
    FTLASSERT(pTarget);

    if (pTarget)
    {
        API_VERIFY(CreateInlineHook(pTarget, pDetour, NULL, ppOutHookInfo));
    }
    return bRet;
}
