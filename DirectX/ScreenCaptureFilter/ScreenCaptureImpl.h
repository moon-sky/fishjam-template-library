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
	CScreenCaptureImplBase()
	{
		m_bMouseOverlay = FALSE;
	}
	virtual BOOL GetMouseOverLay();
	virtual VOID SetMouseOverLay(BOOL bMouseOverLay);
protected:
	BOOL	m_bMouseOverlay;
	
protected:
	virtual BOOL _OverlayMouseToScreen(HDC hdc, LPRECT lpRect);
};