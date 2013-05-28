#ifndef FTL_CONVERSION_HPP
#define FTL_CONVERSION_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlConversion.h"
#endif


namespace FTL
{
	CFConversion::CFConversion(UINT CodePage /* = CP_ACP */, DWORD dwFlags /* = 0 */)
		:m_codePage(CodePage)
		,m_dwFlags(dwFlags)
	{
		m_pDefaultChar = NULL;
		m_bUsedDefaultChar = FALSE;
	}
	CFConversion::~CFConversion()
	{
		SAFE_DELETE_ARRAY(m_pDefaultChar);
	}

	VOID CFConversion::SetDefaultCharForWC2MB(LPSTR pszDefaultChar)
	{
		FTLASSERT(pszDefaultChar != m_pDefaultChar);

		SAFE_DELETE_ARRAY(m_pDefaultChar);
		if (pszDefaultChar)
		{
			int nLength = strlen(pszDefaultChar);
			m_pDefaultChar = new CHAR[nLength + 1];
			if (m_pDefaultChar)
			{
				strncpy(m_pDefaultChar, pszDefaultChar, nLength);
				m_pDefaultChar[nLength] = NULL;
			}
		}
	}
	LPWSTR CFConversion::UTF8_TO_UTF16(LPCSTR szUTF8 , BOOL bDetached /* = FALSE */)
	{
		BOOL bRet = FALSE;
		int wcharsize = MultiByteToWideChar( CP_UTF8, m_dwFlags,  szUTF8, -1, NULL, 0 );
		int size = wcharsize * sizeof( WCHAR );
		API_VERIFY( 0 != MultiByteToWideChar( CP_UTF8, m_dwFlags,  szUTF8, -1, ( LPWSTR )m_Mem.GetMemory( size ), wcharsize ) );
		if (bDetached)
		{
			return (LPWSTR)m_Mem.Detatch();
		}
		return ( LPWSTR )( BYTE* )m_Mem;
	}

	LPSTR  CFConversion::UTF8_TO_MBCS( LPCSTR szUTF8 , BOOL bDetached /* = FALSE */ )
	{
		BOOL bRet = FALSE;
		int wcharsize = MultiByteToWideChar( CP_UTF8, m_dwFlags,  szUTF8, -1, NULL, 0 );
		int size = wcharsize * sizeof( WCHAR );
		API_VERIFY( 0 != MultiByteToWideChar( CP_UTF8, m_dwFlags,  szUTF8, -1, ( LPWSTR )m_Mem.GetMemory( size ), wcharsize ) );

		int charsize = WideCharToMultiByte( m_codePage, m_dwFlags, ( LPWSTR )( BYTE* )m_Mem, -1, NULL, 0, 
			m_pDefaultChar, &m_bUsedDefaultChar);
		size = charsize * sizeof( char );
		API_VERIFY( 0 != WideCharToMultiByte( m_codePage, m_dwFlags, ( LPWSTR )( BYTE* )m_Mem, -1, 
			( LPSTR )m_Mem2.GetMemory( size ), size, m_pDefaultChar, &m_bUsedDefaultChar ) ); // NS
		if (bDetached)
		{
			return (LPSTR)m_Mem2.Detatch();
		}
		return ( LPSTR )( BYTE* )m_Mem2;
	}

	LPWSTR CFConversion::MBCS_TO_UTF16( LPCSTR szMBCS , BOOL bDetached /* = FALSE */)
	{
		BOOL bRet = FALSE;
		int wcharsize = MultiByteToWideChar( m_codePage, m_dwFlags,  szMBCS, -1, NULL, 0 );
		int size = wcharsize * sizeof( WCHAR );
		API_VERIFY(0 != MultiByteToWideChar( m_codePage, m_dwFlags,  szMBCS, -1, ( LPWSTR )m_Mem.GetMemory( size ), wcharsize ) );

		if (bDetached)
		{
			return (LPWSTR)m_Mem.Detatch();
		}
		return ( LPWSTR )( BYTE* )m_Mem;
	}

