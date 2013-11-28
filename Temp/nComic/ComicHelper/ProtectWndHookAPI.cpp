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

                //TODO: 先检测操作系统 ?
				//Windows 8 64Bit OS，使用了新的 Kernelbase.dll 代替 Kernel32.dll
                //部分 Reg 的函数在 Kernel32.dll 中，部分在 Advapi32.dll 中
                LPCTSTR pszModuleName[] = {
                    TEXT("Kernelbase.dll"),
                    TEXT("Kernel32.DLL"),
                    TEXT("Advapi32.dll"),
                };
                for (int i = 0; i < _countof(pszModuleName); i++)
                {
                    HMODULE hModuleHook = GetModuleHandle(pszModuleName[i]);
                    if (hModuleHook)
                    {
                        API_VERIFY(HookApiFromModule(hModuleHook, "OpenProcess", &Hooked_OpenProcess, &g_HookApiInfo.HookApiInfos[hft_OpenProcess]));
                        API_VERIFY(HookApiFromModule(hModuleHook, "TerminateProcess", &Hooked_TerminateProcess, &g_HookApiInfo.HookApiInfos[hft_TerminateProcess]));

                        API_VERIFY(HookApiFromModule(hModuleHook, "RegCloseKey", &Hooked_RegCloseKey, &g_HookApiInfo.HookApiInfos[hft_RegCloseKey]));
                        API_VERIFY(HookApiFromModule(hModuleHook, "RegOpenKeyA", &Hooked_RegOpenKeyA, &g_HookApiInfo.HookApiInfos[hft_RegOpenKeyA]));
                        API_VERIFY(HookApiFromModule(hModuleHook, "RegOpenKeyW", &Hooked_RegOpenKeyW, &g_HookApiInfo.HookApiInfos[hft_RegOpenKeyW]));
                        API_VERIFY(HookApiFromModule(hModuleHook, "RegOpenKeyExA", &Hooked_RegOpenKeyExA, &g_HookApiInfo.HookApiInfos[hft_RegOpenKeyExA]));
                        API_VERIFY(HookApiFromModule(hModuleHook, "RegOpenKeyExW", &Hooked_RegOpenKeyExW, &g_HookApiInfo.HookApiInfos[hft_RegOpenKeyExW]));
                        API_VERIFY(HookApiFromModule(hModuleHook, "RegOpenKeyTransactedA", &Hooked_RegOpenKeyTransactedA, &g_HookApiInfo.HookApiInfos[hft_RegOpenKeyTransactedA]));
                        API_VERIFY(HookApiFromModule(hModuleHook, "RegOpenKeyTransactedW", &Hooked_RegOpenKeyTransactedW, &g_HookApiInfo.HookApiInfos[hft_RegOpenKeyTransactedW]));
                        API_VERIFY(HookApiFromModule(hModuleHook, "RegOpenCurrentUser", &Hooked_RegOpenCurrentUser, &g_HookApiInfo.HookApiInfos[hft_RegOpenCurrentUser]));
                        API_VERIFY(HookApiFromModule(hModuleHook, "RegOpenUserClassesRoot", &Hooked_RegOpenUserClassesRoot, &g_HookApiInfo.HookApiInfos[hft_RegOpenUserClassesRoot]));

                        API_VERIFY(HookApiFromModule(hModuleHook, "RegConnectRegistryA", &Hooked_RegConnectRegistryA, &g_HookApiInfo.HookApiInfos[hft_RegConnectRegistryA]));
                        API_VERIFY(HookApiFromModule(hModuleHook, "RegConnectRegistryW", &Hooked_RegConnectRegistryW, &g_HookApiInfo.HookApiInfos[hft_RegConnectRegistryW]));

                        API_VERIFY(HookApiFromModule(hModuleHook, "RegCreateKeyA", &Hooked_RegCreateKeyA, &g_HookApiInfo.HookApiInfos[hft_RegCreateKeyA]));
                        API_VERIFY(HookApiFromModule(hModuleHook, "RegCreateKeyW", &Hooked_RegCreateKeyW, &g_HookApiInfo.HookApiInfos[hft_RegCreateKeyW]));
                        API_VERIFY(HookApiFromModule(hModuleHook, "RegCreateKeyExA", &Hooked_RegCreateKeyExA, &g_HookApiInfo.HookApiInfos[hft_RegCreateKeyExA]));
                        API_VERIFY(HookApiFromModule(hModuleHook, "RegCreateKeyExW", &Hooked_RegCreateKeyExW, &g_HookApiInfo.HookApiInfos[hft_RegCreateKeyExW]));

                        API_VERIFY(HookApiFromModule(hModuleHook, "RegCreateKeyTransactedA", &Hooked_RegCreateKeyTransactedA, &g_HookApiInfo.HookApiInfos[hft_RegCreateKeyTransactedA]));
                        API_VERIFY(HookApiFromModule(hModuleHook, "RegCreateKeyTransactedW", &Hooked_RegCreateKeyTransactedW, &g_HookApiInfo.HookApiInfos[hft_RegCreateKeyTransactedW]));

                        API_VERIFY(HookApiFromModule(hModuleHook, "RegSetValueA", &Hooked_RegSetValueA, &g_HookApiInfo.HookApiInfos[hft_RegSetValueA]));
                        API_VERIFY(HookApiFromModule(hModuleHook, "RegSetValueW", &Hooked_RegSetValueW, &g_HookApiInfo.HookApiInfos[hft_RegSetValueW]));
                        API_VERIFY(HookApiFromModule(hModuleHook, "RegSetValueExA", &Hooked_RegSetValueExA, &g_HookApiInfo.HookApiInfos[hft_RegSetValueExA]));
                        API_VERIFY(HookApiFromModule(hModuleHook, "RegSetValueExW", &Hooked_RegSetValueExW, &g_HookApiInfo.HookApiInfos[hft_RegSetValueExW]));

                        API_VERIFY(HookApiFromModule(hModuleHook, "RegSetKeyValueA", &Hooked_RegSetKeyValueA, &g_HookApiInfo.HookApiInfos[hft_RegSetKeyValueA]));
                        API_VERIFY(HookApiFromModule(hModuleHook, "RegSetKeyValueW", &Hooked_RegSetKeyValueW, &g_HookApiInfo.HookApiInfos[hft_RegSetKeyValueW]));

                        API_VERIFY(HookApiFromModule(hModuleHook, "DeleteFileA", &Hooked_DeleteFileA, &g_HookApiInfo.HookApiInfos[hft_DeleteFileA]));
                        API_VERIFY(HookApiFromModule(hModuleHook, "DeleteFileW", &Hooked_DeleteFileW, &g_HookApiInfo.HookApiInfos[hft_DeleteFileW]));
                        API_VERIFY(HookApiFromModule(hModuleHook, "CreateFileA", &Hooked_CreateFileA, &g_HookApiInfo.HookApiInfos[hft_CreateFileA]));
                        API_VERIFY(HookApiFromModule(hModuleHook, "CreateFileW", &Hooked_CreateFileW, &g_HookApiInfo.HookApiInfos[hft_CreateFileW]));
                        API_VERIFY(HookApiFromModule(hModuleHook, "MoveFileA", &Hooked_MoveFileA, &g_HookApiInfo.HookApiInfos[hft_MoveFileA]));
                        API_VERIFY(HookApiFromModule(hModuleHook, "MoveFileW", &Hooked_MoveFileW, &g_HookApiInfo.HookApiInfos[hft_MoveFileW]));
                        API_VERIFY(HookApiFromModule(hModuleHook, "CopyFileA", &Hooked_CopyFileA, &g_HookApiInfo.HookApiInfos[hft_CopyFileA]));
                        API_VERIFY(HookApiFromModule(hModuleHook, "CopyFileW", &Hooked_CopyFileW, &g_HookApiInfo.HookApiInfos[hft_CopyFileW]));
                        API_VERIFY(HookApiFromModule(hModuleHook, "CopyFileExA", &Hooked_CopyFileExA, &g_HookApiInfo.HookApiInfos[hft_CopyFileExA]));
                        API_VERIFY(HookApiFromModule(hModuleHook, "CopyFileExW", &Hooked_CopyFileExW, &g_HookApiInfo.HookApiInfos[hft_CopyFileExW]));
                    }
                }

                for (int hookIndex = hft_First + 1; hookIndex < hft_FunctionCount; hookIndex++)
                {
                    FTLASSERT(g_HookApiInfo.HookApiInfos[hookIndex] != NULL);
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

    FTLTRACE(TEXT("<<< Will UnHook API(g_bHooked=%d) in PID=%d(%s),TID=%d\n"), 
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
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_RegOpenKeyExA]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_RegOpenKeyExW]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_RegOpenKeyTransactedA]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_RegOpenKeyTransactedW]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_RegOpenCurrentUser]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_RegOpenUserClassesRoot]));

        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_RegConnectRegistryA]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_RegConnectRegistryW]));

        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_RegCreateKeyA]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_RegCreateKeyW]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_RegCreateKeyExA]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_RegCreateKeyExW]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_RegCreateKeyTransactedA]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_RegCreateKeyTransactedW]));

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
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_CopyFileA]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_CopyFileW]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_CopyFileExA]));
        API_VERIFY(RestoreInlineHook(g_HookApiInfo.HookApiInfos[hft_CopyFileExW]));
        
        g_pSetupInfoMgr->GetAllSetupInfo((HWND)g_pProtectWndInfoFileMap->hWndProtect);

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
