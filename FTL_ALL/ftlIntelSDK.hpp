#ifndef FTL_INTEL_SDK_HPP
#define FTL_INTEL_SDK_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlIntelSDK.h"
#endif

namespace FTL
{
	CFIntelMediaSDKErrorInfo::CFIntelMediaSDKErrorInfo(mfxStatus sts) : CFConvertInfoT<CFIntelMediaSDKErrorInfo,mfxStatus>(sts)
	{
	}

	LPCTSTR CFIntelMediaSDKErrorInfo::ConvertInfo()
	{
		if (NULL == m_bufInfo[0])
		{
			switch(m_Info)
			{
				/* reserved for unexpected errors */
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MFX_ERR_UNKNOWN);	//-1, unknown error

				//error codes <0
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MFX_ERR_NULL_PTR);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MFX_ERR_UNSUPPORTED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MFX_ERR_MEMORY_ALLOC);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MFX_ERR_NOT_ENOUGH_BUFFER);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MFX_ERR_INVALID_HANDLE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MFX_ERR_LOCK_MEMORY);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MFX_ERR_NOT_INITIALIZED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MFX_ERR_NOT_FOUND);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MFX_ERR_MORE_DATA);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MFX_ERR_MORE_SURFACE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MFX_ERR_ABORTED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MFX_ERR_DEVICE_LOST);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MFX_ERR_INCOMPATIBLE_VIDEO_PARAM);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MFX_ERR_INVALID_VIDEO_PARAM);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MFX_ERR_UNDEFINED_BEHAVIOR);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MFX_ERR_DEVICE_FAILED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MFX_ERR_MORE_BITSTREAM);

				// warnings >0
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MFX_WRN_IN_EXECUTION);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MFX_WRN_DEVICE_BUSY);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MFX_WRN_VIDEO_PARAM_CHANGED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MFX_WRN_PARTIAL_ACCELERATION);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MFX_WRN_INCOMPATIBLE_VIDEO_PARAM);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MFX_WRN_VALUE_NOT_CHANGED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MFX_WRN_OUT_OF_RANGE);

				/* threading statuses */
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MFX_TASK_DONE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MFX_TASK_WORKING);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), MFX_TASK_BUSY);
			default:
				StringCchPrintf(m_bufInfo,_countof(m_bufInfo), TEXT("Unknown %d"), m_Info);
				break;
			}
		}
		return m_bufInfo;
	}

}

#endif //FTL_INTEL_SDK_HPP