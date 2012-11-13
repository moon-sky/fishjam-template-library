// ColourExFilter.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ColourExFilter.h"
#include "ColourExFilter_i.c"

CUnknown *CColourExFilter::CreateInstance(LPUNKNOWN punk, HRESULT *phr)
{
	ASSERT(phr);

	CColourExFilter *pNewObject = new CColourExFilter(COLOUREX_FILTER_NAME, punk, phr);

	if (pNewObject == NULL) {
		if (phr)
			*phr = E_OUTOFMEMORY;
	}
	return pNewObject;

} // CreateInstance

CColourExFilter::CColourExFilter(TCHAR *tszName,  LPUNKNOWN punk, HRESULT *phr) 
: CTransformFilter(tszName, punk, CLSID_ColourExFilter)
//, CPersistStream(punk, phr)
{
}


STDMETHODIMP CColourExFilter::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
	CheckPointer(ppv,E_POINTER);

	if (riid == IID_IColourExConfig) 
	{
		return GetInterface((IColourExConfig *) this, ppv);
	} 
	//else if (riid == IID_ISpecifyPropertyPages) 
	//{
	//	return GetInterface((ISpecifyPropertyPages *) this, ppv);
	//}
	else 
	{
		return __super::NonDelegatingQueryInterface(riid, ppv);
	}

} // NonDelegatingQueryInterface

HRESULT CColourExFilter::SetConvertParam(AM_MEDIA_TYPE *pMediaTypeInput, AM_MEDIA_TYPE *pMediaTypeOutput)
{
	return S_OK;
}

HRESULT CColourExFilter::Transform(IMediaSample *pIn, IMediaSample *pOut)
{
	CheckPointer(pIn,E_POINTER);   
	CheckPointer(pOut,E_POINTER);   

	// Copy the properties across

	HRESULT hr = Copy(pIn, pOut);
	if (FAILED(hr)) {
		return hr;
	}

	// Check to see if it is time to do the sample
	//CRefTime tStart, tStop ;
	//hr = pIn->GetTime((REFERENCE_TIME *) &tStart, (REFERENCE_TIME *) &tStop);
	//if (tStart >= m_effectStartTime) 
	//{
	//	if (tStop <= (m_effectStartTime + m_effectTime)) 
	//	{
	//		return Transform(pOut);
	//	}
	//}

	return NOERROR;

} // Transform

//Make destination an identical copy of source
HRESULT CColourExFilter::Copy(IMediaSample *pSource, IMediaSample *pDest)
{
	CheckPointer(pSource,E_POINTER);   
	CheckPointer(pDest,E_POINTER);   

	HRESULT hr = E_FAIL;

	// Copy the sample data

	BYTE *pSourceBuffer = NULL, *pDestBuffer = NULL;
	long lSourceSize = pSource->GetActualDataLength();

#ifdef _DEBUG
	long lDestSize = pDest->GetSize();
	ASSERT(lDestSize >= lSourceSize);
#endif

	DX_VERIFY(pSource->GetPointer(&pSourceBuffer));
	DX_VERIFY(pDest->GetPointer(&pDestBuffer));

	CopyMemory( (PVOID) pDestBuffer,(PVOID) pSourceBuffer, lSourceSize);

	// Copy the sample times

	REFERENCE_TIME TimeStart, TimeEnd;
	DX_VERIFY(pSource->GetTime(&TimeStart, &TimeEnd));
	if (NOERROR == hr)
	{
		DX_VERIFY(pDest->SetTime(&TimeStart, &TimeEnd));
	}

	LONGLONG MediaStart, MediaEnd;
	DX_VERIFY(pSource->GetMediaTime(&MediaStart,&MediaEnd));
	if(NOERROR == hr){
		DX_VERIFY(pDest->SetMediaTime(&MediaStart,&MediaEnd));
	}

	// Copy the Sync point property

	DX_VERIFY_EXCEPT1(pSource->IsSyncPoint(), S_FALSE);
	if (hr == S_OK) {
		pDest->SetSyncPoint(TRUE);
	}
	else if (hr == S_FALSE) {
		pDest->SetSyncPoint(FALSE);
	}
	else {  // an unexpected error has occured...
		return E_UNEXPECTED;
	}

	// Copy the media type

	AM_MEDIA_TYPE *pMediaType = NULL;
	DX_VERIFY(pSource->GetMediaType(&pMediaType));
	DX_VERIFY(pDest->SetMediaType(pMediaType));
	DeleteMediaType(pMediaType);

	// Copy the preroll property

	DX_VERIFY_EXCEPT1(pSource->IsPreroll(), S_FALSE);
	if (hr == S_OK) {
		pDest->SetPreroll(TRUE);
	}
	else if (hr == S_FALSE) {
		pDest->SetPreroll(FALSE);
	}
	else {  // an unexpected error has occured...
		DX_VERIFY(hr);
		return E_UNEXPECTED;
	}

	// Copy the discontinuity property

	DX_VERIFY_EXCEPT1(pSource->IsDiscontinuity(), S_FALSE);
	if (hr == S_OK) {
		pDest->SetDiscontinuity(TRUE);
	}
	else if (hr == S_FALSE) {
		pDest->SetDiscontinuity(FALSE);
	}
	else {  // an unexpected error has occured...
		return E_UNEXPECTED;
	}

	// Copy the actual data length

	long lDataLength = pSource->GetActualDataLength();
	DX_VERIFY(pDest->SetActualDataLength(lDataLength));

	return NOERROR;
}

