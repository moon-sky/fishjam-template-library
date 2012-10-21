#include "stdafx.h"
#include "ScreenCaptureSourcePin.h"
#include "ScreenCapture_h.h"
#include "GdiScreenCaptureImpl.h"
#include "EventParamDefine.h"

CScreenCaptureSourcePin::CScreenCaptureSourcePin(HRESULT *phr, CSource *pFilter, HANDLE hEventContinue, HANDLE hEventStop)
	:CSourceStream(NAME("Fishjam Screen Capture Pin"), phr, pFilter, L"Out")
	, m_hEventContinue(hEventContinue)
	, m_hEventStop(hEventStop)
	//, m_FrameWritten(0)
	//, m_bZeroMemory(FALSE)
	, m_nFrameNumber(0)
	//, m_iImageWidth(320)
	//, m_iImageHeight(240)
	//, m_nAvgTimePerFrame(FPS_25)
	//, m_rtFrameLength(FPS_25)
	//, m_nCurrentBitDepth(32)
	,m_dwAdviseToken(0)
{
	ATLTRACE(TEXT("CScreenCaptureSourcePin::CScreenCaptureSourcePin, this=0x%x, CurThreadId=%d\n"), this, GetCurrentThreadId());
	CAutoLock cAutoLock(&m_cSharedState);
	//m_pBlockElapse = new FTL::CFBlockElapse(TEXT(__FILE__), __LINE__, TEXT(__FUNCDNAME__), FTL::_ReturnAddress());
	//CAutoLock cAutoLock(&m_cSharedState);
	    
	m_bMouseOverlay = TRUE;
	m_nWidth = DEFAULT_WIDTH;
	m_nHeight = DEFAULT_HEIGTH;
	m_nMaxWidth = ::GetSystemMetrics(SM_CXVIRTUALSCREEN);
	m_nMaxHeight = ::GetSystemMetrics(SM_CYVIRTUALSCREEN);
	m_rcCapture.left = m_rcCapture.top = 0;
	m_rcCapture.right = m_rcCapture.left + m_nWidth;
	m_rcCapture.bottom = m_rcCapture.top + m_nHeight;

	m_nBitCount = DEFAULT_BIT_COUNT;
	m_nAvgTimePerFrame = UNITS / DEFAULT_FPS;
	ZeroMemory(&m_bmpInfo, sizeof(m_bmpInfo));
	m_bmpInfo.bmiHeader.biSize = sizeof(m_bmpInfo.bmiHeader);

	m_rtStart = 0;
	m_rtStreamOffset = 0;
	m_rtStartAt = -1;
	m_rtStopAt = -1;
	//m_iImageWidth = GetSystemMetrics(SM_CXSCREEN);
	//m_iImageHeight = GetSystemMetrics(SM_CYSCREEN);
	m_dwStartCookie = 0;
	m_dwStopCookie = 0;
	m_bShouldFlush = FALSE;
	m_bStartNotified = FALSE;
	m_bStopNotified = FALSE;
	m_pProperties = NULL;
	//m_pClock = NULL;
	m_hSemaphore = NULL;
	m_rfMaxRecordTime = 0;

	m_mt.majortype = GUID_NULL;
	m_pScreenCaptureImpl = NULL;

	m_rtClockStart = 0;
	m_rtClockStop = 0;
	m_bFirstFrame = TRUE;
	GetMediaType(0, &m_mt);
	m_hEventAfterFilterRun = ::CreateEvent(NULL, TRUE, FALSE, NULL);
}

CScreenCaptureSourcePin::~CScreenCaptureSourcePin()
{
	FTLASSERT(NULL == m_hSemaphore);
	if (m_hEventAfterFilterRun)
	{
		CloseHandle(m_hEventAfterFilterRun);
		m_hEventAfterFilterRun = NULL;
	}
	ATLTRACE(TEXT("CScreenCaptureSourcePin::~CScreenCaptureSourcePin, this=0x%x, CurThreadId=%d \n"), this, GetCurrentThreadId());
}

HRESULT CScreenCaptureSourcePin::Set(REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData,
DWORD cbInstanceData,LPVOID pPropData, DWORD cbPropData)
{
	return E_NOTIMPL;
}

HRESULT CScreenCaptureSourcePin::Get(REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData,
													DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData, DWORD *pcbReturned)
{
	*pcbReturned = sizeof(GUID);
	if (guidPropSet !=  AMPROPSETID_Pin)
	{
		return E_PROP_SET_UNSUPPORTED;
	}
	if (dwPropID != AMPROPERTY_PIN_CATEGORY)
	{
		return E_PROP_ID_UNSUPPORTED;
	}
	if (pPropData == NULL)
	{
		return NOERROR;
	}
	if (cbPropData < sizeof(GUID))
	{
		return E_UNEXPECTED;
	}
	//*pPropData = PIN_CATEGORY_CAPTURE;
	CopyMemory(pPropData, &PIN_CATEGORY_CAPTURE, sizeof(GUID));
	//Marshal.StructureToPtr(PinCategory.Capture, pPropData, false);
	return NOERROR;
}

