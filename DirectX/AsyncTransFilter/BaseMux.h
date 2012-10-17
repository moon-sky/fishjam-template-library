//-----------------------------------------------------------------------------
//
//	MONOGRAM DirectShow BaseClasses
//
//	Author : Igor Janos
//  
//	http://www.monogrammultimedia.com
//
//-----------------------------------------------------------------------------
#pragma once

#include <list>
#include <queue>

//-----------------------------------------------------------------------------
//
//	Interleaver Subsystem
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
//	CMuxInterPacket class
//
//-----------------------------------------------------------------------------

/** \brief Buffer to hold interleaved data
*
*/
class CMuxInterPacket
{
public:
	REFERENCE_TIME			m_rtStart10mhz;				///< 10 Mhz start time
	REFERENCE_TIME			m_rtStop10mhz;				///< 10 Mhz stop time
	BYTE*					m_pData;						///< Packet payload
	int						m_nSize;						///< Actual data size

	// extended
	bool					m_bTimeStampEx;				///< Is extended time stamp information available?
	__int64					m_pts;						///< PTS value read from IMediaSampleEx interface
	__int64					m_dts;						///< DTS value read from IMediaSampleEx interface
	int						m_clock;					///< Clock value read from IMediaSampleEx interface

	bool					m_bSyncPoint;				///< Is this packet a sync point ?
	bool					m_bHasTime;					///< Is the time stamp information available ?
	int						m_nStreamIndex;				///< associated stream index
	bool					m_bIsEos;					///< Is this an end of stream packet ?

	enum {
		NONE		= 0,
		I_FRAME		= 1,
		P_FRAME		= 2,
		B_FRAME		= 3
	};

	int						m_nPictureType;				///< Picture coding type helper 

public:
	/** Constructor.
	*/
	CMuxInterPacket();
	/** Destructor.
	*/
	virtual ~CMuxInterPacket();

	// initialize from IMediaSample
	/** \brief Initialize from an incomming IMediaSample instance
	*
	*	\param sample IMediaSample object to initialize from
	*/
	void LoadFrom(IMediaSample *sample);
};

typedef std::list<CMuxInterPacket*>			CMuxInterPacketList;
typedef CMuxInterPacketList::iterator		CMuxInterPacketIterator;

//-----------------------------------------------------------------------------
//
//	CMuxInterStream class
//
//-----------------------------------------------------------------------------

/**	\brief Holds a queue of interleaved packets associated to one input stream.
*
*	This class contains a queue with limited size of data packets. Once the
*	queue is full it will not accept any more packets and waits for the muxing 
*	thread to read some packets.
*/
class CMuxInterStream
{
public:
	int						m_nIndex;					///< Index of the stream in the output multiplex
	bool					m_bIsInterleaved;				///< Is this stream interleaved ?
	bool					m_bActive;						///< Is it active (connected) ?
	int						m_nSize;						///< Upper limit of the sample queue size
	void					*m_pData;						///< Associated user data (pin)

	CMuxInterPacketList		m_BufferQueue;						///< Buffer queue
	CAMEvent				m_evCanWrite;				///< Is the queue writable ?
	//CCritSec				m_csLock;						///< Access lock

public:
	/** Constructor.
	*/
	CMuxInterStream();
	/** Destructor.
	*/
	virtual ~CMuxInterStream();

	/** \brief Appends one packet to the queue.
	*
	*	\param packet Interleaver packet ready for processing.
	*	\retval 0 Success.
	*/
	int Write(CMuxInterPacket *packet);
	/** \brief Extracts one packet from the queue.
	*
	*	\param packet Output packet. The caller is responsible for the deletion of this packet.
	*	\retval 0 Success.
	*	\retval -1 Queue is empty - no packet read.
	*	\retval -3 End of stream.
	*/
	int Read(CMuxInterPacket **packet);
	/** \brief Flush the packet queue.
	*
	*	This method also makes the queue writable.
	*/
	void Flush();

	/** \brief Returns the timestamp of the first packet.
	*
	*	\param time Timestamp of the first packet (in 10 MHz clock).
	*	\retval 0 Success.
	*	\retval -1 Queue is empty - no time stamp retrieved.
	*/
	int GetStartTime(REFERENCE_TIME *time);
	/** \brief Returns the number of queued packets.
	*/
	int GetPacketCount();
	/** \brief Returns true if the end of stream was reached.
	*/
	bool IsEOS();
};

