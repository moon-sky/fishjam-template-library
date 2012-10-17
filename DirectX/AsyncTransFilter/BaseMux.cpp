//-----------------------------------------------------------------------------
//
//	MONOGRAM DirectShow BaseClasses
//
//	Author : Igor Janos
//  
//	http://www.monogrammultimedia.com
//
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include <streams.h>
#include "BaseMux.h"
#include "MediaSampleEx.h"

#pragma warning(disable: 4018)		// signed/unsigned mismatch

//-----------------------------------------------------------------------------
//
//	CMuxInterPacket class
//
//-----------------------------------------------------------------------------
CMuxInterPacket::CMuxInterPacket() :
	m_rtStart10mhz(0),
	m_rtStop10mhz(0),
	m_bHasTime(false),
	m_bSyncPoint(false),
	m_nStreamIndex(-1),
	m_bIsEos(false),
	m_pData(NULL),
	m_nSize(0)
{
	m_bTimeStampEx	= false;
	m_pts = 0;
	m_dts = 0;
	m_clock = 0;
}

CMuxInterPacket::~CMuxInterPacket()
{
	if (m_pData) {
		free(m_pData);
		m_pData = NULL;
	}
}

void CMuxInterPacket::LoadFrom(IMediaSample *sample)
{
	// kick any m_pData we might have held
	if (m_pData) {
		free(m_pData);
		m_pData = NULL;
	}

	m_bIsEos = false;

	// get the timestamps
	REFERENCE_TIME	start, stop;
	HRESULT hr = sample->GetTime(&start, &stop);
	if (FAILED(hr)) {
		m_rtStart10mhz = 0;
		m_rtStop10mhz = 0;
		m_bHasTime = false;
	} else {
		if (hr == VFW_S_NO_STOP_TIME) {
			stop = start;
		}

		m_rtStart10mhz = start;
		m_rtStop10mhz = stop;
		m_bHasTime = true;
	}

	CComPtr<IMediaSampleEx>		msex;
	hr = sample->QueryInterface(IID_IMediaSampleEx, (void**)&msex);
	if (SUCCEEDED(hr)) {
		hr = msex->GetTimeEx(&m_pts, &m_dts, &m_clock);
		if (SUCCEEDED(hr)) {
			m_bTimeStampEx = true;
		}
		msex = NULL;
	} else {
		m_bTimeStampEx = false;
	}

	long lsize = sample->GetActualDataLength();
	if (lsize > 0) {

		// now read the m_pData
		m_pData = (BYTE*)malloc(lsize);
		if (!m_pData) {
			// not enough memory !!!
			m_nSize = 0;
			return ;
		}

		BYTE *buf;
		sample->GetPointer(&buf);
		memcpy(m_pData, buf, lsize);
		m_nSize = lsize;
	} else {
		// support for VFW - delay samples
		m_pData = NULL;
		m_nSize = 0;
	}

	// syncpoint ?
	if (sample->IsSyncPoint() == NOERROR) {
		m_bSyncPoint = true; 
	} else {
		m_bSyncPoint = false;
	}
}

//-----------------------------------------------------------------------------
//
//	CMuxInterStream class
//
//-----------------------------------------------------------------------------

CMuxInterStream::CMuxInterStream() :
	m_nIndex(0),
	m_bIsInterleaved(true),
	m_bActive(false),
	m_nSize(10),
	m_evCanWrite(TRUE)
{
	m_evCanWrite.Set();
	m_pData = NULL;
}

CMuxInterStream::~CMuxInterStream()
{
	Flush();
}

void CMuxInterStream::Flush()
{
	// delete all queued samples

	while (m_BufferQueue.size() > 0) {
		CMuxInterPacket *packet = m_BufferQueue.front();
		m_BufferQueue.pop_front();

		delete packet;
	}

	// since we haven't got anything - the stream is writable again
	m_evCanWrite.Set();
}

int CMuxInterStream::Write(CMuxInterPacket *packet)
{
	// append the new packet into the queue
	m_BufferQueue.push_back(packet);
	if (m_BufferQueue.size() >= m_nSize) 
	{
		m_evCanWrite.Reset();
	}

	return 0;
}

