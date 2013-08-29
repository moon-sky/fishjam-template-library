#include "stdafx.h"
#include "InlineHook.h"
#include "HookApiUtil.h"

#pragma pack(push, 1)
struct JMP_x86_OFFSET
{
    //32位相对跳转
    UCHAR   opcode[1];
    ULONG   operand;
    VOID SetJmpTarget(POINTER_TYPE target, POINTER_TYPE from)
    {
        opcode[0] = 0xE9; //jmp +imm32
        operand = ((PBYTE)target - (PBYTE)from - sizeof(*this));
        //return (PBYTE)target + sizeof(*this);
    }
};
C_ASSERT(sizeof(JMP_x86_OFFSET) == 5);

struct JMP_x86_ABS
{
    //32位绝对跳转 -- 未测试
    UCHAR   opcode[2];
    LONG    offsetAddr;
    ULONG   operand;
    VOID SetJmpTarget(POINTER_TYPE target, POINTER_TYPE from)
    {
        opcode[0] = 0xff; opcode[1] = 0x25;         //jmp [+imm32]
        offsetAddr = 0;                             //must zero, 
        operand = target; //PtrToUlong(target);
    }
};
C_ASSERT(sizeof(JMP_x86_ABS) == 10);

struct JMP_x64_ABS
{
    //64位绝对跳转
    UCHAR   opcode[2];
    LONG    offsetAddr;
    ULONGLONG   operand;
    PBYTE SetJmpTarget(POINTER_TYPE target, POINTER_TYPE /*from*/)
    {
        opcode[0] = 0xff; opcode[1] = 0x25;         //jmp [+imm32]
        offsetAddr = 0;                             //must zero, 
        operand = target;
    }
};
C_ASSERT(sizeof(JMP_x64_ABS) == 14);

#pragma pack(pop)

#if defined(_M_IX86)
#  define INSTRUCTION_LENGTH_TYPE 0
#  define GetInstructionLength  GetInstructionLength_x86
//#  define JUMP_CODE_TYPE   JMP_x86_ABS
#  define JUMP_CODE_TYPE   JMP_x86_OFFSET

EXTERN_C int __stdcall    GetInstructionLength_x86(void* InPtr, int InType);
#elif defined(_M_X64)
#  define INSTRUCTION_LENGTH_TYPE 64
#  define GetInstructionLength  GetInstructionLength_x64
#  define JUMP_CODE_TYPE   JMP_x64_ABS

EXTERN_C int __stdcall    GetInstructionLength_x64(void* InPtr, int InType);
#endif 


static ULONG _LhRoundToNextInstruction(PBYTE InCodePtr, LONG InCodeSize);
static PBYTE _InlineHookGetRealCode(PBYTE pbCode, LONG nMaxSize);
static BOOL  _IsEndFunctionCode(PBYTE pbCode); //判断是否是表示结束的代码

