///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   ftlbase.hpp
/// @brief  Functional Template Library Base Implemention File.
/// @author fujie
/// @version 0.6 
/// @date 03/30/2008
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef FTL_BASE_HPP
#define FTL_BASE_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlbase.h"
#endif

#include "shlwapi.h"
#pragma comment( lib, "shlwapi.lib" )

namespace FTL
{
    template<typename T>
    BOOL IsSameNumber(const T& expected, const T& actual, const T& delta)
    {
        if (FTL_ABS(expected - actual) <= delta)
        {
            return TRUE;
        }
        return FALSE;
    }

    template<typename TBase, typename INFO_TYPE, LONG bufLen>
    CFConvertInfoT<TBase,INFO_TYPE,bufLen>::CFConvertInfoT(INFO_TYPE info)
        :m_Info(info)
    {
        ZeroMemory(m_bufInfo,sizeof(m_bufInfo));
    }
    template<typename TBase, typename INFO_TYPE, LONG bufLen>
    CFConvertInfoT<TBase,INFO_TYPE,bufLen>::~CFConvertInfoT()
    {

    }
    template<typename TBase, typename INFO_TYPE, LONG bufLen>
    LPCTSTR CFConvertInfoT<TBase,INFO_TYPE,bufLen>::GetConvertedInfo()
    {
        TBase* pThis = static_cast<TBase*>(this);
        return pThis->ConvertInfo();
    }

    template<typename TBase, typename INFO_TYPE, LONG bufLen>
    const INFO_TYPE CFConvertInfoT<TBase,INFO_TYPE,bufLen>::GetInfo() const
    {
        return m_Info;
    }

    template<typename TBase, typename INFO_TYPE, LONG bufLen>
    void CFConvertInfoT<TBase,INFO_TYPE,bufLen>::SetInfo(INFO_TYPE info)
    {
        m_Info = info;
        m_bufInfo[0] = NULL;
    }

    CFAPIErrorInfo::CFAPIErrorInfo(DWORD dwError) : CFConvertInfoT<CFAPIErrorInfo,DWORD>(dwError)
    {
    }

    LPCTSTR CFAPIErrorInfo::ConvertInfo()
    {
        if (NULL == m_bufInfo[0])
        {
            switch (m_Info)
            {
#ifndef STATUS_DLL_NOT_FOUND
#  define STATUS_DLL_NOT_FOUND      ((DWORD )0xC0000135L)
#endif 
				//Side By Side Error
			    HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),ERROR_SXS_CANT_GEN_ACTCTX);
				
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),STATUS_DLL_NOT_FOUND);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),EXCEPTION_ACCESS_VIOLATION);
            default:
                break;
            }
            if (NULL == m_bufInfo[0])
            {
                LPTSTR pszMsgBuf = NULL;
                DWORD dwCount = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER 
                    | FORMAT_MESSAGE_FROM_SYSTEM 
                    | FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL,
                    m_Info,
                    MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
                    (LPTSTR)&pszMsgBuf,
                    0,
                    NULL);

				static LPCTSTR pszErrorModule[] = {
					TEXT("netmsg.dll"),
					TEXT("WinINet.dll"),
				};
                // Is it a network-related error?
                if (0 == dwCount)
                {
                    HMODULE hDll = LoadLibraryEx(TEXT("netmsg.dll"), NULL, 
                        DONT_RESOLVE_DLL_REFERENCES);
                    if (hDll != NULL) {
                        dwCount = FormatMessage(FORMAT_MESSAGE_FROM_HMODULE |FORMAT_MESSAGE_ALLOCATE_BUFFER
                            | FORMAT_MESSAGE_FROM_SYSTEM,
                            hDll, m_Info, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
                            (LPTSTR)&pszMsgBuf, 0, NULL);
                        FreeLibrary(hDll);
                    }
                }

				// Is it a WININET error?
				if (0 == dwCount)
				{
					TCHAR tmpBuf[1024] = {0};
					DWORD dwCount = 1024;
					HMODULE hDll = LoadLibraryEx(TEXT("WinINet.dll"), NULL, 
						DONT_RESOLVE_DLL_REFERENCES);
					if (hDll != NULL) {
						dwCount = FormatMessage(FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_ALLOCATE_BUFFER,
							//| FORMAT_MESSAGE_FROM_SYSTEM,
							hDll, m_Info, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
							tmpBuf, dwCount, NULL);
						FreeLibrary(hDll);
					}
				}


                if(0 != dwCount && NULL != pszMsgBuf )
                {
                    int length = (int)_tcsclen(pszMsgBuf);

                    lstrcpyn(m_bufInfo,pszMsgBuf,_countof(m_bufInfo) - 1);
                    LocalFree(pszMsgBuf);
                    if(length > 0  && length < _countof(m_bufInfo))
                    {
                        if(m_bufInfo[length - 2 ] == _T('\r') 
                            && m_bufInfo[length - 1] == _T('\n'))
                        {
                            m_bufInfo[length - 2]=_T('\0');
                        }
                    }
                }
                else //无法获取错误值
                {
                    StringCchPrintf(m_bufInfo,_countof(m_bufInfo),TEXT("Unknown Error:0x%0.8x"),m_Info);
                }
            }
        }
        return m_bufInfo;
    }

