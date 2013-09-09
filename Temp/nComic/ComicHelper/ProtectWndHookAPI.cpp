#include "StdAfx.h"
#include "ProtectWndHookAPI.h"
#include "InlineHook.h"
#include <ftlThread.h>

#define HOOKED_API_CALL_ENTER(x) (InterlockedIncrement(&x))
#define HOOKED_API_CALL_LEAVE(x) (InterlockedDecrement(&x))

ProtectWndInfoFileMap32* g_pProtectWndInfoFileMap = NULL;

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

    //hft_NtUserPrintWindow,
    //hft_NtGdiDdLock,
    //hft_NtGdiDdUnlock,
    hft_FunctionCount
};

struct HOOK_API_INFO
{
    BOOL    bHooked;
    FTL::CFCriticalSection csLock;

    LONG    HookedAPICallCount;
    HDC     hCreateDesktopDC;
    PINLINE_HOOK_INFO    HookApiInfos[hft_FunctionCount];

    HOOK_API_INFO()
    {
        bHooked = FALSE;
        HookedAPICallCount = 0;
        hCreateDesktopDC = NULL;
    }
}g_HookApiInfo;

BOOL HookApiFromModule(HMODULE hModule, LPCSTR lpProcName, PVOID pDetour, PINLINE_HOOK_INFO* ppOutHookInfo);
BOOL IsFilterHDC(HDC hDC);

typedef BOOL (WINAPI *BitBltProc)(HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight,
                                  HDC hdcSrc, int nXSrc, int nYSrc, DWORD dwRop);
typedef BOOL (WINAPI *StretchBltProc)(HDC hdcDest, int nXDest, int nYDest, int nWidthDest, int nHeightDest,
                                      HDC hdcSrc, int nXSrc, int nYSrc, int nWidthSrc, int nHeightSrc, 
                                      DWORD dwRop);

typedef HDC (WINAPI* CreateDCWProc)(LPCWSTR pwszDriver, LPCWSTR pwszDevice, LPCWSTR pwszOutput, CONST DEVMODEW* lpInitData);
typedef HDC (WINAPI* CreateDCAProc)(LPCSTR pszDriver, LPCSTR pszDevice, LPCSTR pszPort, CONST DEVMODEA * pdm);
typedef BOOL (WINAPI* DeleteDCProc)(HDC hdc);

BOOL HookApiFromModule(HMODULE hModule, LPCSTR lpProcName, PVOID pDetour, PINLINE_HOOK_INFO* ppOutHookInfo)
{
    BOOL bRet = FALSE;

    TCHAR szProcessName[MAX_PATH] = {0};
    GetModuleFileName(NULL, szProcessName, _countof(szProcessName));
    PVOID pTarget = (PVOID)GetProcAddress(hModule, lpProcName);

    //FTLTRACE(TEXT("HookApiFromModule %s in %s, 0x%x => 0x%x, hWndProtect=0x%x\n"), 
    //    CA2T(lpProcName), PathFindFileName(szProcessName), pTarget, pDetour, g_pProtectWndInfoFileMap->hWndProtect);
    if (pTarget)
    {
        API_VERIFY(CreateInlineHook(pTarget, pDetour, NULL, ppOutHookInfo));
    }
    return bRet;
}

BOOL IsFilterHDC(HDC hDC)
{
    BOOL bWillFilter = FALSE;
    if (g_pProtectWndInfoFileMap && g_pProtectWndInfoFileMap->hWndProtect)
    {
        if (g_HookApiInfo.hCreateDesktopDC == hDC)
        {
            bWillFilter = TRUE;
        }
        HWND hWnd = WindowFromDC(hDC);
        if (hWnd && hWnd == GetDesktopWindow())
        {
            bWillFilter = TRUE;
        }
    }
    
    return bWillFilter;
}

