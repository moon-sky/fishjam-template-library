#pragma once

#include "HookApi.h"

typedef BOOL (WINAPI *BitBltProc)(HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight,
                                  HDC hdcSrc, int nXSrc, int nYSrc, DWORD dwRop);
typedef BOOL (WINAPI *StretchBltProc)(HDC hdcDest, int nXDest, int nYDest, int nWidthDest, int nHeightDest,
                                      HDC hdcSrc, int nXSrc, int nYSrc, int nWidthSrc, int nHeightSrc, 
                                      DWORD dwRop);

typedef HDC (WINAPI* CreateDCWProc)(LPCWSTR pwszDriver, LPCWSTR pwszDevice, LPCWSTR pwszOutput, CONST DEVMODEW* lpInitData);
typedef HDC (WINAPI* CreateDCAProc)(LPCSTR pszDriver, LPCSTR pszDevice, LPCSTR pszPort, CONST DEVMODEA * pdm);
typedef BOOL (WINAPI* DeleteDCProc)(HDC hdc);

BOOL HookApiFromModule(HMODULE hModule, LPCSTR lpProcName, PVOID pDetour, PINLINE_HOOK_INFO* ppOutHookInfo);
BOOL IsFilterHDC(HDC hDC);


BOOL WINAPI Hooked_BitBlt(HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight,
                          HDC hdcSrc, int nXSrc, int nYSrc, DWORD dwRop);
BOOL WINAPI Hooked_StretchBlt(HDC hdcDest, int nXDest, int nYDest, int nWidthDest, int nHeightDest,
                              HDC hdcSrc, int nXSrc, int nYSrc, int nWidthSrc, int nHeightSrc, 
                              DWORD dwRop);
HDC WINAPI Hooked_CreateDCW(LPCWSTR pwszDriver, LPCWSTR pwszDevice, LPCWSTR pwszOutput, CONST DEVMODEW* lpInitData);
HDC WINAPI Hooked_CreateDCA(LPCSTR pszDriver, LPCSTR pszDevice, LPCSTR pszPort, CONST DEVMODEA * pdm);
BOOL WINAPI Hooked_DeleteDC(HDC hdc);

BOOL WINAPI Hooked_DeleteDC(HDC hdc);
