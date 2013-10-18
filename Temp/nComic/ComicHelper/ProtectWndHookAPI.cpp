#include "StdAfx.h"
#include "ProtectWndHookAPI.h"
#include "InlineHook.h"
#include "resource.h"
#include "SetupInfoMgr.h"
#include "HookApi.h"
#include "FileHookApi.h"
#include "GdiHookApi.h"
#include "ProcessHookApi.h"
#include "RegHookAPI.h"


ProtectWndInfoFileMap32* g_pProtectWndInfoFileMap = NULL;
CSetupInfoMgr*           g_pSetupInfoMgr = NULL;

CProtectWndHookAPI::CProtectWndHookAPI(void)
{
}

CProtectWndHookAPI::~CProtectWndHookAPI(void)
{
}

BOOL CProtectWndHookAPI::_InitImgBackground()
{
    BOOL bRet = FALSE;
    FUNCTION_BLOCK_NAME_TRACE_EX(TEXT("UnHookApi"), FTL::TraceDetailExeName, 100);

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
    FUNCTION_BLOCK_NAME_TRACE_EX(TEXT("_ReleaseImgBackground"), FTL::TraceDetailExeName, 100);

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
    FUNCTION_BLOCK_NAME_TRACE(szTrace, 100);

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
                g_pSetupInfoMgr = new CSetupInfoMgr();

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
                    //API_VERIFY(HookApiFromModule(hModuleGdi32, "PlgBlt", &Hooked_PlgBlt, &g_HookApiInfo.HookApiInfos[hft_PlgBlt]));
                    //API_VERIFY(HookApiFromModule(hModuleGdi32, "MaskBlt", &Hooked_MaskBlt, &g_HookApiInfo.HookApiInfos[hft_MaskBlt]));
                    API_VERIFY(HookApiFromModule(hModuleGdi32, "CreateDCA", &Hooked_CreateDCA, &g_HookApiInfo.HookApiInfos[hft_CreateDCA]));
                    API_VERIFY(HookApiFromModule(hModuleGdi32, "CreateDCW", &Hooked_CreateDCW, &g_HookApiInfo.HookApiInfos[hft_CreateDCW]));
                    API_VERIFY(HookApiFromModule(hModuleGdi32, "DeleteDC", &Hooked_DeleteDC, &g_HookApiInfo.HookApiInfos[hft_DeleteDC]));
                    //don't call FreeLibrary
                }
                HMODULE hModuleKernel32 = GetModuleHandle(TEXT("Kernel32.DLL"));
                FTLASSERT(hModuleKernel32);
                if (hModuleKernel32)
                {
                    //API_VERIFY(HookApiFromModule(hModuleKernel32, "OpenProcess", &Hooked_OpenProcess, &g_HookApiInfo.HookApiInfos[hft_OpenProcess]));
                    //API_VERIFY(HookApiFromModule(hModuleKernel32, "TerminateProcess", &Hooked_TerminateProcess, &g_HookApiInfo.HookApiInfos[hft_TerminateProcess]));

                    API_VERIFY(HookApiFromModule(hModuleKernel32, "DeleteFileA", &Hooked_DeleteFileA, &g_HookApiInfo.HookApiInfos[hft_DeleteFileA]));
                    API_VERIFY(HookApiFromModule(hModuleKernel32, "DeleteFileW", &Hooked_DeleteFileW, &g_HookApiInfo.HookApiInfos[hft_DeleteFileW]));
                    API_VERIFY(HookApiFromModule(hModuleKernel32, "CreateFileA", &Hooked_CreateFileA, &g_HookApiInfo.HookApiInfos[hft_CreateFileA]));
                    API_VERIFY(HookApiFromModule(hModuleKernel32, "CreateFileW", &Hooked_CreateFileW, &g_HookApiInfo.HookApiInfos[hft_CreateFileW]));
                    API_VERIFY(HookApiFromModule(hModuleKernel32, "MoveFileA", &Hooked_MoveFileA, &g_HookApiInfo.HookApiInfos[hft_MoveFileA]));
                    API_VERIFY(HookApiFromModule(hModuleKernel32, "MoveFileW", &Hooked_MoveFileW, &g_HookApiInfo.HookApiInfos[hft_MoveFileW]));
                }
                HMODULE hModuleAdvapi32 = GetModuleHandle(TEXT("Advapi32.dll"));
                FTLASSERT(hModuleAdvapi32);
                if (hModuleAdvapi32)
                {
                    API_VERIFY(HookApiFromModule(hModuleAdvapi32, "RegCloseKey", &Hooked_RegCloseKey, &g_HookApiInfo.HookApiInfos[hft_RegCloseKey]));
                    API_VERIFY(HookApiFromModule(hModuleAdvapi32, "RegOpenKeyA", &Hooked_RegOpenKeyA, &g_HookApiInfo.HookApiInfos[hft_RegOpenKeyA]));
                    API_VERIFY(HookApiFromModule(hModuleAdvapi32, "RegOpenKeyW", &Hooked_RegOpenKeyW, &g_HookApiInfo.HookApiInfos[hft_RegOpenKeyW]));
                    API_VERIFY(HookApiFromModule(hModuleAdvapi32, "RegCreateKeyA", &Hooked_RegCreateKeyA, &g_HookApiInfo.HookApiInfos[hft_RegCreateKeyA]));
                    API_VERIFY(HookApiFromModule(hModuleAdvapi32, "RegCreateKeyW", &Hooked_RegCreateKeyW, &g_HookApiInfo.HookApiInfos[hft_RegCreateKeyW]));
                    API_VERIFY(HookApiFromModule(hModuleAdvapi32, "RegCreateKeyExA", &Hooked_RegCreateKeyExA, &g_HookApiInfo.HookApiInfos[hft_RegCreateKeyExA]));
                    API_VERIFY(HookApiFromModule(hModuleAdvapi32, "RegCreateKeyExW", &Hooked_RegCreateKeyExW, &g_HookApiInfo.HookApiInfos[hft_RegCreateKeyExW]));

                    API_VERIFY(HookApiFromModule(hModuleAdvapi32, "RegSetValueA", &Hooked_RegSetValueA, &g_HookApiInfo.HookApiInfos[hft_RegSetValueA]));
                    API_VERIFY(HookApiFromModule(hModuleAdvapi32, "RegSetValueW", &Hooked_RegSetValueW, &g_HookApiInfo.HookApiInfos[hft_RegSetValueW]));
                    API_VERIFY(HookApiFromModule(hModuleAdvapi32, "RegSetValueExA", &Hooked_RegSetValueExA, &g_HookApiInfo.HookApiInfos[hft_RegSetValueExA]));
                    API_VERIFY(HookApiFromModule(hModuleAdvapi32, "RegSetValueExW", &Hooked_RegSetValueExW, &g_HookApiInfo.HookApiInfos[hft_RegSetValueExW]));

                    API_VERIFY(HookApiFromModule(hModuleAdvapi32, "RegSetKeyValueA", &Hooked_RegSetKeyValueA, &g_HookApiInfo.HookApiInfos[hft_RegSetKeyValueA]));
                    API_VERIFY(HookApiFromModule(hModuleAdvapi32, "RegSetKeyValueW", &Hooked_RegSetKeyValueW, &g_HookApiInfo.HookApiInfos[hft_RegSetKeyValueW]));
                }
            }
        }
    }
    return bRet;
}

