#include "StdAfx.h"
#include "ProtectWndHookAPI.h"
#include "InlineHook.h"

ProtectWndInfoFileMap* g_pProtectWndInfoFileMap = NULL;

enum HookFuncType
{
    hft_BitBlt = 0,
    hft_StretchBlt,
    hft_PlgBlt,
    hft_MaskBlt,
    hft_TransparentBlt,
    //hft_ExtTextOutW,

    //hft_NtGdiOpenDCW,
    //hft_NtGdiDeleteObjectApp,

    //hft_NtUserPrintWindow,
    //hft_NtGdiDdLock,
    //hft_NtGdiDdUnlock,
    hft_FunctionCount
};

struct HOOK_API_INFO
{
    BOOL    bHooked;
    PINLINE_HOOK_INFO    HookApiInfos[hft_FunctionCount];

    HOOK_API_INFO()
    {
        bHooked = FALSE;
    }
}g_HookApiInfo;

//#pragma data_seg("MyShare")
//PROTECT_WND_INFO g_ProtectWndInfo;
//#pragma data_seg()
//#pragma comment(linker,"/SECTION:MyShare,RWS")

BOOL HookApiFromModule(HMODULE hModule, LPCSTR lpProcName, PVOID pDetour, PINLINE_HOOK_INFO* ppOutHookInfo);


typedef BOOL (WINAPI *BitBltProc)(HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight,
                                  HDC hdcSrc, int nXSrc, int nYSrc, DWORD dwRop);
typedef BOOL (WINAPI *StretchBltProc)(HDC hdcDest, int nXOriginDest, int nYOriginDest, int nWidthDest, int nHeightDest,
                                      HDC hdcSrc, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc, 
                                      DWORD dwRop);


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

BOOL WINAPI Hooked_BitBlt(HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight,
                          HDC hdcSrc, int nXSrc, int nYSrc, DWORD dwRop)
{
    BOOL bRet = TRUE; 

    BitBltProc pOrigBitBlt = (BitBltProc)g_HookApiInfo.HookApiInfos[hft_BitBlt]->pOriginal;
    bRet = (pOrigBitBlt)(hdcDest, nXDest, nYDest, nWidth, nHeight, hdcSrc, nXSrc, nYSrc, dwRop);

    //FTLTRACE(TEXT("[%d] In Hooked_BitBlt, %s, g_hFilterWnd=%d\n"), 
    //    GetCurrentProcessId(), PathFindFileName(szProcessName), g_ProtectWndInfo.hFilterWnd);

    if (bRet && NULL != g_pProtectWndInfoFileMap)
    {
        HWND hWnd = WindowFromDC(hdcSrc);
        HWND hWndFilter = g_pProtectWndInfoFileMap->hWndProtect;
        if (hWnd != NULL)
        {
            if (hWnd == GetDesktopWindow() || hWnd == hWndFilter|| ::IsChild(hWndFilter, hWnd))
            {
                TCHAR szProcessName[MAX_PATH] = {0};
                GetModuleFileName(NULL, szProcessName, _countof(szProcessName));
                FTLTRACE(TEXT("!!! Hooked_BitBlt Desktop PID=%d(%s), hWnd=%d, g_hFilterWnd=%d\n"),
                    GetCurrentProcessId(), PathFindFileName(szProcessName), hWnd, hWndFilter);
#if 0
                //CRect rcSource(x1, y1, x1 + cx, y1 + cy);
                //::SetBkColor(hdc, g_clrDisabled);
                //::ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rcSource, NULL, 0, NULL);
#else

                CRect rcTarget( nXDest, nYDest,  nXDest + nWidth, nYDest + nHeight);
                CRect rcSource( nXSrc, nYSrc, nXSrc + nWidth, nYSrc   + nHeight);
                CRect rcFilter(0, 0, 0, 0);

                API_VERIFY(GetWindowRect(hWndFilter, &rcFilter));
                if (bRet)
                {
                    //FTLTRACE(TEXT("rcTarget=(%d,%d)-(%d,%d), %dx%d, rcSource=(%d,%d)-(%d,%d), rcFilter=(%d,%d)-(%d,%d), %dx%d"),
                    //    rcTarget.left, rcTarget.top, rcTarget.right, rcTarget.bottom, 
                    //    rcTarget.right - rcTarget.left, rcTarget.bottom - rcTarget.top,
                    //    rcSource.left, rcSource.top, rcSource.right, rcSource.bottom, 
                    //    rcFilter.left, rcFilter.top, rcFilter.right, rcFilter.bottom, 
                    //    rcFilter.right - rcFilter.left, rcFilter.bottom - rcFilter.top
                    //    );

                    rcSource.IntersectRect(&rcSource, &rcFilter);
                    rcSource.OffsetRect(nXDest - nXSrc, nYDest - nYSrc);
                    if (!rcSource.IsRectEmpty())
                    {
                        //FTLTRACE(TEXT("Will FillColor newRcSource=(%d,%d)-(%d,%d), %dx%d"),
                        //    rcSource.left, rcSource.top, rcSource.right, rcSource.bottom, 
                        //    rcSource.right - rcSource.left, rcSource.bottom - rcSource.top);
                        ::SetBkColor(hdcDest, g_pProtectWndInfoFileMap->clrDisabled);
                        ::ExtTextOut(hdcDest, 0, 0, ETO_OPAQUE, &rcSource, NULL, 0, NULL);
                    }
                }
#endif 
            }
        }
    }
    return bRet;
}

