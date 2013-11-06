#pragma once

#include "HookApi.h"

typedef LONG (WINAPI* RegCloseKeyProc)(HKEY hKey);


typedef LONG (WINAPI* RegOpenKeyAProc)(HKEY hKey, LPCSTR lpSubKey, PHKEY phkResult);
typedef LONG (WINAPI* RegOpenKeyWProc)(HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult);

typedef LONG (WINAPI* RegOpenKeyExAProc)(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions,
                                         REGSAM samDesired, PHKEY phkResult);
typedef LONG (WINAPI* RegOpenKeyExWProc)(HKEY hKey, LPCWSTR lpSubKey, DWORD ulOptions,
                                         REGSAM samDesired, PHKEY phkResult);

typedef LONG (WINAPI* RegOpenKeyTransactedAProc)(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions,
    REGSAM samDesired, PHKEY phkResult, HANDLE hTransaction, PVOID pExtendedParemeter);

typedef LONG (WINAPI* RegOpenKeyTransactedWProc)(HKEY hKey, LPCWSTR lpSubKey, DWORD ulOptions,
    REGSAM samDesired, PHKEY phkResult, HANDLE hTransaction, PVOID  pExtendedParemeter);

typedef LONG (WINAPI* RegOpenCurrentUserProc)(REGSAM samDesired, PHKEY phkResult);

typedef LONG (WINAPI* RegOpenUserClassesRootProc)(HANDLE hToken, DWORD dwOptions, REGSAM samDesired, PHKEY phkResult);

typedef LONG (WINAPI* RegConnectRegistryAProc)(LPCSTR lpMachineName, HKEY hKey, PHKEY phkResult);
typedef LONG (WINAPI* RegConnectRegistryWProc)(LPCWSTR lpMachineName, HKEY hKey, PHKEY phkResult);

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

typedef LONG (WINAPI* RegCreateKeyTransactedAProc)(HKEY hKey, LPCSTR lpSubKey, DWORD Reserved,
    LPSTR lpClass, DWORD dwOptions, REGSAM samDesired, CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    PHKEY phkResult, LPDWORD lpdwDisposition, HANDLE hTransaction, PVOID  pExtendedParemeter);

typedef LONG (WINAPI* RegCreateKeyTransactedWProc)(HKEY hKey, LPCWSTR lpSubKey, DWORD Reserved,
    LPWSTR lpClass, DWORD dwOptions, REGSAM samDesired, CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    PHKEY phkResult, LPDWORD lpdwDisposition, HANDLE hTransaction, PVOID  pExtendedParemeter);


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

LONG WINAPI Hooked_RegOpenKeyExA(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions,
                                         REGSAM samDesired, PHKEY phkResult);
LONG WINAPI Hooked_RegOpenKeyExW(HKEY hKey, LPCWSTR lpSubKey, DWORD ulOptions,
                                         REGSAM samDesired, PHKEY phkResult);

LONG WINAPI Hooked_RegOpenKeyTransactedA(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions,
    REGSAM samDesired, PHKEY phkResult, HANDLE hTransaction, PVOID pExtendedParemeter);

LONG WINAPI Hooked_RegOpenKeyTransactedW(HKEY hKey, LPCWSTR lpSubKey, DWORD ulOptions,
    REGSAM samDesired, PHKEY phkResult, HANDLE hTransaction, PVOID  pExtendedParemeter);

LONG WINAPI Hooked_RegOpenCurrentUser(REGSAM samDesired, PHKEY phkResult);

LONG WINAPI Hooked_RegOpenUserClassesRoot(HANDLE hToken, DWORD dwOptions, REGSAM samDesired, PHKEY phkResult);

LONG WINAPI Hooked_RegConnectRegistryA(LPCSTR lpMachineName, HKEY hKey, PHKEY phkResult);
LONG WINAPI Hooked_RegConnectRegistryW(LPCWSTR lpMachineName, HKEY hKey, PHKEY phkResult);

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

LONG WINAPI Hooked_RegCreateKeyTransactedA(HKEY hKey, LPCSTR lpSubKey, DWORD Reserved,
    LPSTR lpClass, DWORD dwOptions, REGSAM samDesired, CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    PHKEY phkResult, LPDWORD lpdwDisposition, HANDLE hTransaction, PVOID  pExtendedParemeter);

LONG WINAPI Hooked_RegCreateKeyTransactedW(HKEY hKey, LPCWSTR lpSubKey, DWORD Reserved,
    LPWSTR lpClass, DWORD dwOptions, REGSAM samDesired, CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    PHKEY phkResult, LPDWORD lpdwDisposition, HANDLE hTransaction, PVOID  pExtendedParemeter);

LONG WINAPI Hooked_RegSetValueA(HKEY hKey, LPCSTR lpSubKey, DWORD dwType, LPCSTR lpData, DWORD cbData);
LONG WINAPI Hooked_RegSetValueW(HKEY hKey, LPCWSTR lpSubKey, DWORD dwType, LPCWSTR lpData, DWORD cbData);

LONG WINAPI Hooked_RegSetValueExA(HKEY hKey, LPCSTR lpValueName, DWORD Reserved, DWORD dwType, CONST BYTE* lpData, DWORD cbData);
LONG WINAPI Hooked_RegSetValueExW(HKEY hKey, LPCWSTR lpValueName, DWORD Reserved, DWORD dwType, CONST BYTE* lpData, DWORD cbData);

LONG WINAPI Hooked_RegSetKeyValueA(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpValueName, 
                                   DWORD dwType, LPCVOID lpData, DWORD cbData);
LONG WINAPI Hooked_RegSetKeyValueW(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpValueName, 
                                   DWORD dwType, LPCVOID lpData, DWORD cbData);
