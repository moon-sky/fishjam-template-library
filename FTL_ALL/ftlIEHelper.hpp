#ifndef FTL_IE_HELPER_HPP
#define FTL_IE_HELPER_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlIEHelper.h"
#endif
#include "ftlComDetect.h"

namespace FTL
{
	HRESULT CFIEUtils::GetIEDocumentFromHWnd(HWND hWnd, IHTMLDocument** ppDocument)
	{
		HRESULT hr = E_FAIL;
		DWORD_PTR nRes = 0;

		UINT nMsg = ::RegisterWindowMessage( TEXT("WM_HTML_GETOBJECT") );
		::SendMessageTimeout( hWnd, nMsg, 0L, 0L, SMTO_ABORTIFHUNG, 1000, (DWORD_PTR*)&nRes );
		hr = ObjectFromLresult( nRes, IID_IHTMLDocument, 0, (void**)&ppDocument );
		return hr;
	}

	BOOL CFIEUtils::IsProtectedModeProcess()
	{
		typedef HRESULT STDAPICALLTYPE IEISPROTECTEDMODEPROCESSFN (BOOL *pbResult);
		typedef HRESULT STDAPICALLTYPE SHGETKNOWNFOLDERPATH(REFGUID clsidFolderID, DWORD dwFlags, HANDLE hToken, PWSTR* ppszPath);

		BOOL IsProtecedMode = FALSE;

		HINSTANCE Instance = LoadLibrary(_T("ieframe.dll") );
		if (Instance)
		{
			IEISPROTECTEDMODEPROCESSFN* IEIsProtectedModeProcessProc = (IEISPROTECTEDMODEPROCESSFN *) 
				GetProcAddress(Instance, "IEIsProtectedModeProcess");
			if (IEIsProtectedModeProcessProc)
			{
				HRESULT hr = IEIsProtectedModeProcessProc(&IsProtecedMode);
				if (FAILED(hr))
				{
					FTLASSERT(E_ACCESSDENIED == 0x80070005);
					if (E_ACCESSDENIED == hr)
					{
						IsProtecedMode = TRUE;    // access denied, assume protected mode
					}
					else
					{
						IsProtecedMode = FALSE;
					}
				}
			}
			FreeLibrary(Instance);
		}

		return IsProtecedMode;
	}

    BOOL CFIEUtils::IsEPMEnabled(){
        /*
        if (GetBrowserVersion() < IE_1000)
        {
            return FALSE;
        }

        TCHAR szREGValue[100];
        BOOL bOK = GetRegistryStr( HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\Main"), _T("Isolation"), _T(""), 
            szREGValue,  _countof(szREGValue));
        CAtlString strRegValue = szREGValue;

        if ( strRegValue.GetLength() < 0 || strRegValue == _T("PMIL"))
        {
            return FALSE;
        }
        */
        return TRUE;
    }
}

#endif //FTL_IE_HELPER_HPP