typedef std::vector<CMuxInterStream*>			CMuxInterStreamArray;

//-----------------------------------------------------------------------------
//
//	CMuxInterleaver class
//
//-----------------------------------------------------------------------------

/** \brief Performs interleaving of incomming packets and delivers them 
*	sorted by time stamp.
*/
class CMuxInterleaver
{
public:
	CMuxInterStreamArray	m_streams;				///< List of input streams

	// eventy
	CCritSec				lock_queue;				///< Access lock
	CAMEvent				ev_abort;				///< Abort signal ?
	CAMEvent				ev_can_read;			///< Any new packets ?

	// 64-bit gettickcount
	__int64					offset;
	DWORD					last_gt;

	__int64 GetTime();								///< Timing helper. Returns time in milliseconds since the system was started.

public:
	/** Constructor.
	*/
	CMuxInterleaver();
	/** Destructor.
	*/
	virtual ~CMuxInterleaver();

	// Packet I/O
	/** \brief Writes the packet into the appropriate queue.
	*	\param packet Packet to be added to the proper queue
	*	\retval 0 Success.
	*	\retval -1 Error occured (e.g. invalid stream was specified)
	*/
	int Write(CMuxInterPacket *packet);
	/** \brief Synchronously reads one packet or aborts with timeout.
	*	This method allows the caller to specify a maximum time this method
	*	can take to return.
	*
	*	\param packet Interleaved packet with lowest time stamp.
	*	\param timeout Timeout value in milliseconds.
	*	\retval 0 Success.
	*	\retval -1 Timeout
	*	\retval -2 Abort operation in progress.
	*/
	int Read(CMuxInterPacket **packet, DWORD timeout=INFINITE);
	/** \brief Waits until a new packet can be written into the specified stream.
	*	\param packet Packet to write data into.
	*	\param stream_no Index of the stream to write into.
	*	\retval 0 Success.
	*	\retval -1 Abort in progress.
	*/
	int GetPacket(CMuxInterPacket **packet, int stream_no);

	// Stream I/O
	/** \brief Destroys all stream objects.
	*/
	void Clear();
	/** \brief Flushes all queues.
	*/
	void Flush();
	/** \brief Flushes a specific stream.
	*	\param stream_no Index of the stream to flush.
	*/
	void FlushStream(int stream_no);
	/** \brief Creates a new stream object with packet queue.
	*	\param stream Newly created stream object.
	*	\retval 0 Success.
	*	\retval -1 Error.
	*/
	int AddStream(CMuxInterStream **stream);
	/** \brief Signals the muxer to begin or end abort operation.
	*	\param abort Requested abort state.
	*/
	void SetAbort(BOOL abort);
};

//-----------------------------------------------------------------------------
//
//	Muxing Subsystem
//
//-----------------------------------------------------------------------------

class CBaseMux;
class CBaseMuxOutputPin;
class CBaseMuxInputPin;

//-----------------------------------------------------------------------------
//
//	CBaseMuxOutputPin class
//
//-----------------------------------------------------------------------------
/** \brief Basic muxer output pin base class.
*/
class CBaseMuxOutputPin : public CBaseOutputPin
{
public:
	
	CBaseMux		*mux;				///< Parent filter object
	IUnknown		*position;			///< PosPassThru object to allow downstream filters to seek

public:
	/** \brief Constructor
	*	\param parent Parent muxer filter.
	*	\param phr Constructor result value.
	*	\param pName Pin name.
	*/
	CBaseMuxOutputPin(CBaseMux *parent, HRESULT *phr, LPCWSTR pName);
	/** \brief Destructor.
	*/
	virtual ~CBaseMuxOutputPin();

