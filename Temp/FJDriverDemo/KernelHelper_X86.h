#pragma once

#include "FDriverHookAPI.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_M_IX86)

	//KIRQL WPOFFx64();
	//void WPONx64(KIRQL irql);
	SYSTEM_SERVICE_TABLE *GetKeServiceDescriptorTableShadowAddress();

	PVOID GetShadowSSDTFuncAddr(int nIndex);
	NTSTATUS ModifyShadowSSDTFunc(int nIndex, PVOID newAddress,
		OUT PVOID *Original_ApiAddress, OUT ULONG *PatchSize);

#endif 


#ifdef __cplusplus
}
#endif