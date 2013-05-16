// CButtonSSL.cpp : implementation file
//
#include "stdafx.h"
#include "ButtonSSL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DROP_BUTTON_WIDTH	16
#define BS_TYPEMASK			SS_TYPEMASK

//#define TIMER_MOUSE_OVER  1
#define TIMER_MOUSE_OVER 0x3CE6C7AF // Abin, try to make it unique!

/////////////////////////////////////////////////////////////////////////////
// CButtonSSL

/////////////////////////////////////////////////////////////////////////////
// Construction

CButtonSSL::CButtonSSL()
{
	m_bDefault = FALSE;
	m_bCanBeDefault = TRUE;
	
	// Set to an invalid value as we don't know what type of button we are yet
	m_nTypeStyle = BS_TYPEMASK;

	m_nSubMenuPos = 0;
	m_bImageLoaded = FALSE;
	m_bMenuLoaded = FALSE;
	m_bMenuPushed = FALSE;
	m_bBtnPushed = FALSE;
	m_bMouseOnBtn = FALSE;
	
	m_nStyle = SSL_BS_FLAT;
	m_nTextAlign = SSL_TEXT_CENTER | SSL_TEXT_VCENTER;
	m_nImageAlign = SSL_IMAGE_LEFT | SSL_IMAGE_TOP;
	m_nImageWidth = 0;
	m_nImageHeight = 0;

	m_ToolTip.m_hWnd = NULL;
	m_hCursor = NULL;
	// Fix for GDI Resource leak. Thanks to Dieter Hammer
	m_hIcon = NULL;
	SetSSLDefaultColors(FALSE);

	ZeroMemory(&m_bmp, sizeof m_bmp);
	ZeroMemory(&m_szURL, sizeof(m_szURL));

	m_bsState = SSL_STATE_UP;
	m_nCheck = 0;
}

/////////////////////////////////////////////////////////////////////////////
// Destruction

CButtonSSL::~CButtonSSL()
{
	m_font.DeleteObject();
	// Fix for GDI Resource leak. Thanks to Dieter Hammer
	if(NULL != m_hIcon) 
	{
	   ::DestroyIcon(m_hIcon);
	}
}

BEGIN_MESSAGE_MAP(CButtonSSL, CButton)
	//{{AFX_MSG_MAP(CButtonSSL)
	ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETFOCUS()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	ON_WM_GETDLGCODE()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(BM_SETSTYLE, OnSetStyle)
	ON_MESSAGE(BM_GETCHECK, OnGetCheck)
	ON_MESSAGE(BM_SETCHECK, OnSetCheck)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButtonSSL message handlers

void CButtonSSL::PreSubclassWindow()
{
	// Set the initial button type
	m_nTypeStyle = GetButtonStyle() & BS_TYPEMASK;

	// Set the initial default state flag
	if(BS_DEFPUSHBUTTON == m_nTypeStyle)
	{
		// Enable default state handling for push buttons
		m_bCanBeDefault = TRUE;
		// Set the default state for a default button
		m_bDefault = TRUE;

		// Adjust the style for a default push button
		m_nTypeStyle = BS_PUSHBUTTON;
	}
	else
	{
		// Enable default state handling for non-push buttons
		m_bCanBeDefault = TRUE;
	}

	// The Owner Drawn style must not be set prior to this call
	// i.e Don't set it the resource editor or call
	// ModifyStyle(NULL, BS_OWNERDRAW) before calling PreSubclassWindow()
	// Otherwise we can't work out what kind of control we are
	ASSERT(m_nTypeStyle != BS_OWNERDRAW);

	// Now we can set the Owner Drawn flag
	ModifyStyle(BS_TYPEMASK, BS_OWNERDRAW, SWP_FRAMECHANGED);

	CButton::PreSubclassWindow();

	// Establish the font
	if(NULL ==(HFONT)m_font)
	{
		// If a font isn't already set, get the default font
		CFont* pFont = GetFont();
		if(!pFont) 
		{
			HFONT hFont =(HFONT)GetStockObject(DEFAULT_GUI_FONT);
			if(NULL == hFont) 
			{
				hFont =(HFONT) GetStockObject(ANSI_VAR_FONT);
			}
			if(hFont) 
			{
				pFont = CFont::FromHandle(hFont);
			}
		}
		ASSERT(pFont->GetSafeHandle());
		LOGFONT lf;
		pFont->GetLogFont(&lf);
		m_font.CreateFontIndirect(&lf);
	}
}

UINT CButtonSSL::OnGetDlgCode() 
{
	UINT nCode = CButton::OnGetDlgCode();

	// Determine if we should have the default state or not
    UINT nType = GetControlType ();
	if (BS_RADIOBUTTON == (BS_RADIOBUTTON & nType) ||
		BS_AUTORADIOBUTTON == (BS_AUTORADIOBUTTON & nType))
	{
		nCode |= DLGC_RADIOBUTTON;
	}
	else 
	{
	    if (m_bCanBeDefault || m_bDefault)
		{
		    nCode |= (m_bDefault ? DLGC_DEFPUSHBUTTON : DLGC_UNDEFPUSHBUTTON);
		}
    }

	return nCode;
}

LRESULT CButtonSSL::OnSetStyle(WPARAM wParam, LPARAM lParam)
{
	UINT nNewType = (wParam & BS_TYPEMASK);

	// Update the default state flag
	if (nNewType == BS_DEFPUSHBUTTON)
	{
		// Default state handling must have been enabled
		ASSERT(m_bCanBeDefault);

		m_bDefault = TRUE;
	}
	else if (nNewType == BS_PUSHBUTTON)
	{
		// We can always lose the default state
		m_bDefault = FALSE;
	}

	// We can't change the control type after the owner drawn style is set.
	// Let the system process changes to other style bits and redrawing, 
	// while keeping the owner drawn style
	return DefWindowProc(BM_SETSTYLE, (wParam & ~BS_TYPEMASK) | BS_OWNERDRAW, lParam);
}

// Thanks to Tomasz Sowinski for helping me with the DDX problems
// Many thanks to Joe Newcomer for pointing out the error of my ways
// by not supplying all of the requried parameters
LRESULT CButtonSSL::OnGetCheck(WPARAM wParam, LPARAM lParam) 
{
	UINT nType = GetControlType();
	if(BS_CHECKBOX ==(BS_CHECKBOX & nType) || 
		BS_AUTOCHECKBOX ==(BS_AUTOCHECKBOX & nType) ||
		BS_RADIOBUTTON ==(BS_RADIOBUTTON & nType) || 
		BS_AUTORADIOBUTTON ==(BS_AUTORADIOBUTTON & nType)) 
	{
		if(0 == m_nCheck) 
		{
			return BST_UNCHECKED;
		}
		else 
		{
			return BST_CHECKED;
		}
	}
	return 0;
}

// Thanks to Tomasz Sowinski for helping me with the DDX problems
// Many thanks to Joe Newcomer for pointing out the error of my ways
// by not supplying all of the requried parameters
LRESULT CButtonSSL::OnSetCheck(WPARAM fCheck, LPARAM lParam) 
{
	if(BST_CHECKED == fCheck) 
	{
		m_nCheck = 1;
		m_bBtnPushed = TRUE;
	}
	else if(BST_UNCHECKED == fCheck) 
	{
		m_nCheck = 0;
		m_bBtnPushed = FALSE;
	}

	Invalidate();

	return 0;
}