BOOL CreateInlineHook(PVOID* ppTarget, PVOID pDetour, PVOID* ppOriginal, PINLINE_HOOK_INFO* ppOutHookInfo)
{
    BOOL bRet = FALSE;
    LONG  EntrySize;
    LONG  nHookJmpSize = sizeof(JUMP_CODE_TYPE);
    PBYTE pTarget = (PBYTE)*ppTarget;
    pTarget = (PBYTE)_InlineHookGetRealCode(pTarget, nHookJmpSize);   //get real code by jmp or call
    pDetour = (PBYTE)_InlineHookGetRealCode((PBYTE)pDetour, 1);

    EntrySize = _LhRoundToNextInstruction((PBYTE)pTarget, nHookJmpSize); //JUMPER_SIZE);
    HOOK_ASSERT(EntrySize + nHookJmpSize < MAX_REPLACE_CODE_SIZE);
    HOOK_ASSERT(EntrySize >= nHookJmpSize);
    if (EntrySize < nHookJmpSize)
    {
        HOOK_TRACE(("InlineHook Fail: pTarget=0x%p, pDetour=0x%p, (EntrySize=%d) < (nHookJmpSize=%d)\n",
            pTarget, pDetour, EntrySize, nHookJmpSize));
        return FALSE;
    }

    PINLINE_HOOK_INFO pHookInfo = (PINLINE_HOOK_INFO)HookAllocate(sizeof(INLINE_HOOK_INFO));
    if (pHookInfo)
    {
        JUMP_CODE_TYPE jmpType;

        RtlZeroMemory(pHookInfo, sizeof(INLINE_HOOK_INFO));
        pHookInfo->pTarget = (PBYTE)pTarget;
        pHookInfo->pDetour = (PBYTE)pDetour;
        pHookInfo->pOriginal = pHookInfo->trampoline;

        //RtlCopyMemory(pHookInfo->targetBackup, pTarget, EntrySize);

        pHookInfo->trampolineSize = EntrySize;
        pHookInfo->targetBackupSize = EntrySize;
        RtlCopyMemory(pHookInfo->trampoline, pTarget, EntrySize);

        //设置 trampoline 函数 -- 跳到 Target 后指定位置
        jmpType.SetJmpTarget((POINTER_TYPE)((PBYTE)(pHookInfo->pTarget) + nHookJmpSize), (POINTER_TYPE)pHookInfo->trampoline + EntrySize);
        RtlCopyMemory(pHookInfo->trampoline + EntrySize, &jmpType, nHookJmpSize);

        //设置 target 函数, 跳转到自定义Detour(Hook) 函数
        jmpType.SetJmpTarget((POINTER_TYPE)((PBYTE)(pHookInfo->pDetour)), (POINTER_TYPE)pHookInfo->pTarget);

        ULONG oldValue = ClearWriteProtect(pHookInfo->pTarget, EntrySize);
        RtlFillMemory(pHookInfo->pTarget, EntrySize, 0x90); //fill nop, because EntrySize >= nHookJmpSize
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
    //RtlCopyMemory(pHookInfo->pTarget, pHookInfo->targetBackup, pHookInfo->targetBackupSize);
    RtlCopyMemory(pHookInfo->pTarget, pHookInfo->trampoline, pHookInfo->targetBackupSize);
    RestoreWriteProtect(pHookInfo->pTarget, pHookInfo->targetBackupSize, oldValue);

    HookFree(pHookInfo);

    return bRet;
}

ULONG _LhRoundToNextInstruction(
            PBYTE InCodePtr,
            LONG InCodeSize)
{
    UCHAR*  Ptr = (UCHAR*)InCodePtr;
    UCHAR*  BasePtr = Ptr;
    LONG    nSize;

    while(BasePtr + InCodeSize > Ptr && !_IsEndFunctionCode(InCodePtr))
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

PBYTE _InlineHookGetRealCode(PBYTE pbCode, LONG nMaxSize)
{
    if (pbCode == NULL) {
        return NULL;
    }

    LONG nIndex = 0;

    for (; nIndex < nMaxSize; nIndex++)
    {
        if (pbCode[nIndex + 0] == 0xe9) {   // jmp +imm32
            PBYTE pbNew = pbCode + 5 + *(INT32 *)&pbCode[nIndex + 1];
            HOOK_TRACE(("%p->%p: skipped over long jump.\n", pbCode, pbNew));
            pbCode = pbNew;
            nIndex = 0; //new place, reset nIndex
        }

        // First, skip over the import vector if there is one.
        if (pbCode[nIndex + 0] == 0xff && pbCode[nIndex + 1] == 0x25) {   // jmp [imm32]
            // Looks like an import alias jump, then get the code it points to.
            PBYTE pbTarget = *(PBYTE *)&pbCode[nIndex + 2];
            if (CheckIsImported(pbCode, pbTarget)) {
                PBYTE pbNew = *(PBYTE *)pbTarget;
                HOOK_TRACE(("%p->%p: skipped over import table.\n", pbCode, pbNew));
                pbCode = pbNew;
                nIndex = 0; //new place, reset nIndex
            }
        }

        // Then, skip over a patch jump
        if (pbCode[nIndex + 0] == 0xeb) {   // jmp +imm8
            PBYTE pbNew = pbCode + 2 + *(CHAR *)&pbCode[1];
            HOOK_TRACE(("%p->%p: skipped over short jump.\n", pbCode, pbNew));
            pbCode = pbNew;
            nIndex = 0; //new place, reset nIndex

            // First, skip over the import vector if there is one.
            if (pbCode[nIndex + 0] == 0xff && pbCode[nIndex + 1] == 0x25) {   // jmp [imm32]
                // Looks like an import alias jump, then get the code it points to.
                PBYTE pbTarget = *(PBYTE *)&pbCode[nIndex + 2];
                if (CheckIsImported(pbCode, pbTarget)) {
                    PBYTE pbNew = *(PBYTE *)pbTarget;
                    HOOK_TRACE(("%p->%p: skipped over import table.\n", pbCode, pbNew));
                    pbCode = pbNew;
                }
            }
            // Finally, skip over a long jump if it is the target of the patch jump.
            else if (pbCode[nIndex + 0] == 0xe9) {   // jmp +imm32
                PBYTE pbNew = pbCode + 5 + *(INT32 *)&pbCode[nIndex + 1];
                HOOK_TRACE(("%p->%p: skipped over long jump.\n", pbCode, pbNew));
                pbCode = pbNew;
                nIndex = 0; //new place, reset nIndex
            }
        }
    }
    return pbCode;
}

BOOL _IsEndFunctionCode(PBYTE pbCode)
{
    if (pbCode[0] == 0xeb ||    // jmp +imm8
        pbCode[0] == 0xe9 ||    // jmp +imm32
        pbCode[0] == 0xe0 ||    // jmp eax
        pbCode[0] == 0xc2 ||    // ret +imm8
        pbCode[0] == 0xc3 ||    // ret
        pbCode[0] == 0xcc) {    // brk
            return TRUE;
    }
    else if (pbCode[0] == 0xff && pbCode[1] == 0x25) {  // jmp [+imm32]
        return TRUE;
    }
    else if ((pbCode[0] == 0x26 ||      // jmp es:
        pbCode[0] == 0x2e ||      // jmp cs:
        pbCode[0] == 0x36 ||      // jmp ss:
        pbCode[0] == 0xe3 ||      // jmp ds:
        pbCode[0] == 0x64 ||      // jmp fs:
        pbCode[0] == 0x65) &&     // jmp gs:
        pbCode[1] == 0xff &&       // jmp [+imm32]
        pbCode[2] == 0x25) {
            return TRUE;
    }
    return FALSE;
}