BOOL DoFilterPaste(HDC hdcDest, int nXDest, int nYDest, int nWidthDest, int nHeightDest,
                   HDC hdcSrc, int nXSrc, int nYSrc, int nWidthSrc, int nHeightSrc, 
                   DWORD dwRop)
{
    BOOL bRet = FALSE;
    if (g_pProtectWndInfoFileMap)
    {
        HWND hWndFilter = (HWND)g_pProtectWndInfoFileMap->hWndProtect;

        CRect rcFilter(0, 0, 0, 0);
        API_VERIFY(GetWindowRect(hWndFilter, &rcFilter));
        if (bRet)
        {
            CRect rcTarget( nXDest, nYDest,  nXDest + nWidthDest, nYDest + nHeightDest);
            CRect rcSource( nXSrc, nYSrc, nXSrc + nWidthSrc, nYSrc + nHeightSrc);
            //FTLTRACE(TEXT("rcTarget=(%d,%d)-(%d,%d), %dx%d, rcSource=(%d,%d)-(%d,%d), rcFilter=(%d,%d)-(%d,%d), %dx%d"),
            //    rcTarget.left, rcTarget.top, rcTarget.right, rcTarget.bottom, 
            //    rcTarget.right - rcTarget.left, rcTarget.bottom - rcTarget.top,
            //    rcSource.left, rcSource.top, rcSource.right, rcSource.bottom, 
            //    rcFilter.left, rcFilter.top, rcFilter.right, rcFilter.bottom, 
            //    rcFilter.right - rcFilter.left, rcFilter.bottom - rcFilter.top
            //    );

            rcFilter.IntersectRect(&rcSource, &rcFilter);
            rcFilter.OffsetRect(nXDest - nXSrc, nYDest - nYSrc);
            if (!rcFilter.IsRectEmpty())
            {
                //FTLTRACE(TEXT("Will FillColor newRcSource=(%d,%d)-(%d,%d), %dx%d"),
                //    rcSource.left, rcSource.top, rcSource.right, rcSource.bottom, 
                //    rcSource.right - rcSource.left, rcSource.bottom - rcSource.top);
                float fZoomX = 1.0f;
                if (nWidthDest != nWidthSrc && nWidthSrc != 0)
                {
                    fZoomX = (float)nWidthDest/(float)nWidthSrc;
                    rcFilter.left = (LONG)(fZoomX * rcFilter.left);
                    rcFilter.right = (LONG)(fZoomX * rcFilter.right); 
                }
                float fZoomY = 1.0f;
                if (nHeightDest != nHeightSrc && nHeightSrc != 0)
                {
                    fZoomY = (float)nHeightDest / (float)nHeightSrc;
                    rcFilter.top = (LONG)(fZoomY * rcFilter.top);
                    rcFilter.bottom = (LONG)(fZoomY * rcFilter.bottom);
                }
                ::SetBkColor(hdcDest, g_pProtectWndInfoFileMap->clrDisabled);
                ::ExtTextOut(hdcDest, 0, 0, ETO_OPAQUE, &rcFilter, NULL, 0, NULL);
            }
        }
    }

    return bRet;
}

BOOL WINAPI Hooked_BitBlt(HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight,
                          HDC hdcSrc, int nXSrc, int nYSrc, DWORD dwRop)
{
    BOOL bRet = TRUE; 
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);
    __try
    {
        BitBltProc pOrigBitBlt = (BitBltProc)g_HookApiInfo.HookApiInfos[hft_BitBlt]->pOriginal;
        if (pOrigBitBlt)
        {
            bRet = (pOrigBitBlt)(hdcDest, nXDest, nYDest, nWidth, nHeight, hdcSrc, nXSrc, nYSrc, dwRop);
        }

        if (bRet && IsFilterHDC(hdcSrc))
        {
            TCHAR szProcessName[MAX_PATH] = {0};
            GetModuleFileName(NULL, szProcessName, _countof(szProcessName));
            FTLTRACE(TEXT("!!! Hooked_BitBlt Desktop PID=%d(%s), n"),
                GetCurrentProcessId(), PathFindFileName(szProcessName));

            DoFilterPaste(hdcDest, nXDest, nYDest, nWidth, nHeight,
                hdcSrc, nXSrc, nYSrc, nWidth, nHeight, dwRop);
        }
    }
    __finally
    {
        HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);
    }
    return bRet;
}

