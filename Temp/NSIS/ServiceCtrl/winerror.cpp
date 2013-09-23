
#include "stdafx.h"
#include "winerror.h"


void PushLastError(void)
{
	PushWinError(GetLastError());
}

void PushWinError(DWORD error)
{
	LPVOID lpMsgBuf;

	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);

	pushstring((TCHAR*)lpMsgBuf);

	LocalFree( lpMsgBuf );
}