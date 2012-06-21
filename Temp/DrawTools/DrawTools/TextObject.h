#pragma once

#include "DrawTypeDefine.h"
#include "DrawObject.h"
#include "RichEditPanel.h"

class CTextObject 
	: public CDrawObject
	, public INotifyCallBack
{
public:
	CTextObject(IDrawCanvas* pDrawCanvas, const CRect& position, DrawObjectType objType,
		DRAWOBJBASEINFO& stDrawObjInfo);
	virtual ~CTextObject();

	//virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	virtual void Draw(HDC hDC, BOOL bOriginal);
	virtual void DrawTracker(HDC hDC, TrackerState state, BOOL bDrawSelectTool);
	virtual void MoveHandleTo(int nHandle, CPoint point);
	virtual CDrawObject* Clone();
	virtual void SetActive(BOOL bActive);
	virtual void MoveTo(const CRect& position);

	//INotifyCallBack
	virtual void OnNotify(int iNotify, void* pParam);
	//virtual void OnExpand(int nDir, int nValue);

	virtual BOOL HitTestMove(CPoint point);
	virtual BOOL HitTestActive(CPoint point);
	virtual HCURSOR GetActiveCursor();
	virtual void NormalizePosition();
	virtual BOOL UpdateDrawInfo(const DRAWOBJBASEINFO& stDrawObjInfo);
	virtual BOOL HandleControlMessage(IDrawCanvas* pView, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult);

	virtual void SetPosition(const CRect& pos);

	CRichEditPanel* GetRichEditPanel()
	{
		FTLASSERT(m_pRichEditPanel);
		return m_pRichEditPanel;
	}
	virtual BOOL CheckAvailObject();
protected:
	void _OnTextRequestResizeNotify(REQRESIZE* pReqResize);
	void _OnTextSelectChangeNotify(SELCHANGE* pSelChange);
	void _OnKillFocus(long nCount);
	void _OnSetFocus();
protected:
	CRichEditPanel*	m_pRichEditPanel;
	BOOL            m_bAvailObject;
};

