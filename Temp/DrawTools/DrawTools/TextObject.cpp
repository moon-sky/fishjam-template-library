#include "stdafx.h"
#include "TextObject.h"
#include "DrawCanvas.h"
#include <ftlGdi.h>
//#include <ftlControls.h>

CTextObject::CTextObject(IDrawCanvas* pDrawCanvas, const CRect& position, DrawObjectType objType, 
						 const DRAWOBJBASEINFO& stDrawObjInfo, LPLOGFONT pLogFont)
: CDrawObject(pDrawCanvas, position, objType, stDrawObjInfo)
{
	HRESULT hr = E_FAIL;

	m_pRichEditPanel = new CRichEditPanel();
	m_pRichEditPanel->Init(pDrawCanvas->GetHWnd(), &position, pDrawCanvas, pLogFont, this);
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

BOOL CTextObject::PreTranslateMessage(MSG* pMsg)
{
	if (m_pRichEditPanel && m_pRichEditPanel->IsActive())
	{
		BOOL bWillTranslateToRTPanel = TRUE;
		//if (WM_MOUSEFIRST <= pMsg->message && pMsg->message <= WM_MOUSELAST)
		//{
		//	CPoint ptClient = pMsg->pt;
		//	ScreenToClient(m_pDrawCanvas->GetHWnd(), &ptClient);
		//	bWillTranslateToRTPanel = HitTestActive(ptClient);
		//}
		if (bWillTranslateToRTPanel)
		{
			return m_pRichEditPanel->PreTranslateMessage(pMsg);
		}
	}
	return FALSE;
}

void CTextObject::Draw(HDC hDC, BOOL bOriginal)
{
	m_pRichEditPanel->DoPaint(hDC);
	//CDCHandle dc(hDC);
	//dc.DrawText(TEXT("In CTextObject Draw"), -1, m_position, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void CTextObject::DrawTracker(HDC hDC, TrackerState state, BOOL bDrawSelectTool)
{
	CRect rcClient = m_position;
	m_pDrawCanvas->DocToClient(&rcClient);

	CDCHandle dc(hDC);
	HBRUSH hOldBrush = dc.SelectBrush((HBRUSH)GetStockObject(NULL_BRUSH));
	CPen pen;
	pen.CreatePen(PS_DOT, 1, RGB(0, 0, 0));
	HPEN hOldPen = dc.SelectPen(pen);
	dc.Rectangle(rcClient);
	dc.SelectPen(hOldPen);
	dc.SelectBrush(hOldBrush);

	CDrawObject::DrawTracker(hDC, state, bDrawSelectTool);
}

void CTextObject::MoveHandleTo(int nHandle, CPoint point)
{
	CDrawObject::MoveHandleTo(nHandle, point);

	m_pRichEditPanel->SetClientBound(&m_position, NULL, FALSE);
	m_pDrawCanvas->InvalObject(this);
}

CDrawObject* CTextObject::Clone()
{
	DRAWOBJBASEINFO stDrawInfo;
	stDrawInfo.logbrush = m_logbrush;
	stDrawInfo.logpen   = m_logpen;
	CTextObject* pClone = new CTextObject(m_pDrawCanvas, m_position, m_objType, stDrawInfo, NULL);

	pClone->m_bPen = m_bPen;
	//pClone->m_logpen = m_logpen;
	pClone->m_bBrush = m_bBrush;
	//pClone->m_logbrush = m_logbrush;

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
	m_pRichEditPanel->SetClientBound(&m_position, NULL, FALSE);
	m_pDrawCanvas->InvalObject(this);
	//m_pDocument->SetModifiedFlag();
}

void CTextObject::OnNotify(int iNotify, void* pParam)
{
	switch (iNotify)
	{
	case EN_REQUESTRESIZE:
		{
			REQRESIZE* pReqResize = (REQRESIZE*)pParam;
			FTLASSERT(pReqResize->rc.bottom > pReqResize->rc.top);
			int nWantHeight = pReqResize->rc.bottom - pReqResize->rc.top + RTPANEL_MARGIN_TOP + RTPANEL_MARGIN_BOTTOM;

			CSize szMin = m_pRichEditPanel->GetMinBoundSize(RTPANEL_MIN_ROW_COUNT, RTPANEL_MIN_COL_COUNT);
			if (nWantHeight < szMin.cy )
			{
				nWantHeight = szMin.cy;
			}

			int nOldHeight = FTL_ABS(m_position.Height());
			if ( nWantHeight > nOldHeight)
			{
				if (m_position.top < m_position.bottom)
				{
					m_position.bottom = m_position.top + nWantHeight;
				}
				else
				{
					m_position.top = m_position.bottom - nWantHeight;
				}
				m_pRichEditPanel->SetClientBound(m_position, NULL, FALSE);
				m_pDrawCanvas->InvalObject(this);
			}
		}
		break;
	}
	//FTLTRACE(TEXT("In CTextObject::OnNotify, iNotify=%d, pParam=0x%x\n"), iNotify, pParam);
}

BOOL CTextObject::HitTestMove(CPoint point)
{
	if (m_position.PtInRect(point))
	{
		CRect rect = m_position;
		if (abs(rect.top - point.y) <= 3 || abs(rect.left - point.x) <= 3
			|| abs(rect.bottom - point.y) <= 3 || abs(rect.right - point.x) <= 3)
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CTextObject::HitTestActive(CPoint point)
{
	CPoint ptLogical = point;
	m_pDrawCanvas->ClientToDoc(&ptLogical);

	if (m_position.PtInRect(ptLogical) && ! HitTestMove(ptLogical))
	{
		return TRUE;
	}
	return FALSE;
}

HCURSOR CTextObject::GetActiveCursor()
{
	//HRESULT hr = E_FAIL;
	//CComPtr<ITextServices> spTextService;
	//COM_VERIFY(m_pRichEditPanel->GetTextServices(&spTextService));
	//spTextService->OnTxSetCursor();

	return ::LoadCursor(NULL, IDC_IBEAM);
}

void CTextObject::NormalizePosition()
{
	CDrawObject::NormalizePosition();
	m_pRichEditPanel->SetClientBound(&m_position, NULL, TRUE);
}

void CTextObject::CheckTextRect()
{
}