#ifndef FACILITY_D3DX
#  define FACILITY_D3D                     0x876
#  define FACILITY_D3DX                    0x877
#  define FACILITY_DSound                  0x878
#endif 

    CFComErrorInfo::CFComErrorInfo(HRESULT hr) : CFConvertInfoT<CFComErrorInfo,HRESULT>(hr)
    {
    }

    LPCTSTR CFComErrorInfo::GetErrorFacility(HRESULT hr, LPTSTR pszFacility,DWORD dwLength)
    {
        int nFacility = HRESULT_FACILITY(hr);
        switch (nFacility)
        {
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_NULL);         //0
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_RPC);          //1
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_DISPATCH);     //2
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_STORAGE);      //3
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_ITF);          //4
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_WIN32);        //7
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_WINDOWS);      //8
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_SECURITY);     //9
            //HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_SSPI);       //9
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_CONTROL);      //10
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_CERT);         //11
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_INTERNET);     //12
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_MEDIASERVER);  //13
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_MSMQ);         //14
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_SETUPAPI);     //15
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_SCARD);        //16
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_COMPLUS);      //17
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_AAF);          //18
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_URT);          //19
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_ACS);          //20
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_DPLAY);        //21
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_UMI);          //22
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_SXS);          //23
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_WINDOWS_CE);   //24
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_HTTP);         //25
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_BACKGROUNDCOPY);//32
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_CONFIGURATION);//33
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_STATE_MANAGEMENT);//34
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_METADIRECTORY);//35

            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_D3D);           //0x876
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_D3DX);          //0x877
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_DSound);        //0x878
        default:
            StringCchPrintf(pszFacility,dwLength,TEXT("Unknown[%d]"),nFacility);
            break;
        }
        return pszFacility;
    }

    LPCTSTR CFComErrorInfo::ConvertInfo()
    {
        if (NULL == m_bufInfo[0])
        {
            switch(m_Info)
            {
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), S_FALSE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), E_FAIL);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), E_INVALIDARG);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), E_NOINTERFACE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), E_NOTIMPL);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), DISP_E_MEMBERNOTFOUND);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), E_UNEXPECTED);
                //HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), CONNECT_E_NOCONNECTION);
            default:
                TCHAR strFacility[20] = {0};
                GetErrorFacility(m_Info,strFacility,_countof(strFacility));

                CFAPIErrorInfo apiErrorInfo(m_Info);
                StringCchPrintf(m_bufInfo,_countof(m_bufInfo),
                    TEXT("%s, Facility:%s"),apiErrorInfo.GetConvertedInfo(),strFacility);
                break;
            }
        }
        return m_bufInfo;
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T, MemoryAllocType allocType  /*= matNew*/, UINT DefaultFixedCount/* = DEFAULT_MEMALLOCATOR_FIXED_COUNT*/>
    CFMemAllocator<T, allocType, DefaultFixedCount>::CFMemAllocator()
        :m_pMem(NULL)
        ,m_allocType(matNew)
        ,m_nCount(DefaultFixedCount)
    {
        Init(DefaultFixedCount);
    }

    template <typename T, MemoryAllocType allocType  /*= matNew*/, UINT DefaultFixedCount/* = DEFAULT_MEMALLOCATOR_FIXED_COUNT*/>
    CFMemAllocator<T, allocType, DefaultFixedCount>::CFMemAllocator(DWORD nCount)
        :m_pMem(NULL)
        ,m_allocType(allocType)
        ,m_nCount(nCount)
    {
        FTLASSERT(nCount > 0);
        Init(nCount);
    }

    template <typename T, MemoryAllocType allocType  /*= matNew*/, UINT DefaultFixedCount/* = DEFAULT_MEMALLOCATOR_FIXED_COUNT*/>
    VOID CFMemAllocator<T, allocType, DefaultFixedCount>::Init(DWORD nCount)
    {
        if (nCount > DefaultFixedCount)
        {
            switch(allocType)
            {
            case matNew:
                m_pMem = new T[nCount];
                m_nCount = nCount;
                //ZeroMemory(m_pMem,sizeof(T) * nCount); //先清除内存，保证没有垃圾数据--是否会造成类指针问题？性能影响？
                break;
            default:
                FTLASSERT(FALSE);
                break;
            }
        }
    }

    template <typename T, MemoryAllocType allocType  /*= matNew*/, UINT DefaultFixedCount/* = DEFAULT_MEMALLOCATOR_FIXED_COUNT*/>
    VOID CFMemAllocator<T, allocType, DefaultFixedCount>::_FreeMemory()
    {
        if (m_pMem)
        {
            switch (allocType)
            {
            case matNew:
                SAFE_DELETE_ARRAY(m_pMem);
                break;
            default:
                FTLASSERT(FALSE);
                break;
            }
        }
    }

    template <typename T, MemoryAllocType allocType  /*= matNew*/, UINT DefaultFixedCount/* = DEFAULT_MEMALLOCATOR_FIXED_COUNT*/>
    CFMemAllocator<T, allocType, DefaultFixedCount>::~CFMemAllocator()
    {
        _FreeMemory();
    }

    template <typename T, MemoryAllocType allocType  /*= matNew*/, UINT DefaultFixedCount/* = DEFAULT_MEMALLOCATOR_FIXED_COUNT*/>
    T* CFMemAllocator<T, allocType, DefaultFixedCount>::Detatch()
    {
        T* pTmpMem = m_pMem;
        m_pMem = NULL;
        m_nCount = DefaultFixedCount;
        return pTmpMem;
    }

    template <typename T, MemoryAllocType allocType  /*= matNew*/, UINT DefaultFixedCount/* = DEFAULT_MEMALLOCATOR_FIXED_COUNT*/>
    T* CFMemAllocator<T, allocType, DefaultFixedCount>::GetMemory( UINT nMaxSize )
    {
        if ( !m_pMem && nMaxSize <= DefaultFixedCount )
        {
            return m_FixedMem;
        }

        if ( m_nCount < nMaxSize || NULL == m_pMem )
        {
            _FreeMemory();
            m_nCount = _GetBlockSize( nMaxSize );
            Init(m_nCount);
        }
		return m_pMem;
    }

    template <typename T, MemoryAllocType allocType  /*= matNew*/, UINT DefaultFixedCount/* = DEFAULT_MEMALLOCATOR_FIXED_COUNT*/>
    UINT CFMemAllocator<T,allocType, DefaultFixedCount>::GetCount() const
    {
        return m_nCount;
    }

    template <typename T, MemoryAllocType allocType  /*= matNew*/, UINT DefaultFixedCount/* = DEFAULT_MEMALLOCATOR_FIXED_COUNT*/>
    UINT CFMemAllocator<T,allocType, DefaultFixedCount>::_GetBlockSize(UINT nMaxCount)
    {
        UINT TempSize = m_nCount;
        while ( TempSize < nMaxCount )
        {
            TempSize *= 2;
        }
        return TempSize;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CFStringFormater::CFStringFormater(DWORD dwInitAllocLength/* = MAX_BUFFER_LENGTH*/)
        : m_dwInitAllocLength(dwInitAllocLength)
    {
        m_pBuf = NULL;
        m_dwTotalSpaceSize = 0;
    }
    CFStringFormater::~CFStringFormater()
    {
        SAFE_DELETE_ARRAY(m_pBuf);
    }

    VOID CFStringFormater::Reset()
    {
        SAFE_DELETE_ARRAY(m_pBuf);
        m_dwTotalSpaceSize = 0;
    }
    HRESULT CFStringFormater::Format(LPCTSTR lpszFormat, ...)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(lpszFormat,E_INVALIDARG);
        HRESULT hr = E_FAIL;

        va_list pArgs;
        va_start(pArgs, lpszFormat);
        COM_VERIFY(FormatV(lpszFormat, pArgs));
        va_end(pArgs);

        return hr;
    }

    HRESULT CFStringFormater::FormatV(LPCTSTR lpszFormat, va_list argList)
    {
        HRESULT hr = E_FAIL;
        m_dwTotalSpaceSize = 0;
        SAFE_DELETE_ARRAY(m_pBuf);  //先删除以前的

        DWORD dwLength = m_dwInitAllocLength;
        LPTSTR pszDestEnd = NULL;
        size_t cchRemaining = 0;

        DWORD dwFlags = 0;
        //STRSAFE_FILL_ON_FAILURE | STRSAFE_FILL_BEHIND_NULL; //失败时全部填充NULL，成功时在最后填充NULL

        m_pBuf = new TCHAR[dwLength];
        if (!m_pBuf)
        {
            return HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY);
        }
        ZeroMemory(m_pBuf,sizeof(TCHAR) * dwLength);

        COM_VERIFY_EXCEPT1(StringCchVPrintfEx(m_pBuf,dwLength, &pszDestEnd,&cchRemaining,dwFlags,lpszFormat,argList)
            ,STRSAFE_E_INSUFFICIENT_BUFFER);

        //如果内存空间不够，每次扩大2倍内存长度，重新尝试，直到成功或内存分配失败
        while (hr == STRSAFE_E_INSUFFICIENT_BUFFER && dwLength < 8 * m_dwInitAllocLength)
        {
            SAFE_DELETE_ARRAY(m_pBuf);
            dwLength <<= 1; //dwLength *=2;
            m_pBuf = new TCHAR[dwLength];
            if (NULL == m_pBuf)
            {
                break;
            }
            ZeroMemory(m_pBuf,sizeof(TCHAR) * dwLength);
            pszDestEnd = NULL;
            COM_VERIFY_EXCEPT1(StringCchVPrintfEx(m_pBuf,dwLength, &pszDestEnd,&cchRemaining,
                dwFlags,lpszFormat,argList),STRSAFE_E_INSUFFICIENT_BUFFER);
        }
        if (NULL == m_pBuf)
        {
            //Not enough memory
            return HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY);
        }
        // 确保最后的结束符，但实际上已经由 StringCchVPrintfEx 完成
        //m_pBuf[dwLength - 1] = TEXT('\0');
        m_dwTotalSpaceSize = dwLength;
        return hr;
    }
    HRESULT CFStringFormater::AppendFormat(LPCTSTR lpszFormat, ...)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(lpszFormat,E_INVALIDARG);
        HRESULT hr = E_FAIL;

        va_list pArgs;
        va_start(pArgs, lpszFormat);
        COM_VERIFY(AppendFormatV(lpszFormat, pArgs));
        va_end(pArgs);
        return hr;
    }

    HRESULT CFStringFormater::AppendFormatV(LPCTSTR lpszFormat, va_list argList)
    {
        HRESULT hr = E_FAIL;
        if (NULL == m_pBuf)
        {
            COM_VERIFY(FormatV(lpszFormat,argList));
            return hr;
        }

        //备份原来的字符串
        DWORD dwOldStringLen = static_cast<DWORD>(_tcslen(m_pBuf));
        LPTSTR pszOldString = new TCHAR[dwOldStringLen + 1];
        COM_VERIFY(::StringCchCopy(pszOldString,dwOldStringLen + 1,m_pBuf));
        pszOldString[dwOldStringLen] = NULL;

        LPTSTR pszDestEnd = NULL;
        size_t cchRemaining = 0;
        DWORD dwFlags = 0;
        //STRSAFE_FILL_ON_FAILURE | STRSAFE_FILL_BEHIND_NULL; //失败时全部填充NULL，成功时在最后填充NULL

        //指向原来字符串的结束位置
        LPTSTR pszAppendPos = m_pBuf + dwOldStringLen;
        DWORD dwLength = m_dwTotalSpaceSize;
        COM_VERIFY_EXCEPT1(StringCchVPrintfEx(pszAppendPos,dwLength - dwOldStringLen, &pszDestEnd,&cchRemaining,dwFlags,lpszFormat,argList)
            ,STRSAFE_E_INSUFFICIENT_BUFFER);

        //如果内存空间不够，每次扩大2倍内存长度，重新尝试，直到成功或内存分配失败
        while (hr == STRSAFE_E_INSUFFICIENT_BUFFER)
        {
            SAFE_DELETE_ARRAY(m_pBuf);
            dwLength *= 2;
            m_pBuf = new TCHAR[dwLength];
            if (NULL == m_pBuf)
            {
                break;
            }
            ZeroMemory(m_pBuf,sizeof(TCHAR) * dwLength);
            COM_VERIFY(::StringCchCopy(m_pBuf,dwOldStringLen + 1,pszOldString));
            pszAppendPos = m_pBuf + dwOldStringLen;
            COM_VERIFY_EXCEPT1(StringCchVPrintfEx(pszAppendPos,dwLength - dwOldStringLen, &pszDestEnd,&cchRemaining,
                dwFlags,lpszFormat,argList),STRSAFE_E_INSUFFICIENT_BUFFER);
        }
        if (NULL == m_pBuf)
        {
            //Not enough memory
            SAFE_DELETE_ARRAY(pszOldString);
            return HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY);
        }

        m_dwTotalSpaceSize = dwLength;
        SAFE_DELETE_ARRAY(pszOldString);
        return hr;
    }

    LPCTSTR CFStringFormater::GetString() const
    {
        return m_pBuf;
    }
    LONG CFStringFormater::GetStringLength() const
    {
        if (m_pBuf)
        {
            return (lstrlen(m_pBuf));
        }
        return 0;
    }
	
    LPTSTR CFStringFormater::Detach()
    {
        LPTSTR pBuf = m_pBuf;
        m_pBuf = NULL;
        m_dwTotalSpaceSize = 0;
        return pBuf;
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CFFastTrace::CFTFileWriter::CFTFileWriter()
    {
        m_hFile = INVALID_HANDLE_VALUE ;
        m_lWrites = 0 ;
        m_dwTID = 0 ;
    }
    CFFastTrace::CFTFileWriter::~CFTFileWriter()
    {
        if (INVALID_HANDLE_VALUE != m_hFile)
        {
            Close();
        }
    }

    BOOL CFFastTrace::CFTFileWriter::Initialize( HANDLE hFile , DWORD dwTID )
    {
        FTLASSERT ( INVALID_HANDLE_VALUE != hFile ) ;
        if ( INVALID_HANDLE_VALUE == hFile )
        {
            return ( FALSE ) ;
        }
        BOOL bRet = FALSE;
        m_hFile = hFile ;
        m_dwTID = dwTID ;
        m_lWrites = 0 ;// Ensure the count starts at zero.
        API_VERIFY( WriteHeader() );
        if ( FALSE == bRet )
        {
            // Not much else I can do.
            API_VERIFY ( CloseHandle ( m_hFile ) ) ;
            m_hFile = INVALID_HANDLE_VALUE ;
        }
        return ( bRet ) ;
    }
    BOOL CFFastTrace::CFTFileWriter::WriteData( LPFTDATA pData )
    {
        FTLASSERT ( INVALID_HANDLE_VALUE != m_hFile ) ;
        if ( INVALID_HANDLE_VALUE == m_hFile )
        {
            return ( FALSE ) ;
        }
        FTLASSERT(pData->nTraceInfoLen == (LONG)(_tcslen(pData->pszTraceInfo) + 1));
        BOOL bRet = FALSE;
        DWORD dwWritten = 0 ;
        API_VERIFY(::WriteFile(m_hFile,pData,
            sizeof(FTDATA) - sizeof(LPCTSTR),  //减去最后的 pszTraceInfo指针
            &dwWritten,NULL));
        FTLASSERT(sizeof(FTDATA) - sizeof(LPCTSTR) == dwWritten);
        if (bRet)
        {
            //写字符串信息
            API_VERIFY(::WriteFile(m_hFile,pData->pszTraceInfo,sizeof(TCHAR) * pData->nTraceInfoLen,&dwWritten,NULL));
        }
        if(bRet)
        {
            // It's a good write.
            m_lWrites++ ;
        }
        else
        {
            // Let's attempt to write the header so it gets updated.
            WriteHeader ( ) ;
            bRet = FALSE ;
            // Not much else I can do.
            API_VERIFY ( CloseHandle ( m_hFile ) ) ;
            m_hFile = INVALID_HANDLE_VALUE ;
        }
        return ( bRet ) ;
    }
    BOOL CFFastTrace::CFTFileWriter::Flush()
    {
        BOOL bRet = FALSE;
        return bRet;
    }
    BOOL CFFastTrace::CFTFileWriter::SnapFile()
    {
        BOOL bRet = FALSE;
        return bRet;
    }
    BOOL CFFastTrace::CFTFileWriter::WriteHeader()
    {
        BOOL bRet = FALSE ;
        FTLASSERT ( INVALID_HANDLE_VALUE != m_hFile ) ;
        if ( INVALID_HANDLE_VALUE != m_hFile )
        {
            // Scoot back to the beginning of the file and update the header.
            DWORD dwMove = SetFilePointer ( m_hFile,0,NULL,FILE_BEGIN) ;
            FTLASSERT ( INVALID_SET_FILE_POINTER != dwMove ) ;
            if ( INVALID_SET_FILE_POINTER != dwMove )
            {
                FTFILEHEADER stFH = {0} ;
                stFH.dwSig = FTFILESIG ;
                stFH.bIsUnicode = (sizeof(TCHAR) == 2);
                stFH.lItemCount= m_lWrites ;
                stFH.lPID = (LONG)GetCurrentProcessId ( ) ;
                stFH.lTID = (LONG)m_dwTID ;
                DWORD dwWritten = 0 ;
                DWORD dwWrtieSize = sizeof(FTFILEHEADER);
                API_VERIFY(::WriteFile(m_hFile,&stFH,dwWrtieSize,&dwWritten,NULL)) ;
                FTLASSERT ( dwWrtieSize == dwWritten ) ;
                if ( ( FALSE == bRet) || ( dwWrtieSize != dwWritten )   )
                {
                    bRet = FALSE ;
                }
            }
            else
            {
                bRet = FALSE ;
            }
        }
        return ( bRet ) ;
    }
    BOOL CFFastTrace::CFTFileWriter::Close()
    {
        BOOL bRet = FALSE;
        FTLASSERT ( INVALID_HANDLE_VALUE != m_hFile ) ;
        if ( INVALID_HANDLE_VALUE != m_hFile )
        {
            // Scoot back to the beginning of the file and update the
            // header.
            API_VERIFY(WriteHeader()) ;
            // I'm done with the file.
            API_VERIFY(::CloseHandle( m_hFile ) ) ;
            m_hFile = INVALID_HANDLE_VALUE ;
            m_lWrites = 0 ;
        }
        return ( bRet ) ;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CFFastTrace& CFFastTrace::GetInstance()
    {
        static CFFastTrace fastTrace(TEXT("Software\\FTL"),NULL);
        return fastTrace;
    }

    CFFastTrace::CFFastTrace(LPCTSTR pszKey, HMODULE hModule)
    {
        BOOL bRet = FALSE;
        InitializeCriticalSection(&m_CsLock);
        m_SequenceNumber = 0;
        m_dwTLSSlot = TlsAlloc();
        API_VERIFY(TLS_OUT_OF_INDEXES != m_dwTLSSlot);
        if (TLS_OUT_OF_INDEXES != m_dwTLSSlot)
        {
            API_VERIFY(TlsSetValue(m_dwTLSSlot,NULL));
        }
        
        TCHAR szModuleName[MAX_PATH] = {0};
        if(GetModuleFileName(hModule,szModuleName,_countof(szModuleName)) > 0)
        {
            LPTSTR pszExtPos = StrRChr(szModuleName,NULL,TEXT('\\'));
            if (pszExtPos)
            {
                pszExtPos++;
                StringCchCopyN(szModuleName,_countof(szModuleName),pszExtPos,
                    _countof(szModuleName) - (pszExtPos - &szModuleName[0]));
            }
        }

        HKEY hKey = NULL;
        TCHAR szModuleKey[MAX_PATH] = {0};
        StringCchPrintf(szModuleKey,_countof(szModuleKey),TEXT("%s\\%s"),pszKey,szModuleName);
        LONG lRet = RegCreateKeyEx(HKEY_CURRENT_USER,szModuleKey,0,NULL,0,KEY_ALL_ACCESS,NULL,&hKey,NULL);
        if (ERROR_SUCCESS == lRet)
        {
            DWORD dwValue = 0;
            DWORD dwBufLen = sizeof(dwValue);
            DWORD dwType;
            lRet = RegQueryValueEx(hKey,TEXT("WriteThrough"),NULL,&dwType,reinterpret_cast<LPBYTE>(&dwValue),&dwBufLen);
            if (ERROR_SUCCESS == lRet && REG_DWORD == dwType)
            {
                m_Options.bWriteThrough = (BOOL)dwValue;
            }
            else
            {
                ::RegSetValueEx(hKey,TEXT("WriteThrough"),0,REG_DWORD,reinterpret_cast<LPBYTE>(&m_Options.bWriteThrough), 
                    sizeof(m_Options.bWriteThrough));
            }

            lRet = RegQueryValueEx(hKey,TEXT("WriteToFile"),NULL,&dwType,reinterpret_cast<LPBYTE>(&dwValue),&dwBufLen);
            if (ERROR_SUCCESS == lRet && REG_DWORD == dwType)
            {
                m_Options.bWriteToFile = (BOOL)dwValue;
            }
            else
            {
                ::RegSetValueEx(hKey,TEXT("WriteToFile"),0,REG_DWORD,reinterpret_cast<LPBYTE>(&m_Options.bWriteToFile),
                    sizeof(m_Options.bWriteToFile));
            }

            lRet = RegQueryValueEx(hKey,TEXT("DoTimings"),NULL,&dwType,reinterpret_cast<LPBYTE>(&dwValue),&dwBufLen);
            if (ERROR_SUCCESS == lRet && REG_DWORD == dwType)
            {
                m_Options.bDoTimings = (BOOL)dwValue;
            }
            else
            {
                ::RegSetValueEx(hKey,TEXT("DoTimings"),0,REG_DWORD,reinterpret_cast<LPBYTE>(&m_Options.bDoTimings),
                    sizeof(m_Options.bDoTimings));
            }


            lRet = RegQueryValueEx(hKey,TEXT("traceThreshold"),NULL,&dwType,reinterpret_cast<LPBYTE>(&dwValue),&dwBufLen);
            if (ERROR_SUCCESS == lRet && REG_DWORD == dwType)
            {
                m_Options.traceThreshold = TraceLevel(dwValue);
            }
            else
            {
                ::RegSetValueEx(hKey,TEXT("traceThreshold"),0,REG_DWORD,reinterpret_cast<LPBYTE>(&m_Options.traceThreshold),
                    sizeof(m_Options.traceThreshold));
            }
            ::RegCloseKey(hKey);
        }
    }

    CFFastTrace::~CFFastTrace()
    {
        CloseAllFileWriters();
        if (TLS_OUT_OF_INDEXES != m_dwTLSSlot)
        {
            TlsFree(m_dwTLSSlot);
            m_dwTLSSlot = TLS_OUT_OF_INDEXES;
        }
        DeleteCriticalSection ( &m_CsLock ) ;
    }

    BOOL CFFastTrace::CloseAllFileWriters()
    {
        BOOL bRet = TRUE;
        EnterCriticalSection(&m_CsLock);
        CFFastTrace::CFTFileWriter* pFileWriter = NULL;
        for (AllFileWriterArrayIterator iter = m_AllFileWriters.begin(); iter != m_AllFileWriters.end();
            ++iter)
        {
            pFileWriter = (*iter);
            FTLASSERT(pFileWriter);
            API_VERIFY(pFileWriter->Close());
            delete pFileWriter;
        }
        m_AllFileWriters.clear();
        LeaveCriticalSection(&m_CsLock);
        return bRet;
    }

    BOOL CFFastTrace::SetTraceOptions(LPFAST_TRACE_OPTIONS pOptions)
    {
        EnterCriticalSection(&m_CsLock);
        m_Options.bDoTimings = pOptions->bDoTimings;
        m_Options.bWriteThrough = pOptions->bWriteThrough;
        m_Options.bWriteToFile = pOptions->bWriteToFile;
        m_Options.traceThreshold = pOptions->traceThreshold;
        m_Options.pDebugOut = pOptions->pDebugOut;
        LeaveCriticalSection(&m_CsLock);
        return TRUE;
    }

    BOOL CFFastTrace::GetTraceOptions(LPFAST_TRACE_OPTIONS pOptions)
    {
        EnterCriticalSection(&m_CsLock);
        pOptions->bDoTimings = m_Options.bDoTimings ;
        pOptions->bWriteThrough = m_Options.bWriteThrough;
        pOptions->bWriteToFile = m_Options.bWriteToFile;
        pOptions->traceThreshold = m_Options.traceThreshold;
        pOptions->pDebugOut = m_Options.pDebugOut;
        LeaveCriticalSection(&m_CsLock);
        return TRUE;
    }


    BOOL CFFastTrace::CheckLevel(TraceLevel level)
    {
        BOOL bRet = (level >= m_Options.traceThreshold);
        return bRet;
    }

    void CFFastTrace::InternalWriteLogData(TraceLevel level, LPCTSTR pStrInfo)
    {
        BOOL bRet = FALSE;
        {
            //此处是否需要互斥保护?
            //EnterCriticalSection(&m_CsLock);
            if (m_Options.pDebugOut)
            {
                m_Options.pDebugOut(pStrInfo);
            }
            //LeaveCriticalSection(&m_CsLock);
        }

        if (m_Options.bWriteToFile)
        {       
            CFTFileWriter* pFileWriter = GetCurrentThreadFile();
            if ((CFTFileWriter*)INVALID_HANDLE_VALUE != pFileWriter)
            {
				FTDATA ftdata = {0} ;
                if ( TRUE == m_Options.bDoTimings )
                {
                    GetSystemTimeAsFileTime ( &ftdata.stTime ) ;// Get the current time stamp.
                }
                else
                {
                    ftdata.stTime.dwHighDateTime = 0 ;
                    ftdata.stTime.dwLowDateTime = 0 ;
                }
                ftdata.level = level;
                ftdata.nTraceInfoLen =  (LONG)(_tcslen(pStrInfo) + 1); //包括后面的结束符
                ftdata.pszTraceInfo = pStrInfo;
                ftdata.lSeqNum = InterlockedIncrement ( (LPLONG)&m_SequenceNumber) ;

                API_VERIFY(pFileWriter->WriteData(&ftdata));
                if (!bRet)
                {   //写信息失败，清除TLS
                    TlsSetValue ( m_dwTLSSlot,(LPVOID)INVALID_HANDLE_VALUE ) ;
                    pFileWriter->Close() ;
                    RemoveFileFromArray ( pFileWriter ) ;
                    // Free the memory.
                    delete pFileWriter ;
                }
            }
        }
    }
    void CFFastTrace::WriteLogInfo(const LPCTSTR lpszFormat,...)
    {
        CFStringFormater formater;
        va_list argList;
        va_start(argList, lpszFormat);
        formater.FormatV(lpszFormat,argList);
        va_end(argList);
        InternalWriteLogData(tlTrace, formater.GetString());
    }

    void CFFastTrace::WriteLogInfoEx(TraceLevel level,const LPCTSTR lpszFormat,...)
    {
        if (CheckLevel(level))
        {
            CFStringFormater formater;
            va_list argList;
            va_start(argList, lpszFormat);
            formater.FormatV(lpszFormat,argList);
            va_end(argList);
            InternalWriteLogData(level, formater.GetString());
        }
    }

    CFFastTrace::CFTFileWriter* CFFastTrace::GetCurrentThreadFile()
    {
        FTLASSERT(TLS_OUT_OF_INDEXES != m_dwTLSSlot);
        if (TLS_OUT_OF_INDEXES == m_dwTLSSlot)
        {
            //此时 CFFastTrace 已经析构，但还有线程在写日志 -- 全局或静态变量，比 CFFastTrace 的生存期长(如 CModulesHolder)
            return (CFFastTrace::CFTFileWriter*)(DWORD_PTR)(TLS_OUT_OF_INDEXES);
        }

        CFFastTrace::CFTFileWriter* pFileWriter = (CFFastTrace::CFTFileWriter*) TlsGetValue(m_dwTLSSlot);
        
        //返回NULL表示第一次访问,但如果返回 INVALID_HANDLE_VALUE, 则说明文件出现错误,已经被关闭
        if (NULL == pFileWriter)  
        {
            BOOL bRet = FALSE ;
            DWORD dwTID = GetCurrentThreadId ( ) ;
            HANDLE hFile = CreateRawThreadFile ( dwTID ) ;
            if (INVALID_HANDLE_VALUE != hFile)
            {
                pFileWriter = new CFFastTrace::CFTFileWriter(); 
                API_VERIFY(pFileWriter->Initialize(hFile,dwTID));
                if (FALSE == bRet)
                {
                    pFileWriter->Close();
                    delete pFileWriter;
                    hFile = INVALID_HANDLE_VALUE ;
                    pFileWriter = (CFFastTrace::CFTFileWriter *)INVALID_HANDLE_VALUE ;
                }
            }

            //将pFileWriter设置到TLS中，这样下次可以直接获取
            API_VERIFY(TlsSetValue( m_dwTLSSlot ,(LPVOID)pFileWriter)) ;
            if (INVALID_HANDLE_VALUE != pFileWriter)
            {
                AddFileToArray(pFileWriter);
            }
        }
        return pFileWriter;
    }

    HANDLE CFFastTrace::CreateRawThreadFile( DWORD dwTID )
    {
        HANDLE hRet = INVALID_HANDLE_VALUE ;
        TCHAR szFileName [ MAX_PATH + 1 ] = {0};
        BOOL bRet = FALSE;
        API_VERIFY( BuildThreadFileName ( szFileName ,MAX_PATH ,dwTID ));
        if ( TRUE == bRet )
        {
            DWORD dwFlags = FILE_ATTRIBUTE_ARCHIVE | FILE_FLAG_SEQUENTIAL_SCAN ;
            if ( TRUE == m_Options.bWriteThrough )
            {
                dwFlags |= FILE_FLAG_WRITE_THROUGH ;
            }

            // I've got the filename so I can now create the file.
            hRet = CreateFile ( szFileName ,
                GENERIC_WRITE      ,
                FILE_SHARE_READ    ,
                NULL               ,
                CREATE_ALWAYS      ,
                dwFlags            ,
                NULL                ) ;
            FTLASSERT( INVALID_HANDLE_VALUE != hRet ) ;
        }
        return ( hRet ) ;
    }

    BOOL CFFastTrace::BuildThreadFileName ( LPTSTR szFileNameBuff ,size_t iBuffLen,DWORD dwTID)
    {
        CHECK_POINTER_WRITABLE_DATA_RETURN_VALUE_IF_FAIL(szFileNameBuff,iBuffLen,FALSE);

        BOOL bRet = FALSE ;
        // Get the process directory and name and peel off the .EXE extension.
        TCHAR szExeName[ MAX_PATH + 1 ] ;
        DWORD dwRet = GetModuleFileName ( NULL , szExeName , MAX_PATH ) ;
        API_VERIFY ( 0 != dwRet ) ;
        if ( 0 != dwRet )
        {
            TCHAR * pPeriod = _tcsrchr ( szExeName , _T ( '.' ) ) ;
            FTLASSERT ( NULL != pPeriod ) ;
            if ( NULL != pPeriod )
            {
                *pPeriod = _T ( '\0' ) ;

                DWORD dwPID = GetCurrentProcessId ( ) ;
                // Build the complete
                HRESULT hr = E_FAIL;
                //考虑到线程ID可能重复利用(频繁创建、删除时)，因此加入一个时间戳标记
                COM_VERIFY(StringCchPrintf ( szFileNameBuff, iBuffLen ,TEXT("%s_%04d_%04d_%04d.FTL") ,
                    szExeName ,dwPID ,dwTID,GetTickCount()));
                if ( SUCCEEDED ( hr ) )
                {
                    bRet = TRUE ;
                }
            }
        }
        return ( bRet ) ;
    }

    BOOL CFFastTrace::AddFileToArray(CFTFileWriter* pFileWriter)
    {
        BOOL bRet = FALSE;
        {
            EnterCriticalSection(&m_CsLock);
            AllFileWriterArrayPair pr;
            pr = m_AllFileWriters.insert(pFileWriter);
            bRet = pr.second;
            LeaveCriticalSection(&m_CsLock);
        }
        return bRet;
    }
    BOOL CFFastTrace::RemoveFileFromArray(CFTFileWriter* pFileWriter)
    {
        BOOL bRet = FALSE;
        {
            EnterCriticalSection(&m_CsLock);
            AllFileWriterArrayIterator iter = m_AllFileWriters.find(pFileWriter);
            if (iter != m_AllFileWriters.end())
            {
                bRet = TRUE;//找到
                m_AllFileWriters.erase(iter);
            }
            LeaveCriticalSection(&m_CsLock);
        }
        return bRet;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CFElapseCounter::CFElapseCounter()
    {
        BOOL bRet = FALSE;
        m_StopTime.QuadPart = 0;
        API_VERIFY(QueryPerformanceFrequency(&m_Frequency));
        API_VERIFY(QueryPerformanceCounter(&m_StartTime));
        if (0 == m_Frequency.QuadPart)
        {
            m_Frequency.QuadPart = 1; //防止被0除，什么样的系统上会出错？486或Win3.1？
        }
        m_PauseTime.QuadPart = m_StartTime.QuadPart;

        m_Status = rsRunning;
    }
    CFElapseCounter::~CFElapseCounter()
    {

    }
    RunningStatus CFElapseCounter::GetStatus() const
    {
        return m_Status;
    }
    BOOL CFElapseCounter::Start()
    {
        BOOL bRet = FALSE;
        API_VERIFY(QueryPerformanceCounter(&m_StartTime));
        m_PauseTime.QuadPart = m_StartTime.QuadPart;
        m_StopTime.QuadPart = 0;
        m_Status = rsRunning;
        return bRet;
    }
    BOOL CFElapseCounter::Pause()
    {
        FTLASSERT(rsRunning == m_Status||!TEXT("Must Start After Stop"));
        if (rsRunning != m_Status)
        {
            SetLastError(ERROR_BAD_COMMAND);
            return FALSE;
        }
        BOOL bRet = FALSE;
        API_VERIFY(QueryPerformanceCounter(&m_PauseTime));
        m_Status = rsPaused;
        return bRet;
    }
    BOOL CFElapseCounter::Resume()
    {
        FTLASSERT(rsPaused == m_Status);
        if (rsPaused != m_Status)
        {
            SetLastError(ERROR_BAD_COMMAND);
            return FALSE;
        }
        BOOL bRet = FALSE;
        LARGE_INTEGER   resumeTime;
        API_VERIFY(QueryPerformanceCounter(&resumeTime));
        m_StartTime.QuadPart += (resumeTime.QuadPart - m_PauseTime.QuadPart);
        m_PauseTime.QuadPart = m_StartTime.QuadPart;
        m_Status = rsRunning;
        return bRet;
    }
    BOOL CFElapseCounter::Stop()
    {
        BOOL bRet = TRUE;
        switch (m_Status)
        {
        case rsRunning://运行状态下，获取新的结束时间
            API_VERIFY(QueryPerformanceCounter(&m_StopTime));
            break;
        case rsPaused://暂停状态下，获取暂停的时间
            m_StopTime.QuadPart = m_PauseTime.QuadPart;
            break;
        case rsStopped://停止状态下，什么都不做
        default:
            break;
        }
        m_Status = rsStopped;
        return bRet;
    }
    LONGLONG CFElapseCounter::GetElapseTime() //返回单位是纳秒(NS)
    {
        //in some machine,will overflow,so change to double count, and convert back

        BOOL bRet = FALSE;
        LONGLONG elapseNs;
        switch (m_Status)
        {
        case rsRunning: //(m_PauseTime.QuadPart == m_StartTime.QuadPart) //运行状态，获取当前时间到开始的时间
            {
                LARGE_INTEGER   nowTime;
                API_VERIFY(QueryPerformanceCounter(&nowTime));
                elapseNs = (LONGLONG)((double)(nowTime.QuadPart - m_StartTime.QuadPart) * ( NANOSECOND_PER_SECOND ) /m_Frequency.QuadPart); 
            }
            break;
        case rsPaused: ////暂停状态，获取暂停结束时间到开始的时间
            elapseNs = (LONGLONG)((double)(m_PauseTime.QuadPart - m_StartTime.QuadPart) * ( NANOSECOND_PER_SECOND ) /m_Frequency.QuadPart);
            break;
        case rsStopped: //m_StopTime.QuadPart != 0  //已经停止，获取结束到开始的时间
            elapseNs = (LONGLONG)((double)(m_StopTime.QuadPart - m_StartTime.QuadPart) * ( NANOSECOND_PER_SECOND ) /m_Frequency.QuadPart); 
            break;
        default:
            FTLASSERT(FALSE);
            elapseNs = -1;
            break;
        }

        FTLASSERT(elapseNs > 0);
        return elapseNs;
    }

#ifdef FTL_DEBUG
    BOOL CFBlockElapse::Init()
    {
        BOOL bRet = TRUE;

        FTLASSERT(TLS_OUT_OF_INDEXES == s_dwTLSIndex);
        s_dwTLSIndex = TlsAlloc();
        API_VERIFY(TLS_OUT_OF_INDEXES != s_dwTLSIndex);
        if (TLS_OUT_OF_INDEXES != s_dwTLSIndex)
        {
            API_VERIFY(TlsSetValue(s_dwTLSIndex, NULL));
        }
        return bRet;
    }
    VOID CFBlockElapse::UnInit()
    {
        if (TLS_OUT_OF_INDEXES != s_dwTLSIndex)
        {
            BlockElapseInfo* pInfo = (BlockElapseInfo*)TlsGetValue(s_dwTLSIndex);
            FTLASSERT(NULL == pInfo);   //防止内存泄漏，必须释放掉
        }
        TlsFree(s_dwTLSIndex);
        s_dwTLSIndex = TLS_OUT_OF_INDEXES;
    }
    CFBlockElapse::CFBlockElapse(LPCTSTR pszFileName,DWORD line, 
        LPCTSTR pBlockName, LPVOID pReturnAddr, DWORD MinElapse/* = 0*/)
        :m_pszFileName(pszFileName)
        ,m_Line(line)
        ,m_pszBlkName(pBlockName)
        ,m_pReturnAdr(pReturnAddr)
        ,m_MinElapse(MinElapse)

    {
        FTLASSERT(TLS_OUT_OF_INDEXES != s_dwTLSIndex && TEXT("Must Call Init"));
        FTLASSERT(pBlockName);
        FTLASSERT(pReturnAddr);

        BlockElapseInfo* pInfo = (BlockElapseInfo*)TlsGetValue(s_dwTLSIndex);
        if (NULL == pInfo)
        {
            FTLTRACEEX(FTL::tlWarning, TEXT("%s New Thread[%d] Begin Block Elapse Trace\n"), 
                __FILE__LINE__, GetCurrentThreadId());
            pInfo = new BlockElapseInfo();
            ZeroMemory(pInfo, sizeof(BlockElapseInfo));
            TlsSetValue(s_dwTLSIndex, pInfo);
        }
        pInfo->indent++;
        LONG curLevel = FTL_MIN(pInfo->indent,MAX_TRACE_INDICATE_LEVEL); //InterlockedIncrement(&s_Indent);
        for (LONG n = 0; n < curLevel; n++)
        {
            pInfo->bufIndicate[n] = TEXT('>');
        }
        pInfo->bufIndicate[curLevel] = 0;
        FAST_TRACE_EX(tlDetail, TEXT("%s(%d) :\t TID(0x%04x) %s (Enter \t%d): %s\n"),
            m_pszFileName,m_Line,GetCurrentThreadId(),pInfo->bufIndicate,pInfo->indent ,
            m_pszBlkName);
        m_StartTime = GetTickCount();
    }
    CFBlockElapse::~CFBlockElapse()
    {
        DWORD dwElapseTime = GetTickCount() - m_StartTime;
        if (m_MinElapse != 0 && dwElapseTime >= m_MinElapse)
        {
            FAST_TRACE_EX(tlWarning, TEXT("%s(%d) :\t TID(0x%04x) \"%s\"(0x%p) Elaspse Too Long (Want-%dms:Real-%dms)\n"),
                m_pszFileName,m_Line,GetCurrentThreadId(), m_pszBlkName,m_pReturnAdr,m_MinElapse,dwElapseTime);
        }
        
        BlockElapseInfo* pInfo = (BlockElapseInfo*)TlsGetValue(s_dwTLSIndex);
        FTLASSERT(pInfo);
        if (pInfo)
        {
            LONG curLevel = FTL_MIN(pInfo->indent,MAX_TRACE_INDICATE_LEVEL);
            for (LONG n = 0; n < curLevel; n++)
            {
                pInfo->bufIndicate[n] = TEXT('<');
            }
            pInfo->bufIndicate[curLevel] = 0;
            FAST_TRACE_EX(tlDetail,TEXT("%s(%d) :\t TID(0x%04x) %s (Leave \t%d): %s\n"),
                m_pszFileName,m_Line,GetCurrentThreadId(),pInfo->bufIndicate,pInfo->indent,m_pszBlkName);
            pInfo->indent--;
            if (0 == pInfo->indent)
            {
                FTLTRACEEX(FTL::tlWarning, TEXT("%s Thread[%d] End Block Elapse Trace\n"), 
                    __FILE__LINE__, GetCurrentThreadId());
                delete pInfo;
                TlsSetValue(s_dwTLSIndex, NULL);
            }
        }
    }

    __declspec(selectany) DWORD   CFBlockElapse::s_dwTLSIndex = TLS_OUT_OF_INDEXES;
    //__declspec(selectany) LONG CFBlockElapse::s_Indent = 0;
    //__declspec(selectany) TCHAR CFBlockElapse::s_bufIndicate[MAX_TRACE_INDICATE_LEVEL + 1];

#endif //FTL_DEBUG

    CModulesHolder::CModulesHolder()
    {
    }

    CModulesHolder::~CModulesHolder()
    {
        ModeulesHolderIterator iter;
        for (iter = m_allModules.begin(); iter != m_allModules.end(); iter++)
        {
            //此部分的代码会在 FastTrace 析构后调用
//#ifdef FTL_DEBUG   
//            BOOL bRet = FALSE;
//            TCHAR bufPath[MAX_PATH] = {0};
//            API_VERIFY(GetModuleFileName(*iter,bufPath,_countof(bufPath)));
//            FTLTRACE(TEXT("CModulesHolder::~CModulesHolder FreeLibrary %s\n"),bufPath);
//#endif 
            ::FreeLibrary(*iter);
        }
        m_allModules.clear();
    }

    void CModulesHolder::Erase(HMODULE hModule)
    {
        ModeulesHolderIterator iter = m_allModules.find(hModule);
        if (iter != m_allModules.end())
        {
#ifdef FTL_DEBUG
            BOOL bRet = FALSE;
            TCHAR bufPath[MAX_PATH] = {0};
            API_VERIFY(GetModuleFileName(*iter,bufPath,_countof(bufPath)));
            FAST_TRACE_EX(tlTrace, TEXT("CModulesHolder Erase %s\n"),bufPath);
#endif 
            ::FreeLibrary(*iter);
            m_allModules.erase(iter);
        }
    }
    bool CModulesHolder::Find(HMODULE hModule)
    {
        ModeulesHolderIterator iter = m_allModules.find(hModule);
        bool bFinded = (iter != m_allModules.end());
        return bFinded;
    }
    bool CModulesHolder::Insert(HMODULE hModule)
    {
        ModeulesHolderPair pr;
        pr = m_allModules.insert(hModule);
        return pr.second;
    }


    int FormatMessageBox(HWND hWnd, LPCTSTR lpCaption, UINT uType, LPCTSTR lpszFormat, ...)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(lpszFormat,FALSE);
        int nRet = 0; //Zero indicates that there is not enough memory to create the message box.
        HRESULT hr = E_FAIL;
        CFStringFormater formater;

        va_list pArgs;
        va_start(pArgs, lpszFormat);

        COM_VERIFY(formater.FormatV(lpszFormat,pArgs));
        va_end(pArgs);

        if (SUCCEEDED(hr) && NULL != formater.GetString())
        {
            nRet = ::MessageBox(hWnd,formater,lpCaption,uType);
        }
        return nRet;
    }
}

#endif //FTL_BASE_HPP