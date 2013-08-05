#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_M_IX86)
#  include "KernelHelper_X86.h"
#elif defined(_M_AMD64)
#  include "KernelHelper_Amd64.h"
#else 
error unsupport OS
#endif 


#ifdef __cplusplus
}
#endif