#pragma once

#include "AsyncTransFilter_h.h"

class CAsyncTransformFilter : public CTransformFilter,
	//public ISpecifyPropertyPages,
	//public CPersistStream,
	public IAsyncTransformFilter
{
public:

	DECLARE_IUNKNOWN;
	static CUnknown * WINAPI CreateInstance(LPUNKNOWN punk, HRESULT *phr);

	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);

	//IAsyncTransformFilter
	virtual HRESULT STDMETHODCALLTYPE Hello( void);

	// Overrride from CTransformFilter base class
	virtual HRESULT Transform(IMediaSample *pIn, IMediaSample *pOut);
	virtual HRESULT CheckInputType(const CMediaType *mtIn);
	virtual HRESULT CheckTransform(const CMediaType *mtIn, const CMediaType *mtOut);
	virtual HRESULT DecideBufferSize(IMemAllocator *pAlloc,
		ALLOCATOR_PROPERTIES *pProperties);
	virtual HRESULT GetMediaType(int iPosition, CMediaType *pMediaType);

	// ISpecifyPropertyPages interface
	//STDMETHODIMP GetPages(CAUUID *pPages);

	// CPersistStream
	//STDMETHODIMP GetClassID(CLSID *pClsid);
	//HRESULT ReadFromStream(IStream *pStream);

protected:
	virtual HRESULT Copy(IMediaSample *pSource, IMediaSample *pDest);
	virtual BOOL CanPerform(const CMediaType *pMediaType)
	{
		return TRUE;
	}
private:
	// Constructor
	CAsyncTransformFilter(TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr);
};