BOOL CButtonSSL::OnClicked() {
	// Update the check state for radio buttons and check boxes
	UINT nType = GetControlType();
	if(BS_CHECKBOX ==(BS_CHECKBOX & nType) || BS_AUTOCHECKBOX==(BS_AUTOCHECKBOX & nType)) 
	{
		m_nCheck = !m_nCheck;
		m_bBtnPushed = !m_bBtnPushed;
		Invalidate();
	}
	else if(BS_RADIOBUTTON ==(BS_RADIOBUTTON & nType) ||
			BS_AUTORADIOBUTTON ==(BS_AUTORADIOBUTTON & nType)) 
	{
        // Send BM_SETCHECK(BST_CHECKED) to this button and
        // BM_SETCHECK(BST_UNCHECKED) to all other buttons in this group
        SendMessage(BM_SETCHECK, BST_CHECKED, 0);
        // Start by searching forwards
        HWND hWndDlg = this->GetParent()->m_hWnd;
        CWnd* pWnd = FromHandle(::GetNextDlgGroupItem(hWndDlg, this->m_hWnd, FALSE));
        while(NULL != pWnd && this != pWnd) 
		{
            pWnd->SendMessage(BM_SETCHECK, BST_UNCHECKED, 0);
            pWnd = FromHandle(::GetNextDlgGroupItem(hWndDlg, pWnd->m_hWnd, FALSE));
        }
        // Then search backwards
        pWnd = FromHandle(::GetNextDlgGroupItem(hWndDlg, this->m_hWnd, TRUE));
        while(NULL != pWnd && this != pWnd) 
		{
            pWnd->SendMessage(BM_SETCHECK, BST_UNCHECKED, 0);
            pWnd = FromHandle(::GetNextDlgGroupItem(hWndDlg, pWnd->m_hWnd, TRUE));
        }
	}
	else 
	{
		// Handle the URL(if any)
		if(::lstrlen(m_szURL) > 0) 
		{
			::ShellExecute(NULL, _T("open"), m_szURL, NULL, NULL, SW_SHOWMAXIMIZED);
		}
	}
	
	return FALSE; // Allow the parent to handle the click event
}

void CButtonSSL::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd*	pWnd;		// Active window
	CWnd*	pParent;	// Window that owns the button

	CButton::OnMouseMove(nFlags, point);

	// Ignore if left button pressed on entering button
	if((nFlags & MK_LBUTTON) && FALSE == m_bMouseOnBtn) 
	{
		return;
	}

	pWnd = GetActiveWindow();
	pParent = GetOwner();

	if((GetCapture() != this) &&(pParent != NULL)) 
	{
		m_bMouseOnBtn = TRUE;
		// Start the timer to check for the mouse leaving
		SetTimer(TIMER_MOUSE_OVER, 100, NULL);
		SetCapture();
		Invalidate();
	}
	else 
	{
		ClientToScreen(&point);
		CWnd* wndUnderMouse = WindowFromPoint(point);
		if(NULL != wndUnderMouse && wndUnderMouse->m_hWnd != this->m_hWnd) 
		{
			// Redraw only if mouse goes out
			if(TRUE == m_bMouseOnBtn) 
			{
				m_bMouseOnBtn = FALSE;
				Invalidate();
			}
			// Release capture if the left button is not pressed
			if(!(nFlags & MK_LBUTTON)) 
			{
				ReleaseCapture();
			}
		}
	}
}

void CButtonSSL::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_bMenuPushed) 
	{
		m_bMenuPushed = FALSE;
		Invalidate();
		return;
	}

	// Show the menu if the click is on the drop-arrow
	// and the SSL_BS_MENU_BTN style is set
	if(HitMenuButton(point) &&(SSL_BS_MENU_BTN & m_nStyle)) 
	{
		m_bMenuPushed = TRUE;
		Invalidate();

		CRect rect;
		GetWindowRect(rect);

		// Modified by Abin, 10/20/2003
		// Popup menu should be aligned to the left edge
		int x = rect.left;
		//int x = rect.right;
		int y = rect.bottom;

		// Modified by Abin, 11/07/2003,
		// Enabled sub menu position and menu item update
		CMenu* pMenu = m_menu.GetSubMenu(m_nSubMenuPos);
		CmdRouteMenu(pMenu);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, x, y, GetParent());
		//pMenu->TrackPopupMenu(TPM_RIGHTALIGN | TPM_LEFTBUTTON, x, y, GetParent());

		m_bMenuPushed = FALSE;
	}
	else 
	{
		m_bBtnPushed = TRUE;
	}

	Invalidate();
	CButton::OnLButtonDown(nFlags, point);
}

void CButtonSSL::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CButton::OnLButtonUp(nFlags, point);

	// Check box state updating done in Clicked handler
	UINT nType = GetControlType();
	if(BS_CHECKBOX !=(BS_CHECKBOX & nType) &&
		BS_AUTOCHECKBOX !=(BS_AUTOCHECKBOX & nType) &&
		BS_RADIOBUTTON !=(BS_RADIOBUTTON & nType) &&
		BS_AUTORADIOBUTTON !=(BS_AUTORADIOBUTTON & nType)) 
	{
		if(TRUE == m_bBtnPushed) 
		{
			m_bBtnPushed = FALSE;
			ReleaseCapture();
			Invalidate();
		}
	}
}

void CButtonSSL::OnSetFocus(CWnd* pOldWnd) 
{
	CButton::OnSetFocus(pOldWnd);
	
	Invalidate();
}

BOOL CButtonSSL::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// Use the specifed cursor if there is one
	if(NULL != m_hCursor) 
	{
		::SetCursor(m_hCursor);
		return TRUE;
	}
	
	return CButton::OnSetCursor(pWnd, nHitTest, message);
}

// Fix for button state problem. Inspired by Chris Maunder's control
// subclassing article
void CButtonSSL::OnTimer(UINT nIDEvent) 
{
	if(TIMER_MOUSE_OVER == nIDEvent) 
	{
		// Get the current mouse position
		CPoint Point(GetMessagePos());
		ScreenToClient(&Point);

		// Get the bounds of the button
		CRect Rect;
		GetClientRect(Rect);

		// Check if the mouse is over the button

		// Modified by Abin, 10/30/2003
		// Do not reset if menu is still pushed down!
		//if(FALSE == Rect.PtInRect(Point)) 
		if (!m_bMenuPushed && !Rect.PtInRect(Point))
		{
			// Update the state if it isn't
			m_bMouseOnBtn = false;
			KillTimer(TIMER_MOUSE_OVER);
			// and redraw the button
			Invalidate();
		}
	}

	CButton::OnTimer(nIDEvent);
}

HBRUSH CButtonSSL::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	return(HBRUSH)::GetStockObject(NULL_BRUSH); 
}

