#include "stdafx.h"
#include "nCaptureView.h"
#include "nCaptureDoc.h"
#include "NCaptureApplication.h"
#include "CapImageObj.h"
#include "ZoomScrollMemDC.h"
#include "MainFrm.h"
#include "../Capture/StringResouceMgr.h"
#include <ftlGdi.h>

#include <SilverlightCpp.h>
using namespace SilverlightCpp;
#include <SilverlightExCpp.h>
using namespace SilverlightExCpp;


const float CNCaptureView::s_FixedZoomScales[ZOOM_COUNT] = 
{
	0.05f,
	0.10f,
	0.20f,
	0.25f,
	0.5f,
	0.6f,
	0.7f,
	0.8f,
	0.9f,
	1.0f,
	2.0f,
	3.0f,
	4.0f,
	6.0f,
	8.0f,
	10.0f,
	15.0f
};

const int CNCaptureView::s_NormalZoomIndex = 9;

CNCaptureView::CNCaptureView()
{
	m_bInited = FALSE;
	m_bIsDrawing = FALSE;
	m_pImage = NULL;
	m_pDefaultImage = NULL;
	m_pCalcRect = CNPVPhotoCalcRect::GetPhotoCalcRectObject(CNPVPhotoCalcRect::E_ZOOM_ORIGINAL, NULL);
	m_bImageChanged = TRUE;
	m_bClipCursor = FALSE;
	m_bTrackMouse = FALSE;
	//m_iFixedZoomIndex = s_NormalZoomIndex;
	m_fLastZoomFactor = s_FixedZoomScales[s_NormalZoomIndex];
	m_hWndNotify = NULL;
	SetZoomScaleMin(s_FixedZoomScales[0]);

	m_rcDrawTarget.SetRectEmpty();
}

CNCaptureView::~CNCaptureView()
{
	m_pImage = NULL;
	SAFE_DELETE(m_pDefaultImage);
	SAFE_DELETE(m_pCalcRect);
}

BOOL CNCaptureView::Initialize()
{
	BOOL bRet = FALSE;
	if (m_brBackground.m_hBrush)
	{
		API_VERIFY(m_brBackground.DeleteObject());
	}

	CWindowDC dcOwner(m_hWnd);
	CDC dcMemory;
	dcMemory.CreateCompatibleDC(dcOwner);

	SilverlightCpp::ZipManager* pZipManager = SilverlightCpp::ZipManager::get_Current();
	FTLASSERT(pZipManager); 

	CBitmap bmpBackGround;
	CImage* pImgBgPattern  = pZipManager->LoadCImage(_T("/Assets/Images/Main/CaptureView/bg_pattern.png")); // NS
	FTLASSERT(pImgBgPattern);
	if (pImgBgPattern)
	{
		bmpBackGround.CreateCompatibleBitmap(dcOwner, pImgBgPattern->GetWidth(), pImgBgPattern->GetHeight());
		CBitmapHandle holdBitmap = dcMemory.SelectBitmap(bmpBackGround);
		API_VERIFY(pImgBgPattern->BitBlt(dcMemory, 0, 0, SRCCOPY));
		m_brBackground.CreatePatternBrush(bmpBackGround);
		dcMemory.SelectBitmap(holdBitmap);
	}
	//else
	//{
	//	bmpBackGround.CreateCompatibleBitmap(dcOwner, 10, 10);
	//	dcMemory.FillSolidRect(0, 0, 5, 5, IMAGE_BACKGROUND_LTGRAY_COLOR);
	//	dcMemory.FillSolidRect(0, 5, 5, 5, IMAGE_BACKGROUND_GRAY_COLOR);
	//	dcMemory.FillSolidRect(5, 0, 5, 5, IMAGE_BACKGROUND_GRAY_COLOR);
	//	dcMemory.FillSolidRect(5, 5, 5, 5, IMAGE_BACKGROUND_LTGRAY_COLOR);
	//}

	//CImage* pDefaultImage = pZipManager->LoadCImage(_T("/Assets/Images/Main/CaptureView/bg_default_img.png")); // NS
	//FTLASSERT(pDefaultImage);
	//if (pDefaultImage)
	//{
	//	m_pDefaultImage = new NDGraphics::CGDIPImage;
	//	m_pDefaultImage->Load(pDefaultImage, ImageFormatPNG);
	//	//API_VERIFY(m_canvasDefaultImage.Create(NULL, m_pDefaultImage->GetWidth(), m_pDefaultImage->GetHeight(), 32));
	//}

	m_strMousePosInfo = CStringResouceMgr::Instance()->GetStringData(TEXT("NCAPTURE_IMAGE_INFO")).c_str();
	m_strMouseSelectInfo = CStringResouceMgr::Instance()->GetStringData(TEXT("NCAPTURE_IMAGE_SELECT_INFO")).c_str();

	_EventHook();

	//API_VERIFY( NULL != m_brBackGround.CreateHatchBrush(HS_CROSS, RGB(127,127,127)));
	m_bInited = TRUE;
	//CRect rcClient;
	//GetClientRect(rcClient);
	//InvalidateRect(rcClient, TRUE);
	//_CalcImageSize();


	return bRet;
}

