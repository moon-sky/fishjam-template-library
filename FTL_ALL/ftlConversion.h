#ifndef FTL_CONVERSION_H
#define FTL_CONVERSION_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlConversion.h requires ftlbase.h to be included first
#endif

namespace FTL
{
	class CFConversion
	{
	public:
		FTLINLINE CFConversion(UINT CodePage = CP_ACP);
		FTLINLINE ~CFConversion();

		FTLINLINE LPWSTR UTF8_TO_UTF16( LPCSTR szUTF8 , BOOL bDetached = FALSE);
		FTLINLINE LPSTR  UTF8_TO_MBCS( LPCSTR szUTF8 , BOOL bDetached = FALSE);
		FTLINLINE LPWSTR MBCS_TO_UTF16( LPCSTR szMBCS , BOOL bDetached = FALSE);
		FTLINLINE LPSTR  MBCS_TO_UTF8( LPCSTR szMBCS , BOOL bDetached = FALSE);
		FTLINLINE LPSTR  UTF16_TO_MBCS( LPCWSTR szUTF16 , BOOL bDetached = FALSE);
		FTLINLINE LPSTR  WCS_TO_MBCS( const UINT CodePage, LPCWSTR szWcs , BOOL bDetached = FALSE);
		FTLINLINE PWSTR  MBCS_TO_WCS( const UINT CodePage, LPCSTR szMBCS , BOOL bDetached = FALSE);
		FTLINLINE LPSTR  UTF16_TO_UTF8( LPCWSTR szUTF16 , BOOL bDetached = FALSE);

#ifdef _UNICODE
#define UTF8_TO_TCHAR	UTF8_TO_UTF16
#define TCHAR_TO_UTF8	UTF16_TO_UTF8
		FTLINLINE LPCTSTR UTF16_TO_TCHAR( LPCTSTR lp , BOOL bDetached = FALSE);
		FTLINLINE LPCTSTR TCHAR_TO_UTF16( LPCTSTR lp , BOOL bDetached = FALSE);

#define MBCS_TO_TCHAR	MBCS_TO_UTF16
#define TCHAR_TO_MBCS	UTF16_TO_MBCS

#else
#define UTF8_TO_TCHAR	UTF8_TO_MBCS
#define UTF16_TO_TCHAR	UTF16_TO_MBCS
#define TCHAR_TO_UTF8	MBCS_TO_UTF8
#define TCHAR_TO_UTF16	MBCS_TO_UTF16

		FTLINLINE LPCTSTR MBCS_TO_TCHAR( LPCTSTR lp , BOOL bDetached = FALSE);
		FTLINLINE LPCTSTR TCHAR_TO_MBCS( LPCTSTR lp , BOOL bDetached = FALSE);
#endif
	private:
		UINT    m_codePage;
		CFMemAllocator<BYTE> m_Mem;
		CFMemAllocator<BYTE> m_Mem2;
	};


	class CFConvUtil
	{
	public:
		//ex. pb = {255,15} -> sz = {"FF0F"} -- TODO: AtlHexDecode/AtlHexEncode
		//FTLINLINE static LONG HexFromBinary(__in const BYTE* pBufSrc, __in LONG nSrcCount, 
		//	__out LPTSTR pBufDest, __inout long* pDestCount, 
		//	__in LPCTSTR pszDivision = NULL);

	};
}

#endif //FTL_CONVERSION_H

#ifndef USE_EXPORT
# include "ftlConversion.hpp"
#endif //USE_EXPORT