HRESULT CScreenCaptureSourcePin::QuerySupported(REFGUID guidPropSet, DWORD dwPropID, DWORD *pTypeSupport)
{
	TCHAR szGUID[128] = {0};
	StringFromGUID2(guidPropSet, szGUID, _countof(szGUID));
	FTLTRACE(TEXT("QuerySupported, guidPropSet=%s, dwPropId=%d\n"), szGUID, dwPropID);

	*pTypeSupport = KSPROPERTY_SUPPORT_GET;// KSPropertySupport.Get;
	if (guidPropSet != AMPROPSETID_Pin) // PropSetID.Pin
	{
		return E_PROP_SET_UNSUPPORTED;
	}
	if (dwPropID != AMPROPERTY_PIN_CATEGORY)// (int)AMPropertyPin.Category)
	{
		return E_PROP_ID_UNSUPPORTED;
	}

	return E_NOTIMPL;
}

HRESULT CScreenCaptureSourcePin::GetLatency(REFERENCE_TIME* pOutLatency)
{
	CHECK_POINTER_RETURN_VALUE_IF_FAIL(pOutLatency, E_POINTER);
	HRESULT hr = E_FAIL;

	*pOutLatency = UNITS / DEFAULT_FPS;
	if (m_mt.majortype == MEDIATYPE_Video)
	{
		VIDEOINFO *pvi = (VIDEOINFO *) m_mt.Format();
		*pOutLatency = pvi->AvgTimePerFrame;
	}
	return S_OK;

}

