#include "stdafx.h"
#include "NRecordGraph.h"
#include "DShowUtility.h"
#include "RecordNotifyProxy.h"

//#define USE_NAVER_CAPTURE

#define INCLUDE_DETECT_CONTROL  1
//#define INCLUDE_DETECT_DDRAW    1
#define INCLUDE_DETECT_DOCOBJ   1
#define INCLUDE_DETECT_KSPROXY  0
#define INCLUDE_DETECT_MEDIAOBJ 1
#define INCLUDE_DETECT_STRMIF   1
#define INCLUDE_DETECT_WMF      1

//#include <ftlCom.h>
//#include <ftlComDetect.h>

//#ifdef USE_NAVER_CAPTURE
//#include "../Filters/include/ISceenCaptureCfg.h"
//#else
//#endif 
#include "NRecordGraphBuilder.h"

//#include <initguid.h>
//DEFINE_GUID(CLSID_DebugInfoFilter,
//			0x205769BC,0xB121,0x4ca8,0xA4,0xE6,0x84,0xA2,0x0E,0xF2,0x53,0xB7);

CNRecordGraph::CNRecordGraph()
{
	//m_AudPID = 0xC0;
	//m_VidPID = 0xE0;
	HRESULT hr = E_FAIL;
	m_pNotify = NULL;
	m_pRecordNotifyProxy = NULL;

#ifdef FTL_DEBUG
	m_dwGraphRegister = 0;
#endif 
	
	m_status = rcNoInit;
}

CNRecordGraph::~CNRecordGraph()
{

}

HRESULT CNRecordGraph::Init(const NRecordParam& param, IRecordGraphNotify* pNotify)
{
	FTLASSERT(!m_spCaptureGraphBuilder);		//just init once
	m_Param = param;
	m_pNotify = pNotify;

	HRESULT hr = E_FAIL;

	DX_VERIFY(CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC,
		IID_IGraphBuilder, (void **)&m_spGraphBuilder));

	DX_VERIFY(CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER, 
		IID_ICaptureGraphBuilder2, (void**)&m_spCaptureGraphBuilder)); 

	if (!m_spCaptureGraphBuilder || !m_spGraphBuilder)
	{
		m_spCaptureGraphBuilder.Release();
		m_spGraphBuilder.Release();
		return E_FAIL;
	}
	
	DX_VERIFY(m_spCaptureGraphBuilder->SetFiltergraph(m_spGraphBuilder));
	DX_VERIFY(_BuildGraph());


	m_status = rcInited;
	return hr;
}

HRESULT CNRecordGraph::Uninit()
{
	HRESULT hr = S_OK;

	if (m_pRecordNotifyProxy)
	{
		delete m_pRecordNotifyProxy;
		m_pRecordNotifyProxy = NULL;
	}

	_TearDownGraph();

	m_spPreviewVideoWindow.Release();
	m_spSmartTee.Release();

	m_spTargetFilter.Release();
	m_spMuxFilter.Release();
	m_spAudioSrcFilter.Release();
	m_spVideoSrcFilter.Release();
	m_spVideoEncodeFilter.Release();

	m_spMediaSeeking.Release();
	m_spMediaControl.Release();
	m_spGraphBuilder.Release();
	m_spCaptureGraphBuilder.Release();

	m_status = rcNoInit;
	return S_OK;
}

HRESULT CNRecordGraph::_BuildGraph()
{
	FTLASSERT(m_spCaptureGraphBuilder);

	HRESULT hr = E_FAIL;

	CFSharePtr<INRecordGraphBuilder> pNRecordGraphBuilder(new CDefaultRecordGraphBuilder(m_spGraphBuilder, m_Param));
	if (pNRecordGraphBuilder)
	{
		//CComPtr<IPin>	spVideoCapturePin;
		//CComPtr<IPin>	spVideoPreviewPin;
		DX_VERIFY(pNRecordGraphBuilder->CreateVideoSrcFilter(&m_spVideoSrcFilter, &m_spSmartTee, NULL, NULL));//&spVideoCapturePin, &spVideoPreviewPin));
		DX_VERIFY(CDShowUtility::ConnectFilters(m_spGraphBuilder, m_spVideoSrcFilter, m_spSmartTee));

		DX_VERIFY(pNRecordGraphBuilder->CreateVideoEncoderFilter(&m_spVideoEncodeFilter, NULL, NULL));

		if (m_Param.hWndPreview)
		{
			//make preview 
			CComPtr<IBaseFilter>	spVideoRender;
            DX_VERIFY(FTL::CFDirectShowUtility::AddFilterByCLSID(m_spGraphBuilder, CLSID_VideoRenderer, L"Video Render", &spVideoRender));
			DX_VERIFY(CDShowUtility::ConnectFilters(m_spGraphBuilder, m_spSmartTee, m_spVideoEncodeFilter));
			DX_VERIFY(CDShowUtility::ConnectFilters(m_spGraphBuilder, m_spSmartTee, spVideoRender));
		}
		else
		{
			DX_VERIFY(CDShowUtility::ConnectFilters(m_spGraphBuilder, m_spSmartTee, m_spVideoEncodeFilter));
		}

		DX_VERIFY(pNRecordGraphBuilder->CreateMuxFilter(&m_spMuxFilter, NULL, NULL, NULL));
		DX_VERIFY(CDShowUtility::ConnectFilters(m_spGraphBuilder, m_spVideoEncodeFilter, m_spMuxFilter));

		hr = pNRecordGraphBuilder->CreateAudioSrcFilter(&m_spAudioSrcFilter);
		if (m_spAudioSrcFilter)
		{
			//CComPtr<IBaseFilter> spDebugFilter;
			//DX_VERIFY(CDShowUtility::AddFilterByClsid(m_spGraphBuilder, CLSID_DebugInfoFilter, L"Debug Info For Audio", &spDebugFilter));

			DX_VERIFY(CDShowUtility::ConnectFilters(m_spGraphBuilder, m_spAudioSrcFilter, m_spMuxFilter));
			//DX_VERIFY(CDShowUtility::ConnectFilters(m_spGraphBuilder, spDebugFilter, m_spMuxFilter));
		}


		CComPtr<IPin>	spTargetInputPin;
		DX_VERIFY(pNRecordGraphBuilder->CreateTargetFilter(L"D:\\Test.avi", &m_spTargetFilter, &spTargetInputPin));
		DX_VERIFY(CDShowUtility::ConnectFilters(m_spGraphBuilder, m_spMuxFilter, m_spTargetFilter));
	}

	////DX_VERIFY(_CreateTargetFilter());
	//DX_VERIFY(_CreateMuxFilter());
	//DX_VERIFY(_CreateVideoEncodeFilter());

	//DX_VERIFY(_CreateVideoSrcFilter());
	//DX_VERIFY(_CreateAudioSrcFilter());

	if (FAILED(hr))
	{
		Uninit();
	}
	DX_VERIFY(_ConnectGraph());

	CComPtr<IMediaEvent>	spMediaEvent;
	DX_VERIFY(m_spGraphBuilder->QueryInterface(IID_IMediaEvent, (void**)&spMediaEvent));

	if (spMediaEvent)
	{
		m_pRecordNotifyProxy = new CRecordNotifyProxy(spMediaEvent);
	}

	DX_VERIFY(m_spGraphBuilder->QueryInterface(IID_IMediaSeeking, (void**)&m_spMediaSeeking));
	DX_VERIFY(m_spGraphBuilder->QueryInterface(IID_IMediaControl, (void**)&m_spMediaControl));
	DX_VERIFY(m_spGraphBuilder->QueryInterface(IID_IVideoWindow, (void**)&m_spPreviewVideoWindow));

#ifdef FTL_DEBUG
    FTL::CFDirectShowUtility::AddGraphToRot(m_spGraphBuilder, &m_dwGraphRegister);
#endif 

	//m_spGraphBuilder->AddSourceFilter(m_spVideoSrcFilter)
	return hr;
}

