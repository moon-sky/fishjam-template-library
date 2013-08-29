#pragma once


#ifdef HOOK_DRIVER
#  define HOOK_ASSERT              NT_ASSERT
#  define HOOK_TRACE               KdPrint
#  include <windef.h>
#else
#  define HOOK_ASSERT
#  define HOOK_VERIFY(x)            bRet=(x); HOOK_ASSERT(bRet)
#  define HOOK_TRACE                
#  include <windows.h>
#endif 

#ifdef __cplusplus
extern "C"{
#endif 

#if defined(_M_IX86)
typedef LONG                        POINTER_TYPE;
#else   //64
typedef LONGLONG                    POINTER_TYPE;
#endif

PVOID HookAllocate(SIZE_T NumberOfBytes);
VOID  HookFree(PVOID pBuffer);


ULONG ClearWriteProtect(LPVOID lpAddress, SIZE_T dwSize);
VOID  RestoreWriteProtect(LPVOID lpAddress, SIZE_T dwSize, ULONG oldProtect);

BOOL CheckIsImported(PBYTE pbCode, PBYTE pbAddress);

#ifdef __cplusplus
}
#endif 
