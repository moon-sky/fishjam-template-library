#ifndef FTL_DIRECTX_HPP
#define FTL_DIRECTX_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlDirectx.h"
#endif

#ifdef _DXERR_H_
#pragma comment( lib, "DxErr.lib" )
#else
#include <errors.h>
#pragma comment( lib, "Quartz.lib")
#endif 
#include <evcode.h>
#include <algorithm>
#include <MMReg.h>
#include "ftlFunctional.h"

namespace FTL
{

    CFDirectXErrorInfo::CFDirectXErrorInfo(HRESULT error) 
        : CFConvertInfoT<CFDirectXErrorInfo,HRESULT>(error)
    {
    }

    LPCTSTR CFDirectXErrorInfo::ConvertInfo()
    {
#ifdef DXUT_H
        //如果使用了DXUT框架
        switch (m_Info)
        {
            HANDLE_CASE_RETURN_STRING(DXUTERR_NODIRECT3D);
            HANDLE_CASE_RETURN_STRING(DXUTERR_NOCOMPATIBLEDEVICES);
            HANDLE_CASE_RETURN_STRING(DXUTERR_MEDIANOTFOUND);
            HANDLE_CASE_RETURN_STRING(DXUTERR_NONZEROREFCOUNT);
            HANDLE_CASE_RETURN_STRING(DXUTERR_CREATINGDEVICE);
            HANDLE_CASE_RETURN_STRING(DXUTERR_RESETTINGDEVICE);
            HANDLE_CASE_RETURN_STRING(DXUTERR_CREATINGDEVICEOBJECTS);
            HANDLE_CASE_RETURN_STRING(DXUTERR_RESETTINGDEVICEOBJECTS);
            HANDLE_CASE_RETURN_STRING(DXUTERR_INCORRECTVERSION);
        }
#endif 

        //没有执行到这里，说明不是DXUT的错误
        if (NULL == m_bufInfo[0])
        {


#ifdef _DXERR_H_
            const TCHAR* strErrorName = DXGetErrorString( m_Info );
            const TCHAR* strDescription = DXGetErrorDescription( m_Info );
            {
                StringCchPrintf(m_bufInfo, _countof(m_bufInfo),
                    TEXT("%s,Name(%s)"), 
                    strDescription,strErrorName);
            }
#else
            DWORD res = AMGetErrorText(m_Info, m_bufInfo, _countof(m_bufInfo));
            if (res == 0)
            {
                StringCchPrintf(m_bufInfo, _countof(m_bufInfo),
                    TEXT("Unknown Error"));
            }
#endif //no define _DXERR_H_
        }
        return m_bufInfo;
    }

#define GET_EVENT_CODE_INFO_ENTRY(lEvent, classEventCodeInfo ) \
	case (lEvent): { StringCchCopy(m_bufInfo, _countof(m_bufInfo), classEventCodeInfo().GetCodeInfo(lEvent, TEXT(#lEvent), m_lParam1, m_lParam2)); break; }

	class CFDefaultEventCodeInfo
	{
	public:
		virtual LPCTSTR GetCodeInfo(LONG /*lEvent*/, LPCTSTR pszEventCodeName, LONG_PTR /*lparam1*/, LONG_PTR /*lParam2*/)
		{
			return pszEventCodeName;
		}
	protected:
		CFStringFormater	m_strFormater;
	};
	
	class CFErrorAbortEventCodeInfo : public CFDefaultEventCodeInfo
	{
	public:
		virtual LPCTSTR GetCodeInfo(LONG /*lEvent*/, LPCTSTR pszEventCodeName, LONG_PTR lparam1, LONG_PTR /*lParam2*/)
		{
			HRESULT hrError = (HRESULT)(lparam1);
			CFDirectXErrorInfo dxErrInfo(hrError);
			m_strFormater.Format(TEXT("%s, Error=0x%x(%s)"), 
				pszEventCodeName, hrError, dxErrInfo.GetConvertedInfo());
			return m_strFormater.GetString();
		}
	};


	CFDirectShowEventInfo::CFDirectShowEventInfo(LONG lEvent, LONG_PTR lParam1, LONG_PTR lParam2)
		: CFConvertInfoT<CFDirectShowEventInfo,LONG>(lEvent)
		, m_lParam1(lParam1)
		, m_lParam2(lParam2)
	{
	
	}

	LPCTSTR CFDirectShowEventInfo::ConvertInfo()
	{
		if (NULL == m_bufInfo[0])
		{
			switch(m_Info)
			{
				GET_EVENT_CODE_INFO_ENTRY(EC_COMPLETE, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_USERABORT, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_ERRORABORT, CFErrorAbortEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_TIME, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_REPAINT, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_STREAM_ERROR_STOPPED, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_STREAM_ERROR_STILLPLAYING, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_ERROR_STILLPLAYING, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_PALETTE_CHANGED, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_VIDEO_SIZE_CHANGED, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_QUALITY_CHANGE, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_SHUTTING_DOWN, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_CLOCK_CHANGED, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_PAUSED, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_OPENING_FILE, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_BUFFERING_DATA, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_FULLSCREEN_LOST, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_ACTIVATE, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_NEED_RESTART, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_WINDOW_DESTROYED, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_DISPLAY_CHANGED, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_STARVATION, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_OLE_EVENT, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_NOTIFY_WINDOW, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_STREAM_CONTROL_STOPPED, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_STREAM_CONTROL_STARTED, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_END_OF_SEGMENT, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_SEGMENT_STARTED, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_LENGTH_CHANGED, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_DEVICE_LOST, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_SAMPLE_NEEDED, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_PROCESSING_LATENCY, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_SAMPLE_LATENCY, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_SCRUB_TIME, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_STEP_COMPLETE, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_TIMECODE_AVAILABLE, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_EXTDEVICE_MODE_CHANGE, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_STATE_CHANGE, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_GRAPH_CHANGED, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_CLOCK_UNSET, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_VMR_RENDERDEVICE_SET, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_VMR_SURFACE_FLIPPED, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_VMR_RECONNECTION_FAILED, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_PREPROCESS_COMPLETE, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_CODECAPI_EVENT, CFDefaultEventCodeInfo);

				//WindowsMedia SDK filter-specific events
				GET_EVENT_CODE_INFO_ENTRY(EC_WMT_INDEX_EVENT, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_WMT_EVENT, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_BUILT, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_UNBUILT, CFDefaultEventCodeInfo);

				//Other miscellaneous events used by various components
				GET_EVENT_CODE_INFO_ENTRY(EC_SKIP_FRAMES, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_PLEASE_REOPEN, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_STATUS, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_MARKER_HIT, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_LOADSTATUS, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_FILE_CLOSED, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_ERRORABORTEX, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_EOS_SOON, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_CONTENTPROPERTY_CHANGED, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_BANDWIDTHCHANGE, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_VIDEOFRAMEREADY, CFDefaultEventCodeInfo);
				
				//GET_EVENT_CODE_INFO_ENTRY(XXXX, CFDefaultEventCodeInfo);
			default:
				{
					if (m_Info >= EC_USER)
					{
						//private window classes
						LONG userMsg = m_Info - EC_USER;
						StringCchPrintf(m_bufInfo,_countof(m_bufInfo),
							TEXT("0x%08x = (EC_USER+%d[0x%x])"),m_Info, userMsg, userMsg );
					}
					else
					{
						StringCchPrintf(m_bufInfo,_countof(m_bufInfo),
							TEXT("Unknown Event Code %d(0x%08x)"),m_Info,m_Info);
					}
				}
				break;
			}
		}
		return m_bufInfo;
	}

    MediaSampleProperty::MediaSampleProperty()
    {
        IsSyncPoint = FALSE;
        IsPreroll = FALSE;
        IsDiscontinuity = FALSE;
        Size = 0;
        ActualDataLength = 0;
        MediaTimeStart = 0;
        MediaTimeEnd = 0;
        TimeStart = 0;
        TimeEnd = 0;
        //pMediaType = NULL;
    }

    //! DirectShow的功能函数，但基本都没有测试
    HRESULT CFDirectShowUtility::AddGraphToRot(IUnknown* pUnkGraph,DWORD* pdwRegister)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pUnkGraph,E_POINTER);
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pdwRegister,E_POINTER);

        HRESULT hr = E_FAIL;
        CComPtr<IMoniker> pMoniker = NULL;
        CComPtr<IRunningObjectTable> pROT = NULL;

        COM_VERIFY(GetRunningObjectTable(0, &pROT));
        if (SUCCEEDED(hr)) 
        {
            FTLASSERT(pROT);
            WCHAR wsz[256] = { 0 };

			//ROT表中 monikers 的格式为 !FilterGraph X pid Y
			//DirectSpy 显示出来的格式为 pid 0xXXXX(nnnn) IFilterGraph = xxxxx
            COM_VERIFY(StringCchPrintfW(wsz,_countof(wsz), L"FilterGraph %08x pid %08x\0",//FilterGraph %08p pid %p",
                (DWORD_PTR)pUnkGraph, GetCurrentProcessId()));
            COM_VERIFY(CreateItemMoniker(L"!", wsz, &pMoniker));
            if (SUCCEEDED(hr)) 
            {
                //ROTFLAGS_REGISTRATIONKEEPSALIVE 参数保证只要没有调用Remove方法，即使重启GE，注册也有效
                COM_VERIFY_EXCEPT1(pROT->Register(ROTFLAGS_REGISTRATIONKEEPSALIVE, pUnkGraph, pMoniker, pdwRegister), 
					MK_S_MONIKERALREADYREGISTERED);
				if (MK_S_MONIKERALREADYREGISTERED == hr)
				{
					//已经注册了，就不再注册了
					hr = S_OK;
				}
                //pMoniker->Release();
            }
            //pROT->Release();
        }
        return hr;
    }

    HRESULT CFDirectShowUtility::RemoveGraphFromRot(DWORD dwRegister)
    {
        HRESULT hr = E_FAIL;
        CComPtr<IRunningObjectTable> pROT = NULL;
        COM_VERIFY(GetRunningObjectTable(0, &pROT));
        if (SUCCEEDED(hr)) 
        {
            COM_VERIFY(pROT->Revoke(dwRegister));
            //pROT->Release();
        }
        return hr;
    }

	HRESULT CFDirectShowUtility::CopyMediaSample(IMediaSample *pSource, IMediaSample *pDest)
	{
		CHECK_POINTER_RETURN_VALUE_IF_FAIL(pSource,E_POINTER);
		CHECK_POINTER_RETURN_VALUE_IF_FAIL(pDest,E_POINTER);

		HRESULT hr = E_FAIL;

		// Copy the sample data
		long lSourceSize = pSource->GetActualDataLength();

#ifdef _DEBUG    
		long lDestSize = pDest->GetSize();
		FTLASSERT(lDestSize >= lSourceSize);

		//AM_MEDIA_TYPE* pMtSource = NULL, *pMtDest = NULL;
		//if(SUCCEEDED(pSource->GetMediaType(&pMtSource)))
		//{
		//	if (SUCCEEDED(pDest->GetMediaType(&pMtDest)))
		//	{
		//		//TODO:compare
		//		DeleteMediaType(pMtDest);
		//	}
		//	DeleteMediaType(pMtSource);				
		//}
#endif

		BYTE *pSourceBuffer = NULL, *pDestBuffer = NULL;
		DX_VERIFY(pSource->GetPointer(&pSourceBuffer));
		DX_VERIFY(pDest->GetPointer(&pDestBuffer));

		CopyMemory((PVOID) pDestBuffer,(PVOID) pSourceBuffer, lSourceSize);

		// Copy the sample times
		REFERENCE_TIME rtStart = 0, rtEnd = 0;
		if(NOERROR == pSource->GetTime(&rtStart, &rtEnd))
		{
			DX_VERIFY(pDest->SetTime(&rtStart, &rtEnd));
		}

		LONGLONG MediaStart = 0, MediaEnd = 0;
		if(NOERROR == pSource->GetMediaTime(&MediaStart,&MediaEnd))
		{
			DX_VERIFY(pDest->SetMediaTime(&MediaStart,&MediaEnd));
		}

		// Copy the actual data length
		long lDataLength = pSource->GetActualDataLength();
		DX_VERIFY(pDest->SetActualDataLength(lDataLength));

		BOOL bIsSyncPoint = (S_OK == pSource->IsSyncPoint());
		DX_VERIFY(pDest->SetSyncPoint(bIsSyncPoint));

		BOOL bIsPreroll = (S_OK == pSource->IsPreroll());
		DX_VERIFY(pDest->SetPreroll(bIsPreroll));

		BOOL bIsDiscontinuity = (S_OK == pSource->IsDiscontinuity());
		DX_VERIFY(pDest->SetDiscontinuity(bIsDiscontinuity));


		return NOERROR;	
	}

    //注意：IVideoWindow::put_MessageDrain 指定窗体来接收并处理Video窗口的鼠标和键盘消息
    //调用(尚未测试)：ToggleFullScreen(m_pVW,hMain,&hVideo); 和 ToggleFullScreen(m_pVW,hVideo,NULL); 
    HRESULT CFDirectShowUtility::ToggleFullScreen(IVideoWindow *pVW,HWND hDrain, HWND *phOldDrain)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pVW,E_POINTER);

