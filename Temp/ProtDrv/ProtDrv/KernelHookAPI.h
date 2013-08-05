#pragma once

//#pragma intrinsic(__readmsr)

#include "ProtDrvDef.h"

#ifdef __cplusplus
extern "C" {
#endif

NTSTATUS InstallHook(HANDLE hProcessId);
NTSTATUS UnInstallHook();

NTSTATUS SetProtectWndInfo(PPROTECT_WND_INFO pProtectWndInfo);


#ifdef __cplusplus
}
#endif


