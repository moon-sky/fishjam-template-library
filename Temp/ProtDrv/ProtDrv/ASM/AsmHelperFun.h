#pragma once

#ifdef __cplusplus
extern "C" {
#endif

PVOID __stdcall GetKiSystemCall64Address();
void __stdcall SetSoftBreakPoint();

void __stdcall ClearWriteProtect();
void __stdcall SetWriteProtect();



#ifdef __cplusplus
}
#endif