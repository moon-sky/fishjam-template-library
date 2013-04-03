#ifndef FTL_CONVERSION_HPP
#define FTL_CONVERSION_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlConversion.h"
#endif


namespace FTL
{
	CFConversion::CFConversion(UINT CodePage /* = CP_ACP */)
		:m_codePage(CodePage)
	{

	}
	CFConversion::~CFConversion()
	{

	}

	LPWSTR CFConversion::UTF8_TO_UTF16(LPCSTR szUTF8 , BOOL bDetached /* = FALSE */)
	{
		int wcharsize = MultiByteToWideChar( CP_UTF8, 0,  szUTF8, -1, NULL, 0 );
		int size = wcharsize * sizeof( WCHAR );
		ATLVERIFY( MultiByteToWideChar( CP_UTF8, 0,  szUTF8, -1, ( LPWSTR )m_Mem.GetMemory( size ), wcharsize ) );
		if (bDetached)
		{
			return (LPWSTR)m_Mem.Detatch();
		}
		return ( LPWSTR )( BYTE* )m_Mem;
	}

	LPSTR  CFConversion::UTF8_TO_MBCS( LPCSTR szUTF8 , BOOL bDetached /* = FALSE */ )
	{
		int wcharsize = MultiByteToWideChar( CP_UTF8, 0,  szUTF8, -1, NULL, 0 );
		int size = wcharsize * sizeof( WCHAR );
		ATLVERIFY( MultiByteToWideChar( CP_UTF8, 0,  szUTF8, -1, ( LPWSTR )m_Mem.GetMemory( size ), wcharsize ) );

		int charsize = WideCharToMultiByte( m_codePage, 0, ( LPWSTR )( BYTE* )m_Mem, -1, NULL, 0, NULL, NULL );
		size = charsize * sizeof( char );
		ATLVERIFY( WideCharToMultiByte( m_codePage, 0, ( LPWSTR )( BYTE* )m_Mem, -1, ( LPSTR )m_Mem2.GetMemory( size ), size, 0, 0 ) ); // NS
		if (bDetached)
		{
			return (LPSTR)m_Mem2.Detatch();
		}
		return ( LPSTR )( BYTE* )m_Mem2;
	}

	LPWSTR CFConversion::MBCS_TO_UTF16( LPCSTR szMBCS , BOOL bDetached /* = FALSE */)
	{
		int wcharsize = MultiByteToWideChar( m_codePage, 0,  szMBCS, -1, NULL, 0 );
		int size = wcharsize * sizeof( WCHAR );
		ATLVERIFY( MultiByteToWideChar( m_codePage, 0,  szMBCS, -1, ( LPWSTR )m_Mem.GetMemory( size ), wcharsize ) );

		if (bDetached)
		{
			return (LPWSTR)m_Mem.Detatch();
		}
		return ( LPWSTR )( BYTE* )m_Mem;
	}

	LPSTR  CFConversion::MBCS_TO_UTF8( LPCSTR szMBCS , BOOL bDetached /* = FALSE */ )
	{
		int wcharsize = MultiByteToWideChar( m_codePage, 0,  szMBCS, -1, NULL, 0 );
		int size = wcharsize * sizeof( WCHAR );
		ATLVERIFY( MultiByteToWideChar( m_codePage, 0,  szMBCS, -1, ( LPWSTR )m_Mem.GetMemory( size ), wcharsize ) );

		int charsize = WideCharToMultiByte( CP_UTF8, 0, ( LPWSTR )( BYTE* )m_Mem, -1, NULL, 0, NULL, NULL );
		size = charsize * sizeof( char );
		ATLVERIFY( WideCharToMultiByte( CP_UTF8, 0, ( LPWSTR )( BYTE* )m_Mem, -1, ( LPSTR )m_Mem2.GetMemory( size ), size, 0, 0 ) ); // NS

		if (bDetached)
		{
			return (LPSTR)m_Mem2.Detatch();
		}
		return ( LPSTR )( BYTE* )m_Mem2;
	}

	LPSTR  CFConversion::UTF16_TO_MBCS( LPCWSTR szUTF16 , BOOL bDetached /* = FALSE */ )
	{
		int charsize = WideCharToMultiByte( m_codePage, 0, szUTF16, -1, NULL, 0, NULL, NULL );
		int size = charsize * sizeof( char );
		ATLVERIFY( WideCharToMultiByte( m_codePage, 0, szUTF16, -1, ( LPSTR )m_Mem.GetMemory( size ), size, 0, 0 ) );

		if (bDetached)
		{
			return (LPSTR)m_Mem.Detatch();
		}
		return ( LPSTR )( BYTE* )m_Mem;
	}

