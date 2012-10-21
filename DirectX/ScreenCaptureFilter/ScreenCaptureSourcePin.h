#ifndef SCREEN_CAPTURE_SOURCE_PIN_H
#define SCREEN_CAPTURE_SOURCE_PIN_H

#include "ScreenCaptureImpl.h"
#include <ftlThread.h>

//Êä³öPinÀàÐÍ RGB video out
class CScreenCaptureSourcePin 
	: public CSourceStream
	//, public IAMStreamControl // Start Stop notify 
	, public IKsPropertySet   // For expose pin category -- ICaptureGraphBuilder2:: 
	//, public IAMPushSource    // For push source settings and configuring
	, public IAMLatency       // Latency 
	, public IAMStreamConfig  // Format configuring 
	//, public IAMBufferNegotiation  // Allocator configuring
{
public:
	CScreenCaptureSourcePin(HRESULT *phr, CSource *pFilter, HANDLE hEventContinue, HANDLE hEventStop);
	~CScreenCaptureSourcePin();

	DECLARE_IUNKNOWN
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, __deref_out void ** ppv);

protected:
	// Override this to provide the worker thread a means of processing a buffer
	virtual HRESULT FillBuffer(IMediaSample *pSample);
	virtual HRESULT DecideBufferSize(
	        IMemAllocator * pAlloc,
	        __inout ALLOCATOR_PROPERTIES * ppropInputRequest
	    );

	// Set the agree media type set up the necessary parameters
	HRESULT SetMediaType(const CMediaType *pMediaType);
	HRESULT CheckMediaType(const CMediaType* pMediaType);
	HRESULT GetMediaType(int iPosition, CMediaType *pmt);
	HRESULT GetDefaultCaps(int nIndex, VIDEO_STREAM_CONFIG_CAPS* pCaps);

	virtual HRESULT Active(void);    // Starts up the worker thread
	virtual HRESULT Inactive(void);  // Exits the worker thread.

	virtual HRESULT Run(REFERENCE_TIME tStart);

	// Quality control notifications sent to us
	STDMETHODIMP Notify(IBaseFilter * pSender, Quality q);

	//IAMLatency
	virtual HRESULT STDMETHODCALLTYPE GetLatency(REFERENCE_TIME* pOutLatency);
