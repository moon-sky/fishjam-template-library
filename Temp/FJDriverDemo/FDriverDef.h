#pragma once


#include <Ntstrsafe.h>

#ifndef _countof
#  define _countof(p)       (sizeof(p)/sizeof((p)[0]))
#endif 


#ifndef HANDLE_CASE_TO_STRING_EX
#  define HANDLE_CASE_TO_STRING_EX(buf,len,c, v)\
			case (c):\
			RtlStringCchCopyW(buf,len,v);\
			break;
#endif
#define HANDLE_CASE_TO_STRING(buf,len,c) HANDLE_CASE_TO_STRING_EX(buf, len, c, L#c )


#ifndef HANDLE_CASE_RETURN_STRING_EX
#  define HANDLE_CASE_RETURN_STRING_EX(c, v) \
		case (c):\
		return v;
#endif 
#define    HANDLE_CASE_RETURN_STRING(c) HANDLE_CASE_RETURN_STRING_EX(c, L#c )


typedef struct _tagCaseReturnStringPair{
    LONG        nValue;
    LPCWSTR     pszValueString;
}CaseReturnStringPair ;

#ifndef DEFINE_CASE_RETURN_STRING_EX
#  define    DEFINE_CASE_RETURN_STRING_EX(c, v)     { c, v },
#endif 
#define DEFINE_CASE_RETURN_STRING(c)        DEFINE_CASE_RETURN_STRING_EX(c, L#c)
