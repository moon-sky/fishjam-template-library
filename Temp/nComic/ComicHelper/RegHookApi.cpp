#include "stdafx.h"
#include "RegHookAPI.h"
#include "SetupInfoMgr.h"
#include "InfoConvUtil.h"

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

    RegOpenKeyAProc pOrigRegOpenKeyA = (RegOpenKeyAProc)g_HookApiInfo.HookApiInfos[hft_RegOpenKeyA]->pOriginal;
    if (pOrigRegOpenKeyA)
    {
        lRet = pOrigRegOpenKeyA(hKey, lpSubKey, phkResult);
        if (ERROR_SUCCESS == lRet)
        {
            CFConversion conv;
            LPCTSTR pszTCharSubKey = conv.MBCS_TO_TCHAR(lpSubKey);
            FTLTRACE(TEXT("!!! Hooked_RegOpenKeyA, hKey=0x%x, *phkResult=0x%x, lpSubKey=%s\n"), hKey, *phkResult, pszTCharSubKey);
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

    RegOpenKeyWProc pOrigRegOpenKeyW = (RegOpenKeyWProc)g_HookApiInfo.HookApiInfos[hft_RegOpenKeyW]->pOriginal;
    if (pOrigRegOpenKeyW)
    {
        lRet = pOrigRegOpenKeyW(hKey, lpSubKey, phkResult);
        if (ERROR_SUCCESS)
        {
            CFConversion conv;
            LPCTSTR pszTCharSubKey = conv.UTF16_TO_TCHAR(lpSubKey);

            FTLTRACE(TEXT("!!! Hooked_RegOpenKeyW, hKey=0x%x, *phkResult=0x%x, lpSubKey=%s\n"), 
                hKey, *phkResult, pszTCharSubKey);
            g_pSetupInfoMgr->OpenSetupReg(hKey, *phkResult, pszTCharSubKey);
        }
    }
    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);
    return lRet;
}

LONG WINAPI Hooked_RegOpenKeyExA(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions,
                                 REGSAM samDesired, PHKEY phkResult)
{
    LONG lRet = 0;
    BOOL bRet = FALSE;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);

    RegOpenKeyExAProc pOrigRegOpenKeyExA = (RegOpenKeyExAProc)g_HookApiInfo.HookApiInfos[hft_RegOpenKeyExA]->pOriginal;
    if (pOrigRegOpenKeyExA)
    {
        lRet = pOrigRegOpenKeyExA(hKey, lpSubKey, ulOptions, samDesired, phkResult);
        if (ERROR_SUCCESS == lRet)
        {
            CFConversion conv;
            LPCTSTR pszTCharSubKey = NULL;
            if (lpSubKey)
            {
                //SetupArcheAge.exe will call RegOpenKeyExA with lpSubKey is NULL when edit install path
                pszTCharSubKey = conv.MBCS_TO_TCHAR(lpSubKey);
            }
            
            CString strTCharSubKey;
            //if (((KEY_WOW64_64KEY & samDesired) == 0) && FTL::CFSystemUtil::Is64BitWindows())
            //{
            //    API_VERIFY(FTL::CFWowUtil::TranslateRegKeyToWow64(hKey, pszTCharSubKey, strTCharSubKey));
            //}
            //else
            {
                strTCharSubKey = pszTCharSubKey;
            }
            FTLTRACE(TEXT("!!! Hooked_RegOpenKeyExA, hKey=0x%x, *phkResult=0x%x, lpSubKey=%s\n"), 
                hKey, *phkResult, strTCharSubKey);
            g_pSetupInfoMgr->OpenSetupReg(hKey, *phkResult, strTCharSubKey);
        }
    }
    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);

    return lRet;
}
LONG WINAPI Hooked_RegOpenKeyExW(HKEY hKey, LPCWSTR lpSubKey, DWORD ulOptions,
                                 REGSAM samDesired, PHKEY phkResult)
{
    LONG lRet = 0;
    BOOL bRet = FALSE;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);

    RegOpenKeyExWProc pOrigRegOpenKeyExW = (RegOpenKeyExWProc)g_HookApiInfo.HookApiInfos[hft_RegOpenKeyExW]->pOriginal;
    if (pOrigRegOpenKeyExW)
    {
        lRet = pOrigRegOpenKeyExW(hKey, lpSubKey, ulOptions, samDesired, phkResult);
        if (ERROR_SUCCESS == lRet)
        {
            CFConversion conv;
            LPCTSTR pszTCharSubKey = conv.UTF16_TO_TCHAR(lpSubKey);
            CString strTCharSubKey;
            //if (((KEY_WOW64_64KEY & samDesired) == 0) && FTL::CFSystemUtil::Is64BitWindows())
            //{
            //    API_VERIFY(FTL::CFWowUtil::TranslateRegKeyToWow64(hKey, pszTCharSubKey, strTCharSubKey));
            //}
            //else
            {
                strTCharSubKey = pszTCharSubKey;
            }

            FTLTRACE(TEXT("!!! Hooked_RegOpenKeyExW, hKey=0x%x, *phkResult=0x%x, lpSubKey=%s\n"), hKey, *phkResult, strTCharSubKey);
            g_pSetupInfoMgr->OpenSetupReg(hKey, *phkResult, strTCharSubKey);
        }
    }
    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);

    return lRet;
}