	/** \brief Allows us to expose the IMediaSeeking interface to allow seeking.
	*/
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);

	// overriden
	/** \brief Forwards the call to the parent muxer filter.
	*/
	virtual HRESULT CheckMediaType(const CMediaType *pmt);
	/** \brief Forwards the call to the parent muxer filter.
	*/
	virtual HRESULT SetMediaType(const CMediaType *pmt);
	/** \brief Forwards the call to the parent muxer filter.
	*/
	virtual HRESULT CompleteConnect(IPin *pReceivePin);
	/** \brief Forwards the call to the parent muxer filter.
	*/
	virtual HRESULT BreakConnect();
	/** \brief Forwards the call to the parent muxer filter.
	*/
	virtual HRESULT GetMediaType(int i, CMediaType *pmt);
	/** \brief Forwards the call to the parent muxer filter.
	*/
	virtual HRESULT DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *pProps);

	// qual prop
	STDMETHODIMP Notify(IBaseFilter *pSender, Quality q);

	/** \brief Helper to expose our current media type.
	*/
	CMediaType &CurrentMediaType() { return m_mt; }
};



//-----------------------------------------------------------------------------
//
//	CBaseMuxInputPin class
//
//-----------------------------------------------------------------------------

/** \brief Basic muxer input pin base class.
*/
class CBaseMuxInputPin : public CBaseInputPin
{
public:

	CBaseMux			*mux;					///< Parent filter
	int					index;					///< Pin index
	CCritSec			lock_objects;			///< Access lock
	CMuxInterStream		*stream;				///< Associated interleaver stream object

public:
	/** \brief Constructor
	*	\param pObjName Name of this object (for debugging purposes).
	*	\param pFilter Parent muxer filter.
	*	\param phr Constructor result value.
	*	\param pName Pin name.
	*/
	CBaseMuxInputPin(LPCTSTR pObjName, CBaseMux *pFilter, HRESULT *phr, LPCWSTR pName);
	/** \brief Destructor.
	*/
	virtual ~CBaseMuxInputPin();
	
	// sample I/O
	/** \brief The beginning of flush operation.
	*/
    STDMETHODIMP BeginFlush();
	/** \brief The end of flush operation.
	*/
    STDMETHODIMP EndFlush();
	/** \brief Forwards the call to the parent muxer filter.
	*/
	STDMETHODIMP EndOfStream();
	/** \brief Forwards the call to the parent muxer filter.
	*/
    STDMETHODIMP Receive(IMediaSample *pMediaSample);
	
	// connection stuff
	/** \brief Forwards the call to the parent muxer filter.
	*/
	virtual HRESULT CheckMediaType(const CMediaType *pmt);
		/** \brief Forwards the call to the parent muxer filter.
	*/
	virtual HRESULT SetMediaType(const CMediaType *pmt);
	/** \brief Forwards the call to the parent muxer filter.
	*/
	virtual HRESULT CompleteConnect(IPin *pReceivePin);
	/** \brief Forwards the call to the parent muxer filter.
	*/
	virtual HRESULT BreakConnect();

    // Helpers
	/** \brief Helper to expose our current allocator.
	*/
    IMemAllocator inline *Allocator() const { return m_pAllocator; }
	/** \brief Helper to expose our current media type.
	*/
	CMediaType &CurrentMediaType() { return m_mt; }
};

typedef std::vector<CBaseMuxInputPin *>		CMuxInputPins;


//-----------------------------------------------------------------------------
//
//	CBaseMux class
//
//-----------------------------------------------------------------------------

/** \brief A base class for muxer filters.
*
*	This base class contains a muxing thread that reads interleaved packets
*	from the contained interleaver. Once the interleaver queues are filled
*	the input pins are blocked and all input streams proceed at the same
*	rate as the muxing thread reads interleaved samples.
*
*	It is necessary to understand that if one input stream stops delivering
*	samples without signalling the EndOfStream notification, the muxer
*	will block indefinitely.
*
*	You can find more information about this base class at the following link:
*	http://blog.monogram.sk/janos/2008/08/23/how-to-make-a-directshow-muxer-filter-part-1/
*/
class CBaseMux : public CBaseFilter, public CAMThread
{
public:

	enum {
		MUX_CMD_EXIT		= 0,
		MUX_CMD_STOP		= 1,
		MUX_CMD_RUN			= 2
	};

	CCritSec			lock_filter;			///< Access lock

	CMuxInputPins		pins;					///< List of input pins
	CBaseMuxOutputPin	*output;				///< One output pin
	CMuxInterleaver		interleaver;			///< The interleaver unit

