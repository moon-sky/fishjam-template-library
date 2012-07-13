#pragma once

#include "DrawTypeDefine.h"
#include "DrawObject.h"
#include "RichEditPanel.h"


typedef std::tr1::shared_ptr<CRichEditPanel> CShareEditPtr;

class CTextObject 
	: public CDrawObject
	, public INotifyCallBack
{
public:
	CTextObject(IDrawCanvas* pDrawCanvas, const CRect& position, DrawObjectType objType,
		DRAWOBJBASEINFO& stDrawObjInfo, CShareEditPtr pEditPanel);
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

	virtual void SetPosition(const CRect& pos, BOOL bCheckSize);

	//CRichEditPanel* GetRichEditPanel()
	//{
	//	FTLASSERT(m_pRichEditPanel);
	//	return m_pRichEditPanel;
	//}
	virtual BOOL CheckAvailObject();

	virtual BOOL Undo();
	virtual BOOL Redo();

	virtual BOOL IsCanUndo();
	virtual BOOL IsCanRedo();

	virtual BOOL BeginEdit();
	virtual BOOL EndEdit(BOOL bIsPushUndo = TRUE);
protected:
	void _OnTextRequestResizeNotify(REQRESIZE* pReqResize);
	void _OnTextSelectChangeNotify(SELCHANGE* pSelChange);
	void _OnKillFocus(long nCount);
	void _OnSetFocus();
protected:
	//CRichEditPanel*	m_pRichEditPanel;
	void _InitDrawInfo(const DRAWOBJBASEINFO& stDrawObjInfo);
	CShareEditPtr   m_pShareEditPtr;
	//BOOL            m_bIsCanUndo;
	//BOOL            m_bIsCanRedo;
};

