#pragma once

#include "HookApi.h"

typedef LONG (WINAPI* RegCloseKeyProc)(HKEY hKey);

typedef LONG (WINAPI* RegOpenKeyAProc)(HKEY hKey, LPCSTR lpSubKey, PHKEY phkResult);
typedef LONG (WINAPI* RegOpenKeyWProc)(HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult);

typedef LONG (WINAPI* RegCreateKeyAProc)(HKEY hKey, LPCSTR lpSubKey, PHKEY phkResult);
typedef LONG (WINAPI* RegCreateKeyWProc)(HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult);

typedef LONG (WINAPI* RegCreateKeyExAProc)(HKEY hKey, LPCSTR lpSubKey, DWORD Reserved,
                                           LPSTR lpClass, DWORD dwOptions, REGSAM samDesired, 
                                           CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                                           PHKEY phkResult, LPDWORD lpdwDisposition);
typedef LONG (WINAPI* RegCreateKeyExWProc)(HKEY hKey, LPCWSTR lpSubKey, DWORD Reserved,
                                           LPWSTR lpClass, DWORD dwOptions, REGSAM samDesired, 
                                           CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                                           PHKEY phkResult, LPDWORD lpdwDisposition);


typedef LONG (WINAPI* RegSetValueAProc)(HKEY hKey, LPCSTR lpSubKey, DWORD dwType, LPCSTR lpData, DWORD cbData);
typedef LONG (WINAPI* RegSetValueWProc)(HKEY hKey, LPCWSTR lpSubKey, DWORD dwType, LPCWSTR lpData, DWORD cbData);

typedef LONG (WINAPI* RegSetValueExAProc)(HKEY hKey, LPCSTR lpValueName, DWORD Reserved, DWORD dwType, CONST BYTE* lpData, DWORD cbData);
typedef LONG (WINAPI* RegSetValueExWProc)(HKEY hKey, LPCWSTR lpValueName, DWORD Reserved, DWORD dwType, CONST BYTE* lpData, DWORD cbData);

typedef LONG (WINAPI* RegSetKeyValueAProc)(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpValueName, 
                                           DWORD dwType, LPCVOID lpData, DWORD cbData);
typedef LONG (WINAPI* RegSetKeyValueWProc)(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpValueName, 
                                           DWORD dwType, LPCVOID lpData, DWORD cbData);

LONG WINAPI Hooked_RegCloseKey(HKEY hKey);

LONG WINAPI Hooked_RegOpenKeyA(HKEY hKey, LPCSTR lpSubKey, PHKEY phkResult);
LONG WINAPI Hooked_RegOpenKeyW(HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult);

LONG WINAPI Hooked_RegCreateKeyA(HKEY hKey, LPCSTR lpSubKey, PHKEY phkResult);
LONG WINAPI Hooked_RegCreateKeyW(HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult);

LONG WINAPI Hooked_RegCreateKeyExA(HKEY hKey, LPCSTR lpSubKey, DWORD Reserved,
                                   LPSTR lpClass, DWORD dwOptions, REGSAM samDesired, 
                                   CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                                   PHKEY phkResult, LPDWORD lpdwDisposition);

LONG WINAPI Hooked_RegCreateKeyExW(HKEY hKey, LPCWSTR lpSubKey, DWORD Reserved,
                                   LPWSTR lpClass, DWORD dwOptions, REGSAM samDesired, 
                                   CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                                   PHKEY phkResult, LPDWORD lpdwDisposition);



LONG WINAPI Hooked_RegSetValueA(HKEY hKey, LPCSTR lpSubKey, DWORD dwType, LPCSTR lpData, DWORD cbData);
LONG WINAPI Hooked_RegSetValueW(HKEY hKey, LPCWSTR lpSubKey, DWORD dwType, LPCWSTR lpData, DWORD cbData);

LONG WINAPI Hooked_RegSetValueExA(HKEY hKey, LPCSTR lpValueName, DWORD Reserved, DWORD dwType, CONST BYTE* lpData, DWORD cbData);
LONG WINAPI Hooked_RegSetValueExW(HKEY hKey, LPCWSTR lpValueName, DWORD Reserved, DWORD dwType, CONST BYTE* lpData, DWORD cbData);

LONG WINAPI Hooked_RegSetKeyValueA(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpValueName, 
                                   DWORD dwType, LPCVOID lpData, DWORD cbData);
LONG WINAPI Hooked_RegSetKeyValueW(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpValueName, 
                                   DWORD dwType, LPCVOID lpData, DWORD cbData);