        HRESULT hr=S_OK;
        LONG lMode = OAFALSE;

        // Read current state
        DX_VERIFY(pVW->get_FullScreenMode(&lMode));

        if (lMode == OAFALSE)
        {
            // Save current message drain
            if (phOldDrain)
            {
                DX_VERIFY(pVW->get_MessageDrain((OAHWND *) phOldDrain));
            }
            // Set message drain to application main window
            DX_VERIFY(pVW->put_MessageDrain((OAHWND) hDrain));

            // Switch to full-screen mode
            lMode = OATRUE;
            DX_VERIFY(pVW->put_FullScreenMode(lMode));
        }
        else
        {
            // Switch back to windowed mode
            lMode = OAFALSE;
            DX_VERIFY(pVW->put_FullScreenMode(lMode));

            // Undo change of message drain
            DX_VERIFY(pVW->put_MessageDrain((OAHWND) hDrain));

            // Reset video window
            DX_VERIFY(pVW->SetWindowForeground(-1));

            // Reclaim keyboard focus for player application
            UpdateWindow(hDrain);
            SetForegroundWindow(hDrain);
            SetFocus(hDrain);
        }
        return hr;
    }

    //加入一个指定CLSID的Filter
    HRESULT CFDirectShowUtility::AddFilterByCLSID(IGraphBuilder* pGraph, REFGUID clsid,
        LPCWSTR pName, IBaseFilter** ppFilter)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pGraph,E_POINTER);
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(ppFilter,E_POINTER);

        HRESULT hr = E_FAIL;
        *ppFilter = NULL;
        CComPtr<IBaseFilter> pTmpFilter = NULL;
        hr = CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter,
            reinterpret_cast<void**>(&pTmpFilter));
        if (SUCCEEDED(hr))
        {
            DX_VERIFY(pGraph->AddFilter(pTmpFilter, pName));
            if (SUCCEEDED(hr))
            {
                *ppFilter = pTmpFilter.Detach();
            }
        }
        return hr;
    }

    //得到Filter上未连接的第一个满足majorType的Pin(如果 majorTyte 为 MEDIATYPE_NULL 则不判断majorType)
    HRESULT CFDirectShowUtility::GetUnconnectedPin(IBaseFilter* pFilter,PIN_DIRECTION dir, REFGUID majorType, IPin ** ppPin)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pFilter,E_POINTER);
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(ppPin,E_POINTER);

        HRESULT hr = E_FAIL;
        *ppPin = NULL;
        CComPtr<IEnumPins> pEnumPins = NULL; //CComPtr

        DX_VERIFY(pFilter->EnumPins(&pEnumPins));
        if (SUCCEEDED(hr))
        {
            CComPtr<IPin> pTmpPin = NULL;
            while ((hr = pEnumPins->Next(1,&pTmpPin,NULL)) == S_OK)
            {
                PIN_DIRECTION thisDir = PINDIR_INPUT;
                DX_VERIFY(pTmpPin->QueryDirection(&thisDir));
                if (thisDir == dir)
                {
                    CComPtr<IPin> pConnectedPin = NULL;
                    DX_VERIFY(pTmpPin->ConnectedTo(&pConnectedPin));//VFW_E_NOT_CONNECTED
                    if (SUCCEEDED(hr))  //已经连接上了，不是想要的，释放掉
                    {
                        //SAFE_RELEASE(pConnectedPin);
                        pConnectedPin.Release();
                    }
                    else
                    {
                        if(majorType == GUID_NULL)
                        {
                            //找到了第一个满足方向，同时是未连接的Pin，需要设置返回值，否则
                            *ppPin = pTmpPin.Detach();
                            //SAFE_RELEASE(pEnumPins);
                            return S_OK;
                        }
                        else
                        {
                            CComPtr<IEnumMediaTypes> pEnumTypes = NULL;
                            DX_VERIFY(pTmpPin->EnumMediaTypes(&pEnumTypes));
                            if (SUCCEEDED(hr))
                            {
                                AM_MEDIA_TYPE* pType = NULL;
                                while(*ppPin == NULL && pEnumTypes->Next(1, &pType, 0) == S_OK)
                                {
                                    if(pType->majortype == majorType)
                                    {
                                        *ppPin = pTmpPin.Detach();
#if 0
                                        DeleteMediaType(pType);
#else 
                                        FTLASSERT(FALSE);
#endif 
                                        return S_OK;
                                    }
#if 0
                                    DeleteMediaType(pType);
#else
                                    FTLASSERT(FALSE);
#endif 
                                }
                                //SAFE_RELEASE(pEnumTypes);
                            }
                        }//majorType != GUID_NULL
                    }
                }
                pTmpPin.Release();
            }//while 循环
            //SAFE_RELEASE(pEnumPins);
            if (S_FALSE == hr)  //枚举完了都没有找到合适的
            {
                hr = VFW_S_NO_MORE_ITEMS;
            }
        }
        return hr;
    }

    //!获得Filter上指定方向的第 dirIndex 个Pin -- 不管是否连接
    HRESULT CFDirectShowUtility::GetPin(IBaseFilter* pFilter, PIN_DIRECTION dirRequest ,DWORD dirIndex, IPin** ppFoundPin)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pFilter,E_INVALIDARG);
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(ppFoundPin,E_INVALIDARG);

        HRESULT hr = E_FAIL;
        IEnumPins* pEnum = NULL;
        *ppFoundPin = NULL;
        DWORD dirCount = 0;

        DX_VERIFY(pFilter->EnumPins(&pEnum));
        if (SUCCEEDED(hr)) 
        {
            IPin* pPin = NULL;
            while ( S_OK == (hr = pEnum->Next(1, &pPin, 0))) 
            {
                PIN_DIRECTION dir;
                pPin->QueryDirection(&dir);
                if (dir == dirRequest) 
                {
                    if (dirCount == dirIndex)
                    {
                        *ppFoundPin = pPin;
                        break;
                    }
                    dirCount++;                        
                }
                SAFE_RELEASE(pPin);
            }
            SAFE_RELEASE(pEnum);
        }
        if (*ppFoundPin)
        {
            hr = S_OK;
        }
        else if(S_FALSE == hr)  //Enum->Next no more ITEM
        {
            hr = VFW_S_NO_MORE_ITEMS;
        }
        return hr;
    }


    //!智能连接输出Pin到Filter(第一个未用的输入Pin) -- IGraphBuilder::Connect 是智能连接两个Pin
    HRESULT CFDirectShowUtility::ConnectFilters(IGraphBuilder* pGraph, IPin *pOut, IBaseFilter* pDest)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pGraph,E_POINTER);
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pOut,E_POINTER);
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pDest,E_POINTER);
        HRESULT hr = E_FAIL;