void CButtonSSL::DrawBackground(CDC* pDC, LPCRECT pRect) 
{
	COLORREF crColor = ::GetSysColor(COLOR_BTNFACE);

	if(SSL_STATE_UP == m_bsState) 
	{
		crColor = m_crColors[SSL_UP_BK_COLOR];
	}
	if(SSL_STATE_OVER == m_bsState) 
	{
		crColor = m_crColors[SSL_OVER_BK_COLOR];
	}
	if(SSL_STATE_DOWN == m_bsState) 
	{
		crColor = m_crColors[SSL_DOWN_BK_COLOR];
	}

	CBrush		brBackground(crColor);

	pDC->FillRect(pRect, &brBackground);
}

void CButtonSSL::DrawBorder(CDC* pDC, LPRECT lpRect) 
{
	if(TRUE == m_bBtnPushed) 
	{
		pDC->Draw3dRect(lpRect, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHIGHLIGHT));
		return;
	}
	if(!(SSL_BS_FLAT & m_nStyle)) 
	{
		if(TRUE == m_bBtnPushed) 
		{
			pDC->DrawFrameControl(lpRect, DFC_BUTTON, DFCS_BUTTONPUSH | DFCS_PUSHED);
		}
		else 
		{
			pDC->DrawFrameControl(lpRect, DFC_BUTTON, DFCS_BUTTONPUSH);
		}
		return;
	}
	if(TRUE == m_bMouseOnBtn && FALSE == m_bBtnPushed) 
	{
		pDC->Draw3dRect(lpRect, ::GetSysColor(COLOR_BTNHIGHLIGHT), ::GetSysColor(COLOR_BTNSHADOW));
	}
}

void CButtonSSL::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if(lpDrawItemStruct->CtlType != ODT_BUTTON) 
	{
		return;
	}

	CDC cdcMem;
	CBitmap bmp;

	CRect btnRect(lpDrawItemStruct->rcItem);
	CRect trueRect(btnRect);

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	// Check to see if it's a checked checkbox or radio button
	UINT nType = GetControlType();
	if((BS_AUTORADIOBUTTON ==(BS_AUTORADIOBUTTON & nType) ||
		BS_RADIOBUTTON ==(BS_RADIOBUTTON & nType) ||
		BS_AUTOCHECKBOX ==(BS_AUTOCHECKBOX & nType) || 
		BS_CHECKBOX ==(BS_CHECKBOX & nType)) && 
		FALSE == m_bBtnPushed) 
	{
		if(0 == m_nCheck) 
		{
			m_bBtnPushed = FALSE;
		}
		else 
		{
			m_bBtnPushed = TRUE;
		}
	}

	// Check to see if the button is disabled
	m_bDisabled =(ODS_DISABLED ==(ODS_DISABLED & lpDrawItemStruct->itemState));

	// Update the button state
	if(TRUE == m_bDisabled) 
	{
		m_bsState = SSL_STATE_DISABLED;
	}
	else 
	{
		m_bsState = SSL_STATE_UP;
		if(TRUE == m_bMouseOnBtn) 
		{
			m_bsState = SSL_STATE_OVER;
		}
		if(TRUE == m_bBtnPushed) 
		{
			m_bsState = SSL_STATE_DOWN;
		}
	}

#ifdef _TRACESTATE
	TraceState(m_bsState);
