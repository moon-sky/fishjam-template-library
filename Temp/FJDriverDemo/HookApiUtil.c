#ifdef HOOK_DRIVER
#include "stdafx.h"
#endif 

#include "HookAPIUtil.h"

#define INLINE_HOOK_TAG             'HOOK'


#ifdef HOOK_DRIVER      //Driver 

PVOID HookAllocate(SIZE_T NumberOfBytes)
{
    return ExAllocatePoolWithTag(NonPagedPool, NumberOfBytes, INLINE_HOOK_TAG);
}
VOID  HookFree(PVOID pBuffer)
{
    ExFreePoolWithTag(pBuffer, INLINE_HOOK_TAG);
}

#if defined(_M_IX86)
ULONG ClearWriteProtect(LPVOID lpAddress, SIZE_T dwSize)
{
    ULONG irql = 0; //KeGetCurrentIrql();

    //cli + sti ¿‡À∆ push eax + pop eax
    __asm
    {
        push  eax;					//cli
        mov   eax, cr0;
        and   eax, not 10000h;		//and     eax, 0x0FFFEFFFF
        mov   cr0, eax;
        pop   eax;					//sti
    }

    return irql;
}

VOID  RestoreWriteProtect(LPVOID lpAddress, SIZE_T dwSize, ULONG oldProtect)
{
    __asm
    {
        push  eax;
        mov   eax, cr0;
        or    eax, 10000h;  //or eax, not 0x0FFFEFFFF
        mov   cr0, eax;
        pop   eax;
    }
}

#  else //64
ULONG ClearWriteProtect(LPVOID lpAddress, SIZE_T dwSize)
{
    KIRQL irql = KeRaiseIrqlToDpcLevel();  //KeGetCurrentIrql();
    UINT64 cr0;
    _disable();
    cr0 = __readcr0();
    cr0 &= ~0x10000; //0xfffffffffffeffff;
    __writecr0(cr0);
    //_disable(); //–Ë“™◊¢ Õ?
    return irql;
}

VOID  RestoreWriteProtect(LPVOID lpAddress, SIZE_T dwSize, ULONG oldProtect)
{
    UINT64 cr0=__readcr0();
    cr0 |= 0x10000;
    //_enable();
    __writecr0(cr0);
    _enable();
    KeLowerIrql((KIRQL)oldProtect);
}
#  endif //64


#else   //Application
PVOID HookAllocate(SIZE_T NumberOfBytes)
{
    return malloc(NumberOfBytes);
}

VOID  HookFree(PVOID pBuffer)
{
    free(pBuffer);
}

ULONG ClearWriteProtect(LPVOID lpAddress, SIZE_T dwSize)
{
    ULONG oldProtect = 0;
    BOOL bRet = FALSE;
    bRet = VirtualProtect(lpAddress, dwSize, PAGE_EXECUTE_READWRITE, &oldProtect);
    HOOK_ASSERT(bRet);
    HOOK_ASSERT(oldProtect == PAGE_EXECUTE_READ);
    return oldProtect;
}

VOID  RestoreWriteProtect(LPVOID lpAddress, SIZE_T dwSize, ULONG oldProtect)
{
    BOOL bRet = FALSE;
    HANDLE hProcess = GetCurrentProcess();
    DWORD dwOld = 0;
    HOOK_VERIFY(VirtualProtect(lpAddress, dwSize, oldProtect, &dwOld));
    HOOK_VERIFY(FlushInstructionCache(hProcess, lpAddress, dwSize));

    HOOK_ASSERT(bRet);
}

#endif 