BOOL WINAPI Hooked_StretchBlt(HDC hdcDest, int nXDest, int nYDest, int nWidthDest, int nHeightDest,
                              HDC hdcSrc, int nXSrc, int nYSrc, int nWidthSrc, int nHeightSrc, 
                              DWORD dwRop)
{
    BOOL bRet = FALSE;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);
    __try
    {
        StretchBltProc pOrigStretchBlt = (StretchBltProc)g_HookApiInfo.HookApiInfos[hft_StretchBlt]->pOriginal;
        if (pOrigStretchBlt)
        {
            bRet = (pOrigStretchBlt)(hdcDest, nXDest, nYDest, nWidthDest, nHeightDest,
                hdcSrc, nXSrc, nYSrc, nWidthSrc, nHeightSrc, dwRop);
        }
        if (bRet && IsFilterHDC(hdcSrc))
        {
            DoFilterPaste(hdcDest, nXDest, nYDest, nWidthDest, nHeightDest, 
                hdcSrc, nXSrc, nYSrc, nWidthSrc, nHeightSrc, dwRop);
        }
    }
    __finally
    {
        HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);
    }

    return bRet;
}

HDC WINAPI Hooked_CreateDCW(LPCWSTR pwszDriver, LPCWSTR pwszDevice, LPCWSTR pwszOutput, CONST DEVMODEW* lpInitData)
{
    HDC hdc = NULL;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);
    __try
    {
        CreateDCWProc pOrigCreateDCW = (CreateDCWProc)g_HookApiInfo.HookApiInfos[hft_CreateDCW]->pOriginal;
        if (pOrigCreateDCW)
        {
            hdc = (pOrigCreateDCW)(pwszDriver, pwszDevice, pwszOutput, lpInitData);
        }
        if (pwszDriver && StrCmpIW(pwszDriver, L"DISPLAY") == 0)
        {
            FTLTRACE(TEXT("!!! In Hooked_CreateDCW, Create DISPLAY DC\n"));
            g_HookApiInfo.hCreateDesktopDC = hdc;
        }
    }
    __finally
    {
        HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);
    }

    return hdc;
}

HDC WINAPI Hooked_CreateDCA(LPCSTR pszDriver, LPCSTR pszDevice, LPCSTR pszPort, CONST DEVMODEA * pdm)
{
    HDC hdc = NULL;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);
    __try
    {
        CreateDCAProc pOrigCreateDCA = (CreateDCAProc)g_HookApiInfo.HookApiInfos[hft_CreateDCA]->pOriginal;
        if (pOrigCreateDCA)
        {
            hdc = (pOrigCreateDCA)(pszDriver, pszDevice, pszPort, pdm);
        }
        if (pszDriver && StrCmpIA(pszDriver, "DISPLAY") == 0)
        {
            FTLTRACE(TEXT("!!! In Hooked_CreateDCA, Create DISPLAY DC\n"));
            g_HookApiInfo.hCreateDesktopDC = hdc;
        }
    }
    __finally
    {
        HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);
    }

    return hdc;
}

BOOL WINAPI Hooked_DeleteDC(HDC hdc)
{
    BOOL bRet = FALSE;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);
    __try
    {
        if (hdc == g_HookApiInfo.hCreateDesktopDC)
        {
            g_HookApiInfo.hCreateDesktopDC = NULL;
            FTLTRACE(TEXT("!!! In Hooked_DeleteDC, Delete DISPLAY DC\n"));
        }
        DeleteDCProc pOrigDeleteDC = (DeleteDCProc)g_HookApiInfo.HookApiInfos[hft_DeleteDC]->pOriginal;
        if (pOrigDeleteDC)
        {
            bRet = (pOrigDeleteDC)(hdc);
        }
    }
    __finally
    {
        HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);
    }

    return bRet;
}