HRESULT CScreenCaptureSourcePin::FillBuffer(IMediaSample *pSample)
{
	FTL::FTLThreadWaitType waitType = _GetWaitType(INFINITE);
	switch (waitType)
	{
	case FTL::ftwtStop:
		return S_FALSE;	//quit
	case FTL::ftwtError:
		return E_UNEXPECTED;
	//case FTL::ftwtContinue:
	//case FTL::ftwtTimeOut:
	default:
		//just continue
		break;
	}

#if 0
	//FUNCTION_BLOCK_TRACE(1);
	CheckPointer(pSample, E_POINTER);
	ASSERT(m_mt.formattype == FORMAT_VideoInfo);

	m_nFrameNumber++;

	//make the samples scheduling
	HRESULT hr = S_OK;
	REFERENCE_TIME	rtLatency = 0;
	if (FAILED(GetLatency(&rtLatency)))
	{
		rtLatency = UNITS / DEFAULT_FPS ;
	}
	REFERENCE_TIME rtStart, rtStop;
	BOOL bShouldDeliver = FALSE;
	do 
	{
		if (m_dwAdviseToken == 0)
		{
			DX_VERIFY(m_pClock->GetTime(&m_rtClockStart));
			//fixed frame rate, so can use AdvisePeriodic
			DX_VERIFY(m_pClock->AdvisePeriodic(m_rtClockStart + rtLatency, 
				rtLatency, (HSEMAPHORE)m_hSemaphore, &m_dwAdviseToken));
		}
		else
		{
			DWORD dwResult = WaitForSingleObject(m_hSemaphore, INFINITE);
		}

		bShouldDeliver = TRUE;
		rtStart = m_rtStart;
		rtStop = m_rtStart + 1;
		DX_VERIFY(pSample->SetTime(&rtStart, &rtStop));
		FTLASSERT(m_pScreenCaptureImpl);
		if (m_pScreenCaptureImpl)
		{
			LPBYTE pBuffer = NULL;
			DX_VERIFY(pSample->GetPointer(&pBuffer));
			VIDEOINFOHEADER *pVih = (VIDEOINFOHEADER*)m_mt.pbFormat;
			//int nSize = min(pVih->bmiHeader.biSizeImage, (DWORD)cbData);
			HBITMAP hDIB = m_pScreenCaptureImpl->CopyScreenToBitmap(&m_rcCapture, pBuffer, (BITMAPINFO *) &(pVih->bmiHeader));
			DeleteObject(hDIB);
		}

		DX_VERIFY(m_pClock->GetTime(&m_rtClockStop));
		DX_VERIFY(pSample->GetTime(&rtStart, &rtStop));

		if (rtLatency > 0 && rtLatency * 3 < m_rtClockStop - m_rtClockStart)
		{
			//Why?
			m_rtClockStop = m_rtClockStart + rtLatency;
		}
		rtStop = rtStart + (m_rtClockStop - m_rtClockStart);
		m_rtStart = rtStop;
		//lock (m_csPinLock)
		{
			rtStart -= m_rtStreamOffset;
			rtStop -= m_rtStreamOffset;
		}

		DX_VERIFY(pSample->SetMediaTime(&m_nFrameNumber, &m_nFrameNumber));
		DX_VERIFY(pSample->SetTime(&rtStart, &rtStop));
		m_rtClockStart = m_rtClockStop;

		bShouldDeliver = ((rtStart >= 0) && (rtStop >= 0));
		if (bShouldDeliver)
		{
			//lock (m_csPinLock)
			if (m_rtStartAt != -1)
			{
				if (m_rtStartAt > rtStart)
				{
					bShouldDeliver = FALSE;
				}
				else
				{
					if (m_dwStartCookie != 0 && !m_bStartNotified)
					{
						m_bStartNotified = TRUE;
						DX_VERIFY(m_pFilter->NotifyEvent(EC_STREAM_CONTROL_STARTED, (LONG_PTR)this, m_dwStartCookie));
						if (FAILED(hr)) 
						{
							return hr;
						}
					}
				}
			}
			if (!bShouldDeliver)
			{
				//Why?
				continue;
			}
			if (m_rtStopAt != -1)
			{
				if (m_rtStopAt < rtStart)
				{
					if (!m_bStopNotified)
					{
						m_bStopNotified = TRUE;
						if (m_dwStopCookie != 0)
						{
							DX_VERIFY(m_pFilter->NotifyEvent(EC_STREAM_CONTROL_STOPPED, (LONG_PTR)this, m_dwStopCookie));
							if (FAILED(hr))
							{
								return hr;
							}
						}
						bShouldDeliver = m_bShouldFlush;
					}
					else
					{
						bShouldDeliver = FALSE;
					}
					// EOS -- EndOfStream
					if (!bShouldDeliver)
					{
						return S_FALSE;
					}
				}
			}
		}

	} while (!bShouldDeliver);
	return hr;

	//DX_VERIFY(m_pFilter->StreamTime(rtStart));

	//LONGLONG llStartTime = m_ElapseCounter.GetElapseTime();
	//REFERENCE_TIME rtStreamTime = m_rtSampleTime;// llStartTime / 100; // rfStreamTime.GetUnits();
	
//loop:   
	//REFERENCE_TIME rtStart = rtStreamTime; //m_iFrameNumber * m_rtFrameLength;    
	//REFERENCE_TIME rtStop  = rtStart + m_rtFrameLength;
	
	//if (rtStreamTime > rtStop)
	//{
	//	OutputDebugString(L"lost capture \r\n");
	//	++m_iFrameNumber;
	//	goto loop;
	//}
	//while (rtStreamTime < rtStart)
	//{
	//	m_pFilter->StreamTime(rfStreamTime);
	//	rtStreamTime = rfStreamTime.GetUnits();
	//	// REFERENCE_TIME rtWaitTime = rtStart - rtStreamTime;
	//	// ::WaitForSingleObject(m_hWaitEvent, rtWaitTime/10000);
	//}

	BYTE *pData = NULL;
	long cbData = 0;

	{
		DX_VERIFY(pSample->GetPointer(&pData));
		cbData = pSample->GetSize();
		//if (m_bZeroMemory)
		//{
		//	ZeroMemory(pData, cbData);
		//}
		{
			CAutoLock cAutoLockShared(&m_cSharedState);

			ASSERT(m_mt.formattype == FORMAT_VideoInfo);
			VIDEOINFOHEADER *pVih = (VIDEOINFOHEADER*)m_mt.pbFormat;

			int nSize = min(pVih->bmiHeader.biSizeImage, (DWORD)cbData);

			//*
			HBITMAP hDib = m_pScreenCaptureImpl->CopyScreenToBitmap(&m_rcCapture, pData, (BITMAPINFO *) &(pVih->bmiHeader));
			if (hDib)
			{
				DeleteObject(hDib);
			}
			//CRefTime rtStart = rfStreamTime; //m_rtSampleTime;
			//m_rtSampleTime +=   (LONG) m_iRepeatTime;
			CRefTime rtStop;// = m_ElapseCounter.GetElapseTime() / 100;
			
			DX_VERIFY(m_pFilter->StreamTime(rtStop));

			//m_rtSampleTime = rtStop;

			ATLTRACE(TEXT("CScreenCaptureSourcePin::FillBuffer , start=%lld(%f ms), stop=%lld(%f ms)\n"),
				rtStart, float(rtStart) / 10000,  rtStop, float(rtStop) / 10000);

			DX_VERIFY(pSample->SetTime((REFERENCE_TIME *)&rtStart, (REFERENCE_TIME *)&rtStop));

			//每一帧都是一个同步点
			DX_VERIFY(pSample->SetSyncPoint(TRUE));


			BOOL bWait = FALSE;
			DWORD dwWillWaitTime = 0;
			//LONGLONG llElapseTime = rtStop.GetUnits() - rtStart.GetUnits();
			//	//m_ElapseCounter.GetElapseTime() -  llStartTime;
			//if ( llElapseTime < MILLISECONDS_TO_100NS_UNITS(m_iRepeatTime))
			//{
			//	bWait = TRUE;
			//	dwWillWaitTime = (MILLISECONDS_TO_100NS_UNITS(m_iRepeatTime) - llElapseTime) / 10000;
			//	if (dwWillWaitTime > 1)
			//	{
			//		//WaitForSingleObject(m_hStopEvent, dwWillWaitTime );
			//	}
			//}

		}
	}

	//FTLTRACE(TEXT("llElapseTime = %lld, bWait=%d, dwWillWaitTime=%d\n"), llElapseTime, bWait, dwWillWaitTime);
#endif

	CheckPointer(pSample, E_POINTER);

	HRESULT hr = E_FAIL;

	CRefTime rfStreamTime;
	{
		//CAutoLock cObjectLock(m_pLock);
		DX_VERIFY(m_pFilter->StreamTime(rfStreamTime));
	}
	REFERENCE_TIME rtStreamTime = rfStreamTime.GetUnits();
	if (m_rfMaxRecordTime != 0 && rtStreamTime > m_rfMaxRecordTime)
	{
		//max time over

		//if there is preview window, just return S_FALSE is OK
		//if there is NOT preview window, can not stop graph automatic
		m_pFilter->NotifyEvent(TIME_OVER, static_cast<LONG_PTR>(m_rfMaxRecordTime / (UNITS / MILLISECONDS)), 0);
		return S_FALSE;
	}
	REFERENCE_TIME rtStart = 0; 
	REFERENCE_TIME rtStop = 0;

	do 
	{
		rtStart = m_nFrameNumber * m_nAvgTimePerFrame;
		rtStop = rtStart + m_nAvgTimePerFrame;
		if( rtStreamTime > rtStop)
		{
			OutputDebugString(L"lost capture \r\n");
			++m_nFrameNumber;
		}
	} while (rtStreamTime > rtStop);

	while (rtStreamTime < rtStart)
	{
		m_pFilter->StreamTime(rfStreamTime);
		rtStreamTime = rfStreamTime.GetUnits();
		// REFERENCE_TIME rtWaitTime = rtStart - rtStreamTime;
		// ::WaitForSingleObject(m_hWaitEvent, rtWaitTime/10000);
	}

	BYTE *pData = NULL;
	long cbData = 0;
	CAutoLock cAutoLockShared(&m_cSharedState);
	DX_VERIFY(pSample->GetPointer(&pData));
	cbData = pSample->GetSize();

	ASSERT(m_mt.formattype == FORMAT_VideoInfo);
	VIDEOINFOHEADER *pVih = (VIDEOINFOHEADER*)m_mt.pbFormat;


	int nSize = min(pVih->bmiHeader.biSizeImage, (DWORD)cbData);
	HBITMAP hDib = m_pScreenCaptureImpl->CopyScreenToBitmap(&m_rcCapture, pData, (BITMAPINFO *) &(pVih->bmiHeader));    

	if (hDib)
	{
		if (m_bFirstFrame)
		{
			m_bFirstFrame = FALSE;
			DX_VERIFY(m_pFilter->NotifyEvent(FIRST_FRAME, (LONG_PTR)(hDib), NULL));
		}
		else
		{
			DeleteObject(hDib);
		}
	}  

	//REFERENCE_TIME rtTemp;
	//IReferenceClock *pClock;
	//m_pFilter->GetSyncSource(&pClock);
	//pClock->GetTime(&rtTemp);
	//pClock->Release();    

	//ST_FRAME_TIME *pTmp = new ST_FRAME_TIME();
	//pTmp->llStartTime = rtStart;
	//pTmp->llStopTime = rtStop;
	//pTmp->nFrameIndex = m_iFrameNumber;    
	//m_pFilter->NotifyEvent(FRAME_TIME, (LONG_PTR)pTmp, NULL);

	DX_VERIFY(pSample->SetTime(&rtStart, &rtStop));
	m_nFrameNumber++;

	DX_VERIFY(pSample->SetSyncPoint(TRUE));

	return S_OK;

	return hr;
}

