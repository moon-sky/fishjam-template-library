#pragma once
#include "DrawTool.h"

class CRectTool : public CDrawTool
{
	// Constructors
public:
	CRectTool(LPDRAWOBJBASEINFO pInfo, ToolType nToolType, LPCTSTR strName);

	// Implementation
	virtual BOOL OnLButtonDown(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	virtual BOOL OnLButtonDblClk(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	virtual BOOL OnLButtonUp(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
};
