#pragma once

#include <initguid.h>
#include "ScreenCapture_h.h"

#pragma TODO(StringCchCopyN Usage)
//StringCchCopyN(wszVideo, NUMELMS(wszVideo), szVideo, NUMELMS(wszVideo)-1);
//wszVideo[1023] = wszAudio[1023] = 0;    // Null-terminate

class CScreenCaptureFilter 
    : public CSource
	//, public IAMFilterMiscFlags   // To allow getting flag that filter is live source
    //, public IAMVideoCompression

{
private:
    //It is only allowed to to create these objects with CreateInstance
	CScreenCaptureFilter(IUnknown *pUnk, HRESULT *phr);
	~CScreenCaptureFilter();

    DECLARE_IUNKNOWN
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, __deref_out void ** ppv);

public:
	static CUnknown * WINAPI CreateInstance(IUnknown *pUnk, HRESULT *phr);
public:
	//覆盖基类方法，支持 Pause/Stop ?
	STDMETHODIMP Stop();
	STDMETHODIMP Pause();
	STDMETHODIMP Run(REFERENCE_TIME tStart);

	////Pin的代理实现
	//HRESULT CheckMediaType(const CMediaType* pMediaType);
	//HRESULT GetMediaType(int iPosition, CMediaType *pmt);
	//HRESULT SetMediaType(const CMediaType *pMediaType);

private:
	HANDLE		m_hEventContinue;
	HANDLE		m_hEventStop;
    //CCritSec m_cStateLock;
};