#ifdef FTL_DEBUG
        //必须是输出Pin
        PIN_DIRECTION pinDir = PINDIR_INPUT;
        COM_VERIFY(pOut->QueryDirection(&pinDir));
        FTLASSERT(pinDir == PINDIR_OUTPUT);
#endif
        IPin* pIn = NULL;
        hr = GetUnconnectedPin(pDest,PINDIR_INPUT,MEDIATYPE_NULL,&pIn);
        if (FAILED(hr))
        {
            FTLASSERT(NULL == pIn);
            return hr;
        }
        //将输出Pin连接到输入Pin
        hr = pGraph->Connect(pOut,pIn);
        SAFE_RELEASE(pIn);
        return hr;
    }

    //! 智能连接两个Filter(第一个Filter的未用输出Pin -> 第二个Filter未用的输入Pin)
    HRESULT CFDirectShowUtility::ConnectFilters(IGraphBuilder* pGraph, IBaseFilter* pSrc, IBaseFilter* pDest)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pGraph,E_POINTER);
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pSrc,E_POINTER);
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pDest,E_POINTER);
        HRESULT hr = E_FAIL;

        //找到源Filter的输出Pin
        IPin *pOut = NULL;
        hr = GetUnconnectedPin(pSrc,PINDIR_OUTPUT,MEDIATYPE_NULL,&pOut);
        if (SUCCEEDED(hr))
        {
            FTLASSERT(NULL != pOut);
            hr = ConnectFilters(pGraph,pOut,pDest);
            SAFE_RELEASE(pOut);
        }
        return hr;
    }

    //! 在Filter Graph 中的Filter中查找特定的某种接口(如DV视频解码的 IIPDVDec 等)
    HRESULT CFDirectShowUtility::FindFilterInterface(IFilterGraph* pGraph,REFIID riid, void** ppUnk)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pGraph,E_POINTER);
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(ppUnk,E_POINTER);
        HRESULT hr = E_FAIL;
        IEnumFilters *pEnumFilters = NULL;
        DX_VERIFY(pGraph->EnumFilters(&pEnumFilters));
        if (SUCCEEDED(hr))
        {
            FTLASSERT(pEnumFilters);
            IBaseFilter* pFilter = NULL;
            while ((hr = pEnumFilters->Next(1,&pFilter,NULL)) == S_OK)
            {
                DX_VERIFY(pFilter->QueryInterface(riid,ppUnk));
                SAFE_RELEASE(pFilter);
                if (SUCCEEDED(hr))
                {
                    break;
                }
            }
            SAFE_RELEASE(pEnumFilters);
            if (S_FALSE == hr) //循环完都没有找到
            {
                hr = E_NOINTERFACE;
            }
        }
        return hr;
    }

    //!查找指定Filter的Pin上实现的特定接口
    HRESULT CFDirectShowUtility::FindPinInterface(IBaseFilter* pFilter, REFIID riid,void** ppUnk)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pFilter,E_POINTER);
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(ppUnk,E_POINTER);
        HRESULT hr = E_FAIL;
        IEnumPins *pEnumPins = NULL;
        DX_VERIFY(pFilter->EnumPins(&pEnumPins));
        if (SUCCEEDED(hr))
        {
            FTLASSERT(pEnumPins);
            IPin *pPin = NULL;
            while ((hr = pEnumPins->Next(1,&pPin,NULL)) == S_OK)
            {
                DX_VERIFY_EXCEPT1(pPin->QueryInterface(riid, ppUnk), E_NOINTERFACE);
                SAFE_RELEASE(pPin);
                if (SUCCEEDED(hr))
                {
                    break;
                }
            }
            SAFE_RELEASE(pEnumPins);
            if (S_FALSE == hr)
            {
                //枚举完所有的Pin都没有找到
                hr = E_NOINTERFACE;
            }
        }
        return hr;
    }

    HRESULT CFDirectShowUtility::FindInterfaceAnywhere(IFilterGraph *pGraph, REFIID riid, void** ppUnk)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pGraph,E_POINTER);
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(ppUnk,E_POINTER);
        *ppUnk = NULL;
        HRESULT hr = E_FAIL;
        IEnumFilters *pEnumFilters = NULL;
        DX_VERIFY(pGraph->EnumFilters(&pEnumFilters));
        if (SUCCEEDED(hr))
        {
            FTLASSERT(pEnumFilters);
            IBaseFilter* pFilter = NULL;
            while ((hr = pEnumFilters->Next(1,&pFilter,NULL)) == S_OK)
            {
                DX_VERIFY_EXCEPT1(pFilter->QueryInterface(riid,ppUnk),E_NOINTERFACE);
                if (FAILED(hr)) //Filter 上没有找到接口,在其上的Pin中找
                {
                    hr = FindPinInterface(pFilter,riid,ppUnk);
                }
                SAFE_RELEASE(pFilter);
                if (SUCCEEDED(hr))
                {
                    FTLASSERT(NULL != ppUnk);
                    break;
                }
            }
            SAFE_RELEASE(pEnumFilters);
            if (S_FALSE == hr)
            {
                hr = E_NOINTERFACE;
            }
        }
        return hr;
    }

    //! 获取指定Filter连接着的 上/下 一个Filter
    //! 注意：只能找第一个满足要求(方向)的
    HRESULT CFDirectShowUtility::GetNextFilter(IBaseFilter* pFilter, PIN_DIRECTION dir, IBaseFilter** ppNext)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pFilter,E_POINTER);
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(ppNext,E_POINTER);
        *ppNext = NULL;
        HRESULT hr = E_FAIL;
        IEnumPins *pEnumPins = NULL;
        DX_VERIFY(pFilter->EnumPins(&pEnumPins));
        if (SUCCEEDED(hr))
        {
            IPin *pPin = NULL;
            while ((hr = pEnumPins->Next(1,&pPin,NULL)) == S_OK)
            {
                PIN_DIRECTION thisDir = PINDIR_INPUT;
                DX_VERIFY(pPin->QueryDirection(&thisDir));
                if (thisDir == dir) //找到合适方向的
                {
                    IPin* pConnectedPin = NULL;
                    DX_VERIFY_EXCEPT1(pPin->ConnectedTo(&pConnectedPin),VFW_E_NOT_CONNECTED);
                    if (SUCCEEDED(hr)) //是连接上的，查找Pin的信息，获取所在的Filter
                    {
                        PIN_INFO pinInfo = {0};
                        DX_VERIFY(pConnectedPin->QueryPinInfo(&pinInfo));
                        *ppNext = pinInfo.pFilter;

                        SAFE_RELEASE(pConnectedPin);
                        SAFE_RELEASE(pPin);
                        SAFE_RELEASE(pEnumPins);

                        return hr;
                    }
                }
                SAFE_RELEASE(pPin);
            }
            SAFE_RELEASE(pEnumPins);
        }
        return hr;
    }

    //! 清除掉Filter Graph中所有的Filter
    HRESULT CFDirectShowUtility::ClearAllFilters(IFilterGraph *pGraph)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pGraph,E_POINTER);
        HRESULT hr = E_FAIL;
