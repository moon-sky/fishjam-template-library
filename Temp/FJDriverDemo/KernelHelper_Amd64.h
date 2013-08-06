#pragma once

#include "FDriverHookAPI.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_M_AMD64)

	//KIRQL WPOFFx64();
	//void WPONx64(KIRQL irql);
	SYSTEM_SERVICE_TABLE *GetKeServiceDescriptorTableShadowAddress();
	PVOID GetShadowSSDTFuncAddr(PSYSTEM_SERVICE_TABLE pServiceTable, int nIndex);
	//NTSTATUS ModifyShadowSSDTFunc(PSYSTEM_SERVICE_TABLE pServiceTable, int nIndex, PVOID newAddress);

	NTSTATUS ModifyShadowSSDTFunc(PSYSTEM_SERVICE_TABLE pServiceTable, int nIndex, PVOID newAddress,
		OUT PVOID *Original_ApiAddress, OUT ULONG *PatchSize);

#endif 


#ifdef __cplusplus
}
#endif