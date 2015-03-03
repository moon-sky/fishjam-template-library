#include "StdAfx.h"
#include "FTLCrackTester.h"
#include <ftlCrack.h>
#include <SoftPub.h>
#pragma comment(lib, "Wintrust.lib")

void CFTLCrackTester::test_GetCrackString()
{
    CFCrackUtility crackUtil;
    CString strTemp = crackUtil.GetCrackString(CFCrackUtility::csKiloString,0);
    strTemp = crackUtil.GetCrackString(CFCrackUtility::csKiloString,16);
    strTemp = crackUtil.GetCrackString(CFCrackUtility::csKiloString,32);
}

void CFTLCrackTester::test_WinVerifyTrust()
{
    BOOL bRet = FALSE;
    HRESULT hr = E_FAIL;
    HWND hWnd = NULL;
    DWORD dwUIChoce = WTD_UI_NONE;
    BOOL bShowUI = TRUE;
    
    if (bShowUI)
    {
        //如果为TRUE，会弹出类似 "未验证发布者" 等确认对话框

        hWnd = ::GetActiveWindow();
        dwUIChoce = WTD_UI_ALL;
    }

    WCHAR szVerifyFilePath[MAX_PATH] = {0};
    API_VERIFY( 0 != GetSystemDirectoryW(szVerifyFilePath, _countof(szVerifyFilePath)));
    if (bRet)
    {
        //API_VERIFY(::PathRemoveFileSpec(szVerifyFilePath));
        API_VERIFY(::PathAppend(szVerifyFilePath, TEXT("Rundll32.exe")));

        GUID guidPublishedSoftware = WINTRUST_ACTION_GENERIC_VERIFY_V2;

        WINTRUST_FILE_INFO fileinfo = {0};
        fileinfo.cbStruct = sizeof fileinfo;
        fileinfo.pcwszFilePath = (LPCWSTR)szVerifyFilePath;
        fileinfo.pgKnownSubject = NULL;
        fileinfo.hFile = NULL;

        WINTRUST_DATA wintrustdata = {0};
        wintrustdata.cbStruct = sizeof wintrustdata;
        wintrustdata.fdwRevocationChecks = WTD_REVOKE_NONE; 
        wintrustdata.dwStateAction = 0;
        wintrustdata.hWVTStateData = 0;
        wintrustdata.pwszURLReference = NULL;
        wintrustdata.dwUIChoice = dwUIChoce;
        wintrustdata.dwProvFlags = 0;
        wintrustdata.dwUnionChoice = WTD_CHOICE_FILE;
        wintrustdata.pFile = &fileinfo;

        //TRUST_E_SUBJECT_NOT_TRUSTED
        COM_VERIFY(WinVerifyTrust(hWnd, &guidPublishedSoftware, &wintrustdata));

    }


    
}
