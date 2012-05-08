#include "stdafx.h"
#include "TextObject.h"
#include "DrawCanvas.h"
#include <ftlGdi.h>

CTextObject::CTextObject(IDrawCanvas* pDrawCanvas, const CRect& position, DrawObjectType objType)
: CDrawObject(pDrawCanvas, position, objType)
{
	m_pRichEditPanel = new CRichEditPanel(pDrawCanvas->GetHWnd(), &position);
	m_pRichEditPanel->Init();
}

CTextObject::~CTextObject()
{
	m_pRichEditPanel->Release();
	m_pRichEditPanel = NULL;
}

void CTextObject::Draw(HDC hDC, BOOL bOriginal)
{
	m_pRichEditPanel->DoPaint(hDC);
	//CDCHandle dc(hDC);
	//dc.DrawText(TEXT("In CTextObject Draw"), -1, m_position, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void CTextObject::MoveHandleTo(int nHandle, CPoint point)
{
	CDrawObject::MoveHandleTo(nHandle, point);
	m_pRichEditPanel->SetClientRect(&m_position, TRUE);
	m_pDrawCanvas->InvalObject(this);	
}

CDrawObject* CTextObject::Clone()
{
	CTextObject* pClone = new CTextObject(m_pDrawCanvas, m_position, m_objType);
	
	pClone->m_bPen = m_bPen;
	pClone->m_logpen = m_logpen;
	pClone->m_bBrush = m_bBrush;
	pClone->m_logbrush = m_logbrush;
	
	return pClone;
}