VOID CNCaptureView::Finalize()
{
	m_bInited = FALSE;
	_EventUnHook();
}

BOOL CNCaptureView::PreTranslateMessage(MSG* pMsg)
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

BOOL CNCaptureView::_SetSelectRectClipInfo(const CPoint& point)
{
	BOOL bRet = FALSE;
	//if (ttSelection == m_nCurToolType && m_pSelectRect && this->IsSelected(m_pSelectRect))
	//{
	//	m_bClipCursor = TRUE;
	//	CRect rcSelect = m_pSelectRect->GetPosition();
	//	DocToClient(&rcSelect);

	//	CRect rcClipTarget = m_rcDrawTarget;
	//	rcClipTarget.OffsetRect( -rcClipTarget.TopLeft());
	//	DocToClient(&rcClipTarget);
	//	ClientToScreen(&rcClipTarget);

	//	if (smMove == m_nCurrentSelectMode )
	//	{
	//		rcClipTarget.right -= rcSelect.Width() ;
	//		rcClipTarget.bottom -= rcSelect.Height();
	//		rcClipTarget.OffsetRect(point.x - rcSelect.left, point.y - rcSelect.top);
	//	}
	//	else if(smSize == m_nCurrentSelectMode || smNetSelectSize == m_nCurrentSelectMode)
	//	{
	//		//now do nothing
	//	}
	//	rcClipTarget.right += 1;
	//	rcClipTarget.bottom += 1;

	//	//FTLTRACE(TEXT("rcClipTarget= [%d,%d], [%d,%d], {%dx%d}\n"), rcClipTarget.left, rcClipTarget.top,
	//	//	rcClipTarget.right, rcClipTarget.bottom, 
	//	//	rcClipTarget.Width(), rcClipTarget.Height());
	//	API_VERIFY(::ClipCursor(&rcClipTarget));
	//}
	return bRet;
}

void CNCaptureView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_pImage)
	{
		CDrawCanvas<CNCaptureView>::OnLButtonDown(nFlags, point);
		_SetSelectRectClipInfo(point);
	}
	SetMsgHandled(FALSE);
}

CRect CNCaptureView::_GetSelectRect(const CPoint& ptLogical)
{
	CRect rcSelect;
	rcSelect.SetRectEmpty();

	//already has select rect
	DrawObjectList& drawObjList = GetSelection();
	if (drawObjList.size() == 1)
	{
		CDrawObject* pDrawObj = drawObjList.front();
		FTLASSERT(pDrawObj);
		if (dotSelectRect == pDrawObj->GetDrawObjType())
		{
			rcSelect = pDrawObj->GetPosition();
			if (!rcSelect.PtInRect(ptLogical) && !IsCapture())
			{
				rcSelect.SetRectEmpty();
			}
		}
	}
	else
	{
		if (IsCapture())
		{
			rcSelect.SetRect(GetMouseDownLogicalPoint(), GetMouseLastLogicalPoint());
			rcSelect.NormalizeRect();
		}
	}

	CRect rcImage(0, 0, m_pImage->GetWidth(), m_pImage->GetHeight());
	rcSelect.IntersectRect(rcSelect, rcImage);

	//FTLTRACE(TEXT("ptLogical=[%d,%d], rcSelect=[%d,%d]-[%d,%d]\n"),
	//	ptLogical.x, ptLogical.y , rcSelect.left, rcSelect.top, rcSelect.right, rcSelect.bottom);

	return rcSelect;
}

