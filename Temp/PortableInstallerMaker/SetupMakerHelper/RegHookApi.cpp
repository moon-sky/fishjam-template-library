#include "stdafx.h"
#include "RegHookAPI.h"
#include "SetupInfoMgr.h"

LONG WINAPI Hooked_RegCloseKey(HKEY hKey)
{
    LONG lRet = 0;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);

    RegCloseKeyProc pOrigRegCloseKey = (RegCloseKeyProc)g_HookApiInfo.HookApiInfos[hft_RegCloseKey]->pOriginal;
    if (pOrigRegCloseKey)
    {
        lRet = pOrigRegCloseKey(hKey);
        if (ERROR_SUCCESS == lRet)
        {
            g_pSetupInfoMgr->CloseSetupReg(hKey);
            FTLTRACE(TEXT("!!! Hooked_RegCloseKey, hKey=0x%x\n"), hKey);
        }
    }
    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);

    return lRet;

}

LONG WINAPI Hooked_RegOpenKeyA(HKEY hKey, LPCSTR lpSubKey, PHKEY phkResult)
{
    LONG lRet = 0;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);
    LPCTSTR pszTCharSubKey = CFConversion().MBCS_TO_TCHAR(lpSubKey);

    FTLTRACE(TEXT("!!! Hooked_RegOpenKeyA, hKey=0x%x, lpSubKey=%s\n"), hKey, pszTCharSubKey);
    RegOpenKeyAProc pOrigRegOpenKeyA = (RegOpenKeyAProc)g_HookApiInfo.HookApiInfos[hft_RegOpenKeyA]->pOriginal;
    if (pOrigRegOpenKeyA)
    {
        lRet = pOrigRegOpenKeyA(hKey, lpSubKey, phkResult);
        if (ERROR_SUCCESS == lRet)
        {
            g_pSetupInfoMgr->OpenSetupReg(hKey, *phkResult, pszTCharSubKey);
        }
    }
    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);

    return lRet;
}

LONG WINAPI Hooked_RegOpenKeyW(HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult)
{
    LONG lRet = 0;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);
    LPCTSTR pszTCharSubKey = CFConversion().UTF16_TO_TCHAR(lpSubKey);

    FTLTRACE(TEXT("!!! Hooked_RegOpenKeyW, hKey=%d, lpSubKey=%s\n"), hKey, pszTCharSubKey);
    
    RegOpenKeyWProc pOrigRegOpenKeyW = (RegOpenKeyWProc)g_HookApiInfo.HookApiInfos[hft_RegOpenKeyW]->pOriginal;
    if (pOrigRegOpenKeyW)
    {
        lRet = pOrigRegOpenKeyW(hKey, lpSubKey, phkResult);
        if (ERROR_SUCCESS)
        {
            g_pSetupInfoMgr->OpenSetupReg(hKey, *phkResult, pszTCharSubKey);
        }
    }
    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);
    return lRet;
}

LONG WINAPI Hooked_RegCreateKeyA(HKEY hKey, LPCSTR lpSubKey, PHKEY phkResult)
{
    LONG lRet = 0;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);
    LPCTSTR pszTCharSubKey = CFConversion().MBCS_TO_TCHAR(lpSubKey);

    RegCreateKeyAProc pOrigRegCreateKeyA = (RegCreateKeyAProc)g_HookApiInfo.HookApiInfos[hft_RegCreateKeyA]->pOriginal;
    if (pOrigRegCreateKeyA)
    {
        lRet = pOrigRegCreateKeyA(hKey, lpSubKey, phkResult);
        if (ERROR_SUCCESS == lRet)
        {
            FTLTRACE(TEXT("!!! Hooked_RegCreateKeyA, hKey=0x%x, lpSubKey=%s\n"), hKey, pszTCharSubKey);
            g_pSetupInfoMgr->OpenSetupReg(hKey, *phkResult, pszTCharSubKey);
        }
    }
    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);

    return lRet;
}

LONG WINAPI Hooked_RegCreateKeyW(HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult)
{
    LONG lRet = 0;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);
    LPCTSTR pszTCharSubKey = CFConversion().UTF16_TO_TCHAR(lpSubKey);

    RegCreateKeyWProc pOrigRegCreateKeyW = (RegCreateKeyWProc)g_HookApiInfo.HookApiInfos[hft_RegCreateKeyW]->pOriginal;
    if (pOrigRegCreateKeyW)
    {
        lRet = pOrigRegCreateKeyW(hKey, lpSubKey, phkResult);
        if (ERROR_SUCCESS == lRet)
        {
            FTLTRACE(TEXT("!!! Hooked_RegCreateKeyW, hKey=0x%x, *phkResult=0x%x, lpSubKey=%s\n"), 
                hKey, *phkResult, pszTCharSubKey);

            g_pSetupInfoMgr->OpenSetupReg(hKey, *phkResult, pszTCharSubKey);
        }
    }

    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);
    return lRet;
}



