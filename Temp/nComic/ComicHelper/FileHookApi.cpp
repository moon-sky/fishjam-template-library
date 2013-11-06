#include "stdafx.h"
#include "FileHookApi.h"
#include "SetupInfoMgr.h"

static DWORD s_dwDesiredAccessFilter = GENERIC_WRITE | GENERIC_ALL;
//static DWORD s_dwCreationDispositionFilter = 0;

BOOL CheckWillWrite(DWORD dwDesiredAccess)
{
    return ((dwDesiredAccess & s_dwDesiredAccessFilter) != 0);
}

BOOL WINAPI Hooked_DeleteFileA(LPCSTR lpFileName)
{
    BOOL bRet = FALSE;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);

    DeleteFileAProc pOrigDeleteFileA = (DeleteFileAProc)g_HookApiInfo.HookApiInfos[hft_DeleteFileA]->pOriginal;
    if (pOrigDeleteFileA)
    {
        bRet = pOrigDeleteFileA(lpFileName);
        if (bRet)
        {
            CFConversion conv;
            LPCTSTR pszTCharFileName = conv.MBCS_TO_TCHAR(lpFileName);
            FTLTRACEEX(tlInfo, TEXT("!!! Hooked_DeleteFileA, lpFileName=%s\n"), pszTCharFileName);
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
            CFConversion conv;
            LPCTSTR pszTCharFileName = conv.UTF16_TO_TCHAR(lpFileName);
            FTLTRACE(TEXT("!!! Hooked_DeleteFileW, lpFileName=%s\n"), pszTCharFileName);
            g_pSetupInfoMgr->DeleteSetupFileInfo(pszTCharFileName);
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

    CreateFileAProc pOrigCreateFileA = (CreateFileAProc)g_HookApiInfo.HookApiInfos[hft_CreateFileA]->pOriginal;
    if (pOrigCreateFileA)
    {
        hFileReturn = pOrigCreateFileA(lpFileName, dwDesiredAccess, 
            dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
        if (INVALID_HANDLE_VALUE != hFileReturn && CheckWillWrite(dwDesiredAccess))
        {
            CFConversion conv;
            LPCTSTR pszTCharFileName = conv.MBCS_TO_TCHAR(lpFileName);
            FTLTRACEEX(tlInfo, TEXT("!!! Hooked_CreateFileA,lpFileName=%s\n"), pszTCharFileName);
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
            CFConversion conv;
            LPCTSTR pszTCharFileName = conv.UTF16_TO_TCHAR(lpFileName);
            FTLTRACEEX(tlInfo, TEXT("!!! Hooked_CreateFileW, lpFileName=%s,dwDesiredAccess=0x%x, dwShareMode=0x%x, CreateDisp=0x%x\n"), 
                pszTCharFileName, dwDesiredAccess, dwShareMode, dwCreationDisposition);
            g_pSetupInfoMgr->SetSetupFileInfo(pszTCharFileName, NULL);
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
            CFConversion convExistFileName;
            CFConversion convNewFileName;
            LPCTSTR pszTCharExistingFileName = convExistFileName.MBCS_TO_TCHAR(lpExistingFileName);
            LPCTSTR pszTCharNewFileName = convNewFileName.MBCS_TO_TCHAR(lpNewFileName);
            FTLTRACEEX(tlInfo, TEXT("!!! Hooked_MoveFileA, %s=>%s\n"), 
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
            CFConversion convExistFileName;
            CFConversion convNewFileName;
            LPCTSTR pszTCharExistingFileName = convExistFileName.UTF16_TO_TCHAR(lpExistingFileName);
            LPCTSTR pszTCharNewFileName = convNewFileName.UTF16_TO_TCHAR(lpNewFileName);

            FTLTRACEEX(tlInfo, TEXT("!!! Hooked_MoveFileW, %s=>%s\n"), pszTCharExistingFileName, pszTCharNewFileName);
            g_pSetupInfoMgr->SetSetupFileInfo(pszTCharExistingFileName, pszTCharNewFileName);
        }
    }
    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);

    return bRet;
}


BOOL WINAPI Hooked_CopyFileA(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, BOOL bFailIfExists)
{
    BOOL bRet = FALSE;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);

    CopyFileAProc pOrigCopyFileA = (CopyFileAProc)g_HookApiInfo.HookApiInfos[hft_CopyFileA]->pOriginal;
    if (pOrigCopyFileA)
    {
        bRet = pOrigCopyFileA(lpExistingFileName, lpNewFileName, bFailIfExists); 
        if (bRet)
        {
            CFConversion convExistFileName;
            CFConversion convNewFileName;
            LPCTSTR pszTCharExistingFileName = convExistFileName.MBCS_TO_TCHAR(lpExistingFileName);
            LPCTSTR pszTCharNewFileName = convNewFileName.MBCS_TO_TCHAR(lpNewFileName);
            FTLTRACEEX(tlInfo, TEXT("!!! Hooked_CopyFileA, %s=>%s\n"), 
                pszTCharExistingFileName, pszTCharNewFileName);

            g_pSetupInfoMgr->SetSetupFileInfo(pszTCharNewFileName, NULL);
        }
    }
    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);

    return bRet;
}