void CNCaptureView::_UpdateMousePosInfo(const CPoint& point)
{
	if (m_pImage && !m_strMousePosInfo.IsEmpty())
	{
		CPoint ptLogical = point;
		ClientToDoc(&ptLogical);

		CMainFrame* pMainFrame = CNCaptureApplication::Instance()->GetMainFrame();
		FTLASSERT(pMainFrame);
		CString strInfo = TEXT("");

		CPoint ptMousePos(0, 0);
		ptLogical.Offset(m_rcDrawTarget.TopLeft());
		if (m_rcDrawTarget.PtInRect(ptLogical))
		{
			ptMousePos = ptLogical - m_rcDrawTarget.TopLeft();
		}

		//FTLTRACE(TEXT("OnMouseMove,ptMousePos=[%d,%d], rcDrawTarget=[%d,%d]\n"),
		//	ptMousePos.x, ptMousePos.y,
		//	m_rcDrawTarget.TopLeft().x, m_rcDrawTarget.TopLeft().y);
		STATUSBARINFO stStatusBarInfo;
		stStatusBarInfo.rcSelect = _GetSelectRect(ptMousePos);
		stStatusBarInfo.ptMouse = ptMousePos;
		stStatusBarInfo.csImageSize.SetSize(m_pImage->GetWidth(), m_pImage->GetHeight());
		stStatusBarInfo.bEnable = TRUE;
		if (pMainFrame)
		{
			pMainFrame->SetStatusInfo(&stStatusBarInfo);
		}


		//CRect rcSelect = _GetSelectRect(ptMousePos);
		//if (stStatusBarInfo.rcSelect.IsRectEmpty())
		//{
		//	strInfo.Format(_T("%d, %d, %d, %d"),
		//		m_pImage->GetWidth(), m_pImage->GetHeight(),
		//		stStatusBarInfo.ptMouse.x, stStatusBarInfo.ptMouse.y);
		//}
		//else
		//{
		//	strInfo.Format(_T("%d, %d, %d, %d %d"),
		//		m_pImage->GetWidth(), m_pImage->GetHeight(),
		//		rcSelect.left, rcSelect.top, rcSelect.Width(), rcSelect.Height());
		//}
		//tstring str = (LPCTSTR)strInfo;
		//pMainFrame->SetStatusInfo(str);
	}
}

void CNCaptureView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bTrackMouse)
	{
		m_bTrackMouse = TRUE;
		TRACKMOUSEEVENT mouseEvent;
		mouseEvent.cbSize = sizeof ( mouseEvent );
		mouseEvent.dwFlags = TME_LEAVE ;
		mouseEvent.hwndTrack = m_hWnd;
		mouseEvent.dwHoverTime = HOVER_DEFAULT;
		m_bTrackMouse = TrackMouseEvent(&mouseEvent);
		//FTLTRACE(TEXT("TrackMouseEvent, Result=%d\n"), m_bTrackMouse);
	}
	if (m_pImage)
	{
		_UpdateMousePosInfo(point);
		CDrawCanvas<CNCaptureView>::OnMouseMove(nFlags, point);
		if (::GetCapture() == m_hWnd)
		{
			_SetSelectRectClipInfo(point);
		}
	}
	SetMsgHandled(FALSE);
}

void CNCaptureView::OnMouseLeave()
{
	//FTLTRACE(TEXT("CNCaptureView::OnMouseLeave\n"));
	m_bTrackMouse = FALSE;
	SetMsgHandled(FALSE);
}

void CNCaptureView::OnLButtonUp(UINT nFlags, CPoint point)
{
	BOOL bHandle = FALSE;
	if (m_pImage)
	{
		bHandle = CDrawCanvas<CNCaptureView>::OnLButtonUp(nFlags, point);
		if (m_bClipCursor)
		{
			ClipCursor(NULL);
			m_bClipCursor = FALSE;
		}
	}
	SetMsgHandled(bHandle);
}

void CNCaptureView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (m_pImage)
	{
		if (!CDrawCanvas<CNCaptureView>::OnLButtonDblClk(nFlags, point))
		{
			if (!FTL::IsSameNumber(GetZoomScale(), s_FixedZoomScales[s_NormalZoomIndex], 0.01f))
			{
				m_fLastZoomFactor = GetZoomScale();
				SetZoom(s_FixedZoomScales[s_NormalZoomIndex]);
			}
			else
			{
				SetZoom(m_fLastZoomFactor);
			}
		}
	}
}

//WTL.CScrollImpl.OnHScroll/OnVScroll can not handle SB_THUMBPOSITION and SB_THUMBTRACK correctly,
//so must SetScrollInfo before CScrollImpl::DoScroll
void CNCaptureView::OnHScroll(UINT nSBCode, UINT /*nPos*/, CScrollBar pScrollBar)
{
	if (SB_THUMBPOSITION == nSBCode || SB_THUMBTRACK == nSBCode)
	{
		if (pScrollBar.m_hWnd)
		{
			SCROLLINFO si = { sizeof(SCROLLINFO), SIF_POS };
			if(pScrollBar.GetScrollInfo(&si))
			{
				si.fMask = SIF_POS;
				si.nPos = si.nPos;
				SetScrollInfo(SB_HORZ, &si, TRUE);
			}
		}
	}
	SetMsgHandled(FALSE);
}