#endif
	
	if(TRUE == IsDefault()) 
	{
		btnRect.DeflateRect(1, 1);
	}

	// Button Background and Border
	DrawBackground(pDC, &trueRect);
	DrawBorder(pDC, &btnRect);

	CRect rectFocus(btnRect);	// Focus rectangle
	rectFocus.DeflateRect(3, 3);

	if(SSL_BS_MENU_BTN & m_nStyle) 
	{
		rectFocus.right -= DROP_BUTTON_WIDTH;	// Exclude drop button from focus rectangle
	}
	// Default Button State
	if(TRUE == IsDefault() && FALSE == m_bDisabled && !(SSL_BS_FLAT & m_nStyle)) 
	{
		pDC->FrameRect(&lpDrawItemStruct->rcItem, 
			CBrush::FromHandle((HBRUSH)GetStockObject(BLACK_BRUSH)));
		if(TRUE == m_bBtnPushed && FALSE == m_bMenuPushed) 
		{
			pDC->FrameRect(&btnRect, CBrush::FromHandle(GetSysColorBrush(COLOR_3DSHADOW))); 
		}
	}

	// State Focus
	if((lpDrawItemStruct->itemState & ODS_FOCUS || TRUE == m_bBtnPushed) && !(SSL_BS_FLAT & m_nStyle) &&
		BS_AUTOCHECKBOX !=(BS_AUTOCHECKBOX & nType) && BS_CHECKBOX !=(BS_CHECKBOX & nType)) 
	{
		if(FALSE == m_bMenuPushed) 
		{
			pDC->DrawFocusRect(&rectFocus);
		}
	}

	// Action Focus
	if((lpDrawItemStruct->itemAction & ODA_FOCUS) && !(SSL_BS_FLAT & m_nStyle)) 
	{
		if(FALSE == m_bMenuPushed) 
		{
			pDC->DrawFocusRect(&rectFocus);
		}
	}

	// Draw the image on the button.
	// Extract the image as an icon regardless of what was added originally
	if(TRUE == m_bImageLoaded) 
	{
		HICON hIcon = m_imageList.ExtractIcon(m_bsState);
		// Defer to the Up image. This should only happen if there isn't a disabled image
		if(NULL == hIcon) 
		{
			hIcon = m_imageList.ExtractIcon(SSL_STATE_UP);
			pDC->DrawState(GetImagePoint(trueRect),
				CSize(m_nImageWidth, m_nImageHeight), hIcon, 
				DST_ICON |(m_bsState == SSL_STATE_DISABLED ? DSS_DISABLED : DSS_NORMAL),(CBrush*)NULL);
		}
		else 
		{
			pDC->DrawState(GetImagePoint(trueRect),
				CSize(m_nImageWidth, m_nImageHeight), hIcon, 
				DST_ICON | DSS_NORMAL,(CBrush*)NULL);
		}
		// Fix for GDI Resource leak. Thanks to Dieter Hammer
		if(hIcon) 
		{
		  ::DestroyIcon(hIcon);
		}
	}

	// Draw out text
	CFont* pOldFont = pDC->SelectObject(&m_font);
	CRect rectText(rectFocus);

	// Added by Abin, 10/30, 2003
	// Brings out the effect that the button caption seems to be pushed down too
	if (m_bBtnPushed)
		rectText.OffsetRect(1, 1);

	CString strCaption;
	GetWindowText(strCaption);
	pDC->SetBkMode(TRANSPARENT);

	if(ODS_DISABLED & lpDrawItemStruct->itemState) 
	{
		pDC->SetBkColor(GetSysColor(COLOR_BTNFACE));
		rectFocus.OffsetRect(1,1);
		pDC->SetTextColor(GetSysColor(COLOR_WINDOW));
		pDC->DrawText(strCaption, rectFocus, DT_SINGLELINE | m_nTextAlign);

		rectFocus.OffsetRect(-1,-1);
		pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
		pDC->DrawText(strCaption, rectFocus, DT_SINGLELINE | m_nTextAlign);
	}
	else 
	{
		if(SSL_STATE_UP == m_bsState) 
		{
			pDC->SetBkColor(m_crColors[SSL_UP_BK_COLOR]);
			pDC->SetTextColor(m_crColors[SSL_UP_TEXT_COLOR]);
		}
		if(SSL_STATE_OVER == m_bsState ) 
		{
			pDC->SetBkColor(m_crColors[SSL_OVER_BK_COLOR]);
			pDC->SetTextColor(m_crColors[SSL_OVER_TEXT_COLOR]);
		}
		if(SSL_STATE_DOWN == m_bsState) 
		{
			pDC->SetBkColor(m_crColors[SSL_DOWN_BK_COLOR]);
			pDC->SetTextColor(m_crColors[SSL_DOWN_TEXT_COLOR]);
		}

		// Changed by Abin, 10/30, 2003
		// Use rectText instead
		//pDC->DrawText(strCaption, rectFocus, DT_SINGLELINE | m_nTextAlign); // Original
		pDC->DrawText(strCaption, rectText, DT_SINGLELINE | m_nTextAlign); // Now
	}

	// Restore the old font in the device context
	pDC->SelectObject(pOldFont);

	if(SSL_BS_MENU_BTN & m_nStyle) 
	{
		CPen* pOldPen = NULL;
		// Drop down split
		CRect rectSplit(btnRect);		
		
		// By ABin, 10/30/2003
		//rectSplit.DeflateRect(2,2);
		if (SSL_BS_FLAT & m_nStyle)
		{
			rectSplit.DeflateRect(2,0); 
		}
		else
		{
			rectSplit.DeflateRect(1, 1, 1, 3);
		}

		rectSplit.right -= DROP_BUTTON_WIDTH;

		if(!(SSL_BS_FLAT & m_nStyle) || m_bMouseOnBtn) 
		{
			// Modified by Abin
			// To make the split line look more alike what IE toolbar has...
			CPen brFace(PS_SOLID, 1, ::GetSysColor((SSL_BS_FLAT & m_nStyle) && m_bBtnPushed ? COLOR_BTNHIGHLIGHT : COLOR_BTNSHADOW));
			CPen brLite(PS_SOLID, 1, ::GetSysColor((SSL_BS_FLAT & m_nStyle) && m_bBtnPushed ? COLOR_BTNSHADOW : COLOR_BTNHIGHLIGHT));

			// Added by Abin, 10/31/2003
			if (!(SSL_BS_FLAT & m_nStyle) && m_bBtnPushed)
				rectSplit.OffsetRect(1, 1);

			//CPen brFace(PS_SOLID, 1, ::GetSysColor(COLOR_BTNSHADOW));
			// Save the old pen
			pOldPen = pDC->SelectObject(&brFace);

			pDC->MoveTo(rectSplit.right, rectSplit.top);
			pDC->LineTo(rectSplit.right, rectSplit.bottom);

			//CPen brLite(PS_SOLID, 1, ::GetSysColor(COLOR_BTNHIGHLIGHT));
			pDC->SelectObject(&brLite);

			pDC->MoveTo(rectSplit.right + 1 , rectSplit.top);
			pDC->LineTo(rectSplit.right + 1, rectSplit.bottom);

			// Restore the old pen to the device context
			pDC->SelectObject(pOldPen);
		}
		rectSplit.left = rectSplit.right;
		rectSplit.right += DROP_BUTTON_WIDTH;

		CPoint pt(rectSplit.CenterPoint());
		pt += CPoint(m_bBtnPushed, m_bBtnPushed);

		CPen penBlack(PS_SOLID, 1, m_bDisabled ? 
			GetSysColor(COLOR_GRAYTEXT) : GetSysColor(COLOR_WINDOWTEXT));
		pOldPen = pDC->SelectObject(&penBlack);
		//DrawArrow(pDC, pt);

		// Drop down state
		if(TRUE == m_bMenuPushed && FALSE == m_bDisabled) 
		{    
			// Added by Abin, 10/31/2003
			if (SSL_BS_FLAT & m_nStyle)
				rectSplit.InflateRect(-1, 0, 3, 1);
			else
				rectSplit.InflateRect(0, 0, 0, 2);

			pDC->DrawEdge(rectSplit, BDR_SUNKENOUTER, BF_RECT);
			pt += CPoint(1, 1);
		}
		
		// Modified by Abin, 10/31/2003
		// Arrow should be drawn here
		DrawArrow(pDC, pt);

		// Restore the old pen to the device context
		pDC->SelectObject(pOldPen);
	}
}

