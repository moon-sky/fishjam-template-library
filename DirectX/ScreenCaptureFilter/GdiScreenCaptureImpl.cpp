#include "StdAfx.h"
#include "GdiScreenCaptureImpl.h"
#ifndef DEBUG
//¸üÐÂ ftlDebug.h
#include <ftlDebug.h>
#endif

CGdiScreenCaptureImpl::CGdiScreenCaptureImpl(INT nWidth, INT nHeight, INT nBitCount)
:CScreenCaptureImplBase(nWidth, nHeight, nBitCount)
{
	m_hScreenDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
	m_hMemDC = CreateCompatibleDC(m_hScreenDC);
	m_hBitmap = CreateCompatibleBitmap(m_hScreenDC, m_nWidth, FTL_ABS(m_nHeight));
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
	HDC         hScrDC, hMemDC;         // screen DC and memory DC
	HBITMAP     hBitmap, hOldBitmap;    // handles to deice-dependent bitmaps
	int         nX, nY, nX2, nY2;       // coordinates of rectangle to grab
	int         nWidth, nHeight;        // DIB width and height
	int         xScrn, yScrn;           // screen resolution

	// check for an empty rectangle
	if (IsRectEmpty(lpRect))
		return NULL;

	// create a DC for the screen and create
	// a memory DC compatible to screen DC
	hScrDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
	hMemDC = CreateCompatibleDC(hScrDC);

	// get points of rectangle to grab
	nX  = lpRect->left;
	nY  = lpRect->top;
	nX2 = lpRect->right;
	nY2 = lpRect->bottom;

	// get screen resolution
	xScrn = GetDeviceCaps(hScrDC, HORZRES);
	yScrn = GetDeviceCaps(hScrDC, VERTRES);

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

	// create a bitmap compatible with the screen DC
	hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);
	if (hBitmap)
	{

	// select new bitmap into memory DC
	hOldBitmap = (HBITMAP) SelectObject(hMemDC, hBitmap);

	// bitblt screen DC to memory DC
	API_VERIFY(BitBlt(hMemDC, 0, 0, nWidth, nHeight, hScrDC, nX, nY, SRCCOPY));


	if(m_bMouseOverlay)
	{
			API_VERIFY(_OverlayMouseToScreen(hMemDC, lpRect));
	}


	// select old bitmap back into memory DC and get handle to
	// bitmap of the screen
		SelectObject(hMemDC, hOldBitmap);

	// Copy the bitmap data into the provided BYTE buffer
		GetDIBits(hMemDC /*hScrDC*/, hBitmap, 0, nHeight, pData, pHeader, DIB_RGB_COLORS);
	}

	// clean up
	DeleteDC(hScrDC);
	DeleteDC(hMemDC);

	// return handle to the bitmap
	return hBitmap;
}