void CNCaptureView::OnVScroll(UINT nSBCode, UINT /*nPos*/, CScrollBar pScrollBar)
{
	if (SB_THUMBPOSITION == nSBCode || SB_THUMBTRACK == nSBCode)
	{
		if (pScrollBar.m_hWnd)
		{
			SCROLLINFO si = { sizeof(SCROLLINFO), SIF_POS };
			if(pScrollBar.GetScrollInfo(&si))
			{
				si.fMask = SIF_POS;
				si.nPos = si.nPos;
				SetScrollInfo(SB_VERT, &si, TRUE);
			}
		}
	}
	SetMsgHandled(FALSE);
}


BOOL CNCaptureView::OnSetCursor(CWindow wnd, UINT nHitTest, UINT message)
{
	BOOL bRet = CDrawCanvas<CNCaptureView>::OnSetCursor(wnd, nHitTest, message);
	if (!bRet)
	{
		//bRet = DefWindowProc();
		SetMsgHandled(FALSE);
	}
	return bRet;
}

void CNCaptureView::SetNotifyWnd( const HWND hWndNotify )
{
	m_hWndNotify = hWndNotify;
}

void CNCaptureView::OnDestroy()
{
	LRESULT lRet = __super::DefWindowProc();

	if ( IsWindow() )
	{
		this->DestroyWindow();
		m_hWnd = NULL;
	}
	SetMsgHandled(FALSE);
}

void CNCaptureView::DoPaint(CDCHandle dc)
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

BOOL CNCaptureView::OnEraseBkgnd(CDCHandle dc)
{
	BOOL bRet = FALSE;
	if (m_bInited)
	{
		//API_VERIFY(ERROR != dc.ExcludeClipRect(m_rcImageView));
		//dc.FillSolidRect(m_rcClient, RGB(255,0,0));
	}
	return TRUE;
}


//void CNCaptureView::OnSize(UINT nType, CSize size)
//{
//	_CalcImageSize();
//	SetMsgHandled(FALSE);
//}

void CNCaptureView::OnSetFocus(CWindow wndOld)
{
	if ( m_hWndNotify )
	{
		::SetFocus( m_hWndNotify );
	}
}

void CNCaptureView::OnKillFocus(CWindow wndFocus)
{
	if ( m_hWndNotify )
	{
		::SetFocus( m_hWndNotify );
	}
	SetMsgHandled(FALSE);
}

void CNCaptureView::_EventHook()
{
	BOOL bRet = FALSE;
	CNCaptureDoc* pDoc = CNCaptureApplication::Instance()->GetDocument();
	FTLASSERT( NULL != pDoc );
	__hook(&CNCaptureDoc::OnCaptureImageChanged, pDoc, &CNCaptureView::OnCaptureImageChanged, this );
}

void CNCaptureView::_EventUnHook()
{
	CNCaptureDoc* pDoc = CNCaptureApplication::Instance()->GetDocument();
	FTLASSERT( NULL != pDoc );
	__unhook(&CNCaptureDoc::OnCaptureImageChanged, pDoc, &CNCaptureView::OnCaptureImageChanged, this );	
}


