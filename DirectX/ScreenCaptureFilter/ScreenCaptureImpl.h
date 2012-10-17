#pragma once

class IScreenCaptureImpl
{
public:
	virtual ~IScreenCaptureImpl() {}
public:
	virtual BOOL GetMouseOverLay() = 0;
	virtual VOID SetMouseOverLay(BOOL bMouseOverLay) = 0;
	virtual HBITMAP	CopyScreenToBitmap(LPRECT lpRect, BYTE* pData, BITMAPINFO* pHeader) = 0;
};

class CScreenCaptureImplBase : public IScreenCaptureImpl
{
public:
	CScreenCaptureImplBase(INT nWidth, INT nHeight, INT nBitCount)
		:m_nWidth(nWidth)
		,m_nHeight(nHeight)
		,m_nBitCount(nBitCount)
	{
		m_bMouseOverlay = FALSE;
	}
	virtual BOOL GetMouseOverLay();
	virtual VOID SetMouseOverLay(BOOL bMouseOverLay);
protected:
	BOOL	m_bMouseOverlay;
	INT		m_nWidth;
	INT		m_nHeight;
	INT		m_nBitCount;
	
protected:
	virtual BOOL _OverlayMouseToScreen(HDC hdc, LPRECT lpRect);
};