public:
	/////////////////////////      IKsPropertySet       /////////////////////////////////////////////////
	virtual /* [local] */ HRESULT STDMETHODCALLTYPE Set( 
	/* [in] */ REFGUID guidPropSet,
	/* [in] */ DWORD dwPropID,
	/* [annotation][size_is][in] */ 
	__in_bcount(cbInstanceData)  LPVOID pInstanceData,
	/* [in] */ DWORD cbInstanceData,
	/* [annotation][size_is][in] */ 
	__in_bcount(cbPropData)  LPVOID pPropData,
	/* [in] */ DWORD cbPropData);

	virtual /* [local] */ HRESULT STDMETHODCALLTYPE Get( 
		/* [in] */ REFGUID guidPropSet,
		/* [in] */ DWORD dwPropID,
		/* [annotation][size_is][in] */ 
		__in_bcount(cbInstanceData)  LPVOID pInstanceData,
		/* [in] */ DWORD cbInstanceData,
		/* [annotation][size_is][out] */ 
		__out_bcount_part(cbPropData, *pcbReturned)  LPVOID pPropData,
		/* [in] */ DWORD cbPropData,
		/* [annotation][out] */ 
		__out  DWORD *pcbReturned);

	virtual HRESULT STDMETHODCALLTYPE QuerySupported( 
		/* [in] */ REFGUID guidPropSet,
		/* [in] */ DWORD dwPropID,
		/* [annotation][out] */ 
		__out  DWORD *pTypeSupport);

	/////////////////////////      IAMStreamConfig       /////////////////////////////////////////////////
	virtual HRESULT STDMETHODCALLTYPE SetFormat( 
		/* [in] */ AM_MEDIA_TYPE *pmt)
	{
		if (m_pFilter->IsActive())
		{
			return VFW_E_WRONG_STATE;
		}
		HRESULT hr = CheckMediaType((CMediaType*)pmt);
		if (FAILED(hr))
		{
			return hr;
		}
		/*
		HRESULT hr;
		AMMediaType _newType = new AMMediaType(pmt);
		AMMediaType _oldType = new AMMediaType(m_mt);
		hr = (HRESULT)CheckMediaType(_newType);
		if (FAILED(hr)) return hr;
		m_mt.Set(_newType);
		if (IsConnected)
		{
			hr = (HRESULT)Connected.QueryAccept(_newType);
			if (SUCCEEDED(hr))
			{
				hr = (HRESULT)m_Filter.ReconnectPin(this, _newType);
				if (SUCCEEDED(hr))
				{
					hr = (HRESULT)(m_Filter as VirtualCamFilter).SetMediaType(_newType);
				}
				else
				{
					m_mt.Set(_oldType);
					m_Filter.ReconnectPin(this, _oldType);
				}
			}
		}
		else
		{
			hr = (HRESULT)(m_Filter as VirtualCamFilter).SetMediaType(_newType);
		}
		*/
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE GetFormat( 
		/* [out] */ __out  AM_MEDIA_TYPE **ppmt)
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE GetNumberOfCapabilities( 
		/* [out] */ __out  int *piCount,
		/* [out] */	__out  int *piSize)
	{
		HRESULT hr =E_FAIL;
		int iCount = 0;
		CMediaType mt;
		while (hr = GetMediaType(iCount, &mt), SUCCEEDED(hr))
		{
			iCount++;
			mt.ResetFormatBuffer();
		}
		*piCount = iCount;
		*piSize = sizeof(VIDEO_STREAM_CONFIG_CAPS);

		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetStreamCaps( 
		/* [in] */ int iIndex,
		/* [out] */ __out  AM_MEDIA_TYPE **ppmt,
		/* [out] */ __out  BYTE *pSCC)   
	{
		// return VideoStreamConfigCaps
		if (iIndex < 0)
		{
			return E_INVALIDARG;
		}

		CMediaType mt;
		HRESULT hr = (HRESULT)GetMediaType(iIndex, &mt);
		if (FAILED(hr))
		{
			*ppmt = NULL;
			return hr;
		}

		if (hr == VFW_S_NO_MORE_ITEMS)
		{
			return S_FALSE;
		}
		//VIDEO_STREAM_CONFIG_CAPS* pVideoCaps = (VIDEO_STREAM_CONFIG_CAPS*)CoTaskMemAlloc(sizeof(VIDEO_STREAM_CONFIG_CAPS));
		//if (!pVideoCaps)
		//{
		//	CoTaskMemFree(*ppmt);
		//	*ppmt = NULL;
		//	return E_OUTOFMEMORY;
		//}
		//ZeroMemory(pVideoCaps, sizeof(VIDEO_STREAM_CONFIG_CAPS));
		hr = (HRESULT)GetDefaultCaps(iIndex, (VIDEO_STREAM_CONFIG_CAPS*)pSCC);

		*ppmt = (AM_MEDIA_TYPE*)CoTaskMemAlloc(sizeof(AM_MEDIA_TYPE));
		CopyMemory(*ppmt, &mt, sizeof(AM_MEDIA_TYPE));
		return hr;

		return E_NOTIMPL;
	}

protected:
    virtual HRESULT OnThreadCreate(void);
    virtual HRESULT OnThreadDestroy(void);
    virtual HRESULT OnThreadStartPlay(void);
protected:
	FTL::FTLThreadWaitType _GetWaitType(DWORD dwTimeOut = INFINITE);
private:
	BOOL			m_bFirstFrame;
	BOOL			m_bMouseOverlay;
	HANDLE			m_hEventContinue;
	HANDLE			m_hEventStop;
	//CMediaType     m_MediaType;
	CCritSec       m_cSharedState;
	//CImageDisplay  m_Display; //
	//int            m_nCurrentBitDepth;
    //int            m_iRepeatTime;       // Time in msec between frames
	//int            m_iImageWidth;
	//int            m_iImageHeight;
	RECT			m_rcCapture;
	//REFERENCE_TIME m_rtFrameLength;
	REFERENCE_TIME	m_rfMaxRecordTime;
	LONGLONG       m_nFrameNumber;
	//CRefTime       m_rtSampleTime;      // The time stamp for each sample
	//int            m_FrameWritten;
	//BOOL           m_bZeroMemory;
protected:
	//CCritSec			m_csPinLock;
	//CCritSec			m_csTimeLock;


	static const INT DEFAULT_WIDTH  = 320;
	static const INT DEFAULT_HEIGTH = 240;
	static const INT FORMATS_COUNT	= 8;
	static const INT MIN_FPS		= 1;
	static const INT MAX_FPS		= 100;
	static const INT DEFAULT_FPS	= 25;
	static const INT DEFAULT_BIT_COUNT = 32;

	INT		m_nMaxWidth;
	INT		m_nMaxHeight;
	INT		m_nWidth;
	INT		m_nHeight;
	INT		m_nBitCount;
	BITMAPINFO  m_bmpInfo;
	REFERENCE_TIME		m_nAvgTimePerFrame; //Average time per frame (100ns units), UNITS / DEFAULT_FPS


	REFERENCE_TIME		m_rtStart;
	REFERENCE_TIME		m_rtStreamOffset;

	//REFERENCE_TIME		m_rtStreamOffsetMax;// = -1
	REFERENCE_TIME		m_rtStartAt;// = -1;
	REFERENCE_TIME		m_rtStopAt;// = -1;
	int					m_dwStopCookie;// = 0;
	int					m_dwStartCookie;// = 0;
	bool				m_bShouldFlush;// = false;
	bool				m_bStartNotified;// = false;
	bool				m_bStopNotified;// = false;
	ALLOCATOR_PROPERTIES*	m_pProperties;// = null;
	CComPtr<IReferenceClock>	m_pClock;  //IReferenceClockImpl m_pClock = null;
	IScreenCaptureImpl*		m_pScreenCaptureImpl;
	//CScreenCaptureImpl m_ScreenCaptureImpl;

	// Clock Token
	DWORD_PTR m_dwAdviseToken;
	// Clock Semaphore
	HANDLE m_hSemaphore;//
	HANDLE	m_hEventAfterFilterRun;
	//Semaphore m_hSemaphore = null;

	// Clock Start time
	CRefTime m_rtClockStart;
	// Clock Stop time
	CRefTime m_rtClockStop;

};


#endif //SCREEN_CAPTURE_SOURCE_PIN_H