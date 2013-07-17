// ComicHelper.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ComicHelper.h"
#include <atlbase.h>
#include "detours.h"

#define API_VERIFY(x)	bRet = (x); if(!bRet){ ATLTRACE(TEXT("Error:%d, %s"), GetLastError(), TEXT(#x)); }

static LONG dwSlept = 0;
static DWORD (WINAPI * TrueSleepEx)(DWORD dwMilliseconds, BOOL bAlertable) = SleepEx;

#pragma data_seg("MyShare")
HWND		g_hFilterWnd = NULL;
COLORREF	g_clrDisabled = RGB(127, 127, 127);
#pragma data_seg()
#pragma comment(linker,"/SECTION:MyShare,RWS")

HHOOK g_hHook = NULL;
BOOL  g_Hooked = FALSE;
HMODULE g_hModule = NULL;

static BOOL (WINAPI* TrueBitBlt)(HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop) = BitBlt;
static HANDLE (WINAPI* TrueCreateFileW)(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, 
				LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, 
				DWORD dwFlagsAndAttributes,HANDLE hTemplateFile)
				= CreateFileW;

static HANDLE (WINAPI* TrueCreateFileA)(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, 
			   LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, 
			   DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
			   = CreateFileA;


UINT UM_HELPER_HOOK = RegisterWindowMessage(TEXT("UM_HELPER_HOOK"));

LPCWSTR pszSystemSnapFilePathW = L"C:\\Windows\\System32\\SnapShot";
LPCSTR pszSystemSnapFilePathA = "C:\\Windows\\System32\\SnapShot";

BOOL WINAPI FilterBitBlt(HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop)
{
	//ATLTRACE(TEXT("Enter FilterBitBlt\n"));
	BOOL bRet = TrueBitBlt(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop);
	if (bRet)
	{
		HWND hWnd = WindowFromDC(hdcSrc);
		if (hWnd != NULL)
		{
			if (hWnd == GetDesktopWindow() || hWnd == g_hFilterWnd || ::IsChild(g_hFilterWnd, hWnd))
			{
				TCHAR szProcessName[MAX_PATH] = {0};
				GetModuleFileName(NULL, szProcessName, _countof(szProcessName));

				ATLTRACE(TEXT("FilterBitBlt, PID=%d(%s), hWnd=%d, g_hFilterWnd=%d\n"),
					GetCurrentProcessId(), PathFindFileName(szProcessName), hWnd, g_hFilterWnd);

				RECT rcTarget = { x, y, cx, cy };
				RECT rcFilter = {0};
				GetWindowRect(g_hFilterWnd, &rcFilter);
				ATLTRACE(TEXT("rcTarget=(%d,%d)-(%d,%d), %dx%d, rcFilter=(%d,%d)-(%d,%d), %dx%d"),
					rcTarget.left, rcTarget.top, rcTarget.right, rcTarget.bottom, 
					rcTarget.right - rcTarget.left, rcTarget.bottom - rcTarget.top,
					rcFilter.left, rcFilter.top, rcFilter.right, rcFilter.bottom, 
					rcFilter.right - rcFilter.left, rcFilter.bottom - rcFilter.top
					);

				IntersectRect(&rcFilter, &rcTarget, &rcFilter);
				if (!::IsRectEmpty(&rcFilter))
				{
					::SetBkColor(hdc, g_clrDisabled);
					::ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rcFilter, NULL, 0, NULL);
				}
			}
		}
	}
	return bRet;
}

HANDLE WINAPI FilterCreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, 
										LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, 
										DWORD dwFlagsAndAttributes,HANDLE hTemplateFile)
{
	if (StrCmpICW(lpFileName, pszSystemSnapFilePathW) == 0)
	{
		SetLastError(ERROR_ACCESS_DENIED);
		return NULL;
	}
	return TrueCreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition,
		dwFlagsAndAttributes, hTemplateFile);
}

