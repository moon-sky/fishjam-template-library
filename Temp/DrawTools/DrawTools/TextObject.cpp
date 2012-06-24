#include "stdafx.h"
#include "TextObject.h"
#include "DrawCanvas.h"
#include <ftlGdi.h>
//#include <ftlControls.h>

CTextObject::CTextObject(IDrawCanvas* pDrawCanvas, const CRect& position, DrawObjectType objType, 
						 DRAWOBJBASEINFO& stDrawObjInfo)
: CDrawObject(pDrawCanvas, position, objType, stDrawObjInfo)
{
	HRESULT hr = E_FAIL;

	m_pRichEditPanel = new CRichEditPanel();

	if (stDrawObjInfo.strFontName == _T(""))
	{
		stDrawObjInfo.strFontName = _T("Arial");

	}
	if (stDrawObjInfo.nFontSize == -1)
	{
		stDrawObjInfo.nFontSize = 18;
	}

	LOGFONT stFont = {0};
	#pragma TODO(Font Height)
	stFont.lfHeight = stDrawObjInfo.nFontSize;
	StringCchCopy(stFont.lfFaceName, _countof(stFont.lfFaceName), stDrawObjInfo.strFontName);

	m_pRichEditPanel->Init(pDrawCanvas->GetHWnd(), &position, pDrawCanvas, &stFont, 
		stDrawObjInfo.clrFontFore, this);
	UpdateDrawInfo(stDrawObjInfo);
	m_bAvailObject = FALSE;
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

BOOL CTextObject::HandleControlMessage(IDrawCanvas* pView, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
{
	if (m_pRichEditPanel)
	{
		return m_pRichEditPanel->HandleControlMessage(pView, uMsg, wParam, lParam, lResult);
	}
	return FALSE;
}

void CTextObject::SetPosition(const CRect& pos)
{
	CRect rect = pos;
	if (abs(rect.Width()) < 100)
	{
		if (rect.left > rect.right)
		{
			rect.right = rect.left - 100;
		}
		else
		{
			rect.right = rect.left + 100;
		}
	}
	if (abs(rect.Height()) < 30)
	{
		if (rect.top > rect.bottom)
		{
			rect.bottom = rect.top - 30;
		}
		else
		{
			rect.bottom = rect.top + 30;
		}
	}
	m_position = rect;
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
	stDrawInfo.bBrush   = m_bBrush;
	stDrawInfo.bPen     = m_bPen;

	//TCHAR lfFaceName[LF_FACESIZE] = {0};
	//m_pRichEditPanel->GetTextFontName(0, 0, lfFaceName, LF_FACESIZE);
	//stDrawInfo.strFontName = lfFaceName;
	//m_pRichEditPanel->GetTextFontSize(0, 0, &stDrawInfo.nFontSize);
	//m_pRichEditPanel->GetTextForeColor(0, 0, stDrawInfo.clrFontFore);

	CTextObject* pClone = new CTextObject(m_pDrawCanvas, m_position, m_objType, stDrawInfo);
	
	HRESULT hr = E_FAIL;
	CComPtr<IStream>	spStream;
	COM_VERIFY(m_pRichEditPanel->GetTextStream(0, -1, &spStream));
	COM_VERIFY(pClone->m_pRichEditPanel->SetTextStream(0, -1, spStream));
		
	pClone->m_bPen = m_bPen;
	//pClone->m_logpen = m_logpen;
	pClone->m_bBrush = m_bBrush;
	//pClone->m_logbrush = m_logbrush;
	pClone->m_bAvailObject = m_bAvailObject;
	pClone->m_bActive      = m_bActive;
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
	//m_position = position;
	SetPosition(position);
	m_pRichEditPanel->SetClientBound(&m_position, NULL, TRUE);//FALSE);
	m_pDrawCanvas->InvalObject(this);
	//m_pDocument->SetModifiedFlag();
}

void CTextObject::_OnTextRequestResizeNotify(REQRESIZE* pReqResize)
{
	if (m_pDrawCanvas->IsCapture())
	{
		return;
	}
	//want size is device unit
	FTLASSERT(pReqResize->rc.bottom > pReqResize->rc.top);
	CRect rcWantRect = pReqResize->rc;
	m_pDrawCanvas->ClientToDoc(&rcWantRect);
	int nWantHeight = rcWantRect.Height() + RTPANEL_MARGIN_TOP + RTPANEL_MARGIN_BOTTOM;

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
	CRect rcDrawTarget = m_pDrawCanvas->GetDrawTarget();
	rcDrawTarget.OffsetRect(-rcDrawTarget.TopLeft());

	rcPosition.IntersectRect(rcPosition, rcDrawTarget);
	if (rcPosition != m_position && bWillSetBound)
	{
		m_position = rcPosition;
		m_pRichEditPanel->SetClientBound(m_position, NULL, TRUE);
		m_pDrawCanvas->InvalObject(this);
	}
}

void CTextObject::_OnTextSelectChangeNotify(SELCHANGE* pSelChange)
{
	DRAWOBJBASEINFO stDrawObjectInfo;
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
		stDrawObjectInfo.strFontName = _T("");
	}
	if (szFontName[0] == _T('\0'))
	{
		stDrawObjectInfo.strFontName = _T("");
	}
	else
	{
		stDrawObjectInfo.strFontName = szFontName;
	}
	
	COM_VERIFY_EXCEPT1(m_pRichEditPanel->GetTextFontSize(nStart, nEnd, &nFontSize), S_FALSE)
	if (S_FALSE == hr)
	{
		//multi select , nFontSize is -1
		stDrawObjectInfo.nFontSize = -1;
	}		
	if (nFontSize == NULL)
	{
		stDrawObjectInfo.nFontSize = -1;
	}
	else
	{
		stDrawObjectInfo.nFontSize = nFontSize;
	}
	COLORREF clrText = RGB(0, 0, 0);
	COM_VERIFY_EXCEPT1(m_pRichEditPanel->GetTextForeColor(nStart, nEnd, &clrText), S_FALSE);
	if (S_FALSE == hr)
	{
		stDrawObjectInfo.clrFontFore = clrText;
	}
	stDrawObjectInfo.dwDrawMask = DRAWOBJECT_BASE_FONTCLR | 
		DRAWOBJECT_BASE_FONTSIZE | DRAWOBJECT_BASE_FONTNAME;
	stDrawObjectInfo.clrFontFore = clrText;
	m_pDrawCanvas->NotifyDrawObjectBaseInfo(stDrawObjectInfo);
	//m_pDrawCanvas->SetDrawObjectBaseInfo(stDrawObjectInfo, FALSE);
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
	case EN_KILLFOCUS:
		{
			HRESULT hr = E_FAIL;
			CComPtr<ITextRange> spRange;
			COM_VERIFY(m_pRichEditPanel->GetTextRange(0, -1, spRange));
			long nLength = 0;
			COM_VERIFY(spRange->GetStoryLength(&nLength));
			_OnKillFocus(nLength);
			break;
		}
	case EN_SETFOCUS:
		{

			break;
		}
	default:
		break;
	}
	//FTLTRACE(TEXT("In CTextObject::OnNotify, iNotify=%d, pParam=0x%x\n"), iNotify, pParam);
}

