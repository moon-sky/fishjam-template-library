// ComicHelper.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ComicHelper.h"
#include <atlbase.h>
#include "ftlbase.h"

#define USE_INLINE_HOOK

#ifdef USE_INLINE_HOOK
#  include "InlineHook.h"
#else
#  include "detours.h"
#  pragma comment(lib, "detours.lib")
#endif 

#include "ProtectWndHookAPI.h"

HMODULE g_hModule;
HHOOK g_hHookCallWndProc = NULL;
HHOOK g_hHookKeyboard = NULL;
BOOL  g_bHooked = FALSE;

CProtectWndHookAPI g_ProtectWndHookApi;

LRESULT CALLBACK My_CallWndProc(
                                _In_  int nCode,
                                _In_  WPARAM wParam,
                                _In_  LPARAM lParam
                                )
{
    CWPSTRUCT * pWPStruct = (CWPSTRUCT*)lParam;
    if (!g_bHooked && /*!g_bIsSelfProcess &&*/ pWPStruct)
    {
        //g_bIsSelfProcess = (g_ProtectWndInfo.curProcessId == GetCurrentProcessId());

#ifdef _DEBUG
        TCHAR szModuleName[MAX_PATH] = {0};
        GetModuleFileName(NULL, szModuleName, _countof(szModuleName));
        FTLTRACE(TEXT("Will Hook API in PID=%d(%s),TID=%d\n"), 
            GetCurrentProcessId(), PathFindFileName(szModuleName), GetCurrentThreadId());
#endif 
        BOOL bRet = FALSE;
        API_VERIFY(HookApi());
    }
    return CallNextHookEx(g_hHookCallWndProc, nCode, wParam, lParam);
}

LRESULT CALLBACK My_LowLevelKeyboardProc(int nCode,
                                         WPARAM wParam,
                                         LPARAM lParam
                                         )
{
    LRESULT nResult = 1;
    KBDLLHOOKSTRUCT* pKbdLlHookStruct = (KBDLLHOOKSTRUCT*)lParam;
    //FTLTRACE(TEXT("LowLevelKeyboardProc, vkCode=0x%x, ScanCode=0x%x, flags=0x%x, dwExtraInfo=0x%p\n"), 
    //    pKbdLlHookStruct->vkCode, pKbdLlHookStruct->scanCode, pKbdLlHookStruct->flags, pKbdLlHookStruct->dwExtraInfo);
    if (0x37 == pKbdLlHookStruct->scanCode     //PrtSc(55)
        || 0x54 == pKbdLlHookStruct->scanCode) //Alt+PrtSc(84)
    {
        //Skip Call Next Hook

        TCHAR szModuleName[MAX_PATH] = {0};
        GetModuleFileName(NULL, szModuleName, _countof(szModuleName));
        ATLTRACE(TEXT("LowLevelKeyboardProc In PID=%d(%s),TID=%d\n"), 
            GetCurrentProcessId(), PathFindFileName(szModuleName), 
            GetCurrentThreadId());
    }
    else
    {
        nResult = CallNextHookEx(g_hHookKeyboard,nCode,wParam,lParam);
    }
    return nResult;
}

COMICHELPER_API BOOL EnableWindowProtected(DWORD curProcessId, HWND hWndFilter, COLORREF clrDisabled)
{
    ATLTRACE(TEXT("Enter EnableWindowProtected for hWndFilter=0x%x\n"), hWndFilter);
    BOOL bRet = TRUE;
    if (NULL == g_hHookCallWndProc)
    {
        API_VERIFY((g_hHookCallWndProc = SetWindowsHookEx(WH_CALLWNDPROC, My_CallWndProc, g_hModule, 0)) != NULL);
        API_VERIFY((g_hHookKeyboard = SetWindowsHookEx(WH_KEYBOARD_LL, My_LowLevelKeyboardProc, g_hModule, 0)) != NULL);
    }

    ATLTRACE(TEXT("Leave EnableWindowProtected g_hHook=0x%x, g_hHookKeyboard=0x%x, bRet=%d\n"), g_hHookCallWndProc, g_hHookKeyboard, bRet);
    return bRet;
}

COMICHELPER_API BOOL DisableWindowProtected(HWND hWnd)
{
    BOOL bRet = TRUE;
    if (g_hHookCallWndProc)
    {
        API_VERIFY(UnHookApi());
        API_VERIFY(UnhookWindowsHookEx(g_hHookCallWndProc));
        API_VERIFY(UnhookWindowsHookEx(g_hHookKeyboard));
        g_hHookCallWndProc = NULL;
        g_hHookKeyboard = NULL;
    }
    return bRet;
}

COMICHELPER_API BOOL HookApi()
{
    BOOL bRet = FALSE;
    g_bHooked = TRUE;
    API_VERIFY(g_ProtectWndHookApi.StartHook());
    return bRet;
}

COMICHELPER_API BOOL UnHookApi()
{
    BOOL bRet = FALSE;
    API_VERIFY(g_ProtectWndHookApi.StopHook());
    g_bHooked = FALSE;
    return bRet;
}