HRESULT CColourExFilter::CheckInputType(const CMediaType *mtIn)
{
	CheckPointer(mtIn,E_POINTER);

	// check this is a VIDEOINFOHEADER type

	if (*mtIn->FormatType() != FORMAT_VideoInfo) {
		return E_INVALIDARG;
	}

	// Can we transform this type

	if (CanPerform(mtIn)) {
		return NOERROR;
	}
	return E_FAIL;
}

HRESULT CColourExFilter::CheckTransform(const CMediaType *mtIn, const CMediaType *mtOut)
{
	CheckPointer(mtIn,E_POINTER);
	CheckPointer(mtOut,E_POINTER);

	if (CanPerform(mtIn)) 
	{
		if (*mtIn == *mtOut) 
		{
			return NOERROR;
		}
	}

	return E_FAIL;

} // CheckTransform

//
// DecideBufferSize
//
// Tell the output pin's allocator what size buffers we
// require. Can only do this when the input is connected
//
HRESULT CColourExFilter::DecideBufferSize(IMemAllocator *pAlloc,ALLOCATOR_PROPERTIES *pProperties)
{
	CheckPointer(pAlloc,E_POINTER);
	CheckPointer(pProperties,E_POINTER);

	// Is the input pin connected

	if (m_pInput->IsConnected() == FALSE) {
		return E_UNEXPECTED;
	}

	HRESULT hr = NOERROR;

	pProperties->cBuffers = 1;
	pProperties->cbBuffer = m_pInput->CurrentMediaType().GetSampleSize();
	ASSERT(pProperties->cbBuffer);

	// Ask the allocator to reserve us some sample memory, NOTE the function
	// can succeed (that is return NOERROR) but still not have allocated the
	// memory that we requested, so we must check we got whatever we wanted

	ALLOCATOR_PROPERTIES Actual;
	DX_VERIFY(pAlloc->SetProperties(pProperties,&Actual));
	if (FAILED(hr)) {
		return hr;
	}

	ASSERT( Actual.cBuffers == 1 );

	if (pProperties->cBuffers > Actual.cBuffers ||
		pProperties->cbBuffer > Actual.cbBuffer) 
	{
		FTLASSERT(FALSE);
		return E_FAIL;
	}
	return NOERROR;

} // DecideBufferSize


//
// GetMediaType
//
// I support one type, namely the type of the input pin
// This type is only available if my input is connected
//
HRESULT CColourExFilter::GetMediaType(int iPosition, CMediaType *pMediaType)
{
	CheckPointer(pMediaType,E_POINTER);

	// Is the input pin connected

	if (m_pInput->IsConnected() == FALSE) {
		return E_UNEXPECTED;
	}

	// This should never happen

	if (iPosition < 0) {
		return E_INVALIDARG;
	}

	// Do we have more items to offer

	if (iPosition > 0) {
		return VFW_S_NO_MORE_ITEMS;
	}

	*pMediaType = m_pInput->CurrentMediaType();

	return NOERROR;

} // GetMediaType
