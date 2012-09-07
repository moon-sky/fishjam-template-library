#ifndef FTL_NP_API_HPP
#define FTL_NP_API_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlnpAPI.h"
#endif
#include "ftlNLS.h"

namespace FTL
{
	CFnpAPIErrorInfo::CFnpAPIErrorInfo(NPError err) : CFConvertInfoT<CFnpAPIErrorInfo, NPError>(err)
	{
	}

	LPCTSTR CFnpAPIErrorInfo::ConvertInfo()
	{
		if (NULL == m_bufInfo[0])
		{
			switch(m_Info)
			{
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), NPERR_NO_ERROR);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), NPERR_GENERIC_ERROR);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), NPERR_INVALID_INSTANCE_ERROR);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), NPERR_INVALID_FUNCTABLE_ERROR);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), NPERR_MODULE_LOAD_FAILED_ERROR);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), NPERR_OUT_OF_MEMORY_ERROR);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), NPERR_INVALID_PLUGIN_ERROR);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), NPERR_INVALID_PLUGIN_DIR_ERROR);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), NPERR_INCOMPATIBLE_VERSION_ERROR);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), NPERR_INVALID_PARAM);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), NPERR_INVALID_URL);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), NPERR_FILE_NOT_FOUND);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), NPERR_NO_DATA);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), NPERR_STREAM_NOT_SEEKABLE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), NPERR_TIME_RANGE_NOT_SUPPORTED);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), NPERR_MALFORMED_SITE);
			default:
				StringCchPrintf(m_bufInfo,_countof(m_bufInfo),
					TEXT("Unknown Error %d"), m_Info);
				break;
			}
		}
		return m_bufInfo;
	}

	CFNPVariantInfo::CFNPVariantInfo(const NPVariant& info) : CFConvertInfoT<CFNPVariantInfo, const NPVariant&>(info)
	{
	}

	LPCTSTR CFNPVariantInfo::ConvertInfo()
	{
		if (NULL == m_bufInfo[0])
		{
			//Get NPVariant Type
			FTL::CFStringFormater formaterType;
			GetTypeInfo(formaterType);

			//Get NPVariant Value
			FTL::CFStringFormater formaterValue;
			GetValueInfo(formaterValue);

			StringCchPrintf(m_bufInfo, _countof(m_bufInfo),TEXT("Type = %s, Value = %s"),
				formaterType.GetString(),formaterValue.GetString());
		}
		return m_bufInfo;
	}

	VOID CFNPVariantInfo::GetTypeInfo(CFStringFormater& formaterType)
	{
		CFStringFormater formaterTypeEx;
		formaterTypeEx.Format(TEXT(""));	//make sure empty string (not null)

		NPVariantType varType = m_Info.type;

		TCHAR szType[32] = {0};
		switch(varType)
		{
			HANDLE_CASE_TO_STRING(szType,_countof(szType), NPVariantType_Void);
			HANDLE_CASE_TO_STRING(szType,_countof(szType), NPVariantType_Null);
			HANDLE_CASE_TO_STRING(szType,_countof(szType), NPVariantType_Bool);
			HANDLE_CASE_TO_STRING(szType,_countof(szType), NPVariantType_Int32);
			HANDLE_CASE_TO_STRING(szType,_countof(szType), NPVariantType_Double);
			HANDLE_CASE_TO_STRING(szType,_countof(szType), NPVariantType_String);
			HANDLE_CASE_TO_STRING(szType,_countof(szType), NPVariantType_Object);
		default:
			StringCchPrintf(szType,_countof(szType),TEXT("Unknown Type:%d"),varType);
			break;
		}
		formaterType.Format(TEXT("%s%s"), formaterTypeEx.GetString(), szType);
	}

	VOID CFNPVariantInfo::GetValueInfo(CFStringFormater& formaterValue)
	{
		NPVariantType varType = m_Info.type;
		switch (varType)
		{
		case NPVariantType_Bool:
			formaterValue.Format(TEXT("%s"), NPVARIANT_TO_BOOLEAN(m_Info) ? TEXT("true") : TEXT("false"));
			break;
		case NPVariantType_Int32:
			formaterValue.Format(TEXT("%d(0x%x)"), NPVARIANT_TO_INT32(m_Info), NPVARIANT_TO_INT32(m_Info));
			break;
		case NPVariantType_Double:
			formaterValue.Format(TEXT("%f"), NPVARIANT_TO_DOUBLE(m_Info));
			break;
		case NPVariantType_String:
			{
				const NPString& strInfo = NPVARIANT_TO_STRING(m_Info);
				std::string strUTF8(strInfo.UTF8Characters, strInfo.UTF8Length);
				formaterValue.Format(TEXT("%s)"), CFConversion().UTF8_TO_TCHAR(strUTF8.c_str()));
			}
			break;
		case NPVariantType_Object:
			formaterValue.Format(TEXT("0x%x"), NPVARIANT_TO_OBJECT(m_Info));
			break;
		case NPVariantType_Void:
		case NPVariantType_Null:
			break;
		default:
			ATLASSERT(FALSE);
			break;
		}
	}

	LPCTSTR CFnpAPIUtil::GetNPPVariableString(NPPVariable variable)
	{
		switch (variable)
		{
			HANDLE_CASE_RETURN_STRING(NPPVpluginNameString);
			HANDLE_CASE_RETURN_STRING(NPPVpluginDescriptionString);
			HANDLE_CASE_RETURN_STRING(NPPVpluginWindowBool);
			HANDLE_CASE_RETURN_STRING(NPPVpluginTransparentBool);
			HANDLE_CASE_RETURN_STRING(NPPVjavaClass);
			HANDLE_CASE_RETURN_STRING(NPPVpluginWindowSize);
			HANDLE_CASE_RETURN_STRING(NPPVpluginTimerInterval);
			HANDLE_CASE_RETURN_STRING(NPPVpluginScriptableInstance);
			HANDLE_CASE_RETURN_STRING(NPPVpluginScriptableIID);
			HANDLE_CASE_RETURN_STRING(NPPVjavascriptPushCallerBool);
			HANDLE_CASE_RETURN_STRING(NPPVpluginKeepLibraryInMemory);
			HANDLE_CASE_RETURN_STRING(NPPVpluginNeedsXEmbed);
			HANDLE_CASE_RETURN_STRING(NPPVpluginScriptableNPObject);
			HANDLE_CASE_RETURN_STRING(NPPVformValue);
			HANDLE_CASE_RETURN_STRING(NPPVpluginUrlRequestsDisplayedBool);
			HANDLE_CASE_RETURN_STRING(NPPVpluginWantsAllNetworkStreams);
			HANDLE_CASE_RETURN_STRING(NPPVpluginNativeAccessibleAtkPlugId);
			HANDLE_CASE_RETURN_STRING(NPPVpluginCancelSrcStream);
			HANDLE_CASE_RETURN_STRING(NPPVsupportsAdvancedKeyHandling);
			HANDLE_CASE_RETURN_STRING(NPPVpluginUsesDOMForCursorBool);
			HANDLE_CASE_RETURN_STRING(NPPVpluginDrawingModel);
#if defined(XP_MACOSX)
			HANDLE_CASE_RETURN_STRING(NPPVpluginEventModel);
			HANDLE_CASE_RETURN_STRING(NPPVpluginCoreAnimationLayer);
#endif

#if defined(MOZ_PLATFORM_MAEMO) && ((MOZ_PLATFORM_MAEMO == 5) || (MOZ_PLATFORM_MAEMO == 6))
			HANDLE_CASE_RETURN_STRING(NPPVpluginWindowlessLocalBool);
#endif
		default:
			break;
		}
		FTLTRACEEX(tlWarning, TEXT("GetNPPVariableString Unknown variable %d\n"), variable);
		return TEXT("Unknown");
	}

	LPCTSTR CFnpAPIUtil::GetNPNVariableString(NPPVariable variable)
	{
		switch (variable)
		{
			HANDLE_CASE_RETURN_STRING(NPNVxDisplay);
			HANDLE_CASE_RETURN_STRING(NPNVxtAppContext);
			HANDLE_CASE_RETURN_STRING(NPNVnetscapeWindow);
			HANDLE_CASE_RETURN_STRING(NPNVjavascriptEnabledBool);
			HANDLE_CASE_RETURN_STRING(NPNVasdEnabledBool);
			HANDLE_CASE_RETURN_STRING(NPNVisOfflineBool);
			HANDLE_CASE_RETURN_STRING(NPNVserviceManager);
			HANDLE_CASE_RETURN_STRING(NPNVDOMElement);
			HANDLE_CASE_RETURN_STRING(NPNVDOMWindow);
			HANDLE_CASE_RETURN_STRING(NPNVToolkit);
			HANDLE_CASE_RETURN_STRING(NPNVSupportsXEmbedBool);
			HANDLE_CASE_RETURN_STRING(NPNVWindowNPObject);
			HANDLE_CASE_RETURN_STRING(NPNVPluginElementNPObject);
			HANDLE_CASE_RETURN_STRING(NPNVSupportsWindowless);
			HANDLE_CASE_RETURN_STRING(NPNVprivateModeBool);
			HANDLE_CASE_RETURN_STRING(NPNVsupportsAdvancedKeyHandling);
			HANDLE_CASE_RETURN_STRING(NPNVdocumentOrigin);
			HANDLE_CASE_RETURN_STRING(NPNVpluginDrawingModel);
#if defined(XP_MACOSX)
			HANDLE_CASE_RETURN_STRING(NPNVcontentsScaleFactor);
#ifndef NP_NO_QUICKDRAW
				HANDLE_CASE_RETURN_STRING(NPNVsupportsQuickDrawBool);
#endif
			HANDLE_CASE_RETURN_STRING(NPNVsupportsCoreGraphicsBool);
			HANDLE_CASE_RETURN_STRING(NPNVsupportsOpenGLBool);
			HANDLE_CASE_RETURN_STRING(NPNVsupportsCoreAnimationBool);
			HANDLE_CASE_RETURN_STRING(NPNVsupportsInvalidatingCoreAnimationBool);
#endif
			HANDLE_CASE_RETURN_STRING(NPNVsupportsAsyncBitmapSurfaceBool);

#if defined(XP_WIN)
			HANDLE_CASE_RETURN_STRING(NPNVsupportsAsyncWindowsDXGISurfaceBool);
#endif
#if defined(XP_MACOSX)
#ifndef NP_NO_CARBON
			HANDLE_CASE_RETURN_STRING(NPNVsupportsCarbonBool);
#endif
			HANDLE_CASE_RETURN_STRING(NPNVsupportsCocoaBool);
			HANDLE_CASE_RETURN_STRING(NPNVsupportsUpdatedCocoaTextInputBool);
			HANDLE_CASE_RETURN_STRING(NPNVsupportsCompositingCoreAnimationPluginsBool);
#endif
#if defined(MOZ_PLATFORM_MAEMO) && ((MOZ_PLATFORM_MAEMO == 5) || (MOZ_PLATFORM_MAEMO == 6))
			HANDLE_CASE_RETURN_STRING(NPNVSupportsWindowlessLocal);
#endif
		default:
			break;
		}
		FTLTRACEEX(tlWarning, TEXT("GetNPNVariableString Unknown variable %d\n"), variable);
		return TEXT("Unknown");
	}
}
#endif //FTL_NP_API_HPP