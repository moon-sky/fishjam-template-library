#pragma once
#include "DrawTool.h"

class CTextTool : public CDrawTool
{
	// Constructors
public:
	CTextTool(LPDRAWOBJBASEINFO pDrawObjInfo, LPCTSTR strName);

	// Implementation
	virtual BOOL OnLButtonDown(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	virtual BOOL OnLButtonDblClk(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	virtual BOOL OnLButtonUp(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	virtual BOOL HandleControlMessage(IDrawCanvas* pView, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult);

	virtual BOOL IsNeedClip();
	virtual void InitResource();
protected:
	HCURSOR m_hTextCursor;
};
