// DrawToolsView.h : interface of the CDrawToolsView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DrawCanvas.h"
#include <atlscrl.h>

class CDrawToolsView 
	//: public CWindowImpl<CDrawToolsView>
	: public CZoomScrollWindowImpl<CDrawToolsView>
	, public CDrawCanvas<CDrawToolsView>
{
public:
	DECLARE_WND_CLASS(NULL)

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP_EX(CDrawToolsView)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_MOUSEWHEEL(OnMouseWheel)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		MSG_WM_KEYDOWN(OnKeyDown)
		MSG_WM_CHAR(OnChar)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		
		CHAIN_MSG_MAP( CZoomScrollWindowImpl<CDrawToolsView> )
	END_MSG_MAP()

	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	void OnMouseMove(UINT nFlags, CPoint point);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	BOOL OnEraseBkgnd(CDCHandle dc);
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	void DoPaint(CDCHandle dc);

public:
	BOOL				m_bInited;
	BOOL				m_bIsDrawing;
	BOOL				m_bImageChanged;
	BOOL				m_bClipCursor;

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
	virtual void SetCurrentOffsetPoint(LPPOINT lpPoint);
	virtual void GetCurrentOffsetPoint(LPPOINT lpPoint);

	virtual BOOL GetImageByRect(const CRect& rcSrc, CImage& Image);

	//virtual void OnPrepareDC(HDC hDC);

	//void CNCaptureView::PrepareDC(CDCHandle dc);
	void SelectToolTypeByMenu(const CPoint& ptPoint);

	virtual BOOL BackupDrawObjectData(LPCTSTR strName);
	void PrepareDC(CDCHandle dc);

private:
	CRect m_rcDrawTarget;

	void _RefreshDrawCanvasInfo();
};