BOOL WINAPI Hooked_StretchBlt(HDC hdcDest, int nXOriginDest, int nYOriginDest, int nWidthDest, int nHeightDest,
                              HDC hdcSrc, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc, 
                              DWORD dwRop)
{
    BOOL bRet = FALSE;
    StretchBltProc pOrigStretchBlt = (StretchBltProc)g_HookApiInfo.HookApiInfos[hft_StretchBlt]->pOriginal;
    if (pOrigStretchBlt)
    {
        bRet = (pOrigStretchBlt)(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest,
            hdcSrc, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, dwRop);
    }
    if (bRet && g_pProtectWndInfoFileMap && g_pProtectWndInfoFileMap->hWndProtect)
    {
        HWND hWnd = WindowFromDC(hdcSrc);
        if (hWnd != NULL)
        {
            HWND hWndFilter = g_pProtectWndInfoFileMap->hWndProtect;
            if (hWnd == GetDesktopWindow() || hWnd == hWndFilter || ::IsChild(hWndFilter, hWnd))
            {
                FTLTRACE(TEXT("!!! In Hooked_StretchBlt, Desktop\n"));
                CRect rcSource(nXOriginDest, nYOriginDest, nXOriginDest + nWidthDest, nYOriginDest + nHeightDest);
                ::SetBkColor(hdcDest, g_pProtectWndInfoFileMap->clrDisabled);
                ::ExtTextOut(hdcDest, 0, 0, ETO_OPAQUE, &rcSource, NULL, 0, NULL);
            }
        }
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

    if ( !g_HookApiInfo.bHooked)
    {
        g_HookApiInfo.bHooked = TRUE;

        HRESULT hr = E_FAIL;
        COM_VERIFY(m_FileMap.OpenMapping(COMIC_PROTECT_WND_FILE_MAP_NAME, sizeof(ProtectWndInfoFileMap), 0, FILE_MAP_READ));
        if (SUCCEEDED(hr))
        {
            g_pProtectWndInfoFileMap = (ProtectWndInfoFileMap*)m_FileMap.GetData();
            if (g_pProtectWndInfoFileMap)
            {
                ATLTRACE(TEXT(">>> Will Hook API(g_bHooked=%d) in PID=%d(%s),TID=%d, ProtectWnd=0x%x\n"), 
                    g_HookApiInfo.bHooked, GetCurrentProcessId(), PathFindFileName(szModuleName), GetCurrentThreadId(), 
                    g_pProtectWndInfoFileMap->hWndProtect);

                HMODULE hModuleGdi32 = GetModuleHandle(TEXT("Gdi32.DLL"));
                FTLASSERT(hModuleGdi32);

                if (hModuleGdi32)
                {
                    API_VERIFY(HookApiFromModule(hModuleGdi32, "BitBlt", &Hooked_BitBlt, &g_HookApiInfo.HookApiInfos[hft_BitBlt]));
                    API_VERIFY(HookApiFromModule(hModuleGdi32, "StretchBlt", &Hooked_StretchBlt, &g_HookApiInfo.HookApiInfos[hft_StretchBlt]));

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

    TCHAR szModuleName[MAX_PATH] = {0};
    GetModuleFileName(NULL, szModuleName, _countof(szModuleName));
    ATLTRACE(TEXT("<<< Will UnHook API(g_bHooked=%d) in PID=%d(%s),TID=%d\n"), 
        g_HookApiInfo.bHooked, GetCurrentProcessId(), PathFindFileName(szModuleName), GetCurrentThreadId());

    if (g_HookApiInfo.bHooked)
    {
        g_HookApiInfo.bHooked = FALSE;

        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_BitBlt]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_StretchBlt]));

        m_FileMap.Unmap();
    }
    return bRet;
}
