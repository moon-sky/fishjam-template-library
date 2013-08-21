#pragma once

#ifdef __cplusplus
extern "C" {
#endif

	KIRQL   ClearWriteProtect(VOID);
	void    RestoreWriteProtect(KIRQL irql);

	SYSTEM_SERVICE_TABLE *GetKeServiceDescriptorTableShadowAddress ();

	PVOID GetSSDTFuncAddr(LONG nServiceIndex);
	NTSTATUS HookSSDTFunc(PHOOK_API_INFO pHookApiInfo);
	NTSTATUS RestoreSSDTFunc(PHOOK_API_INFO pHookApiInfo);


#ifdef __cplusplus
}
#endif