int CMuxInterStream::Read(CMuxInterPacket **packet)
{
	if (m_BufferQueue.size() == 0) return -1;

	// load the first packet
	ASSERT(packet);
	CMuxInterPacket *p = m_BufferQueue.front();

	// if it's an EOS packet we won't remove it from the queue
	if (p->m_bIsEos) return -3;

	m_BufferQueue.pop_front();
	*packet = p;

	if (m_BufferQueue.size() < m_nSize) 
	{
		m_evCanWrite.Set();
	}
	return 0;
}

int CMuxInterStream::GetStartTime(REFERENCE_TIME *time)
{
	if (m_BufferQueue.size() == 0) return -1;

	// get the first timestamp
	// Note: This makes only sense if the incomming packets carry timestamps
	CMuxInterPacket *packet = m_BufferQueue.front();
	ASSERT(time);

	*time = packet->m_rtStart10mhz;
	return 0;
}

int CMuxInterStream::GetPacketCount()
{
	return (int)m_BufferQueue.size();
}

bool CMuxInterStream::IsEOS()
{
	// no packets = no EOS
	if (m_BufferQueue.size() == 0) return false;

	CMuxInterPacket *packet = m_BufferQueue.front();
	if (packet->m_bIsEos) return true;

	return false;
}

//-----------------------------------------------------------------------------
//
//	CMuxInterleaver class
//
//-----------------------------------------------------------------------------

CMuxInterleaver::CMuxInterleaver() :
	ev_abort(TRUE),
	ev_can_read(TRUE)
{
	offset = 0;
	last_gt = GetTickCount();

	ev_can_read.Reset();
	ev_abort.Reset();
}

CMuxInterleaver::~CMuxInterleaver()
{
	Clear();
}

void CMuxInterleaver::Clear()
{
	// delete all streams
	for (int i=0; i<m_streams.size(); i++) {
		CMuxInterStream	*stream = m_streams[i];
		delete stream;
	}
	m_streams.clear();
}

int CMuxInterleaver::AddStream(CMuxInterStream **stream)
{
	CMuxInterStream *st = new CMuxInterStream();
	m_streams.push_back(st);
	st->m_nIndex = (int)m_streams.size() - 1;

	*stream = st;
	return 0;
}

void CMuxInterleaver::SetAbort(BOOL abort)
{
	if (abort) {
		ev_abort.Set();
	} else {
		ev_abort.Reset();
	}
}

void CMuxInterleaver::FlushStream(int stream_no)
{
	CAutoLock	lck(&lock_queue);
	if (stream_no < 0 || stream_no >= m_streams.size()) return ;

	m_streams[stream_no]->Flush();

	// update the can_read event

	bool can_read = false;
	for (int i=0; i<m_streams.size(); i++) {
		CMuxInterStream	*stream = m_streams[i];
		if (stream->m_BufferQueue.size() > 0) {
			can_read = true;
		}
	}

	if (!can_read) ev_can_read.Reset();
}

void CMuxInterleaver::Flush()
{
	CAutoLock	lck(&lock_queue);

	// flush everything
	for (int i=0; i<m_streams.size(); i++) {
		m_streams[i]->Flush();
	}
	ev_can_read.Reset();
}

int CMuxInterleaver::Write(CMuxInterPacket *packet)
{
	CAutoLock	lck(&lock_queue);

	int strno = packet->m_nStreamIndex;

	if (strno < 0 || strno >= m_streams.size()) {
		// invalid stream index - delete the packet
		delete packet;
		return -1;
	}

	// append it into the queue
	CMuxInterStream *stream = m_streams[strno];
	int ret = stream->Write(packet);
	if (ret < 0) return ret;

	// mark as ready to read
	ev_can_read.Set();
	return ret;
}

