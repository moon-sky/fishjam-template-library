#pragma once

#ifdef __cplusplus
extern "C" {
#endif

SYSTEM_SERVICE_TABLE *GetKeServiceDescriptorTableShadowAddress ();

PVOID GetShadowSSDTFuncAddr(int nIndex);
NTSTATUS HookShadowSSDTFunc(PHOOK_API_INFO pHookApiInfo);
NTSTATUS RestoreShadowSSDTFunc(PHOOK_API_INFO pHookApiInfo);


#ifdef __cplusplus
}
#endif