void CNCaptureView::_SetScrollInfo(float newZoomScale, BOOL bUseCenterPoint)
{
	if (m_pImage && !m_pImage->IsNull())
	{
		BOOL bRet = FALSE;
		CRect rcClient;
		API_VERIFY(GetClientRect(&rcClient));

		//save old ptLogical
		CPoint ptMouseDevice(0, 0);
		API_VERIFY(::GetCursorPos(&ptMouseDevice));
		API_VERIFY(ScreenToClient(&ptMouseDevice));

		if (!rcClient.PtInRect(ptMouseDevice) || bUseCenterPoint)
		{
			ptMouseDevice = rcClient.CenterPoint();
		}

		CPoint ptOldLogical = ptMouseDevice;
		ClientToDoc(&ptOldLogical);
		//ptOldPos.Offset(m_rcDrawTarget.TopLeft());

		SetZoomScale(newZoomScale);

		//FTLTRACE(TEXT("Zoom=%f, image=[%d,%d], sizeAll=[%d,%d]\n"),
		//	s_FixedZoomScales[m_iFixedZoomIndex], m_pImage->GetWidth(), m_pImage->GetHeight(),
		//	(int)((float)m_pImage->GetWidth() * s_FixedZoomScales[m_iFixedZoomIndex]),
		//	(int)((float)m_pImage->GetHeight() * s_FixedZoomScales[m_iFixedZoomIndex]));

		SetScrollSize(m_pImage->GetWidth(), m_pImage->GetHeight(), TRUE, true);

		CPoint ptNewClient = ptOldLogical;

		//ptNewPos.Offset(-m_rcDrawTarget.TopLeft());
		DocToClient(&ptNewClient);
		CPoint ptNewOffset = ptNewClient;
		ptNewOffset.Offset(-ptMouseDevice);
		SetScrollOffset(ptNewOffset.x, ptNewOffset.y);

/*
		if (!GetSelection().empty())
		{
			CTextObject* pTextObject = dynamic_cast<CTextObject*>(GetSelection().front());
			if (pTextObject)
			{
				//SIZEL newExtent = {0};
				//newExtent.cy = (double)pTextObject->GetPosition().Height() * 2540 * newZoomScale * 96;
				//newExtent.cx = (double)pTextObject->GetPosition().Width() * 2540 * newZoomScale * 96;

				pTextObject->GetRichEditPanel()->SetZoom(m_sizeAll.cx, m_sizeLogAll.cx);
				//pTextObject->GetRichEditPanel()->SetExtent(&newExtent, TRUE);
			}
		}
*/
		//FTLTRACE(TEXT("ptMouseDevice=[%d,%d], ptOldLogical=[%d,%d], ptNewClient=[%d,%d], ptNewOffset=[%d,%d]\n"),
		//	ptMouseDevice.x, ptMouseDevice.y, ptOldLogical.x, ptOldLogical.y, ptNewClient.x, ptNewClient.y,
		//	ptNewOffset.x, ptNewOffset.y);

		_UpdateMousePosInfo(ptMouseDevice);

		//DrawObjectList& SelectObjects = GetSelection();
		//if (SelectObjects.size() == 1 )
		//{
		//	CDrawObject* pDrawObj = SelectObjects.front();
		//	if (dotSelectRect == pDrawObj->GetDrawObjType())
		//	{
		//		CRect rcDevice = pDrawObj->GetPosition();
		//		DocToClient(&rcDevice);
		//		ScrollToView(rcDevice);
		//	}
		//}
	}
	else //make scrollbar disappear
	{
		SetZoomScale(s_FixedZoomScales[s_NormalZoomIndex]);
		SetScrollSize(1, 1, FALSE, FALSE);
	}
}

void CNCaptureView::_CalcImageSize()
{
	if (m_bInited)
	{
		_SetScrollInfo(s_FixedZoomScales[s_NormalZoomIndex]);
		//if (!m_pCalcRect && m_pImage != NULL)
		//{
		//	m_pCalcRect = CNPVPhotoCalcRect::GetPhotoCalcRectObject(m_pImage->GetZoomMode(), NULL);
		//}
	}
}


void CNCaptureView::OnCaptureImageChanged()
{
	FTLTRACE(TEXT("CNCaptureView::OnCaptureImageChanged\n"));

	CNCaptureDoc* pDoc = CNCaptureApplication::Instance()->GetDocument();
	if (pDoc)
	{
		CCapImageObj* pCaptureImage = pDoc->GetCurCaptureImage();
		//save zoom to imageobject
		if (m_pImage != NULL)
		{
			m_pImage->SetZoom(GetZoom());
		}
		m_pImage = pCaptureImage;
		m_bImageChanged = TRUE;
	}
	//m_iFixedZoomIndex = s_NormalZoomIndex;
	//ReleaseSelectRect();
	_CalcImageSize();
	//make scrollbar disappear and set statusbar text unvisible
	if (NULL == m_pImage)
	{
		CRect rcClient;
		GetClientRect(&rcClient);
		SetScrollSize(0, 0, FALSE, FALSE);
		STATUSBARINFO stStatusBarInfo;
		stStatusBarInfo.bEnable = FALSE;
		SetCurrentToolType(ttNone);
		CMainFrame* pMainFrame = CNCaptureApplication::Instance()->GetMainFrame();
		if (pMainFrame)
		{
			pMainFrame->SetStatusInfo(&stStatusBarInfo);
		}
	}
	else
	{	
		//first capture
		if (1 == pDoc->GetCaptureCount() )
		{
			SetCurrentToolType(ttRect);
		}
		SetZoom(m_pImage->GetZoom(), TRUE, TRUE);
		m_pImage->GetCurrentObjectInfo(m_allObjects, m_selection);
	}
		
	Invalidate();


}

void CNCaptureView::InvalidateFrame(LPRECT lpRect, BOOL bRePaint)
{
	//CVirtualFrame::SetUpdateLayer();
	if (IsWindow())
	{
		InvalidateRect(lpRect, bRePaint);
	}
}

