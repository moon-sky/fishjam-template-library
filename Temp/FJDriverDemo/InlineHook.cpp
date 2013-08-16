#include "stdafx.h"
#include "KernelHookAPI.h"
#include "InlineHook.h"
#include "FDriverUtil.h"
#include "KernelHelper.h"

#define INLINE_HOOK_TAG				'HOOK'

#pragma pack(push, 1)
struct JMP_x86_OFFSET
{
	UCHAR		opcode[1];
	ULONG		operand;
	//UCHAR		nop[6];
	VOID Init()
	{
		opcode[0] = 0x9E;	//jmp operand
		operand = 0;
	}
};
struct JMP_x64_ABS
{
	//UCHAR		pushcode[1];
	UCHAR		opcode[2];
	LONG		offsetAddr;
	ULONGLONG	operand;
	//UCHAR		jmpcode[2];
	//UCHAR		popcode[1];
	VOID Init() //BOOL bPush, BOOL bPop)
	{
		//opcode[0] = 0x48; opcode[1] = 0xb8;		//mov rax, operand
		//operand = 0;
		//jmpcode[0] = 0xff; jmpcode[1] = 0xe0;	//jmp rax

		opcode[0] = 0xff; opcode[1] = 0x25;			//jmp [+imm32]
		offsetAddr = 0;								//must zero, 
		operand = 0;
	}
};
#pragma pack(pop)

#if defined(_M_IX86)
//#  define JUMPER_SIZE				5
#  define INSTRUCTION_LENGTH_TYPE	0
#  define GetInstructionLength		GetInstructionLength_x86
#  define JUMP_CODE_TYPE			JMP_x86_OFFSET
typedef LONG						POINTER_TYPE;

EXTERN_C int __stdcall				GetInstructionLength_x86(void* InPtr, int InType);
#elif defined(_M_X64)
//#  define JUMPER_SIZE				12
#  define INSTRUCTION_LENGTH_TYPE	64
#  define GetInstructionLength		GetInstructionLength_x64
#  define JUMP_CODE_TYPE			JMP_x64_ABS
typedef LONGLONG					POINTER_TYPE;

EXTERN_C int __stdcall				GetInstructionLength_x64(void* InPtr, int InType);
#endif 


ULONG LhRoundToNextInstruction(void* InCodePtr, ULONG InCodeSize);


NTSTATUS CreateInlineHook(PVOID pTarget, PVOID const pDetour, PVOID* ppOriginal, PINLINE_HOOK_INFO* ppOutHookInfo)
{
	NTSTATUS status = STATUS_SUCCESS;
	LONG           			EntrySize;
	LONG					nHookJmpSize = sizeof(JUMP_CODE_TYPE);
	//LONG					popSize;
	//POINTER_TYPE				RelAddr;

//#if defined(_M_IX86)
//	UCHAR			            Jumper[12] = {0xE9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//#elif defined(_M_X64)
//	UCHAR			            Jumper[12] = {0x48, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xe0};
//#endif 

	EntrySize = LhRoundToNextInstruction(pTarget, nHookJmpSize); //JUMPER_SIZE);
	NT_ASSERT(EntrySize + nHookJmpSize < MAX_REPLACE_CODE_SIZE);


	PINLINE_HOOK_INFO pHookInfo = (PINLINE_HOOK_INFO)ExAllocatePoolWithTag(NonPagedPool, sizeof(INLINE_HOOK_INFO), INLINE_HOOK_TAG);
	if (pHookInfo)
	{
		JUMP_CODE_TYPE	jmpType;

		RtlZeroMemory(pHookInfo, sizeof(INLINE_HOOK_INFO));
		pHookInfo->pTarget = (PBYTE)pTarget;
		pHookInfo->pDetour = (PBYTE)pDetour;
		pHookInfo->pOriginal = pHookInfo->trampoline;


		RtlCopyMemory(pHookInfo->targetBackup, pTarget, EntrySize);
		pHookInfo->targetBackupSize = EntrySize;

		pHookInfo->trampolineSize = EntrySize;
		RtlCopyMemory(pHookInfo->trampoline, pTarget, EntrySize);

		//设置 trampoline 函数 -- 调到 Target 后指定位置，需要push
		jmpType.Init();
		jmpType.operand = (POINTER_TYPE)((PBYTE)(pHookInfo->pTarget) + nHookJmpSize);
		RtlCopyMemory(pHookInfo->trampoline + EntrySize, &jmpType, nHookJmpSize);

		//设置 target 函数, 跳转到自定义Detour(Hook) 函数
		jmpType.Init();
		jmpType.operand = (POINTER_TYPE)((PBYTE)(pHookInfo->pDetour));

		KIRQL irql = WPOFFx64();
		RtlCopyMemory(pHookInfo->pTarget, &jmpType, nHookJmpSize);
		RtlFillMemory(pHookInfo->pTarget + nHookJmpSize, EntrySize - nHookJmpSize, 0x90);  //fill nop
		WPONx64(irql);

		if (ppOriginal)
		{
			*ppOriginal = pHookInfo->pOriginal;
		}
		*ppOutHookInfo = pHookInfo;
	}
	return status;
}

NTSTATUS RestoreInlineHook(PINLINE_HOOK_INFO pHookInfo)
{
	NTSTATUS status = STATUS_SUCCESS;

	KIRQL irql = WPOFFx64();
	RtlCopyMemory(pHookInfo->pTarget, pHookInfo->targetBackup, pHookInfo->targetBackupSize);
	WPONx64(irql);

	ExFreePoolWithTag(pHookInfo, INLINE_HOOK_TAG);

	return status;
}

ULONG LhRoundToNextInstruction(
			void* InCodePtr,
			ULONG InCodeSize)
{
	UCHAR*				Ptr = (UCHAR*)InCodePtr;
	UCHAR*				BasePtr = Ptr;
    LONG                nSize;

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