LONG WINAPI Hooked_RegCreateKeyExA(HKEY hKey, LPCSTR lpSubKey, DWORD Reserved,
                                   LPSTR lpClass, DWORD dwOptions, REGSAM samDesired, 
                                   CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                                   PHKEY phkResult, LPDWORD lpdwDisposition)
{
    LONG lRet = 0;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);
    LPCTSTR pszTCharSubKey = CFConversion().MBCS_TO_TCHAR(lpSubKey);

    RegCreateKeyExAProc pOrigRegCreateKeyExA = (RegCreateKeyExAProc)g_HookApiInfo.HookApiInfos[hft_RegCreateKeyExW]->pOriginal;
    if (pOrigRegCreateKeyExA)
    {
        lRet = pOrigRegCreateKeyExA(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired,
            lpSecurityAttributes, phkResult, lpdwDisposition);
        if (ERROR_SUCCESS == lRet)
        {
            FTLTRACE(TEXT("!!! Hooked_RegCreateKeyExA, hKey=0x%x, *phkResult=0x%x, lpSubKey=%s\n"), 
                hKey, *phkResult, pszTCharSubKey);
            g_pSetupInfoMgr->OpenSetupReg(hKey, *phkResult, pszTCharSubKey);
        }
    }

    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);
    return lRet;
}

LONG WINAPI Hooked_RegCreateKeyExW(HKEY hKey, LPCWSTR lpSubKey, DWORD Reserved,
                                   LPWSTR lpClass, DWORD dwOptions, REGSAM samDesired, 
                                   CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                                   PHKEY phkResult, LPDWORD lpdwDisposition)
{
    LONG lRet = 0;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);
    LPCTSTR pszTCharSubKey = CFConversion().UTF16_TO_TCHAR(lpSubKey);

    RegCreateKeyExWProc pOrigRegCreateKeyExW = (RegCreateKeyExWProc)g_HookApiInfo.HookApiInfos[hft_RegCreateKeyExW]->pOriginal;
    if (pOrigRegCreateKeyExW)
    {
        lRet = pOrigRegCreateKeyExW(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired,
            lpSecurityAttributes, phkResult, lpdwDisposition);
        if (ERROR_SUCCESS == lRet)
        {
            FTLTRACE(TEXT("!!! Hooked_RegCreateKeyExW, hKey=0x%x, *phkResult=0x%x, lpSubKey=%s\n"), 
                hKey, *phkResult, pszTCharSubKey);

            g_pSetupInfoMgr->OpenSetupReg(hKey, *phkResult, pszTCharSubKey);
        }
    }

    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);
    return lRet;

}

LONG WINAPI Hooked_RegSetValueA(HKEY hKey, LPCSTR lpSubKey, DWORD dwType, LPCSTR lpData, DWORD cbData)
{
    LONG lRet = 0;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);
    LPCTSTR pszTCharSubKey = CFConversion().MBCS_TO_TCHAR(lpSubKey);

    RegSetValueAProc pOrigRegSetValueA = (RegSetValueAProc)g_HookApiInfo.HookApiInfos[hft_RegSetValueA]->pOriginal;
    if (pOrigRegSetValueA)
    {
        lRet = pOrigRegSetValueA(hKey, lpSubKey, dwType, lpData, cbData);
        if (ERROR_SUCCESS == lRet)
        {
            FTLTRACE(TEXT("!!! Hooked_RegSetValueA, hKey=0x%x, lpSubKey=%s\n"), hKey, pszTCharSubKey);
            //g_pSetupInfoMgr->SetSetupInfo(hsit_NewReg, lpSubKey, NULL);
        }
    }
    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);
    return lRet;
}

LONG WINAPI Hooked_RegSetValueW(HKEY hKey, LPCWSTR lpSubKey, DWORD dwType, LPCWSTR lpData, DWORD cbData)
{
    LONG lRet = 0;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);
    LPCTSTR pszTCharSubKey = CFConversion().UTF16_TO_TCHAR(lpSubKey);

    FTLTRACE(TEXT("!!! Hooked_RegSetValueW, hKey=0x%x, lpSubKey=%s\n"), hKey, pszTCharSubKey);

    RegSetValueWProc pOrigRegSetValueW = (RegSetValueWProc)g_HookApiInfo.HookApiInfos[hft_RegSetValueW]->pOriginal;
    if (pOrigRegSetValueW)
    {
        lRet = pOrigRegSetValueW(hKey, lpSubKey, dwType, lpData, cbData);
        if (ERROR_SUCCESS == lRet)
        {
            //g_pSetupInfoMgr->SetSetupInfo(hsit_NewReg, pszTCharSubKey, NULL);
        }
    }
    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);
    return lRet;
}