BOOL CNCaptureView::SelectAll()
{
	BOOL bRet = FALSE;
	//if (m_pImage)
	//{
	//	API_VERIFY(ReleaseSelectRect());
	//	SetCurrentToolType(ttSelection);
	//	CRect rcSelectAll = m_rcDrawTarget;
	//	rcSelectAll.OffsetRect(-m_rcDrawTarget.TopLeft());
	//	API_VERIFY(CreateSelectRect(rcSelectAll));
	//}
	return bRet;
}

BOOL CNCaptureView::CopyImageToClipboard()
{
	BOOL bRet = FALSE;
	
	//rcSelect.OffsetRect(-m_rcDrawTarget.TopLeft());

	if (m_pImage)
	{
		CRect rcCopy(0, 0, m_pImage->GetWidth(), m_pImage->GetHeight());
		
		DrawObjectList& drawObjList = GetSelection();
		if (drawObjList.size() == 1)
		{
			CDrawObject* pDrawObj = drawObjList.front();
			FTLASSERT(pDrawObj);
			if (pDrawObj->GetDrawObjType() == dotSelectRect)
			{
				rcCopy = pDrawObj->GetPosition();
			}
		}

		API_VERIFY(OpenClipboard());
		API_VERIFY(EmptyClipboard());

		HANDLE hDIB = m_pImage->CopyToHandle(rcCopy);
		if (hDIB)
		{
			API_VERIFY(NULL != SetClipboardData(CF_DIB, hDIB));
		}
		API_VERIFY(CloseClipboard());
	}
	return bRet;
}

BOOL CNCaptureView::CropImage()
{
	if (m_pImage)
	{
		CRect rcCopy(0, 0, m_pImage->GetWidth(), m_pImage->GetHeight());

		DrawObjectList& drawObjList = GetSelection();
		if (drawObjList.size() == 1)
		{
			CDrawObject* pDrawObj = drawObjList.front();
			FTLASSERT(pDrawObj);
			if (pDrawObj->GetDrawObjType() == dotSelectRect)
			{
				rcCopy = pDrawObj->GetPosition();
			}
		}
		return m_pImage->CropImage(rcCopy);
	}
	return FALSE;
}

BOOL CNCaptureView::_EditWithMspaint(LPCTSTR lpszImagePath)
{
	BOOL bRet = TRUE;
	HRESULT hr = E_FAIL;
	TCHAR szMsPaintPath[MAX_PATH] = {0};
	COM_VERIFY(SHGetFolderPath(m_hWnd, CSIDL_SYSTEM, NULL, SHGFP_TYPE_CURRENT, szMsPaintPath));
	if (SUCCEEDED(hr))
	{
		ATLPath::Append(szMsPaintPath, TEXT("MsPaint.exe"));

		//add quotation marks for blank in file name
		CString strNewPath;
		if (lpszImagePath && lpszImagePath[0])
		{
			strNewPath.Format(TEXT("\"%s\""), lpszImagePath);
		}

		DWORD dwError = (DWORD)ShellExecute(NULL, NULL, szMsPaintPath, 
			strNewPath , NULL, SW_SHOW);
		if (dwError < 32)
		{
			SetLastError(dwError);
			bRet = FALSE;
		}
	}
	return bRet;
}