BOOL CButtonSSL::PreTranslateMessage(MSG* pMsg) 
{
	// Initialise the tooltip if necessary
	InitToolTipCtrl();
	m_ToolTip.RelayEvent(pMsg);

	return CButton::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// CButtonSSL implementation

/////////////////////////////////////////////////////////////////////////////
// Initialisation

void CButtonSSL::SetSSLButtonStyle(UINT nStyle) 
{
	m_nStyle = nStyle;

	// Resize the button if necessary
	if(SSL_BS_AUTOSIZE ==(SSL_BS_AUTOSIZE & m_nStyle)) 
	{
		SetSize();
	}

	if(TRUE == IsDefault()) 
	{
		PostMessage(BM_SETSTYLE, BS_PUSHBUTTON, TRUE);
	}
	else 
	{
		PostMessage(BM_SETSTYLE, BS_DEFPUSHBUTTON, TRUE);
	}
}

UINT CButtonSSL::GetSSLButtonStyle() 
{
	return m_nStyle;
}

/////////////////////////////////////////////////////////////////////////////
// Image Functions

BOOL CButtonSSL::SetSSLButtonBitmap(UINT nResourceID, COLORREF crMask) 
{
	BOOL bReturn = m_bmpImage.LoadBitmap(nResourceID);
	if(TRUE == bReturn) 
	{
		m_bmpImage.GetObject(sizeof m_bmp, &m_bmp);
		m_nImageWidth = m_bmp.bmWidth;
		m_nImageHeight = m_bmp.bmHeight;
		if(NULL != m_imageList.m_hImageList) 
		{
			m_imageList.Detach();
		}
		bReturn = m_imageList.Create(m_bmp.bmWidth, m_bmp.bmHeight, ILC_MASK | ILC_COLOR32, 1, 0);
		if(TRUE == bReturn) 
		{
			if(m_imageList.Add(&m_bmpImage, crMask) > -1) 
			{
				m_bImageLoaded = TRUE;
			}
			else 
			{
				m_bImageLoaded = FALSE;
			}
		}
		else 
		{
			m_bImageLoaded = FALSE;
		}
	}

	// Resize the button if necessary
	if(SSL_BS_AUTOSIZE ==(SSL_BS_AUTOSIZE & m_nStyle)) 
	{
		SetSize();
	}

	return bReturn;
}

BOOL CButtonSSL::SetSSLButtonBitmaps(UINT nUpID, COLORREF crUpMask, 
					   UINT nOverID/* = 0*/, COLORREF crOverMask/* = SSL_MASK*/,
					   UINT nDownID/* = 0*/, COLORREF crDownMask/* = SSL_MASK*/, 
					   UINT nDisabledID/* = 0*/, COLORREF crDisabledMask/* = SSL_MASK*/) 
{
	ASSERT(0 != nUpID);
	BOOL bReturn = m_bmpImage.LoadBitmap(nUpID);
	if(TRUE == bReturn) 
	{
		m_bmpImage.GetObject(sizeof m_bmp, &m_bmp);
		m_nImageWidth = m_bmp.bmWidth;
		m_nImageHeight = m_bmp.bmHeight;
		if(NULL != m_imageList.m_hImageList) 
		{
			m_imageList.Detach();
		}
		bReturn = m_imageList.Create(m_nImageWidth, m_nImageHeight, ILC_MASK | ILC_COLOR32, 4, 0);
		if(TRUE == bReturn) 
		{
			if(m_imageList.Add(&m_bmpImage, crUpMask) > -1) 
			{
				m_bImageLoaded = TRUE;
				BOOL bOver = FALSE;
				// Add the Over image
				if(nOverID > 0) 
				{
					AddBitmap(nOverID, crOverMask);
					bOver = TRUE;
				}
				else 
				{
					// Use Up image if Over not supplied
					AddBitmap(nUpID, crUpMask);
				}
				// Add the Down image
				if(nDownID > 0) 
				{
					AddBitmap(nDownID, crDownMask);
				}
				else 
				{
					// If there's no Down image supplied
					if(TRUE == bOver) 
					{
						// Use the Over image is supplied
						AddBitmap(nOverID, crOverMask);
					}
					else 
					{
						// Otherwise use the Up image
						AddBitmap(nUpID, crUpMask);
					}
				}
				// Add the Disabled image
				// If one isn't one DrawState will use the Up image and draw it disabled.
				if(nDisabledID > 0) 
				{
					AddBitmap(nDisabledID, crDisabledMask);
				}
			}
			else 
			{
				m_bImageLoaded = FALSE;
			}
		}
	}


	// Resize the button if necessary
	if(SSL_BS_AUTOSIZE ==(SSL_BS_AUTOSIZE & m_nStyle)) 
	{
		SetSize();
	}

	return bReturn;
}

BOOL CButtonSSL::SetSSLButtonIcon(UINT nResourceID) 
{
	BOOL bReturn = FALSE;
	HINSTANCE hInstance = AfxFindResourceHandle(MAKEINTRESOURCE(nResourceID), RT_GROUP_ICON);

	m_hIcon =(HICON)::LoadImage(hInstance, MAKEINTRESOURCE(nResourceID), IMAGE_ICON, 0, 0, 0);
	if(NULL != m_hIcon) 
	{
		ICONINFO iconInfo;
		ZeroMemory(&iconInfo, sizeof(ICONINFO));
		bReturn = ::GetIconInfo(m_hIcon, &iconInfo);
		if(TRUE == bReturn) 
		{
			m_nImageWidth = iconInfo.xHotspot * 2;
			m_nImageHeight = iconInfo.yHotspot * 2;
			::DeleteObject(iconInfo.hbmMask);
			::DeleteObject(iconInfo.hbmColor);
			if(NULL != m_imageList.m_hImageList) 
			{
				m_imageList.Detach();
			}
			bReturn = m_imageList.Create(m_nImageWidth, m_nImageHeight, ILC_COLOR32 | ILC_MASK, 1, 1);
			if(TRUE == bReturn) 
			{
				if(m_imageList.Add(m_hIcon) > -1) 
				{
					m_bImageLoaded = TRUE;
				}
				else 
				{
					m_bImageLoaded = FALSE;
				}
			}
		}
	}


	// Resize the button if necessary
	if(SSL_BS_AUTOSIZE ==(SSL_BS_AUTOSIZE & m_nStyle)) 
	{
		SetSize();
	}

	return bReturn;
}

BOOL CButtonSSL::SetSSLButtonIcons(UINT nUpID, UINT nOverID/* = 0*/,
					   UINT nDownID/* = 0*/, UINT nDisabledID/* = 0*/) 
{
	ASSERT(0 != nUpID);
	BOOL bReturn = FALSE;
	HINSTANCE hInstance = AfxFindResourceHandle(MAKEINTRESOURCE(nUpID), RT_GROUP_ICON);

	m_hIcon =(HICON)::LoadImage(hInstance, MAKEINTRESOURCE(nUpID), IMAGE_ICON, 0, 0, 0);
	if(NULL != m_hIcon) 
	{
		ICONINFO iconInfo;
		ZeroMemory(&iconInfo, sizeof(ICONINFO));
		bReturn = ::GetIconInfo(m_hIcon, &iconInfo);
		if(TRUE == bReturn) 
		{
			m_nImageWidth = iconInfo.xHotspot * 2;
			m_nImageHeight = iconInfo.yHotspot * 2;
			::DeleteObject(iconInfo.hbmMask);
			::DeleteObject(iconInfo.hbmColor);
			if(NULL != m_imageList.m_hImageList) 
			{
				m_imageList.Detach();
			}
			bReturn = m_imageList.Create(m_nImageWidth, m_nImageHeight, ILC_MASK | ILC_COLOR32, 4, 0);
			if(TRUE == bReturn) 
			{
				if(m_imageList.Add(m_hIcon) > -1) 
				{
					m_bImageLoaded = TRUE;
					BOOL bOver = FALSE;
					// Add the Over image
					if(nOverID > 0) 
					{
						AddIcon(nOverID);
						bOver = TRUE;
					}
					else 
					{
						// Use Up image if Over not supplied
						AddIcon(nUpID);
					}
					// Add the Down image
					if(nDownID > 0) 
					{
						AddIcon(nDownID);
					}
					else 
					{
						// If there's no Down image supplied
						if(TRUE == bOver) 
						{
							// Use the Over image is supplied
							AddIcon(nOverID);
						}
						else 
						{
							// Otherwise use the Up image
							AddIcon(nUpID);
						}
					}
					// Add the Disabled image
					// If one isn't one DrawState will use the Up image and draw it disabled.
					if(nDisabledID > 0) 
					{
						AddIcon(nDisabledID);
					}
				}
				else 
				{
					m_bImageLoaded = FALSE;
				}
			}
		}
	}

	// Resize the button if necessary
	if(SSL_BS_AUTOSIZE ==(SSL_BS_AUTOSIZE & m_nStyle)) 
	{
		SetSize();
	}

	return bReturn;
}

void CButtonSSL::SetSSLImageAlign(UINT nImageAlign) 
{
	m_nImageAlign = nImageAlign;
	Invalidate();
}

UINT CButtonSSL::GetSSLImageAlign() 
{
	return m_nImageAlign;
}

/////////////////////////////////////////////////////////////////////////////
// Text Functions

void CButtonSSL::SetSSLTextAlign(UINT nTextAlign) 
{
	m_nTextAlign = nTextAlign;
	Invalidate();
}

UINT CButtonSSL::GetSSLTextAlign() 
{
	return m_nTextAlign;
}

BOOL CButtonSSL::SetSSLButtonFont(LPCTSTR lpszFaceName, int nSizePoints/* = 8*/,
									BOOL bUnderline/* = FALSE*/, BOOL bBold/* = FALSE*/,
									BOOL bStrikeOut/* = FALSE*/, BOOL bItalic/* = FALSE*/) 
{
	if(NULL !=(HFONT)m_font) 
	{
		m_font.DeleteObject();
	}

	CDC* pDC = GetDC();
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));

	StringCchCopy(lf.lfFaceName, _countof(lf.lfFaceName) -1,  lpszFaceName);
	lf.lfHeight = -MulDiv(nSizePoints, GetDeviceCaps(pDC->m_hDC, LOGPIXELSY), 72);
	lf.lfUnderline = bUnderline;
	if(TRUE == bBold) 
	{
		lf.lfWeight = FW_BOLD;
	}
	lf.lfStrikeOut = bStrikeOut;
	lf.lfItalic = bItalic;

	BOOL bReturn = m_font.CreateFontIndirect(&lf);

	Invalidate();
	ReleaseDC(pDC);

	return bReturn;
}

