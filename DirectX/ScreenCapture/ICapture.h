#pragma once

class IImageCaptureResult
{
public:
	/*
		hbmp will be NULL when be canced or error.
	 */
	virtual int OnCaptureResult(HBITMAP hbmp, int nError) = 0;
};

class IImageCapture
{
public:
	virtual int Capture(IImageCaptureResult* captureResult) = 0;
	virtual void Abort(void) = 0;
};

template <class OptionT = int>
class CaptureBase :
	public IImageCapture
{
public:
	virtual void SetOption(OptionT& option)
	{
		m_option = option;
	}
	virtual void Abort(void)
	{
		// do nothing in default
	}

protected:
	OptionT m_option;
};

struct NRecordParam
{
	NRecordParam()
	{
		bIncludeCursor = FALSE;
		hWndPreview = NULL;
		maxRecordTime = 0;
		::SetRectEmpty(&rcRecord);
		strAudioInputName = TEXT("");
	}
	BOOL			bIncludeCursor;
	HWND			hWndPreview;
	LONGLONG		maxRecordTime;		//ms, 0 is unlimit
	RECT			rcRecord;
	CAtlString		strAudioInputName;

	//HWND			hWndTarget;
	//HWND			hWndOwner;
};

class IVideoRecordResult
{
public:
	virtual ~IVideoRecordResult() {}
public:
	virtual int OnVideoRecordResult(BSTR pszFilePath, HBITMAP hBmpSnap, int nError) = 0;
	//virtual int OnStatusChange(); -- Pos ?
};

class INVideoRecord
{
public:
	virtual ~INVideoRecord() { }
public:
	//virtual HRESULT Init(const NRecordParam& param) = 0;
	//virtual HRESULT Unint();
	//virtual HRESULT Start(IVideoRecordResult* pRecordResult) = 0;
	//virtual HRESULT Pause() = 0;
	//virtual HRESULT Resume() = 0;
	////virtual HRESULT OnStatusChange(RecordStatus oldStatus, RecordStatus newStatus);
	//virtual HRESULT Stop(BOOL bCancel) = 0;
};