LONG WINAPI Hooked_RegOpenKeyTransactedA(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions,
    REGSAM samDesired, PHKEY phkResult, HANDLE hTransaction, PVOID pExtendedParemeter)
{
    LONG lRet = 0;
    BOOL bRet = FALSE;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);

    RegOpenKeyTransactedAProc pOrigRegOpenKeyTransactedA = (RegOpenKeyTransactedAProc)g_HookApiInfo.HookApiInfos[hft_RegOpenKeyTransactedA]->pOriginal;
    if (pOrigRegOpenKeyTransactedA)
    {
        lRet = pOrigRegOpenKeyTransactedA(hKey, lpSubKey, ulOptions, samDesired, phkResult, hTransaction, pExtendedParemeter);
        if (ERROR_SUCCESS == lRet)
        {
            CFConversion conv;
            LPCTSTR pszTCharSubKey = conv.MBCS_TO_TCHAR(lpSubKey);
            CString strTCharSubKey;
            //if (((KEY_WOW64_64KEY & samDesired) == 0) && FTL::CFSystemUtil::Is64BitWindows())
            //{
            //    API_VERIFY(FTL::CFWowUtil::TranslateRegKeyToWow64(hKey, pszTCharSubKey, strTCharSubKey));
            //}
            //else
            {
                strTCharSubKey = pszTCharSubKey;
            }
            FTLTRACE(TEXT("!!! Hooked_RegOpenKeyTransactedA, hKey=0x%x, *phkResult=0x%x, lpSubKey=%s\n"), 
                hKey, *phkResult, strTCharSubKey);
            g_pSetupInfoMgr->OpenSetupReg(hKey, *phkResult, strTCharSubKey);
        }
    }
    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);

    return lRet;

}

