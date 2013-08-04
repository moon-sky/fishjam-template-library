#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_M_IX86)
    SYSTEM_SERVICE_TABLE *GetKeServiceDescriptorTableShadowX86();
#elif defined(_M_AMD64)
    ULONGLONG GetKeServiceDescriptorTableShadow64();
    ULONGLONG GetSSSDTFuncCurAddr64(ULONGLONG KeServiceDescriptorTableShadow,  ULONG64 Index);
#elif defined(_M_IA64)
#  error NOT SUPPORT IA64 now
#endif 




#ifdef __cplusplus
}
#endif