int CMuxInterleaver::Read(CMuxInterPacket **packet, DWORD timeout)
{
	/*
		Synchronnous read operation with timeout.
		Result values:
			0: success
			-1: timeout
			-2: abort
	*/

	__int64		end_time = GetTime() + timeout;
	__int64		start_time, lowest_time;
	int			lowest_time_index = -1;
	int			ret;
	bool		can_deliver;
	bool		is_eos;
	HANDLE		events[] = { ev_can_read, ev_abort };
	DWORD		dw;

	do {
		// let's wait and see what happens
		dw = WaitForMultipleObjects(2, events, FALSE, (end_time - GetTime()));

		// abort ?
		if (ev_abort.Check()) return -2;
		if (dw == WAIT_TIMEOUT || GetTime() > end_time) return -1;

		{
			CAutoLock	lck(&lock_queue);

			/*
				Scan through all the streams and find the one with 
				lowest start time.
			*/

			lowest_time_index = -1;
			can_deliver = true;
			is_eos = true;

			for (int i=0; i<m_streams.size(); i++) {
				CMuxInterStream	*stream = m_streams[i];
				if (!stream->m_bActive) continue;

				// is at least one still working ?
				if (stream->m_bIsInterleaved && stream->IsEOS() == 0) {
					is_eos = false;
				}

				// is there some packet in the queue ?
				ret = stream->GetStartTime(&start_time);
				if (ret == 0) {
					
					if (lowest_time_index == -1) {
						// always take the first one
						lowest_time_index = i;
						lowest_time = start_time;
					} else {
						// take the others if they are lower
						if (start_time < lowest_time) {
							lowest_time_index = i;
							lowest_time = start_time;
						}
					}
				}			

				// all interleaved streams must have at least one
				// packet in the queue
				int	packet_count = stream->GetPacketCount();
				if (stream->m_bIsInterleaved && packet_count == 0) {
					can_deliver = false;

					// nothing to do - wait for some more packets
					ev_can_read.Reset();
				}
			}

			// end of stream - all interleaved streams report EOS
			if (is_eos) return -3;

			// now we've got one. so read it.
			if (lowest_time_index >= 0 && can_deliver) {
				return m_streams[lowest_time_index]->Read(packet);			
			}
		}
	} while (1);

	return 0;
}

int CMuxInterleaver::GetPacket(CMuxInterPacket **packet, int stream_no)
{
	ASSERT(packet);

	HANDLE	events[2] = { m_streams[stream_no]->m_evCanWrite, ev_abort };
	DWORD	dwret = WaitForMultipleObjects(2, events, FALSE, INFINITE);

	// are we being aborted ?
	if (ev_abort.Check()) {
		*packet = NULL;
		return -1;
	}

	// return a new packet instance with associated stream index
	*packet = new CMuxInterPacket();
	(*packet)->m_nStreamIndex = stream_no;

	return 0;
}

__int64 CMuxInterleaver::GetTime()
{
	// 32-bit overflow counter
	DWORD	time = GetTickCount();
	if (time < last_gt) {
		offset += 0x0100000000;
	}
	last_gt = time;
	__int64	ret = time + offset;
	return ret;
}

//-----------------------------------------------------------------------------
//
//	Muxing Subsystem
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
//	CBaseMuxOutputPin class
//
//-----------------------------------------------------------------------------
CBaseMuxOutputPin::CBaseMuxOutputPin(CBaseMux *parent, HRESULT *phr, LPCWSTR pName) :
	CBaseOutputPin(NAME("Mux Out Pin"), parent, &parent->lock_filter, phr, pName),
	mux(parent),
	position(NULL)
{
}

CBaseMuxOutputPin::~CBaseMuxOutputPin()
{
	if (position) {
		position->Release();
		position = NULL;
	}
}

STDMETHODIMP CBaseMuxOutputPin::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
	if (riid == IID_IMediaPosition || riid == IID_IMediaSeeking) {

		// create a PosPassThru object to work with the first input pin
		if (position == NULL) {
			HRESULT hr = CreatePosPassThru(GetOwner(), FALSE, (IPin*)mux->pins.front(), &position);
			if (FAILED(hr)) return hr;
		}

		return position->QueryInterface(riid, ppv);

	} else {
		return __super::NonDelegatingQueryInterface(riid, ppv);
	}
}

HRESULT CBaseMuxOutputPin::CheckMediaType(const CMediaType *pmt)
{
	// forward to mux class
	return mux->CheckOutputType(pmt);
}

HRESULT CBaseMuxOutputPin::SetMediaType(const CMediaType *pmt)
{
	// forward to mux class
	HRESULT	hr = __super::SetMediaType(pmt);
	if (FAILED(hr)) return hr;
	return mux->SetOutputType(pmt);
}