LONG WINAPI Hooked_RegOpenKeyTransactedW(HKEY hKey, LPCWSTR lpSubKey, DWORD ulOptions,
    REGSAM samDesired, PHKEY phkResult, HANDLE hTransaction, PVOID  pExtendedParemeter)
{
    LONG lRet = 0;
    BOOL bRet = FALSE;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);

    RegOpenKeyTransactedWProc pOrigRegOpenKeyTransactedW = (RegOpenKeyTransactedWProc)g_HookApiInfo.HookApiInfos[hft_RegOpenKeyTransactedW]->pOriginal;
    if (pOrigRegOpenKeyTransactedW)
    {
        lRet = pOrigRegOpenKeyTransactedW(hKey, lpSubKey, ulOptions, samDesired, phkResult, hTransaction, pExtendedParemeter);
        if (ERROR_SUCCESS == lRet)
        {
            CFConversion conv;
            LPCTSTR pszTCharSubKey = conv.UTF16_TO_TCHAR(lpSubKey);
            CString strTCharSubKey;
            //if (((KEY_WOW64_64KEY & samDesired) == 0) && FTL::CFSystemUtil::Is64BitWindows())
            //{
            //    API_VERIFY(FTL::CFWowUtil::TranslateRegKeyToWow64(hKey, pszTCharSubKey, strTCharSubKey));
            //}
            //else
            {
                strTCharSubKey = pszTCharSubKey;
            }

            FTLTRACE(TEXT("!!! Hooked_RegOpenKeyTransactedW, hKey=0x%x, *phkResult=0x%x, lpSubKey=%s\n"), 
                hKey, *phkResult, strTCharSubKey);
            g_pSetupInfoMgr->OpenSetupReg(hKey, *phkResult, strTCharSubKey);
        }
    }
    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);

    return lRet;
}

LONG WINAPI Hooked_RegOpenCurrentUser(REGSAM samDesired, PHKEY phkResult)
{
    LONG lRet = 0;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);

    RegOpenCurrentUserProc pOrigRegOpenCurrentUser = (RegOpenCurrentUserProc)g_HookApiInfo.HookApiInfos[hft_RegOpenCurrentUser]->pOriginal;
    if (pOrigRegOpenCurrentUser)
    {
        lRet = pOrigRegOpenCurrentUser(samDesired, phkResult);
        if (ERROR_SUCCESS == lRet)
        {
            //CFConversion conv;
            LPCTSTR pszTCharSubKey = _T(""); //conv.MBCS_TO_TCHAR(lpSubKey);
            FTLTRACE(TEXT("!!! Hooked_RegOpenCurrentUser, samDesired=0x%x, *phkResult=0x%x\n"), samDesired, *phkResult);
            g_pSetupInfoMgr->OpenSetupReg(HKEY_CURRENT_USER, *phkResult, pszTCharSubKey);
        }
    }
    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);

    return lRet;
}

LONG WINAPI Hooked_RegOpenUserClassesRoot(HANDLE hToken, DWORD dwOptions, REGSAM samDesired, PHKEY phkResult)
{
    LONG lRet = 0;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);

    RegOpenUserClassesRootProc pOrigRegOpenUserClassesRoot = (RegOpenUserClassesRootProc)g_HookApiInfo.HookApiInfos[hft_RegOpenUserClassesRoot]->pOriginal;
    if (pOrigRegOpenUserClassesRoot)
    {
        lRet = pOrigRegOpenUserClassesRoot(hToken, dwOptions, samDesired, phkResult);
        if (ERROR_SUCCESS == lRet)
        {
            //CFConversion conv;
            LPCTSTR pszTCharSubKey =  _T("");//conv.MBCS_TO_TCHAR();
            FTLTRACE(TEXT("!!! Hooked_RegOpenUserClassesRoot, samDesired=0x%x, *phkResult=0x%x\n"), 
                samDesired, *phkResult);
            g_pSetupInfoMgr->OpenSetupReg(HKEY_CLASSES_ROOT , *phkResult, pszTCharSubKey);
        }
    }
    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);

    return lRet;
}

