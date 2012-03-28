#include "StdAfx.h"
#include "SkinComboEdit.h"

const int FRAME_MARGIN_TOP = 2;
const int FRAME_MARGIN_BOTTOM = 1;
const int EDIT_MARGIN_LEFT  = 7;  // 8-1(line width)
const int EDIT_MARGIN_TOP = 4;    // 5-1(line width)

const int ARROW_MARGIN_TOP = 5;
const int ARROW_MARGIN_RIGHT = 4;
const int ARROW_MARGIN_BOTTOM = 5;
//const int ARROW_WIDTH = 10;

CSkinComboEdit::CSkinComboEdit(void)
{
	m_bOver = FALSE;
	m_bOverOnArrow = FALSE;
	m_nArrowWidth = 9;
	m_nArrowHeight = 9;
	m_hParentCombobox = NULL;
}

CSkinComboEdit::~CSkinComboEdit(void)
{
}

BOOL  CSkinComboEdit::Initialize(const COMBOEDIT_SKIN_INFO& rSkinInfo, HWND hParentCombobox )
{
	m_skinInfo = rSkinInfo;
	m_hParentCombobox = hParentCombobox;

	m_imgMenuDown.Create(rSkinInfo.m_pImgMenuDown, ImageFormatPNG, 2 );
	
	m_nArrowWidth = m_imgMenuDown[0]->GetWidth();
	m_nArrowHeight = m_imgMenuDown[0]->GetHeight();
	m_hParentCombobox = hParentCombobox;

	return TRUE;
}

void CSkinComboEdit::OnPaint(CDCHandle /*dc*/)
{
	DoPaint();
}

void CSkinComboEdit::DoPaint()
{
	CRect rcClient;
	GetClientRect(rcClient);
	rcClient.DeflateRect(0, FRAME_MARGIN_TOP, 0, FRAME_MARGIN_BOTTOM);

	CPaintDC dc(m_hWnd);

	HPEN hOldPen = NULL;
	HFONT hOldFont = NULL;
	HBRUSH hOldBrush = NULL;

	CRect rcFrame = rcClient;

	hOldPen = dc.SelectPen((HPEN)GetStockObject(NULL_PEN));
	dc.FillSolidRect(rcFrame, m_skinInfo.m_clrBackgroundNormal);;

	//draw menu down arrow
	CRect rcArrow = rcFrame;
	rcArrow.right -= ARROW_MARGIN_RIGHT;
	rcArrow.left = rcArrow.right - m_nArrowWidth;
	rcArrow.top += ARROW_MARGIN_TOP;
	rcArrow.bottom -= ARROW_MARGIN_BOTTOM;

	if (m_imgMenuDown.IsValid())
	{
		POINT ptMouse = {0};
		GetCursorPos(&ptMouse);
		ScreenToClient(&ptMouse);
		if (_IsMouseInMenuDownRect(ptMouse))
		{
			m_imgMenuDown[0]->Draw(dc, rcArrow.left, rcArrow.top, rcArrow.Width(), rcArrow.Height());
		}
		else
		{
			m_imgMenuDown[1]->Draw(dc, rcArrow.left, rcArrow.top, rcArrow.Width(), rcArrow.Height());
		}
	}

	//draw text
	CRect rcText = rcFrame;
	rcText.DeflateRect(EDIT_MARGIN_LEFT, EDIT_MARGIN_TOP,
		ARROW_MARGIN_RIGHT + m_nArrowWidth, 0 );

	CString strText;
	GetWindowText(strText);

	COLORREF clrOldFont = RGB(0, 0, 0);
	if ( ::IsWindowEnabled(m_hParentCombobox) )
	{
		clrOldFont = dc.SetTextColor(m_skinInfo.m_clrFontNormal);
		hOldFont = dc.SelectFont(m_skinInfo.m_hFontNormal);
	}
	else
	{
		clrOldFont = dc.SetTextColor(m_skinInfo.m_clrFontDisabled);
		hOldFont = dc.SelectFont(m_skinInfo.m_hFontDisabled);
	}
	dc.DrawText(strText, -1, rcText, DT_LEFT | DT_TOP);
	dc.SetTextColor(clrOldFont);

	CPen strokePen;
	//draw Frame
	if ( ::IsWindowEnabled(m_hParentCombobox))
	{
		if (GetFocus() == m_hWnd)
		{
			strokePen.CreatePen(PS_SOLID, 1, m_skinInfo.m_clrStrokeFocus);
		}
		else
		{
			strokePen.CreatePen(PS_SOLID, 1, m_skinInfo.m_clrStrokeNormal); 
		}
	}
	else
	{
		//disabled
		strokePen.CreatePen(PS_SOLID, 1, m_skinInfo.m_clrStrokeDisabled);
	}
	dc.SelectBrush((HBRUSH)GetStockObject(NULL_BRUSH));
	dc.SelectPen(strokePen);
	dc.Rectangle(rcFrame);

	//restore dc
	dc.SelectBrush(hOldBrush);
	dc.SelectPen(hOldPen);
	dc.SelectFont(hOldFont);
}


