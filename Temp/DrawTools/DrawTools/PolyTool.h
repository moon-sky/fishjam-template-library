#pragma once
#include "drawtool.h"

#include "DrawPoly.h"

class CPolyTool : public CDrawTool
{
public:
	CPolyTool(LPDRAWOBJBASEINFO pInfo, ToolType nToolType, LPCTSTR strName);

	virtual BOOL OnLButtonDown(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	virtual BOOL OnLButtonDblClk(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	virtual BOOL OnLButtonUp(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	virtual void OnEditProperties(IDrawCanvas* pView);
	virtual void OnCancel(IDrawCanvas* pView);

private:
	CDrawPoly* m_pCurPolyObject;
};	