LONG WINAPI Hooked_RegConnectRegistryA(LPCSTR lpMachineName, HKEY hKey, PHKEY phkResult)
{
    LONG lRet = 0;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);

    RegConnectRegistryAProc pOrigRegConnectRegistryA = (RegConnectRegistryAProc)g_HookApiInfo.HookApiInfos[hft_RegConnectRegistryA]->pOriginal;
    if (pOrigRegConnectRegistryA)
    {
        lRet = pOrigRegConnectRegistryA(lpMachineName, hKey, phkResult);
        if (ERROR_SUCCESS == lRet)
        {
            CFConversion conv;
            LPCTSTR pszTCharMachineName = conv.MBCS_TO_TCHAR(lpMachineName);
            FTLTRACE(TEXT("!!! Hooked_RegConnectRegistryA, lpMachineName=%s, hKey=0x%x, *phkResult=0x%x\n"), 
                pszTCharMachineName, hKey, *phkResult);
            g_pSetupInfoMgr->OpenSetupReg(hKey, *phkResult, ConvertRegRootKeyToString(hKey));
        }
    }
    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);

    return lRet;

}
LONG WINAPI Hooked_RegConnectRegistryW(LPCWSTR lpMachineName, HKEY hKey, PHKEY phkResult)
{
    LONG lRet = 0;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);

    RegConnectRegistryWProc pOrigRegConnectRegistryW = (RegConnectRegistryWProc)g_HookApiInfo.HookApiInfos[hft_RegConnectRegistryW]->pOriginal;
    if (pOrigRegConnectRegistryW)
    {
        lRet = pOrigRegConnectRegistryW(lpMachineName, hKey, phkResult);
        if (ERROR_SUCCESS == lRet)
        {
            CFConversion conv;
            LPCTSTR pszTCharMachineName = conv.UTF16_TO_TCHAR(lpMachineName);
            FTLTRACE(TEXT("!!! Hooked_RegConnectRegistryW, lpMachineName=%s, hKey=0x%x, *phkResult=0x%x\n"), 
                pszTCharMachineName, hKey, *phkResult);
            g_pSetupInfoMgr->OpenSetupReg(hKey, *phkResult, ConvertRegRootKeyToString(hKey));
        }
    }

    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);
    return lRet;

}

