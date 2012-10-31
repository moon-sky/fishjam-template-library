//------------------------------------------------------------------------------
// File: TextType.cpp
//
// Desc: DirectShow sample code - implementation of CTextMediaType class.
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "TextMediaType.h"

#define DETECT_OTHER_GUID_NAMES

#ifdef DETECT_OTHER_GUID_NAMES


GUID_STRING_ENTRY g_OtherGuidNames[] = {
    #define OUR_GUID_ENTRY(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
        { #name, { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } } },

	//{205769BC-B121-4ca8-A4E6-84A20EF253B7}
	OUR_GUID_ENTRY(CLSID_DebugInfoFilter,
		0x205769BC,0xB121,0x4ca8,0xA4,0xE6,0x84,0xA2,0x0E,0xF2,0x53,0xB7)


	//以下内容是DirectShow中的Sample Filter
	#include "DShowSampleUuids.h"

	//以下内容是网上的一些开源软件的GUID
	#include "OpenSourceUuids.h"

	//以下内容是Windows SDK 中得到的，如 <wmsdkidl.h>
	#include "WinSDKUuids.h"

    // MPEG4 AAC
    // wFormatTag   0x0278
    // Format Block [WAVEFORMAT_EX][ES_ID(16bit)]
    // MEDIATYPE_Audio
    // MEDIASUBTYPE_SonyMpeg4AAC
    OUR_GUID_ENTRY(MEDIASUBTYPE_SonyMpeg4AAC,
        0x00000278, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
	
};

int g_cOtherGuidNames = sizeof(g_OtherGuidNames) / sizeof(g_OtherGuidNames[0]);

#endif 


HRESULT CTextMediaType::AsText(FTL::CFStringFormater& strFormater)
{
	return ERROR_CALL_NOT_IMPLEMENTED;
}
//
// AsText
//
// Return the media type as a text string. Will place szAfterMajor after
// the text string for the major type and szAfterOthers after all other
// strings, apart from the last one.
//
HRESULT CTextMediaType::AsText(LPTSTR szType, unsigned int iLen)
{
    ASSERT(szType);
    HRESULT hr = E_FAIL;

    //
    // Convert Majortype to string
    //
	TCHAR szMajorType[100] = {0};
    UINT  iMajorType = 100;

    COM_VERIFY(CLSID2String(szMajorType, iMajorType, &majortype));
    szMajorType[99] = 0;        // Ensure null-termination

    //
    // Convert Subtype to string
    //
	TCHAR szSubType[100] = {0};
    UINT  iSubType = 100;
    COM_VERIFY(CLSID2String(szSubType, iSubType, &subtype));
    szSubType[99] = 0;        // Ensure null-termination

    //
    // Convert Format to string
    //
    TCHAR szFormat[500];
    COM_VERIFY(Format2String(szFormat, _countof(szFormat), FormatType(), Format(), FormatLength()));
    szFormat[_countof(szFormat) -1 ] = 0;        // Ensure null-termination

    COM_VERIFY(StringCchPrintf(szType, iLen, TEXT("Major:%s;  Sub:%s;  Format:%s"),
        szMajorType,szSubType, szFormat)); //GetSubtypeName(&subtype)

    return hr;
}


//
// CLSID2String
//
// Find a string description for a given GUID
//
HRESULT CTextMediaType::CLSID2String(LPTSTR szBuffer,
                                  UINT iLength,
                                  const GUID* pGuid)
{
    ASSERT(szBuffer);
    ASSERT(pGuid);
    HRESULT hr = S_OK;

    USES_CONVERSION;

    // Read the GUID name from the global table.  Use A2T macro
    // to convert to UNICODE string if necessary.

    TCHAR *pszGuidName = NULL;

#ifdef DETECT_OTHER_GUID_NAMES
    for (int i = 0; i < g_cOtherGuidNames; i++) {
        if (g_OtherGuidNames[i].guid == *pGuid) {
            pszGuidName =  A2T(g_OtherGuidNames[i].szName);
        }
    }
#endif //DETECT_OTHER_GUID_NAMES

    if (NULL == pszGuidName)
    {
        //CBasePin::DisplayTypeInfo 中为什么是 %hs 
        pszGuidName = A2T(GuidNames[*pGuid]);
    }

	TCHAR szGuidString[160] = {0};
	if (_tcscmp(pszGuidName, TEXT("Unknown GUID Name")) == 0)
	{
		StringFromGUID2(*pGuid, szGuidString, _countof(szGuidString));
		FTLTRACE(TEXT("CTextMediaType Unknown GUID %s\n"), szGuidString);
		pszGuidName = szGuidString;
		hr = S_FALSE;
	}
    size_t nLength = _tcslen(pszGuidName);

    _tcsncpy(szBuffer, pszGuidName, min(nLength, iLength));
    szBuffer[nLength] = 0;      // Ensure null-termination

    return hr;
}

HRESULT CTextMediaType::GetFilenameByCLSIDString(const WCHAR *szGUID, WCHAR *szFile, size_t cch)
{
    HRESULT hr = E_FAIL;
    const DWORD STR_LEN = 512;
	WCHAR szKey[STR_LEN] = {0};

    int rc=0;
    // Create key name for reading filename from registry
    COM_VERIFY(StringCchPrintf(
        szKey, STR_LEN, 
        L"Software\\Classes\\CLSID\\%s\\InprocServer32\0",
        szGUID));

    if (SUCCEEDED(hr))
    {
        HKEY hkeyFilter=0;
        DWORD dwSize=MAX_PATH;
        BYTE szFilename[MAX_PATH] = {0};

        // Open the CLSID key that contains information about the filter
        rc = RegOpenKey(HKEY_LOCAL_MACHINE, szKey, &hkeyFilter);
        if (rc == ERROR_SUCCESS)
        {
            rc = RegQueryValueEx(hkeyFilter, NULL,  // Read (Default) value
                NULL, NULL, szFilename, &dwSize);

            if (rc == ERROR_SUCCESS)
            {
                hr = StringCchPrintf(szFile, cch, L"%s\0", szFilename);
            }
            else
            {
                hr = E_FAIL;
            }
            RegCloseKey(hkeyFilter);
        }
        else
        {
            hr = E_FAIL;
        }
    }
    return hr;
}

HRESULT CTextMediaType::GetFilenameByCLSID(const GUID *pGUID, WCHAR *szFile, size_t cch)
{
    HRESULT hr = E_FAIL;
    const DWORD GUID_STR_LEN = 40;
    WCHAR szGUID[GUID_STR_LEN];
    if (StringFromGUID2(*pGUID, szGUID, GUID_STR_LEN) > 0)
    {
        COM_VERIFY(GetFilenameByCLSIDString(szGUID, szFile, cch));
    }
    return hr;
}

//
// Format2String
//
// Converts a format block to a string
//
HRESULT CTextMediaType::Format2String(LPTSTR szBuffer,
                                   UINT iLength,
                                   const GUID* pFormatType,
                                   BYTE* pFormat,
                                   ULONG lFormatLength)
{
    UNREFERENCED_PARAMETER(lFormatLength);
    ASSERT(szBuffer);
    ASSERT(pFormatType);
    HRESULT hr = E_FAIL;

    //
    // Get the name of the format
    //
	TCHAR szName[50] = {0};
    UINT iName = 50;
    COM_VERIFY(CLSID2String(szName, iName, pFormatType));
    szName[49] = 0;     // Ensure null-termination

    //
    // Video Format
    //
    if (IsEqualGUID(*pFormatType, FORMAT_VideoInfo) ||
		//IsEqualGUID(*pFormatType, FORMAT_VideoInfo2) ||
        IsEqualGUID(*pFormatType, FORMAT_MPEGVideo) ||
		IsEqualGUID(*pFormatType, FORMAT_MPEG2Video)  //MPEG2VIDEOINFO
		)
    {
		return _GetVideoInfoHeaderInfo(szBuffer, iLength, szName, pFormat, lFormatLength);
    }

	//
    // Audio Format
    //
    if (IsEqualGUID(*pFormatType, FORMAT_WaveFormatEx)) 
    {
		return _GetWaveFormatInfo(szBuffer, iLength, szName, pFormat, lFormatLength);
    }

    COM_VERIFY(StringCchPrintf(szBuffer, iLength, TEXT("%s"), szName));
    return hr;
}

HRESULT CTextMediaType::_GetVideoInfoHeaderInfo(LPTSTR szBuffer, 
	UINT iLength, LPCTSTR pszInfo, BYTE* pFormat, ULONG lFormatLength)
{
	HRESULT hr = S_OK;
	VIDEOINFOHEADER * pVideoFormat = (VIDEOINFOHEADER *) pFormat;

	if (pVideoFormat)
	{
		CString strCompression;
		DWORD dwCompression = pVideoFormat->bmiHeader.biCompression;
		if (0 == dwCompression)
		{
			strCompression = TEXT("RGB");
		}
		else if(BI_BITFIELDS == dwCompression)
		{
			strCompression = TEXT("BITF");
		}
		else
		{
			CHAR* pchCompression = (CHAR*)&dwCompression;
			strCompression.Format(TEXT("%c%c%c%c(0x%x)"), 
				pchCompression[0], pchCompression[1],pchCompression[2], pchCompression[3],
				dwCompression);
		}
		COM_VERIFY(StringCchPrintf(szBuffer, iLength, TEXT("%s %dx%d, %d bits, %d SizeImage")
			, strCompression
			, pVideoFormat->bmiHeader.biWidth
			, pVideoFormat->bmiHeader.biHeight
			, pVideoFormat->bmiHeader.biBitCount
			, pVideoFormat->bmiHeader.biSizeImage));
	}
	return hr;
}

HRESULT CTextMediaType::_GetWaveFormatInfo(LPTSTR szBuffer, UINT iLength, 
						   LPCTSTR pszInfo, BYTE* pFormat, ULONG lFormatLength)
{
	HRESULT hr = S_OK;

	WAVEFORMATEX *pWaveFormat = (WAVEFORMATEX *) pFormat;
	if (pWaveFormat)
	{
		// !!! use ACM to get format type name?
		COM_VERIFY(StringCchPrintf(szBuffer, iLength
			, TEXT("%s: Tag(0x%04x,%s), nChannel(%d), %.3f KHz, %.3f kbps, ")
			  TEXT("BlockAlign(%d), BitsPerSample(%d), cbSize(%d)")
			, pszInfo
			, pWaveFormat->wFormatTag, FTL::CFDirectShowUtility::GetWaveFormatExTagString(pWaveFormat->wFormatTag)
			, pWaveFormat->nChannels
			, (double) pWaveFormat->nSamplesPerSec / 1000.0
			, (double)(pWaveFormat->nAvgBytesPerSec * 8) / 1000.0
			, pWaveFormat->nBlockAlign
			, pWaveFormat->wBitsPerSample
			, pWaveFormat->cbSize
			));
	}
	return hr;

}
