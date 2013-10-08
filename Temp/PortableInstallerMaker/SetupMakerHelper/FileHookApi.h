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
