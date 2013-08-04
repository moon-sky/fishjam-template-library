#pragma once

//#pragma intrinsic(__readmsr)

#include "ProtDrvDef.h"

NTSTATUS InstallHook();
NTSTATUS UnInstallHook();

NTSTATUS SetProtectWndInfo(PPROTECT_WND_INFO pProtectWndInfo);



