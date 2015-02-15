#ifndef FTL_NLS_HPP
#define FTL_NLS_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlNLS.h"
#endif

#include "ftlSystem.h"
#include <imm.h>

namespace FTL
{
    // Type definitions
    typedef struct LANGINFO_DEF {
        int		Count;
        LANGID	LangID;
    } LANGINFO;
    typedef LANGINFO *PLANGINFO;

    BOOL CALLBACK CFNLS::EnumLangProc(HANDLE, LPCTSTR, LPCTSTR,
        WORD wIDLanguage, LONG_PTR lParam)
    {
        PLANGINFO LangInfo;
        LangInfo = (PLANGINFO) lParam;
        LangInfo->Count++;
        LangInfo->LangID  = wIDLanguage;
        return (TRUE);        // continue enumeration
    }

    // Detects the language of ntdll.dll with some specific processing for 
    // the Hongkong SAR version
    LANGID CFNLS::GetNTDLLNativeLangID()
    {

        LANGINFO LangInfo;
        LPCTSTR Type = RT_VERSION;//(LPCTSTR) ((LPVOID)((WORD)16));
        LPCTSTR Name = (LPCTSTR) 1;

        ZeroMemory(&LangInfo,sizeof(LangInfo));

        // Get the HModule for ntdll.
        HMODULE hMod = GetModuleHandle(_T("ntdll.dll"));
        if (hMod==NULL) {
            return(0);
        }
        BOOL result = EnumResourceLanguages(hMod, Type, Name, (ENUMRESLANGPROC)EnumLangProc, (LONG_PTR) &LangInfo);
        if (!result || (LangInfo.Count > 2) || (LangInfo.Count < 1) ) {
            return (0);
        }
        return (LangInfo.LangID);
    }

    // Checks if NT4 system is Hongkong SAR version
    BOOL CFNLS::IsHongKongVersion()
    {
        HMODULE hMod;
        BOOL bRet = FALSE;
        typedef BOOL (WINAPI *IMMRELEASECONTEXT)(HWND,HIMC);
        IMMRELEASECONTEXT pImmReleaseContext;

        hMod = LoadLibrary(_T("imm32.dll"));
        if (hMod) {
            pImmReleaseContext = (IMMRELEASECONTEXT)GetProcAddress(hMod,"ImmReleaseContext");
            if (pImmReleaseContext) {
                bRet = pImmReleaseContext(NULL,NULL);
            }
            FreeLibrary(hMod);
        }
        return (bRet);
    }