	LPSTR  CFConversion::MBCS_TO_UTF8( LPCSTR szMBCS , BOOL bDetached /* = FALSE */ )
	{
		BOOL bRet = FALSE;
		int wcharsize = MultiByteToWideChar( m_codePage, m_dwFlags,  szMBCS, -1, NULL, 0 );
		int size = wcharsize * sizeof( WCHAR );
		API_VERIFY( MultiByteToWideChar( m_codePage, m_dwFlags,  szMBCS, -1, ( LPWSTR )m_Mem.GetMemory( size ), wcharsize ) );

		int charsize = WideCharToMultiByte( CP_UTF8, m_dwFlags, ( LPWSTR )( BYTE* )m_Mem, -1, NULL, 0, 
			m_pDefaultChar, &m_bUsedDefaultChar);
		size = charsize * sizeof( char );
		API_VERIFY( WideCharToMultiByte( CP_UTF8, m_dwFlags, ( LPWSTR )( BYTE* )m_Mem, -1, 
			( LPSTR )m_Mem2.GetMemory( size ), size, m_pDefaultChar, &m_bUsedDefaultChar ) ); // NS

		if (bDetached)
		{
			return (LPSTR)m_Mem2.Detatch();
		}
		return ( LPSTR )( BYTE* )m_Mem2;
	}

	LPSTR  CFConversion::UTF16_TO_MBCS( LPCWSTR szUTF16 , BOOL bDetached /* = FALSE */ )
	{
		BOOL bRet = FALSE;
		int charsize = WideCharToMultiByte( m_codePage, m_dwFlags, szUTF16, -1, NULL, 0, NULL, NULL );
		int size = charsize * sizeof( char );
		API_VERIFY(0 != WideCharToMultiByte( m_codePage, m_dwFlags, szUTF16, -1, 
			( LPSTR )m_Mem.GetMemory( size ), size, m_pDefaultChar, &m_bUsedDefaultChar ) );

		if (bDetached)
		{
			return (LPSTR)m_Mem.Detatch();
		}
		return ( LPSTR )( BYTE* )m_Mem;
	}

	LPSTR  CFConversion::WCS_TO_MBCS( const UINT CodePage, LPCWSTR szWcs  , BOOL bDetached /* = FALSE */)
	{
		BOOL bRet = FALSE;
		int charsize = WideCharToMultiByte( CodePage, m_dwFlags, szWcs, -1, NULL, 0, NULL, NULL );
		int size = charsize * sizeof( char );
		API_VERIFY( 0 != WideCharToMultiByte( CodePage, m_dwFlags, szWcs, -1, 
			( LPSTR )m_Mem.GetMemory( size ), size, m_pDefaultChar, &m_bUsedDefaultChar ) );

		if (bDetached)
		{
			return (LPSTR)m_Mem.Detatch();
		}
		return ( LPSTR )( BYTE* )m_Mem;
	}

	LPWSTR  CFConversion::MBCS_TO_WCS( const UINT CodePage, LPCSTR szMBCS , BOOL bDetached /* = FALSE */ )
	{
		BOOL bRet = FALSE;
		int wcharsize = MultiByteToWideChar( CodePage, m_dwFlags,  szMBCS, -1, NULL, 0 );
		int size = wcharsize * sizeof( WCHAR );
		API_VERIFY(0 != MultiByteToWideChar( CodePage, m_dwFlags,  szMBCS, -1, ( LPWSTR )m_Mem.GetMemory( size ), size ));

		if (bDetached)
		{
			return (LPWSTR)m_Mem.Detatch();
		}
		return ( LPWSTR )( BYTE* )m_Mem;
	}

	LPSTR  CFConversion::UTF16_TO_UTF8( LPCWSTR szUTF16 , BOOL bDetached /* = FALSE */)
	{
		BOOL bRet = FALSE;
		int charsize = WideCharToMultiByte( CP_UTF8, m_dwFlags, szUTF16, -1, NULL, 0, NULL, NULL );
		int size = charsize * sizeof( char );
		API_VERIFY( 0 != WideCharToMultiByte( CP_UTF8, m_dwFlags, szUTF16, -1, 
			( LPSTR )m_Mem.GetMemory( size ), size, NULL, NULL ) );

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

}

#endif //FTL_CONVERSION_HPP