#include "StdAfx.h"
#include "LayedButton.h"

//CLayeredButton::CLayeredButton(void) 
//: m_pImage(NULL)
//{
//    m_bPressed = FALSE;
//    m_bHover = FALSE;
//}
//
//CLayeredButton::~CLayeredButton(void)
//{
//	if (m_pImage)
//	{
//		delete m_pImage;
//	}
//}

//LRESULT CLayeredButton::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
//{
//	LPCREATESTRUCT lpCreateStruct = (LPCREATESTRUCT)lParam;
//
//	m_nID = (UINT)lpCreateStruct->lpCreateParams;
//	//m_hWndOwner = lpCreateStruct->hwndParent;
//
//	// ModifyStyleEx(WS_CHILD | WS_CHILD, WS_EX_LAYERED | WS_POPUPWINDOW);
//	//CRect rc;
//	//SetWindowPos(HWND_TOPMOST, &rc, SWP_NOMOVE | SWP_NOSIZE);
//
//	_DoDraw();
//	return 0;
//}

//BOOL CLayeredButton::SubclassWindow(HWND hWnd)
//{
//    UINT nBS = ::GetWindowLong(hWnd, GWL_STYLE);
//    // Be sure that we have an owner drawn button
//    ::SetWindowLong(hWnd, GWL_STYLE, (nBS | BS_OWNERDRAW));
//    // Check if this is a checkbox
//    //m_bIsCheckBox = (nBS & BS_CHECKBOX) != 0;
//    // Check if this is the default button
//    //if (!m_bIsCheckBox)
//    //{
//    //    m_bIsDefault =  (nBS & BS_DEFPUSHBUTTON) != 0;
//    //}
//    return inherited::SubclassWindow(hWnd);
//}


//void CLayeredButton::SetImages(Gdiplus::Image* pImage, int nGrow, int nNormal, int nPushed, 
//							  int nFocusOrHover, int nDisabled)
//{
//	if (m_pImage)
//	{
//		delete m_pImage;
//	}
//	m_pImage = pImage->Clone();
//
//	m_nGrow = nGrow;
//	m_nImageCX = pImage->GetWidth() / nGrow;
//	m_nImageCY = pImage->GetHeight();
//
//	if(nNormal != -1)
//	{
//		m_nImage[_nImageNormal] = nNormal;
//	}
//		
//	if(nPushed != -1)
//	{
//		m_nImage[_nImagePushed] = nPushed;
//	}
//	if(nFocusOrHover != -1)
//	{
//		m_nImage[_nImageFocusOrHover] = nFocusOrHover;
//	}
//	if(nDisabled != -1)
//	{
//		m_nImage[_nImageDisabled] = nDisabled;
//	}
//}

//void CLayeredButton::OnMouseMove(UINT nFlags, CPoint point)
//{
//	if (::GetCapture() == m_hWnd)
//	{
//		POINT ptCursor(point);
//		ClientToScreen(&ptCursor);
//		RECT rect = { 0 };
//		GetWindowRect(&rect);
//		BOOL bPressed = ::PtInRect(&rect, ptCursor) ? TRUE : FALSE;
//		if(m_bPressed != bPressed)
//		{
//			m_bPressed = bPressed;
//			_DoDraw();
//		}
//	}
//	else if (!m_bHover)
//	{
//		m_bHover = TRUE;
//		_DoDraw();
//
//
//		TRACKMOUSEEVENT tme = { 0 };
//		tme.cbSize = sizeof(tme);
//		tme.dwFlags = TME_LEAVE;
//		tme.hwndTrack = m_hWnd;
//		_TrackMouseEvent(&tme);
//	}
//}

//int CLayeredButton::GetWidth(void)
//{
//	return m_nImageCX;
//}
//
//int CLayeredButton::GetHeight(void)
//{
//	return m_nImageCY;
//}

void CLayeredButton::DoPaint(CDCHandle dc)
{
    BOOL bRet = FALSE;
    Gdiplus::Status sts = Gdiplus::Ok;
    
    int cx = 0;
    int cy = 0;
    m_ImageList.GetIconSize(cx, cy);
    FTLASSERT(cx == 128);
    FTLASSERT(cy == 128);

    CPoint ptSrc(0, 0); 
    CPoint ptWindowScreenPosition(0, 0);
    CSize szWindow(cx, cy);

    //CRect rcWnd;
    //GetWindowRect(&rcWnd);
    //ptWindowScreenPosition.SetPoint(rcWnd.left, rcWnd.top);

    CDC dcMemory;
    API_VERIFY(NULL != dcMemory.CreateCompatibleDC(dc));

    //Bitmap bmp(cx, cy, PixelFormat32bppARGB);
    //Graphics graphic(&bmp);

    ATLASSERT(m_ImageList.m_hImageList != NULL);   // image list must be set
    ATLASSERT(m_nImage[0] != -1);                  // main bitmap must be set

    // set bitmap according to the current button state
    int nImage = -1;
    bool bHover = IsHoverMode();
    if(!IsWindowEnabled())
        nImage = m_nImage[_nImageDisabled];
    else if(m_fPressed == 1)
        nImage = m_nImage[_nImagePushed];
    else if((!bHover && m_fFocus == 1) || (bHover && m_fMouseOver == 1))
        nImage = m_nImage[_nImageFocusOrHover];
    if(nImage == -1)   // not there, use default one
        nImage = m_nImage[_nImageNormal];

    // draw the button image
    int xyPos = 0;
    if((m_fPressed == 1) && ((m_dwExtendedStyle & (BMPBTN_AUTO3D_SINGLE | BMPBTN_AUTO3D_DOUBLE)) != 0) && (m_nImage[_nImagePushed] == -1))
        xyPos = 1;

    API_VERIFY(m_ImageList.Draw(dcMemory, nImage, xyPos, xyPos, ILD_NORMAL));

    //if (m_bHover)
    //{
    //    GDIPLUS_VERIFY(graphic.DrawImage(&m_ImageList, (INT)0, 0, cx * m_nImage[_nImageFocusOrHover], 0, 
    //        cx, m_nImageCY, UnitPixel));
    //}
    //else if (m_bPressed)
    //{
    //    GDIPLUS_VERIFY(graphic.DrawImage(m_pImage, (INT)0, 0, cx * m_nImage[_nImagePushed], 0, 
    //        cx, m_nImageCY, UnitPixel));
    //}
    //else
    //{
    //    GDIPLUS_VERIFY(graphic.DrawImage(m_pImage, (INT)0, 0, cx * m_nImage[_nImageNormal], 0, 
    //        cx, m_nImageCY, UnitPixel));
    //}
    //HBITMAP hbmp = NULL;
    //Color color(0, 0, 0, 0);
    //GDIPLUS_VERIFY(bmp.GetHBITMAP(color, &hbmp));
    //HBITMAP hOldBmp = (HBITMAP)SelectObject(dcMemory, hbmp);
    
    BLENDFUNCTION blendPixelFunction = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
    API_VERIFY(::UpdateLayeredWindow(m_hWnd, dc, &ptWindowScreenPosition, &szWindow, dcMemory,
        &ptSrc, 0, &blendPixelFunction, ULW_ALPHA));
    //SelectObject(dcMemory, hOldBmp);
    //API_VERIFY(DeleteObject(hbmp));
}