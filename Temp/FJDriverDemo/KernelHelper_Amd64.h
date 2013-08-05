#pragma once

#include "FDriverHookAPI.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_M_AMD64)

	KIRQL ClearWriteProtect();
	void SetWriteProtect(KIRQL irql);
	SYSTEM_SERVICE_TABLE *GetKeServiceDescriptorTableShadowAddress();
	PVOID GetShadowSSDTFuncAddr(PSYSTEM_SERVICE_TABLE pServiceTable, int nIndex);
	NTSTATUS ModifyShadowSSDTFunc(PSYSTEM_SERVICE_TABLE pServiceTable, int nIndex, PVOID newAddress);

#endif 


#ifdef __cplusplus
}
#endif