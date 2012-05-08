#pragma once
#include "DrawTool.h"

class CRectTool : public CDrawTool
{
	// Constructors
public:
	CRectTool(ToolType nToolType);

	// Implementation
	virtual void OnLButtonDown(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDblClk(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
};