BOOL CProtectWndHookAPI::StopHook()
{
    BOOL bRet = TRUE;
    HRESULT hr = E_FAIL;
    //FTL::CFAutoLock<FTL::CFLockObject> autoLock(&g_HookApiInfo.csLock);

    TCHAR szModuleName[MAX_PATH] = {0};
    GetModuleFileName(NULL, szModuleName, _countof(szModuleName));

    TCHAR szTrace[MAX_PATH] = {0};
    StringCchPrintf(szTrace, _countof(szTrace), TEXT("StopHook in %s"), PathFindFileName(szModuleName));
    FUNCTION_BLOCK_NAME_TRACE(szTrace, 100);

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
        //API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_PlgBlt]));
        //API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_MaskBlt]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_CreateDCW]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_CreateDCA]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_DeleteDC]));

        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_TerminateProcess]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_OpenProcess]));

        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_RegCloseKey]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_RegOpenKeyA]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_RegOpenKeyW]));

        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_RegCreateKeyA]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_RegCreateKeyW]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_RegCreateKeyExA]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_RegCreateKeyExW]));

        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_RegSetValueA]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_RegSetValueW]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_RegSetValueExA]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_RegSetValueExW]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_RegSetKeyValueA]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_RegSetKeyValueW]));

        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_DeleteFileA]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_DeleteFileW]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_CreateFileA]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_CreateFileW]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_MoveFileA]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_MoveFileW]));

        g_pSetupInfoMgr->DumpAllSetupInfo();
        SAFE_DELETE(g_pSetupInfoMgr);
        {
            FUNCTION_BLOCK_NAME_TRACE_EX(TEXT("ReleaseImage + UnMap"), FTL::TraceDetailExeName, 100);

            API_VERIFY(_ReleaseImgBackground());
        }
        COM_VERIFY(m_FileMap.Unmap());
    }
    return bRet;
}
