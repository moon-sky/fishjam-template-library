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
#endif 

//static LONG dwSlept = 0;
static DWORD (WINAPI * TrueSleepEx)(DWORD dwMilliseconds, BOOL bAlertable) = SleepEx;

#pragma data_seg("MyShare")
HWND        g_hFilterWnd = NULL;
DWORD       g_curProcessId = 0;
COLORREF    g_clrDisabled = RGB(127, 127, 127);
#pragma data_seg()
#pragma comment(linker,"/SECTION:MyShare,RWS")

HMODULE g_hModule = NULL;
HHOOK g_hHook = NULL;
HHOOK g_hHookKeyboard = NULL;
BOOL  g_bHooked = FALSE;
BOOL  g_bIsSelfProcess = FALSE;

#ifdef USE_INLINE_HOOK
    PINLINE_HOOK_INFO    g_pHookBitBlt;
    PINLINE_HOOK_INFO    g_pHookStretchBlt;

#else
#endif

typedef BOOL (WINAPI *BitBltProc)(HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop);
typedef BOOL (WINAPI *StretchBltProc)(HDC hdc, int x, int y, int cx, int cy,
                                  HDC hdcSrc, int x1, int y1, int nWidthSrc, int nHeightSrc, 
                                  DWORD rop);

static BOOL (WINAPI* TrueBitBlt)(HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop) = BitBlt;
static BOOL (WINAPI* TrueStretchBlt)(HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1,int nWidthSrc, int nHeightSrc, DWORD rop) = StretchBlt;

static HANDLE (WINAPI* TrueSetClipboardData)(UINT uFormat, HANDLE hMem) = SetClipboardData;

UINT UM_HELPER_HOOK = RegisterWindowMessage(TEXT("UM_HELPER_HOOK"));


BOOL WINAPI Hooked_BitBlt(HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop)
{
    //ATLTRACE(TEXT("Enter Hooked_BitBlt\n"));
    if (cx >= 1024 && cy >= 1024)
    {
        ATLTRACE(TEXT("!!!In Hooked_BitBlt, cx = %d, cy=%d\n"), cx, cy);
    }
#ifdef USE_INLINE_HOOK
    BitBltProc pOrigBitBlt = (BitBltProc)g_pHookBitBlt->pOriginal;
    BOOL bRet = (pOrigBitBlt)(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop);
    //ATLTRACE(TEXT("After Hooked_BitBlt\n"));
#else 
    BOOL bRet = TrueBitBlt(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop);
#endif 
    if (bRet && NULL != g_hFilterWnd)
    {
        HWND hWnd = WindowFromDC(hdcSrc);
        if (hWnd != NULL)
        {
            if (hWnd == GetDesktopWindow() || hWnd == g_hFilterWnd || ::IsChild(g_hFilterWnd, hWnd))
            {

                TCHAR szProcessName[MAX_PATH] = {0};
                GetModuleFileName(NULL, szProcessName, _countof(szProcessName));

                FTLTRACE(TEXT("!!! Hooked_BitBlt Desktop PID=%d(%s), hWnd=%d, g_hFilterWnd=%d\n"),
                    GetCurrentProcessId(), PathFindFileName(szProcessName), hWnd, g_hFilterWnd);
#if 0
                //CRect rcSource(x1, y1, x1 + cx, y1 + cy);
                //::SetBkColor(hdc, g_clrDisabled);
                //::ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rcSource, NULL, 0, NULL);
#else

                CRect rcTarget( x, y,  x + cx, y + cy);
                CRect rcSource(x1, y1, x1 + cx, y1 + cy);
                CRect rcFilter(0, 0, 0, 0);

                API_VERIFY(GetWindowRect(g_hFilterWnd, &rcFilter));
                //FTLTRACE(TEXT("rcTarget=(%d,%d)-(%d,%d), %dx%d, rcSource=(%d,%d)-(%d,%d), rcFilter=(%d,%d)-(%d,%d), %dx%d"),
                //    rcTarget.left, rcTarget.top, rcTarget.right, rcTarget.bottom, 
                //    rcTarget.right - rcTarget.left, rcTarget.bottom - rcTarget.top,
                //    rcSource.left, rcSource.top, rcSource.right, rcSource.bottom, 
                //    rcFilter.left, rcFilter.top, rcFilter.right, rcFilter.bottom, 
                //    rcFilter.right - rcFilter.left, rcFilter.bottom - rcFilter.top
                //    );

                rcSource.IntersectRect(&rcSource, &rcFilter);
                rcSource.OffsetRect(x - x1, y - y1);
                if (!rcSource.IsRectEmpty())
                {
                    //FTLTRACE(TEXT("Will FillColor newRcSource=(%d,%d)-(%d,%d), %dx%d"),
                    //    rcSource.left, rcSource.top, rcSource.right, rcSource.bottom, 
                    //    rcSource.right - rcSource.left, rcSource.bottom - rcSource.top);
                    ::SetBkColor(hdc, g_clrDisabled);
                    ::ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rcSource, NULL, 0, NULL);
                }
#endif 
            }
        }
    }
    return bRet;
}

