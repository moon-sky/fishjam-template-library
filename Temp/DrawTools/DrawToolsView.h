// DrawToolsView.h : interface of the CDrawToolsView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DrawCanvas.h"

class CDrawToolsView : public CWindowImpl<CDrawToolsView>,
	public CDrawCanvas<CDrawToolsView>
{
public:
	DECLARE_WND_CLASS(NULL)

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP_EX(CDrawToolsView)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		MSG_WM_KEYDOWN(OnKeyDown)
		MSG_WM_CHAR(OnChar)
	END_MSG_MAP()

	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	void OnMouseMove(UINT nFlags, CPoint point);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

public:
	virtual	CPoint GetOffset()
	{
		return m_rcDrawTarget.TopLeft();
	}
	virtual CRect GetDrawTarget()
	{
		return m_rcDrawTarget;
	}
	virtual void ClientToDoc(CPoint* pPoint)
	{

	}
	virtual void ClientToDoc(CRect* pRect)
	{

	}
	virtual void DocToClient(CPoint* pPoint)
	{

	}
	virtual void DocToClient(CRect* pRect)
	{

	}
private:
	CRect m_rcDrawTarget;

	void _RefreshDrawCanvasInfo();
};
