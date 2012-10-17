#include "stdafx.h"
#include "NRecordGraphBuilder.h"
#include "NRecordGraph.h"
#include "DShowUtility.h"

#include <initguid.h>
DEFINE_GUID(CLSID_TestScreenCapture, 
			0xe4db0b81, 0x4288, 0x425d, 0xb2, 0x5e, 0xd1, 0x7, 0xa1, 0x6f, 0xec, 0x62);

DEFINE_GUID(CLSID_BouncingBall,
			0xfd501041, 0x8ebe, 0x11ce, 0x81, 0x83, 0x00, 0xaa, 0x00, 0x57, 0x7d, 0xa1);

//#include "../Filters/include/ISceenCaptureCfg.h"

CDefaultRecordGraphBuilder::CDefaultRecordGraphBuilder(IGraphBuilder* pGraphBuilder, const NRecordParam& param)
: m_spGraphBuilder(pGraphBuilder)
, m_Param(param)
{
}

HRESULT CDefaultRecordGraphBuilder::CreateVideoSrcFilter(IBaseFilter** ppSrcFilter,  
														 IBaseFilter** ppSmartTee,
														 IPin** ppCapturePin, 
														 IPin** ppPreviewPin)
{
	CHECK_POINTER_RETURN_VALUE_IF_FAIL(ppSrcFilter, E_POINTER);
	//CHECK_POINTER_RETURN_VALUE_IF_FAIL(ppSmartTee, E_POINTER);
	//CHECK_POINTER_RETURN_VALUE_IF_FAIL(ppCapturePin, E_POINTER);
	//CHECK_POINTER_RETURN_VALUE_IF_FAIL(ppPreviewPin, E_POINTER);

	HRESULT hr = E_FAIL;
	CComPtr<IBaseFilter>	spVideoSrcFilter;

	//TODO: check m_Param.hWndTarget to detect DX
#if 0
	DX_VERIFY(CDShowUtility::AddFilterByClsid(m_spGraphBuilder, CLSID_NaverScreenCapture, L"Naver Screen Capture", &spVideoSrcFilter));
	CComPtr<IScreenCaptureCfg>	spScreenCaptureCfg;
	if (SUCCEEDED(hr))
	{
		DX_VERIFY(spVideoSrcFilter->QueryInterface(IID_IScreenCaptureCfg, (void**)&spScreenCaptureCfg));
		if (spScreenCaptureCfg)
		{
			CRect rcRecord = m_Param.rcRecord;
			DX_VERIFY(spScreenCaptureCfg->SetCaptureRect(rcRecord.left, rcRecord.top, rcRecord.Width(), rcRecord.Height()));
		}
	}
#else
	//DX_VERIFY(CDShowUtility::AddFilterByClsid(m_spGraphBuilder, CLSID_BouncingBall, L"Bouncing Ball", &spVideoSrcFilter));
    DX_VERIFY(FTL::CFDirectShowUtility::AddFilterByCLSID(m_spGraphBuilder, CLSID_TestScreenCapture, L"Test Screen Capture", &spVideoSrcFilter));
#endif 
	if (FAILED(hr))
	{
		return hr;
	}
	
	if (m_Param.hWndPreview && ::IsWindow(m_Param.hWndPreview))
	{
		//need Preview
		CComPtr<IBaseFilter>	spSmartTee;
        DX_VERIFY(FTL::CFDirectShowUtility::AddFilterByCLSID(m_spGraphBuilder, CLSID_SmartTee, L"SmartTee", &spSmartTee));
		if (spSmartTee)
		{
			//connect video and smart tee for capture and preview -- TODO: Just Debug
			//DX_VERIFY(CDShowUtility::ConnectFilters(m_spGraphBuilder, spVideoSrcFilter, spSmartTee));
			
			*ppSmartTee = spSmartTee.Detach();
			//(*ppSmartTee)->AddRef();

			//CComPtr<IPin> spPin;
			//CComPtr<IEnumPins>	spEnumPins;
			//DX_VERIFY(spSmartTee->EnumPins(&spEnumPins));
			//if (spEnumPins)
			//{
			//	BOOL bFirstPin = TRUE;
			//	while (spEnumPins->Next(1, &spPin, NULL) == S_OK)
			//	{
			//		PIN_DIRECTION pinDir;
			//		DX_VERIFY(spPin->QueryDirection(&pinDir));
			//		if (PINDIR_OUTPUT == pinDir)
			//		{
			//			CComPtr<IPin>	spConnectedPin;
			//			spPin->ConnectedTo(&spConnectedPin);
			//			FTLASSERT(!spConnectedPin);  //not connected

			//			if (bFirstPin)
			//			{
			//				//first is CapturePin
			//				bFirstPin = FALSE;
			//				*ppCapturePin = spPin;
			//			}
			//			else
			//			{
			//				//Second is Preview Pin, and leave
			//				if (ppPreviewPin)
			//				{
			//					*ppPreviewPin = spPin;
			//				}
			//				break;
			//			}
			//		}
			//		spPin.Release();
			//	}
			//	//if can enum
			//	hr = S_OK;
			//}
		}
	}
	else
	{
		//doest not need preview
		//DX_VERIFY(CDShowUtility::GetFirstUnConnectedPin(spVideoSrcFilter, PINDIR_OUTPUT, ppCapturePin));
	}

	*ppSrcFilter = spVideoSrcFilter.Detach();
	//(*ppSrcFilter)->AddRef();
	return hr;
}