	// internals
	bool				abort;					///< We don't want any more data
	bool				streaming;				///< Is streaming active ?
	bool				eos;					///< End of stream ?

	// output I/O
	CComPtr<IStream>	outstream;				///< Downstream IStream object
	__int64				bytes_written;			///< The total number of bytes written so far

public:
	/** \brief Constructor
	*	\param pName Name of this object (for debugging purposes).
	*	\param pUnk Owner object.
	*	\param phr Constructor result value.
	*	\param clsid Class ID for this filter.
	*/
	CBaseMux(LPCTSTR pName, LPUNKNOWN pUnk, HRESULT *phr, REFCLSID clsid);
	/** \brief Destructor
	*/
	virtual ~CBaseMux();

	// overriden from CBaseFilter
	/** \brief Returns the number of pins.
	*/
    virtual int GetPinCount();
	/** \brief Returns the instance of the specified pin.
	*/
    virtual CBasePin *GetPin(int n);

	// pin instances
	/** \brief Creates one input pin.
	*
	*	Derived classes might need to override this method to provide
	*	custom classes for input pins.
	*
	*	\param pin The newly created pin.
	*	\param name Recommended name to use for the pin.
	*	\retval 0 Success.
	*	\retval -1 Error occured.
	*/
	virtual int CreatePin(CBaseMuxInputPin **pin, LPCWSTR name);
	/** \brief Called when a new input pin is needed.
	*/
	virtual int AddPin();
	/** \brief Removes all input pins.
	*/
	virtual int RemovePins();

	// pin connection - input
	/** \brief Checks for supported media types.
	*
	*	By default this method returns NOERROR for any input media type.
	*	The derived class must surely override this behavior.
	*
	*	\param pmt Media type to check.
	*	\retval NOERROR This type is acceptable.
	*	\retval E_FAIL Otherwise.
	*/
	virtual HRESULT CheckInputType(const CMediaType *pmt);
	/** \brief Notifies that an input media type has been set (or changed).
	*
	*	By default this method returns NOERROR for any input media type.
	*	The derived class might override this behavior.
	*
	*	\param pmt Media type to check.
	*	\retval NOERROR This type is acceptable.
	*	\retval E_FAIL Otherwise.
	*/
	virtual HRESULT SetInputType(CBaseMuxInputPin *pin, const CMediaType *pmt);
	/** \brief Notifies that an input pin is to be disconnected.
	*	\param pin The pin that is to be disconnected.
	*	\retval NOERROR The pin can be disconnected.
	*	\retval E_FAIL The pin cannot be disconnected.
	*/
	virtual HRESULT BreakInputConnect(CBaseMuxInputPin *pin);
	/** \brief Notifies that an input connection has been completed.
	*
	*	By default this method checks if there are any input pins
	*	left disconnected. If all input pins are connected a new input
	*	pin is created.
	*
	*	\param pin Input pin that is being connected.
	*	\param pReceivePin Input pin's peer.
	*	\retval NOERROR Everything is fine.
	*	\retval E_FAIL The connection cannot be made.
	*/
	virtual HRESULT CompleteInputConnect(CBaseMuxInputPin *pin, IPin *pReceivePin);