HRESULT CScreenCaptureSourcePin::DecideBufferSize(
								 IMemAllocator * pAlloc,
								 __inout ALLOCATOR_PROPERTIES * ppropInputRequest
								 )
{
	//FUNCTION_BLOCK_TRACE(1);

	CheckPointer(pAlloc, E_POINTER);
	CheckPointer(ppropInputRequest, E_POINTER);

	HRESULT hr = S_OK;
    //对 Filter 加锁
	{
	CAutoLock cAutoLock(m_pFilter->pStateLock());

	VIDEOINFO *pvi = (VIDEOINFO *) m_mt.Format();
	ppropInputRequest->cBuffers = 1;
	ppropInputRequest->cbBuffer = pvi->bmiHeader.biSizeImage;
    ASSERT(ppropInputRequest->cbBuffer > 0);

	ALLOCATOR_PROPERTIES Actual = {0};
    // Ask the allocator to reserve us some sample memory,
	hr = pAlloc->SetProperties(ppropInputRequest, &Actual);
	if (FAILED(hr))
	{
		return hr;
	}

    //检查是否真正的分配了Sample所需的内存
	if (Actual.cbBuffer < ppropInputRequest->cbBuffer)
	{
			FTLASSERT(FALSE);
		return E_FAIL;
	}
    ASSERT(ppropInputRequest->cBuffers == Actual.cBuffers);
	ASSERT(Actual.cBuffers == 1);
	}
	return hr;
}

