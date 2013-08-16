#pragma once

#ifdef __cplusplus
extern "C" {
#endif

	#define MAX_REPLACE_CODE_SIZE		0x40

	typedef struct _INLINE_HOOK_INFO
	{
		PBYTE	pTarget;
		PBYTE	pDetour;
		//PVOID	pTrampoline;
		PBYTE   pOriginal;

		UCHAR   targetBackup[MAX_REPLACE_CODE_SIZE];
		ULONG	targetBackupSize;

		UCHAR   trampoline[MAX_REPLACE_CODE_SIZE];
		ULONG	trampolineSize;

		//#if defined _M_X64
		//	void*	pRelay;
		//#endif
		//	void*	pTrampoline;
		//	void*	pBackup;
		//	bool	isEnabled;
		//	std::vector<uintptr_t>	oldIPs;
		//	std::vector<uintptr_t>	newIPs;

	}INLINE_HOOK_INFO, *PINLINE_HOOK_INFO;

	//struct _INLINE_HOOK_INFO;
	//typedef _INLINE_HOOK_INFO* PINLINE_HOOK_INFO;

	NTSTATUS CreateInlineHook(PVOID pTarget, PVOID const pDetour, PVOID* ppOriginal, PINLINE_HOOK_INFO* ppOutHookInfo);
	NTSTATUS RestoreInlineHook(PINLINE_HOOK_INFO pHookInfo);
	//NTSTATUS EnableInlineHook(PVOID pTarget, PINLINE_HOOK_INFO pHookInfo);
	//NTSTATUS DisableInlineHook(PVOID pTarget, PINLINE_HOOK_INFO pHookInfo);


#ifdef __cplusplus
}
#endif