void CTextObject::_OnKillFocus(long nCount)
{
	if (nCount > 0)
	{
		m_bAvailObject = TRUE;
		m_pDrawCanvas->BackupDrawObjectData(_T("Edit KillFocus"));
	}
	else
	{
		m_bAvailObject = FALSE;
	}
}

void CTextObject::_OnSetFocus()
{

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

BOOL CTextObject::CheckAvailObject()
{
	return m_bAvailObject;
}

BOOL CTextObject::UpdateDrawInfo(const DRAWOBJBASEINFO& stDrawObjInfo)
{
	if (m_pRichEditPanel)
	{
		if (stDrawObjInfo.dwDrawMask & DRAWOBJECT_BASE_FONTNAME)
		{
			m_pRichEditPanel->SetTextFontName(0, 0, stDrawObjInfo.strFontName);
		}

		if (stDrawObjInfo.dwDrawMask & DRAWOBJECT_BASE_FONTCLR)
		{
			m_pRichEditPanel->SetTextForeColor(0, 0, stDrawObjInfo.clrFontFore);
		}

		if (stDrawObjInfo.dwDrawMask & DRAWOBJECT_BASE_FONTSIZE)
		{
			m_pRichEditPanel->SetTextFontSize(0, 0, stDrawObjInfo.nFontSize);
		}
		return TRUE;
	}
	return FALSE;
}