LONG WINAPI Hooked_RegCreateKeyA(HKEY hKey, LPCSTR lpSubKey, PHKEY phkResult)
{
    LONG lRet = 0;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);

    RegCreateKeyAProc pOrigRegCreateKeyA = (RegCreateKeyAProc)g_HookApiInfo.HookApiInfos[hft_RegCreateKeyA]->pOriginal;
    if (pOrigRegCreateKeyA)
    {
        lRet = pOrigRegCreateKeyA(hKey, lpSubKey, phkResult);
        if (ERROR_SUCCESS == lRet)
        {
            CFConversion conv;
            LPCTSTR pszTCharSubKey = conv.MBCS_TO_TCHAR(lpSubKey);
            FTLTRACE(TEXT("!!! Hooked_RegCreateKeyA, hKey=0x%x, *phkResult=0x%x, lpSubKey=%s\n"), hKey, *phkResult, pszTCharSubKey);
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

    RegCreateKeyWProc pOrigRegCreateKeyW = (RegCreateKeyWProc)g_HookApiInfo.HookApiInfos[hft_RegCreateKeyW]->pOriginal;
    if (pOrigRegCreateKeyW)
    {
        lRet = pOrigRegCreateKeyW(hKey, lpSubKey, phkResult);
        if (ERROR_SUCCESS == lRet)
        {
            CFConversion conv;
            LPCTSTR pszTCharSubKey = conv.UTF16_TO_TCHAR(lpSubKey);

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
    BOOL bRet = FALSE;
    LONG lRet = 0;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);

    RegCreateKeyExAProc pOrigRegCreateKeyExA = (RegCreateKeyExAProc)g_HookApiInfo.HookApiInfos[hft_RegCreateKeyExA]->pOriginal;
    if (pOrigRegCreateKeyExA)
    {
        lRet = pOrigRegCreateKeyExA(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired,
            lpSecurityAttributes, phkResult, lpdwDisposition);
        if (ERROR_SUCCESS == lRet)
        {
            CFConversion conv;
            LPCTSTR pszTCharSubKey = conv.MBCS_TO_TCHAR(lpSubKey);
            CString strTCharSubKey;
            //if (((KEY_WOW64_64KEY & samDesired) == 0) && FTL::CFSystemUtil::Is64BitWindows())
            //{
            //    API_VERIFY(FTL::CFWowUtil::TranslateRegKeyToWow64(hKey, pszTCharSubKey, strTCharSubKey));
            //}
            //else
            {
                strTCharSubKey = pszTCharSubKey;
            }

            FTLTRACE(TEXT("!!! Hooked_RegCreateKeyExA, hKey=0x%x, *phkResult=0x%x, lpSubKey=%s, samDesired=0x%x\n"), 
                hKey, *phkResult, strTCharSubKey, samDesired);
            g_pSetupInfoMgr->OpenSetupReg(hKey, *phkResult, strTCharSubKey);
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
    BOOL bRet = FALSE;
    LONG lRet = 0;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);

    RegCreateKeyExWProc pOrigRegCreateKeyExW = (RegCreateKeyExWProc)g_HookApiInfo.HookApiInfos[hft_RegCreateKeyExW]->pOriginal;
    if (pOrigRegCreateKeyExW)
    {
        lRet = pOrigRegCreateKeyExW(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired,
            lpSecurityAttributes, phkResult, lpdwDisposition);
        if (ERROR_SUCCESS == lRet)
        {
            CFConversion conv;
            LPCTSTR pszTCharSubKey = conv.UTF16_TO_TCHAR(lpSubKey);
            CString strTCharSubKey;
            //if (((KEY_WOW64_64KEY & samDesired) == 0) && FTL::CFSystemUtil::Is64BitWindows())
            //{
            //    API_VERIFY(FTL::CFWowUtil::TranslateRegKeyToWow64(hKey, pszTCharSubKey, strTCharSubKey));
            //}
            //else
            {
                 strTCharSubKey = pszTCharSubKey;
            }

            FTLTRACE(TEXT("!!! Hooked_RegCreateKeyExW, hKey=0x%x, *phkResult=0x%x, lpSubKey=%s, samDesired=0x%x\n"), 
                hKey, *phkResult, strTCharSubKey, samDesired);

            g_pSetupInfoMgr->OpenSetupReg(hKey, *phkResult, strTCharSubKey);
        }
    }

    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);
    return lRet;

}

LONG WINAPI Hooked_RegCreateKeyTransactedA(HKEY hKey, LPCSTR lpSubKey, DWORD Reserved,
                                           LPSTR lpClass, DWORD dwOptions, REGSAM samDesired, CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                                           PHKEY phkResult, LPDWORD lpdwDisposition, HANDLE hTransaction, PVOID  pExtendedParemeter)
{
    BOOL bRet = FALSE;
    LONG lRet = 0;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);

    RegCreateKeyTransactedAProc pOrigRegCreateKeyTransactedA = (RegCreateKeyTransactedAProc)g_HookApiInfo.HookApiInfos[hft_RegCreateKeyTransactedA]->pOriginal;
    if (pOrigRegCreateKeyTransactedA)
    {
        lRet = pOrigRegCreateKeyTransactedA(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired,lpSecurityAttributes,
            phkResult, lpdwDisposition, hTransaction, pExtendedParemeter);
        if (ERROR_SUCCESS == lRet)
        {
            CFConversion conv;
            LPCTSTR pszTCharSubKey = conv.MBCS_TO_TCHAR(lpSubKey);
            CString strTCharSubKey;
            //if (((KEY_WOW64_64KEY & samDesired) == 0) && FTL::CFSystemUtil::Is64BitWindows())
            //{
            //    API_VERIFY(FTL::CFWowUtil::TranslateRegKeyToWow64(hKey, pszTCharSubKey, strTCharSubKey));
            //}
            //else
            {
                strTCharSubKey = pszTCharSubKey;
            }

            FTLTRACE(TEXT("!!! Hooked_RegCreateKeyTransactedA, hKey=0x%x, *phkResult=0x%x, lpSubKey=%s, samDesired=0x%x\n"), 
                hKey, *phkResult, strTCharSubKey, samDesired);
            g_pSetupInfoMgr->OpenSetupReg(hKey, *phkResult, strTCharSubKey);
        }
    }

    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);
    return lRet;
}

