#ifndef FTL_MULTI_MEDIA_HPP
#define FTL_MULTI_MEDIA_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlMultiMedia.h"
#endif

namespace FTL
{
	BOOL CFMultiMediaUtil::GetContentType(LPCTSTR pszFileName, LPTSTR pszContentType, DWORD dwCharSize)
	{
		FTLASSERT(pszFileName);

		if (!pszFileName)
		{
			SetLastError(ERROR_INVALID_PARAMETER);
			return FALSE;
		}

		//Set Default Value
		StringCchCopy(pszContentType, dwCharSize, TEXT("application/octet-stream"));
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
					DWORD dwBufferSize = dwCharSize * sizeof(TCHAR);
					REG_VERIFY(RegQueryValueEx(hKey, TEXT("Content Type"), NULL, NULL, (BYTE*)pszContentType, &dwBufferSize));
					RegCloseKey(hKey);
				}
			}
		}
		return TRUE;
	}
}

#endif //FTL_MULTI_MEDIA_HPP