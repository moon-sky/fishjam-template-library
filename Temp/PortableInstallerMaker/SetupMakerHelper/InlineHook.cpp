#include "stdafx.h"
#include "InlineHook.h"
#include "HookApiUtil.h"

#pragma pack(push, 1)
#if defined _M_IX86
struct JMP_x86_OFFSET
{
    //32位相对跳转
    UCHAR   opcode[1];
    ULONG   operand;
    VOID SetJmpTarget(POINTER_TYPE target, POINTER_TYPE from)
    {
        opcode[0] = 0xE9; //jmp +imm32
        operand = (ULONG)((PBYTE)target - (PBYTE)from - sizeof(*this));
        //return (PBYTE)target + sizeof(*this);
    }
};
C_ASSERT(sizeof(JMP_x86_OFFSET) == 5);

struct JMP_x86_ABS
{
    //32位绝对跳转 -- 其offsetAddr就是一个绝对地址?
    UCHAR   opcode[2];
    ULONG   offsetAddr;
    ULONG   operand;
    VOID SetJmpTarget(POINTER_TYPE target, POINTER_TYPE from)
    {
        opcode[0] = 0xff; opcode[1] = 0x25;         //jmp [imm32]
        offsetAddr = (ULONG)from + sizeof(opcode) + sizeof(offsetAddr); // +  
        operand = target; //PtrToUlong(target);
    }
};
C_ASSERT(sizeof(JMP_x86_ABS) == 10);

#else

struct JMP_x64_ABS
{
    //64位绝对跳转
    UCHAR   opcode[2];
    ULONG   offsetAddr;
    ULONGLONG   operand;
    VOID SetJmpTarget(POINTER_TYPE target, POINTER_TYPE /*from*/)
    {
        opcode[0] = 0xff; opcode[1] = 0x25;         //jmp [+imm32]
        offsetAddr = 0;                             //must zero, 
        operand = target;
    }
};
C_ASSERT(sizeof(JMP_x64_ABS) == 14);
#endif
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
static PBYTE _InlineHookGetRealCode(PBYTE pbCode, LONG MinCheckSize);
static BOOL  _IsEndFunctionCode(PBYTE pbCode); //判断是否是表示结束的代码

BOOL __cdecl CreateInlineHook(PVOID pTarget, PVOID pDetour, PVOID* ppOriginal, PINLINE_HOOK_INFO* ppOutHookInfo)
{
    BOOL bRet = FALSE;
    LONG  EntrySize;
    LONG  nHookJmpSize = sizeof(JUMP_CODE_TYPE);
    //PBYTE pTarget = (PBYTE)*ppTarget;
    pTarget = (PBYTE)_InlineHookGetRealCode((PBYTE)pTarget, nHookJmpSize);   //get real code by jmp or call
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
        JUMP_CODE_TYPE jmpType = {0};

        RtlFillMemory(pHookInfo, sizeof(INLINE_HOOK_INFO), 0xcc);  //fill brk
        pHookInfo->pTarget = (PBYTE)pTarget;
        pHookInfo->pDetour = (PBYTE)pDetour;
        pHookInfo->pOriginal = pHookInfo->trampoline;
        
        //RtlCopyMemory(pHookInfo->targetBackup, pTarget, EntrySize);

        pHookInfo->trampolineSize = EntrySize + nHookJmpSize;
        pHookInfo->targetBackupSize = EntrySize;
        RtlCopyMemory(pHookInfo->trampoline, pTarget, EntrySize);

        //设置 trampoline 函数 -- 跳到 Target 后指定位置
        jmpType.SetJmpTarget((POINTER_TYPE)((PBYTE)(pHookInfo->pTarget) + nHookJmpSize), (POINTER_TYPE)pHookInfo->trampoline + EntrySize);
        RtlCopyMemory(pHookInfo->trampoline + EntrySize, &jmpType, nHookJmpSize);
        SetExecuteCodeProtect(pHookInfo->trampoline, pHookInfo->trampolineSize);

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
    if (pHookInfo)
    {
        ULONG oldValue = ClearWriteProtect(pHookInfo->pTarget, pHookInfo->targetBackupSize);
        //RtlCopyMemory(pHookInfo->pTarget, pHookInfo->targetBackup, pHookInfo->targetBackupSize);
        RtlCopyMemory(pHookInfo->pTarget, pHookInfo->trampoline, pHookInfo->targetBackupSize);
        RestoreWriteProtect(pHookInfo->pTarget, pHookInfo->targetBackupSize, oldValue);

        HookFree(pHookInfo);
    }

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

PBYTE _InlineHookGetRealCode(PBYTE pbCode, LONG MinCheckSize)
{
    if (pbCode == NULL) {
        return NULL;
    }
    PBYTE   pPtrOffset = pbCode;
    LONG    nOffset = 0;
    LONG    nCurLen = 0;


    while (nOffset < MinCheckSize)
    {
        //skip ILT(Incremental Link Table) -- Debug Version
        if (pPtrOffset[0] == 0xe9) {   // jmp +imm32
            PBYTE pbNew = pPtrOffset + 5 + *(INT32 *)&pPtrOffset[1];
            HOOK_TRACE(("%p->%p: skipped over long jump.\n", pPtrOffset, pbNew));
            pbCode = pbNew;
            pPtrOffset = pbCode;
            nOffset = 0;
            continue;
        }

        // First, skip over the import vector if there is one.
        if (pPtrOffset[0] == 0xff && pPtrOffset[1] == 0x25) {   // jmp [imm32]
            // Looks like an import alias jump, then get the code it points to.
            PBYTE pbTarget = pPtrOffset + 6 + (ULONG)(*(PBYTE *)&pPtrOffset[2]);
            //PBYTE pbTarget = *(PBYTE *)&pPtrOffset[2];
            //if (CheckIsImported(pPtrOffset, pbTarget)) 
            {
                PBYTE pbNew = *(PBYTE *)pbTarget;
                HOOK_TRACE(("%p->%p: skipped over import table.\n", pPtrOffset, pbNew));
                pbCode = pbNew;
                pPtrOffset = pbCode; //new place, reset
                nOffset = 0;
                continue;
            }
        }

        // Then, skip over a patch jump
        if (pPtrOffset[0] == 0xeb) {   // jmp +imm8
            PBYTE pbNew = pPtrOffset + 2 + *(CHAR *)&pPtrOffset[1];
            HOOK_TRACE(("%p->%p: skipped over short jump.\n", pPtrOffset, pbNew));
            pbCode = pbNew;
            pPtrOffset = pbCode; //new place, reset
            nOffset = 0;

            // First, skip over the import vector if there is one.
            if (pPtrOffset[0] == 0xff && pPtrOffset[1] == 0x25) {   // jmp [imm32]
                // Looks like an import alias jump, then get the code it points to.
                PBYTE pbTarget = pPtrOffset + 6 + (ULONG)(*(PBYTE *)&pPtrOffset[2]);
                //PBYTE pbTarget = *(PBYTE *)&pPtrOffset[2];
                //if (CheckIsImported(pPtrOffset, pbTarget)) 
                {
                    PBYTE pbNew = *(PBYTE *)pbTarget;
                    HOOK_TRACE(("%p->%p: skipped over import table.\n", pPtrOffset, pbNew));
                    pbCode = pbNew;
                    pPtrOffset = pbCode; //new place, reset
                    nOffset = 0;
                }
            }
        }

        nCurLen = GetInstructionLength(pPtrOffset, INSTRUCTION_LENGTH_TYPE);
        if (nCurLen <= 0)
        {
            return pbCode;
        }
        nOffset += nCurLen;
        pPtrOffset += nCurLen;
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