BOOL CButtonSSL::SetSSLButtonFont(CFont& font) 
{
	if(NULL !=(HFONT)m_font) 
	{
		m_font.DeleteObject();
	}

	LOGFONT lf;
	font.GetLogFont(&lf);

	BOOL bReturn = m_font.CreateFontIndirect(&lf);

	Invalidate();
	
	return bReturn;
}

/////////////////////////////////////////////////////////////////////////////
// Colour Functions

void CButtonSSL::SetSSLDefaultColors(BOOL bRedraw/* = TRUE*/) 
{
	m_crColors[SSL_UP_BK_COLOR]	= ::GetSysColor(COLOR_BTNFACE);
	m_crColors[SSL_UP_TEXT_COLOR]	= ::GetSysColor(COLOR_BTNTEXT);
	m_crColors[SSL_OVER_BK_COLOR]	= ::GetSysColor(COLOR_BTNFACE);
	m_crColors[SSL_OVER_TEXT_COLOR]	= ::GetSysColor(COLOR_BTNTEXT);
	m_crColors[SSL_DOWN_BK_COLOR]	= ::GetSysColor(COLOR_BTNFACE);
	m_crColors[SSL_DOWN_TEXT_COLOR]	= ::GetSysColor(COLOR_BTNTEXT);

	if(TRUE == bRedraw) 
	{
		Invalidate();
	}
}

BOOL CButtonSSL::SetSSLColor(BYTE byColorIndex, COLORREF crColor, BOOL bRedraw/* = TRUE*/) 
{
	if(byColorIndex > SSL_MAX_COLORS) 
	{
		return FALSE;
	}

	m_crColors[byColorIndex] = crColor;

	if(TRUE == bRedraw) 
	{
		Invalidate();
	}
	
	return TRUE;
}

BOOL CButtonSSL::GetSSLColor(BYTE byColorIndex, COLORREF* pcrColor) 
{
	if(byColorIndex > SSL_MAX_COLORS) 
	{
		return FALSE;
	}

	*pcrColor = m_crColors[byColorIndex];

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Check Box Functions

void CButtonSSL::SetSSLCheck(int nCheck, BOOL bRedraw/* = TRUE*/) 
{
	UINT nType = GetControlType();
	if(BS_AUTORADIOBUTTON ==(BS_AUTORADIOBUTTON & nType) ||
		BS_RADIOBUTTON ==(BS_RADIOBUTTON & nType) ||
		BS_AUTOCHECKBOX ==(BS_AUTOCHECKBOX & nType) ||
		BS_CHECKBOX ==(BS_CHECKBOX & nType)) 
	{
		if(0 == nCheck) 
		{
			m_nCheck = 0;
			m_bBtnPushed = false;
		}
		else 
		{
			m_nCheck = 1;
			m_bBtnPushed = true;
		}

		if(TRUE == bRedraw) 
		{
			Invalidate();
		}
	}
}

int CButtonSSL::GetSSLCheck() 
{
	return m_nCheck;
}

/////////////////////////////////////////////////////////////////////////////
// Menu Functions

BOOL CButtonSSL::SetSSLButtonMenu(UINT nMenuResourceID, UINT nSubMenuPos) 
{
	// Ensure the SSL_BS_MENU_BTN style is set
	m_nStyle |= SSL_BS_MENU_BTN;
	if(NULL != m_menu.m_hMenu) 
	{
		m_menu.DestroyMenu();
	}
	BOOL bReturn = m_menu.LoadMenu(nMenuResourceID);
	m_nSubMenuPos = nSubMenuPos;

	m_bMenuLoaded |= bReturn;

	// Resize the button if necessary
	if(SSL_BS_AUTOSIZE ==(SSL_BS_AUTOSIZE & m_nStyle)) 
	{
		SetSize();
	}

	return bReturn;
}

BOOL CButtonSSL::SetSSLButtonMenu(LPCTSTR lpszResourceName, UINT nSubMenuPos) 
{
	// Ensure the SSL_BS_MENU_BTN style is set
	m_nStyle |= SSL_BS_MENU_BTN;
	if(NULL != m_menu.m_hMenu) 
	{
		m_menu.DestroyMenu();
	}
	BOOL bReturn = m_menu.LoadMenu(lpszResourceName);
	m_nSubMenuPos = nSubMenuPos;
	m_bMenuLoaded |= bReturn;


	// Resize the button if necessary
	if(SSL_BS_AUTOSIZE ==(SSL_BS_AUTOSIZE & m_nStyle)) 
	{
		SetSize();
	}

	return bReturn;
}

UINT CButtonSSL::CheckSSLMenuItem(UINT nIDCheckItem, UINT nCheck/* = MF_CHECKED*/) 
{
	if(FALSE == m_bMenuLoaded) 
	{
		return MF_DOES_NOT_EXIST;
	}

    return m_menu.CheckMenuItem(nIDCheckItem, nCheck);
}

BOOL CButtonSSL::CheckSSLMenuRadioItem(UINT nIDFirst, UINT nIDLast, UINT nIDItem, UINT nFlags) 
{
    if(FALSE == m_bMenuLoaded) 
	{
        return FALSE;
    }

    return m_menu.CheckMenuRadioItem(nIDFirst, nIDLast, nIDItem, nFlags);
}

UINT CButtonSSL::EnableSSLMenuItem(UINT nIDEnableItem, UINT nEnable) 
{
    if(FALSE == m_bMenuLoaded) 
	{
        return MF_DOES_NOT_EXIST;
    }

    return m_menu.EnableMenuItem(nIDEnableItem, nEnable);
}

UINT CButtonSSL::GetSSLMenuItemCount() const 
{
    if(FALSE == m_bMenuLoaded) 
	{
        return MF_DOES_NOT_EXIST;
    }

    return m_menu.GetMenuItemCount();
}

UINT CButtonSSL::GetSSLMenuItemID(int nPos) const 
{
    if(FALSE == m_bMenuLoaded) 
	{
        return MF_DOES_NOT_EXIST;
    }

    return m_menu.GetMenuItemID(nPos);
}

UINT CButtonSSL::GetSSLMenuState(UINT nID, UINT nFlags) 
{
	if(FALSE == m_bMenuLoaded) 
	{
		return MF_DOES_NOT_EXIST;
	}
	
	return m_menu.GetMenuState(nID, nFlags);
}

int CButtonSSL::GetSSLMenuString(UINT nIDItem, LPTSTR lpString, int nMaxCount, UINT nFlags) 
{
    if(FALSE == m_bMenuLoaded) 
	{
        return 0;
    }

    return m_menu.GetMenuString(nIDItem, lpString, nMaxCount, nFlags);
}

int CButtonSSL::GetSSLMenuString(UINT nIDItem, CString& rString, UINT nFlags) 
{
    if(FALSE == m_bMenuLoaded) 
	{
        return 0;
    }

    return m_menu.GetMenuString(nIDItem, rString, nFlags);
}

BOOL CButtonSSL::GetSSLMenuItemInfo(UINT nIDItem, LPMENUITEMINFO lpMenuItemInfo,
                                     BOOL ByPos/*=FALSE*/) 
{
    if(FALSE == m_bMenuLoaded) 
	{
        return FALSE;
    }

    return m_menu.GetMenuItemInfo(nIDItem, lpMenuItemInfo, ByPos);
}

BOOL CButtonSSL::ModifySSLMenu(UINT nPosition, UINT nFlags, UINT nIDNewItem/*=0*/,
                                LPCTSTR lpszNewItem/*=NULL*/) 
{
    if(FALSE == m_bMenuLoaded) 
	{
        return FALSE;
    }

    return m_menu.ModifyMenu(nPosition, nFlags, nIDNewItem, lpszNewItem);
}

BOOL CButtonSSL::ModifySSLMenu(UINT nPosition, UINT nFlags, UINT nIDNewItem,
                                const CBitmap* pBmp) 
{
    if(FALSE == m_bMenuLoaded) 
	{
        return FALSE;
    }

    return m_menu.ModifyMenu(nPosition, nFlags, nIDNewItem, pBmp);
}

BOOL CButtonSSL::RemoveSSLMenu(UINT nPosition, UINT nFlags) 
{
    if(FALSE == m_bMenuLoaded) 
	{
        return FALSE;
    }

    return m_menu.RemoveMenu(nPosition, nFlags);
}

BOOL CButtonSSL::SetSSLMenuItemBitmaps(UINT nPosition, UINT nFlags,
                                        const CBitmap* pBmpUnchecked,
                                        const CBitmap* pBmpChecked)
{
    if(FALSE == m_bMenuLoaded) 
	{
        return FALSE;
    }

    return m_menu.SetMenuItemBitmaps(nPosition, nFlags, pBmpUnchecked,
                                      pBmpChecked);
}

/////////////////////////////////////////////////////////////////////////////
// Miscellaneous Functions

BOOL CButtonSSL::SetSSLButtonCursor(UINT nResourceID) 
{
	HINSTANCE hInstResource;
	// Destroy any previous cursor
	if(NULL != m_hCursor) 
	{
		::DestroyCursor(m_hCursor);
		m_hCursor = NULL;
	}

	// If we want a cursor
	if(nResourceID != NULL) 
	{
		hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nResourceID), RT_GROUP_CURSOR);
		// Load icon resource
		m_hCursor =(HCURSOR)::LoadImage(hInstResource, MAKEINTRESOURCE(nResourceID),
			IMAGE_CURSOR, 0, 0, 0);
		// If something wrong then return FALSE
		if(NULL == m_hCursor) 
		{
			return FALSE;
		}
	}

	return TRUE;
}

