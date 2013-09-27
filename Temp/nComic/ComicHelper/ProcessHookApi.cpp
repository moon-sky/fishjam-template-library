#include "stdafx.h"
#include "ProcessHookApi.h"
#include "../ComicHelperProxy/ComicHelperProxy.h"

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
            FTLTRACE(TEXT("In Hooked_OpenProcess dwProcessId=%d, hProcess=0x%x\n"), dwProcessId, hProcess);
            if (dwProcessId == g_pProtectWndInfoFileMap->dwProtectProcessId && hProcess)
            {
                FTLTRACE(TEXT("!!!In Hooked_OpenProcess hProtectProcessInCurProcess=0x%x\n"), hProcess);
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