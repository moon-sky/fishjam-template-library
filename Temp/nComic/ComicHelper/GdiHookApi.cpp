#include "stdafx.h"
#include "HookApi.h"
#include "GdiHookApi.h"
#include "../ComicHelperProxy/ComicHelperProxy.h"

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

CRect GetFitRect( const CRect& rcMargin, const CSize& szContent )
{
    CRect rcResult = rcMargin;

    INT nWidthAllowed = rcMargin.Width();
    INT nHeightAllowed = rcMargin.Height();
    INT nWidthFactor = szContent.cx;
    INT nHeightFactor = szContent.cy;

    if ( ( nWidthAllowed >= nWidthFactor ) && ( nHeightAllowed >= nHeightFactor ) )
    {
        rcResult.left = rcMargin.left + ( rcMargin.Width() - szContent.cx ) / 2;
        rcResult.top = rcMargin.top + (rcMargin.Height() - szContent.cy ) / 2;
        rcResult.right = rcResult.left + szContent.cx;
        rcResult.bottom = rcResult.top + szContent.cy;
    }
    else
    {
        if ( MulDiv( nWidthAllowed, nHeightFactor, nWidthFactor ) < nHeightAllowed )
        {
            INT nHeight = MulDiv( nWidthAllowed, nHeightFactor, nWidthFactor );
            INT nDiff = ( nHeightAllowed - nHeight ) / 2;
            rcResult.top = rcResult.top + nDiff;
            rcResult.bottom = rcResult.bottom - nDiff;
        }
        else
        {
            INT nWidth = MulDiv( nHeightAllowed, nWidthFactor, nHeightFactor );
            INT nDiff = ( nWidthAllowed - nWidth ) / 2;
            rcResult.left = rcResult.left + nDiff;
            rcResult.right = rcResult.right - nDiff;
        }
    }
    return rcResult;
}

BOOL DoFilterPaste(HDC hdcDest, int nXDest, int nYDest, int nWidthDest, int nHeightDest,
                   HDC hdcSrc, int nXSrc, int nYSrc, int nWidthSrc, int nHeightSrc, 
                   DWORD dwRop, RECT* prcFilter)
{
    BOOL bRet = FALSE;
    if (g_pProtectWndInfoFileMap)
    {
        CRect rcFilter(0, 0, 0, 0);
        if (prcFilter)
        {
            rcFilter = *prcFilter;
        }
        else
        {
            HWND hWndFilter = (HWND)g_pProtectWndInfoFileMap->hWndProtect;
            API_VERIFY(GetWindowRect(hWndFilter, &rcFilter));
        }

        //if (bRet)
        {
            CRect rcTarget( nXDest, nYDest,  nXDest + nWidthDest, nYDest + nHeightDest);
            CRect rcSource( nXSrc, nYSrc, nXSrc + nWidthSrc, nYSrc + nHeightSrc);
            FTLTRACE(TEXT("rcTarget=(%d,%d)-(%d,%d), %dx%d, rcSource=(%d,%d)-(%d,%d), %dx%d, rcFilter=(%d,%d)-(%d,%d), %dx%d"),
                rcTarget.left, rcTarget.top, rcTarget.right, rcTarget.bottom, rcTarget.Width(), rcTarget.Height(),
                rcSource.left, rcSource.top, rcSource.right, rcSource.bottom, rcSource.Width(), rcSource.Height(),
                rcFilter.left, rcFilter.top, rcFilter.right, rcFilter.bottom, rcFilter.Width(), rcFilter.Height()
                );

            if (rcSource.left == 0 && rcSource.top == 0)
            {

            }
            else
            {
                //rcFilter.OffsetRect(-rcFilter.TopLeft());
            }

            rcFilter.IntersectRect(&rcSource, &rcFilter);
            if (!rcFilter.IsRectEmpty())
            {
                rcFilter.OffsetRect(-nXSrc, -nYSrc);

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
            }

            //rcFilter.OffsetRect(-rcFilter.TopLeft());

            FTLTRACE(TEXT("Will FillColor rcFilter=(%d,%d)-(%d,%d), %dx%d"),
                rcFilter.left, rcFilter.top, rcFilter.right, rcFilter.bottom, 
                rcFilter.Width(), rcFilter.Height());

            ::SetBkColor(hdcDest, g_pProtectWndInfoFileMap->clrDisabled);
            ::ExtTextOut(hdcDest, 0, 0, ETO_OPAQUE, &rcFilter, NULL, 0, NULL);

            StretchBltProc pOrigStretchBlt = (StretchBltProc)g_HookApiInfo.HookApiInfos[hft_StretchBlt]->pOriginal;
            if (pOrigStretchBlt && g_HookApiInfo.hDCMemory)
            {
                CSize szImage(g_HookApiInfo.nImageWidth, g_HookApiInfo.nImageHeight);
                rcFilter = GetFitRect(rcFilter, szImage);

                FTLTRACE(TEXT("!!!DoFilterPaste: [%d,%d],%dx%d <== [%d,%d],%dx%d\n"),
                    rcFilter.left, rcFilter.top, rcFilter.Width(), rcFilter.Height(),
                    0, 0, g_HookApiInfo.nImageWidth, g_HookApiInfo.nImageHeight);
                (pOrigStretchBlt)(hdcDest, rcFilter.left, rcFilter.top, rcFilter.Width(), rcFilter.Height(), 
                    g_HookApiInfo.hDCMemory, 0, 0, g_HookApiInfo.nImageWidth, g_HookApiInfo.nImageHeight, SRCCOPY);
            }
        }
    }

    return bRet;
}

