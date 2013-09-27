#include "stdafx.h"
#include "FileHookApi.h"
#include "SetupInfoMgr.h"

HANDLE WINAPI Hooked_CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess,
                                 DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                                 DWORD dwCreationDisposition,  DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
    HANDLE hFileReturn = NULL;
    FTLTRACE(TEXT("!!! Hooked_CreateFileA\n"));

    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);
    __try
    {
        CreateFileAProc pOrigCreateFileA = (CreateFileAProc)g_HookApiInfo.HookApiInfos[hft_CreateFileA]->pOriginal;
        if (pOrigCreateFileA)
        {
            hFileReturn = pOrigCreateFileA(lpFileName, dwDesiredAccess, 
                dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
        }
    }
    __finally
    {
        HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);
    }

    return hFileReturn;
}

HANDLE WINAPI Hooked_CreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess,
                                 DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                                 DWORD dwCreationDisposition,  DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
    HANDLE hFileReturn = NULL;
    FTLTRACE(TEXT("!!! Hooked_CreateFileW, lpFileName=%s\n"), lpFileName);
    g_pSetupInfoMgr->SetSetupInfo(hsit_NewFile, lpFileName, NULL);

    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);
    __try
    {
        CreateFileWProc pOrigCreateFileW = (CreateFileWProc)g_HookApiInfo.HookApiInfos[hft_CreateFileW]->pOriginal;
        if (pOrigCreateFileW)
        {
            hFileReturn = pOrigCreateFileW(lpFileName, dwDesiredAccess, 
                dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
        }
    }
    __finally
    {
        HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);
    }

    return hFileReturn;
}


BOOL WINAPI Hooked_MoveFileA(LPCSTR lpExistingFileName, LPCSTR lpNewFileName)
{
    BOOL bRet = FALSE;
    //FTLTRACE(TEXT("!!! Hooked_MoveFileA, %s=>%s\n"), lpExistingFileName, lpNewFileName);

    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);
    __try
    {
        MoveFileAProc pOrigMoveFileA = (MoveFileAProc)g_HookApiInfo.HookApiInfos[hft_MoveFileA]->pOriginal;
        if (pOrigMoveFileA)
        {
            bRet = pOrigMoveFileA(lpExistingFileName, lpNewFileName); 
        }
    }
    __finally
    {
        HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);
    }

    return bRet;
}

BOOL WINAPI Hooked_MoveFileW(LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName)
{
    BOOL bRet = FALSE;
    FTLTRACE(TEXT("!!! Hooked_MoveFileW, %s=>%s\n"), lpExistingFileName, lpNewFileName);
    g_pSetupInfoMgr->SetSetupInfo(hsit_NewFile, lpExistingFileName, lpNewFileName);

    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);
    __try
    {
        MoveFileWProc pOrigMoveFileW = (MoveFileWProc)g_HookApiInfo.HookApiInfos[hft_MoveFileW]->pOriginal;
        if (pOrigMoveFileW)
        {
            bRet = pOrigMoveFileW(lpExistingFileName, lpNewFileName); 
        }
    }
    __finally
    {
        HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);
    }

    return bRet;
}