BOOL CNCaptureView::ShowImageInMsPaint()
{
	BOOL bRet = FALSE;
	HRESULT hr = E_FAIL;

	if (m_pImage) // && !m_pImage->IsNull())
	{
		TCHAR szSavePath[MAX_PATH] = {0};
		API_VERIFY(SHGetSpecialFolderPath(NULL, szSavePath, CSIDL_MYDOCUMENTS , TRUE));
		if (bRet)
		{
			CString strFilePath(szSavePath);
			strFilePath += TEXT("\\nCapture\\mspaint\\");
			COM_VERIFY_EXCEPT1(SHCreateDirectoryEx(NULL, strFilePath, NULL), ERROR_ALREADY_EXISTS);

			//CTime now = CTime::GetCurrentTime();
			//strFilePath.AppendFormat(TEXT("%04d-%02d-%02d %02d-%02d-%02d.PNG"), 
			//	now.GetYear(), now.GetMonth(), now.GetDay(),
			//	now.GetHour(), now.GetMinute(), now.GetSecond());
			CString strName = m_pImage->GetFileName();
			strFilePath.AppendFormat(TEXT("%s"), strName);
			COM_VERIFY(m_pImage->Save(strFilePath, ImageFormatPNG));
			if (SUCCEEDED(hr))
			{
				m_pImage->SetEditTmpFile(strFilePath);
				return _EditWithMspaint(strFilePath);
			}
		}
	}
	return _EditWithMspaint(NULL);
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

void CNCaptureView::OnCancelMode()// OnCaptureChanged(CWindow wnd)
{
#ifdef _DEBUG
	CPoint MouseClient;
	GetCursorPos(&MouseClient);
	ScreenToClient(&MouseClient);
	CPoint ptMouseLogical = MouseClient;
	ClientToDoc(&ptMouseLogical);

	FTLTRACE(TEXT("OnCancelMode, MouseClient=[%d,%d], MouseDoc=[%d,%d]\n"), 
		MouseClient.x, MouseClient.y, ptMouseLogical.x, ptMouseLogical.y);
#endif 
	CDrawCanvas<CNCaptureView>::OnCancelMode();

	SetMsgHandled(FALSE);
}

int  CNCaptureView::_GetNextFixedZoomIndex(float curZoom, BOOL bBigger)
{
	if (bBigger)
	{
		for (int i = 0; i < _countof(s_FixedZoomScales); ++i)
		{
			if (curZoom < s_FixedZoomScales[i])
			{
				return i;
			}
		}
		return _countof(s_FixedZoomScales)-1;
	}
	else
	{
		for (int i = _countof(s_FixedZoomScales)-1; i >= 0 ; --i)
		{
			if (curZoom > s_FixedZoomScales[i])
			{
				return i;
			}
		}
		return 0;
	}
}

void CNCaptureView::_SyncFrameScaleInfo()
{
	CMainFrame* pMainFrame = CNCaptureApplication::Instance()->GetMainFrame();
	if (pMainFrame)
	{
		pMainFrame->SyncViewImageScale();
	}
}

BOOL CNCaptureView::SetZoom(float zoom, BOOL bSyncFrame, BOOL bUseCenterPoint)
{
	if (zoom < s_FixedZoomScales[0])
	{
		zoom = s_FixedZoomScales[0];
	}
	if (zoom > s_FixedZoomScales[ZOOM_COUNT-1])
	{
		zoom = s_FixedZoomScales[ZOOM_COUNT-1];
	}

	_SetScrollInfo(zoom, bUseCenterPoint);
	if (bSyncFrame)
	{
		_SyncFrameScaleInfo();
	}
	return TRUE;
	//if (zoom >= s_FixedZoomScales[0] && 
	//	zoom <= s_FixedZoomScales[ZOOM_COUNT-1])
	//{
		//int iChooseIndex = 0;
		//for (; iChooseIndex < _countof(s_FixedZoomScales); iChooseIndex++)
		//{
		//	if (FTL::IsSameNumber(zoom, s_FixedZoomScales[iChooseIndex], 0.01f))
		//	{
		//		break;
		//	}
		//}
		//if (iChooseIndex >= _countof(s_FixedZoomScales))
		//{
		//	iChooseIndex = _countof(s_FixedZoomScales)-1;
		//}
		//if (iChooseIndex != m_iFixedZoomIndex)
		//{
			//m_iFixedZoomIndex = iChooseIndex;
			//_SetScrollInfo(zoom);
			//if (bSyncFrame)
			//{
			//	_SyncFrameScaleInfo();
			//}
		//}
		//return TRUE;
	//}
	//return FALSE;
}

float CNCaptureView::GetZoom()
{
	return this->GetZoomScale();
}

BOOL CNCaptureView::NextZoom(BOOL bBigger)
{
	if (!m_pImage)
	{
		return FALSE;
	}

	float zoomScale = this->GetZoomScale();
	//int oldZoomIndex = m_iFixedZoomIndex;
	int newZoomIndex = 0;
	if (bBigger)
	{
		newZoomIndex = _GetNextFixedZoomIndex(zoomScale, TRUE);
	}
	else
	{
		newZoomIndex = _GetNextFixedZoomIndex(zoomScale, FALSE);
	}
	if (!FTL::IsSameNumber(zoomScale, s_FixedZoomScales[newZoomIndex], 0.01f))
	{
		_SetScrollInfo(s_FixedZoomScales[newZoomIndex]);
		_SyncFrameScaleInfo();
		return TRUE;
	}
	return FALSE;
}

void CNCaptureView::ClientToDoc(CPoint* pPoint) 
{
	CWindowDC dc(m_hWnd);
	PrepareDC(dc.m_hDC);
	dc.DPtoLP(pPoint, 1);
}
void CNCaptureView::ClientToDoc(CRect* pRect)
{
	CWindowDC dc(m_hWnd);
	PrepareDC(dc.m_hDC);
	dc.DPtoLP((LPPOINT)pRect, 2);
}

void CNCaptureView::DocToClient(CPoint* pPoint)
{
	CWindowDC dc(m_hWnd);
	PrepareDC(dc.m_hDC);
	dc.LPtoDP(pPoint, 1);
}
void CNCaptureView::DocToClient(CRect* pRect)
{
	CWindowDC dc(m_hWnd);
	PrepareDC(dc.m_hDC);
	dc.LPtoDP((LPPOINT)pRect, 2);
}

void CNCaptureView::SetCurrentOffsetPoint(LPPOINT lpPoint)
{
	SetScrollOffset(lpPoint->x, lpPoint->y, TRUE);
}

void CNCaptureView::GetCurrentOffsetPoint(LPPOINT lpPoint)
{
	GetScrollOffset(*lpPoint);
}
CRect CNCaptureView::_GetPhotoRect( CRect rcClient, LPSIZE lpSize)
{
	CRect rcTarget( 0, 0, 0, 0 );
	if (m_pCalcRect)
	{
		rcTarget = m_pCalcRect->GetPhotoRect(rcClient, lpSize, 1.0f, FALSE, FALSE);
	}
	
	//CPoint ptOriginalLogical(0,0);
	//CPoint ptOriginalDevice(0,0);
	//DocToClient(&ptOriginalLogical);
	//ClientToDoc(&ptOriginalDevice);

	//FTLTRACE(TEXT("rcClient=[%d,%d]x[%d,%d], lpSize={%d,%d}, rcTarget=[%d,%d]x[%d,%d],{%d,%d}, DocToClient(0,0)=[%d,%d], ClientToDoc(0,0)=[%d,%d]\n"), 
	//	rcClient.left, rcClient.top, rcClient.right, rcClient.bottom, lpSize->cx, lpSize->cy,
	//	rcTarget.left, rcTarget.top, rcTarget.right, rcTarget.bottom,
	//	rcTarget.Width(), rcTarget.Height(),
	//	ptOriginalLogical.x, ptOriginalLogical.y, ptOriginalDevice.x, ptOriginalDevice.y);

	return 	rcTarget;
}

BOOL CNCaptureView::GetImageByRect(const CRect& rcSrc, CImage& Image)
{
	if (m_pImage)
	{
		CImageDC dcImage(Image);
		DrawObjects(m_canvasImage.GetMemoryDC());
		BitBlt(dcImage, 0, 0, rcSrc.Width(), rcSrc.Height(), m_canvasImage.GetMemoryDC(), rcSrc.left, rcSrc.top, SRCCOPY);
		CRect rcImage(0, 0, m_pImage->GetWidth(), m_pImage->GetHeight());
		m_pImage->Draw(m_canvasImage, rcImage);
		return TRUE;
	}
	return FALSE;
}

void CNCaptureView::PrepareDC(CDCHandle dc)
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


void CNCaptureView::SelectToolTypeByMenu(const CPoint& ptPoint)
{
	CMenu PopMenu;
	if(PopMenu.CreatePopupMenu())
	{
		UINT nID = 500;

		ToolType emToolType = GetCurrentToolType();
		for (DrawToolList::iterator it = m_tools.begin(); it != m_tools.end(); ++it)
		{
			UINT uFlags = MF_STRING;
			if (emToolType == (*it)->GetToolType())
			{
				uFlags |= MF_CHECKED;
			}
			PopMenu.AppendMenu(uFlags, nID ++, (*it)->GetToolName());
		}
		

		UINT uRetID = PopMenu.TrackPopupMenuEx(TPM_RETURNCMD | TPM_RIGHTALIGN, ptPoint.x, ptPoint.y, m_hWnd);
		if (uRetID >= 500)
		{
			size_t uIndex = uRetID - 500;
			if (uIndex < m_tools.size() && emToolType != m_tools[uIndex]->GetToolType())
			{
				SetCurrentToolType(m_tools[uIndex]->GetToolType());
			}
		}
		PopMenu.DestroyMenu();
	}
}

BOOL CNCaptureView::BackupDrawObjectData(LPCTSTR strName)
{
	if (m_pImage)
	{
		return m_pImage->PushDrawObjectInfo(m_allObjects, m_selection, strName);
	}
	return FALSE;
}

void CNCaptureView::Undo()
{
	if(m_pImage && m_pImage->GetPrevDrawObjectInfo(m_allObjects, m_selection))
	{
		Invalidate();
	}
}

void CNCaptureView::Redo()
{
	if(m_pImage && m_pImage->GetNextDrawObjectInfo(m_allObjects, m_selection))
	{
		Invalidate();
	}
}