HRESULT CBaseMuxOutputPin::CompleteConnect(IPin *pReceivePin)
{
	// forward to mux class
	HRESULT hr = __super::CompleteConnect(pReceivePin);
	if (FAILED(hr)) return hr;
	return mux->CompleteOutputConnect(pReceivePin);
}

HRESULT CBaseMuxOutputPin::BreakConnect()
{
	// forward to mux class
	HRESULT hr = __super::BreakConnect();
	if (FAILED(hr)) return hr;
	return mux->BreakOutputConnect();
}

HRESULT CBaseMuxOutputPin::GetMediaType(int i, CMediaType *pmt)
{
	// forward to mux class
	return mux->GetMediaType(i, pmt);
}

HRESULT CBaseMuxOutputPin::DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *pProps)
{
	// forward to mux class
	return mux->DecideBufferSize(pAlloc, pProps);
}

STDMETHODIMP CBaseMuxOutputPin::Notify(IBaseFilter *pSender, Quality q)
{
	// right now we don't do anything ...
	return NOERROR;
}

//-----------------------------------------------------------------------------
//
//	CBaseMuxInputPin class
//
//-----------------------------------------------------------------------------

CBaseMuxInputPin::CBaseMuxInputPin(LPCTSTR pObjName, CBaseMux *pFilter, HRESULT *phr, LPCWSTR pName) :
	CBaseInputPin(pObjName, pFilter, &pFilter->lock_filter, phr, pName),
	mux(pFilter),
	index(0)
{
}

CBaseMuxInputPin::~CBaseMuxInputPin()
{
}

STDMETHODIMP CBaseMuxInputPin::BeginFlush()
{
	HRESULT	hr = __super::BeginFlush();
	if (FAILED(hr)) return hr;

	// nothing more so far....
	return NOERROR;
}

STDMETHODIMP CBaseMuxInputPin::EndFlush()
{
	HRESULT hr = __super::EndFlush();
	if (FAILED(hr)) return hr;

	// nothing more so far....
	return NOERROR;
}

STDMETHODIMP CBaseMuxInputPin::EndOfStream()
{
	// let the mux know
	return mux->EndOfStream(this);
}

STDMETHODIMP CBaseMuxInputPin::Receive(IMediaSample *pMediaSample)
{
	// let the mux know
	HRESULT hr = mux->ReceivePacket(this, pMediaSample);
	if (FAILED(hr)) return hr;

    return NOERROR;
}

HRESULT CBaseMuxInputPin::CheckMediaType(const CMediaType *pmt)
{
	// forward to mux class
	return mux->CheckInputType(pmt);
}

HRESULT CBaseMuxInputPin::SetMediaType(const CMediaType *pmt)
{
	HRESULT hr = __super::SetMediaType(pmt);
	if (FAILED(hr)) return hr;
	return mux->SetInputType(this, pmt);
}

HRESULT CBaseMuxInputPin::CompleteConnect(IPin *pReceivePin)
{
	HRESULT	hr = __super::CompleteConnect(pReceivePin);
	if (FAILED(hr)) return hr;

	// if all input pins are connected, the mux will add a new pin
	return mux->CompleteInputConnect(this, pReceivePin);
}

HRESULT CBaseMuxInputPin::BreakConnect()
{
	HRESULT hr = __super::BreakConnect();
	if (FAILED(hr)) return hr;

	// let the mux know
	return mux->BreakInputConnect(this);
}

//-----------------------------------------------------------------------------
//
//	CBaseMux class
//
//-----------------------------------------------------------------------------

CBaseMux::CBaseMux(LPCTSTR pName, LPUNKNOWN pUnk, HRESULT *phr, REFCLSID clsid) :
	CBaseFilter(pName, pUnk, &lock_filter, clsid, phr),
	abort(false),
	streaming(false),
	eos(false),
	output(NULL)
{
	bytes_written = 0;

	// note !!!! There is no pin being created in the base class !!!!!
	// you have to do it in the derived class
}

CBaseMux::~CBaseMux()
{
	// delete all pins we might have
	if (output) {
		delete output;
		output = NULL;
	}

	RemovePins();
}

int CBaseMux::GetPinCount()
{
	CAutoLock	lck(&lock_filter);
	return (int)pins.size() + (output != NULL ? 1 : 0);
}

