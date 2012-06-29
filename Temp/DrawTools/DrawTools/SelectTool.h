#pragma once

#include "DrawTypeDefine.h"
#include "DrawTool.h"

class CSelectTool : public CDrawTool
{
	// Constructors
public:
	CSelectTool(LPDRAWOBJBASEINFO pInfo);
	// Implementation
	virtual BOOL OnLButtonDown(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	virtual BOOL OnLButtonDblClk(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	virtual BOOL OnLButtonUp(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	virtual void OnEditProperties(IDrawCanvas* pView);
	virtual BOOL IsNeedClip();
	virtual void InitResource();
private:
	//void _OnCapturedMouseMove(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	//void _OnNotCapturedMouseMove(IDrawCanvas* pView, UINT nFlags, const CPoint& point);

	void _LimitSelectPoint(IDrawCanvas* pView, CPoint& ptClient);
	void _ScrollForSelect(IDrawCanvas* pView, const CPoint& point);
	BOOL _OffView(IDrawCanvas* pView, const CPoint& point);
	HCURSOR	m_hCursorSelect;
	HCURSOR m_hCursorSelecting;

	CRect      m_rcLast;
};