CProtectWndHookAPI::CProtectWndHookAPI(void)
{
}

CProtectWndHookAPI::~CProtectWndHookAPI(void)
{
}

BOOL CProtectWndHookAPI::StartHook()
{
    BOOL bRet = TRUE;

    TCHAR szModuleName[MAX_PATH] = {0};
    GetModuleFileName(NULL, szModuleName, _countof(szModuleName));

    FTL::CFAutoLock<FTL::CFLockObject> autoLock(&g_HookApiInfo.csLock);
    if ( !g_HookApiInfo.bHooked)
    {
        HRESULT hr = E_FAIL;
        COM_VERIFY(m_FileMap.OpenMapping(COMIC_PROTECT_WND_FILE_MAP_NAME, sizeof(ProtectWndInfoFileMap32), 0, FILE_MAP_READ));
        if (SUCCEEDED(hr))
        {
            g_pProtectWndInfoFileMap = (ProtectWndInfoFileMap32*)m_FileMap.GetData();
            if (g_pProtectWndInfoFileMap)
            {
                ATLTRACE(TEXT(">>> Will Hook API(g_bHooked=%d) in PID=%d(%s),TID=%d, ProtectWnd=0x%x\n"), 
                    g_HookApiInfo.bHooked, GetCurrentProcessId(), PathFindFileName(szModuleName), GetCurrentThreadId(), 
                    g_pProtectWndInfoFileMap->hWndProtect);

                HMODULE hModuleGdi32 = GetModuleHandle(TEXT("Gdi32.DLL"));
                FTLASSERT(hModuleGdi32);

                if (hModuleGdi32)
                {
                    g_HookApiInfo.bHooked = TRUE;

                    API_VERIFY(HookApiFromModule(hModuleGdi32, "BitBlt", &Hooked_BitBlt, &g_HookApiInfo.HookApiInfos[hft_BitBlt]));
                    API_VERIFY(HookApiFromModule(hModuleGdi32, "StretchBlt", &Hooked_StretchBlt, &g_HookApiInfo.HookApiInfos[hft_StretchBlt]));
                    API_VERIFY(HookApiFromModule(hModuleGdi32, "CreateDCA", &Hooked_CreateDCA, &g_HookApiInfo.HookApiInfos[hft_CreateDCA]));
                    API_VERIFY(HookApiFromModule(hModuleGdi32, "CreateDCW", &Hooked_CreateDCW, &g_HookApiInfo.HookApiInfos[hft_CreateDCW]));
                    API_VERIFY(HookApiFromModule(hModuleGdi32, "DeleteDC", &Hooked_DeleteDC, &g_HookApiInfo.HookApiInfos[hft_DeleteDC]));

                    //don't call FreeLibrary
                }
            }
        }
    }
    return bRet;
}

BOOL CProtectWndHookAPI::StopHook()
{
    BOOL bRet = TRUE;
    FTL::CFAutoLock<FTL::CFLockObject> autoLock(&g_HookApiInfo.csLock);

    TCHAR szModuleName[MAX_PATH] = {0};
    GetModuleFileName(NULL, szModuleName, _countof(szModuleName));
    ATLTRACE(TEXT("<<< Will UnHook API(g_bHooked=%d) in PID=%d(%s),TID=%d\n"), 
        g_HookApiInfo.bHooked, GetCurrentProcessId(), PathFindFileName(szModuleName), GetCurrentThreadId());

    
    if (g_HookApiInfo.bHooked)
    {
        while (g_HookApiInfo.HookedAPICallCount > 0)
        {
            FTLTRACE(TEXT("!!!g_HookApiInfo.HookedAPICallCount : %d\n"), g_HookApiInfo.HookedAPICallCount);
            Sleep(10);
        }
        g_HookApiInfo.bHooked = FALSE;
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_BitBlt]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_StretchBlt]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_CreateDCW]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_CreateDCA]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_DeleteDC]));
        m_FileMap.Unmap();
    }
    return bRet;
}