LONG WINAPI Hooked_RegCreateKeyTransactedW(HKEY hKey, LPCWSTR lpSubKey, DWORD Reserved,
                                           LPWSTR lpClass, DWORD dwOptions, REGSAM samDesired, CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                                           PHKEY phkResult, LPDWORD lpdwDisposition, HANDLE hTransaction, PVOID  pExtendedParemeter)
{
    BOOL bRet = FALSE;
    LONG lRet = 0;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);

    RegCreateKeyTransactedWProc pOrigRegCreateKeyTransactedW = (RegCreateKeyTransactedWProc)g_HookApiInfo.HookApiInfos[hft_RegCreateKeyTransactedW]->pOriginal;
    if (pOrigRegCreateKeyTransactedW)
    {
        lRet = pOrigRegCreateKeyTransactedW(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired,lpSecurityAttributes,
            phkResult, lpdwDisposition, hTransaction, pExtendedParemeter);
        if (ERROR_SUCCESS == lRet)
        {
            CFConversion conv;
            LPCTSTR pszTCharSubKey = conv.UTF16_TO_TCHAR(lpSubKey);
            CString strTCharSubKey;
            //if (((KEY_WOW64_64KEY & samDesired) == 0) && FTL::CFSystemUtil::Is64BitWindows())
            //{
            //    API_VERIFY(FTL::CFWowUtil::TranslateRegKeyToWow64(hKey, pszTCharSubKey, strTCharSubKey));
            //}
            //else
            {
                strTCharSubKey = pszTCharSubKey;
            }

            FTLTRACE(TEXT("!!! Hooked_RegCreateKeyTransactedW, hKey=0x%x, *phkResult=0x%x, lpSubKey=%s, samDesired=0x%x\n"), 
                hKey, *phkResult, strTCharSubKey, samDesired);
            g_pSetupInfoMgr->OpenSetupReg(hKey, *phkResult, strTCharSubKey);
        }
    }

    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);
    return lRet;
}

LONG WINAPI Hooked_RegSetValueA(HKEY hKey, LPCSTR lpSubKey, DWORD dwType, LPCSTR lpData, DWORD cbData)
{
    LONG lRet = 0;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);

    RegSetValueAProc pOrigRegSetValueA = (RegSetValueAProc)g_HookApiInfo.HookApiInfos[hft_RegSetValueA]->pOriginal;
    if (pOrigRegSetValueA)
    {
        lRet = pOrigRegSetValueA(hKey, lpSubKey, dwType, lpData, cbData);
        if (ERROR_SUCCESS == lRet)
        {
            CFConversion conv;
            LPCTSTR pszTCharSubKey = conv.MBCS_TO_TCHAR(lpSubKey);
            FTLTRACE(TEXT("!!! Hooked_RegSetValueA, hKey=0x%x, lpSubKey=%s\n"), hKey, pszTCharSubKey);
            g_pSetupInfoMgr->SetSetupRegInfo(hKey, pszTCharSubKey);
        }
    }
    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);
    return lRet;
}

LONG WINAPI Hooked_RegSetValueW(HKEY hKey, LPCWSTR lpSubKey, DWORD dwType, LPCWSTR lpData, DWORD cbData)
{
    LONG lRet = 0;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);

    RegSetValueWProc pOrigRegSetValueW = (RegSetValueWProc)g_HookApiInfo.HookApiInfos[hft_RegSetValueW]->pOriginal;
    if (pOrigRegSetValueW)
    {
        lRet = pOrigRegSetValueW(hKey, lpSubKey, dwType, lpData, cbData);
        if (ERROR_SUCCESS == lRet)
        {
            CFConversion conv;
            LPCTSTR pszTCharSubKey = conv.UTF16_TO_TCHAR(lpSubKey);
            FTLTRACE(TEXT("!!! Hooked_RegSetValueW, hKey=0x%x, lpSubKey=%s\n"), hKey, pszTCharSubKey);
            g_pSetupInfoMgr->SetSetupRegInfo(hKey, pszTCharSubKey);
        }
    }
    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);
    return lRet;
}

