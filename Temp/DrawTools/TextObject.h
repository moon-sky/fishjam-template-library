#pragma once

#include "DrawTypeDefine.h"
#include "DrawObject.h"
#include "RichEditPanel.h"

class CTextObject 
	: public CDrawObject
	, public INotifyCallBack
{
public:
	CTextObject(IDrawCanvas* pDrawCanvas, const CRect& position, DrawObjectType objType, const DRAWOBJBASEINFO& stDrawObjInfo);
	virtual ~CTextObject();

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if (m_pRichEditPanel)
		{
			return m_pRichEditPanel->PreTranslateMessage(pMsg);
		}
		return FALSE;
	}

public:
	virtual void Draw(HDC hDC, BOOL bOriginal);
	virtual void MoveHandleTo(int nHandle, CPoint point);
	virtual CDrawObject* Clone();
	virtual void SetActive(BOOL bActive);
	virtual void MoveTo(const CRect& position);

	//INotifyCallBack
	virtual void OnNotify(int iNotify, void* pParam);
	virtual void OnExpand(int nDir, int nValue);

	virtual BOOL HitTestMove(CPoint point);
	virtual BOOL HitTestActive(CPoint point);

	void CheckTextRect();

	CRichEditPanel* GetRichEditPanel()
	{
		FTLASSERT(m_pRichEditPanel);
		return m_pRichEditPanel;
	}
protected:
	CRichEditPanel*	m_pRichEditPanel;	
};