BOOL WINAPI Hooked_StretchBlt(  HDC hdcDest,
                int nXOriginDest,
                int nYOriginDest,
                int nWidthDest,
                int nHeightDest,
                HDC hdcSrc,
                int nXOriginSrc,
                int nYOriginSrc,
                int nWidthSrc,
                int nHeightSrc,
                DWORD dwRop)
{
#ifdef USE_INLINE_HOOK
    StretchBltProc pOrigStretchBlt = (StretchBltProc)g_pHookStretchBlt->pOriginal;
    BOOL bRet = (pOrigStretchBlt)(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest,
        hdcSrc, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, dwRop);
#else 
    BOOL bRet = TrueStretchBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest,
        hdcSrc, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, dwRop);
#endif 
    if (bRet)
    {
        HWND hWnd = WindowFromDC(hdcSrc);
        if (hWnd != NULL)
        {
            if (hWnd == GetDesktopWindow() || hWnd == g_hFilterWnd || ::IsChild(g_hFilterWnd, hWnd))
            {
                FTLTRACE(TEXT("!!! In Hooked_StretchBlt, Desktop\n"));
                CRect rcSource(nXOriginDest, nYOriginDest, nXOriginDest + nWidthDest, nYOriginDest + nHeightDest);
                ::SetBkColor(hdcDest, g_clrDisabled);
                ::ExtTextOut(hdcDest, 0, 0, ETO_OPAQUE, &rcSource, NULL, 0, NULL);
            }
        }
    }
    return bRet;
}

HANDLE WINAPI FilterSetClipboardData(UINT uFormat, HANDLE hMem)
{
    DWORD dwProcessId = GetCurrentProcessId();
    TCHAR szModuleName[MAX_PATH] = {0};
    GetModuleFileName(NULL, szModuleName, _countof(szModuleName));
    ATLTRACE(TEXT("FilterSetClipboardData, PID=%d(%s)\n"), dwProcessId, PathFindFileName(szModuleName));
    return TrueSetClipboardData(uFormat, hMem);
}

