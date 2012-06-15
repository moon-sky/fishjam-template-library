#pragma once

#include "DrawTypeDefine.h"

class CDrawTool
{
	// Constructors
public:
	CDrawTool(LPDRAWOBJBASEINFO pDrawObjInfo, ToolType nToolType, LPCTSTR strName);

	// Overridables
	virtual BOOL OnSetCursor(HWND hWnd, UINT nHitTest, UINT message);
	virtual BOOL OnLButtonDown(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	virtual BOOL OnLButtonDblClk(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	virtual BOOL OnLButtonUp(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(IDrawCanvas* pView, UINT nFlags, const CPoint& point);
	virtual void OnEditProperties(IDrawCanvas* pView);
	virtual void OnCancel(IDrawCanvas* pView);
	virtual BOOL IsNeedClip();
	virtual BOOL HandleControlMessage(IDrawCanvas* pView, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult);

	ToolType GetToolType() const
	{
		return m_nToolType;
	}

	LPCTSTR GetToolName() const
	{
		return m_strToolName;
	}
protected:
	// Attributes
	HCURSOR		m_hCursor;
	ToolType	m_nToolType;
	void _CheckSelectPostion(IDrawCanvas* pView);
	CString     m_strToolName;
	LPDRAWOBJBASEINFO m_pDrawObjInfo;

private:
	void _ScrollForSelect(IDrawCanvas* pView, const CPoint& point);
};


