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

        CFOSInfo osInfo;
        if (osInfo.IsGreaterWinNT())
        {
            // On Windows NT, Windows 2000 or higher
            if (osInfo.GetOSType() >= CFOSInfo::ostWin2000)
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
    CFConversion::CFConversion(UINT CodePage /* = CP_ACP */)
        :m_codePage(CodePage)
    {

    }
    CFConversion::~CFConversion()
    {

    }

    LPWSTR CFConversion::UTF8_TO_UTF16( LPCSTR szUTF8 )
    {
        int wcharsize = MultiByteToWideChar( CP_UTF8, 0,  szUTF8, -1, NULL, 0 );
        int size = wcharsize * sizeof( WCHAR );
        ATLVERIFY( MultiByteToWideChar( CP_UTF8, 0,  szUTF8, -1, ( LPWSTR )m_Mem.GetMemory( size ), wcharsize ) );

        return ( LPWSTR )( BYTE* )m_Mem;
    }

    LPSTR  CFConversion::UTF8_TO_MBCS( LPCSTR szUTF8 )
    {
        int wcharsize = MultiByteToWideChar( CP_UTF8, 0,  szUTF8, -1, NULL, 0 );
        int size = wcharsize * sizeof( WCHAR );
        ATLVERIFY( MultiByteToWideChar( CP_UTF8, 0,  szUTF8, -1, ( LPWSTR )m_Mem.GetMemory( size ), wcharsize ) );

        int charsize = WideCharToMultiByte( m_codePage, 0, ( LPWSTR )( BYTE* )m_Mem, -1, NULL, 0, NULL, NULL );
        size = charsize * sizeof( char );
        ATLVERIFY( WideCharToMultiByte( m_codePage, 0, ( LPWSTR )( BYTE* )m_Mem, -1, ( LPSTR )m_Mem2.GetMemory( size ), size, 0, 0 ) ); // NS

        return ( LPSTR )( BYTE* )m_Mem2;
    }

    LPWSTR CFConversion::MBCS_TO_UTF16( LPCSTR szMBCS )
    {
        int wcharsize = MultiByteToWideChar( m_codePage, 0,  szMBCS, -1, NULL, 0 );
        int size = wcharsize * sizeof( WCHAR );
        ATLVERIFY( MultiByteToWideChar( m_codePage, 0,  szMBCS, -1, ( LPWSTR )m_Mem.GetMemory( size ), wcharsize ) );

        return ( LPWSTR )( BYTE* )m_Mem;
    }

    LPSTR  CFConversion::MBCS_TO_UTF8( LPCSTR szMBCS )
    {
        int wcharsize = MultiByteToWideChar( m_codePage, 0,  szMBCS, -1, NULL, 0 );
        int size = wcharsize * sizeof( WCHAR );
        ATLVERIFY( MultiByteToWideChar( m_codePage, 0,  szMBCS, -1, ( LPWSTR )m_Mem.GetMemory( size ), wcharsize ) );

        int charsize = WideCharToMultiByte( CP_UTF8, 0, ( LPWSTR )( BYTE* )m_Mem, -1, NULL, 0, NULL, NULL );
        size = charsize * sizeof( char );
        ATLVERIFY( WideCharToMultiByte( CP_UTF8, 0, ( LPWSTR )( BYTE* )m_Mem, -1, ( LPSTR )m_Mem2.GetMemory( size ), size, 0, 0 ) ); // NS

        return ( LPSTR )( BYTE* )m_Mem2;
    }

    LPSTR  CFConversion::UTF16_TO_MBCS( LPCWSTR szUTF16 )
    {
        int charsize = WideCharToMultiByte( m_codePage, 0, szUTF16, -1, NULL, 0, NULL, NULL );
        int size = charsize * sizeof( char );
        ATLVERIFY( WideCharToMultiByte( m_codePage, 0, szUTF16, -1, ( LPSTR )m_Mem.GetMemory( size ), size, 0, 0 ) );

        return ( LPSTR )( BYTE* )m_Mem;
    }

    LPSTR  CFConversion::WCS_TO_MBCS( const UINT CodePage, LPCWSTR szWcs )
    {
        int charsize = WideCharToMultiByte( CodePage, 0, szWcs, -1, NULL, 0, NULL, NULL );
        int size = charsize * sizeof( char );
        ATLVERIFY( WideCharToMultiByte( CodePage, 0, szWcs, -1, ( LPSTR )m_Mem.GetMemory( size ), size, 0, 0 ) );

        return ( LPSTR )( BYTE* )m_Mem;
    }

    LPWSTR  CFConversion::MBCS_TO_WCS( const UINT CodePage, LPCSTR szMBCS )
    {
        int wcharsize = MultiByteToWideChar( CodePage, 0,  szMBCS, -1, NULL, 0 );
        int size = wcharsize * sizeof( WCHAR );
        MultiByteToWideChar( CodePage, 0,  szMBCS, -1, ( LPWSTR )m_Mem.GetMemory( size ), size );

        return ( LPWSTR )( BYTE* )m_Mem;
    }

    LPSTR  CFConversion::UTF16_TO_UTF8( LPCWSTR szUTF16 )
    {
        int charsize = WideCharToMultiByte( CP_UTF8, 0, szUTF16, -1, NULL, 0, NULL, NULL );
        int size = charsize * sizeof( char );
        ATLVERIFY( WideCharToMultiByte( CP_UTF8, 0, szUTF16, -1, ( LPSTR )m_Mem.GetMemory( size ), size, 0, 0 ) );

        return ( LPSTR )( BYTE* )m_Mem;
    }

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