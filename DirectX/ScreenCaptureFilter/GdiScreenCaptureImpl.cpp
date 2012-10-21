#include "StdAfx.h"
#include "GdiScreenCaptureImpl.h"
#ifndef DEBUG
//¸üÐÂ ftlDebug.h
#include <ftlDebug.h>
#endif

CGdiScreenCaptureImpl::CGdiScreenCaptureImpl()
{
	m_hScreenDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
	m_hMemDC = CreateCompatibleDC(m_hScreenDC);
	m_hBitmap = NULL;
	m_nWidth = 0;
	m_nHeight = 0;
}

CGdiScreenCaptureImpl::~CGdiScreenCaptureImpl(void)
{
	_ReleaseResource();
}

VOID CGdiScreenCaptureImpl::_ReleaseResource()
{
	if (m_hBitmap)
	{
		DeleteObject(m_hBitmap);
		m_hBitmap = NULL;
	}
	if (m_hMemDC)
	{
		DeleteDC(m_hMemDC);
		m_hMemDC = NULL;
	}
	if (m_hScreenDC)
	{
		DeleteDC(m_hScreenDC);
		m_hScreenDC = NULL;
	}
}

HBITMAP CGdiScreenCaptureImpl::CopyScreenToBitmap(LPRECT lpRect, BYTE* pData, BITMAPINFO* pHeader)
{
	//FTLTRACE(TEXT("CGdiScreenCaptureImpl::CopyScreenToBitmap, rect=%s, "),
	//	FTL::CFRectDumpInfo(*lpRect).GetConvertedInfo());
	BOOL bRet = FALSE;
	//HDC         hScrDC, hMemDC;         // screen DC and memory DC
	int         nX, nY, nX2, nY2;       // coordinates of rectangle to grab
	int         nWidth, nHeight;        // DIB width and height
	int         xScrn, yScrn;           // screen resolution

	// check for an empty rectangle
	if (IsRectEmpty(lpRect))
		return NULL;

	// create a DC for the screen and create
	// a memory DC compatible to screen DC
	//hScrDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
	//hMemDC = CreateCompatibleDC(hScrDC);

	// get points of rectangle to grab
	nX  = lpRect->left;
	nY  = lpRect->top;
	nX2 = lpRect->right;
	nY2 = lpRect->bottom;

	// get screen resolution
	xScrn = GetDeviceCaps(m_hScreenDC, HORZRES);
	yScrn = GetDeviceCaps(m_hScreenDC, VERTRES);

	//make sure bitmap rectangle is visible
	if (nX < 0)
		nX = 0;
	if (nY < 0)
		nY = 0;
	if (nX2 > xScrn)
		nX2 = xScrn;
	if (nY2 > yScrn)
		nY2 = yScrn;

	nWidth  = nX2 - nX;
	nHeight = nY2 - nY;
	if (m_nWidth != nWidth || m_nHeight != nHeight)
	{
		DeleteObject(m_hBitmap);
		// create a bitmap compatible with the screen DC
		m_hBitmap = CreateCompatibleBitmap(m_hScreenDC, nWidth, nHeight);
		if (m_hBitmap)
		{
			m_nWidth = nWidth;
			m_nHeight = nHeight;
		}
	}

	if (m_hBitmap)
	{
		// select new bitmap into memory DC
		HBITMAP hOldBitmap = (HBITMAP) SelectObject(m_hMemDC, m_hBitmap);
		// bitblt screen DC to memory DC
		API_VERIFY(BitBlt(m_hMemDC, 0, 0, nWidth, nHeight, m_hScreenDC, nX, nY, SRCCOPY));


		if(m_bMouseOverlay)
		{
			API_VERIFY(_OverlayMouseToScreen(m_hMemDC, lpRect));
		}


		// select old bitmap back into memory DC and get handle to
		// bitmap of the screen
		SelectObject(m_hMemDC, hOldBitmap);

		// Copy the bitmap data into the provided BYTE buffer
		GetDIBits(m_hMemDC /*hScrDC*/, m_hBitmap, 0, nHeight, pData, pHeader, DIB_RGB_COLORS);
	}

	// return handle to the bitmap
	return m_hBitmap;
}
