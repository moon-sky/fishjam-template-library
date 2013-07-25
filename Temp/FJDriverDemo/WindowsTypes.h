#pragma once

typedef void *HWND;
typedef void *HDC;
typedef void *HRGN;

typedef unsigned int UINT;
//typedef short unsigned int WORD;
typedef unsigned char BYTE, *PBYTE;

#pragma push(1)

typedef struct _RECT { 
	LONG left; 
	LONG top; 
	LONG right; 
	LONG bottom; 
} RECT, *PRECT;
typedef RECT *LPRECT;

#pragma push(0)