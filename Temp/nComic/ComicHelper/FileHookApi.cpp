#include "stdafx.h"
#include "FileHookApi.h"
#include "SetupInfoMgr.h"

static DWORD s_dwDesiredAccessFilter = FILE_APPEND_DATA | FILE_WRITE_DATA;
static DWORD s_dwCreationDispositionFilter = 0;

BOOL CheckWillWrite(DWORD dwDesiredAccess)
{
    return TRUE; //((dwDesiredAccess & s_dwCreationDispositionFilter) != 0);
}


BOOL WINAPI Hooked_DeleteFileA(LPCSTR lpFileName)
{
    BOOL bRet = FALSE;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);

    LPCTSTR pszTCharFileName = CFConversion().MBCS_TO_TCHAR(lpFileName);
    DeleteFileAProc pOrigDeleteFileA = (DeleteFileAProc)g_HookApiInfo.HookApiInfos[hft_DeleteFileA]->pOriginal;
    if (pOrigDeleteFileA)
    {
        bRet = pOrigDeleteFileA(lpFileName);
        if (bRet)
        {
            FTLTRACE(TEXT("!!! Hooked_DeleteFileA, lpFileName=%s\n"), pszTCharFileName);
            g_pSetupInfoMgr->DeleteSetupFileInfo(pszTCharFileName);
        }
    }
    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);

    return bRet;
}
BOOL WINAPI Hooked_DeleteFileW(LPCWSTR lpFileName)
{
    BOOL bRet = FALSE;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);

    DeleteFileWProc pOrigDeleteFileW = (DeleteFileWProc)g_HookApiInfo.HookApiInfos[hft_DeleteFileW]->pOriginal;
    if (pOrigDeleteFileW)
    {
        bRet = pOrigDeleteFileW(lpFileName);
        if (bRet)
        {
            FTLTRACE(TEXT("!!! Hooked_DeleteFileW, lpFileName=%s\n"), lpFileName);
            g_pSetupInfoMgr->DeleteSetupFileInfo(lpFileName);
        }
    }
    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);

    return bRet;
}

HANDLE WINAPI Hooked_CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess,
                                 DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                                 DWORD dwCreationDisposition,  DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
    HANDLE hFileReturn = NULL;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);

    LPCTSTR pszTCharFileName = CFConversion().MBCS_TO_TCHAR(lpFileName);

    CreateFileAProc pOrigCreateFileA = (CreateFileAProc)g_HookApiInfo.HookApiInfos[hft_CreateFileA]->pOriginal;
    if (pOrigCreateFileA)
    {
        hFileReturn = pOrigCreateFileA(lpFileName, dwDesiredAccess, 
            dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
        if (INVALID_HANDLE_VALUE != hFileReturn && CheckWillWrite(dwDesiredAccess))
        {
            FTLTRACE(TEXT("!!! Hooked_CreateFileA,lpFileName=%s\n"), pszTCharFileName);
            g_pSetupInfoMgr->SetSetupFileInfo(pszTCharFileName, NULL);
        }
    }
    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);

    return hFileReturn;
}

HANDLE WINAPI Hooked_CreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess,
                                 DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                                 DWORD dwCreationDisposition,  DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
    HANDLE hFileReturn = NULL;

    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);
    CreateFileWProc pOrigCreateFileW = (CreateFileWProc)g_HookApiInfo.HookApiInfos[hft_CreateFileW]->pOriginal;
    if (pOrigCreateFileW)
    {
        hFileReturn = pOrigCreateFileW(lpFileName, dwDesiredAccess, 
            dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
        if (INVALID_HANDLE_VALUE != hFileReturn && CheckWillWrite(dwDesiredAccess))
        {
            FTLTRACE(TEXT("!!! Hooked_CreateFileW, lpFileName=%s\n"), lpFileName);
            g_pSetupInfoMgr->SetSetupFileInfo(lpFileName, NULL);
        }
    }
    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);

    return hFileReturn;
}


BOOL WINAPI Hooked_MoveFileA(LPCSTR lpExistingFileName, LPCSTR lpNewFileName)
{
    BOOL bRet = FALSE;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);
    MoveFileAProc pOrigMoveFileA = (MoveFileAProc)g_HookApiInfo.HookApiInfos[hft_MoveFileA]->pOriginal;
    if (pOrigMoveFileA)
    {
        bRet = pOrigMoveFileA(lpExistingFileName, lpNewFileName); 
        if (bRet)
        {
            LPCTSTR pszTCharExistingFileName = CFConversion().MBCS_TO_TCHAR(lpExistingFileName);
            LPCTSTR pszTCharNewFileName = CFConversion().MBCS_TO_TCHAR(lpNewFileName);
            FTLTRACE(TEXT("!!! Hooked_MoveFileA, %s=>%s\n"), 
                pszTCharExistingFileName, pszTCharNewFileName);

            g_pSetupInfoMgr->SetSetupFileInfo(pszTCharExistingFileName, pszTCharNewFileName);
        }
    }
    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);

    return bRet;
}

BOOL WINAPI Hooked_MoveFileW(LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName)
{
    BOOL bRet = FALSE;

    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);
    MoveFileWProc pOrigMoveFileW = (MoveFileWProc)g_HookApiInfo.HookApiInfos[hft_MoveFileW]->pOriginal;
    if (pOrigMoveFileW)
    {
        bRet = pOrigMoveFileW(lpExistingFileName, lpNewFileName); 
        if (bRet)
        {
            FTLTRACE(TEXT("!!! Hooked_MoveFileW, %s=>%s\n"), lpExistingFileName, lpNewFileName);
            g_pSetupInfoMgr->SetSetupFileInfo(lpExistingFileName, lpNewFileName);
        }
    }
    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);

    return bRet;
}