HRESULT CDefaultRecordGraphBuilder::CreateVideoEncoderFilter(IBaseFilter** ppFilter, IPin** ppInputPin, IPin** ppOutputPin)
{
	CHECK_POINTER_RETURN_VALUE_IF_FAIL(ppFilter, E_POINTER);

	HRESULT hr = E_FAIL;
	CComPtr<IBaseFilter>	spVideoEncoderFilter;
	CFSharePtr<FTL::CFDShowHardwareMgr> spVideoCaptureHardwareMgr(new FTL::CFDShowHardwareMgr());

	DX_VERIFY(spVideoCaptureHardwareMgr->Refresh(&CLSID_VideoCompressorCategory));
	if(SUCCEEDED(hr))
	{
		DX_VERIFY(spVideoCaptureHardwareMgr->GetBindObject(L"LAME Audio Encoder", IID_IBaseFilter, (void**)&spVideoEncoderFilter));
		if (SUCCEEDED(hr))
		{
			DX_VERIFY(m_spGraphBuilder->AddFilter(spVideoEncoderFilter,  L"Video Encoder"));
			if (SUCCEEDED(hr))
			{
				*ppFilter = spVideoEncoderFilter.Detach();
				//(*ppFilter)->AddRef();
				if (ppInputPin)
				{
					//DX_VERIFY(CDShowUtility::GetFirstUnConnectedPin(spVideoEncoderFilter, PINDIR_INPUT, ppInputPin));
				}
				if (ppOutputPin)
				{
					//DX_VERIFY(CDShowUtility::GetFirstUnConnectedPin(spVideoEncoderFilter, PINDIR_OUTPUT, ppOutputPin));
				}
			}
		}
	}
	return hr;
}

HRESULT CDefaultRecordGraphBuilder::CreateAudioSrcFilter(IBaseFilter** ppSrcFilter)
{
	CHECK_POINTER_RETURN_VALUE_IF_FAIL(ppSrcFilter, E_POINTER);
	HRESULT hr = E_FAIL;

	CFSharePtr<FTL::CFDShowHardwareMgr> spAudioCaptureHardwareMgr(new FTL::CFDShowHardwareMgr());

	DX_VERIFY_EXCEPT1(spAudioCaptureHardwareMgr->Refresh(&CLSID_AudioInputDeviceCategory), S_FALSE);
	if (S_OK == hr)
	{
		CAtlStringW	wstrAudioInputString;
		if (m_Param.strAudioInputName.IsEmpty())
		{
			//get first -- TODO: use enum
			FTL::HardwareMonikerInfoContainer&	rHardwares = spAudioCaptureHardwareMgr->GetAllHardwares();
			if (!rHardwares.empty())
			{
				wstrAudioInputString = (*rHardwares.begin())->wachFriendlyName;
				//m_spAudioCaptureHardwareMgr->GetDefault
			}
		}
		else
		{
			//User Save audio
			wstrAudioInputString = CT2W(m_Param.strAudioInputName);
		}

		if (!wstrAudioInputString.IsEmpty())
		{
			CComPtr<IBaseFilter>	spAudioSrcFilter;
			DX_VERIFY(spAudioCaptureHardwareMgr->GetBindObject(wstrAudioInputString, IID_IBaseFilter, (void **)&spAudioSrcFilter));
			if (SUCCEEDED(hr)) 
			{
				DX_VERIFY(m_spGraphBuilder->AddFilter(spAudioSrcFilter, wstrAudioInputString));
				if (SUCCEEDED(hr))
				{
					*ppSrcFilter = spAudioSrcFilter.Detach();
					//(*ppSrcFilter)->AddRef();
				}
			}
			else
			{
				//maybe user change audio device ?
			}
		}
	}
	return hr;

}

HRESULT CDefaultRecordGraphBuilder::CreateMuxFilter(IBaseFilter** ppFilter, IPin** ppVideoInPin, IPin** ppAudioInPin, IPin** ppOutPin)
{
	CHECK_POINTER_RETURN_VALUE_IF_FAIL(ppFilter, E_POINTER);

	HRESULT hr = S_OK;
	CComPtr<IBaseFilter>	spMuxFilter;

	DX_VERIFY(FTL::CFDirectShowUtility::AddFilterByCLSID(m_spGraphBuilder, CLSID_AviDest, L"Avi Dest", &spMuxFilter));
	if(SUCCEEDED(hr))
	{
		*ppFilter = spMuxFilter.Detach();
		//(*ppFilter)->AddRef();
	}
	return hr;
}

HRESULT CDefaultRecordGraphBuilder::CreateTargetFilter(LPWSTR pwszTargetFileName ,IBaseFilter** ppFilter, IPin** ppInputPin)
{
	CHECK_POINTER_RETURN_VALUE_IF_FAIL(ppFilter, E_POINTER);

	HRESULT hr = E_FAIL;
	CComPtr<IBaseFilter>	spTargetFilter;

	DX_VERIFY(FTL::CFDirectShowUtility::AddFilterByCLSID(m_spGraphBuilder, CLSID_FileWriter, L"File Writer" , &spTargetFilter));
	if (SUCCEEDED(hr))
	{
		//IAMovieSetup, IPersistStream,IAMFilterMiscFlags
		//COM_DETECT_INTERFACE_FROM_LIST(spTargetFilter);
		CComQIPtr<IFileSinkFilter2>	spFileSink(spTargetFilter);
		if (spFileSink)
		{
			AM_MEDIA_TYPE mType = {0};
			//ZeroMemory(&mType, sizeof(AM_MEDIA_TYPE));
			mType.majortype = MEDIATYPE_Stream;
			mType.subtype = MEDIASUBTYPE_Avi;

			DX_VERIFY(spFileSink->SetFileName(pwszTargetFileName, NULL));//&mType));
		}
		*ppFilter = spTargetFilter.Detach();
		//(*ppFilter)->AddRef();
	}
	return hr;

}