	// pin connection - output
	/** \brief Checks for supported media types.
	*
	*	By default this method returns NOERROR for any output media type.
	*	The derived class must surely override this behavior.
	*
	*	\param pmt Media type to check.
	*	\retval NOERROR This type is acceptable.
	*	\retval E_FAIL Otherwise.
	*/
	virtual HRESULT CheckOutputType(const CMediaType *pmt);
	/** \brief Notifies that an output media type has been set (or changed).
	*
	*	By default this method returns NOERROR for any output media type.
	*	The derived class might override this behavior.
	*
	*	\param pmt Media type to check.
	*	\retval NOERROR This type is acceptable.
	*	\retval E_FAIL Otherwise.
	*/
	virtual HRESULT SetOutputType(const CMediaType *pmt);
	/** \brief Notifies that an output connection has been completed.
	*
	*	By default this method queries the peer pin for IStream interface
	*	to allow random access write operations. If the peer pin does not
	*	expose IStream interface, the connection will be rejected. If you
	*	derived class only delivers data sequentially you might need to override
	*	this method.
	*
	*	\param pReceivePin Output pin's peer.
	*	\retval NOERROR Everything is fine.
	*	\retval E_FAIL The connection cannot be made.
	*/
	virtual HRESULT CompleteOutputConnect(IPin *pReceivePin);
	/** \brief Notifies that the output pin is to be disconnected.
	*	\retval NOERROR The pin can be disconnected.
	*	\retval E_FAIL The pin cannot be disconnected.
	*/
	virtual HRESULT BreakOutputConnect();
	/** \brief Enumerates the list of output media types.
	*	
	*	The basic implementation returns a simple MEDIATYPE_Stream
	*	mediatype with no specific subtype nor format type.
	*	The buffer size is set to 512 KB.
	*
	*	If you need to deliver some more specific media types
	*	you will need to override this method.
	*
	*	\param pos Index of the requested media type.
	*	\param pmt Requested media type.
	*	\retval NOERROR Success.
	*	\retval VFW_S_NO_MORE_ITEMS The muxer does not support any more media types.
	*	\retval E_INVALIDARG Negative index requested.
	*/
	virtual HRESULT GetMediaType(int pos, CMediaType *pmt);
	/** \brief Specifies the buffer requirements.
	*
	*	By default this method asks for one buffer in the size specified
	*	by the current output pin's media type - this value is set to 512 KB.
	*
	*	\param pAlloc Allocator to use.
	*	\param pProp Properties to receive the buffer requirements.
	*	\retval NOERROR Success.
	*	\retval E_FAIL The allocator does not meet our requirements.
	*/
	virtual HRESULT DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *pProp);

	// state changes
	/** \brief Stops the muxer.
	*/
    STDMETHODIMP Stop();
	/** \brief Brings the muxer to active state.
	*/
    STDMETHODIMP Pause();
	/** \brief Brings the muxer to running state.
	*/
    STDMETHODIMP Run(REFERENCE_TIME StartTime);

	// helpers
	/** \brief Returns true when all input pins are connected.
	*/
	bool AllConnected();
	/** \brief Returns true when all input pins are not connected.
	*/
	bool AllDisconnected();

	// reception of packets
	/** \brief Receives a sample to be interleaved and muxer.
	*	
	*	The muxer delivers all incomming packets into the interleaver
	*	queues and waits for the muxing thread to perform the job.
	*
	*	\param pin Input pin that has received the sample.
	*	\param sample Incomming sample.
	*	\retval 0 Success.
	*	\retval -1 Error - the muxer cannot receive samples.
	*/
	int ReceivePacket(CBaseMuxInputPin *pin, IMediaSample *sample);
	/** \brief Notifies that a given pin will not deliver samples any more.
	*/
	int EndOfStream(CBaseMuxInputPin *pin);

	// to be overriden
	/** \brief Called when the muxing is about to begin.
	*
	*	The derived class might override this method to perform
	*	any last-minut configurations and to write the output
	*	headers.
	*	
	*	\retval 0 Success.
	*/
	virtual int OnStartStreaming();
	/** \brief Called when the muxing is about to end.
	*
	*	The derived class might override this method to
	*	write trailers to the output stream/file.
	*	
	*	\retval 0 Success.
	*/
	virtual int OnStopStreaming();
	/** \brief Called when a packet is ready to be muxed.
	*
	*	The derived class needs to override this method to
	*	implement a reasonable muxing functionality.
	*
	*	\param packet Interleaved packet.
	*	\param inpin Input pin that the packet was received by.
	*	\retval 0 Success.
	*/
	virtual int OnMuxPacket(CMuxInterPacket *packet, CBaseMuxInputPin *inpin);
	/** \brief Called to find out whether the media type needs to be
	*	processed by the interleaver or can be delivered to the muxer directly.
	*/
	virtual bool IsInterleaved(CMediaType *pmt);

	// streaming
	/** \brief Configures the interleaver and calls virtual OnStartStreaming method.
	*/
	void StartStreaming();
	/** \brief Deactivates the interleaver and calls virtual OnStopStreaming method.
	*/
	void StopStreaming();

	// muxing thread
	/** \brief The muxing itself...
	*/
	virtual DWORD ThreadProc();

	// binary output helper
	/** \brief Helper to write sequential output.
	*/
	virtual int WriteBinary(BYTE *data, int size);

};




