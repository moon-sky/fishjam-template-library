#ifndef _ZOOM_SCROLL_MEMDC_H_
#define _ZOOM_SCROLL_MEMDC_H_

//ATL::CMemoryDC has bug for Scroll + Zoom

class CZoomScrollMemDC : public CDC 
{
public:
	// Data members
	HDC m_hDCOriginal;
	CRect  m_rcPaint;		//logical unit
	CBitmap m_bmp;
	HBITMAP m_hBmpOld;

	// Constructor/destructor
	CZoomScrollMemDC(HDC hDC, RECT& rcPaint) 
		: m_hDCOriginal(hDC)
		, m_hBmpOld(NULL)
	{
		BOOL bRet = FALSE;
		m_rcPaint = rcPaint;

		API_VERIFY( NULL != CreateCompatibleDC(m_hDCOriginal));

		//change to device and create bitmap
		API_VERIFY(::LPtoDP(m_hDCOriginal, (LPPOINT)&m_rcPaint, 2));
		ATLASSERT(m_hDC != NULL);
		API_VERIFY(NULL != m_bmp.CreateCompatibleBitmap(m_hDCOriginal, m_rcPaint.Width(), m_rcPaint.Height()));
		ATLASSERT(m_bmp.m_hBitmap != NULL);
		m_hBmpOld = SelectBitmap(m_bmp);
		//SetMapMode(::GetMapMode(m_hDCOriginal));
		API_VERIFY(::DPtoLP(m_hDCOriginal, (LPPOINT)&m_rcPaint, 2));

		//SetWindowOrg(m_rcClip.left, m_rcClip.top);  //logical
		//SetViewportOrg(-m_rcClip.left, -m_rcClip.top);	//device
	}

	~CZoomScrollMemDC()
	{
		::BitBlt(m_hDCOriginal, m_rcPaint.left, m_rcPaint.top, 
			m_rcPaint.Width(), m_rcPaint.Height(), 
			m_hDC, m_rcPaint.left, m_rcPaint.top, SRCCOPY);
		SelectBitmap(m_hBmpOld);
	}
};

#endif	//_ZOOM_SCROLL_MEMDC_H_