BOOL CSkinComboEdit::OnEraseBkgnd(CDCHandle dc)
{
	return TRUE;
}

void CSkinComboEdit::OnMouseMove(UINT nFlags, CPoint point)
{

	//CRect rcClient;
	//GetClientRect(rcClient);
	//CRect rcArrow = rcClient;
	//rcArrow.right -= ARROW_MARGIN_RIGHT;
	//rcArrow.left = rcArrow.right - ARROW_WIDTH;
	//rcArrow.top += ARROW_MARGIN_TOP;
	//rcArrow.bottom -= ARROW_MARGIN_BOTTOM;
	
	//FTLTRACE(TEXT("CSkinComboBox::OnMouseMove, rcArrow =  {%d,%d} - {%d,%d}, point= {%d,%d} \n"),
	//	rcArrow.left, rcArrow.top, rcArrow.right, rcArrow.bottom, point.x, point.y);

	BOOL bOldOverOnArrow = m_bOverOnArrow;
	m_bOverOnArrow = _IsMouseInMenuDownRect(point);
		//rcArrow.PtInRect(point);
	
	if ( bOldOverOnArrow != m_bOverOnArrow )
	{
		Invalidate(FALSE);
	}

	if ( m_bOver == FALSE )
	{
		m_bOver = TRUE;
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof( TRACKMOUSEEVENT );
		tme.dwFlags = TME_LEAVE;
		tme.dwHoverTime = 0;
		tme.hwndTrack = m_hWnd;

		_TrackMouseEvent( &tme );
	}
	SetMsgHandled(FALSE);
}

void CSkinComboEdit::OnMouseLeave()
{
	//FTLTRACE(TEXT("CSkinComboEdit::OnMouseLeave\n"));
	POINT ptMouse = {0};
	GetCursorPos(&ptMouse);
	ScreenToClient(&ptMouse);

	if (!_IsMouseInMenuDownRect(ptMouse))
	{
		m_bOverOnArrow = FALSE;
		Invalidate();
	}
	m_bOver = FALSE;
	//SetMsgHandled(FALSE);
}

BOOL CSkinComboEdit::_IsMouseInMenuDownRect(const POINT& pt)
{
	CRect rcWindowPos;
	GetWindowRect(&rcWindowPos);
	ScreenToClient(&rcWindowPos);

	CRect rcMenuDown = rcWindowPos;
	rcMenuDown.right -= ARROW_MARGIN_RIGHT;
	rcMenuDown.left = rcMenuDown.right - m_nArrowWidth;
	rcMenuDown.top += ARROW_MARGIN_TOP;
	rcMenuDown.bottom -= ARROW_MARGIN_BOTTOM;

	if (rcMenuDown.PtInRect(pt))
	{
		return TRUE;
	}

	return FALSE;
}