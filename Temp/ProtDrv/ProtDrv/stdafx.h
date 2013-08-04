#pragma once

#ifdef __cplusplus
extern "C" 
{
#endif
	//#include <ntddk.h>                      //NT式驱动(不支持PnP)
	//#include <ntddstor.h>
	//#include <mountdev.h>
	//#include <ntddvol.h>
	#include <ntifs.h>
	#include <windef.h>
	#include <wingdi.h>

	//#include <intrin.h>

	//#include <winddi.h>
	//#include <wdm.h>                        //WDM式驱动(支持PnP)


#ifndef FNT_VERIFY
#  define FNT_VERIFY(x)     status=(x);
#endif 

#ifdef __cplusplus
}
#endif