BOOL WINAPI Hooked_BitBlt(HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight,
                          HDC hdcSrc, int nXSrc, int nYSrc, DWORD dwRop)
{
    BOOL bRet = TRUE; 
    //HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);
    __try
    {
        BitBltProc pOrigBitBlt = (BitBltProc)g_HookApiInfo.HookApiInfos[hft_BitBlt]->pOriginal;

        if (IsFilterHDC(hdcSrc))
        {
            TCHAR szProcessName[MAX_PATH] = {0};
            GetModuleFileName(NULL, szProcessName, _countof(szProcessName));
            FTLTRACE(TEXT("!!! Hooked_BitBlt Desktop PID=%d(%s), Dest=[%d, %d], %dx%d, Src=[%d, %d]\n"),
                GetCurrentProcessId(), PathFindFileName(szProcessName), nXDest, nYDest, nWidth, nHeight, nXSrc, nYSrc);

            HWND hWndFilter = (HWND)g_pProtectWndInfoFileMap->hWndProtect;
            RECT rcFilterWnd = {0};
            bRet = GetWindowRect(hWndFilter, &rcFilterWnd);
            if (pOrigBitBlt)
            {
                bRet = (pOrigBitBlt)(hdcDest, nXDest, nYDest, nWidth, nHeight, hdcSrc, nXSrc, nYSrc, dwRop);
            }
            DoFilterPaste(hdcDest, nXDest, nYDest, nWidth, nHeight,
                hdcSrc, nXSrc, nYSrc, nWidth, nHeight, dwRop, &rcFilterWnd);
        }
        else
        {
            if (pOrigBitBlt)
            {
                bRet = (pOrigBitBlt)(hdcDest, nXDest, nYDest, nWidth, nHeight, hdcSrc, nXSrc, nYSrc, dwRop);
            }
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
    //FUNCTION_BLOCK_MODULE_NAME_TRACE(TEXT("Hooked_StretchBlt"), 10);
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);
    __try
    {
        StretchBltProc pOrigStretchBlt = (StretchBltProc)g_HookApiInfo.HookApiInfos[hft_StretchBlt]->pOriginal;
        if (IsFilterHDC(hdcSrc))
        {
            HWND hWndFilter = (HWND)g_pProtectWndInfoFileMap->hWndProtect;
            RECT rcFilterWnd = {0};
            bRet = GetWindowRect(hWndFilter, &rcFilterWnd);

            if (pOrigStretchBlt)
            {
                bRet = (pOrigStretchBlt)(hdcDest, nXDest, nYDest, nWidthDest, nHeightDest,
                    hdcSrc, nXSrc, nYSrc, nWidthSrc, nHeightSrc, dwRop);
            }

            DoFilterPaste(hdcDest, nXDest, nYDest, nWidthDest, nHeightDest, 
                hdcSrc, nXSrc, nYSrc, nWidthSrc, nHeightSrc, dwRop, &rcFilterWnd);
        }
        else
        {
            if (pOrigStretchBlt)
            {
                bRet = (pOrigStretchBlt)(hdcDest, nXDest, nYDest, nWidthDest, nHeightDest,
                    hdcSrc, nXSrc, nYSrc, nWidthSrc, nHeightSrc, dwRop);
            }
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
    //FUNCTION_BLOCK_MODULE_NAME_TRACE(TEXT("Hooked_CreateDCW"), 10);

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
    //FUNCTION_BLOCK_MODULE_NAME_TRACE(TEXT("Hooked_CreateDCA"), 10);

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
    //FUNCTION_BLOCK_MODULE_NAME_TRACE(TEXT("Hooked_DeleteDC"), 10);

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
