#ifndef FTL_IE_HELPER_HPP
#define FTL_IE_HELPER_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlIEHelper.h"
#endif
#include "ftlComDetect.h"

namespace FTL
{
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
				HRESULT Res = IEIsProtectedModeProcessProc(&IsProtecedMode);
				if (FAILED(Res))
				{
					FTLASSERT(E_ACCESSDENIED == 0x80070005);
					if (E_ACCESSDENIED == Res)
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
}

#endif //FTL_IE_HELPER_HPP