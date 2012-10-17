#include "stdafx.h"
#include "ScreenCaptureFilter.h"
#include "ScreenCaptureSourcePin.h"
#include "ScreenCapture_i.c"

CUnknown * WINAPI CScreenCaptureFilter::CreateInstance(IUnknown *pUnk, HRESULT *phr)
{
	CScreenCaptureFilter *pNewFilter = new CScreenCaptureFilter(pUnk, phr);

    if (NULL == pNewFilter)
    {
        if (phr)
        {
            *phr = E_OUTOFMEMORY;
        }
    }

	return pNewFilter;
}

CScreenCaptureFilter::CScreenCaptureFilter(IUnknown *pUnk, HRESULT *phr)
: CSource(NAME("Fishjam Screen Capture"), pUnk, CLSID_FishjamScreenCapture)
{
	FTLTRACE(TEXT("CScreenCaptureFilter::CScreenCaptureFilter, this=0x%x\n"), this);

    CAutoLock cAutoLock(&m_cStateLock);
	m_hEventContinue = ::CreateEvent(NULL, TRUE, TRUE, NULL);
	m_hEventStop	 = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	m_paStreams = (CSourceStream **)new CSourceStream*[1];
	if (m_paStreams)
	{
		m_paStreams[0] = new CScreenCaptureSourcePin(phr, this, m_hEventContinue, m_hEventStop);
	}

	if (phr)
	{
		if (!m_paStreams || !m_paStreams[0])
		{
			*phr = E_OUTOFMEMORY;
		}
	}

	//m_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

CScreenCaptureFilter::~CScreenCaptureFilter()
{
	CloseHandle(m_hEventContinue);
	CloseHandle(m_hEventStop);
	m_hEventContinue = NULL;
	m_hEventStop = NULL;
	FTLTRACE(TEXT("CScreenCaptureFilter::~CScreenCaptureFilter, this=0x%x\n"), this);
}

STDMETHODIMP CScreenCaptureFilter::Stop()
{
	FTLTRACEEX(FTL::tlWarning, TEXT("Enter CScreenCaptureFilter::Stop, m_State=%d\n"), m_State);
	BOOL bRet = FALSE;
	API_VERIFY(::SetEvent(m_hEventStop));
	HRESULT hr = E_FAIL;
	DX_VERIFY(__super::Stop());
	FTLTRACEEX(FTL::tlWarning, TEXT("Leave CScreenCaptureFilter::Stop, m_State=%d\n"), m_State);
	return hr;
}

STDMETHODIMP CScreenCaptureFilter::Pause()
{
	FTLTRACEEX(FTL::tlWarning, TEXT("Enter CScreenCaptureFilter::Pause, m_State=%d\n"), m_State);
	BOOL bRet = FALSE;
	API_VERIFY(::ResetEvent(m_hEventContinue));
	if (m_State == State_Stopped)
	{
		//First Run
	}
	HRESULT hr = E_FAIL;
	DX_VERIFY(__super::Pause());
	FTLTRACEEX(FTL::tlWarning, TEXT("Leave CScreenCaptureFilter::Pause, m_State=%d, hr=0x%x\n"), m_State, hr);
	return hr;
}

STDMETHODIMP CScreenCaptureFilter::Run(REFERENCE_TIME tStart)
{
	FTLTRACEEX(FTL::tlWarning, TEXT("Enter CScreenCaptureFilter::Run, m_State=%d, tStart=%lld\n"), m_State, tStart);
	BOOL bRet = FALSE;
	API_VERIFY(SetEvent(m_hEventContinue));
	HRESULT hr = E_FAIL;
	DX_VERIFY(__super::Run(tStart));
	FTLTRACEEX(FTL::tlWarning, TEXT("Leave CScreenCaptureFilter::Run, m_State=%d\n"), m_State);
	return hr;
}

STDMETHODIMP CScreenCaptureFilter::NonDelegatingQueryInterface(REFIID riid, __deref_out void ** ppv)
{
    //COM_DETECT_RIID_FROM_LIST(riid);
    HRESULT hr = E_NOINTERFACE; 
    //FUNCTION_BLOCK_TRACE(1);
	//if (riid == IID_IScreenCaptureCfg)
	//{
	//	hr = m_paStreams[0]->QueryInterface(riid, ppv);
	//}

	if (riid == IID_IUnknown)
	{
		hr = __super::NonDelegatingQueryInterface(riid, ppv);
	}
    //else if(riid == IID_IAMStreamConfig)
    //{
    //    hr = GetInterface((IAMStreamConfig *) this, ppv);   
    //}
    //else if (riid == IID_IAMVideoCompression)
    //{
    //    //让输出接口返回 -- 是否应该这样?
    //    //hr = m_paStreams[0]->QueryInterface(riid, ppv);
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

//STDMETHODIMP CScreenCaptureFilter::Run(REFERENCE_TIME tStart)
//{
//	HRESULT hr = __super::Run(tStart);    
//	SetEvent(m_hEvent);
//
//	return hr;
//}
//
//STDMETHODIMP CScreenCaptureFilter::Stop()
//{
//	HRESULT hr = __super::Stop();    
//	ResetEvent(m_hEvent);
//
//	return hr;
//}
//
//void CScreenCaptureFilter::Wait()
//{
//	WaitForSingleObject(m_hEvent, INFINITE);
//}