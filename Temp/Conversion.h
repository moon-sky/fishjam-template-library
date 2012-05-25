#pragma once
#include <wtypes.h>
#include <crtdbg.h>
#include "WtlCustomDefile.h"
#define CP_NAVERTOOLBAR CP_ACP //949 // CP_NAVERTOOLBAR

class CMemAllocator
{
#define MA_BLOCK_SIZE		1024

private:
	KLOCWORK_CL_FFM( CMemAllocator ); // NS

public:
	CMemAllocator()
		: m_pMem ( NULL )
		, m_nSize ( MA_BLOCK_SIZE )
	{
		Init( MA_BLOCK_SIZE );
	}

	CMemAllocator( UINT nSize )
		: m_pMem ( NULL )
		, m_nSize ( MA_BLOCK_SIZE )
	{
		Init( nSize );
	}

	void Init( UINT nSize )
	{
		if ( nSize > MA_BLOCK_SIZE )
		{
			m_nSize = nSize;
			m_pMem = new BYTE[nSize];
		}

#ifdef _DEBUG
		m_dwThreadID = GetCurrentThreadId();
#endif
	}

	~CMemAllocator()
	{
		if ( m_pMem )
		{
			delete []m_pMem;
		}
	}

	PVOID GetMemory( UINT nMaxSize )
	{
#ifdef _DEBUG
		_ASSERTE( m_dwThreadID == GetCurrentThreadId() );
#endif
		if ( !m_pMem && nMaxSize <= MA_BLOCK_SIZE )
		{
			return m_FixedMem;
		}

		if ( m_nSize < nMaxSize || NULL == m_pMem )
		{
			if ( m_pMem )
			{
				delete []m_pMem;
				m_pMem = 0;
			}

			m_nSize = GetBlockSize( nMaxSize );
			m_pMem = new BYTE[m_nSize];
		}

		return m_pMem;
	}

	operator PVOID() const
	{
#ifdef _DEBUG
		_ASSERTE( m_dwThreadID == GetCurrentThreadId() );
#endif
		if ( !m_pMem && m_nSize <= MA_BLOCK_SIZE )
		{
			return ( PVOID )m_FixedMem;
		}

		return m_pMem;
	}

	UINT GetSize()
	{
		return m_nSize;
	}

protected:
	UINT GetBlockSize( UINT nMaxSize )
	{
		_ASSERTE( nMaxSize < MA_BLOCK_SIZE * 1000 );
		UINT TempSize = m_nSize;
		while ( TempSize < nMaxSize )
		{
			TempSize *= 2;
		}

		return TempSize;
	}

private:
	UINT	m_nSize;
	BYTE*	m_pMem;
	BYTE	m_FixedMem[MA_BLOCK_SIZE];

#ifdef _DEBUG
	DWORD m_dwThreadID;
#endif

};
class CConversion
{
public:
	CConversion( void ) {};
	~CConversion( void ) {};

	LPWSTR UTF8_TO_UTF16( LPCSTR szUTF8 )
	{
		int wcharsize = MultiByteToWideChar( CP_UTF8, 0,  szUTF8, -1, NULL, 0 );
		int size = wcharsize * sizeof( WCHAR );
		ATLVERIFY( MultiByteToWideChar( CP_UTF8, 0,  szUTF8, -1, ( LPWSTR )_Mem.GetMemory( size ), wcharsize ) );

		return ( LPWSTR )( PVOID )_Mem;
	}

	LPSTR  UTF8_TO_MBCS( LPCSTR szUTF8 )
	{
#ifdef _DEBUG
		OutputDebugString( _T( "UTF8_TO_MBCS() start" ) );
#endif
		int wcharsize = MultiByteToWideChar( CP_UTF8, 0,  szUTF8, -1, NULL, 0 );
		int size = wcharsize * sizeof( WCHAR );
		ATLVERIFY( MultiByteToWideChar( CP_UTF8, 0,  szUTF8, -1, ( LPWSTR )_Mem.GetMemory( size ), wcharsize ) );

		int charsize = WideCharToMultiByte( CP_NAVERTOOLBAR, 0, ( LPWSTR )( PVOID )_Mem, -1, NULL, 0, NULL, NULL );
		size = charsize * sizeof( char );
		ATLVERIFY( WideCharToMultiByte( CP_NAVERTOOLBAR, 0, ( LPWSTR )( PVOID )_Mem, -1, ( LPSTR )_Mem2.GetMemory( size ), size, 0, 0 ) ); // NS

		return ( LPSTR )( PVOID )_Mem2;
	}