#ifdef FTL_DEBUG
        //必须是停止状态
        IMediaControl *pControl = NULL;
        FILTER_STATE state = State_Paused;
        DX_VERIFY(pGraph->QueryInterface(IID_IMediaControl,(void**)&pControl));
        DX_VERIFY(pControl->GetState(INFINITE,(OAFilterState*)&state));
        FTLASSERT(State_Stopped == state); 
#endif
        IEnumFilters* pEnumFilters = NULL;
        DX_VERIFY(pGraph->EnumFilters(&pEnumFilters));
        if (SUCCEEDED(hr))
        {
            IBaseFilter* pFilter = NULL;
            while((hr = pEnumFilters->Next(1,&pFilter,NULL)) == S_OK)
            {
                DX_VERIFY(pGraph->RemoveFilter(pFilter));
                DX_VERIFY(pEnumFilters->Reset());
                SAFE_RELEASE(pFilter);
            }
            SAFE_RELEASE(pEnumFilters);
            if (S_FALSE == hr)  //循环完毕
            {
                hr = S_OK;
            }
        }
        return hr;
    }

    HRESULT CFDirectShowUtility::DisconnectDirFilters(IFilterGraph* pGraph, IBaseFilter* pFilter, 
        PIN_DIRECTION dir, BOOL bRemoveFromGraph/* = FALSE */)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pGraph,E_POINTER);
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pFilter,E_POINTER);
        HRESULT hr = E_FAIL;
        IEnumPins *pEnumPins = NULL;
        //BOOL bPass = TRUE;
        //枚举Filter上的所有Pin
        DX_VERIFY(pFilter->EnumPins(&pEnumPins));
        if (SUCCEEDED(hr))
        {   
            IPin *pPin = NULL;
			DX_VERIFY(pEnumPins->Reset());
            while ((hr = pEnumPins->Next(1,&pPin,NULL)) == S_OK)
            {
                PIN_DIRECTION thisDir = PINDIR_INPUT;
                DX_VERIFY(pPin->QueryDirection(&thisDir));
                if (dir == thisDir)  //是我们想删除方向的Pin，看是否有连接
                {
                    IPin *pConnectedPin = NULL;
                    DX_VERIFY_EXCEPT1(pPin->ConnectedTo(&pConnectedPin), VFW_E_NOT_CONNECTED);
                    if (SUCCEEDED(hr)) //有连接的Pin，获取该Pin所在的Filter，递归调用
                    {
                        PIN_INFO pinInfo = {0};
                        DX_VERIFY(pConnectedPin->QueryPinInfo(&pinInfo));

                        DX_VERIFY(DisconnectDirFilters(pGraph, pinInfo.pFilter, dir, bRemoveFromGraph));

                        //断开当前Pin连接
                        DX_VERIFY(pGraph->Disconnect(pConnectedPin));
                        DX_VERIFY(pGraph->Disconnect(pPin));

                        if (bRemoveFromGraph)
                        {
                            DX_VERIFY(pGraph->RemoveFilter(pinInfo.pFilter));
                        }

                        SAFE_RELEASE(pinInfo.pFilter);
                        SAFE_RELEASE(pConnectedPin);
                    }
                }
                SAFE_RELEASE(pPin);
            }
			if (S_FALSE == hr)
			{
				//IEnumPins::Next 枚举完毕
				hr = S_OK;
			}
            SAFE_RELEASE(pEnumPins);
        }
        return hr;
    }

    //! 在进行格式转换时，如果不需要预览，可以设置不使用参考时钟，以使数据流中的Sample以最快速度传送。
    //  使用 IFilterGraph::SetDefaultSyncSource 恢复缺省的
    HRESULT CFDirectShowUtility::SetNoClock(IFilterGraph *pGraph, IReferenceClock ** pOldClock/* = NULL*/)
    {
        CHECK_POINTER_READABLE_RETURN_VALUE_IF_FAIL(pGraph,E_POINTER);

        HRESULT hr = E_FAIL;
        IMediaFilter *pFilter = NULL;
        DX_VERIFY(pGraph->QueryInterface(IID_IMediaFilter,(void**)&pFilter));
        if (SUCCEEDED(hr))
        {
            if (NULL != pOldClock)
            {
                DX_VERIFY(pFilter->GetSyncSource(pOldClock));
            }
            DX_VERIFY(pFilter->SetSyncSource(NULL));
            SAFE_RELEASE(pFilter);
        }
        return hr;
    }

    HRESULT CFDirectShowUtility::RenderOutputPins(IGraphBuilder *pGB, IBaseFilter *pFilter)
    {
        HRESULT         hr = S_OK;
        IEnumPins *     pEnumPin = NULL;
        IPin *          pConnectedPin = NULL;
        IPin *          pPin = NULL;
        PIN_DIRECTION   PinDirection;
        ULONG           ulFetched;

        // Enumerate all pins on the filter
        DX_VERIFY(pFilter->EnumPins( &pEnumPin ));
        if(SUCCEEDED(hr))
        {
            // Step through every pin, looking for the output pins
            while (S_OK == (hr = pEnumPin->Next( 1L, &pPin, &ulFetched)))
            {
                // Is this pin connected?  We're not interested in connected pins.
                DX_VERIFY_EXCEPT1(pPin->ConnectedTo(&pConnectedPin),VFW_E_NOT_CONNECTED);
                if (pConnectedPin)
                {
                    pConnectedPin->Release();
                    pConnectedPin = NULL;
                }
                // If this pin is not connected, render it.
                if (VFW_E_NOT_CONNECTED == hr)
                {
                    DX_VERIFY(pPin->QueryDirection( &PinDirection ));
                    if ( ( S_OK == hr ) && ( PinDirection == PINDIR_OUTPUT ) )
                    {
                        DX_VERIFY(pGB->Render(pPin));
                    }
                }
                pPin->Release();
                // If there was an error, stop enumerating
                if (FAILED(hr))
                    break;
            }
        }
        // Release pin enumerator
        pEnumPin->Release();
        return hr;
    }


    HRESULT CFDirectShowUtility::GetMediaSampleProperty(IMediaSample* pSample,MediaSampleProperty* pProperty)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pSample,E_POINTER);    
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pProperty,E_POINTER);

        HRESULT hr = E_FAIL;
        pProperty->Size = pSample->GetSize();
        pProperty->ActualDataLength = pSample->GetActualDataLength();
        pProperty->IsSyncPoint = (S_OK == pSample->IsSyncPoint());
        pProperty->IsPreroll = (S_OK == pSample->IsPreroll());
        pProperty->IsPreroll = (S_OK == pSample->IsPreroll());
        pProperty->IsDiscontinuity = (S_OK == pSample->IsDiscontinuity());

        hr = pSample->GetMediaTime(&pProperty->MediaTimeStart,&pProperty->MediaTimeEnd); //VFW_E_MEDIA_TIME_NOT_SET
        DX_VERIFY(pSample->GetTime(&pProperty->TimeStart,&pProperty->TimeEnd));
        return S_OK;
    }

    //TODO:接口设计
    HRESULT CFDirectShowUtility::GetMediaTime(IMediaSeeking *pMS)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pMS,E_POINTER);
        HRESULT hr;

        // Is media time supported for this file?
        DX_VERIFY(pMS->IsFormatSupported(&TIME_FORMAT_MEDIA_TIME));

        // Read the time format to restore later
        GUID guidOriginalFormat;
        DX_VERIFY(pMS->GetTimeFormat(&guidOriginalFormat));
        if (FAILED(hr))
            return hr;

        // Ensure media time format for easy display
        DX_VERIFY(pMS->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME));
        if (FAILED(hr))
            return hr;

        // Read the file's duration
        LONGLONG llDuration=0;
        DX_VERIFY(pMS->GetDuration(&llDuration));
        if (FAILED(hr))
            return hr;

        // Return to the original format
        if (guidOriginalFormat != TIME_FORMAT_MEDIA_TIME)
        {
            DX_VERIFY(pMS->SetTimeFormat(&guidOriginalFormat));
            if (FAILED(hr))
                return hr;
        }

        // Convert the LONGLONG duration into human-readable format
        unsigned long nTotalMS = (unsigned long) ((float) llDuration / 10000.0); // 100ns -> ms
        int nSeconds = nTotalMS / (int) 1000;
        int nMinutes = nSeconds / (int) 60;
        nSeconds %= 60;

        // Update the display
        TCHAR szDuration[64];
        hr = StringCchPrintf(szDuration, NUMELMS(szDuration), _T("Duration: %d min %2d sec\0"), nMinutes, nSeconds);

        REFERENCE_TIME rtNow;
        // Read the current stream position
        DX_VERIFY(pMS->GetCurrentPosition(&rtNow));
        return hr;
    }

    //从Filter上抓图
    HRESULT CFDirectShowUtility::SnapShotBitmap(IBasicVideo* pBasicVideo, LPCTSTR pszOutFile)
    {
        CHECK_POINTER_READABLE_RETURN_VALUE_IF_FAIL(pBasicVideo,E_POINTER);
        CHECK_POINTER_ISSTRING_PTR_RETURN_VALUE_IF_FAIL(pszOutFile,E_POINTER);
        HRESULT hr = E_FAIL;
        long bitmapSize = 0;
        DX_VERIFY(pBasicVideo->GetCurrentImage(&bitmapSize,NULL));
        if (SUCCEEDED(hr))
        {
            //分配内存
            BYTE* pBuffer = new BYTE[bitmapSize];
            DX_VERIFY(pBasicVideo->GetCurrentImage(&bitmapSize,(long*)pBuffer));
            if (SUCCEEDED(hr))
            {
                LPBITMAPINFOHEADER lpBitmapInfoHeader = (LPBITMAPINFOHEADER)pBuffer;
                int nColors = 1 << lpBitmapInfoHeader->biBitCount;
                if (nColors > 256 ) //如果颜色数大于256，就不需要调色板了
                {
                    nColors = 0;
                }
                BITMAPFILEHEADER bmpFileHeader = {0};
                bmpFileHeader.bfType = ((DWORD)('M' << 8) | 'B' ); //标记为 "BM"
                bmpFileHeader.bfSize = bitmapSize + sizeof(bmpFileHeader);
                bmpFileHeader.bfOffBits = (DWORD)(sizeof(BITMAPFILEHEADER) 
                    + lpBitmapInfoHeader->biSize + nColors * sizeof(RGBQUAD));

                HANDLE hFile = ::CreateFile(pszOutFile,GENERIC_WRITE,0,NULL,
                    CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
                if (INVALID_HANDLE_VALUE != hFile)
                {
                    BOOL bRet = FALSE;
                    API_VERIFY(::WriteFile(hFile,&bmpFileHeader,sizeof(BITMAPFILEHEADER),NULL,NULL));
                    API_VERIFY(::WriteFile(hFile,pBuffer,bitmapSize,NULL,NULL));
                    SAFE_CLOSE_HANDLE(hFile,INVALID_HANDLE_VALUE);
                    if (!bRet)
                    {
                        hr = HRESULT_FROM_WIN32(GetLastError());
                    }
                }
            }
            SAFE_DELETE_ARRAY(pBuffer);
        }
        return hr;
    }

    HRESULT CFDirectShowUtility::InitCaptureGraphBuilder(IGraphBuilder **ppGraph, 
        ICaptureGraphBuilder2 ** ppCapture)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(ppGraph,E_POINTER);
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(ppCapture,E_POINTER);

        HRESULT hr = E_FAIL;
        *ppGraph = NULL;
        *ppCapture = NULL;
        IGraphBuilder *pGraph = NULL;
        ICaptureGraphBuilder2 * pCapture = NULL;

		// Create the capture graph builder
        DX_VERIFY(CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL,
            CLSCTX_INPROC,	// CLSCTX_INPROC_SERVER
			IID_ICaptureGraphBuilder2, (void**)&pCapture));
        if (SUCCEEDED(hr))
        {
			// Create the filter graph
            DX_VERIFY(CoCreateInstance(CLSID_FilterGraph, NULL,
				CLSCTX_INPROC, //CLSCTX_INPROC_SERVER,
                IID_IGraphBuilder,(void**)&pGraph));
            if (SUCCEEDED(hr))
            {
				//Attach the filter graph to the capture graph
                DX_VERIFY(pCapture->SetFiltergraph(pGraph));
                if (SUCCEEDED(hr))
                {
                    *ppCapture = pCapture;
                    *ppGraph = pGraph;
                    return S_OK;
                }
            }
        }
        SAFE_RELEASE(pGraph);
        SAFE_RELEASE(pCapture);
        return hr;
    }


	LPCTSTR CFDirectShowUtility::GetWaveFormatExTagString(WORD wWaveFormatTag)
	{
		switch (wWaveFormatTag)
		{
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_UNKNOWN);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_PCM);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_ADPCM);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_IEEE_FLOAT);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_VSELP);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_IBM_CVSD);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_ALAW);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_MULAW);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_DTS);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_DRM);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_WMAVOICE9);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_WMAVOICE10);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_OKI_ADPCM);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_DVI_ADPCM | WAVE_FORMAT_IMA_ADPCM);
			//HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_IMA_ADPCM);  //WAVE_FORMAT_DVI_ADPCM
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_MEDIASPACE_ADPCM);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_SIERRA_ADPCM);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_G723_ADPCM);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_DIGISTD);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_DIGIFIX);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_DIALOGIC_OKI_ADPCM);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_MEDIAVISION_ADPCM);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_CU_CODEC);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_YAMAHA_ADPCM);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_SONARC);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_DSPGROUP_TRUESPEECH);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_ECHOSC1);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_AUDIOFILE_AF36);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_APTX);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_AUDIOFILE_AF10);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_PROSODY_1612);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_LRC);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_DOLBY_AC2);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_GSM610);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_MSNAUDIO);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_ANTEX_ADPCME);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_CONTROL_RES_VQLPC);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_DIGIREAL);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_DIGIADPCM);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_CONTROL_RES_CR10);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_NMS_VBXADPCM);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_CS_IMAADPCM);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_ECHOSC3);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_ROCKWELL_ADPCM);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_ROCKWELL_DIGITALK);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_XEBEC);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_G721_ADPCM);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_G728_CELP);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_MSG723);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_MPEG);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_RT24);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_PAC);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_MPEGLAYER3);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_LUCENT_G723);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_CIRRUS);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_ESPCM);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_VOXWARE);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_CANOPUS_ATRAC);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_G726_ADPCM);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_G722_ADPCM);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_DSAT_DISPLAY);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_VOXWARE_BYTE_ALIGNED);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_VOXWARE_AC8);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_VOXWARE_AC10);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_VOXWARE_AC16);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_VOXWARE_AC20);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_VOXWARE_RT24);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_VOXWARE_RT29);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_VOXWARE_RT29HW);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_VOXWARE_VR12);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_VOXWARE_VR18);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_VOXWARE_TQ40);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_SOFTSOUND);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_VOXWARE_TQ60);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_MSRT24);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_G729A);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_MVI_MVI2);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_DF_G726);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_DF_GSM610);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_ISIAUDIO);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_ONLIVE);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_SBC24);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_DOLBY_AC3_SPDIF);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_MEDIASONIC_G723);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_PROSODY_8KBPS);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_ZYXEL_ADPCM);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_PHILIPS_LPCBB);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_PACKED);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_MALDEN_PHONYTALK);
#if 0
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_RAW_AAC1);
#endif 
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_RHETOREX_ADPCM);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_IRAT);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_VIVO_G723);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_VIVO_SIREN);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_DIGITAL_G723);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_SANYO_LD_ADPCM);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_SIPROLAB_ACEPLNET);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_SIPROLAB_ACELP4800);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_SIPROLAB_ACELP8V3);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_SIPROLAB_G729);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_SIPROLAB_G729A);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_SIPROLAB_KELVIN);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_G726ADPCM);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_QUALCOMM_PUREVOICE);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_QUALCOMM_HALFRATE);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_TUBGSM);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_MSAUDIO1);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_WMAUDIO2);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_WMAUDIO3);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_WMAUDIO_LOSSLESS);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_WMASPDIF);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_UNISYS_NAP_ADPCM);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_UNISYS_NAP_ULAW);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_UNISYS_NAP_ALAW);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_UNISYS_NAP_16K);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_CREATIVE_ADPCM);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_CREATIVE_FASTSPEECH8);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_CREATIVE_FASTSPEECH10);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_UHER_ADPCM);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_QUARTERDECK);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_ILINK_VC);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_RAW_SPORT);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_ESST_AC3);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_GENERIC_PASSTHRU);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_IPI_HSX);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_IPI_RPELP);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_CS2);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_SONY_SCX);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_FM_TOWNS_SND);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_BTV_DIGITAL);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_QDESIGN_MUSIC);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_VME_VMPCM);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_TPC);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_OLIGSM);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_OLIADPCM);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_OLICELP);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_OLISBC);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_OLIOPR);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_LH_CODEC);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_NORRIS);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_SOUNDSPACE_MUSICOMPRESS);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_MPEG_ADTS_AAC);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_MPEG_RAW_AAC);
#if 0
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_MPEG_LOAS);
#endif 
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_NOKIA_MPEG_ADTS_AAC);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_NOKIA_MPEG_RAW_AAC);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_VODAFONE_MPEG_ADTS_AAC);
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_VODAFONE_MPEG_RAW_AAC);
#if 0
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_MPEG_HEAAC);
#endif 
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_DVM);
#if 0
			HANDLE_CASE_RETURN_STRING(WAVE_FORMAT_DTS2);