LRESULT CALLBACK My_CallWndProc(
                                _In_  int nCode,
                                _In_  WPARAM wParam,
                                _In_  LPARAM lParam
                             )
{
    CWPSTRUCT * pWPStruct = (CWPSTRUCT*)lParam;
    if (!g_bHooked && !g_bIsSelfProcess && pWPStruct)// && pWPStruct->message == UM_HELPER_HOOK)
    {
        g_bIsSelfProcess = (g_curProcessId == GetCurrentProcessId());

        TCHAR szModuleName[MAX_PATH] = {0};
        GetModuleFileName(NULL, szModuleName, _countof(szModuleName));
        ATLTRACE(TEXT("Will Hook API in PID=%d(%s),TID=%d\n"), GetCurrentProcessId(), PathFindFileName(szModuleName), GetCurrentThreadId());

        BOOL bRet = FALSE;
        API_VERIFY(HookApi());
    }
    return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

LRESULT CALLBACK My_LowLevelKeyboardProc(int nCode,
                                      WPARAM wParam,
                                      LPARAM lParam
                                      )
{
    LRESULT nResult = 1;
    KBDLLHOOKSTRUCT* pKbdLlHookStruct = (KBDLLHOOKSTRUCT*)lParam;
    FTLTRACE(TEXT("LowLevelKeyboardProc, vkCode=0x%x, ScanCode=0x%x, flags=0x%x, dwExtraInfo=0x%p\n"), 
        pKbdLlHookStruct->vkCode, pKbdLlHookStruct->scanCode, pKbdLlHookStruct->flags, pKbdLlHookStruct->dwExtraInfo);
    if (0x37 == pKbdLlHookStruct->scanCode     //PrtSc(55)
        || 0x54 == pKbdLlHookStruct->scanCode) //Alt+PrtSc(84)
    {
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
    BOOL bRet = FALSE;
    g_hFilterWnd = hWndFilter;
    g_clrDisabled = clrDisabled;
    g_curProcessId = curProcessId;

    if (NULL == g_hHook)
    {
        //UM_HELPER_HOOK = RegisterWindowMessage(TEXT("UM_HELPER_HOOK"));
        g_hHook = SetWindowsHookEx(WH_CALLWNDPROC, My_CallWndProc, g_hModule, 0);
        g_hHookKeyboard = SetWindowsHookEx(WH_KEYBOARD_LL, My_LowLevelKeyboardProc, g_hModule, 0);
    }

    ATLTRACE(TEXT("Leave EnableWindowProtected g_hHook=0x%x, g_hHookKeyboard=0x%x, bRet=%d\n"), g_hHook, g_hHookKeyboard, bRet);
    return bRet;
}

COMICHELPER_API BOOL DisableWindowProtected(HWND hWnd)
{
    BOOL bRet = TRUE;
    //ATLASSERT(hWnd == g_hFilterWnd);
    if (g_hHook)// && hWnd == g_hFilterWnd
    {
        bRet = UnhookWindowsHookEx(g_hHook);
        UnhookWindowsHookEx(g_hHookKeyboard);
        g_hHook = NULL;
        g_hHookKeyboard = NULL;

        UnHookApi();
        g_hFilterWnd = NULL;
    }
    return bRet;
}

COMICHELPER_API BOOL UpdateProtectWindowInfo(HWND hWndFilter, LPRECT pRect)
{
    BOOL bRet = FALSE;
    g_hFilterWnd = hWndFilter;

    return TRUE;
}

COMICHELPER_API BOOL HookApi()
{
    BOOL bRet = TRUE;
    if ( !g_bHooked)
    {
        g_bHooked = TRUE;

        TCHAR szModuleName[MAX_PATH] = {0};
        GetModuleFileName(NULL, szModuleName, _countof(szModuleName));
        ATLTRACE(TEXT(">>> Will Hook API(g_bHooked=%d) in PID=%d(%s),TID=%d\n"), 
            g_bHooked, GetCurrentProcessId(), PathFindFileName(szModuleName), GetCurrentThreadId());

#ifdef USE_INLINE_HOOK
        API_VERIFY(CreateInlineHook((PVOID*)&TrueBitBlt, &Hooked_BitBlt, NULL, &g_pHookBitBlt));
        API_VERIFY(CreateInlineHook((PVOID*)&TrueStretchBlt, &Hooked_StretchBlt, NULL, &g_pHookStretchBlt));
#else
        DetourRestoreAfterWith();
        API_VERIFY(DetourTransactionBegin() == NO_ERROR);
        API_VERIFY(DetourUpdateThread(GetCurrentThread()) == NO_ERROR);
        API_VERIFY(DetourAttach(&(PVOID&)TrueBitBlt, Hooked_BitBlt) == NO_ERROR);
        API_VERIFY(DetourAttach(&(PVOID&)TrueStretchBlt, Hooked_StretchBlt) == NO_ERROR);
        API_VERIFY(DetourAttach(&(PVOID&)TrueSetClipboardData, FilterSetClipboardData) == NO_ERROR);
        API_VERIFY(DetourTransactionCommit() == NO_ERROR);
#endif 
    }
    return bRet;
}
COMICHELPER_API BOOL UnHookApi()
{
    BOOL bRet = TRUE;

    TCHAR szModuleName[MAX_PATH] = {0};
    GetModuleFileName(NULL, szModuleName, _countof(szModuleName));
    ATLTRACE(TEXT("<<< Will UnHook API(g_bHooked=%d) in PID=%d(%s),TID=%d\n"), 
        g_bHooked, GetCurrentProcessId(), PathFindFileName(szModuleName), GetCurrentThreadId());

    if (g_bHooked)
    {
        g_bHooked = FALSE;

#ifdef USE_INLINE_HOOK
        API_VERIFY(RestoreInlineHook(g_pHookBitBlt));
        API_VERIFY(RestoreInlineHook(g_pHookStretchBlt));
#else 
        API_VERIFY(DetourTransactionBegin()== NO_ERROR);
        API_VERIFY(DetourUpdateThread(GetCurrentThread()) == NO_ERROR);
        API_VERIFY(DetourDetach(&(PVOID&)TrueBitBlt, Hooked_BitBlt) == NO_ERROR);
        API_VERIFY(DetourDetach(&(PVOID&)TrueStretchBlt, Hooked_StretchBlt) == NO_ERROR);
        API_VERIFY(DetourDetach(&(PVOID&)TrueSetClipboardData, FilterSetClipboardData) == NO_ERROR);
        API_VERIFY(DetourTransactionCommit() == NO_ERROR);
#endif 
    }

    return bRet;
}

