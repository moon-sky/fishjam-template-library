#include "stdafx.h"
#include "TextObject.h"
#include "DrawCanvas.h"
#include <ftlGdi.h>
//#include <ftlControls.h>

CTextObject::CTextObject(IDrawCanvas* pDrawCanvas, const CRect& position, DrawObjectType objType, 
						 const DRAWOBJBASEINFO& stDrawObjInfo)
: CDrawObject(pDrawCanvas, position, objType, stDrawObjInfo)
{
	HRESULT hr = E_FAIL;

	m_pRichEditPanel = new CRichEditPanel();
	m_pRichEditPanel->Init(pDrawCanvas->GetHWnd(), &position, pDrawCanvas, &stDrawObjInfo.logfont, 
		stDrawObjInfo.clrFontFore, this);
	UpdateDrawInfo(stDrawObjInfo);
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

//BOOL CTextObject::PreTranslateMessage(MSG* pMsg)
//{
//	//FTLASSERT(pMsg->hwnd == m_pDrawCanvas->GetHWnd());
//	if (pMsg->hwnd == m_pDrawCanvas->GetHWnd() &&  m_pRichEditPanel && m_pRichEditPanel->IsActive())
//	{
//		BOOL bWillTranslateToRTPanel = TRUE;
//		//if (WM_MOUSEFIRST <= pMsg->message && pMsg->message <= WM_MOUSELAST)
//		//{
//		//	CPoint ptClient = pMsg->pt;
//		//	ScreenToClient(m_pDrawCanvas->GetHWnd(), &ptClient);
//		//	bWillTranslateToRTPanel = HitTestActive(ptClient);
//		//}
//		if (bWillTranslateToRTPanel)
//		{
//			return m_pRichEditPanel->PreTranslateMessage(pMsg);
//		}
//	}
//	return FALSE;
//}

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

	m_pRichEditPanel->SetClientBound(&m_position, NULL, TRUE); //FALSE);
	m_pDrawCanvas->InvalObject(this);
}

CDrawObject* CTextObject::Clone()
{
	DRAWOBJBASEINFO stDrawInfo;
	stDrawInfo.logbrush = m_logbrush;
	stDrawInfo.logpen   = m_logpen;
	CTextObject* pClone = new CTextObject(m_pDrawCanvas, m_position, m_objType, stDrawInfo);
	
	HRESULT hr = E_FAIL;
	CComPtr<IStream>	spStream;
	COM_VERIFY(m_pRichEditPanel->GetTextStream(0, -1, &spStream));
	COM_VERIFY(pClone->m_pRichEditPanel->SetTextStream(0, -1, spStream));
		
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
	m_pRichEditPanel->SetClientBound(&m_position, NULL, TRUE);//FALSE);
	m_pDrawCanvas->InvalObject(this);
	//m_pDocument->SetModifiedFlag();
}

void CTextObject::_OnTextRequestResizeNotify(REQRESIZE* pReqResize)
{
	//if (m_pDrawCanvas->IsCapture())
	//{
	//	return;
	//}
	FTLASSERT(pReqResize->rc.bottom > pReqResize->rc.top);
	int nWantHeight = pReqResize->rc.bottom - pReqResize->rc.top + RTPANEL_MARGIN_TOP + RTPANEL_MARGIN_BOTTOM;

	CSize szMin = m_pRichEditPanel->GetMinBoundSize(RTPANEL_MIN_ROW_COUNT, RTPANEL_MIN_COL_COUNT);
	if (nWantHeight < szMin.cy )
	{
		nWantHeight = szMin.cy;
	}
	BOOL bWillSetBound = FALSE;
	CRect rcPosition = m_position;
	rcPosition.NormalizeRect();

	//if (!rcTarget.IsRectEmpty())
	{
		int nOldHeight = FTL_ABS(rcPosition.Height());
		if ( nWantHeight > nOldHeight)
		{
			rcPosition.bottom = rcPosition.top + nWantHeight;
			bWillSetBound = TRUE;
		}

		int nOldWidth = FTL_ABS(rcPosition.Width());
		if (szMin.cx > nOldWidth)
		{
			rcPosition.right = rcPosition.left + szMin.cx;
			bWillSetBound = TRUE;
		}
	}
	CRect rcTarget = m_pDrawCanvas->GetDrawTarget();
	rcTarget.IntersectRect(rcTarget, rcPosition);
	if (rcTarget != m_position && bWillSetBound)
	{
		m_position = rcTarget;
		m_pRichEditPanel->SetClientBound(m_position, NULL, TRUE);
		m_pDrawCanvas->InvalObject(this);
	}
}

void CTextObject::_OnTextSelectChangeNotify(SELCHANGE* pSelChange)
{
	const int _cchStyleName = 64;
	TCHAR szFontName[_cchStyleName] = {0};
	int nFontSize = 0;

	HRESULT hr = E_FAIL;
	long nStart = pSelChange->chrg.cpMin;
	long nEnd = pSelChange->chrg.cpMax;

	COM_VERIFY_EXCEPT1(m_pRichEditPanel->GetTextFontName(nStart, nEnd,
		szFontName, _countof(szFontName)), S_FALSE);
	if (S_FALSE == hr)
	{
		//multi select, szFontName is NULL
	}
	COM_VERIFY_EXCEPT1(m_pRichEditPanel->GetTextFontSize(nStart, nEnd, &nFontSize), S_FALSE)
	if (S_FALSE == hr)
	{
		//multi select , nFontSize is -1
	}
	COLORREF clrText = RGB(0, 0, 0);
	COM_VERIFY_EXCEPT1(m_pRichEditPanel->GetTextForeColor(nStart, nEnd, &clrText), S_FALSE);
}

void CTextObject::OnNotify(int iNotify, void* pParam)
{
	switch (iNotify)
	{
	case EN_REQUESTRESIZE:
		{
			_OnTextRequestResizeNotify((REQRESIZE*)pParam);
			break;
		}
	case EN_SELCHANGE:
		{
			_OnTextSelectChangeNotify((SELCHANGE*)pParam);
			break;
		}
	default:
		break;
	}
	//FTLTRACE(TEXT("In CTextObject::OnNotify, iNotify=%d, pParam=0x%x\n"), iNotify, pParam);
}

BOOL CTextObject::HitTestMove(CPoint point)
{
	CRect rcDevice = m_position;
	m_pDrawCanvas->DocToClient(&rcDevice);
	rcDevice.NormalizeRect();
	if (rcDevice.PtInRect(point))
	{
		//CRect rect = m_position;
		if ( abs(rcDevice.left - point.x) <= TRACK_MARGIN
			|| abs(rcDevice.top - point.y) <= TRACK_MARGIN 
			|| abs(rcDevice.right - point.x) <= TRACK_MARGIN
			|| abs(rcDevice.bottom - point.y) <= TRACK_MARGIN 
			)
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CTextObject::HitTestActive(CPoint point)
{
	CRect rcDevice = m_position;
	m_pDrawCanvas->DocToClient(&rcDevice);
	if (rcDevice.PtInRect(point) && ! HitTestMove(point))
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

void CTextObject::UpdateDrawInfo(const DRAWOBJBASEINFO& stDrawObjInfo)
{
	if (m_pRichEditPanel)
	{
		m_pRichEditPanel->SetTextForeColor(0, 0, stDrawObjInfo.clrFontFore);
		LOGFONT stLogFont = stDrawObjInfo.logfont;
		m_pRichEditPanel->SetTextFont(0, 0, &stLogFont, RICH_EDIT_PANEL_FONT_MASK_ALL);
	}
}