CBasePin *CBaseMux::GetPin(int n)
{
	CAutoLock	lck(&lock_filter);

	// output pin
	if (output) {
		if (n == 0) return output;
		n -= 1;
	}

	// input pins
	if (n < 0 || n >= (int)pins.size()) return NULL;
	return pins[n];
}

int CBaseMux::CreatePin(CBaseMuxInputPin **pin, LPCWSTR name)
{
	// the derived class migh need to override this method
	// to provide custom pins

	if (!pin) return -1;

	HRESULT				hr = NOERROR;
	CBaseMuxInputPin	*new_pin = new CBaseMuxInputPin(L"MuxPin", this, &hr, name);

	if (!new_pin) {
		*pin = NULL;
		return -1;
	}

	*pin = new_pin;
	return 0;
}

int CBaseMux::AddPin()
{
	CBaseMuxInputPin	*pin = NULL;
	WCHAR				name[1024];

	// prepare a name for the pin
	_swprintf(name, L"In %d", (int)pins.size());

	int ret = CreatePin(&pin, name);
	if (ret < 0) return -1;

	// and append it into the list
	pin->index = (int)pins.size();		
	pins.push_back(pin);

	return 0;
}

int CBaseMux::RemovePins()
{
	// delete all input pins
	for (unsigned int i=0; i<pins.size(); i++) {
		CBaseMuxInputPin	*pin = pins[i];
		if (pin) {
			delete pin;
			pins[i] = NULL;
		}
	}
	pins.clear();

	return 0;
}


HRESULT CBaseMux::BreakInputConnect(CBaseMuxInputPin *pin)
{
	// to be overriden
	return NOERROR;
}

HRESULT CBaseMux::CompleteInputConnect(CBaseMuxInputPin *pin, IPin *pReceivePin)
{
	// if all pins are connected, we should add a new one
	CAutoLock		lck(&lock_filter);

	if (AllConnected()) {
		AddPin();
	}

	return NOERROR;
}

HRESULT CBaseMux::CompleteOutputConnect(IPin *pReceivePin)
{
	HRESULT		hr;
	
	// we require the output pin to expose IStream interface
	hr = pReceivePin->QueryInterface(IID_IStream, (void**)&outstream);
	if (FAILED(hr)) return E_FAIL;

	return NOERROR;
}

HRESULT CBaseMux::BreakOutputConnect()
{
	// get rid of the writer obejct
	outstream = NULL;
	return NOERROR;
}

HRESULT CBaseMux::GetMediaType(int pos, CMediaType *pmt)
{
	if (pos < 0) return E_INVALIDARG;
	if (pos > 0) return VFW_S_NO_MORE_ITEMS;

	// This is a very basic implementation.
	// If you need more specific options, you should consider
	// overrriding this method

	pmt->majortype = MEDIATYPE_Stream;
	pmt->subtype = MEDIASUBTYPE_None;
	pmt->formattype = MEDIASUBTYPE_None;
	pmt->lSampleSize = 512 * 1024;

	return NOERROR;
}

HRESULT CBaseMux::CheckInputType(const CMediaType *pmt)
{
	// by default we support everything
	return NOERROR;
}

HRESULT CBaseMux::SetInputType(CBaseMuxInputPin *pin, const CMediaType *pmt)
{
	// no big deal...
	return NOERROR;
}

HRESULT CBaseMux::CheckOutputType(const CMediaType *pmt)
{
	return NOERROR;
}

HRESULT CBaseMux::SetOutputType(const CMediaType *pmt)
{
	return NOERROR;
}

HRESULT CBaseMux::DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *pProp)
{
	ALLOCATOR_PROPERTIES	act;
	HRESULT					hr;

	// by default we do something like this...
	pProp->cbAlign		= 1;
	pProp->cBuffers	= 1;
	pProp->cbBuffer	= output->CurrentMediaType().lSampleSize;
	pProp->cbPrefix	= 0;

	hr = pAlloc->SetProperties(pProp, &act);
	if (FAILED(hr)) return hr;

	// make sure the allocator is OK with it.
	if ((pProp->cBuffers > act.cBuffers)  ||
		(pProp->cbBuffer > act.cbBuffer) ||
		(pProp->cbAlign > act.cbAlign)) 
		return E_FAIL;

	return NOERROR;
}

