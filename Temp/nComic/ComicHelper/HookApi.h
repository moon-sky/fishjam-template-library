#pragma once
#include "InlineHook.h"
#include <ftlThread.h>

enum HookFuncType
{
    hft_BitBlt = 0,
    hft_StretchBlt,

    //hft_PlgBlt,
    //hft_MaskBlt,
    //hft_TransparentBlt,
    //hft_ExtTextOutW,

    hft_CreateDCW,
    hft_CreateDCA,
    hft_DeleteDC,

    hft_OpenProcess,
    hft_TerminateProcess,
    //hft_NtUserPrintWindow,
    //hft_NtGdiDdLock,
    //hft_NtGdiDdUnlock,

    hft_RegOpenKeyA,
    hft_RegOpenKeyW,
    hft_RegSetKeyValueA,
    hft_RegSetKeyValueW,

    hft_CreateFileA,
    hft_CreateFileW,
    hft_MoveFileA,
    hft_MoveFileW,

    hft_FunctionCount
};

struct HOOK_API_INFO
{
    BOOL    bHooked;
    FTL::CFCriticalSection csLock;

    LONG    HookedAPICallCount;
    HDC     hCreateDesktopDC;

    HDC     hDCMemory;          //HDC memory for store background
    HBITMAP hBmpBackground;
    HBITMAP hOldBitmap;
    BYTE*   pBackgroundBuffer;
    int     nImageWidth;
    int     nImageHeight;

    HANDLE  hProtectProcessInCurProcess;
    PINLINE_HOOK_INFO    HookApiInfos[hft_FunctionCount];

    HOOK_API_INFO();
};

extern HMODULE g_hModule;
extern HOOK_API_INFO g_HookApiInfo;

#define HOOKED_API_CALL_ENTER(x) (InterlockedIncrement(&x))
#define HOOKED_API_CALL_LEAVE(x) (InterlockedDecrement(&x))

BOOL HookApiFromModule(HMODULE hModule, LPCSTR lpProcName, PVOID pDetour, PINLINE_HOOK_INFO* ppOutHookInfo);