BOOL WINAPI Hooked_CopyFileW(LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName, BOOL bFailIfExists)
{
    BOOL bRet = FALSE;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);

    CopyFileWProc pOrigCopyFileW = (CopyFileWProc)g_HookApiInfo.HookApiInfos[hft_CopyFileW]->pOriginal;
    if (pOrigCopyFileW)
    {
        bRet = pOrigCopyFileW(lpExistingFileName, lpNewFileName, bFailIfExists); 
        if (bRet)
        {
            CFConversion convExistFileName;
            CFConversion convNewFileName;
            LPCTSTR pszTCharExistingFileName = convExistFileName.UTF16_TO_TCHAR(lpExistingFileName);
            LPCTSTR pszTCharNewFileName = convNewFileName.UTF16_TO_TCHAR(lpNewFileName);
            FTLTRACEEX(tlInfo, TEXT("!!! Hooked_CopyFileW, %s=>%s\n"), 
                pszTCharExistingFileName, pszTCharNewFileName);

            g_pSetupInfoMgr->SetSetupFileInfo(pszTCharNewFileName, NULL);
        }
    }
    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);

    return bRet;
}

BOOL WINAPI Hooked_CopyFileExA(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, LPPROGRESS_ROUTINE lpProgressRoutine,
                               LPVOID lpData, LPBOOL pbCancel, DWORD dwCopyFlags)
{
    BOOL bRet = FALSE;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);

    CopyFileExAProc pOrigCopyFileExA = (CopyFileExAProc)g_HookApiInfo.HookApiInfos[hft_CopyFileExA]->pOriginal;
    if (pOrigCopyFileExA)
    {
        bRet = pOrigCopyFileExA(lpExistingFileName, lpNewFileName, lpProgressRoutine, lpData, pbCancel, dwCopyFlags); 
        if (bRet)
        {
            CFConversion convExistFileName;
            CFConversion convNewFileName;
            LPCTSTR pszTCharExistingFileName = convExistFileName.MBCS_TO_TCHAR(lpExistingFileName);
            LPCTSTR pszTCharNewFileName = convNewFileName.MBCS_TO_TCHAR(lpNewFileName);
            FTLTRACEEX(tlInfo, TEXT("!!! Hooked_CopyFileExA, %s=>%s\n"), 
                pszTCharExistingFileName, pszTCharNewFileName);

            g_pSetupInfoMgr->SetSetupFileInfo(pszTCharNewFileName, NULL);
        }
    }
    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);

    return bRet;
}
BOOL WINAPI Hooked_CopyFileExW(LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName, LPPROGRESS_ROUTINE lpProgressRoutine,
                               LPVOID lpData, LPBOOL pbCancel, DWORD dwCopyFlags)
{
    BOOL bRet = FALSE;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);

    CopyFileExWProc pOrigCopyFileExW = (CopyFileExWProc)g_HookApiInfo.HookApiInfos[hft_CopyFileExW]->pOriginal;
    if (pOrigCopyFileExW)
    {
        bRet = pOrigCopyFileExW(lpExistingFileName, lpNewFileName, lpProgressRoutine, lpData, pbCancel, dwCopyFlags); 
        if (bRet)
        {
            CFConversion convExistFileName;
            CFConversion convNewFileName;
            LPCTSTR pszTCharExistingFileName = convExistFileName.UTF16_TO_TCHAR(lpExistingFileName);
            LPCTSTR pszTCharNewFileName = convNewFileName.UTF16_TO_TCHAR(lpNewFileName);
            FTLTRACEEX(tlInfo, TEXT("!!! Hooked_CopyFileExW, %s=>%s\n"), 
                pszTCharExistingFileName, pszTCharNewFileName);

            g_pSetupInfoMgr->SetSetupFileInfo(pszTCharNewFileName, NULL);
        }
    }
    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);

    return bRet;
}