bool CBaseMux::AllDisconnected()
{
	// if we don't have any pins, we behave like all of them are connected
	// = we should create a new one
	if (pins.size() == 0) return true;

	for (unsigned int i=0; i<pins.size(); i++) {
		CBaseMuxInputPin	*pin = pins[i];
		if (pin->IsConnected()) return false;
	}

	return true;
}

bool CBaseMux::AllConnected()
{
	// if we don't have any pins, we behave like all of them are connected
	// = we should create a new one
	if (pins.size() == 0) return true;

	for (unsigned int i=0; i<pins.size(); i++) {
		CBaseMuxInputPin	*pin = pins[i];
		if (pin->IsConnected() == FALSE) return false;
	}

	return true;
}

int CBaseMux::OnStartStreaming()
{
	// to be overriden
	return 0;
}

int CBaseMux::OnStopStreaming()
{
	// to be overriden
	return 0;
}

int CBaseMux::OnMuxPacket(CMuxInterPacket *packet, CBaseMuxInputPin *inpin)
{
	// to be overriden
	return 0;
}

STDMETHODIMP CBaseMux::Stop()
{
    CAutoLock	lck(&lock_filter);

    // Make sure there really is a state change
    if (m_State == State_Stopped) return NOERROR;

	// let the interleaver know we're about to abort
	interleaver.SetAbort(TRUE);
	abort = true;

	CBaseFilter::Stop();

	// cancel the muxing thread
	if (ThreadExists()) {
		CallWorker(CBaseMux::MUX_CMD_EXIT);
		Close();
	}

    StopStreaming();

	eos = false;
    return NOERROR;
}

STDMETHODIMP CBaseMux::Pause()
{
    CAutoLock		lck(&lock_filter);
    FILTER_STATE	OldState = m_State;

    // Make sure there really is a state change
    if (m_State == State_Paused) return NOERROR;

	// cancel the abort flags
	interleaver.SetAbort(FALSE);
	abort = false;

	// Pause the base filter class
    HRESULT hr = CBaseFilter::Pause();
    if (FAILED(hr)) return hr;

    // Has our input pin been connected
	if (AllDisconnected()) return NOERROR;

    if (OldState == State_Stopped) {
		StartStreaming();

		// start the muxing thread
		if (!ThreadExists()) {
			Create();
			CallWorker(CBaseMux::MUX_CMD_RUN);
		}
    }
    return NOERROR;
}

STDMETHODIMP CBaseMux::Run(REFERENCE_TIME StartTime)
{
    CAutoLock		lck(&lock_filter);
    FILTER_STATE	OldState = m_State;

    if (m_State == State_Running) return NOERROR;

    // just run the filter
    HRESULT hr = CBaseFilter::Run(StartTime);
    if (FAILED(hr)) return hr;

    return NOERROR;
}

void CBaseMux::StartStreaming()
{
	// scan through the pins
	for (unsigned int i=0; i<pins.size(); i++) {
		CBaseMuxInputPin	*pin = pins[i];

		// by default there is no stream associated with the pin
		pin->stream = NULL;

		if (pin->IsConnected()) {

			CMuxInterStream	*stream = NULL;
			CMediaType		&mt = pin->CurrentMediaType();
			int				ret;

			// apped the new stream
			ret = interleaver.AddStream(&stream);
			if (ret == 0) {
				pin->stream = stream;
				pin->stream->m_bActive = true;
				pin->stream->m_pData = (void*)pin;	// associate it with the pin

				// ask the derived class if this stream should be interleaved
				pin->stream->m_bIsInterleaved = IsInterleaved(&mt);
			}
		}
	}

	// reset the output byte counter
	bytes_written = 0;

	// and then let the derived class do something about it
	OnStartStreaming();
}

void CBaseMux::StopStreaming()
{
	// let the derived class know
	OnStopStreaming();

	// and remove all streams from the interleaver
	interleaver.Clear();
	for (unsigned int i=0; i<pins.size(); i++) {
		CBaseMuxInputPin	*pin = pins[i];
		pin->stream = NULL;
	}
}

bool CBaseMux::IsInterleaved(CMediaType *pmt)
{
	// by default we consider everything interleaved
	return true;
}

