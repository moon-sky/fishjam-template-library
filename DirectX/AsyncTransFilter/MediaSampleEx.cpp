//-----------------------------------------------------------------------------
//
//	MONOGRAM DirectShow BaseClasses
//
//	Author : Igor Janos
//
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include <streams.h>
#include "MediaSampleEx.h"


//-----------------------------------------------------------------------------
//
//	CMediaSampleEx class
//
//-----------------------------------------------------------------------------

CMediaSampleEx::CMediaSampleEx(LPCTSTR pName, CBaseAllocator *pAllocator, HRESULT *phr, LPBYTE pBuffer, LONG length) :
	CMediaSample(pName, pAllocator, phr, pBuffer, length),
	clock(0),
	pts(0),
	dts(0)
{

}

CMediaSampleEx::~CMediaSampleEx()
{
	// nic extra
}

STDMETHODIMP CMediaSampleEx::QueryInterface(REFIID riid, void **ppv)
{
	if (riid == IID_IMediaSampleEx) {
		return GetInterface((IMediaSampleEx*)this, ppv);
	} else {
		return CMediaSample::QueryInterface(riid, ppv);
	}
}

STDMETHODIMP_(ULONG) CMediaSampleEx::AddRef()
{
	return CMediaSample::AddRef();
}

STDMETHODIMP_(ULONG) CMediaSampleEx::Release()
{
	return CMediaSample::Release();
}

STDMETHODIMP CMediaSampleEx::SetTimeEx(REFERENCE_TIME *pts, REFERENCE_TIME *dts, int clock)
{
	this->clock = clock;
	this->pts = 0;
	this->dts = 0;

	if (pts) this->pts = *pts;
	if (dts) this->dts = *dts;
	return NOERROR;
}

STDMETHODIMP CMediaSampleEx::GetTimeEx(REFERENCE_TIME *pts, REFERENCE_TIME *dts, int *clock)
{
	if (pts) *pts = this->pts;
	if (dts) *dts = this->dts;
	if (clock) *clock = this->clock;

	return NOERROR;
}

//-----------------------------------------------------------------------------
//
//	CMemAllocatorEx class
//
//-----------------------------------------------------------------------------

/*!	\fn STDAPI CreateMemoryAllocatorEx(IMemAllocator **ppAllocator)
 *	\brief Creates an instance of the CMemAllocatorEx class.
 *
 *	@param ppAllocator pointer to an interface pointer to receive the memory allocator object.
 *	@retval NOERROR Success
 *	@retval E_POINTER NULL argument
 *	@retval E_OUTOFMEMORY Not enough memory available to create the allocator
 *	@retval E_UNEXPECTED Unexpected error occured
 */
STDAPI CreateMemoryAllocatorEx(IMemAllocator **ppAllocator)
{
	if (!ppAllocator) return E_POINTER;

	HRESULT				hr = NOERROR;
	CMemAllocatorEx		*alloc = new CMemAllocatorEx(_T("MemoryAllocatorEx"), NULL, &hr);

	if (!alloc) return E_OUTOFMEMORY;

	hr = alloc->QueryInterface(IID_IMemAllocator, (void**)ppAllocator);
	if (FAILED(hr)) {
		delete alloc;
		return E_UNEXPECTED;
	}

	return NOERROR;
}


CMemAllocatorEx::CMemAllocatorEx(LPCTSTR name, LPUNKNOWN pUnk, HRESULT *phr) :
	CMemAllocator(name, pUnk, phr)
{

}

CMemAllocatorEx::~CMemAllocatorEx()
{
	// zatial nic
}

HRESULT CMemAllocatorEx::Alloc(void)
{
    CAutoLock lck(this);

    HRESULT hr = CBaseAllocator::Alloc();
    if (FAILED(hr)) return hr;

    /* If the requirements haven't changed then don't reallocate */
    if (hr == S_FALSE) {
        ASSERT(m_pBuffer);
        return NOERROR;
    }
    ASSERT(hr == S_OK); // we use this fact in the loop below

    /* Free the old resources */
    if (m_pBuffer) ReallyFree();

    /* Make sure we've got reasonable values */
    if ( m_lSize < 0 || m_lPrefix < 0 || m_lCount < 0 ) return E_OUTOFMEMORY;

    /* Compute the aligned size */
    LONG lAlignedSize = m_lSize + m_lPrefix;

    /*  Check overflow */
    if (lAlignedSize < m_lSize) return E_OUTOFMEMORY;
    if (m_lAlignment > 1) {
        LONG lRemainder = lAlignedSize % m_lAlignment;
        if (lRemainder != 0) {
            LONG lNewSize = lAlignedSize + m_lAlignment - lRemainder;
            if (lNewSize < lAlignedSize) return E_OUTOFMEMORY;
            lAlignedSize = lNewSize;
        }
    }

    /* Create the contiguous memory block for the samples
       making sure it's properly aligned (64K should be enough!)
    */
    ASSERT(lAlignedSize % m_lAlignment == 0);
    LONGLONG lToAllocate = m_lCount * (LONGLONG)lAlignedSize;

    /*  Check overflow */
    if (lToAllocate > MAXLONG) return E_OUTOFMEMORY;

    m_pBuffer = (PBYTE)VirtualAlloc(NULL, (LONG)lToAllocate, MEM_COMMIT, PAGE_READWRITE);
    if (m_pBuffer == NULL) return E_OUTOFMEMORY;

    LPBYTE pNext = m_pBuffer;
    CMediaSample *pSample;

    ASSERT(m_lAllocated == 0);

    // Create the new samples - we have allocated m_lSize bytes for each sample
    // plus m_lPrefix bytes per sample as a prefix. We set the pointer to
    // the memory after the prefix - so that GetPointer() will return a pointer
    // to m_lSize bytes.
    for (; m_lAllocated < m_lCount; m_lAllocated++, pNext += lAlignedSize) {

        pSample = new CMediaSampleEx(NAME("Default memory media sample"), this, &hr, pNext + m_lPrefix, m_lSize);         
        ASSERT(SUCCEEDED(hr));
        if (pSample == NULL) return E_OUTOFMEMORY;

        // This CANNOT fail
        m_lFree.Add(pSample);
    }

    m_bChanged = FALSE;
    return NOERROR;
}