HRESULT CScreenCaptureSourcePin::GetDefaultCaps(int nIndex, VIDEO_STREAM_CONFIG_CAPS* pCaps)
{
	pCaps->guid = FORMAT_VIDEOINFO2;
	pCaps->VideoStandard = AnalogVideo_None; //AnalogVideoStandard::
	pCaps->InputSize.cx = DEFAULT_WIDTH;
	pCaps->InputSize.cy = DEFAULT_HEIGTH;
	pCaps->MinCroppingSize.cx = 1;
	pCaps->MinCroppingSize.cy = 1;
	pCaps->MaxCroppingSize.cx = m_nMaxWidth;
	pCaps->MaxCroppingSize.cy = m_nMaxHeight;
	pCaps->CropGranularityX = 160;
	pCaps->CropGranularityY = 120;
	pCaps->CropAlignX = 0;
	pCaps->CropAlignY = 0;
	pCaps->MinOutputSize = pCaps->MinCroppingSize;
	pCaps->MaxOutputSize = pCaps->MaxCroppingSize;
	pCaps->OutputGranularityX = pCaps->CropGranularityX;
	pCaps->OutputGranularityY = pCaps->CropGranularityY;

	pCaps->StretchTapsX = 0;
	pCaps->StretchTapsY = 0;
	pCaps->ShrinkTapsX = 0;
	pCaps->ShrinkTapsY = 0;
	pCaps->MinFrameInterval = UNITS / MAX_FPS;
	pCaps->MaxFrameInterval = UNITS / MIN_FPS;
	pCaps->MinBitsPerSecond = pCaps->MinOutputSize.cx * pCaps->MinOutputSize.cy * m_nBitCount * MIN_FPS;
	pCaps->MaxBitsPerSecond = pCaps->MaxOutputSize.cx * pCaps->MaxOutputSize.cy * m_nBitCount * MAX_FPS;
	return S_OK;
}

// Set the agree media type set up the necessary parameters
HRESULT CScreenCaptureSourcePin::SetMediaType(const CMediaType *pMediaType)
{
	CHECK_POINTER_RETURN_VALUE_IF_FAIL(pMediaType, E_POINTER);
	if (NULL == pMediaType->Format())
	{
		return VFW_E_INVALIDMEDIATYPE;
	}
	HRESULT hr = E_FAIL;
	//CAutoLock cAutoLock(m_pFilter->pStateLock());

#ifdef _DEBUG
	DX_VERIFY(CheckMediaType(pMediaType));
	if (FAILED(hr))
	{
		return hr;
	}
#endif 

	{
		CAutoLock cAutoLock(m_pFilter->pStateLock());
	COM_VERIFY(__super::SetMediaType(pMediaType));
	if (SUCCEEDED(hr))
	{
		VIDEOINFO *pvi = (VIDEOINFO*)m_mt.Format();

		//ASSERT(m_iImageWidth == pvi->bmiHeader.biWidth);
        //ASSERT(m_iImageHeight == abs(pvi->bmiHeader.biHeight));
        //ASSERT(m_nCurrentBitDepth == pvi->bmiHeader.biBitCount);

		//CAutoLock cAutoLock(&m_cStateLock);
		//if (m_pScreenCaptureImpl)
		//{
		//	SAFE_DELETE(m_pScreenCaptureImpl);
		//}
			FTLASSERT(pvi->bmiHeader.biHeight > 0);
		FTLASSERT(pvi->bmiHeader.biWidth > 0);
		m_nWidth = pvi->bmiHeader.biWidth;
			m_nHeight = FTL_ABS(pvi->bmiHeader.biHeight);
		m_nBitCount = pvi->bmiHeader.biBitCount;

			m_nAvgTimePerFrame = pvi->AvgTimePerFrame;
			m_bmpInfo.bmiHeader.biWidth = pvi->bmiHeader.biWidth;
			m_bmpInfo.bmiHeader.biHeight = pvi->bmiHeader.biHeight;
		m_bmpInfo.bmiHeader.biBitCount = m_nBitCount;
		m_bmpInfo.bmiHeader.biCompression = BI_RGB;
		m_bmpInfo.bmiHeader.biPlanes = 1;
		m_bmpInfo.bmiHeader.biSizeImage = GetBitmapSize(&pvi->bmiHeader);
		}
	}
	return NOERROR;
}


