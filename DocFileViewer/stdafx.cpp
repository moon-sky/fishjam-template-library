// stdafx.cpp : source file that includes just the standard includes
//	DocFileViewer.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#if (_ATL_VER < 0x0700)
#include <atlimpl.cpp>
#endif //(_ATL_VER < 0x0700)

#pragma comment(lib, "shlwapi.lib") 

#include <locale> 


LPCTSTR AdjustFileName(LPTSTR lpszName) 
{ 
    TCHAR * pIter = lpszName; 

    while (*pIter) 
    { 
        if (false == _istprint(*pIter) ) 
            *pIter = 'A'; 
        pIter ++; 
    } 
    return lpszName; 
} 


HRESULT WriteStreamToFile(LPCTSTR lpszFileName, IStream *pStrm) 
{ 
    HRESULT hr = E_FAIL; 

    if (NULL == pStrm) { 
        return hr; 
    } 

    HANDLE hFile = ::CreateFile(lpszFileName, GENERIC_WRITE, 0, 
        NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); 
    if (INVALID_HANDLE_VALUE == hFile) { 
        WriteToLog(_T("Can't create file %s\n"), lpszFileName); 
        return hr; 
    } 

    STATSTG statstg = { 0 }; 
    hr = pStrm->Stat(&statstg, STATFLAG_NONAME); 

    ULONGLONG ullIter = 0; 

    LARGE_INTEGER liBegin = { 0 }; 
    hr = pStrm->Seek(liBegin, STREAM_SEEK_SET, NULL); 
    if (FAILED(hr)) return hr; 
    
#undef STRM_BUFF_SIZE 
#define STRM_BUFF_SIZE  1024*64 
    BYTE * pBuffer = new BYTE[STRM_BUFF_SIZE]; 
    ULONG nReadSize = 0; 
    DWORD nWritten = 0; 

    BOOL nRet = FALSE; 
    while( 1 ) 
    { 
        hr = pStrm->Read(pBuffer, sizeof(BYTE)*STRM_BUFF_SIZE, &nReadSize); 
        if (FAILED(hr) || 0==nReadSize) { 
            break; 
        } 

        nRet = ::WriteFile(hFile, pBuffer, nReadSize, &nWritten, NULL); 
        if(FAILED(hr) || nWritten!=nReadSize) { 
            break; 
        } 

        ullIter+=nReadSize; 
    } 

    ::CloseHandle(hFile); 

    delete [] pBuffer; 
    return hr; 
} 