#endif
			
			//HANDLE_CASE_RETURN_STRING(XXXXXXXX);
		default:
			FTLASSERT(FALSE);
			return TEXT("");
		}
	}

//#ifdef USE_DIRECTX_9
    //////////////////////////////////////////////////////////////////////////
    //                  Direct3DUtility
    //////////////////////////////////////////////////////////////////////////
    DWORD Direct3DUtility::EnterMsgLoop( bool (*ptr_display)(float timeDelta) )
    {
        MSG msg;
        ::ZeroMemory(&msg, sizeof(MSG));
        static float lastTime = (float)timeGetTime();

        while(TRUE)
        {
            if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
            {
                if( WM_QUIT == msg.message )
                {
                    break;
                }
                //如果有消息需要处理
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }
            else
            {
                //没有消息，进行其他的游戏逻辑、AI等处理
                float currTime = (float)timeGetTime();
                float timeDelta = (currTime - lastTime)*0.001f;
                ptr_display(timeDelta); // call display function
                lastTime = currTime;
            }
        }
        return 0;//msg.wParam;

#if 0
        //以下是使用 DXUT 的自定义消息循环,通常是使用 DXUTMainLoop 
        // Custom main loop    
        HWND hWnd = DXUTGetHWND();
        BOOL bGotMsg;
        MSG  msg;
        msg.message = WM_NULL;
        PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );

        while( WM_QUIT != msg.message )
        {        
            // Use PeekMessage() so we can use idle time to render the scene
            bGotMsg = ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) != 0 );
            if( bGotMsg )
            {
                // Translate and dispatch the message
                if( 0 == TranslateAccelerator( hWnd, NULL, &msg ) )
                {
                    TranslateMessage( &msg );
                    DispatchMessage( &msg );
                }
            }
            else
            {
                // Render a frame during idle time (no messages are waiting)
                DXUTRender3DEnvironment();
            }
        }
        return DXUTGetExitCode();
