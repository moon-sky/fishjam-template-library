#ifndef FTL_MULTI_MEDIA_HPP
#define FTL_MULTI_MEDIA_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlMultiMedia.h"
#endif

namespace FTL
{
	CAtlString CFMultiMediaUtil::GetContentType(LPCTSTR pszFileName)
	{
		FTLASSERT(pszFileName);
		CAtlString strContentType = TEXT("application/octet-stream");

		if (pszFileName)
		{
			HRESULT hr = E_FAIL;
			LPTSTR pszExtName = StrRChr(pszFileName, NULL, TEXT('.'));
			if (pszExtName)
			{
				TCHAR szReturn[MAX_PATH] = {0};
				DWORD dwLength = sizeof(szReturn);

				HKEY hKey = NULL;
				LONG lRet = ERROR_SUCCESS;
				REG_VERIFY(RegOpenKeyEx(HKEY_CLASSES_ROOT, pszExtName, 0, KEY_QUERY_VALUE, &hKey));
				if (ERROR_SUCCESS == lRet)
				{
					REG_VERIFY(RegQueryValueEx(hKey, TEXT("Content Type"), NULL, NULL, (BYTE*)&szReturn, &dwLength));
					if (ERROR_SUCCESS == lRet)
					{
						strContentType = szReturn;
					}
					RegCloseKey(hKey);
				}
			}
		}
		return strContentType;
	}
}

#endif //FTL_MULTI_MEDIA_HPP