    LANGID CFNLS::DetectUILanguage()
    {
        //OSVERSIONINFO		VersionInfo;
        LANGID				uiLangID = 0;
        HKEY				hKey;
        DWORD				Type, BuffLen = MAX_BUFFER_LENGTH;
        TCHAR				LangKeyValue[MAX_BUFFER_LENGTH];

        if (CFOSInfo::Instance()->IsGreaterWinNT())
        {
            // On Windows NT, Windows 2000 or higher
            if (CFOSInfo::Instance()->GetOSType() >= CFOSInfo::ostWin2000)
            { 
                // Windows 2000 or higher
                uiLangID = GetUserDefaultUILanguage();
            }
            else
            {
                // for NT4 check the language of ntdll.dll
                uiLangID = GetNTDLLNativeLangID();   
                if (uiLangID == 1033) {		// special processing for Honkong SAR version of NT4
                    if (IsHongKongVersion()) {
                        uiLangID = 3076;
                    }
                }
            }
        }
        else
        {
            // On Windows 98 or Windows ME
            // Open the registry key for the UI language
            if( RegOpenKeyEx(HKEY_CURRENT_USER,_T("Default\\Control Panel\\Desktop\\ResourceLocale"), 0,
                KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS ) {
                    // Get the type of the default key
                    if( RegQueryValueEx(hKey, NULL, NULL, &Type, NULL, NULL) == ERROR_SUCCESS 
                        && Type == REG_SZ ) {
                            // Read the key value
                            if( RegQueryValueEx(hKey, NULL, NULL, &Type, (LPBYTE)LangKeyValue, &BuffLen) 
                                == ERROR_SUCCESS ) {
                                    uiLangID = (LANGID)_ttoi(LangKeyValue);
                            }
                    }
                    RegCloseKey(hKey);
            }				
        }
        if (uiLangID == 0) 
        {
            uiLangID = GetUserDefaultLangID();
        }
        // Return the found language ID.
        return (uiLangID);
    }

    //////////////////////////////////////////////////////////////////////////
	LPCTSTR CFIMEUtil::GetCompositionValueString(CFStringFormater& formater, UINT nGCSValue, LPCTSTR pszDivide /* = TEXT */)
	{
		UINT    nOldGCSValue = nGCSValue;

		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nGCSValue, GCS_COMPREADSTR, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nGCSValue, GCS_COMPREADATTR, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nGCSValue, GCS_COMPREADCLAUSE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nGCSValue, GCS_COMPSTR, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nGCSValue, GCS_COMPATTR, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nGCSValue, GCS_COMPCLAUSE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nGCSValue, GCS_CURSORPOS, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nGCSValue, GCS_DELTASTART, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nGCSValue, GCS_RESULTREADSTR, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nGCSValue, GCS_RESULTREADCLAUSE, pszDivide);

		//此时已经转换完成？可以通过 ImmGetCompositionString(hIMC, GCS_RESULTSTR, lpstr, dwSize) 获取结果
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nGCSValue, GCS_RESULTSTR, pszDivide);

		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nGCSValue, GCS_RESULTCLAUSE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nGCSValue, CS_INSERTCHAR, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nGCSValue, CS_NOMOVECARET, pszDivide);

		FTLASSERT( 0 == nGCSValue);
		if (0 != nGCSValue)
		{
			FTLTRACEEX(FTL::tlWarning, TEXT("%s: Get Composition Value String Not Complete, total=0x%08x, remain=0x%08x\n"),
				__FILE__LINE__, nOldGCSValue, nGCSValue);
		}
		return formater.GetString();
	}

	LPCTSTR CFIMEUtil::GetIMENotifyInfoString(CFStringFormater& formater, WPARAM wParam, LPARAM lParam)
	{
		UINT nNotifyCmd = (UINT)(wParam);
		UNREFERENCED_PARAMETER(lParam);

		nNotifyCmd &= 0xF;		//根据实测，输入韩文时 nNotifyCmd 为 0x010X, 多了 0x0100(但MSDN中没有写)
		//FTLASSERT(nNotifyCmd == wParam);
		switch (nNotifyCmd)
		{
			HANDLE_CASE_TO_STRING_FORMATER(formater, IMN_CLOSESTATUSWINDOW);
			HANDLE_CASE_TO_STRING_FORMATER(formater, IMN_OPENSTATUSWINDOW);
			HANDLE_CASE_TO_STRING_FORMATER(formater, IMN_CHANGECANDIDATE);
			HANDLE_CASE_TO_STRING_FORMATER(formater, IMN_CLOSECANDIDATE);
			HANDLE_CASE_TO_STRING_FORMATER(formater, IMN_OPENCANDIDATE);
			HANDLE_CASE_TO_STRING_FORMATER(formater, IMN_SETCONVERSIONMODE);
			HANDLE_CASE_TO_STRING_FORMATER(formater, IMN_SETSENTENCEMODE);
			HANDLE_CASE_TO_STRING_FORMATER(formater, IMN_SETOPENSTATUS);
			HANDLE_CASE_TO_STRING_FORMATER(formater, IMN_SETCANDIDATEPOS);
			HANDLE_CASE_TO_STRING_FORMATER(formater, IMN_SETCOMPOSITIONFONT);
			HANDLE_CASE_TO_STRING_FORMATER(formater, IMN_SETCOMPOSITIONWINDOW);
			HANDLE_CASE_TO_STRING_FORMATER(formater, IMN_SETSTATUSWINDOWPOS);
			HANDLE_CASE_TO_STRING_FORMATER(formater, IMN_GUIDELINE);
			HANDLE_CASE_TO_STRING_FORMATER(formater, IMN_PRIVATE);
		default:
			//FTLASSERT(FALSE);
			 
			formater.Format(TEXT("Unknown, 0x%x"), nNotifyCmd);
			break;
		}
		return formater.GetString();
	}
}

#endif //FTL_NLS_HPP