#include "StdAfx.h"
#include "SkinScrollBar.h"
#include "SkinScrollWnd.h"
#include <atlimage.h>


#define TIMER_UPDATE 100

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
CSkinScrollWnd::CSkinScrollWnd ( void )
{
	m_hTargetWnd = NULL;
	m_fnOldWinProc = NULL;
	m_nScrollWidth = DEF_WIDTH;
	//m_crBack = RGB ( 255, 255, 255 );
	m_nPos = -1;
}

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
CSkinScrollWnd::~CSkinScrollWnd ( void )
{
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Windows Event Handler
//////////////////////////////////////////////////////////////////////////
LRESULT CSkinScrollWnd::OnSize ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	bHandled = FALSE;

	CSize size ( LOWORD ( lParam ), HIWORD ( lParam ) );

	if ( 0 == size.cx || 0 == size.cy )
	{
		return S_FALSE;
	}

	if ( TRUE == ::IsWindow ( m_MidWnd.m_hWnd ) )
	{
		//m_MidWnd.MoveWindow ( 0, 0, size.cx, size.cy, TRUE );
		//m_MidWnd.SetWindowPos(NULL, 0, 0, size.cx, size.cy, SWP_NOZORDER);
		m_MidWnd.GetWindow ( GW_CHILD ).MoveWindow ( 0, 0, size.cx, size.cy, TRUE );
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Windows Event Handler
//////////////////////////////////////////////////////////////////////////
LRESULT CSkinScrollWnd::OnVScroll ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if ( FALSE == ::IsWindow ( ( HWND ) lParam ) )
	{
		return 0;
	}

	CScrollBar ScrollBarWnd;

	ScrollBarWnd.Attach ( ( HWND ) lParam );

	CWindow Wnd = m_MidWnd.GetWindow ( GW_CHILD );

	INT32 nSBCode = LOWORD ( wParam );

	INT32 nPos = HIWORD ( wParam );

	m_bOperation = TRUE;

	Wnd.SendMessage ( WM_VSCROLL, MAKELONG ( nSBCode, nPos ), 0 );
	m_bOperation = FALSE;


	SCROLLINFO SInfo = { 0x00 };

	SInfo.cbSize = sizeof ( SInfo );

	SInfo.fMask = SIF_ALL;

	::GetScrollInfo ( Wnd.m_hWnd, SB_VERT, &SInfo );

	ScrollBarWnd.SetScrollInfo ( &SInfo );


	return 0;
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Windows Event Handler
//////////////////////////////////////////////////////////////////////////
LRESULT CSkinScrollWnd::OnHScroll ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if ( FALSE == ::IsWindow ( ( HWND ) lParam ) )
	{
		return S_OK;
	}

	CScrollBar ScrollBarWnd;

	ScrollBarWnd.Attach ( ( HWND ) lParam );

	CWindow Wnd = m_MidWnd.GetWindow ( GW_CHILD );

	INT32 nSBCode = LOWORD ( wParam );

	INT32 nPos = HIWORD ( wParam );

	m_bOperation = TRUE;
	Wnd.SendMessage ( WM_HSCROLL, MAKELONG ( nSBCode, nPos ), 0 );
	m_bOperation = FALSE;

	SCROLLINFO SInfo = { 0x00 };
	SInfo.cbSize = sizeof ( SInfo );
	SInfo.fMask = SIF_ALL;
	::GetScrollInfo ( Wnd.m_hWnd, SB_HORZ, &SInfo );
	ScrollBarWnd.SetScrollInfo ( &SInfo );

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Windows Event Handler
//////////////////////////////////////////////////////////////////////////
LRESULT CSkinScrollWnd::OnPaint ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	_DoPaint();
	return 0;
}

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Windows Event Handler
//////////////////////////////////////////////////////////////////////////
LRESULT CSkinScrollWnd::OnTimer ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if ( FALSE == m_MidWnd.IsWindow() || FALSE == ::IsWindow ( m_hTargetWnd ) )
	{
		return 0;
	}

	CWindow Wnd = m_MidWnd.GetWindow ( GW_CHILD );

	SCROLLINFO SInfo1;

	SCROLLINFO SInfo2;

	ZeroMemory ( &SInfo1, sizeof ( SInfo1 ) );

	ZeroMemory ( &SInfo2, sizeof ( SInfo2 ) );

	SInfo1.cbSize = sizeof ( SCROLLINFO );

	SInfo1.fMask = SIF_ALL;

	SInfo2.cbSize = sizeof ( SCROLLINFO );

	SInfo2.fMask = SIF_ALL;

	if ( m_sbVertical.IsWindowVisible() )
	{
		Wnd.GetScrollInfo ( SB_VERT, &SInfo1 );
		m_sbVertical.GetScrollInfo ( &SInfo2 );

		if ( SInfo1.nMax != SInfo2.nMax || SInfo1.nMin != SInfo2.nMin || SInfo1.nPos != SInfo2.nPos || SInfo1.nPage != SInfo2.nPage ) // NS
		{
			m_sbVertical.SetScrollInfo ( &SInfo1 );
		}
	}

	if ( m_sbHorizontal.IsWindowVisible() )
	{
		Wnd.GetScrollInfo( SB_HORZ, &SInfo1 );
		m_sbHorizontal.GetScrollInfo( &SInfo2 );

		if ( SInfo1.nMax != SInfo2.nMax || SInfo1.nMin != SInfo2.nMin || SInfo1.nPos != SInfo2.nPos || SInfo1.nPage != SInfo2.nPage ) // NS
		{
			m_sbHorizontal.SetScrollInfo( &SInfo1 );
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Windows Procedure
//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK CSkinScrollWnd::WindowProc ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	CSkinScrollWnd* pWnd = reinterpret_cast<CSkinScrollWnd*> ( ::GetWindowLong ( hWnd, GWL_USERDATA ) );
	if ( TRUE == ::IsBadReadPtr ( pWnd, sizeof ( *pWnd ) ) )
	{
		return S_FALSE;
	}
	LRESULT lResult = ::CallWindowProc ( pWnd->m_fnOldWinProc, hWnd, uMsg, wParam, lParam );
	if ( TRUE == pWnd->m_bOperation )
	{
		return lResult;
	}

	if ( WM_ERASEBKGND == uMsg )
	{
		SCROLLINFO SInfo;
		ZeroMemory ( &SInfo, sizeof ( SInfo ) );
		SInfo.cbSize = sizeof ( SInfo );
		SInfo.fMask = SIF_ALL;

		DWORD dwStyle = ::GetWindowLong ( hWnd, GWL_STYLE );

		if ( dwStyle & WS_VSCROLL )
		{
			::GetScrollInfo ( hWnd, SB_VERT, &SInfo );
			pWnd->m_sbVertical.SetScrollInfo ( &SInfo );
			pWnd->m_sbVertical.EnableWindow ( SInfo.nMax >= ( int ) SInfo.nPage );
			if ( TRUE == pWnd->m_sbVertical.IsWindow() && TRUE == pWnd->m_sbVertical.IsWindowVisible() )
			{
				pWnd->m_sbVertical.Invalidate();
				pWnd->m_sbVertical.UpdateWindow();
			}
		}

		if ( dwStyle & WS_HSCROLL )
		{
			::GetScrollInfo( hWnd, SB_HORZ, &SInfo );
			pWnd->m_sbHorizontal.SetScrollInfo( &SInfo );
			pWnd->m_sbHorizontal.EnableWindow( SInfo.nMax >= ( int )SInfo.nPage );
			if ( TRUE == pWnd->m_sbHorizontal.IsWindow() && TRUE == pWnd->m_sbHorizontal.IsWindowVisible() )
			{
				pWnd->m_sbHorizontal.Invalidate();
				pWnd->m_sbHorizontal.UpdateWindow();
			}
		}
	}
	else if ( WM_NCCALCSIZE == uMsg && wParam )
	{
		LPNCCALCSIZE_PARAMS pNcCalcSizeParam = reinterpret_cast<LPNCCALCSIZE_PARAMS> ( lParam );
		DWORD dwStyle = ::GetWindowLong ( hWnd, GWL_STYLE );
		DWORD dwExStyle = ::GetWindowLong ( hWnd, GWL_EXSTYLE );
		BOOL  bLeftScroll = dwExStyle & WS_EX_LEFTSCROLLBAR;
		int nWidth = ::GetSystemMetrics ( SM_CXVSCROLL );

		if ( dwStyle & WS_VSCROLL )
		{
			if ( bLeftScroll )
			{
				pNcCalcSizeParam->rgrc[0].left -= nWidth - pWnd->m_nScrollWidth;
			}
			else
			{
				pNcCalcSizeParam->rgrc[0].right += nWidth - pWnd->m_nScrollWidth;
			}
		}

		if ( dwStyle & WS_HSCROLL )
		{
			pNcCalcSizeParam->rgrc[0].bottom += nWidth - pWnd->m_nScrollWidth;
		}

		CRect rcVert, rcHorz;

		nWidth = pWnd->m_nScrollWidth;

		if ( TRUE == bLeftScroll )
		{
			int nLeft = pNcCalcSizeParam->rgrc[0].left;
			int nBottom = pNcCalcSizeParam->rgrc[0].bottom;
			rcVert.SetRect ( nLeft - nWidth, 0, nLeft, nBottom );
			rcHorz.SetRect ( nLeft, nBottom, pNcCalcSizeParam->rgrc[0].right, nBottom + nWidth );
		}

		else
		{
			int nRight = pNcCalcSizeParam->rgrc[0].right;
			int nBottom = pNcCalcSizeParam->rgrc[0].bottom;
			rcVert.SetRect ( nRight, 0, nRight + nWidth, nBottom );
			rcHorz.SetRect ( 0, nBottom, nRight, nBottom + nWidth );
		}

		if ( ( dwStyle & WS_VSCROLL ) && ( dwStyle & WS_HSCROLL ) )
		{
			pWnd->m_nAngleType = bLeftScroll ? 1 : 2;
		}
		else
		{
			pWnd->m_nAngleType = 0;
		}

		if ( dwStyle & WS_VSCROLL )
		{
			pWnd->m_sbVertical.SetWindowPos ( NULL, rcVert, SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW | SWP_NOREDRAW );
			//pWnd->m_sbVertical.PostMessage(WM_VSCROLL, SB_ENDSCROLL, (LPARAM)pWnd->m_sbVertical.m_hWnd);
		}

		else
		{
			pWnd->m_sbVertical.ShowWindow ( SW_HIDE );
		}

		if ( dwStyle & WS_HSCROLL )
		{
			pWnd->m_sbHorizontal.SetWindowPos ( NULL, rcHorz, SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW | SWP_NOREDRAW );
		}
		else
		{
			pWnd->m_sbHorizontal.ShowWindow( SW_HIDE );
		}



		pWnd->SendMessage ( UM_DESTMOVE, ( WPARAM )hWnd, bLeftScroll );
		pWnd->Invalidate(FALSE);
		// pWnd->ShowWindow(SW_SHOWNOACTIVATE);
	}

	return lResult;
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
BOOL CSkinScrollWnd::Initialize ( CONST HWND hWnd, CONST SCROLLBAR_SKIN_INFO& ResSet )
{
	if ( FALSE == ::IsWindow ( hWnd ) )
	{
		return FALSE;
	}

	if ( FALSE == SetSkinInfo ( ResSet ) )
	{
		return FALSE;
	}

	m_imgCorner.Load(ResSet.pImageCorner, ImageFormatPNG);
	m_brBackground.Attach(ResSet.hBrBackground);

	CWindow ParentWnd = ::GetParent ( hWnd );

	CRect rcFrm, rcWnd;

	::GetWindowRect ( hWnd, &rcFrm );

	ParentWnd.ScreenToClient ( &rcFrm );

	rcWnd = rcFrm;

	rcWnd.OffsetRect ( -rcWnd.left, -rcWnd.top );

	DWORD dwStyle = ::GetWindowLong ( hWnd, GWL_STYLE );

	DWORD dwFrmStyle = WS_CHILD | SS_NOTIFY ;

	DWORD dwFrmStyleEx = 0;

	if ( dwStyle & WS_VISIBLE )
	{
		dwFrmStyle |= WS_VISIBLE;
	}

	if ( dwStyle & WS_BORDER )
	{
		dwStyle &= ~WS_BORDER;
		::SetWindowLong ( hWnd, GWL_STYLE, dwStyle );

		int nBorder = ::GetSystemMetrics ( SM_CXBORDER );
		rcWnd.right -= nBorder * 2;
		rcWnd.bottom -= nBorder * 2;
		dwFrmStyle |= WS_BORDER;
	}

	DWORD dwExStyle = ::GetWindowLong ( hWnd, GWL_EXSTYLE );

	if ( dwExStyle & WS_EX_CLIENTEDGE )
	{
		dwExStyle &= ~WS_EX_CLIENTEDGE;
		::SetWindowLong ( hWnd, GWL_EXSTYLE, dwExStyle );

		int nCXEDGE = ::GetSystemMetrics ( SM_CXEDGE );

		rcWnd.right -= nCXEDGE * 2;
		rcWnd.bottom -= nCXEDGE * 2;
		dwFrmStyleEx |= WS_EX_CLIENTEDGE;
	}

	this->Create ( ParentWnd.m_hWnd, rcFrm, _T ( "" ), dwFrmStyle, dwFrmStyleEx );

	m_MidWnd.Create ( m_hWnd, NULL, _T ( "" ), WS_CHILD | WS_VISIBLE /*| WS_CLIPCHILDREN | WS_CLIPSIBLINGS */ );
	m_sbHorizontal.Create( m_hWnd, NULL, NULL, WS_CHILD | SBS_HORZ );
	m_sbVertical.Create ( m_hWnd, NULL, NULL, WS_CHILD | SBS_VERT );

	::SetParent ( hWnd, m_MidWnd.m_hWnd );

	::SetWindowLong ( hWnd, GWL_USERDATA, ( LONG ) this );
	m_fnOldWinProc = ( WNDPROC ) ::SetWindowLong ( hWnd, GWL_WNDPROC, ( LONG ) WindowProc );

	m_hTargetWnd = hWnd;

	::SetWindowPos ( hWnd, NULL, rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom,
					 SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOACTIVATE );

	::SetTimer ( m_hWnd, TIMER_UPDATE, 100, NULL );

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
LRESULT CSkinScrollWnd::OnDestMove ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if ( !m_MidWnd.IsWindow() || !m_sbVertical.IsWindow() || !m_sbHorizontal.IsWindow() ) // || FALSE == IsWindowVisible() ) // NS
	{
		return 0;
	}
	HWND hWnd = ( HWND )( wParam );

	m_bOperation = TRUE;
	DWORD dwStyle = ::GetWindowLong ( hWnd , GWL_STYLE );


	BOOL bLeftScroll = static_cast<BOOL> ( lParam );

	RECT rcClient;

	GetClientRect ( &rcClient );

	CRect rcLimit, rcWnd;

	rcWnd = rcClient;

	rcLimit = rcClient;


	//if ( m_sbHorizontal.IsWindowVisible() )
	if ( dwStyle & WS_HSCROLL )
	{
		rcLimit.bottom -= m_nScrollWidth;
	}

	if ( bLeftScroll )
	{
		if (  dwStyle & WS_VSCROLL )
		{
			rcLimit.left += m_nScrollWidth;
			rcWnd.OffsetRect ( -m_nScrollWidth, 0 );
		}
	}
	else if ( dwStyle & WS_VSCROLL )
	{
		rcLimit.right -= m_nScrollWidth;
	}

	m_MidWnd.MoveWindow ( &rcLimit, TRUE );
	ShowWindow ( SW_SHOWNOACTIVATE );

	CWindow wnd = m_MidWnd.GetWindow ( GW_CHILD );
	wnd.MoveWindow(&rcClient, TRUE);
	//wnd.SetWindowPos ( HWND_TOPMOST, &rcClient, SWP_NOSIZE | SWP_NOACTIVATE );
	//wnd.Invalidate(TRUE);
	//wnd.UpdateWindow();
	//wnd.SendMessage(WM_PAINT, 0,0);
	m_bOperation = FALSE;

	return 0;
}

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
BOOL CSkinScrollWnd::SetSkinInfo ( CONST SCROLLBAR_SKIN_INFO& ResSet)
{
	//if ( ResSet.pVImgThumbBody )
	//{
	//	m_nScrollWidth = ResSet.pVImgThumbBody->GetWidth() / 4 + 1;  //17
	//}

	if ( ResSet.pVImgUp )
	{
		m_sbVertical.SetSkinInfo ( ResSet.pVImgUp ,ResSet.pVImgDown,ResSet.pVImgBack,
									ResSet.pVImgThumbTop, ResSet.pVImgThumbBody, ResSet.pVImgThumbBottom,
									ResSet.pVImgThumbLine, ResSet.hBrBackground);
	}

	if ( ResSet.pHImgLeft )
	{
		m_sbHorizontal.SetSkinInfo( ResSet.pHImgLeft , ResSet.pHImgRight, ResSet.pHImgBack,
									ResSet.pHImgThumbLeft, ResSet.pHImgThumbBody, ResSet.pHImgThumbRight,
									ResSet.pHImgThumbLine, ResSet.hBrBackground);
	}

	if ( ResSet.pVImgThumbBody )
	{
		m_sbVertical.setWidth( DEF_WIDTH ); // ResSet.pVImgThumbBody->GetWidth() / 4 );
	}
	if ( ResSet.pHImgThumbBody )
	{
		m_sbHorizontal.setWidth( DEF_WIDTH ); // ResSet.pHImgThumbBody->GetHeight() / 4);
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
BOOL CSkinScrollWnd::Finalize ( VOID )
{
	if ( NULL != m_fnOldWinProc )
	{
		::SetParent ( m_hTargetWnd, ::GetParent ( m_hWnd ) );

		::SetWindowLong ( m_hTargetWnd, GWL_WNDPROC, ( LONG ) m_fnOldWinProc );
		m_hTargetWnd = NULL;
		m_fnOldWinProc = NULL;

		if ( TRUE == m_MidWnd.IsWindow() )
		{
			m_MidWnd.DestroyWindow();
		}

		if ( TRUE == m_sbHorizontal.IsWindow() )
		{
			m_sbHorizontal.DestroyWindow();
		}

		if ( TRUE == m_sbVertical.IsWindow() )
		{
			m_sbVertical.DestroyWindow();
		}

		if ( TRUE == IsWindow() )
		{
			DestroyWindow();
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
LRESULT CSkinScrollWnd::OnEraseBaGnd ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
LRESULT CSkinScrollWnd::OnRButtonDown ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{

	return 0;
}
LRESULT CSkinScrollWnd::OnDestroy( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 0;
}


LRESULT CSkinScrollWnd::OnHThumbTrack( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if ( FALSE == ::IsWindow ( ( HWND ) lParam ) )
	{
		return 0;
	}
	CScrollBar ScrollBarWnd;
	ScrollBarWnd.Attach ( ( HWND ) lParam );
	CWindow Wnd = m_MidWnd.GetWindow ( GW_CHILD );
	INT32 nPos =  wParam ;
	m_bOperation = TRUE;
	if ( m_nPos != nPos )
	{
		Wnd.SendMessage ( WM_HSCROLL, MAKELONG ( SB_THUMBTRACK, nPos ),  nPos  );
	}
	m_nPos = nPos;
	m_bOperation = FALSE;
	SCROLLINFO SInfo = { 0x00 };
	SInfo.cbSize = sizeof ( SInfo );
	SInfo.fMask = SIF_ALL;
	::GetScrollInfo ( Wnd.m_hWnd, SB_HORZ, &SInfo );
	ScrollBarWnd.SetScrollInfo ( &SInfo );
	return 0;
}
LRESULT CSkinScrollWnd::OnVThumbTrack( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if ( FALSE == ::IsWindow ( ( HWND ) lParam ) )
	{
		return 0;
	}
	CScrollBar ScrollBarWnd;
	ScrollBarWnd.Attach ( ( HWND ) lParam );
	CWindow Wnd = m_MidWnd.GetWindow ( GW_CHILD );
	INT32 nPos =  wParam ;
	m_bOperation = TRUE;
	if ( m_nPos != nPos )
	{
		Wnd.SendMessage ( WM_VSCROLL, MAKELONG ( SB_THUMBTRACK, nPos ), nPos  );
	}
	m_nPos = nPos;
	m_bOperation = FALSE;
	SCROLLINFO SInfo = { 0x00 };
	SInfo.cbSize = sizeof ( SInfo );
	SInfo.fMask = SIF_ALL;
	::GetScrollInfo ( Wnd.m_hWnd, SB_VERT, &SInfo );
	ScrollBarWnd.SetScrollInfo ( &SInfo );

	return 0;
}

LRESULT CSkinScrollWnd::OnUpdateScrollCorner( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if ( FALSE == ::IsWindow ( ( HWND ) lParam ) )
	{
		return 0;
	}
	
	_DoPaint();
	
	return 0;
}

BOOL CSkinScrollWnd::_DoPaint()
{
	//bHandled = FALSE;
	CPaintDC dc ( m_hWnd );

	CRect rcClient;
	GetClientRect ( &rcClient );

	CRect rcFill;
	rcFill.SetRectEmpty();
	INT32 nLeft = 0, nTop = 0;

	if ( 1 == m_nAngleType ) // LeftScrollBar
	{
		nLeft = rcClient.left;
		nTop = rcClient.bottom - m_nScrollWidth;
		rcFill.SetRect ( nLeft, nTop, nLeft + m_nScrollWidth, nTop + m_nScrollWidth );
	}
	else if ( 2 == m_nAngleType )
	{
		nLeft = rcClient.right - m_nScrollWidth;
		nTop = rcClient.bottom - m_nScrollWidth;
		rcFill.SetRect ( nLeft, nTop, nLeft + m_nScrollWidth, nTop + m_nScrollWidth );
	}

	if ( !rcFill.IsRectEmpty() )
	{
		CRgn rgn;
		rgn.CreateRectRgnIndirect(&rcFill);
		dc.SelectClipRgn(rgn);


		dc.FillRect(rcFill, m_brBackground);
		m_imgCorner.Draw( dc.m_hDC, rcFill.left, rcFill.top, rcFill.Width(), rcFill.Height() );

	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////