int CBaseMux::ReceivePacket(CBaseMuxInputPin *pin, IMediaSample *sample)
{
	// the pin must have a valid stream
	if (!pin->stream) return -1;

	// now try to make an interleaver-packet out of it
	CMuxInterPacket	*packet;
	int				ret;

	HANDLE			ev[] = { pin->stream->m_evCanWrite, interleaver.ev_abort };
	DWORD			dw;

	// wait until the stream is writable, or we're being aborted
	while (true) {

		// if the pin is flushing - we're done
		if (pin->IsFlushing()) return -1;

		dw = WaitForMultipleObjects(2, ev, FALSE, 20);
		if (abort) return -1;
		if (dw == WAIT_TIMEOUT) continue;
		if (dw == WAIT_OBJECT_0) break;

		// it must have been an abort event
		return -1;
	}

	ret = interleaver.GetPacket(&packet, pin->stream->m_nIndex);
	if (ret < 0) return -1;

	// load the packet and interleave it
	packet->LoadFrom(sample);

	// bye bye
	ret = interleaver.Write(packet);
	if (ret < 0) {
		delete packet;
		return -1;
	}

	return 0;
}

int CBaseMux::EndOfStream(CBaseMuxInputPin *pin)
{
	// the stream must be valid
	if (!pin->stream) return -1;

	CMuxInterPacket	*packet;
	int				ret;

	HANDLE			ev[] = { pin->stream->m_evCanWrite, interleaver.ev_abort };
	DWORD			dw;

	// wait until the stream is writable, or we're being aborted
	while (true) {

		// if the pin is flushing - we're done
		if (pin->IsFlushing()) return -1;

		dw = WaitForMultipleObjects(2, ev, FALSE, 20);
		if (abort) return -1;
		if (dw == WAIT_TIMEOUT) continue;
		if (dw == WAIT_OBJECT_0) break;

		// it must have been an abort event
		return -1;
	}

	ret = interleaver.GetPacket(&packet, pin->stream->m_nIndex);
	if (ret < 0) return -1;

	// END OF STREAM
	packet->m_bIsEos = true;
	ret = interleaver.Write(packet);
	if (ret < 0) { delete packet; return -1; }

	return 0;
}


DWORD CBaseMux::ThreadProc()
{
	while (true) {
		int cmd = GetRequest();
		switch (cmd) {
		case CBaseMux::MUX_CMD_EXIT:		Reply(0); return -1;
		case CBaseMux::MUX_CMD_STOP:		Reply(0); break;
		case CBaseMux::MUX_CMD_RUN:
			{
				Reply(0);

				/**************************************************************
				**
				**	Muxing thread
				**
				***************************************************************/

				bool		done = false;
				while (!done && !CheckRequest(NULL)) {

					CMuxInterPacket		*packet = NULL;
					int					ret;

					/*
						0  - success
						-1 - timeout
						-2 - abort
						-3 - EOS
					*/
					ret = interleaver.Read(&packet, 40);
					switch (ret) {					
					case -2:		done = true; break;		// we're being aborted
					case -3:
						{
							if (output && output->IsConnected()) {
								output->DeliverEndOfStream();								
							} else {
								// yell as much as we can - we're done
								NotifyEvent(EC_COMPLETE, 0, 0);
							}
							done = true;
						}
						break;
					case 0:
						{
							// let's do something with the packet
							CMuxInterStream	 *stream = interleaver.m_streams[packet->m_nStreamIndex];
							CBaseMuxInputPin *inpin = (CBaseMuxInputPin*)stream->m_pData;

							OnMuxPacket(packet, inpin);
						}
						break;
					}

					// get rid of the packet
					if (packet) delete packet;
				}

			}
			break;
		default:
			{
				Reply(-1);
			}
			break;
		}
	}

	return 0;
}

int CBaseMux::WriteBinary(BYTE *data, int size)
{
	// no output pin ?
	if (!output || !output->IsConnected()) return -1;
	if (!outstream) return -1;
	if (size <= 0) return -1;

	// int64 position
	HRESULT				hr;
	LARGE_INTEGER		pos;
	pos.QuadPart = bytes_written;

	// seek there
	hr = outstream->Seek(pos, STREAM_SEEK_SET, NULL);
	if (FAILED(hr)) return -1;

	bytes_written += size;

	// and write the data
	hr = outstream->Write(data, size, NULL);
	if (FAILED(hr)) return -1;

	return 0;
}

