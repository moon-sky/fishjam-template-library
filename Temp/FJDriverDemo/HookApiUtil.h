#pragma once


#ifdef HOOK_DRIVER
#  define HOOK_ASSERT              NT_ASSERT
#  include <windef.h>
#else
#  define HOOK_ASSERT
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


#ifdef __cplusplus
}
#endif 