HRESULT CScreenCaptureSourcePin::CheckMediaType(const CMediaType* pMediaType)
{
	CHECK_POINTER_RETURN_VALUE_IF_FAIL(pMediaType, E_POINTER);
	CHECK_POINTER_RETURN_VALUE_IF_FAIL(pMediaType->Format(), VFW_E_INVALIDMEDIATYPE);

	HRESULT hr = E_FAIL;
	if ((pMediaType->majortype != MEDIATYPE_Video)    //only output video
		|| !(pMediaType->IsFixedSize()))              //in fixed size samples
	{
		return VFW_E_INVALIDMEDIATYPE;
	}

	// Check for the subtypes we support
	if (pMediaType->subtype != MEDIASUBTYPE_RGB32
		&& pMediaType->subtype != MEDIASUBTYPE_RGB24
		//&& pMediaType->subtype != MEDIASUBTYPE_ARGB32
		)
	{
		return VFW_E_INVALIDMEDIATYPE;
	}

	// Get the format area of the media type
	VIDEOINFO *pvi = (VIDEOINFO*)pMediaType->Format();
	FTLASSERT(pvi); // && pvi->bmiHeader.biCompression == BI_RGB
	if (pvi->bmiHeader.biCompression != BI_RGB)
	{
		return VFW_E_TYPE_NOT_ACCEPTED;
	}
	if (pvi->bmiHeader.biBitCount != 24 && pvi->bmiHeader.biBitCount != 32)
	{
		return VFW_E_TYPE_NOT_ACCEPTED;
	}
	if (pvi->bmiHeader.biWidth != m_nWidth || abs(pvi->bmiHeader.biHeight) != m_nHeight)
	{
		return VFW_E_TYPE_NOT_ACCEPTED;
	}

	if (pvi->bmiHeader.biHeight < 0)
	{
		//FTLASSERT(FALSE); //TODO: need this ?
		return VFW_E_TYPE_NOT_ACCEPTED;
	}
#if 0
	VIDEO_STREAM_CONFIG_CAPS videoStreamCaps = {0};
	DX_VERIFY(GetDefaultCaps(0, &videoStreamCaps));
	if (pvi->bmiHeader.biWidth < videoStreamCaps.MinOutputSize.cx
		|| pvi->bmiHeader.biWidth > videoStreamCaps.MaxOutputSize.cx)
	{
		return VFW_E_INVALIDMEDIATYPE;
	}
	long nRate = pvi->AvgTimePerFrame;
	if (pvi->AvgTimePerFrame < videoStreamCaps.MinFrameInterval ||
		pvi->AvgTimePerFrame > videoStreamCaps.MaxFrameInterval)
	{
		return VFW_E_INVALIDMEDIATYPE;
	}
	REFERENCE_TIME rate = pvi->AvgTimePerFrame;
	if (rate < videoStreamCaps.MinFrameInterval || rate > videoStreamCaps.MaxFrameInterval)
	{
		return VFW_E_INVALIDMEDIATYPE;
	}
#endif 
	//// Check if the image width & height have changed
	//if (pvi->bmiHeader.biWidth != m_iImageWidth || abs(pvi->bmiHeader.biHeight) != m_iImageHeight)
	//{
	//	//因为之前在 GetMediaType 中返回了大小，理论上说不会到这里

	//	// If the image width/height is changed, fail CheckMediaType() to force
	//	// the renderer to resize the image.
	//	ASSERT(FALSE);
	//	return E_INVALIDARG;
	//}

	//if (pvi->bmiHeader.biHeight < 0)
	//{
	//	return E_INVALIDARG;
	//}

	return S_OK;    // This format is acceptable.
}

//返回这个Pin支持的媒体类型，框架会从iPosition为0递归调用进行检查，每次加1，直到返回错误(VFW_S_NO_MORE_ITEMS)
//本程序中支持以下几种媒体类型
//  0 -- 32bit mediatype
//  1 -- 24bit mediatype
HRESULT CScreenCaptureSourcePin::GetMediaType(int iPosition, CMediaType *pmt)
{
	//FUNCTION_BLOCK_TRACE(1);
	CheckPointer(pmt, E_POINTER);
	if (iPosition < 0)
	{
		return E_INVALIDARG;
	}
	if (iPosition >= 2)
	{
		return VFW_S_NO_MORE_ITEMS;
	}

	CAutoLock(m_pFilter->pStateLock());
	//VIDEO_STREAM_CONFIG_CAPS	videoCaps = {0};
	//GetDefaultCaps(0, &videoCaps);

	//hr = m_pInput->ConnectionMediaType(pmt);  //TransformFilter 时
	//VIDEOINFO *pOldVideInfo = (VIDEOINFO *)pmt->Format();
	VIDEOINFO *pvi = (VIDEOINFO*) pmt->AllocFormatBuffer(sizeof(VIDEOINFO));
	if (NULL == pvi)
	{
		return (E_OUTOFMEMORY);
	}
	ZeroMemory(pvi, sizeof(VIDEOINFO));

	switch(iPosition)
	{
	case 0:
		{
			// Return our highest quality 32bit format
			// Since we use RGB888 (the default for 32 bit), there is
			// no reason to use BI_BITFIELDS to specify the RGB
			// masks. Also, not everything supports BI_BITFIELDS
            //SetPaletteEntries(Yellow);
			pvi->bmiHeader.biCompression = BI_RGB;
			pvi->bmiHeader.biBitCount    = 32;
			break;
		}
	case 1:
		{
			// Return our 24bit format
			pvi->bmiHeader.biCompression = BI_RGB;
			pvi->bmiHeader.biBitCount    = 24;
			break;
		}
	default:
		FTLASSERT(FALSE);
		break;
	}

	pvi->bmiHeader.biSize       = sizeof(BITMAPINFOHEADER);
	pvi->bmiHeader.biWidth      = m_nWidth;
	pvi->bmiHeader.biHeight     = m_nHeight;
	pvi->bmiHeader.biPlanes     = 1;
	pvi->bmiHeader.biSizeImage  = GetBitmapSize(&pvi->bmiHeader);
	pvi->bmiHeader.biClrImportant = 0;
	
	pvi->AvgTimePerFrame = m_nAvgTimePerFrame;//

	//pvi->rcSource = m_rcCapture;
	SetRectEmpty(&(pvi->rcSource)); // we want the whole image area rendered.
	SetRectEmpty(&(pvi->rcTarget)); // no particular destination rectangle

	pmt->SetType(&MEDIATYPE_Video);
	pmt->SetFormatType(&FORMAT_VideoInfo);
	pmt->SetTemporalCompression(FALSE);
	// Work out the GUID for the subtype from the header info.
	const GUID SubTypeGUID = GetBitmapSubtype(&pvi->bmiHeader);
	pmt->SetSubtype(&SubTypeGUID);
	pmt->SetSampleSize(pvi->bmiHeader.biSizeImage);
	return NOERROR;
}