#endif 
    }

    DWORD Direct3DUtility::DumpAdapterModes(IDirect3D9* pD3D)
    {
        const D3DFORMAT allowedAdapterFormatArray[] = 
        {   
            D3DFMT_R8G8B8 ,
            D3DFMT_A8R8G8B8 ,
            D3DFMT_X8R8G8B8 ,
            D3DFMT_R5G6B5 ,
            D3DFMT_X1R5G5B5 ,
            D3DFMT_A1R5G5B5 ,
            D3DFMT_A4R4G4B4 ,
            D3DFMT_R3G3B2 ,
            D3DFMT_A8 ,
            D3DFMT_A8R3G3B2 ,
            D3DFMT_X4R4G4B4 ,
            D3DFMT_A2B10G10R10 ,
            D3DFMT_A8B8G8R8 ,
            D3DFMT_X8B8G8R8 ,
            D3DFMT_G16R16 ,
            D3DFMT_A2R10G10B10 ,
            D3DFMT_A16B16G16R16 ,

            D3DFMT_A8P8 ,
            D3DFMT_P8 ,

            D3DFMT_L8 ,
            D3DFMT_A8L8 ,
            D3DFMT_A4L4 ,

            D3DFMT_V8U8 ,
            D3DFMT_L6V5U5 ,
            D3DFMT_X8L8V8U8 ,
            D3DFMT_Q8W8V8U8 ,
            D3DFMT_V16U16 ,
            D3DFMT_A2W10V10U10 ,

            D3DFMT_UYVY ,
            D3DFMT_R8G8_B8G8 ,
            D3DFMT_YUY2 ,
            D3DFMT_G8R8_G8B8 ,
            D3DFMT_DXT1 ,
            D3DFMT_DXT2 ,
            D3DFMT_DXT3 ,
            D3DFMT_DXT4 ,
            D3DFMT_DXT5 ,

            D3DFMT_D16_LOCKABLE ,
            D3DFMT_D32 ,
            D3DFMT_D15S1 ,
            D3DFMT_D24S8 ,
            D3DFMT_D24X8 ,
            D3DFMT_D24X4S4 ,
            D3DFMT_D16 ,

            D3DFMT_D32F_LOCKABLE ,
            D3DFMT_D24FS8 ,

            D3DFMT_L16 ,

            D3DFMT_VERTEXDATA ,
            D3DFMT_INDEX16 ,
            D3DFMT_INDEX32 ,

            D3DFMT_Q16W16V16U16 ,

            D3DFMT_MULTI2_ARGB8 ,

            // Floating point surface formats

            // s10e5 formats (16-bits per channel)
            D3DFMT_R16F ,
            D3DFMT_G16R16F ,
            D3DFMT_A16B16G16R16F ,

            // IEEE s23e8 formats (32-bits per channel)
            D3DFMT_R32F ,
            D3DFMT_G32R32F ,
            D3DFMT_A32B32G32R32F ,

            D3DFMT_CxV8U8 ,
        };

        HRESULT hr = E_FAIL;
        DWORD dwCount = 0;
        const UINT allowedAdapterFormatArrayCount  = _countof(allowedAdapterFormatArray);

        USES_CONVERSION;

        //获取适配器的个数
        UINT numAdapters = pD3D->GetAdapterCount();
        for (UINT adapterOrdinal = 0; adapterOrdinal < numAdapters; adapterOrdinal++)
        {
            D3DADAPTER_IDENTIFIER9 adapterIdentifier = {0};
            DX_VERIFY(pD3D->GetAdapterIdentifier(adapterOrdinal, 0, &adapterIdentifier));

            FTLTRACE(TEXT("Adapter Identifier[%d]:driver=%s, desc=%s, deviceName=%s\n\t")
                TEXT("version=[%d,%d], vendorId=%d, deviceid=%d,subsysid=%d, Revision=%d, WHQLLevel=%d\n"),
                adapterOrdinal,
                A2T(adapterIdentifier.Driver), A2T(adapterIdentifier.Description), A2T(adapterIdentifier.DeviceName),
                adapterIdentifier.DriverVersion.HighPart, adapterIdentifier.DriverVersion.LowPart,
                adapterIdentifier.VendorId, adapterIdentifier.DeviceId,adapterIdentifier.SubSysId,
                adapterIdentifier.Revision,adapterIdentifier.WHQLLevel);

            for( UINT iFormatList = 0; iFormatList < allowedAdapterFormatArrayCount; iFormatList++ )
            {
                D3DFORMAT allowedAdapterFormat = allowedAdapterFormatArray[iFormatList];
                UINT numAdapterModes = pD3D->GetAdapterModeCount( adapterOrdinal, allowedAdapterFormat );
                for (UINT mode = 0; mode < numAdapterModes; mode++)
                {
                    D3DDISPLAYMODE displayMode = {0};
                    DX_VERIFY( pD3D->EnumAdapterModes( adapterOrdinal, allowedAdapterFormat, mode, &displayMode ));
                    dwCount++;

                    //FTLTRACEEX(FTL::tlInfo, "")
                }
            }
        }

        FTLTRACE(TEXT("DumpAdapterModes, ModeCount = %d\n"), dwCount);
        return dwCount;
    }


	CFDShowHardwareMgr::CFDShowHardwareMgr()
	{

	}

	CFDShowHardwareMgr::~CFDShowHardwareMgr()
	{
		Clear();
	}

	HRESULT CFDShowHardwareMgr::Refresh(const CLSID* pDevClsid)
	{
		HRESULT hr = E_FAIL;
		CComPtr<ICreateDevEnum> pSysDevEnum;
		COM_VERIFY(CoCreateInstance(CLSID_SystemDeviceEnum,NULL,CLSCTX_INPROC,IID_ICreateDevEnum,(void**)&pSysDevEnum));
		if (SUCCEEDED(hr))
		{
			CComPtr<IEnumMoniker> pEnumCat;
			COM_VERIFY_EXCEPT1(pSysDevEnum->CreateClassEnumerator(*pDevClsid, &pEnumCat,0), S_FALSE); //如果类型下没有对象，则会返回S_FALSE
			if (pEnumCat)
			{
				//if create class Enumerator Success, then clear old, otherwise will remain
				Clear();

				COM_VERIFY(pEnumCat->Reset());
				CComPtr<IMoniker> spMoniker;
				ULONG ulFetched = 0;
				while (pEnumCat->Next(1, &spMoniker, &ulFetched) == S_OK)
				{
					COM_VERIFY(_AddNewMoniker(spMoniker));
					spMoniker.Release();
				}

				//If CreateClassEnumerator Success then result is Success, even there are no Item
				hr = S_OK;
			}
		}
		return hr;
	}

	HRESULT CFDShowHardwareMgr::Clear()
	{
		std::for_each(m_Hardwares.begin(), m_Hardwares.end(), FTL::ObjecteDeleter<HardwareMonikerInfo*>());
		m_Hardwares.clear();
		return S_OK;
	}

	HRESULT CFDShowHardwareMgr::_AddNewMoniker(IMoniker* pMoniker)
	{
		CHECK_POINTER_RETURN_VALUE_IF_FAIL(pMoniker, E_POINTER);

		HRESULT hr = E_FAIL;
		CComPtr<IPropertyBag> spPropertyBag;
		COM_VERIFY(pMoniker->BindToStorage(NULL, NULL, IID_IPropertyBag, (void **)&spPropertyBag));
		if (spPropertyBag)
		{
			CComVariant varFriendlyName;
			COM_VERIFY(spPropertyBag->Read(L"FriendlyName", &varFriendlyName, NULL));
			if (S_OK == hr)
			{
				HardwareMonikerInfo* pHardwareMonikerInfo = new HardwareMonikerInfo();
				//pHardwareMonikerInfo->strFriendlyName = var.bstrVal;
				StringCchCopyW(pHardwareMonikerInfo->wachFriendlyName, _countof(pHardwareMonikerInfo->wachFriendlyName), 
					varFriendlyName.bstrVal);
				
				CComVariant varClsid;
				HRESULT hrTemp = spPropertyBag->Read(L"CLSID", &varClsid, NULL);
				if(SUCCEEDED(hrTemp))
				{
					CLSIDFromString(varClsid.bstrVal, &pHardwareMonikerInfo->clsid);
					//pHardwareMonikerInfo->clsid = varClsid.clsid;
				}
				pHardwareMonikerInfo->pMoniker = pMoniker;
				pMoniker->AddRef();

				//m_Hardwares[pHardwareMonikerInfo->strFriendlyName] = pHardwareMonikerInfo;
				m_Hardwares.push_back(pHardwareMonikerInfo);
			}
		}
		return hr;
	}

	HRESULT CFDShowHardwareMgr::GetBindObject(LPCWSTR pszName, REFIID riidResult, void **ppvResult)
	{
		HRESULT hr = E_FAIL;
		HardwareMonikerInfoContainerIter iter = m_Hardwares.begin();
		for ( ;	iter != m_Hardwares.end(); ++iter)
		{
			if (lstrcmpW(pszName, (*iter)->wachFriendlyName) == 0)
			{
				//Found
				break;
			}
			else
			{
				//Because IPropertyBag::Read(L"FriendlyName") limit 32 TCHAR, so just check part of the string
				if (lstrlenW((*iter)->wachFriendlyName) >= DS_FRIENDLY_NAME_MAX_LENGTH - 1
					&& NULL != wcsstr(pszName, (*iter)->wachFriendlyName))
				{
					//Found
					break;
				}
			}
		}
		if (iter != m_Hardwares.end())
		{
			//Found
			IMoniker* pMoniker = (*iter)->pMoniker;
			COM_VERIFY(pMoniker->BindToObject(NULL, NULL, riidResult, (void**)ppvResult));
		}
		return hr;
	}

//#endif //USE_DIRECTX_9
}

#endif //FTL_DIRECTX_HPP