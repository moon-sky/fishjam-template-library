// DrawToolsView.cpp : implementation of the CDrawToolsView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "DrawToolsView.h"
#include "ZoomScrollMemDC.h"

BOOL CDrawToolsView::PreTranslateMessage(MSG* pMsg)
{
	if (GetSelection().size() == 1)
	{
		CDrawObject* pActiveObject = GetSelection().front();
		if (pActiveObject)
		{
			if(pActiveObject->PreTranslateMessage(pMsg))
			{
				return TRUE;
			}
		}
	}
	//FTLTRACE(TEXT("In CNCaptureView::PreTranslateMessage, msg=%d\n"), pMsg->message);
	if (WM_MOUSEWHEEL == pMsg->message)
	{
		LRESULT lResult = 0;
		return ProcessWindowMessage(pMsg->hwnd, pMsg->message, pMsg->wParam, pMsg->lParam, lResult, 0);
	}
	pMsg;
	return FALSE;

}

void CDrawToolsView::DoPaint(CDCHandle dc)
{
	//FUNCTION_BLOCK_TRACE(20);
	if ( m_bIsDrawing )
	{
		return;
	}

	m_bIsDrawing = TRUE;
	BOOL bRet = FALSE;

	CRect rcClientLogical;
	GetClientRect(rcClientLogical);
	dc.DPtoLP(rcClientLogical);

	rcClientLogical.InflateRect(4, 4);	//fix scroll brush display error

	{
		//make sure MemDC bitblt

		//CMemoryDC memDC(dc.m_hDC, rcClient);
		CZoomScrollMemDC memDC ( dc.m_hDC, rcClientLogical);
		PrepareDC(memDC.m_hDC);
		memDC.SetBrushOrg( -m_ptOffset.x , -m_ptOffset.y);

		//API_VERIFY(memDC.FillRect(&rcClip, m_brBackground));
		API_VERIFY(memDC.FillRect(&rcClientLogical, m_brBackground));

		if (m_pImage)
		{
			CRect rcImage(0, 0, m_pImage->GetWidth(), m_pImage->GetHeight());
			if (m_canvasImage.IsCanvasChanged(rcImage) || m_bImageChanged)
			{
				m_canvasImage.Release();
				API_VERIFY(m_canvasImage.Create(m_hWnd, rcImage.Width(), rcImage.Height()));

				m_bImageChanged = FALSE;
				API_VERIFY(m_pImage->Draw(m_canvasImage, rcImage));
			}

			//DrawObjects(dcDraw, FALSE, TRUE);
			{
				//#define USE_GDI_PLUS
#if 0
				Gdiplus::Graphics graphics(memDC);
				graphics.SetSmoothingMode(SmoothingModeInvalid);
				graphics.SetInterpolationMode(InterpolationModeHighQualityBilinear); //InterpolationModeInvalid);
				graphics.SetCompositingMode(CompositingModeSourceCopy);
				graphics.SetCompositingQuality(CompositingQualityInvalid);

				Gdiplus::Bitmap bmp(m_canvasImage.GetBitmap(), NULL);
				graphics.DrawImage(&bmp,m_rcDrawTarget.left,m_rcDrawTarget.top, 
					0,0, rcImage.Width(), rcImage.Height(),UnitPixel);

				//memDC.SetStretchBltMode(COLORONCOLOR); //HALFTONE);
				//bRet = memDC.StretchBlt(m_rcDrawTarget.left,m_rcDrawTarget.top,m_rcDrawTarget.Width(),
				//	m_rcDrawTarget.Height(), 
				//	m_canvasImage.GetMemoryDC(), 
				//	0,0, rcImage.Width(), rcImage.Height(), SRCCOPY);
#else
				//When Capture image size multiply by zoom bigger than 32767, GDI CAN NOT display the image
				memDC.SetStretchBltMode(COLORONCOLOR); //HALFTONE);

				bRet = memDC.BitBlt(
					rcClientLogical.left,  //m_rcDrawTarget.left + 
					rcClientLogical.top,  //m_rcDrawTarget.top + 
					rcClientLogical.Width(), rcClientLogical.Height(),
					m_canvasImage.GetMemoryDC(), 
					rcClientLogical.left, rcClientLogical.top, 
					SRCCOPY);
				if(!bRet)
				{
					FTLTRACE(TEXT("%s StretchBlt Failed, LastError=%d\n"), __FILE__LINE__, ::GetLastError());
				}
#endif 
			}
			DrawObjects(memDC.m_hDC);
			{
				CFMMTextDCGuard mmTextDCGuard(memDC);

				if (!m_pImage && m_pDefaultImage)
				{
					//no image ,just draw default image
					CRect rcClientDevice;
					GetClientRect(&rcClientDevice);
					CSize szDefaultImage(m_pDefaultImage->GetWidth(), m_pDefaultImage->GetHeight());
					CRect rcDefaultImageTarget = m_pCalcRect->GetPhotoRect(rcClientDevice, &szDefaultImage, 1.0f, FALSE, FALSE);
					NDGraphics::DrawGdiplusImage(memDC, m_pDefaultImage, rcDefaultImageTarget.left, rcDefaultImageTarget.top,
						rcDefaultImageTarget.Width(), rcDefaultImageTarget.Height());
				}
				DrawObjectsTracker(memDC.m_hDC);
			}
		}
	}


	//FTLTRACE(TEXT("DoPaint, m_rcDrawTarget=[%d,%d]x[%d,%d]\n"), m_rcDrawTarget.left, m_rcDrawTarget.top,
	//	m_rcDrawTarget.right, m_rcDrawTarget.bottom);
	//CClientDC clientDC(m_hWnd);

	m_bIsDrawing = FALSE;
}

