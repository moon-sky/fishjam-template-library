#ifndef __STR_CONVERT_H__ 
#define __STR_CONVERT_H__ 

#pragma once 

// 
// convert wide char string to multi-byte char string 
// 
inline char * WChar2Char(const wchar_t * src, char * dest, int nBuflen, UINT cp=CP_ACP) 
{ 
    WideCharToMultiByte( cp, 0, src, -1, dest, nBuflen, NULL, NULL); 
    return dest; 
} 

// 
// convert multi-byte char string to wide char string 
// 
inline wchar_t * Char2WChar(const char * src ,wchar_t * dest,int nBufLen, UINT cp=CP_ACP) 
{ 
    MultiByteToWideChar( cp, 0, src, (int)strlen(src)+1, dest, nBufLen ); 
    return dest; 
} 

// 
// convert wide char string to wide char string 
// 
inline wchar_t * WChar2WChar(const wchar_t * src, wchar_t * dest, int nBuflen, UINT cp=CP_ACP) 
{ 
    lstrcpynW(dest, src, nBuflen); 
    return dest; 
} 

// 
// convert multi-byte char string to multi-byte char string 
// 
inline char * Char2Char(const char * src, char * dest, int nBufLen, UINT cp=CP_ACP) 
{ 
    lstrcpynA(dest, src, nBufLen); 
    return dest; 
} 


#ifdef UNICODE 
#   define TCHAR2CHAR  WChar2Char 
#   define TCHAR2WCHAR WChar2WChar 
#   define CHAR2TCHAR  Char2WChar 
#   define WCHAR2TCHAR WChar2WChar 
#else 
#   define TCHAR2CHAR  Char2Char 
#   define TCHAR2WCHAR Char2WChar 
#   define CHAR2TCHAR  Char2Char 
#   define WCHAR2TCHAR WChar2Char 
#endif 


#endif  // __STR_CONVERT_H__ 
