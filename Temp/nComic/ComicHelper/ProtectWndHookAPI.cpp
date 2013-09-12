#include "StdAfx.h"
#include "ProtectWndHookAPI.h"
#include "InlineHook.h"
#include <ftlThread.h>
#include <ftlGdi.h>
#include "resource.h"

extern HMODULE g_hModule;

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

    hft_OpenProcess,
    hft_TerminateProcess,
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

    HDC     hDCMemory;          //HDC memory for store background
    HBITMAP hBmpBackground;
    HBITMAP hOldBitmap;
    BYTE*   pBackgroundBuffer;
    int     nImageWidth;
    int     nImageHeight;

    HANDLE  hProtectProcessInCurProcess;
    PINLINE_HOOK_INFO    HookApiInfos[hft_FunctionCount];

    HOOK_API_INFO()
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

typedef HANDLE (WINAPI* OpenProcessProc)(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId);
typedef BOOL (WINAPI* TerminateProcessProc)(HANDLE hProcess, UINT uExitCode);

BOOL HookApiFromModule(HMODULE hModule, LPCSTR lpProcName, PVOID pDetour, PINLINE_HOOK_INFO* ppOutHookInfo)
{
    BOOL bRet = FALSE;

    TCHAR szProcessName[MAX_PATH] = {0};
    GetModuleFileName(NULL, szProcessName, _countof(szProcessName));
    PVOID pTarget = (PVOID)GetProcAddress(hModule, lpProcName);

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

HANDLE WINAPI Hooked_OpenProcess(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId)
{
    HANDLE hProcess = NULL;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);
    __try
    {
        OpenProcessProc pOrigOpenProcess = (OpenProcessProc)g_HookApiInfo.HookApiInfos[hft_OpenProcess]->pOriginal;
        if (pOrigOpenProcess)
        {
            hProcess = (pOrigOpenProcess)(dwDesiredAccess, bInheritHandle, dwProcessId);
            if (dwProcessId == g_pProtectWndInfoFileMap->dwProtectProcessId)
            {
                g_HookApiInfo.hProtectProcessInCurProcess = hProcess;
            }
        }
    }
    __finally
    {
        HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);
    }
    return hProcess;
}

