#pragma once

#ifdef __cplusplus
extern "C" {
#endif

	KIRQL  WPOFFx64(VOID);
	void WPONx64(KIRQL irql);

	SYSTEM_SERVICE_TABLE *GetKeServiceDescriptorTableShadowAddress ();

	PVOID GetSSDTFuncAddr(LONG nServiceIndex);
	NTSTATUS HookSSDTFunc(PHOOK_API_INFO pHookApiInfo);
	NTSTATUS RestoreSSDTFunc(PHOOK_API_INFO pHookApiInfo);


#ifdef __cplusplus
}
#endif