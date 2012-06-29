#pragma once
#include "drawtool.h"
#include "DrawObject.h"

class CFreeDrawTool : public CDrawTool
{
public:
	CFreeDrawTool(LPDRAWOBJBASEINFO pInfo, ToolType nToolType, LPCTSTR strName);

	virtual BOOL OnLButtonDown(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	virtual BOOL OnLButtonUp(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(IDrawCanvas* pView, UINT nFlags, const CPoint& point);

	virtual void InitResource();

private:
	HCURSOR m_hFreeDrawCur;
};
