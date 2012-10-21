#pragma once
#include "ScreenCaptureImpl.h"

class CGdiScreenCaptureImpl : public CScreenCaptureImplBase
{
public:
	CGdiScreenCaptureImpl();
	virtual ~CGdiScreenCaptureImpl(void);

public:
	//IScreenCaptureImpl
	virtual HBITMAP	CopyScreenToBitmap(LPRECT lpRect, BYTE* pData, BITMAPINFO* pHeader);
protected:
	HDC m_hScreenDC;
	HDC m_hMemDC;
	HBITMAP m_hBitmap;
	INT		m_nWidth;
	INT		m_nHeight;
protected:
	VOID	_ReleaseResource();
};
