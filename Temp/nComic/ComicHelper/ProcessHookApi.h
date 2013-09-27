#pragma once
#include "HookApi.h"

typedef HANDLE (WINAPI* OpenProcessProc)(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId);
typedef BOOL (WINAPI* TerminateProcessProc)(HANDLE hProcess, UINT uExitCode);


HANDLE WINAPI Hooked_OpenProcess(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId);
BOOL WINAPI Hooked_TerminateProcess(HANDLE hProcess, UINT uExitCode);