	LPSTR  CFConversion::WCS_TO_MBCS( const UINT CodePage, LPCWSTR szWcs  , BOOL bDetached /* = FALSE */)
	{
		int charsize = WideCharToMultiByte( CodePage, 0, szWcs, -1, NULL, 0, NULL, NULL );
		int size = charsize * sizeof( char );
		ATLVERIFY( WideCharToMultiByte( CodePage, 0, szWcs, -1, ( LPSTR )m_Mem.GetMemory( size ), size, 0, 0 ) );

		if (bDetached)
		{
			return (LPSTR)m_Mem.Detatch();
		}
		return ( LPSTR )( BYTE* )m_Mem;
	}

	LPWSTR  CFConversion::MBCS_TO_WCS( const UINT CodePage, LPCSTR szMBCS , BOOL bDetached /* = FALSE */ )
	{
		int wcharsize = MultiByteToWideChar( CodePage, 0,  szMBCS, -1, NULL, 0 );
		int size = wcharsize * sizeof( WCHAR );
		MultiByteToWideChar( CodePage, 0,  szMBCS, -1, ( LPWSTR )m_Mem.GetMemory( size ), size );

		if (bDetached)
		{
			return (LPWSTR)m_Mem.Detatch();
		}
		return ( LPWSTR )( BYTE* )m_Mem;
	}

	LPSTR  CFConversion::UTF16_TO_UTF8( LPCWSTR szUTF16 , BOOL bDetached /* = FALSE */)
	{
		int charsize = WideCharToMultiByte( CP_UTF8, 0, szUTF16, -1, NULL, 0, NULL, NULL );
		int size = charsize * sizeof( char );
		ATLVERIFY( WideCharToMultiByte( CP_UTF8, 0, szUTF16, -1, ( LPSTR )m_Mem.GetMemory( size ), size, 0, 0 ) );

		if (bDetached)
		{
			return (LPSTR)m_Mem.Detatch();
		}
		return ( LPSTR )( BYTE* )m_Mem;
	}

#ifdef _UNICODE
	LPCTSTR CFConversion::UTF16_TO_TCHAR( LPCTSTR lp , BOOL bDetached /* = FALSE */)
	{
		if (bDetached)
		{
			int nSrc = lstrlen(lp) + 1;
			StringCchCopy((LPTSTR)m_Mem.GetMemory(nSrc * sizeof(TCHAR)), nSrc, lp);
			return (LPCTSTR)m_Mem.Detatch();
		}
		else
		{
			return lp;
		}
	}
	LPCTSTR CFConversion::TCHAR_TO_UTF16( LPCTSTR lp , BOOL bDetached /* = FALSE */)
	{
		if (bDetached)
		{
			int nSrc = lstrlen(lp) + 1;
			StringCchCopy((LPTSTR)m_Mem.GetMemory(nSrc * sizeof(TCHAR)), nSrc, lp);
			return (LPCTSTR)m_Mem.Detatch();
		}
		else
		{
			return lp;
		}
	}
#else	//Non _UNICODE
	LPCTSTR CFConversion::MBCS_TO_TCHAR( LPCTSTR lp , BOOL bDetached /* = FALSE */)
	{
		if (bDetached)
		{
			int nSrc = lstrlen(lp) + 1;
			StringCchCopy((LPTSTR)m_Mem.GetMemory(nSrc * sizeof(TCHAR)), nSrc, lp);
			return (LPCTSTR)m_Mem.Detatch();
		}
		else
		{
			return lp;
		}
	}
	LPCTSTR CFConversion::TCHAR_TO_MBCS( LPCTSTR lp , BOOL bDetached /* = FALSE */)
	{
		if (bDetached)
		{
			int nSrc = lstrlen(lp) + 1;
			StringCchCopy((LPTSTR)m_Mem.GetMemory(nSrc * sizeof(TCHAR)), nSrc, lp);
			return (LPCTSTR)m_Mem.Detatch();
		}
		else
		{
			return lp;
		}
	}
#endif //_UNICODE

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	LONG CFConvUtil::HexFromBinary(__in const BYTE* pBufSrc, __in LONG nSrcCount, 
		__out LPTSTR pBufDest, __inout long* pDestCount, 
		__in LPCTSTR pszDivision/* = NULL*/)
	{
		LPTSTR pLocalBuffTranslate = pBufDest;
		pLocalBuffTranslate [0] = _T('\0');

		for (long i = 0; i < nSrcCount; i++) {
			//nSrcCount -= nTransCount;
			//StringCchVPrintfEx(pLocalBuffTranslate, n)
			//pLocalBuffTranslate += _stprintf(pLocalBuffTranslate, _T("%02X"), pBufSrc[i]);
		}


		return *pDestCount;
	}
}

#endif //FTL_CONVERSION_HPP