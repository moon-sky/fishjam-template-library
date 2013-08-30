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

    //cli + sti ÀàËÆ push eax + pop eax
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
    //_disable(); //ÐèÒª×¢ÊÍ?
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

BOOL CheckIsImported(PBYTE pbCode, PBYTE pbAddress)
{
    return FALSE;
}

VOID SetExecuteCodeProtect(LPVOID lpAddress, SIZE_T dwSize)
{

}

#else   //Application

PVOID HookAllocate(SIZE_T NumberOfBytes)
{
    return VirtualAlloc(NULL, NumberOfBytes, MEM_COMMIT|MEM_TOP_DOWN, PAGE_EXECUTE_READWRITE);
    //return malloc(NumberOfBytes);
}

VOID  HookFree(PVOID pBuffer)
{
    return VirtualFree(pBuffer, 0, MEM_RELEASE);
    //free(pBuffer);
}

ULONG ClearWriteProtect(LPVOID lpAddress, SIZE_T dwSize)
{
    ULONG oldProtect = 0;
    BOOL bRet = FALSE;
    bRet = VirtualProtect(lpAddress, dwSize, PAGE_EXECUTE_READWRITE, &oldProtect);
    HOOK_ASSERT(bRet);
    return oldProtect;
}

VOID  RestoreWriteProtect(LPVOID lpAddress, SIZE_T dwSize, ULONG oldProtect)
{
    BOOL bRet = FALSE;
    DWORD dwOld = 0;
    HOOK_VERIFY(VirtualProtect(lpAddress, dwSize, oldProtect, &dwOld));

    HOOK_ASSERT(bRet);
}

VOID SetExecuteCodeProtect(LPVOID lpAddress, SIZE_T dwSize)
{
    BOOL bRet = FALSE;
    HANDLE hProcess = GetCurrentProcess();
    DWORD dwOld = 0;
    HOOK_VERIFY(VirtualProtect(lpAddress, dwSize, PAGE_EXECUTE, &dwOld));
    HOOK_VERIFY(FlushInstructionCache(hProcess, lpAddress, dwSize));
}

BOOL CheckIsImported(PBYTE pbCode, PBYTE pbAddress)
{
    BOOL bRet = FALSE;
    MEMORY_BASIC_INFORMATION mbi = {0};
    PIMAGE_DOS_HEADER pDosHeader = NULL;
    PIMAGE_NT_HEADERS pNtHeader = NULL;
    PBYTE pStartAddress = NULL;
    PBYTE pEndAddress = NULL;

    VirtualQuery((PVOID)pbCode, &mbi, sizeof(mbi));
    //__try {
    pDosHeader = (PIMAGE_DOS_HEADER)mbi.AllocationBase;
    if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
        return FALSE;
    }

    pNtHeader = (PIMAGE_NT_HEADERS)((PBYTE)pDosHeader +
        pDosHeader->e_lfanew);
    if (pNtHeader->Signature != IMAGE_NT_SIGNATURE) {
        return FALSE;
    }

    pStartAddress = (PBYTE)pDosHeader + pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress;
    pEndAddress = pStartAddress + pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size;

    if (pbAddress >= pStartAddress &&  pbAddress < pEndAddress) {
        return TRUE;
    }
    return FALSE;
    //}
    //__except(EXCEPTION_EXECUTE_HANDLER) {
    //    return false;
    //}
}

#endif 
