#pragma once
#include "drawtool.h"

class CMoveTool : public CDrawTool
{
public:
	CMoveTool(LPDRAWOBJBASEINFO pDrawObjInfo);
	virtual ~CMoveTool();

	virtual BOOL OnLButtonUp(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	virtual BOOL OnLButtonDown(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
private:
	CPoint m_ptBegin;
	BOOL   m_bDrag;
};
