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

	// Set up the example rich text
	m_pRichEditPanel->SetText(
		L"The quick brown fox jumped over the lazy dog. "
		L"The quick brown fox jumped over the lazy dog. "
		L"The quick brown fox jumped over the lazy dog. "
		L"The quick brown fox jumped over the lazy dog. "
		L"The quick brown fox jumped over the lazy dog. "
		L"The quick brown fox jumped over the lazy dog. "
		L"The quick brown fox jumped over the lazy dog. ");

	CComPtr<ITextRange> spOriRange;
	COM_VERIFY(m_pRichEditPanel->Range(0, 100, &spOriRange));
	CComPtr<ITextFont> spFont;
	COM_VERIFY(spOriRange->GetFont(&spFont));
	FTL::CFTextFontDumper	textFontDumper(spFont, FTL::CFOutputWindowInfoOutput::Instance(), 0);

	COLORREF colours[] = {
		RGB(0x00,0x00,0x77),
		//RGB(0x00,0xFF,0x00),
		//RGB(0x00,0x00,0xFF),
		//RGB(0xFF,0xFF,0x00),
		//RGB(0xFF,0x00,0xFF),
		//RGB(0x00,0xFF,0xFF),
		//RGB(0x00,0x00,0x00)
	};
	for (int i = 0; i < sizeof colours / sizeof colours[0]; i++)
	{
		CComPtr<ITextRange> range;
		m_pRichEditPanel->Range(i*46,(i+1)*46,&range);

		CComPtr<ITextFont> font;
		range->GetFont(&font);
		font->SetForeColor(colours[i]);
		font->SetSize((float)(9+i));

		FTL::CFTextFontDumper	newTextFontDumper(font, FTL::CFOutputWindowInfoOutput::Instance(), 0);
	}
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

void CTextObject::SetActive(BOOL bActive)
{
	__super::SetActive(bActive);
	if (m_pRichEditPanel)
	{
		m_pRichEditPanel->SetActive(bActive);
	}
}