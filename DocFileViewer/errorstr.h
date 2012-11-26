#pragma once 

inline LPCTSTR GetErrorString(HRESULT hr, LPTSTR lpszError, UINT cchMax) 
{ 
    DWORD dwErr = (DWORD)hr; 
    DWORD dwReturn; 
    LPTSTR lpMsgBuf = NULL; 

    if (IsBadStringPtr(lpszError, cchMax)) 
        return NULL; 

    if (0 == dwErr) { 
        dwErr = GetLastError(); 
    } 

    // lookup error code and get it 
    dwReturn = FormatMessage( 
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM | 
        FORMAT_MESSAGE_IGNORE_INSERTS, 
        NULL, 
        dwErr, 
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language 
        (LPTSTR) &lpMsgBuf, 
        0, 
        NULL 
        ); 

    memset(lpszError, 0, cchMax*sizeof(TCHAR)); 

    if (dwReturn == 0) 
        return NULL; 

    lstrcpyn(lpszError, lpMsgBuf, cchMax); 

    LocalFree( lpMsgBuf ); 

    return lpszError; 
} 


inline BOOL WriteToLogW(LPCWSTR lpszFmt, ...) 
{ 
#ifdef  _DEBUG 
    WCHAR buffer[4096] = { 0 }; 
    va_list args; 

    va_start(args, lpszFmt); 
    wvsprintfW(buffer, lpszFmt, args); 
    va_end(args); 

    OutputDebugStringW( buffer ); 
#endif  // _DEBUG 

    return TRUE; 
} 

inline BOOL WriteToLogA(LPCSTR lpszFmt, ...) 
{ 
#ifdef  _DEBUG 
    char buffer[4096] = { 0 }; 
    va_list args; 

    va_start(args, lpszFmt); 
    wvsprintfA(buffer, lpszFmt, args); 
    va_end(args); 

    TCHAR temp[4096] = { 0 }; 
    OutputDebugStringA( buffer ); 
#endif  // _DEBUG 
    return TRUE; 
} 

#ifdef  UNICODE 
#define WriteToLog WriteToLogW 
#else   // UNICODE 
#define WriteToLog WriteToLogA 
#endif  // UNICODE 