	LPWSTR MBCS_TO_UTF16( LPCSTR szMBCS )
	{
		int wcharsize = MultiByteToWideChar( CP_NAVERTOOLBAR, 0,  szMBCS, -1, NULL, 0 );
		int size = wcharsize * sizeof( WCHAR );
		ATLVERIFY( MultiByteToWideChar( CP_NAVERTOOLBAR, 0,  szMBCS, -1, ( LPWSTR )_Mem.GetMemory( size ), wcharsize ) );

		return ( LPWSTR )( PVOID )_Mem;
	}

	LPSTR  MBCS_TO_UTF8( LPCSTR szMBCS )
	{
		int wcharsize = MultiByteToWideChar( CP_NAVERTOOLBAR, 0,  szMBCS, -1, NULL, 0 );
		int size = wcharsize * sizeof( WCHAR );
		ATLVERIFY( MultiByteToWideChar( CP_NAVERTOOLBAR, 0,  szMBCS, -1, ( LPWSTR )_Mem.GetMemory( size ), wcharsize ) );

		int charsize = WideCharToMultiByte( CP_UTF8, 0, ( LPWSTR )( PVOID )_Mem, -1, NULL, 0, NULL, NULL );
		size = charsize * sizeof( char );
		ATLVERIFY( WideCharToMultiByte( CP_UTF8, 0, ( LPWSTR )( PVOID )_Mem, -1, ( LPSTR )_Mem2.GetMemory( size ), size, 0, 0 ) ); // NS

		return ( LPSTR )( PVOID )_Mem2;
	}

	LPSTR  UTF16_TO_MBCS( LPCWSTR szUTF16 )
	{
		int charsize = WideCharToMultiByte( CP_NAVERTOOLBAR, 0, szUTF16, -1, NULL, 0, NULL, NULL );
		int size = charsize * sizeof( char );
		ATLVERIFY( WideCharToMultiByte( CP_NAVERTOOLBAR, 0, szUTF16, -1, ( LPSTR )_Mem.GetMemory( size ), size, 0, 0 ) );

		return ( LPSTR )( PVOID )_Mem;
	}

	LPSTR  WCS_TO_MBCS( const UINT CodePage, LPCWSTR szWcs )
	{
		int charsize = WideCharToMultiByte( CodePage, 0, szWcs, -1, NULL, 0, NULL, NULL );
		int size = charsize * sizeof( char );
		ATLVERIFY( WideCharToMultiByte( CodePage, 0, szWcs, -1, ( LPSTR )_Mem.GetMemory( size ), size, 0, 0 ) );

		return ( LPSTR )( PVOID )_Mem;
	}

	LPWSTR  MBCS_TO_WCS( const UINT CodePage, LPCSTR szMBCS )
	{
		int wcharsize = MultiByteToWideChar( CodePage, 0,  szMBCS, -1, NULL, 0 );
		int size = wcharsize * sizeof( WCHAR );
		MultiByteToWideChar( CodePage, 0,  szMBCS, -1, ( LPWSTR )_Mem.GetMemory( size ), size );

		return ( LPWSTR )( PVOID )_Mem;
	}

	LPSTR  UTF16_TO_UTF8( LPCWSTR szUTF16 )
	{
		int charsize = WideCharToMultiByte( CP_UTF8, 0, szUTF16, -1, NULL, 0, NULL, NULL );
		int size = charsize * sizeof( char );
		ATLVERIFY( WideCharToMultiByte( CP_UTF8, 0, szUTF16, -1, ( LPSTR )_Mem.GetMemory( size ), size, 0, 0 ) );

		return ( LPSTR )( PVOID )_Mem;
	}

#ifdef _UNICODE
#define UTF8_TO_TCHAR	UTF8_TO_UTF16
#define TCHAR_TO_UTF8	UTF16_TO_UTF8
	inline LPCTSTR UTF16_TO_TCHAR( LPCTSTR lp )
	{
		return lp;
	}
	inline LPCTSTR TCHAR_TO_UTF16( LPCTSTR lp )
	{
		return lp;
	}

#define MBCS_TO_TCHAR	MBCS_TO_UTF16
#define TCHAR_TO_MBCS	UTF16_TO_MBCS

#else
#define UTF8_TO_TCHAR	UTF8_TO_MBCS
#define UTF16_TO_TCHAR	UTF16_TO_MBCS
#define TCHAR_TO_UTF8	MBCS_TO_UTF8
#define TCHAR_TO_UTF16	MBCS_TO_UTF16

	inline LPCTSTR MBCS_TO_TCHAR( LPCTSTR lp )
	{
		return lp;
	}
	inline LPCTSTR TCHAR_TO_MBCS( LPCTSTR lp )
	{
		return lp;
	}
#endif

private:
	CMemAllocator _Mem;
	CMemAllocator _Mem2;
};