HRESULT CScreenCaptureSourcePin::Active(void)    // Starts up the worker thread
{
	HRESULT hr = E_FAIL;
	m_rtStart = 0;
	m_bStartNotified = false;
	m_bStopNotified = false;
	{
		CAutoLock lock(m_pFilter->pStateLock()); // Get's the filter clock
		//DX_VERIFY(m_pFilter->GetSyncSource(&m_pClock)); //如果调用了 SetSyncSource，则可能为NULL
		m_hSemaphore = CreateSemaphore(NULL, 0, 0x7FFFFFFF, NULL);
	}
	hr = __super::Active();
	//::SetEvent(m_hEventAfterFilterRun);
	return hr;
}

// Pin become inactive (usually due Stop calls)
HRESULT CScreenCaptureSourcePin::Inactive(void)  // Exits the worker thread.
{
	HRESULT hr = __super::Inactive();
	//if (m_pClock) // we have clock
	{
		if (m_dwAdviseToken != 0) // do we advice
		{
			CComPtr<IReferenceClock> pClock;
			m_pFilter->GetSyncSource(&pClock);
			if (pClock)
			{
				pClock->Unadvise(m_dwAdviseToken); // shutdown advice
			}
			m_dwAdviseToken = 0;
		}
		//m_pClock.Release(); // release instance
		if (m_hSemaphore != NULL)
		{
			CloseHandle(m_hSemaphore);
			m_hSemaphore = NULL;
		}
	}
	return hr;
}

HRESULT CScreenCaptureSourcePin::Run(REFERENCE_TIME tStart)
{
	HRESULT hr = CBasePin::Run(tStart);
	::SetEvent(m_hEventAfterFilterRun);
	return hr;
}

// Alter the repeat rate according to quality management messages sent from
// the downstream filter (often the renderer).  Wind it up or down according
// to the flooding level - also skip forward if we are notified of Late-ness
//
STDMETHODIMP CScreenCaptureSourcePin::Notify(IBaseFilter * pSender, Quality q)
{
	//LPWSTR pszVendorInfo = NULL;
	//pSender->QueryVendorInfo(&pszVendorInfo);
	FTLTRACE(TEXT("CScreenCaptureSourcePin::Notify, Sender=%s, Proportion = %lld, Late=%lld\n"), 
		/*pszVendorInfo ? pszVendorInfo :*/ TEXT("Sender"),
		q.Proportion, q.Late);

	//if (pszVendorInfo)
	//{
	//	CoTaskMemFree(pszVendorInfo);
	//}
 //   // Adjust the repeat rate.
 //   if(q.Proportion<=0)
 //   {
 //       m_iRepeatTime = 1000;        // We don't go slower than 1 per second
 //   }
 //   else
 //   {
 //       m_iRepeatTime = m_iRepeatTime*1000 / q.Proportion;
 //       if(m_iRepeatTime>1000)
 //       {
 //           m_iRepeatTime = 1000;    // We don't go slower than 1 per second
 //       }
 //       else if(m_iRepeatTime<10)
 //       {
 //           m_iRepeatTime = 10;      // We don't go faster than 100/sec
 //       }
 //   }

 //   // skip forwards
 //   if(q.Late > 0)
 //   {
 //       m_rtSampleTime += q.Late;
 //   }

	return NOERROR;

} // Notify

