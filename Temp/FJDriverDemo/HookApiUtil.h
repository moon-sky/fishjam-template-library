#pragma once

#include <windef.h>

#ifdef HOOK_DRIVER
#  define HOOK_ASSERT              NT_ASSERT
#else
#  define HOOK_ASSERT
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


#ifdef __cplusplus
}
#endif 