void CButtonSSL::SetSSLButtonToolTip(LPCTSTR lpszTipText, BOOL bActivate) 
{
	// Can't have a NULL text string
	if(NULL == lpszTipText) 
	{
		return;
	}

	// Initialise the tooltip
	InitToolTipCtrl();

	// Need to add a tooltip if none defined
	if(0 == m_ToolTip.GetToolCount()) 
	{
		CRect rectBtn;
		GetClientRect(rectBtn);
		m_ToolTip.AddTool(this, lpszTipText, rectBtn, 1);
	}

	// Set the text
	m_ToolTip.UpdateTipText(lpszTipText, this, 1);
	m_ToolTip.Activate(bActivate);
}

void CButtonSSL::SetSSLButtonToolTip(UINT nResourceID, BOOL bActivate) 
{
	CString strTipText;
	strTipText.LoadString(nResourceID);
	if(false == strTipText.IsEmpty()) 
	{
		SetSSLButtonToolTip((LPCTSTR)strTipText, bActivate);
	}
}

void CButtonSSL::GetSSLButtonToolTip(CString& strTipText) 
{
	m_ToolTip.GetText(strTipText, this, 1);
}

// The following appeared in Paul DiLascia's Jan 1998 MSJ articles.
// It loads a "hand" cursor from the winhlp32.exe module
void CButtonSSL::SetSSLButtonURL(LPCTSTR lpszURL) 
{
	if(NULL != lpszURL) 
	{
		// Store the URL
		::lstrcpyn(m_szURL, lpszURL, _MAX_PATH);

		if(NULL == m_hCursor) 
		{
			// Try to load the 'standard' hand cursor
			CString szWndDir;
			GetWindowsDirectory(szWndDir.GetBuffer(MAX_PATH), MAX_PATH);
			szWndDir.ReleaseBuffer();

			// Try cursor #106 in WinHlp32.exe
			CString szHlp32 = szWndDir + _T("\\winhlp32.exe");
			HMODULE hModule = LoadLibrary(szHlp32);
			if(NULL != hModule) 
			{
				HCURSOR hHandCursor;
				hHandCursor = ::LoadCursor(hModule, MAKEINTRESOURCE(106));
				if(NULL != hHandCursor) 
				{
					m_hCursor = CopyCursor(hHandCursor);
				}
			}
		}
	}
	else 
	{
		// Remove any existing URL
		::ZeroMemory(&m_szURL, sizeof(m_szURL));
	}
}