HRESULT CNRecordGraph::_ConnectGraph()
{
	HRESULT hr = S_OK;

	return hr;
}

HRESULT CNRecordGraph::_TearDownGraph()
{
	HRESULT hr = S_OK;

	if (m_spPreviewVideoWindow)
	{
		DX_VERIFY(m_spPreviewVideoWindow->put_Owner((OAHWND)NULL));
	}

	if (m_spVideoSrcFilter)
	{
		FTL::CFDirectShowUtility::DisconnectDirFilters(m_spGraphBuilder, m_spVideoSrcFilter,
            PINDIR_OUTPUT, FALSE);
	}
	if (m_spAudioSrcFilter)
	{
		FTL::CFDirectShowUtility::DisconnectDirFilters(m_spGraphBuilder, m_spAudioSrcFilter,
            PINDIR_OUTPUT, FALSE);
	}

#ifdef FTL_DEBUG
	if (m_dwGraphRegister != 0)
	{
        COM_VERIFY(FTL::CFDirectShowUtility::RemoveGraphFromRot(m_dwGraphRegister));
		m_dwGraphRegister = 0;
	}
#endif 

	return hr;
}

HRESULT CNRecordGraph::Start()
{
	HRESULT hr = E_FAIL;
	if (m_spPreviewVideoWindow && ::IsWindow(m_Param.hWndPreview))
	{
		BOOL bRet = FALSE;
		CRect rcClient;
		API_VERIFY(::GetClientRect(m_Param.hWndPreview, rcClient));
		DX_VERIFY(m_spPreviewVideoWindow->put_Owner((OAHWND)m_Param.hWndPreview));
		DX_VERIFY(m_spPreviewVideoWindow->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN));
		DX_VERIFY(m_spPreviewVideoWindow->SetWindowPosition(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height()));
	}

	FTLASSERT(m_spMediaControl);
	if (m_spMediaControl)
	{
		DX_VERIFY_EXCEPT1(m_spMediaControl->Run(), S_FALSE);
		if (SUCCEEDED(hr))
		{
			hr = S_OK;
			m_status = rcRunning;
		}
	}
	return hr;
}

HRESULT CNRecordGraph::Pause()
{
	HRESULT hr = E_FAIL;
	FTLASSERT(m_spMediaControl);
	if (m_spMediaControl)
	{
		DX_VERIFY_EXCEPT1(m_spMediaControl->Pause(), S_FALSE);
		if (SUCCEEDED(hr))
		{
			hr = S_OK;
			m_status = rcPaused;
		}
	}
	return hr;
}

HRESULT CNRecordGraph::Resume()
{
	HRESULT hr = E_FAIL;
	FTLASSERT(m_spMediaControl);
	if (m_spMediaControl)
	{
		DX_VERIFY(m_spMediaControl->Run());
		m_status = rcRunning;
	}
	return hr;
}

HRESULT CNRecordGraph::Stop()
{
	HRESULT hr = E_FAIL;
	FTLASSERT(m_spMediaControl);
	if (m_spMediaControl)
	{
		DX_VERIFY(m_spMediaControl->Stop());
		m_status = rcStopped;
	}
	return hr;
}

HRESULT CNRecordGraph::GetCurrentPosition(LONGLONG *pCurrent)
{
	HRESULT hr = E_FAIL;
	FTLASSERT(m_spMediaSeeking);
	if (m_spMediaSeeking)
	{
		DX_VERIFY(m_spMediaSeeking->GetCurrentPosition(pCurrent));
	}
	return hr;
}