#pragma once

#include "DrawTypeDefine.h"
#include "DrawTool.h"

class CSelectTool : public CDrawTool
{
	// Constructors
public:
	CSelectTool();
	// Implementation
	virtual void OnLButtonDown(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDblClk(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	virtual void OnEditProperties(IDrawCanvas* pView);
	virtual void OnCancel(IDrawCanvas* pView);
private:
	void _OnCapturedMouseMove(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	void _OnNotCapturedMouseMove(IDrawCanvas* pView, UINT nFlags, const CPoint& point);

	void _LimitSelectPoint(IDrawCanvas* pView, CPoint& ptClient);
	void _ScrollForSelect(IDrawCanvas* pView, const CPoint& point);
	BOOL _OffView(IDrawCanvas* pView, const CPoint& point);
	HCURSOR	m_hCursorSelect;
	HCURSOR m_hCursorSelecting;

	CRect m_rcLast;
};