LONG WINAPI Hooked_RegSetValueExA(HKEY hKey, LPCSTR lpValueName, DWORD Reserved, DWORD dwType, CONST BYTE* lpData, DWORD cbData)
{
    LONG lRet = 0;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);
    LPCTSTR pszTCharValueName = CFConversion().MBCS_TO_TCHAR(lpValueName);

    RegSetValueExAProc pOrigRegSetValueExA = (RegSetValueExAProc)g_HookApiInfo.HookApiInfos[hft_RegSetValueExA]->pOriginal;
    if (pOrigRegSetValueExA)
    {
        lRet = pOrigRegSetValueExA(hKey, lpValueName, Reserved, dwType, lpData, cbData);
        if (ERROR_SUCCESS == lRet)
        {
            FTLTRACE(TEXT("!!! Hooked_RegSetValueExA, hKey=0x%x, lpValueName=%s\n"), 
                hKey, pszTCharValueName);

            g_pSetupInfoMgr->SetSetupRegInfo(hKey, pszTCharValueName);
        }
    }
    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);
    return lRet;
}

LONG WINAPI Hooked_RegSetValueExW(HKEY hKey, LPCWSTR lpValueName, DWORD Reserved, DWORD dwType, CONST BYTE* lpData, DWORD cbData)
{
    LONG lRet = 0;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);
    LPCTSTR pszTCharValueName = CFConversion().UTF16_TO_TCHAR(lpValueName);

    RegSetValueExWProc pOrigRegSetValueExW = (RegSetValueExWProc)g_HookApiInfo.HookApiInfos[hft_RegSetValueExW]->pOriginal;
    if (pOrigRegSetValueExW)
    {
        lRet = pOrigRegSetValueExW(hKey, lpValueName, Reserved, dwType, lpData, cbData);
        if (ERROR_SUCCESS == lRet)
        {
            FTLTRACE(TEXT("!!! Hooked_RegSetValueExW, hKey=0x%x, lpValueName=%s\n"), hKey, pszTCharValueName);
            g_pSetupInfoMgr->SetSetupRegInfo(hKey, pszTCharValueName);
        }
    }
    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);
    return lRet;
}


LONG WINAPI Hooked_RegSetKeyValueA(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpValueName, 
                                   DWORD dwType, LPCVOID lpData, DWORD cbData)
{
    LONG lRet = 0;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);
    LPCTSTR pszTCharSubKey = CFConversion().MBCS_TO_TCHAR(lpSubKey);
    LPCTSTR pszTCharValueName = CFConversion().MBCS_TO_TCHAR(lpValueName);

    FTLTRACE(TEXT("!!! Hooked_RegSetKeyValueA, hKey=0x%x, lpSubKey=%s, lpValueName\n"), 
        hKey, pszTCharSubKey, pszTCharValueName);

    RegSetKeyValueAProc pOrigRegSetKeyValueA = (RegSetKeyValueAProc)g_HookApiInfo.HookApiInfos[hft_RegSetKeyValueA]->pOriginal;
    if (pOrigRegSetKeyValueA)
    {
        lRet = pOrigRegSetKeyValueA(hKey, lpSubKey, lpValueName, dwType, lpData, cbData);
        if (ERROR_SUCCESS == lRet)
        {
            //g_pSetupInfoMgr->SetSetupInfo(hsit_NewReg, pszTCharSubKey, NULL);
        }
    }

    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);
    return lRet;
}

LONG WINAPI Hooked_RegSetKeyValueW(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpValueName, 
                                   DWORD dwType, LPCVOID lpData, DWORD cbData)
{
    LONG lRet = 0;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);
    LPCTSTR pszTCharSubKey = CFConversion().UTF16_TO_TCHAR(lpSubKey);
    LPCTSTR pszTCharValueName = CFConversion().UTF16_TO_TCHAR(lpValueName);

    FTLTRACE(TEXT("!!! Hooked_RegSetKeyValueW, hKey=0x%x, lpSubKey=%s, lpValueName\n"), 
        hKey, pszTCharSubKey, pszTCharValueName);

    RegSetKeyValueWProc pOrigRegSetKeyValueW = (RegSetKeyValueWProc)g_HookApiInfo.HookApiInfos[hft_RegSetKeyValueW]->pOriginal;
    if (pOrigRegSetKeyValueW)
    {
        lRet = pOrigRegSetKeyValueW(hKey, lpSubKey, lpValueName, dwType, lpData, cbData);
        if (ERROR_SUCCESS == lRet)
        {
            //g_pSetupInfoMgr->SetSetupInfo(hsit_NewReg, lpSubKey, NULL);
        }
    }

    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);
    return lRet;
}