HRESULT CScreenCaptureSourcePin::OnThreadCreate(void)
{
	CAutoLock cAutoLockShared(&m_cSharedState);

	FTLASSERT(NULL == m_pScreenCaptureImpl);
	m_pScreenCaptureImpl = new CGdiScreenCaptureImpl();
	if (!m_pScreenCaptureImpl)
	{
		return E_OUTOFMEMORY;
	}
	m_pScreenCaptureImpl->SetMouseOverLay(m_bMouseOverlay);

    //m_rtSampleTime = 0;
	//m_iRepeatTime = m_rtFrameLength / 10000;  //change from 100ns to ms
	m_nFrameNumber = 0;
    //// we need to also reset the repeat time in case the system
    //// clock is turned off after m_iRepeatTime gets very big
    //m_iRepeatTime = m_iDefaultRepeatTime;
	//ATLTRACE(TEXT("CScreenCaptureSourcePin::OnThreadCreate, m_iRepeatTime=%d\n"), m_iRepeatTime);

	//m_pThreadBlockElapse = new FTL::CFBlockElapse(TEXT(__FILE__), __LINE__, TEXT(__FUNCDNAME__), FTL::_ReturnAddress());

	//HRESULT hr = E_FAIL;
	//hr = __super::OnThreadCreate();
	//return hr;

    return S_OK;
}
HRESULT CScreenCaptureSourcePin::OnThreadDestroy(void)
{
	HRESULT hr = E_FAIL;
	//SAFE_DELETE(m_pThreadBlockElapse);
	//m_ElapseCounter.Stop();

	SAFE_DELETE(m_pScreenCaptureImpl);
	hr = __super::OnThreadDestroy();
    return hr;
}
HRESULT CScreenCaptureSourcePin::OnThreadStartPlay(void)
{
    //FUNCTION_BLOCK_TRACE(1);
	BOOL bRet = FALSE;
	//API_VERIFY(m_ElapseCounter.Start());
	::WaitForSingleObject(m_hEventAfterFilterRun, INFINITE);
    return S_OK;
}

FTL::FTLThreadWaitType CScreenCaptureSourcePin::_GetWaitType(DWORD dwTimeOut /* = INFINITE */)
{
	HANDLE waitEvent[] = 
	{
		{m_hEventStop},
		{m_hEventContinue}
	};
	FTL::FTLThreadWaitType waitType = FTL::ftwtError;
	DWORD dwResult = ::WaitForMultipleObjects(sizeof(waitEvent)/sizeof(waitEvent[0]),waitEvent,FALSE,dwTimeOut);
	switch(dwResult)
	{
	case WAIT_OBJECT_0: // stop
		waitType = FTL::ftwtStop;
		break;
	case WAIT_OBJECT_0 + 1: //continue
		waitType = FTL::ftwtContinue;
		break;
	case WAIT_TIMEOUT:
		waitType = FTL::ftwtTimeOut;
		break;
	default:
		FTLTRACEEX(FTL::tlError,_T("CScreenCaptureSourcePin::GetWaitType Error!!!\r\n"));
		waitType = FTL::ftwtError;
		FTLASSERT(FALSE);
		break;
	}
	return waitType;
}
STDMETHODIMP CScreenCaptureSourcePin::NonDelegatingQueryInterface(REFIID riid, __deref_out void ** ppv)
{
    //COM_DETECT_RIID_FROM_LIST(riid);

    HRESULT hr = E_NOINTERFACE;
    //FUNCTION_BLOCK_TRACE(1);

    if (riid == IID_IFScreenCaptureConfig)
    {
		hr = GetInterface((IFScreenCaptureConfig*) this, ppv);
    }
	else if (riid == IID_IAMLatency)
	{
		hr = GetInterface((IAMLatency*) this, ppv);
	}
	//else if(riid == IID_IAMStreamConfig)
	//{
	//	hr = GetInterface((IAMStreamConfig *) this, ppv);   
	//}
    //else if(riid == IID_IKsPropertySet)
    //{
    //    return GetInterface((IKsPropertySet *) this, ppv);
    //}
    //else if(riid == IID_IAMVideoCompression)
    //{
    //    hr = GetInterface((IAMVideoCompression *) this, ppv);
    //}

    //else if (riid == IID_IScreenCaptureConfig) 
    //{
    //    hr = GetInterface((IScreenCaptureConfig *) this, ppv);   
    //} 
    else 
    {
		hr = __super::NonDelegatingQueryInterface(riid, ppv);
	}
    return hr;
}

STDMETHODIMP CScreenCaptureSourcePin::SetCaptureRect(IN INT nLeft, IN INT nTop, IN INT nWidth, IN INT nHeight)
{
	m_rcCapture.left = nLeft;
	m_rcCapture.top = nTop;
	m_rcCapture.right = m_rcCapture.left + nWidth;
	m_rcCapture.bottom = m_rcCapture.top + nHeight;

	m_nWidth  = nWidth;
	m_nHeight = nHeight;

	return S_OK;
}

STDMETHODIMP CScreenCaptureSourcePin::SetFrameRate (IN INT nFPS)
{
	if (nFPS <= 0)
	{
		return E_INVALIDARG;
	}
	m_nAvgTimePerFrame = UNITS / nFPS;
	return S_OK;
}

STDMETHODIMP CScreenCaptureSourcePin::SetMouseOverlay(IN BOOL bOverlay)
{
	m_bMouseOverlay = bOverlay;
	return S_OK;
}

STDMETHODIMP CScreenCaptureSourcePin::SetMaxRecordTime(IN INT nMillSecs)
{
	m_rfMaxRecordTime = MILLISECONDS_TO_100NS_UNITS(nMillSecs);
	return S_OK;
}