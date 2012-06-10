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
		const DRAWOBJBASEINFO& stDrawObjInfo,
		LPLOGFONT pLogFont);
	virtual ~CTextObject();

	virtual BOOL PreTranslateMessage(MSG* pMsg);

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

	void CheckTextRect();

	CRichEditPanel* GetRichEditPanel()
	{
		FTLASSERT(m_pRichEditPanel);
		return m_pRichEditPanel;
	}
protected:
	void _OnTextRequestResizeNotify(REQRESIZE* pReqResize);
	void _OnTextSelectChangeNotify(SELCHANGE* pSelChange);
protected:
	CRichEditPanel*	m_pRichEditPanel;	
};