LONG WINAPI Hooked_RegSetValueExA(HKEY hKey, LPCSTR lpValueName, DWORD Reserved, DWORD dwType, CONST BYTE* lpData, DWORD cbData)
{
    LONG lRet = 0;
    HOOKED_API_CALL_ENTER(g_HookApiInfo.HookedAPICallCount);

    RegSetValueExAProc pOrigRegSetValueExA = (RegSetValueExAProc)g_HookApiInfo.HookApiInfos[hft_RegSetValueExA]->pOriginal;
    if (pOrigRegSetValueExA)
    {
        lRet = pOrigRegSetValueExA(hKey, lpValueName, Reserved, dwType, lpData, cbData);
        if (ERROR_SUCCESS == lRet)
        {
            CFConversion conv;
            LPCTSTR pszTCharValueName = conv.MBCS_TO_TCHAR(lpValueName);
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

    RegSetValueExWProc pOrigRegSetValueExW = (RegSetValueExWProc)g_HookApiInfo.HookApiInfos[hft_RegSetValueExW]->pOriginal;
    if (pOrigRegSetValueExW)
    {
        lRet = pOrigRegSetValueExW(hKey, lpValueName, Reserved, dwType, lpData, cbData);
        if (ERROR_SUCCESS == lRet)
        {
            CFConversion conv;
            LPCTSTR pszTCharValueName = conv.UTF16_TO_TCHAR(lpValueName);
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

    CHAR szInfo[MAX_PATH * 2] = {0};
    StringCchPrintfA(szInfo, _countof(szInfo), "!!! Hooked_RegSetKeyValueA, lpSubKey=%s, lpValueName=%s\n", 
        hKey, lpSubKey, lpValueName);
    OutputDebugStringA(szInfo);

    RegSetKeyValueAProc pOrigRegSetKeyValueA = (RegSetKeyValueAProc)g_HookApiInfo.HookApiInfos[hft_RegSetKeyValueA]->pOriginal;
    if (pOrigRegSetKeyValueA)
    {
        lRet = pOrigRegSetKeyValueA(hKey, lpSubKey, lpValueName, dwType, lpData, cbData);
        if (ERROR_SUCCESS == lRet)
        {
            CFConversion convSubKey;
            CFConversion convValueName;
            LPCTSTR pszTCharSubKey = convSubKey.MBCS_TO_TCHAR(lpSubKey);
            LPCTSTR pszTCharValueName = convValueName.MBCS_TO_TCHAR(lpValueName);
            FTLTRACE(TEXT("!!! Hooked_RegSetKeyValueA, hKey=0x%x, lpSubKey=%s, lpValueName=%s\n"), 
                hKey, pszTCharSubKey, pszTCharValueName);
            g_pSetupInfoMgr->SetSetupRegInfo(hKey, pszTCharSubKey);
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

    RegSetKeyValueWProc pOrigRegSetKeyValueW = (RegSetKeyValueWProc)g_HookApiInfo.HookApiInfos[hft_RegSetKeyValueW]->pOriginal;
    if (pOrigRegSetKeyValueW)
    {
        lRet = pOrigRegSetKeyValueW(hKey, lpSubKey, lpValueName, dwType, lpData, cbData);
        if (ERROR_SUCCESS == lRet)
        {
            CFConversion convSubKey;
            CFConversion convValueName;
            LPCTSTR pszTCharSubKey = convSubKey.UTF16_TO_TCHAR(lpSubKey);
            LPCTSTR pszTCharValueName = convValueName.UTF16_TO_TCHAR(lpValueName);

            FTLTRACE(TEXT("!!! Hooked_RegSetKeyValueW, hKey=0x%x, lpSubKey=%s, lpValueName=%s\n"), 
                hKey, pszTCharSubKey, pszTCharValueName);

            g_pSetupInfoMgr->SetSetupRegInfo(hKey, pszTCharSubKey);
        }
    }

    HOOKED_API_CALL_LEAVE(g_HookApiInfo.HookedAPICallCount);
    return lRet;
}
