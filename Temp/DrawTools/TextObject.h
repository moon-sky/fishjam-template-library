#pragma once
#include "DrawObject.h"
#include "RichEditPanel.h"

class CTextObject 
	: public CDrawObject
	, public CMessageFilter
{
public:
	CTextObject(IDrawCanvas* pDrawCanvas, const CRect& position, DrawObjectType objType);
	~CTextObject();

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if (m_pRichEditPanel)
		{
			return m_pRichEditPanel->PreTranslateMessage(pMsg);
		}
		return FALSE;
	}

	// Implementation
public:
	virtual void Draw(HDC hDC, BOOL bOriginal);
	virtual void MoveHandleTo(int nHandle, CPoint point);
	virtual CDrawObject* Clone();
	virtual void SetActive(BOOL bActive);

public:
	CRichEditPanel*	m_pRichEditPanel;	
};
