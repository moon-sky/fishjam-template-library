#pragma once


#include <Ntstrsafe.h>

#ifndef HANDLE_CASE_TO_STRING_EX
# define HANDLE_CASE_TO_STRING_EX(buf,len,c, v)\
			case (c):\
			RtlStringCchCopyW(buf,len,v);\
			break;
#endif
# define HANDLE_CASE_TO_STRING(buf,len,c) HANDLE_CASE_TO_STRING_EX(buf, len, c, L#c )


#ifndef HANDLE_CASE_RETURN_STRING_EX
# define HANDLE_CASE_RETURN_STRING_EX(c, v) \
		case (c):\
		return v;
#endif 

#ifndef HANDLE_CASE_RETURN_STRING
# define HANDLE_CASE_RETURN_STRING(c) HANDLE_CASE_RETURN_STRING_EX(c, L#c )
#endif 
