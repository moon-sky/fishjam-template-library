// ComicHelper.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ComicHelper.h"
#include <atlbase.h>
#include "ftlbase.h"

//#define USE_INLINE_HOOK

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
BOOL  g_bHooked = FALSE;
BOOL  g_bIsSelfProcess = FALSE;

#ifdef USE_INLINE_HOOK
    PINLINE_HOOK_INFO    g_pHookBitBlt;
#else
#endif

typedef BOOL (WINAPI *BITBLT)(HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop);
static BOOL (WINAPI* TrueBitBlt)(HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop) = BitBlt;
static HANDLE (WINAPI* TrueSetClipboardData)(UINT uFormat, HANDLE hMem) = SetClipboardData;

UINT UM_HELPER_HOOK = RegisterWindowMessage(TEXT("UM_HELPER_HOOK"));


BOOL WINAPI FilterBitBlt(HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop)
{
    //ATLTRACE(TEXT("Enter FilterBitBlt\n"));
#ifdef USE_INLINE_HOOK
    BITBLT pOrigBitBlt = (BITBLT)g_pHookBitBlt->pOriginal;
    BOOL bRet = pOrigBitBlt(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop);
#else 
    BOOL bRet = TrueBitBlt(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop);
#endif 
    if (bRet)
    {
        HWND hWnd = WindowFromDC(hdcSrc);
        if (hWnd != NULL)
        {
            if (hWnd == GetDesktopWindow() || hWnd == g_hFilterWnd || ::IsChild(g_hFilterWnd, hWnd))
            {
                TCHAR szProcessName[MAX_PATH] = {0};
                GetModuleFileName(NULL, szProcessName, _countof(szProcessName));

                FTLTRACE(TEXT("FilterBitBlt, PID=%d(%s), hWnd=%d, g_hFilterWnd=%d\n"),
                    GetCurrentProcessId(), PathFindFileName(szProcessName), hWnd, g_hFilterWnd);

                CRect rcTarget( x, y,  x + cx, y + cy);
                CRect rcSource(x1, y1, x1 + cx, y1 + cy);
                CRect rcFilter(0, 0, 0, 0);

                API_VERIFY(GetWindowRect(g_hFilterWnd, &rcFilter));
                FTLTRACE(TEXT("rcTarget=(%d,%d)-(%d,%d), %dx%d, rcSource=(%d,%d)-(%d,%d), rcFilter=(%d,%d)-(%d,%d), %dx%d"),
                    rcTarget.left, rcTarget.top, rcTarget.right, rcTarget.bottom, 
                    rcTarget.right - rcTarget.left, rcTarget.bottom - rcTarget.top,
                    rcSource.left, rcSource.top, rcSource.right, rcSource.bottom, 
                    rcFilter.left, rcFilter.top, rcFilter.right, rcFilter.bottom, 
                    rcFilter.right - rcFilter.left, rcFilter.bottom - rcFilter.top
                    );

                rcSource.IntersectRect(&rcSource, &rcFilter);
                rcSource.OffsetRect(x - x1, y - y1);
                if (!rcSource.IsRectEmpty())
                {
                    FTLTRACE(TEXT("Will FillColor newRcSource=(%d,%d)-(%d,%d), %dx%d"),
                        rcSource.left, rcSource.top, rcSource.right, rcSource.bottom, 
                        rcSource.right - rcSource.left, rcSource.bottom - rcSource.top);
                    ::SetBkColor(hdc, g_clrDisabled);
                    ::ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rcSource, NULL, 0, NULL);
                }
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
    }

    ATLTRACE(TEXT("Leave EnableWindowProtected g_hHook=0x%x, bRet=%d\n"), g_hHook, bRet);
    return bRet;
}

COMICHELPER_API BOOL DisableWindowProtected(HWND hWnd)
{
    BOOL bRet = TRUE;
    ATLASSERT(hWnd == g_hFilterWnd);
    if (hWnd == g_hFilterWnd && g_hHook)
    {
        bRet = UnhookWindowsHookEx(g_hHook);
        g_hHook = NULL;
        UnHookApi();

        g_hFilterWnd = NULL;
    }
    return bRet;
}

COMICHELPER_API BOOL HookApi()
{
    BOOL bRet = FALSE;
    if ( !g_bHooked)
    {
        g_bHooked = TRUE;

        TCHAR szModuleName[MAX_PATH] = {0};
        GetModuleFileName(NULL, szModuleName, _countof(szModuleName));
        ATLTRACE(TEXT(">>> Will Hook API(g_bHooked=%d) in PID=%d(%s),TID=%d\n"), 
            g_bHooked, GetCurrentProcessId(), PathFindFileName(szModuleName), GetCurrentThreadId());

#ifdef USE_INLINE_HOOK
        API_VERIFY(CreateInlineHook(BitBlt, FilterBitBlt, NULL, &g_pHookBitBlt));
#else
        DetourRestoreAfterWith();
        API_VERIFY(DetourTransactionBegin() == NO_ERROR);
        API_VERIFY(DetourUpdateThread(GetCurrentThread()) == NO_ERROR);
        API_VERIFY(DetourAttach(&(PVOID&)TrueBitBlt, FilterBitBlt) == NO_ERROR);
        API_VERIFY(DetourAttach(&(PVOID&)TrueSetClipboardData, FilterSetClipboardData) == NO_ERROR);
        API_VERIFY(DetourTransactionCommit() == NO_ERROR);
#endif 
    }
    return bRet;
}
COMICHELPER_API BOOL UnHookApi()
{
    BOOL bRet = FALSE;

    TCHAR szModuleName[MAX_PATH] = {0};
    GetModuleFileName(NULL, szModuleName, _countof(szModuleName));
    ATLTRACE(TEXT("<<< Will UnHook API(g_bHooked=%d) in PID=%d(%s),TID=%d\n"), 
        g_bHooked, GetCurrentProcessId(), PathFindFileName(szModuleName), GetCurrentThreadId());

    if (g_bHooked)
    {
        g_bHooked = FALSE;

#ifdef USE_INLINE_HOOK
        API_VERIFY(RestoreInlineHook(g_pHookBitBlt));
#else 
        API_VERIFY(DetourTransactionBegin()== NO_ERROR);
        API_VERIFY(DetourUpdateThread(GetCurrentThread()) == NO_ERROR);
        API_VERIFY(DetourDetach(&(PVOID&)TrueBitBlt, FilterBitBlt) == NO_ERROR);
        API_VERIFY(DetourDetach(&(PVOID&)TrueSetClipboardData, FilterSetClipboardData) == NO_ERROR);
        API_VERIFY(DetourTransactionCommit() == NO_ERROR);
#endif 
    }

    return bRet;
}

