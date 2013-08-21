#include "stdafx.h"
#include "InlineHook.h"
#include "HookAPIUtil.h"

//#include "KernelHookAPI.h"
//#ifdef FDRIVER_DEMO_PROJECT
//#  include "FDriverUtil.h"
//#endif 
//#include "KernelHelper.h"

#pragma pack(push, 1)
struct JMP_x86_OFFSET
{
    //UCHAR   Jumper[12] = {0xE9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    UCHAR   opcode[1];
    ULONG   operand;
    VOID SetJmpTarget(POINTER_TYPE target, POINTER_TYPE from)
    {
        //32位是相对跳转
        opcode[0] = 0xE9; //jmp operand
        operand = (target - from - sizeof(*this));
    }
};
struct JMP_x64_ABS
{
    //UCHAR   Jumper[12] = {0x48, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xe0};
    UCHAR   opcode[2];
    LONG    offsetAddr;
    ULONGLONG   operand;
    VOID SetJmpTarget(POINTER_TYPE target, POINTER_TYPE /*from*/)
    {
        //64位是绝对跳转
        opcode[0] = 0xff; opcode[1] = 0x25;         //jmp [+imm32]
        offsetAddr = 0;                             //must zero, 
        operand = target;
    }
};
#pragma pack(pop)

#if defined(_M_IX86)
#  define INSTRUCTION_LENGTH_TYPE 0
#  define GetInstructionLength  GetInstructionLength_x86
#  define JUMP_CODE_TYPE   JMP_x86_OFFSET

EXTERN_C int __stdcall    GetInstructionLength_x86(void* InPtr, int InType);
#elif defined(_M_X64)
#  define INSTRUCTION_LENGTH_TYPE 64
#  define GetInstructionLength  GetInstructionLength_x64
#  define JUMP_CODE_TYPE   JMP_x64_ABS

EXTERN_C int __stdcall    GetInstructionLength_x64(void* InPtr, int InType);
#endif 


ULONG LhRoundToNextInstruction(void* InCodePtr, ULONG InCodeSize);


BOOL CreateInlineHook(PVOID pTarget, PVOID const pDetour, PVOID* ppOriginal, PINLINE_HOOK_INFO* ppOutHookInfo)
{
    BOOL bRet = FALSE;
    LONG    EntrySize;
    LONG    nHookJmpSize = sizeof(JUMP_CODE_TYPE);
    //LONG  popSize;
    //POINTER_TYPE  RelAddr;

//#if defined(_M_IX86)
//#elif defined(_M_X64)
//#endif 

    EntrySize = LhRoundToNextInstruction(pTarget, nHookJmpSize); //JUMPER_SIZE);
    HOOK_ASSERT(EntrySize + nHookJmpSize < MAX_REPLACE_CODE_SIZE);


    PINLINE_HOOK_INFO pHookInfo = (PINLINE_HOOK_INFO)HookAllocate(sizeof(INLINE_HOOK_INFO));
    if (pHookInfo)
    {
        JUMP_CODE_TYPE jmpType;

        RtlZeroMemory(pHookInfo, sizeof(INLINE_HOOK_INFO));
        pHookInfo->pTarget = (PBYTE)pTarget;
        pHookInfo->pDetour = (PBYTE)pDetour;
        pHookInfo->pOriginal = pHookInfo->trampoline;


        RtlCopyMemory(pHookInfo->targetBackup, pTarget, EntrySize);
        pHookInfo->targetBackupSize = EntrySize;

        pHookInfo->trampolineSize = EntrySize;
        RtlCopyMemory(pHookInfo->trampoline, pTarget, EntrySize);

        //设置 trampoline 函数 -- 调到 Target 后指定位置
        jmpType.SetJmpTarget((POINTER_TYPE)((PBYTE)(pHookInfo->pTarget) + nHookJmpSize), (POINTER_TYPE)pHookInfo->trampoline + EntrySize);
        RtlCopyMemory(pHookInfo->trampoline + EntrySize, &jmpType, nHookJmpSize);

        //设置 target 函数, 跳转到自定义Detour(Hook) 函数
        jmpType.SetJmpTarget((POINTER_TYPE)((PBYTE)(pHookInfo->pDetour)), (POINTER_TYPE)pHookInfo->pTarget);

        ULONG oldValue = ClearWriteProtect(pHookInfo->pTarget, EntrySize);
        RtlFillMemory(pHookInfo->pTarget, EntrySize, 0x90);     //fill nop
        RtlCopyMemory(pHookInfo->pTarget, &jmpType, nHookJmpSize);
        RestoreWriteProtect(pHookInfo->pTarget, EntrySize, oldValue);

        if (ppOriginal)
        {
            *ppOriginal = pHookInfo->pOriginal;
        }
        *ppOutHookInfo = pHookInfo;

        bRet = TRUE;
    }
    return bRet;
}

BOOL RestoreInlineHook(PINLINE_HOOK_INFO pHookInfo)
{
    BOOL bRet = TRUE;

    ULONG oldValue = ClearWriteProtect(pHookInfo->pTarget, pHookInfo->targetBackupSize);
    RtlCopyMemory(pHookInfo->pTarget, pHookInfo->targetBackup, pHookInfo->targetBackupSize);
    RestoreWriteProtect(pHookInfo->pTarget, pHookInfo->targetBackupSize, oldValue);

    HookFree(pHookInfo);

    return bRet;
}

ULONG LhRoundToNextInstruction(
            void* InCodePtr,
            ULONG InCodeSize)
{
    UCHAR*  Ptr = (UCHAR*)InCodePtr;
    UCHAR*  BasePtr = Ptr;
    LONG    nSize;

    while(BasePtr + InCodeSize > Ptr)
    {
        nSize = GetInstructionLength(Ptr, INSTRUCTION_LENGTH_TYPE);
        if (nSize <= 0)
        {
            return nSize;
        }
        Ptr += nSize;
    }

    return (ULONG)(Ptr - BasePtr);
}
