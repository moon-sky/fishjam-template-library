#pragma once

#include <strmif.h>
#include <control.h>
#include <uuids.h>


#include <ftlDirectx.h>
#include <ftlSharePtr.h>

#include "../ICapture.h"

class VideoRecordOption
{
public:
	VideoRecordOption(void) 
		: m_pImgCursor(NULL)
		, m_pImgMagnifier(NULL)
		, m_pImgBtnHide(NULL)
		, m_pimgBtnShow(NULL)
		, m_pImgBtnStart(NULL)
		, m_pImgBtnPause(NULL)
		, m_pImgBtnStop(NULL)
		, m_pImgBtnOption(NULL)
		, m_pImgBtnClose(NULL)
	{

	}
	NRecordParam	m_recordParam;
	CImage* m_pImgCursor;
	Gdiplus::Image* m_pImgMagnifier;
	CImage* m_pImgBtnHide;
	CImage *m_pimgBtnShow;
	CImage* m_pImgBtnStart;
	CImage* m_pImgBtnPause;
	CImage* m_pImgBtnStop;
	CImage* m_pImgBtnOption;
	CImage* m_pImgBtnClose;
};

enum RecordStatus
{
	rcNoInit = 0,
	rcInited,
	rcRunning,
	rcPaused,
	rcStopped,
};

class IRecordGraphNotify
{
public:
	//TODO:Error ?
	virtual BOOL OnNotifyEvent(long EventCode, LONG_PTR EventParam1,LONG_PTR EventParam2) = 0;
};



#include <initguid.h>
//{4697AC37-FF57-47b6-ABD8-E394D44696D5}
DEFINE_GUID(CLSID_NaverScreenCapture,
			0x4697ac37, 0xff57, 0x47b6, 0xab, 0xd8, 0xe3, 0x94, 0xd4, 0x46, 0x96, 0xd5);

DEFINE_GUID(IID_IScreenCaptureCfg, 
			0x47a1f132, 0x7b43, 0x414c, 0x90, 0xaf, 0xce, 0xb2, 0x5e, 0x7f, 0xbd, 0x6d);

DECLARE_INTERFACE_(IScreenCaptureCfg, IUnknown)
{
	STDMETHOD(SetCaptureRect) (IN int nLeft, IN int nTop, IN int nWidth, IN int nHeight) PURE;
	STDMETHOD(SetDefaultCaptureRect) () PURE;
	STDMETHOD(SetFrameRate) (IN int nFPS) PURE;
	STDMETHOD(SetMouseOverlay) (IN bool bOverlay) PURE;
};
