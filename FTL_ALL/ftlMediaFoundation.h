#ifndef FTL_MEDIA_FOUNDATION_H
#define FTL_MEDIA_FOUNDATION_H
#pragma once

#include "ftlDefine.h"
#include <ftlBase.h>

namespace FTL
{
	/*********************************************************************************************************************************
	*
	*********************************************************************************************************************************/

	# define MF_VERIFY(x)   \
	hr = (x);\
	if(S_OK != hr)\
	{\
		REPORT_ERROR_INFO(FTL::CFMediaFoundationErrorInfo, hr,x);\
	}

	# define MF_VERIFY_EXCEPT1(x,h1) \
	hr = (x);\
	if(S_OK != hr && (h1)!= hr)\
	{\
		REPORT_ERROR_INFO(FTL::CFMediaFoundationErrorInfo, hr,x);\
	}

	# define MF_VERIFY_EXCEPT2(x,h1,h2) \
	hr = (x);\
	if(S_OK != hr && (h1)!= hr && (h2) != hr)\
	{\
		REPORT_ERROR_INFO(FTL::CFMediaFoundationErrorInfo, hr,x);\
	}

	FTLEXPORT class CFMediaFoundationErrorInfo : public CFComErrorInfo
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFMediaFoundationErrorInfo);
	public:
		FTLINLINE explicit CFMediaFoundationErrorInfo(HRESULT hr);
		FTLINLINE virtual LPCTSTR ConvertInfo();
	};

}//FTL

#endif //FTL_MEDIA_FOUNDATION_H

#ifndef USE_EXPORT
#  include "ftlMediaFoundation.hpp"
#endif