void CButtonSSL::SetSSLButtonURL(UINT nResourceID)
{
	CString strURL;
	strURL.LoadString(nResourceID);
	if(false == strURL.IsEmpty())
	{
		SetSSLButtonURL((LPCTSTR)strURL);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CButtonSSL helper functions

void CButtonSSL::DrawArrow(CDC* pDC, CPoint ptTip) 
{
	CPoint ptDest;

	pDC->SetPixel(ptTip, RGB(0, 0, 0));

	ptTip -= CPoint(1, 1);
	pDC->MoveTo(ptTip);

	ptDest = ptTip;
	ptDest += CPoint(3, 0);
	pDC->LineTo(ptDest);

	ptTip -= CPoint(1, 1);
	pDC->MoveTo(ptTip);

	ptDest = ptTip;
	ptDest += CPoint(5, 0);
	pDC->LineTo(ptDest);

	ptTip -= CPoint(1, 1);
	pDC->MoveTo(ptTip);

	ptDest = ptTip;
	ptDest += CPoint(7, 0);
	pDC->LineTo(ptDest);
}

BOOL CButtonSSL::HitMenuButton(CPoint pt)
{
	if(FALSE == m_bMenuLoaded)
	{
		return FALSE; // Don't allow menu button drop down if no menu items are loaded
	}

	ClientToScreen(&pt);

	CRect rect;
	GetWindowRect(rect);
	rect.left = rect.right - DROP_BUTTON_WIDTH;

	return rect.PtInRect(pt);
}

void CButtonSSL::InitToolTipCtrl() 
{
	if(NULL == m_ToolTip.m_hWnd) 
	{
		m_ToolTip.Create(this);
		m_ToolTip.Activate(FALSE); // Inactive initially
		m_ToolTip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 300); // Enable multiline tooltip
	}
}

void CButtonSSL::AddBitmap(UINT nResourceID, COLORREF crMask) 
{
	m_bmpImage.Detach();
	m_bmpImage.LoadBitmap(nResourceID);
	m_imageList.Add(&m_bmpImage, crMask);
}

void CButtonSSL::AddIcon(UINT nResourceID) 
{
	HINSTANCE hInstance = AfxFindResourceHandle(MAKEINTRESOURCE(nResourceID), RT_GROUP_ICON);

	m_hIcon =(HICON)::LoadImage(hInstance, MAKEINTRESOURCE(nResourceID), IMAGE_ICON, 0, 0, 0);
	if(NULL != m_hIcon) 
	{
		ICONINFO iconInfo;
		ZeroMemory(&iconInfo, sizeof(ICONINFO));
		::GetIconInfo(m_hIcon, &iconInfo);
		::DeleteObject(iconInfo.hbmMask);
		::DeleteObject(iconInfo.hbmColor);
		m_imageList.Add(m_hIcon);
	}
}

CPoint CButtonSSL::GetImagePoint(CRect btnRect) 
{
	int nLeft = 3, nTop = 3; // Default position for SSL_IMAGE_LEFT / _TOP
	// Determine the left coordinate
	if(SSL_IMAGE_CENTER ==(SSL_IMAGE_CENTER & m_nImageAlign)) 
	{
		if(SSL_BS_MENU_BTN ==(SSL_BS_MENU_BTN & m_nStyle)) 
		{
			nLeft =(btnRect.Width() - m_nImageWidth - DROP_BUTTON_WIDTH) / 2;
		}
		else 
		{
			nLeft =(btnRect.Width() - m_nImageWidth) / 2;
		}
	}
	else if(SSL_IMAGE_RIGHT ==(SSL_IMAGE_RIGHT & m_nImageAlign)) 
	{
		nLeft = btnRect.right - m_nImageWidth - 3;
		if(SSL_BS_MENU_BTN ==(SSL_BS_MENU_BTN & m_nStyle)) 
		{
			nLeft -= DROP_BUTTON_WIDTH;
		}
	}
	// Determine the top coordinate
	if(SSL_IMAGE_VCENTER ==(SSL_IMAGE_VCENTER & m_nImageAlign)) 
	{
		nTop =(btnRect.Height() - m_nImageHeight) / 2;
	}
	else if(SSL_IMAGE_BOTTOM ==(SSL_IMAGE_BOTTOM & m_nImageAlign)) 
	{
		nTop = btnRect.bottom - m_nImageHeight - 3;
	}

	return CPoint(nLeft + m_bBtnPushed, nTop + m_bBtnPushed);
}

void CButtonSSL::SetSize() 
{
	// Get the size of the button text
	CDC* pDC = GetDC();
	SIZE size;
	CString szText;
	GetWindowText(szText);
	// Need to select the font to ensure correct sizing for GetTextExtentPoint32
	CFont* pOldFont = pDC->SelectObject(&m_font); 
	GetTextExtentPoint32(pDC->m_hDC, szText, szText.GetLength(), &size);
	// Restore the old font into the device context
	pDC->SelectObject(pOldFont);

	// Get the current size of the button
	CRect btnRect;
	GetWindowRect(btnRect);

	// Adjust the size according to the contents
	btnRect.right = btnRect.left + max(m_nImageWidth, size.cx) + 6;
	btnRect.bottom = btnRect.top + max(m_nImageHeight, size.cy) + 6;

	// Take in to account the drop button if there is one
	if(SSL_BS_MENU_BTN ==(SSL_BS_MENU_BTN & m_nStyle)) 
	{
		btnRect.right += DROP_BUTTON_WIDTH + 3;
	}

	// Resize the button
	SetWindowPos(NULL, 0, 0, btnRect.Width(), btnRect.Height(), SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW);
}

UINT CButtonSSL::GetControlType()
{
	return m_nTypeStyle;
}

BOOL CButtonSSL::IsDefault()
{
	// By Abin, 11/07/2003
	// Something is wrong, buttons are always drawn as "default" at the beginning.
	// I couldn't find out the reason so... Plus, default push buttons look so
	// ugly anyway.
	return FALSE;

	// Sanity check
	ASSERT((m_bCanBeDefault && m_bDefault) == m_bDefault);

	return m_bDefault;
}

void CButtonSSL::EnableDefault(BOOL bEnable)
{
	m_bCanBeDefault = bEnable;

	// We need to remove the default state if we've got it
	// and we're disabling default state handling
	if(!bEnable && m_bDefault)
	{
		// Remove the default state
		SendMessage(BM_SETSTYLE, 
			(GetStyle() & ~BS_TYPEMASK) | BS_PUSHBUTTON, TRUE);
		ASSERT(m_bDefault == FALSE);

		// Update the button
		LRESULT lRes = GetParent()->SendMessage(DM_GETDEFID);
		if(HIWORD(lRes) == DC_HASDEFID)
		{
			GetParent()->SendMessage(DM_SETDEFID, LOWORD(lRes));
		}
	}
}

void CButtonSSL::TraceState(ButtonState state) 
{
	CString strState = _T("Button State: ");
	switch(state) {
	case SSL_STATE_UP:
		strState += _T("Up");
		break;
	case SSL_STATE_OVER:
		strState += _T("Over");
		break;
	case SSL_STATE_DOWN:
		strState += _T("Down");
		break;
	case SSL_STATE_DISABLED:
		strState += _T("Disabled");
		break;
	default:
		strState += _T("Invalid");
	}
	TRACE("%s. m_bDisabled: %i. m_bMouseOnBtn: %i. m_bBtnPushed: %i. m_bMenuPushed: %i.\n",
		strState, m_bDisabled, m_bMouseOnBtn, m_bBtnPushed, m_bMenuPushed);
}

void CButtonSSL::OnDestroy() 
{
	CButton::OnDestroy();
	
	// TODO: Add your message handler code here
	KillTimer(TIMER_MOUSE_OVER);
}

// Added by Abin, 11/07/2003
// Call this function before TrackPopupMenu, to enable menu item update 
void CButtonSSL::CmdRouteMenu(CMenu* pPopupMenu)
{
	CCmdUI state;
	state.m_pMenu = pPopupMenu;
	state.m_pParentMenu = pPopupMenu;
	state.m_nIndexMax = pPopupMenu->GetMenuItemCount();

	for (state.m_nIndex = 0; 
	     state.m_nIndex < state.m_nIndexMax; 
	     state.m_nIndex++) 
	{
		state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);

		// menu separator or invalid cmd - ignore it
		if (state.m_nID == 0) continue; 

		if (state.m_nID == (UINT)-1)
		{
			// possibly a popup menu, route to child menu if so
			CMenu* pSub=pPopupMenu->GetSubMenu(state.m_nIndex);
			if(pSub) CmdRouteMenu(pSub);
		}
		else 
		{
			// normal menu item, Auto disable if command is 
			// _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(GetParent(), FALSE);
		}
	}
}

