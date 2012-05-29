#include "stdafx.h"
#include "TextObject.h"
#include "DrawCanvas.h"
#include <ftlGdi.h>
#include <ftlControls.h>

CTextObject::CTextObject(IDrawCanvas* pDrawCanvas, const CRect& position, DrawObjectType objType)
: CDrawObject(pDrawCanvas, position, objType)
{
	HRESULT hr = E_FAIL;

	m_pRichEditPanel = new CRichEditPanel();
	m_pRichEditPanel->Init(pDrawCanvas->GetHWnd(), &position);
	//m_pRichEditPanel->OnTxInPlaceActivate(&position);
}

CTextObject::~CTextObject()
{
	if (m_pRichEditPanel)
	{
		m_pRichEditPanel->Release();
		m_pRichEditPanel = NULL;
	}
}

void CTextObject::Draw(HDC hDC, BOOL bOriginal)
{
	CRect rcClient = m_position;
	CDCHandle dc(hDC);
	dc.Rectangle(rcClient);

	m_pRichEditPanel->DoPaint(hDC);
	//CDCHandle dc(hDC);
	//dc.DrawText(TEXT("In CTextObject Draw"), -1, m_position, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void CTextObject::MoveHandleTo(int nHandle, CPoint point)
{
	CDrawObject::MoveHandleTo(nHandle, point);
	CRect rcRichEditPanel = m_position;
	rcRichEditPanel.DeflateRect(1, 1);

	m_pRichEditPanel->SetClientRect(&rcRichEditPanel, FALSE);
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

void CTextObject::SetActive(BOOL bActive)
{
	CDrawObject::SetActive(bActive);
	if (m_pRichEditPanel)
	{
		m_pRichEditPanel->SetActive(bActive);
	}
}

void CTextObject::MoveTo(const CRect& position)
{
	if (position == m_position)
	{
		return;
	}

	m_pDrawCanvas->InvalObject(this);
	m_position = position;
	CRect rcRichEditPanel = m_position;
	rcRichEditPanel.DeflateRect(1, 1);
	m_pRichEditPanel->SetClientRect(&rcRichEditPanel, FALSE);
	m_pDrawCanvas->InvalObject(this);
	//m_pDocument->SetModifiedFlag();
}