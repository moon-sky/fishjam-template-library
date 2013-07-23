#pragma once


#include <Ntstrsafe.h>

#ifndef _countof
#  define _countof(p)       (sizeof(p)/sizeof((p)[0]))
#endif 


#ifndef HANDLE_CASE_TO_STRING_A_EX
#  define HANDLE_CASE_TO_STRING_A_EX(buf,len,c, v)\
			case (c):\
			RtlStringCchCopyA(buf,len,v);\
			break;
#endif
#define HANDLE_CASE_TO_STRING_A(buf,len,c) HANDLE_CASE_TO_STRING_A_EX(buf, len, c, #c )


#ifndef HANDLE_CASE_RETURN_STRING_A_EX
#  define HANDLE_CASE_RETURN_STRING_A_EX(c, v) \
		case (c):\
		return v;
#endif 
#define    HANDLE_CASE_RETURN_STRING_A(c) HANDLE_CASE_RETURN_STRING_A_EX(c, #c )


typedef struct _tagCaseReturnStringPair{
    LONG        nValue;
    LPSTR      pszValueString;
}CaseReturnStringPair ;

#ifndef DEFINE_CASE_RETURN_STRING_A_EX
#  define    DEFINE_CASE_RETURN_STRING_A_EX(c, v)     { c, v },
#endif 
#define DEFINE_CASE_RETURN_STRING_A(c)        DEFINE_CASE_RETURN_STRING_A_EX(c, #c)


#ifndef HANDLE_COMBINATION_VALUE_TO_STRING_A_EX
#  define HANDLE_COMBINATION_VALUE_TO_STRING_A_EX(buf, n, v, c, s, d) \
	if(((v) & (c)) == (c))\
{\
	RtlStringCchCatA(buf, n, s);\
	v &= ~c;\
}
#  define HANDLE_COMBINATION_VALUE_TO_STRING_A(buf, n, v, c, d)	HANDLE_COMBINATION_VALUE_TO_STRING_A_EX(buf, n, v, c, #c, d) 
#endif 
