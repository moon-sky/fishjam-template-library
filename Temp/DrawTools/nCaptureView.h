// nCaptureView.h : interface of the CNCaptureView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "NPVPhotoCalcRect.h"
#include "./AlphaBitmapUI/Canvas.h"
#include "./DrawTools/DrawCanvas.h"
#include "CapImageObj.h"
#include <atlscrl.h>
#ifdef FTL_DEBUG
#include <ftlWindow.h>
#endif 

#define ZOOM_COUNT	17

class CNCaptureView : public CZoomScrollWindowImpl<CNCaptureView>,
	public CDrawCanvas<CNCaptureView>
{
	DISABLE_COPY_AND_ASSIGNMENT(CNCaptureView);
	//typedef CWindowImpl<CNCaptureView> super;

public:
	DECLARE_WND_CLASS(TEXT("NCaptureView"))

	CNCaptureView();
	~CNCaptureView();

	BEGIN_MSG_MAP_EX(CNCaptureView)
#ifdef FTL_DEBUG
		//DUMP_WINDOWS_MSG(__FILE__LINE__, DEFAULT_DUMP_FILTER_MESSAGE, uMsg, wParam, lParam)
#endif 
		//MSG_WM_CREATE(OnCreate)
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		//MSG_WM_SIZE(OnSize)
		MSG_WM_SETFOCUS(OnSetFocus)
		MSG_WM_KILLFOCUS(OnKillFocus)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_MOUSELEAVE(OnMouseLeave)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		MSG_WM_LBUTTONDBLCLK(OnLButtonDblClk)
		MSG_WM_SETCURSOR(OnSetCursor)
		MSG_WM_MOUSEWHEEL(OnMouseWheel)
		MSG_WM_HSCROLL(OnHScroll)
		MSG_WM_VSCROLL(OnVScroll)
		MSG_WM_CANCELMODE(OnCancelMode)
		//MSG_WM_CAPTURECHANGED(OnCaptureChanged)
		CHAIN_MSG_MAP( CZoomScrollWindowImpl<CNCaptureView> )
	END_MSG_MAP()

public:
	BOOL Initialize();
	VOID Finalize();
	void SetNotifyWnd( const HWND hWndNotify );
	VOID InvalidateFrame(LPRECT lpRect, BOOL bRePaint);
	BOOL CopyImageToClipboard();
	BOOL CropImage();
	BOOL SelectAll();
	BOOL ShowImageInMsPaint();

	void DoPaint(CDCHandle dc);
	
	BOOL NextZoom(BOOL bBigger);
	BOOL SetZoom(float zoom, BOOL bSyncFrame = TRUE, BOOL bUseCenterPoint = FALSE);
	float GetZoom();

	static const float s_FixedZoomScales[ZOOM_COUNT];
	static const int   s_NormalZoomIndex;

//Event Handler
	void OnCaptureImageChanged();

//Windows message handler
	BOOL PreTranslateMessage(MSG* pMsg);

	//int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnDestroy();
	BOOL OnEraseBkgnd(CDCHandle dc);
	//void OnSize(UINT nType, CSize size);
	void OnSetFocus(CWindow wndOld);
	void OnKillFocus(CWindow wndFocus);
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	void OnMouseMove(UINT nFlags, CPoint point);
	void OnMouseLeave();
	void OnLButtonDblClk(UINT nFlags, CPoint point);

	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar);
	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar);

	void OnCancelMode();
	BOOL OnSetCursor(CWindow wnd, UINT nHitTest, UINT message);
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//void OnCaptureChanged(CWindow wnd);
//Override
	void PrepareDC(CDCHandle dc);


	virtual	CPoint GetOffset()
	{
		return m_rcDrawTarget.TopLeft();
	}
	virtual CRect GetDrawTarget()
	{
		return m_rcDrawTarget;
	}

	virtual void ClientToDoc(CPoint* pPoint);
	virtual void ClientToDoc(CRect* pRect);
	virtual void DocToClient(CPoint* pPoint);
	virtual void DocToClient(CRect* pRect);

	//virtual void OnPrepareDC(HDC hDC);

	//void CNCaptureView::PrepareDC(CDCHandle dc);

private:
	BOOL				m_bInited;
	BOOL				m_bIsDrawing;
	BOOL				m_bImageChanged;
	BOOL				m_bClipCursor;
	//int					m_iFixedZoomIndex;
	float				m_fLastZoomFactor;
	HWND				m_hWndNotify;

	CCapImageObj*		m_pImage;
	NDGraphics::CGDIPImage* m_pDefaultImage;
	CCanvas				m_canvasImage;
	BOOL				m_bTrackMouse;

	CBrush   m_brBackground;
	CNPVPhotoCalcRect*  m_pCalcRect;
	CRect				m_rcDrawTarget;
	//CVirtualViewLayer*	m_pImageViewLayer;
	CString				m_strMousePosInfo;
	CString				m_strMouseSelectInfo;

	void _CalcImageSize();
	BOOL _DrawCacheImage(const CRect& rcClient, const CRect& rcImage);
	void _EventHook();
	void _EventUnHook();
	void _SetScrollInfo(float newZoomScale, BOOL bUseCenterPoint = FALSE);
	void _UpdateMousePosInfo(const CPoint& point);
	int  _GetNextFixedZoomIndex(float curZoom, BOOL bBigger);
	CRect _GetPhotoRect( CRect rcClient, LPSIZE lpSize); // DOUBLE dZoomFactor, BOOL bFirst, BOOL bFirstView )
	CRect _GetSelectRect(const CPoint& ptLogical);
	void _SyncFrameScaleInfo();
	BOOL _EditWithMspaint(LPCTSTR lpszImagePath);
	BOOL _SetSelectRectClipInfo(const CPoint& point);
};