HANDLE WINAPI FilterCreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, 
										LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, 
										DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	if (StrCmpICA(lpFileName, pszSystemSnapFilePathA) == 0)
	{
		SetLastError(ERROR_ACCESS_DENIED);
		return NULL;
	}

	return TrueCreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition,
		dwFlagsAndAttributes, hTemplateFile);

}


LRESULT CALLBACK My_CallWndProc(
								_In_  int nCode,
								_In_  WPARAM wParam,
								_In_  LPARAM lParam
							 )
{
	CWPSTRUCT * pWPStruct = (CWPSTRUCT*)lParam;
	if (!g_Hooked && pWPStruct)// && pWPStruct->message == UM_HELPER_HOOK)
	{
        TCHAR szModuleName[MAX_PATH] = {0};
        GetModuleFileName(NULL, szModuleName, _countof(szModuleName));
		ATLTRACE(TEXT("Will Hook API in PID=%d(%s),TID=%d\n"), GetCurrentProcessId(), PathFindFileName(szModuleName), GetCurrentThreadId());

		BOOL bRet = FALSE;
		g_Hooked = TRUE;
		DetourRestoreAfterWith();
		API_VERIFY(DetourTransactionBegin() == NO_ERROR);
		API_VERIFY(DetourUpdateThread(GetCurrentThread()) == NO_ERROR);
		//API_VERIFY(DetourAttach(&(PVOID&)TrueSleepEx, TimedSleepEx) == NO_ERROR);
		API_VERIFY(DetourAttach(&(PVOID&)TrueBitBlt, FilterBitBlt) == NO_ERROR);
		API_VERIFY(DetourAttach(&(PVOID&)TrueCreateFileW, FilterCreateFileW) == NO_ERROR);
		API_VERIFY(DetourAttach(&(PVOID&)TrueCreateFileA, FilterCreateFileA) == NO_ERROR);
		API_VERIFY(DetourTransactionCommit() == NO_ERROR);
	}
	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}


COMICHELPER_API BOOL SetApiHook(HWND hWndFilter, COLORREF clrDisabled)
{
	ATLTRACE(TEXT("Enter SetApiHook for hWndFilter=0x%x\n"), hWndFilter);
	BOOL bRet = FALSE;
	g_hFilterWnd = hWndFilter;
	g_clrDisabled = clrDisabled;
	if (NULL == g_hHook)
	{
		ATLTRACE(TEXT("Enter SetApiHook  for 0x%x\n"), hWndFilter);

		//UM_HELPER_HOOK = RegisterWindowMessage(TEXT("UM_HELPER_HOOK"));
		g_hHook = SetWindowsHookEx(WH_CALLWNDPROC, My_CallWndProc, g_hModule, 0);
	}

	ATLTRACE(TEXT("Leave SetApiHook g_hHook=0x%x, bRet=%d\n"), g_hHook, bRet);
	return bRet;
}

COMICHELPER_API BOOL SetApiUnHook()
{
	BOOL bRet = TRUE;

	if (g_hHook)
	{
		bRet = UnhookWindowsHookEx(g_hHook);
		g_hHook = NULL;
		UnHookApi();
	}
	return bRet;
}

COMICHELPER_API BOOL UnHookApi()
{
	ATLTRACE(TEXT("Will UnHookApi in PID=%d, TID=%d\n"), GetCurrentProcessId(), GetCurrentThreadId());

	BOOL bRet = FALSE;
	API_VERIFY(DetourTransactionBegin()== NO_ERROR);
	API_VERIFY(DetourUpdateThread(GetCurrentThread()) == NO_ERROR);
	API_VERIFY(DetourDetach(&(PVOID&)TrueBitBlt, FilterBitBlt) == NO_ERROR);
	API_VERIFY(DetourDetach(&(PVOID&)TrueCreateFileW, FilterCreateFileW) == NO_ERROR);
	API_VERIFY(DetourDetach(&(PVOID&)TrueCreateFileA, FilterCreateFileA) == NO_ERROR);
	API_VERIFY(DetourTransactionCommit() == NO_ERROR);

	return bRet;
}

