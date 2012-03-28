#pragma once

#define DEF_WIDTH		17
#include "../SkinButtom/ImageButton.h"

int const UM_V_THUMBTRACK_MSG  = WM_USER + 1001;
int const UM_H_THUMBTRACK_MSG  = WM_USER + 1002;
int const UM_UPDATE_SCROLL_CORNER = WM_USER + 1003;

enum MOUSE_STATE
{
	MOUSE_NOMAL = 0,
	MOUSE_OVER,
	MOUSE_DOWN,
	MOUSE_DISABLE
};

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
class CSkinScrollBar : public CWindowImpl<CSkinScrollBar, CScrollBar>
{
public:
	DECLARE_WND_CLASS_EX( _T( "SkinScrollBar" ), CS_DBLCLKS, COLOR_WINDOW );

	CSkinScrollBar ( void );
	virtual ~CSkinScrollBar ( VOID );

	BEGIN_MSG_MAP ( CSkinScrollBar )
		MESSAGE_HANDLER ( WM_CREATE, OnCreate )
		MESSAGE_HANDLER ( WM_SIZE, OnSize )
		MESSAGE_HANDLER ( WM_PAINT, OnPaint )
		MESSAGE_HANDLER ( WM_ERASEBKGND, OnEraseBkGnd )
		MESSAGE_HANDLER ( WM_LBUTTONDOWN, OnLButtonDown )
		MESSAGE_HANDLER ( WM_LBUTTONUP, OnLButtonUp )
		MESSAGE_HANDLER ( WM_MOUSEMOVE, OnMouseMove )
		MESSAGE_HANDLER ( WM_TIMER, OnTimer )
		MESSAGE_HANDLER ( WM_MOUSELEAVE, OnMouseLeave )
		MESSAGE_HANDLER ( WM_LBUTTONDBLCLK, OnLButtonDblClk )
		MESSAGE_HANDLER ( WM_CONTEXTMENU, OnLButtonDblClk )
		MESSAGE_HANDLER ( SBM_GETSCROLLINFO, OnGetScrollInfo )
		MESSAGE_HANDLER ( SBM_SETSCROLLINFO, OnSetScrollInfo )
	END_MSG_MAP()

	VOID setWidth ( int nWidth )
	{
		m_nWidth = nWidth;
	}

protected:
	VOID DrawButtonTop(HDC hDC, const CRect& rcTop, const CPoint& pt);
	VOID DrawButtonBottom( HDC hDC, const CRect& rcBottom, const CPoint& pt );
	VOID DrawThumb ( HDC hDC, const CRect& rcThumb );
	void DrawThumbTop( HDC hDC, const CRect& rcThumb );
	void DrawThumbMiddle( HDC hDC, const CRect& rcThumb );
	void DrawThumbBottom( HDC hDC, const CRect& rcThumb );
	void DrawThumbLine( HDC hDC, const CRect& rcThumb );
	UINT HitTest ( CONST CPoint& pt );
	CRect GetRect ( UINT uSBCode );

	// Windows Event Handler
	LRESULT OnSize ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT OnCreate ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );

	LRESULT OnPaint ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT OnEraseBkGnd ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );

	LRESULT OnLButtonDown ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT OnLButtonUp ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT OnMouseMove ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT OnTimer ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT OnMouseLeave ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );

	LRESULT OnLButtonDblClk ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT OnGetScrollInfo ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT OnSetScrollInfo ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );

	VOID DrawScrollBar ( VOID );

public:
	BOOL IsVertical ( VOID );
	VOID SetTargetWnd ( HWND hWnd );
	BOOL SetSkinInfo ( CImage* pBtnUp, CImage* pBtnDown, CImage* pBack, CImage* pThumbTop,
						CImage* pThumbBody, CImage* pThumbBottom, CImage* pThumbLine,
						HBRUSH hBrBackground /*, COLORREF crBack*/ ); // NS

private:
	void _DrawBtnState( HDC hDC, NDGraphics::CGDIPImageList& imgBtn, MOUSE_STATE mouseState, CRect rcRect );
	void _RepeatDrawBtnState( HDC hDC, NDGraphics::CGDIPImageList& imgBtn, MOUSE_STATE mouseState, CRect rcRect );

private:
	// Member Veriable
	INT32		m_nWidth;
	INT32		m_nHeight;
	SCROLLINFO	m_SInfo;

	BOOL		m_bDrag;
	CPoint		m_ptDrag;
	INT32		m_nDragPos;

	UINT		m_uClicked;
	BOOL		m_bNotify;
	UINT		m_uHitPrev;

	NDGraphics::CGDIPImage		m_imgBack;
	NDGraphics::CGDIPImageList	m_ilBtnUp;
	NDGraphics::CGDIPImageList	m_ilBtnDown;

	NDGraphics::CGDIPImageList		m_imgThumbTop;
	NDGraphics::CGDIPImageList		m_imgThumbBottom;
	NDGraphics::CGDIPImageList		m_imgThumbBody;
	NDGraphics::CGDIPImageList		m_imgThumbLine;
	
	CImageButton	m_btnUp;
	CImageButton	m_btnDown;
	CBrushHandle	m_brBackground;

	MOUSE_STATE	m_enumThumbMouseState;

	enum BUTTON_TYPE
	{
		BT_NONE = 0,
		BT_UP,
		BT_DOWN
	};
	BUTTON_TYPE		m_eClicked;
	BOOL			m_bVertical;
	HWND			m_hTargetWnd;
};
//////////////////////////////////////////////////////////////////////////