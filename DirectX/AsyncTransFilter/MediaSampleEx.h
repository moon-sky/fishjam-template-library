//-----------------------------------------------------------------------------
//
//	MONOGRAM DirectShow BaseClasses
//
//	Author : Igor Janos
//
//-----------------------------------------------------------------------------
#pragma once


//-----------------------------------------------------------------------------
//
//	IMediaSampleEx class
//
//-----------------------------------------------------------------------------

// {5C4B73E4-DBF8-4146-A6F6-8D0F30FA4C41}
static const GUID IID_IMediaSampleEx = 
{ 0x5c4b73e4, 0xdbf8, 0x4146, { 0xa6, 0xf6, 0x8d, 0xf, 0x30, 0xfa, 0x4c, 0x41 } };

/**	\brief Allows the user to set and get values for both PTS and DTS timestamps.
 */
DECLARE_INTERFACE_(IMediaSampleEx, IUnknown)
{
	/** \brief Set timestamp values.
	 *
	 *	@param pts pointer to a REFERENCE_TIME variable containing the PTS timestamp.
	 *	@param dts pointer to a REFERENCE_TIME variable containing the DTS timestamp.
	 *	@param clock the number of units in which PTS and DTS values are measured that make one second.
	 */
	STDMETHOD(SetTimeEx)(REFERENCE_TIME *pts, REFERENCE_TIME *dts, int clock);
	/** \brief Get timestamp values.
	 *
	 *	@param pts pointer to a REFERENCE_TIME variable to receive the PTS timestamp.
	 *	@param dts pointer to a REFERENCE_TIME variable to receive the DTS timestamp.
	 *	@param clock pointer to a variable to receive the clock rate.
	 */
	STDMETHOD(GetTimeEx)(REFERENCE_TIME *pts, REFERENCE_TIME *dts, int *clock);
};


//-----------------------------------------------------------------------------
//
//	CMediaSampleEx class
//
//-----------------------------------------------------------------------------

/**	\brief Extended Media Sample class.
 *
 *	This class allows the user to set both PTS and DTS timestamp values
 *	including the clock rate at which the timestamps are measured.
 */
class CMediaSampleEx : 
	public CMediaSample,
	public IMediaSampleEx
{
public:

	// rozsirene informacie
	int					clock;			///< specifies how long does one second take.
	REFERENCE_TIME		pts;			///< presentation timestamp
	REFERENCE_TIME		dts;			///< decoding timestamp

public:
	/**	Constructor.
	 *
	 *	@param pName object name (for debugging purposes)
	 *	@param pAllocator pointer to the owning allocator object
	 *	@param phr constructor result value
	 *	@param pBuffer data buffer to work with
	 *	@param length length of the supplied buffer in bytes
	 */
    CMediaSampleEx(LPCTSTR pName, CBaseAllocator *pAllocator, HRESULT *phr, LPBYTE pBuffer = NULL, LONG length = 0);

	/**	Destructor.
	 */
    virtual ~CMediaSampleEx();

	// exposovanie IMediaSampleEx
    STDMETHODIMP QueryInterface(REFIID riid, void **ppv);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();

	// IMediaSampleEx
	/** \brief Set timestamp values.
	 *
	 *	@param pts pointer to a REFERENCE_TIME variable containing the PTS timestamp.
	 *	@param dts pointer to a REFERENCE_TIME variable containing the DTS timestamp.
	 *	@param clock the number of units in which PTS and DTS values are measured that make one second.
	 */
	STDMETHODIMP SetTimeEx(REFERENCE_TIME *pts, REFERENCE_TIME *dts, int clock);
	/** \brief Get timestamp values.
	 *
	 *	@param pts pointer to a REFERENCE_TIME variable to receive the PTS timestamp.
	 *	@param dts pointer to a REFERENCE_TIME variable to receive the DTS timestamp.
	 *	@param clock pointer to a variable to receive the clock rate.
	 */
	STDMETHODIMP GetTimeEx(REFERENCE_TIME *pts, REFERENCE_TIME *dts, int *clock);
};


//-----------------------------------------------------------------------------
//
//	CMemAllocatorEx class
//
//-----------------------------------------------------------------------------

/**	\brief Allocator to provide samples supporting the IMediaSampleEx interface.
 
 	This class creates IMediaSample instances that allow the user to
 	take advantage of the IMediaSampleEx features and to specify PTS
 	and DTS values in any desired clock.
 	
 	The following example shows how to use the CMemAllocatorEx class
 	with your output pins.

\code
//------------------------------------------------------------------------------
//
//	CX264OutputPin class
//
//------------------------------------------------------------------------------
class CX264OutputPin : public CTransformOutputPin
{
public:
	CX264OutputPin(CTransformFilter *pFilter, HRESULT *phr, LPCWSTR pName);
	~CX264OutputPin();
	
	// use CMemAllocatorEx
	virtual HRESULT InitAllocator(IMemAllocator **ppAlloc);
	virtual HRESULT DecideAllocator(IMemInputPin *pPin, IMemAllocator **pAlloc);
};


HRESULT CX264OutputPin::InitAllocator(IMemAllocator **ppAlloc)
{
	// creat the new allocator instead
	return CreateMemoryAllocatorEx(ppAlloc);
} 

HRESULT CX264OutputPin::DecideAllocator(IMemInputPin *pPin, IMemAllocator **pAlloc)
{
	HRESULT		hr 	= NOERROR;
	*pAlloc			= NULL;
	
	ALLOCATOR_PROPERTIES	prop;
	ZeroMemory(&prop, sizeof(prop));
	
	pPin->GetAllocatorRequirements(&prop);
	if (prop.cbAlign == 0) { prop.cbAlign = 1; }
	
	// now we only want to use our allocator with PTS/DTS capability
	hr = InitAllocator(pAlloc);
	if (SUCCEEDED(hr)) {
		hr = DecideBufferSize(*pAlloc, &prop);
		if (SUCCEEDED(hr)) {
			hr = pPin->NotifyAllocator(*pAlloc, FALSE);
			if (SUCCEEDED(hr)) return NOERROR;
		}
	}
	
	// in case something went wrong, abort ...
	if (*pAlloc) {
		(*pAlloc)->Release();
		*pAlloc = NULL;
	}
	return hr;
}
 
\endcode
 
*/
class CMemAllocatorEx : public CMemAllocator
{
protected:

    // overriden to allocate the memory when commit called
    HRESULT Alloc(void);

public:
    CMemAllocatorEx(LPCTSTR name, LPUNKNOWN pUnk, HRESULT *phr);
    ~CMemAllocatorEx();
};

STDAPI CreateMemoryAllocatorEx(IMemAllocator **ppAllocator);

