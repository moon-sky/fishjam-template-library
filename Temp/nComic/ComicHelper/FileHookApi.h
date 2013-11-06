#pragma once

#include "HookApi.h"

typedef BOOL (WINAPI* DeleteFileAProc)(LPCSTR lpFileName);
typedef BOOL (WINAPI* DeleteFileWProc)(LPCWSTR lpFileName);

typedef HANDLE (WINAPI* CreateFileAProc)(LPCSTR lpFileName, DWORD dwDesiredAccess,
                                     DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                                     DWORD dwCreationDisposition,  DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);

typedef HANDLE (WINAPI* CreateFileWProc)(LPCWSTR lpFileName, DWORD dwDesiredAccess,
                                     DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                                     DWORD dwCreationDisposition,  DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);

typedef BOOL (WINAPI* MoveFileAProc)(LPCSTR lpExistingFileName, LPCSTR lpNewFileName);
typedef BOOL (WINAPI* MoveFileWProc)(LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName);

typedef BOOL (WINAPI* CopyFileAProc)(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, BOOL bFailIfExists);
typedef BOOL (WINAPI* CopyFileWProc)(LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName, BOOL bFailIfExists);

typedef BOOL (WINAPI* CopyFileExAProc)(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, LPPROGRESS_ROUTINE lpProgressRoutine,
    LPVOID lpData, LPBOOL pbCancel, DWORD dwCopyFlags);
typedef BOOL (WINAPI* CopyFileExWProc)(LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName, LPPROGRESS_ROUTINE lpProgressRoutine,
                                       LPVOID lpData, LPBOOL pbCancel, DWORD dwCopyFlags);


BOOL WINAPI Hooked_DeleteFileA(LPCSTR lpFileName);
BOOL WINAPI Hooked_DeleteFileW(LPCWSTR lpFileName);

HANDLE WINAPI Hooked_CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess,
                                 DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                                 DWORD dwCreationDisposition,  DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);

HANDLE WINAPI Hooked_CreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess,
                                 DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                                 DWORD dwCreationDisposition,  DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);

BOOL WINAPI Hooked_MoveFileA(LPCSTR lpExistingFileName, LPCSTR lpNewFileName);
BOOL WINAPI Hooked_MoveFileW(LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName);


BOOL WINAPI Hooked_CopyFileA(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, BOOL bFailIfExists);
BOOL WINAPI Hooked_CopyFileW(LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName, BOOL bFailIfExists);
BOOL WINAPI Hooked_CopyFileExA(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, LPPROGRESS_ROUTINE lpProgressRoutine,
                               LPVOID lpData, LPBOOL pbCancel, DWORD dwCopyFlags);
BOOL WINAPI Hooked_CopyFileExW(LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName, LPPROGRESS_ROUTINE lpProgressRoutine,
                               LPVOID lpData, LPBOOL pbCancel, DWORD dwCopyFlags);