int CDrawToolsView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	SetCurrentToolType(ttText);
	SetMsgHandled(FALSE);
	return 0;
}

void CDrawToolsView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDrawCanvas<CDrawToolsView>::OnLButtonDown(nFlags, point);
	//_SetSelectRectClipInfo(point);
	SetMsgHandled(FALSE);
}

void CDrawToolsView::OnMouseMove(UINT nFlags, CPoint point)
{

	CDrawCanvas<CDrawToolsView>::OnMouseMove(nFlags, point);
	SetMsgHandled(FALSE);
}

void CDrawToolsView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CDrawCanvas<CDrawToolsView>::OnLButtonUp(nFlags, point);
	_RefreshDrawCanvasInfo();
	SetMsgHandled(FALSE);
}

void CDrawToolsView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CDrawCanvas<CDrawToolsView>::OnKeyDown(nChar, nRepCnt, nFlags);
	
	_RefreshDrawCanvasInfo();
	SetMsgHandled(FALSE);
}

BOOL CNCaptureView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	//FTLTRACEEX(FTL::tlDetail,  TEXT("OnMouseWheel, nFlags=%d, zDelta=%d, pt=[%d,%d]\n"), nFlags, zDelta, pt.x, pt.y);

	//hold Control + MouseWheel
	if (KEY_DOWN(VK_CONTROL))
	{
		if (m_pImage)
		{
			float curZoomScale = GetZoomScale();
			if (zDelta > 0)
			{
				curZoomScale *= 1.1f;
			} 
			else
			{
				curZoomScale /= 1.1f;
			}
			BOOL bRet = SetZoom(curZoomScale);
			//BOOL bRet = NextZoom(zDelta > 0);
			if (bRet)
			{
				_SyncFrameScaleInfo();
			}
		}
		return TRUE;
	}
	SetMsgHandled(FALSE);

	return FALSE;
}

void CDrawToolsView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CDrawCanvas<CDrawToolsView>::OnChar(nChar, nRepCnt, nFlags);
	SetMsgHandled(FALSE);
}

void CDrawToolsView::_RefreshDrawCanvasInfo()
{
	CString strObjectSize;
	strObjectSize.Format(TEXT("Objects:%d"), m_allObjects.size());
	GetParent().SetWindowText(strObjectSize);
}

BOOL CDrawToolsView::OnEraseBkgnd(CDCHandle dc)
{
	//SetMsgHandled(FALSE);
	return FALSE;
}
void CDrawToolsView::ClientToDoc(CPoint* pPoint) 
{
	CWindowDC dc(m_hWnd);
	PrepareDC(dc.m_hDC);
	dc.DPtoLP(pPoint, 1);
}
void CDrawToolsView::ClientToDoc(CRect* pRect)
{
	CWindowDC dc(m_hWnd);
	PrepareDC(dc.m_hDC);
	dc.DPtoLP((LPPOINT)pRect, 2);
}

void CDrawToolsView::DocToClient(CPoint* pPoint)
{
	CWindowDC dc(m_hWnd);
	PrepareDC(dc.m_hDC);
	dc.LPtoDP(pPoint, 1);
}
void CDrawToolsView::DocToClient(CRect* pRect)
{
	CWindowDC dc(m_hWnd);
	PrepareDC(dc.m_hDC);
	dc.LPtoDP((LPPOINT)pRect, 2);
}

void CDrawToolsView::SetCurrentOffsetPoint(LPPOINT lpPoint)
{
	SetScrollOffset(lpPoint->x, lpPoint->y, TRUE);
}

void CDrawToolsView::GetCurrentOffsetPoint(LPPOINT lpPoint)
{
	GetScrollOffset(*lpPoint);
}

void CDrawToolsView::PrepareDC(CDCHandle dc)
{
	ATLASSERT(m_sizeAll.cx >= 0 && m_sizeAll.cy >= 0);
	dc.SetMapMode(MM_ANISOTROPIC);
	dc.SetWindowExt(m_sizeLogAll);
	dc.SetViewportExt(m_sizeAll);
	dc.SetViewportOrg(-m_ptOffset.x, -m_ptOffset.y);

	if (m_pImage)
	{
		CRect rcPaintAll(CPoint(0), m_sizeAll);	//device unit
		if (m_sizeClient.cx > m_sizeAll.cx)
		{
			rcPaintAll.right = m_sizeClient.cx;
		}
		if (m_sizeClient.cy > m_sizeAll.cy)
		{
			rcPaintAll.bottom = m_sizeClient.cy;
		}

		dc.DPtoLP((LPPOINT)(&rcPaintAll), 2);  //change to logical
		CRect rcImage(0, 0, m_pImage->GetWidth(), m_pImage->GetHeight());
		m_rcDrawTarget = _GetPhotoRect(rcPaintAll, &(rcImage.Size()));
	}

	dc.SetWindowOrg(-m_rcDrawTarget.left, -m_rcDrawTarget.top);
}