BOOL WINAPI Hooked_TerminateProcess(HANDLE hProcess, UINT uExitCode)
{
    BOOL bRet = FALSE;
    SetLastError(ERROR_ACCESS_DENIED);
    FTLTRACE(TEXT("!!! TerminateProcess HANDLE=0x%x, \n"), hProcess);
    
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);
    __try
    {
        TerminateProcessProc pOrigTerminateProcess = (TerminateProcessProc)g_HookApiInfo.HookApiInfos[hft_TerminateProcess]->pOriginal;
        if (pOrigTerminateProcess)
        {
            if (g_HookApiInfo.hProtectProcessInCurProcess != hProcess)
            {
                bRet = pOrigTerminateProcess(hProcess, uExitCode);
            }
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

BOOL CProtectWndHookAPI::_InitImgBackground()
{
    BOOL bRet = FALSE;
    FUNCTION_BLOCK_NAME_TRACE_EX(TEXT("UnHookApi"), FTL::TraceDetailExeName, 10);

    ATL::CImage* pImgBackground = new ATL::CImage();
    if (pImgBackground)
    {
        API_VERIFY(FTL::CFGdiUtil::LoadPNGFromResource(*pImgBackground, g_hModule, IDB_FILTER_BACKGROUND));
        if (bRet)
        {
            g_HookApiInfo.nImageWidth = pImgBackground->GetWidth();
            g_HookApiInfo.nImageHeight = pImgBackground->GetHeight();

            HWND hWndDesktop = GetDesktopWindow();
            HDC hDCDesktop = ::GetWindowDC(hWndDesktop);
            HDC hDCMemory = ::CreateCompatibleDC(hDCDesktop);

            BITMAPINFO bmpInfo = {0};
            bmpInfo.bmiHeader.biSize = sizeof(bmpInfo.bmiHeader); // sizeof(BITMAPINFO);
            bmpInfo.bmiHeader.biWidth = g_HookApiInfo.nImageWidth;
            bmpInfo.bmiHeader.biHeight = g_HookApiInfo.nImageHeight;
            bmpInfo.bmiHeader.biPlanes = 1;
            bmpInfo.bmiHeader.biBitCount = (WORD)32;
            bmpInfo.bmiHeader.biClrUsed = 0;
            bmpInfo.bmiHeader.biCompression = BI_RGB;
            bmpInfo.bmiHeader.biSizeImage = ((g_HookApiInfo.nImageWidth * g_HookApiInfo.nImageHeight * 32 + 31) >> 3) & ~3;

            HBITMAP hBmpBackground = NULL;
            API_VERIFY((hBmpBackground = ::CreateDIBSection(hDCDesktop, &bmpInfo, DIB_RGB_COLORS,  
                (VOID**)&g_HookApiInfo.pBackgroundBuffer, NULL, 0)) != NULL);

            //FTLTRACE(TEXT("CreateDIBSection, hBmpBackground=0x%x, Err=%d, width=%d, height=%d, sizeImage=%d\n"), 
            //    hBmpBackground, GetLastError(), bmpInfo.bmiHeader.biWidth, bmpInfo.bmiHeader.biHeight, bmpInfo.bmiHeader.biSizeImage);

            g_HookApiInfo.hOldBitmap = (HBITMAP)::SelectObject(hDCMemory, hBmpBackground);
            CImageDC imgDC(*pImgBackground);
            API_VERIFY(BitBlt(hDCMemory, 0, 0, g_HookApiInfo.nImageWidth, g_HookApiInfo.nImageHeight, imgDC, 0, 0, SRCCOPY));
            FTLTRACE(TEXT("_InitImgBackground, BitBlt ret %d, HDCmemory=0x%x, hBmpBackground=0x%x\n"),
                bRet, hDCMemory, hBmpBackground);
            if (bRet)
            {
                g_HookApiInfo.hDCMemory = hDCMemory;
                g_HookApiInfo.hBmpBackground = hBmpBackground;
            }
            else
            {
                DeleteDC(hDCMemory);
                DeleteObject(hBmpBackground);
            }

            ReleaseDC(hWndDesktop, hDCDesktop);
        }
        delete pImgBackground;
    }


    return bRet;
}
BOOL CProtectWndHookAPI::_ReleaseImgBackground()
{
    BOOL bRet = TRUE;
    FUNCTION_BLOCK_NAME_TRACE_EX(TEXT("_ReleaseImgBackground"), FTL::TraceDetailExeName, 10);

    if (g_HookApiInfo.hBmpBackground)
    {
        ::SelectObject(g_HookApiInfo.hDCMemory, g_HookApiInfo.hOldBitmap);
        API_VERIFY(DeleteObject(g_HookApiInfo.hBmpBackground));
        API_VERIFY(DeleteDC(g_HookApiInfo.hDCMemory));
    }
    g_HookApiInfo.nImageWidth = 0;
    g_HookApiInfo.nImageHeight = 0;

    return bRet;
}
BOOL CProtectWndHookAPI::StartHook()
{
    BOOL bRet = TRUE;

    TCHAR szModuleName[MAX_PATH] = {0};
    GetModuleFileName(NULL, szModuleName, _countof(szModuleName));

    TCHAR szTrace[MAX_PATH] = {0};
    StringCchPrintf(szTrace, _countof(szTrace), TEXT("StartHook in %s"), PathFindFileName(szModuleName));
    FUNCTION_BLOCK_NAME_TRACE(szTrace, 1);

    //FTL::CFAutoLock<FTL::CFLockObject> autoLock(&g_HookApiInfo.csLock);
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
                    API_VERIFY(_InitImgBackground());

                    API_VERIFY(HookApiFromModule(hModuleGdi32, "BitBlt", &Hooked_BitBlt, &g_HookApiInfo.HookApiInfos[hft_BitBlt]));
                    API_VERIFY(HookApiFromModule(hModuleGdi32, "StretchBlt", &Hooked_StretchBlt, &g_HookApiInfo.HookApiInfos[hft_StretchBlt]));
                    API_VERIFY(HookApiFromModule(hModuleGdi32, "CreateDCA", &Hooked_CreateDCA, &g_HookApiInfo.HookApiInfos[hft_CreateDCA]));
                    API_VERIFY(HookApiFromModule(hModuleGdi32, "CreateDCW", &Hooked_CreateDCW, &g_HookApiInfo.HookApiInfos[hft_CreateDCW]));
                    API_VERIFY(HookApiFromModule(hModuleGdi32, "DeleteDC", &Hooked_DeleteDC, &g_HookApiInfo.HookApiInfos[hft_DeleteDC]));
                    //don't call FreeLibrary
                }
                HMODULE hModuleKernel32 = GetModuleHandle(TEXT("Kernel32.DLL"));
                if (hModuleKernel32)
                {
                    //API_VERIFY(HookApiFromModule(hModuleKernel32, "OpenProcess", &Hooked_OpenProcess, &g_HookApiInfo.HookApiInfos[hft_OpenProcess]));
                    //API_VERIFY(HookApiFromModule(hModuleKernel32, "TerminateProcess", &Hooked_TerminateProcess, &g_HookApiInfo.HookApiInfos[hft_TerminateProcess]));
                }
            }
        }
    }
    return bRet;
}

BOOL CProtectWndHookAPI::StopHook()
{
    BOOL bRet = TRUE;
    //FTL::CFAutoLock<FTL::CFLockObject> autoLock(&g_HookApiInfo.csLock);

    TCHAR szModuleName[MAX_PATH] = {0};
    GetModuleFileName(NULL, szModuleName, _countof(szModuleName));

    TCHAR szTrace[MAX_PATH] = {0};
    StringCchPrintf(szTrace, _countof(szTrace), TEXT("StopHook in %s"), PathFindFileName(szModuleName));
    FUNCTION_BLOCK_NAME_TRACE(szTrace, 10);

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
       // API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_TerminateProcess]));
        //API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_OpenProcess]));

        {
            FUNCTION_BLOCK_NAME_TRACE_EX(TEXT("ReleaseImage + UnMap"), FTL::TraceDetailExeName, 100);

            API_VERIFY(_ReleaseImgBackground());
            m_FileMap.Unmap();
        }
    }
    return bRet;
}
