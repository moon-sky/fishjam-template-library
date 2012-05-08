#pragma once

#include "DrawTypeDefine.h"

class CDrawTool
{
	// Constructors
public:
	CDrawTool(ToolType nToolType);

	// Overridables
	virtual BOOL OnSetCursor(HWND hWnd, UINT nHitTest, UINT message);
	virtual void OnLButtonDown(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDblClk(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	virtual void OnEditProperties(IDrawCanvas* pView);
	virtual void OnCancel(IDrawCanvas* pView);

	ToolType GetToolType() const
	{
		return m_nToolType;
	}
protected:
	// Attributes
	HCURSOR		m_hCursor;
	ToolType	m_nToolType;
	void _CheckSelectPostion(IDrawCanvas* pView);
};
