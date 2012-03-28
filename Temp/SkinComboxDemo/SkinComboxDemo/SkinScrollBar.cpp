#include "StdAfx.h"
#include "Skinscrollbar.h"


#define TIMER_NOTIFY 100
#define TIMER_INTERVAL 50

#define THUMB_BORDER 3
#define THUMB_MINSIZE ( 13 )

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
CSkinScrollBar::CSkinScrollBar ( void )
{
	ZeroMemory ( &m_SInfo, sizeof ( m_SInfo ) );
	m_SInfo.nTrackPos = -1;

	m_bDrag   = FALSE;
	m_uClicked  = ( UINT ) - 1;
	m_bNotify  = FALSE;
	m_uHitPrev  = ( UINT ) - 1;
	m_nWidth  = DEF_WIDTH;
	m_eClicked  = BT_NONE;
	m_bVertical  = FALSE;

	m_enumThumbMouseState = MOUSE_NOMAL;
}

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
CSkinScrollBar::~CSkinScrollBar ( VOID )
{
}

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
BOOL CSkinScrollBar::IsVertical ( VOID )
{
	return GetStyle() & SBS_VERT;
}

//////////////////////////////////////////////////////////////////////////

UINT CSkinScrollBar::HitTest ( CONST CPoint& pt )
{
	INT32 nTestPos = pt.y;

	if ( FALSE == IsVertical() )
	{
		nTestPos = pt.x;
	}

	if ( nTestPos < 0 )
	{
		return ( UINT ) - 1;
	}

	SCROLLINFO SInfo;

	CopyMemory ( &SInfo, &m_SInfo, sizeof ( m_SInfo ) );

	int nInterHeight = m_nHeight - 2 * m_nWidth;

	int nSlideHeight = SInfo.nPage * nInterHeight / ( SInfo.nMax - SInfo.nMin + 1 );

	if ( nSlideHeight < THUMB_MINSIZE )
	{
		nSlideHeight = THUMB_MINSIZE;
	}

	if ( nInterHeight < THUMB_MINSIZE )
	{
		nSlideHeight = 0;
	}

	int nEmptyHeight = nInterHeight - nSlideHeight;

	int nSegLen = m_nWidth;

	int nBottom = nSegLen;

	UINT uHit = SB_LINEUP;

	if ( nTestPos < nBottom )
	{
		goto END;
	}

	if ( SInfo.nTrackPos == -1 )
	{
		SInfo.nTrackPos = SInfo.nPos;
	}

	uHit = SB_PAGEUP;

	if ( ( SInfo.nMax - SInfo.nMin - SInfo.nPage + 1 ) == 0 )
	{
		nSegLen = nEmptyHeight / 2;
	}
	else
	{
		nSegLen = nEmptyHeight * SInfo.nTrackPos / ( SInfo.nMax - SInfo.nMin - SInfo.nPage + 1 );
	}

	nBottom += nSegLen;

	if ( nTestPos < nBottom )
	{
		goto END;
	}

	nSegLen = nSlideHeight;

	nBottom += nSegLen;

	uHit = SB_THUMBTRACK;

	if ( nTestPos < nBottom )
	{
		goto END;
	}

	nBottom = m_nHeight - m_nWidth;

	uHit = SB_PAGEDOWN;

	if ( nTestPos < nBottom )
	{
		goto END;
	}

	uHit = SB_LINEDOWN;

END:

	return uHit;
}

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Windows Event Handler
//////////////////////////////////////////////////////////////////////////
LRESULT CSkinScrollBar::OnSize ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{

	bHandled = FALSE;
	m_nHeight = IsVertical() ? HIWORD ( lParam ) : LOWORD ( lParam ) ;
	m_nWidth = IsVertical() ?  LOWORD( lParam ) : HIWORD ( lParam )  ;

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Windows Event Handler
//////////////////////////////////////////////////////////////////////////
LRESULT CSkinScrollBar::OnPaint ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	DrawScrollBar();
	
	HWND hParent = GetParent();
	::PostMessage ( hParent, UM_UPDATE_SCROLL_CORNER, 0, ( LPARAM ) m_hWnd );
	return 0;
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Windows Event Handler
//////////////////////////////////////////////////////////////////////////
LRESULT CSkinScrollBar::OnLButtonDown ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	CPoint pt ( GET_X_LPARAM ( lParam ), GET_Y_LPARAM ( lParam ) );

	SetCapture();
	UINT uHit = HitTest ( pt );
	CRect rcDest = GetRect ( SB_LINEUP );

	if ( rcDest.PtInRect ( pt ) )
	{
		m_eClicked = BT_UP;
	}

	rcDest = GetRect ( SB_LINEDOWN );

	if ( rcDest.PtInRect ( pt ) )
	{
		m_eClicked = BT_DOWN;
	}

	if ( uHit == SB_THUMBTRACK )
	{
		m_bDrag = TRUE;
		m_ptDrag = pt;
		m_SInfo.nTrackPos = m_SInfo.nPos;
		m_nDragPos = m_SInfo.nPos;
		m_enumThumbMouseState = MOUSE_DOWN;
		::PostMessage ( GetParent(), WM_SYSCOMMAND, IsVertical() ? SC_VSCROLL : SC_HSCROLL, 0 );
	}

	else
	{
		m_uClicked = uHit;
		m_bNotify = TRUE;
		SetTimer ( TIMER_NOTIFY, TIMER_INTERVAL, NULL );
	}
	Invalidate ( FALSE );
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Windows Event Handler
//////////////////////////////////////////////////////////////////////////
LRESULT CSkinScrollBar::OnLButtonUp ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	ReleaseCapture();

	m_eClicked = BT_NONE;

	if ( TRUE == m_bDrag )
	{
		m_bDrag = FALSE;
		UINT uMessage = IsVertical() ? WM_VSCROLL : WM_HSCROLL;
		HWND hParent = GetParent();
		//::PostMessage ( hParent, uMessage, MAKELONG ( SB_THUMBPOSITION, m_SInfo.nTrackPos ), ( LPARAM ) m_hWnd );
		if ( TRUE == IsVertical() )
		{
			::PostMessage ( hParent, UM_V_THUMBTRACK_MSG, m_SInfo.nTrackPos,  ( LPARAM ) m_hWnd );
		}
		else
		{
			::PostMessage ( hParent, UM_H_THUMBTRACK_MSG, m_SInfo.nTrackPos,  ( LPARAM ) m_hWnd );
		}

		::PostMessage ( hParent, uMessage, SB_ENDSCROLL, ( LPARAM ) m_hWnd );

		CPaintDC dc ( m_hWnd );

		if ( m_SInfo.nTrackPos != m_SInfo.nPos )
		{
			DrawThumb ( dc.m_hDC, GetRect ( SB_THUMBTRACK ) );
		}

		m_SInfo.nTrackPos = -1;
	}

	else if ( -1 != m_uClicked )
	{
		if ( m_bNotify )
		{
			KillTimer ( TIMER_NOTIFY );
			m_bNotify = FALSE;
		}

		m_uClicked = ( UINT ) - 1;
	}

	Invalidate ( FALSE );

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Windows Event Handler
//////////////////////////////////////////////////////////////////////////
LRESULT CSkinScrollBar::OnMouseMove ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	CPoint point ( GET_X_LPARAM ( lParam ), GET_Y_LPARAM ( lParam ) );

	TRACKMOUSEEVENT mouseEvent;
	mouseEvent.cbSize = sizeof ( mouseEvent );
	mouseEvent.dwFlags = TME_LEAVE;
	mouseEvent.hwndTrack = m_hWnd;
	TrackMouseEvent ( &mouseEvent );

	if ( TRUE == m_bDrag )
	{
		int nInterHeight = m_nHeight - 2 * m_nWidth;
		int nSlideHeight = m_SInfo.nPage * nInterHeight / ( m_SInfo.nMax - m_SInfo.nMin );

		if ( nSlideHeight < THUMB_MINSIZE )
		{
			nSlideHeight = THUMB_MINSIZE;
		}

		if ( nInterHeight < THUMB_MINSIZE )
		{
			nSlideHeight = 0;
		}

		int nEmptyHeight = nInterHeight - nSlideHeight;

		if ( 0 >= nEmptyHeight )
		{
			nEmptyHeight = 1;
		}

		int nDragLen = IsVertical() ? point.y - m_ptDrag.y : point.x - m_ptDrag.x;

		int nSlide = nDragLen * ( int ) ( m_SInfo.nMax - m_SInfo.nMin - m_SInfo.nPage + 1 ) / nEmptyHeight;

		int nNewTrackPos = m_nDragPos + nSlide;

		if ( nNewTrackPos < m_SInfo.nMin )
		{
			nNewTrackPos = m_SInfo.nMin;
		}
		else if ( nNewTrackPos > ( int ) ( m_SInfo.nMax - m_SInfo.nMin - m_SInfo.nPage + 1 ) )
		{
			nNewTrackPos = m_SInfo.nMax - m_SInfo.nMin - m_SInfo.nPage + 1;
		}


		if ( nNewTrackPos != m_SInfo.nTrackPos )
		{
			CRect rcThumb1 = GetRect ( SB_THUMBTRACK );

			m_SInfo.nTrackPos = nNewTrackPos;

			if ( TRUE == IsVertical() )
			{
				::SendMessage ( GetParent(), UM_V_THUMBTRACK_MSG,  m_SInfo.nTrackPos , ( LPARAM ) m_hWnd );
			}
			else
			{
				::SendMessage ( GetParent(), UM_H_THUMBTRACK_MSG,  m_SInfo.nTrackPos , ( LPARAM ) m_hWnd );
			}
		}
	}

	UINT uHit = HitTest ( point );
	if ( -1 != m_uClicked )
	{
		CRect rc = GetRect ( m_uClicked );

		if ( FALSE == rc.PtInRect ( point ) )
		{
			if ( m_bNotify )
			{
				KillTimer ( TIMER_NOTIFY );
				m_bNotify = FALSE;
			}
		}

		else
		{
			if ( !m_bNotify )
			{
				OnTimer ( WM_TIMER, NULL, TIMER_NOTIFY, m_bNotify );
				SetTimer ( TIMER_NOTIFY, TIMER_INTERVAL, NULL );
				m_bNotify = TRUE;
			}
		}
	}

	else
	{


		if ( uHit != m_uHitPrev )
		{
			if ( -1 != m_uHitPrev )
			{
				if ( SB_THUMBTRACK == m_uHitPrev )
				{
					//Invalidate ( FALSE );
					//UpdateWindow();
				}
			}

			if ( -1 != uHit )
			{
				if ( SB_THUMBTRACK == uHit )
				{
					//Invalidate ( FALSE );
					//UpdateWindow();
				}
			}

			m_uHitPrev = uHit;

			return S_OK;
		}
	}
	if ( SB_THUMBTRACK == uHit  )
	{
		if ( MK_LBUTTON ==  wParam )
		{
			m_enumThumbMouseState = MOUSE_DOWN;
		}
		else
		{
			m_enumThumbMouseState = MOUSE_OVER;
		}

	}
	else if ( MK_LBUTTON != wParam )
	{
		m_enumThumbMouseState = MOUSE_NOMAL;
	}
	Invalidate ( FALSE );
	UpdateWindow();
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Windows Event Handler
//////////////////////////////////////////////////////////////////////////
LRESULT CSkinScrollBar::OnTimer ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	UINT uTimerID = static_cast<UINT> ( wParam );

	if ( TIMER_NOTIFY == uTimerID )
	{
		CRect rcThumb = GetRect ( SB_THUMBTRACK );
		CPoint pt;

		::GetCursorPos ( &pt );
		::ScreenToClient ( m_hWnd, &pt );

		if ( TRUE == rcThumb.PtInRect ( pt ) )
		{
			KillTimer ( TIMER_NOTIFY );
			return S_OK;
		}

		::PostMessage ( GetParent(), IsVertical() ? WM_VSCROLL : WM_HSCROLL, MAKELONG ( m_uClicked, 0 ), ( LPARAM ) m_hWnd );
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Windows Event Handler
//////////////////////////////////////////////////////////////////////////
LRESULT CSkinScrollBar::OnMouseLeave ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	m_enumThumbMouseState = MOUSE_NOMAL;
	Invalidate ( FALSE );
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Windows Event Handler
//////////////////////////////////////////////////////////////////////////
LRESULT CSkinScrollBar::OnLButtonDblClk ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	bHandled = TRUE;
	return 1;
}

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Windows Event Handler
//////////////////////////////////////////////////////////////////////////
LRESULT CSkinScrollBar::OnGetScrollInfo ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	LPSCROLLINFO lpScrollInfo = reinterpret_cast<LPSCROLLINFO> ( lParam );

	int nMask = lpScrollInfo->fMask;

	if ( nMask & SIF_PAGE )
	{
		lpScrollInfo->nPage = m_SInfo.nPage;
	}

	if ( nMask & SIF_POS )
	{
		lpScrollInfo->nPos = m_SInfo.nPos;
	}

	if ( nMask & SIF_TRACKPOS )
	{
		lpScrollInfo->nTrackPos = m_SInfo.nTrackPos;
	}

	if ( nMask & SIF_RANGE )
	{
		lpScrollInfo->nMin = m_SInfo.nMin;
		lpScrollInfo->nMax = m_SInfo.nMax;
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Windows Event Handler
//////////////////////////////////////////////////////////////////////////
LRESULT CSkinScrollBar::OnSetScrollInfo ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	bHandled = TRUE;

	BOOL bRedraw = static_cast<BOOL> ( wParam );
	LPSCROLLINFO lpScrollInfo = reinterpret_cast<LPSCROLLINFO> ( lParam );

	if ( lpScrollInfo->fMask & SIF_PAGE )
	{
		m_SInfo.nPage = lpScrollInfo->nPage;
	}

	if ( lpScrollInfo->fMask & SIF_POS )
	{
		m_SInfo.nPos = lpScrollInfo->nPos;
	}

	if ( lpScrollInfo->fMask & SIF_RANGE )
	{
		m_SInfo.nMin = lpScrollInfo->nMin;
		m_SInfo.nMax = lpScrollInfo->nMax;
	}

	if ( bRedraw )
	{
		Invalidate ( FALSE );
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
CRect CSkinScrollBar::GetRect ( UINT uSBCode )
{
	SCROLLINFO SInfo = m_SInfo;

	if ( SInfo.nTrackPos == -1 )
	{
		SInfo.nTrackPos = SInfo.nPos;
	}

	int nSlotHeight = m_nHeight - 2 * m_nWidth;

	int nSlideHeight = SInfo.nPage * nSlotHeight / ( SInfo.nMax - SInfo.nMin + 1 );

	if ( nSlideHeight < THUMB_MINSIZE )
	{
		nSlideHeight = THUMB_MINSIZE;
	}

	if ( nSlotHeight < THUMB_MINSIZE )
	{
		nSlideHeight = 0;
	}

	if ( ( SInfo.nMax - SInfo.nMin - SInfo.nPage + 1 ) == 0 )
	{
		nSlideHeight = 0;
	}

	int nEmptyHeight = nSlotHeight - nSlideHeight;

	CRect rcRet ( 0, 0, m_nWidth + 1, m_nWidth + 1 );

	if ( uSBCode == SB_LINEUP )
	{
		goto END;
	}

	rcRet.top = rcRet.bottom;

	if ( ( SInfo.nMax - SInfo.nMin - SInfo.nPage + 1 ) == 0 )
	{
		rcRet.bottom += nEmptyHeight / 2;
	}
	else
	{
		rcRet.bottom += nEmptyHeight * SInfo.nTrackPos / ( SInfo.nMax - SInfo.nMin - SInfo.nPage + 1 );
	}

	if ( uSBCode == SB_PAGEUP )
	{
		goto END;
	}

	rcRet.top = rcRet.bottom;

	rcRet.bottom += nSlideHeight;

	if ( uSBCode == SB_THUMBTRACK )
	{
		goto END;
	}

	rcRet.top = rcRet.bottom;

	rcRet.bottom = m_nHeight - m_nWidth;

	if ( uSBCode == SB_PAGEDOWN )
	{
		goto END;
	}

	rcRet.top = rcRet.bottom;

	rcRet.bottom = m_nHeight;
	if (IsVertical())
	{
		rcRet.bottom += 1;
	}

END:

	if ( FALSE == IsVertical() )
	{
		int nTemp = rcRet.left;
		rcRet.left = rcRet.top;
		rcRet.top = nTemp;
		nTemp = rcRet.right;
		rcRet.right = rcRet.bottom;
		rcRet.bottom = nTemp;
	}

	return rcRet;
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////


void CSkinScrollBar::_DrawBtnState( HDC hDC, NDGraphics::CGDIPImageList& imgBtn, MOUSE_STATE mouseState, CRect rcRect )
{
	if ( MOUSE_NOMAL == mouseState )
	{
		NDGraphics::DrawGdiplusImage ( hDC, imgBtn[0], rcRect.left, rcRect.top );
	}
	else if ( MOUSE_OVER == mouseState )
	{
		NDGraphics::DrawGdiplusImage ( hDC, imgBtn[1], rcRect.left, rcRect.top );
	}
	else if ( MOUSE_DOWN == mouseState )
	{
		NDGraphics::DrawGdiplusImage ( hDC, imgBtn[2], rcRect.left, rcRect.top );
	}
	else if ( MOUSE_DISABLE == mouseState )
	{
		NDGraphics::DrawGdiplusImage ( hDC, imgBtn[3], rcRect.left, rcRect.top );
	}
}

void CSkinScrollBar::_RepeatDrawBtnState( HDC hDC, NDGraphics::CGDIPImageList& imgBtn, MOUSE_STATE mouseState, CRect rcRect ) // NS
{
	if ( MOUSE_NOMAL == mouseState )
	{
		imgBtn[0]->Draw( hDC, rcRect.left, rcRect.top, rcRect.Width(), rcRect.Height() ); //NS
	}
	else if ( MOUSE_OVER == mouseState )
	{
		imgBtn[1]->Draw( hDC, rcRect.left, rcRect.top, rcRect.Width(), rcRect.Height()  ); //NS
	}
	else if ( MOUSE_DOWN == mouseState )
	{
		imgBtn[2]->Draw( hDC, rcRect.left, rcRect.top, rcRect.Width(), rcRect.Height()  ); //NS
	}
	else if ( MOUSE_DISABLE == mouseState )
	{
		imgBtn[3]->Draw( hDC, rcRect.left, rcRect.top, rcRect.Width(), rcRect.Height()  ); //NS
	}
}

void CSkinScrollBar::DrawThumbTop( HDC hDC, const CRect& rcThumb )
{
	if ( FALSE ==  m_imgThumbTop.IsValid() )
	{
		return;
	}
	CRect rcTop = rcThumb;
	if ( TRUE == IsVertical() )
	{
		rcTop.bottom = rcThumb.top + m_imgThumbTop[0]->GetHeight();
	}
	else
	{
		rcTop.right = rcThumb.left + m_imgThumbTop[0]->GetWidth();
	}

	//_DrawBtnState( hDC, m_imgThumbTop, m_enumThumbMouseState, rcTop );
	_RepeatDrawBtnState(hDC, m_imgThumbTop, m_enumThumbMouseState, rcTop);
}

void CSkinScrollBar::DrawThumbBottom( HDC hDC, const CRect& rcThumb )
{
	if ( FALSE ==  m_imgThumbBottom.IsValid() )
	{
		return;
	}

	CRect rcBottom = rcThumb;
	if ( TRUE == IsVertical() )
	{
		rcBottom.top = rcThumb.bottom - m_imgThumbBottom[0]->GetHeight();
	}
	else
	{
		rcBottom.left = rcThumb.right - m_imgThumbBottom[0]->GetWidth();
	}
	//_DrawBtnState( hDC, m_imgThumbBottom, m_enumThumbMouseState, rcBottom );
	_RepeatDrawBtnState(hDC, m_imgThumbBottom, m_enumThumbMouseState, rcBottom);

}

void CSkinScrollBar::DrawThumbMiddle( HDC hDC, const CRect& rcThumb )
{
	if ( FALSE ==  m_imgThumbBody.IsValid() )
	{
		return;
	}
	if ( FALSE ==  m_imgThumbTop.IsValid() )
	{
		return;
	}
	if ( FALSE ==  m_imgThumbBottom.IsValid() )
	{
		return;
	}

	CRect rcBody = rcThumb;
	if ( TRUE == IsVertical() )
	{
		rcBody.top = rcBody.top + m_imgThumbTop[0]->GetHeight();
		rcBody.bottom = rcBody.bottom - m_imgThumbBottom[0]->GetHeight();
	}
	else
	{
		rcBody.left = rcBody.left + m_imgThumbTop[0]->GetWidth();
		rcBody.right = rcBody.right - m_imgThumbBottom[0]->GetWidth();
	}
	_RepeatDrawBtnState( hDC, m_imgThumbBody, m_enumThumbMouseState, rcBody );
}

void CSkinScrollBar::DrawThumbLine( HDC hDC, const CRect& rcThumb )
{
	if ( FALSE ==  m_imgThumbLine.IsValid() )
	{
		return;
	}

	if ( TRUE == IsVertical() )
	{
		CRect rcLine( 0, 0, 0, 0 );
		rcLine.left = rcThumb.left - 1 + ( ( rcThumb.Width() / 2 ) - ( m_imgThumbLine[0]->GetWidth() / 2 ) );
		rcLine.top = rcThumb.top + ( rcThumb.Height() / 2 ) - ( m_imgThumbLine[0]->GetHeight() / 2 );
		rcLine.bottom = rcLine.top + m_imgThumbLine[0]->GetHeight();
		rcLine.right = rcLine.left + 2 +  m_imgThumbLine[0]->GetWidth();
		_RepeatDrawBtnState ( hDC, m_imgThumbLine, m_enumThumbMouseState, rcLine );
	}
	else
	{
		CRect rcLine( 0, 0, 0, 0 );
		rcLine.left = rcThumb.left + ( ( rcThumb.Width() / 2 ) - ( m_imgThumbLine[0]->GetWidth() / 2 ) );
		rcLine.top = rcThumb.top - 1 + ( rcThumb.Height() / 2 ) - ( m_imgThumbLine[0]->GetHeight() / 2 );
		rcLine.bottom = rcLine.top + 2 + m_imgThumbLine[0]->GetHeight();
		rcLine.right = rcLine.left + m_imgThumbLine[0]->GetWidth();
		_RepeatDrawBtnState ( hDC, m_imgThumbLine, m_enumThumbMouseState, rcLine );
	}
}

VOID CSkinScrollBar::DrawThumb ( HDC hDC, CONST CRect& rcThumb )
{
	if ( NULL == hDC )
	{
		return;
	}
	//m_imgBack.Draw( hDC, rcThumb.left, rcThumb.top, rcThumb.Width(), rcThumb.Height() ); //NS
	if ( TRUE == m_imgThumbTop.IsValid() )
	{
		DrawThumbMiddle( hDC, rcThumb );
		DrawThumbTop( hDC, rcThumb );
		DrawThumbBottom( hDC, rcThumb );
		DrawThumbLine( hDC, rcThumb );
	}
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
VOID CSkinScrollBar::DrawScrollBar ( VOID )
{
	CPaintDC dc ( m_hWnd );

	CRect rcClient;
	GetClientRect ( rcClient );
	
	CPoint pt;
	::GetCursorPos ( &pt );
	::ScreenToClient ( m_hWnd, &pt );

	CMemoryDC memDC ( dc, rcClient );
	memDC.FillRect(rcClient, m_brBackground);

	CRect rcDest;
	if ( TRUE == m_imgBack.IsValid() )
	{
		rcDest = GetRect ( SB_PAGEUP );
		m_imgBack.Draw( memDC.m_hDC, rcDest.left, rcDest.top, rcDest.Width(), rcDest.Height() ); //NS
		rcDest = GetRect ( SB_PAGEDOWN );
		m_imgBack.Draw( memDC.m_hDC, rcDest.left, rcDest.top, rcDest.Width(), rcDest.Height() ); //NS
	}

	rcDest = GetRect ( SB_LINEUP );
	DrawButtonTop(memDC.m_hDC, rcDest, pt);

	rcDest = GetRect ( SB_LINEDOWN );
	DrawButtonBottom(memDC.m_hDC, rcDest, pt);

	rcDest = GetRect ( SB_THUMBTRACK );
	DrawThumb ( memDC.m_hDC, rcDest );
}

//////////////////////////////////////////////////////////////////////////

VOID CSkinScrollBar::DrawButtonTop(HDC hDC, const CRect& rcTop, const CPoint& pt)
{
	if ( TRUE == m_ilBtnUp.IsValid() )
	{
		if ( FALSE == m_bDrag && TRUE == rcTop.PtInRect ( pt ) )
		{
			if ( BT_UP == m_eClicked )
			{
				m_ilBtnUp[2]->Draw( hDC, rcTop.left, rcTop.top, rcTop.Width(), rcTop.Height() );
				//NDGraphics::DrawGdiplusImage ( hDC, m_ilBtnUp[2], rcTop.left, rcTop.top );
			}
			else
			{
				m_ilBtnUp[1]->Draw( hDC, rcTop.left, rcTop.top, rcTop.Width(), rcTop.Height() );
				//NDGraphics::DrawGdiplusImage ( hDC, m_ilBtnUp[1], rcTop.left, rcTop.top );
			}
		}
		else
		{
			m_ilBtnUp[0]->Draw( hDC, rcTop.left, rcTop.top, rcTop.Width(), rcTop.Height() );
			//NDGraphics::DrawGdiplusImage (hDC, m_ilBtnUp[0], rcTop.left, rcTop.top );
		}
		//_RepeatDrawBtnState( hDC, m_ilBtnUp, m_enumThumbMouseState, rcTop );
	}
}

VOID CSkinScrollBar::DrawButtonBottom( HDC hDC, const CRect& rcBottom, const CPoint& pt )
{
	if ( TRUE == m_ilBtnDown.IsValid() )
	{
		if ( FALSE == m_bDrag && TRUE == rcBottom.PtInRect ( pt ) )
		{
			if ( BT_DOWN == m_eClicked )
			{
				m_ilBtnDown[2]->Draw( hDC, rcBottom.left, rcBottom.top, rcBottom.Width(), rcBottom.Height() );
				//NDGraphics::DrawGdiplusImage ( hDC, m_ilBtnDown[2], rcBottom.left, rcBottom.bottom - m_ilBtnDown[2]->GetHeight() );
			}
			else
			{
				m_ilBtnDown[1]->Draw( hDC, rcBottom.left, rcBottom.top, rcBottom.Width(), rcBottom.Height() );
				//NDGraphics::DrawGdiplusImage ( hDC, m_ilBtnDown[1], rcBottom.left, rcBottom.bottom - m_ilBtnDown[2]->GetHeight() );
			}
		}
		else
		{
			m_ilBtnDown[0]->Draw( hDC, rcBottom.left, rcBottom.top, rcBottom.Width(), rcBottom.Height() );
			//NDGraphics::DrawGdiplusImage ( hDC, m_ilBtnDown[0], rcBottom.left, rcBottom.bottom - m_ilBtnDown[2]->GetHeight() );
		}
	}
}

BOOL CSkinScrollBar::SetSkinInfo ( CImage* pBtnUp, CImage* pBtnDown, CImage* pBack, CImage* pThumbTop,
								   CImage* pThumbBody, CImage* pThumbBottom, CImage* pThumbLine, 
								   HBRUSH hBrBackground/*, COLORREF crBack*/ ) // NS
{
	if ( TRUE == m_ilBtnUp.IsValid() )
	{
		m_ilBtnUp.Empty();
	}
	m_ilBtnUp.Create( pBtnUp, ImageFormatPNG, 4 );

	if ( TRUE == m_ilBtnDown.IsValid() )
	{
		m_ilBtnDown.Empty();
	}
	m_brBackground.Attach(hBrBackground);

	m_ilBtnDown.Create( pBtnDown, ImageFormatPNG, 4 );

	m_imgBack.Load( pBack, ImageFormatPNG );
	//m_imgBack.Create(pBack, ImageFormatPNG, 1);
	//m_imgBack = pBack;

	m_imgThumbTop.Create( pThumbTop, ImageFormatPNG, 4 );
	m_imgThumbBody.Create( pThumbBody, ImageFormatPNG, 4 );
	m_imgThumbBottom.Create( pThumbBottom, ImageFormatPNG, 4 );
	m_imgThumbLine.Create( pThumbLine, ImageFormatPNG, 4 );
	//m_crBack = crBack;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
VOID CSkinScrollBar::SetTargetWnd ( HWND hWnd )
{
	m_hTargetWnd = hWnd;
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
LRESULT CSkinScrollBar::OnCreate ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	m_bVertical = IsVertical();
	bHandled = FALSE;
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
LRESULT CSkinScrollBar::OnEraseBkGnd ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////
