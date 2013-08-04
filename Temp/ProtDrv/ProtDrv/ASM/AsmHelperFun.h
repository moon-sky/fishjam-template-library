#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_M_AMD64)
  PVOID __stdcall GetKiSystemCall64Address();
#endif 

void __stdcall SetSoftBreakPoint();

//void __stdcall ClearWriteProtect();
//void __stdcall SetWriteProtect();



#ifdef __cplusplus
}
#endif