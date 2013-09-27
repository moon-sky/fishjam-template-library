#include "stdafx.h"
#include "RegHookAPI.h"
#include "SetupInfoMgr.h"

LONG WINAPI Hooked_RegOpenKeyA(HKEY hKey, LPCSTR lpSubKey, PHKEY phkResult)
{
    LONG lRet = 0;
    FTLTRACE(TEXT("!!! Hooked_RegOpenKeyA\n"));

    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);
    __try
    {
        RegOpenKeyAProc pOrigRegOpenKeyA = (RegOpenKeyAProc)g_HookApiInfo.HookApiInfos[hft_RegOpenKeyA]->pOriginal;
        if (pOrigRegOpenKeyA)
        {
            lRet = pOrigRegOpenKeyA(hKey, lpSubKey, phkResult);
        }
    }
    __finally
    {
        HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);
    }

    return lRet;
}

LONG WINAPI Hooked_RegOpenKeyW(HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult)
{
    LONG lRet = 0;
    FTLTRACE(TEXT("!!! Hooked_RegOpenKeyW, hKey=%d, lpSubKey=%s\n"), hKey, lpSubKey);

    g_pSetupInfoMgr->SetSetupInfo(hsit_NewReg, lpSubKey, NULL);

    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);
    __try
    {
        RegOpenKeyWProc pOrigRegOpenKeyW = (RegOpenKeyWProc)g_HookApiInfo.HookApiInfos[hft_RegOpenKeyW]->pOriginal;
        if (pOrigRegOpenKeyW)
        {
            lRet = pOrigRegOpenKeyW(hKey, lpSubKey, phkResult);
        }
    }
    __finally
    {
        HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);
    }
    return lRet;
}


LONG WINAPI Hooked_RegSetKeyValueA(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpValueName, 
                                   DWORD dwType, LPCVOID lpData, DWORD cbData)
{
    LONG lRet = 0;
    //FTLTRACE(TEXT("!!! Hooked_RegSetKeyValueA, hKey=0x%x, lpSubKey=%s, lpValueName\n"), hKey, lpSubKey, lpValueName);

    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);
    __try
    {
        RegSetKeyValueAProc pOrigRegSetKeyValueA = (RegSetKeyValueAProc)g_HookApiInfo.HookApiInfos[hft_RegSetKeyValueA]->pOriginal;
        if (pOrigRegSetKeyValueA)
        {
            lRet = pOrigRegSetKeyValueA(hKey, lpSubKey, lpValueName, dwType, lpData, cbData);
            if (ERROR_SUCCESS == lRet)
            {
                //g_pSetupInfoMgr->SetSetupInfo(hsit_NewReg, lpSubKey, NULL);
            }
        }
    }
    __finally
    {
        HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);
    }
    return lRet;
}

LONG WINAPI Hooked_RegSetKeyValueW(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpValueName, 
                                   DWORD dwType, LPCVOID lpData, DWORD cbData)
{
    LONG lRet = 0;
    FTLTRACE(TEXT("!!! Hooked_RegSetKeyValueW, hKey=0x%x, lpSubKey=%s, lpValueName\n"), 
        hKey, lpSubKey, lpValueName);

    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);
    __try
    {
        RegSetKeyValueWProc pOrigRegSetKeyValueW = (RegSetKeyValueWProc)g_HookApiInfo.HookApiInfos[hft_RegSetKeyValueW]->pOriginal;
        if (pOrigRegSetKeyValueW)
        {
            lRet = pOrigRegSetKeyValueW(hKey, lpSubKey, lpValueName, dwType, lpData, cbData);
            if (ERROR_SUCCESS == lRet)
            {
                //g_pSetupInfoMgr->SetSetupInfo(hsit_NewReg, lpSubKey, NULL);
            }
        }
    }
    __finally
    {
        HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);
    }
    return lRet;
}
