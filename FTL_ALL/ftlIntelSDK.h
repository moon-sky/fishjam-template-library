#ifndef FTL_INTEL_SDK_H
#define FTL_INTEL_SDK_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlIntelSDK.h requires ftlbase.h to be included first
#endif

#ifndef __MFXDEFS_H__
#  error ftlIntelSDK.h requires mfxdefs.h to be included first
#endif 
namespace FTL
{
	/*****************************************************************************************************
	* Intel Media SDK
	*  
	*
	*****************************************************************************************************/
	# define MFX_VERIFY(x)   \
		sts = (x);\
		if(MFX_ERR_NONE != sts)\
		{\
			REPORT_ERROR_INFO(FTL::CFIntelMediaSDKErrorInfo, sts, x);\
		}

	FTLEXPORT class CFIntelMediaSDKErrorInfo : public CFConvertInfoT<CFIntelMediaSDKErrorInfo, mfxStatus>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFIntelMediaSDKErrorInfo);
	public:
		FTLINLINE explicit CFIntelMediaSDKErrorInfo(mfxStatus sts);
		FTLINLINE virtual LPCTSTR ConvertInfo();
	};


}

#endif //FTL_INTEL_SDK_H

#ifndef USE_EXPORT
#  include "ftlIntelSDK.hpp"
#endif