#pragma once

#include "HookApi.h"

typedef LONG (WINAPI* RegOpenKeyAProc)(HKEY hKey, LPCSTR lpSubKey, PHKEY phkResult);
typedef LONG (WINAPI* RegOpenKeyWProc)(HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult);

typedef LONG (WINAPI* RegSetKeyValueAProc)(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpValueName, 
                                           DWORD dwType, LPCVOID lpData, DWORD cbData);
typedef LONG (WINAPI* RegSetKeyValueWProc)(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpValueName, 
                                           DWORD dwType, LPCVOID lpData, DWORD cbData);


LONG WINAPI Hooked_RegOpenKeyA(HKEY hKey, LPCSTR lpSubKey, PHKEY phkResult);
LONG WINAPI Hooked_RegOpenKeyW(HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult);

LONG WINAPI Hooked_RegSetKeyValueA(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpValueName, 
                                   DWORD dwType, LPCVOID lpData, DWORD cbData);
LONG WINAPI Hooked_RegSetKeyValueW(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpValueName, 
                                   DWORD dwType, LPCVOID lpData, DWORD cbData);
