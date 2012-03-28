#pragma once

#include "SkinScrollBar.h"

#pragma warning(disable: 4311)
#pragma warning(disable: 4312)

#define UM_DESTMOVE		( WM_USER + 100 )

typedef struct tagSCROLLBAR_SKIN_INFO
{
	CImage*		pVImgUp;
	CImage*		pVImgDown;
	CImage*		pVImgBack;
	CImage*		pVImgThumbTop;
	CImage*		pVImgThumbBody;
	CImage*		pVImgThumbBottom;
	CImage*		pVImgThumbLine;

	CImage*		pHImgLeft;
	CImage*		pHImgRight;
	CImage*		pHImgBack;
	CImage*		pHImgThumbLeft;
	CImage*		pHImgThumbBody;
	CImage*		pHImgThumbRight;
	CImage*		pHImgThumbLine;

	CImage*		pImageCorner;

	HBRUSH		hBrBackground;
	//COLORREF	crBack;

	tagSCROLLBAR_SKIN_INFO()
		: pVImgUp( NULL ),
		  pVImgDown( NULL ),
		  pVImgBack( NULL ),
		  pVImgThumbTop( NULL ),
		  pVImgThumbBody( NULL ),
		  pVImgThumbBottom( NULL ),
		  pVImgThumbLine( NULL ),
		  pHImgLeft( NULL ),
		  pHImgRight( NULL ),
		  pHImgBack( NULL ),
		  pHImgThumbLeft( NULL ),
		  pHImgThumbBody( NULL ),
		  pHImgThumbRight( NULL ),
		  pHImgThumbLine( NULL ),
		  pImageCorner(NULL),
		  hBrBackground(NULL)
		 // crBack ( RGB( 0xff, 0xff, 0xff ) )
	{
	}
} SCROLLBAR_SKIN_INFO;

class CMidWnd : public CWindowImpl<CMidWnd, CWindow>
{
public:
	DECLARE_WND_CLASS_EX( _T( "SkinScrollMidWnd" ), CS_DBLCLKS, COLOR_WINDOW );

	BEGIN_MSG_MAP ( CMidWnd )
	MESSAGE_HANDLER ( WM_ERASEBKGND, OnEraseBaGnd )
	END_MSG_MAP()

	LRESULT OnEraseBaGnd ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
	{
		return 0;
	}
};

class CSkinScrollWnd : public CWindowImpl<CSkinScrollWnd, CWindow>
{
public:
	DECLARE_WND_CLASS_EX( _T( "SkinScrollWnd" ), CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW, COLOR_WINDOW );

	CSkinScrollWnd ( void );
	virtual ~CSkinScrollWnd ( void );

	// Windows Procedure
	static LRESULT CALLBACK WindowProc ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	BOOL Initialize ( CONST HWND hWnd, CONST SCROLLBAR_SKIN_INFO& ResSet);
	BOOL Finalize ( VOID );
	BOOL SetSkinInfo ( CONST SCROLLBAR_SKIN_INFO& ResSet );

	VOID setWidth ( int nWidth )
	{
		m_nScrollWidth = nWidth;
		m_sbVertical.setWidth ( nWidth );
	}

protected:

	BEGIN_MSG_MAP ( CSkinScrollWnd )
	MESSAGE_HANDLER ( WM_SIZE, OnSize )
	MESSAGE_HANDLER ( WM_VSCROLL, OnVScroll )
	MESSAGE_HANDLER ( WM_HSCROLL, OnHScroll )
	MESSAGE_HANDLER ( WM_PAINT, OnPaint )
	MESSAGE_HANDLER ( WM_ERASEBKGND, OnEraseBaGnd )
	MESSAGE_HANDLER ( WM_TIMER, OnTimer )
	MESSAGE_HANDLER ( UM_DESTMOVE, OnDestMove )
	MESSAGE_HANDLER ( WM_RBUTTONDOWN, OnRButtonDown )
	MESSAGE_HANDLER ( WM_DESTROY, OnDestroy )
	MESSAGE_HANDLER ( UM_V_THUMBTRACK_MSG, OnVThumbTrack )
	MESSAGE_HANDLER ( UM_H_THUMBTRACK_MSG, OnHThumbTrack )
	MESSAGE_HANDLER ( UM_UPDATE_SCROLL_CORNER, OnUpdateScrollCorner )
	END_MSG_MAP()

	// Windows Event Handler
	LRESULT OnSize ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT OnEraseBaGnd ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT OnVScroll ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT OnHScroll ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT OnPaint ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT OnTimer ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT OnDestMove ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT OnNCDestroy ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT OnRButtonDown ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT OnDestroy( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT OnVThumbTrack( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT OnHThumbTrack( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT OnUpdateScrollCorner( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );

protected:
	BOOL _DoPaint();

private:
	// Member Variable
	INT32			m_nScrollWidth;
	INT32			m_nAngleType;
	NDGraphics::CGDIPImage		m_imgCorner;
	CBrushHandle	m_brBackground;

	CSkinScrollBar	m_sbHorizontal;
	CSkinScrollBar	m_sbVertical;
	CMidWnd			m_MidWnd;
	BOOL			m_bOperation;
	HWND			m_hTargetWnd;
	int		m_nPos;
	WNDPROC			m_fnOldWinProc;
};
