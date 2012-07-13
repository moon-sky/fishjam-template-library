#include "stdafx.h"
#include "TextObject.h"
#include "DrawCanvas.h"
#include <ftlGdi.h>
//#include <ftlControls.h>

#include "..\..\..\Capture\StringResouceMgr.h"

CTextObject::CTextObject(IDrawCanvas* pDrawCanvas, const CRect& position, DrawObjectType objType, 
						 DRAWOBJBASEINFO& stDrawObjInfo, CShareEditPtr pEditPanel)
: CDrawObject(pDrawCanvas, position, objType, stDrawObjInfo)
{
	HRESULT hr = E_FAIL;

	m_pShareEditPtr = pEditPanel;

	m_bAvailObject = FALSE;
	//m_bIsCanUndo = TRUE;
	//m_bIsCanRedo = FALSE;
}

CTextObject::CTextObject(IDrawCanvas* pDrawCanvas, const CRect& position, DrawObjectType objType,
			DRAWOBJBASEINFO& stDrawObjInfo)
: CDrawObject(pDrawCanvas, position, objType, stDrawObjInfo), m_pShareEditPtr(new CRichEditPanel())
{
	HRESULT hr = E_FAIL;

	//m_pRichEditPanel = new CRichEditPanel();

	if (stDrawObjInfo.strFontName == _T(""))
	{
		stDrawObjInfo.strFontName = CStringResouceMgr::Instance()->GetEditFontName().c_str();
	}
	if (stDrawObjInfo.nFontSize == -1)
	{
		stDrawObjInfo.nFontSize = 18;
	}

	LOGFONT stFont = {0};
	stFont.lfHeight = -stDrawObjInfo.nFontSize;
	StringCchCopy(stFont.lfFaceName, _countof(stFont.lfFaceName), stDrawObjInfo.strFontName);

	m_pShareEditPtr->Init(pDrawCanvas->GetHWnd(), &position, pDrawCanvas, &stFont, 
		stDrawObjInfo.clrFontFore, this);
	_InitDrawInfo(stDrawObjInfo);
	m_pShareEditPtr->BeginEdit();
	m_pShareEditPtr->EndEdit();
	m_pShareEditPtr->InitEmptyUndoInfo();
	m_bAvailObject = FALSE;
	//m_bIsCanUndo = TRUE;
	//m_bIsCanRedo = FALSE;
}


CTextObject::~CTextObject()
{

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
	m_pShareEditPtr->DoPaint(hDC);
	//CDCHandle dc(hDC);
	//dc.DrawText(TEXT("In CTextObject Draw"), -1, m_position, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

BOOL CTextObject::HandleControlMessage(IDrawCanvas* pView, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
{
	if (m_pShareEditPtr && m_pShareEditPtr.get())
	{
		return m_pShareEditPtr->HandleControlMessage(pView, uMsg, wParam, lParam, lResult);
	}
	return FALSE;
}

void CTextObject::SetPosition(const CRect& pos, BOOL bCheckSize)
{
	CRect rect = pos;
	if (abs(rect.Width()) < 100)
	{
		return;
	}
	if (abs(rect.Height()) < 30)
	{
		return;
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

	m_pShareEditPtr->SetClientBound(&m_position, NULL, TRUE); //FALSE);
	m_pDrawCanvas->InvalObject(this);
}

CDrawObject* CTextObject::Clone()
{
	DRAWOBJBASEINFO stDrawInfo(_T(""));
	stDrawInfo.logbrush = m_logbrush;
	stDrawInfo.logpen   = m_logpen;
	stDrawInfo.bBrush   = m_bBrush;
	stDrawInfo.bPen     = m_bPen;

	//TCHAR lfFaceName[LF_FACESIZE] = {0};
	//m_pRichEditPanel->GetTextFontName(0, 0, lfFaceName, LF_FACESIZE);
	//stDrawInfo.strFontName = lfFaceName;
	//m_pRichEditPanel->GetTextFontSize(0, 0, &stDrawInfo.nFontSize);
	//m_pRichEditPanel->GetTextForeColor(0, 0, stDrawInfo.clrFontFore);

	CTextObject* pClone = new CTextObject(m_pDrawCanvas, m_position, m_objType, stDrawInfo, m_pShareEditPtr);
	

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
	if (m_pShareEditPtr && m_pShareEditPtr.get())
	{
		m_pShareEditPtr->SetActive(bActive);
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
	SetPosition(position, FALSE);
	m_pShareEditPtr->SetClientBound(&m_position, NULL, TRUE);
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

	CSize szMin = m_pShareEditPtr->GetMinBoundSize(RTPANEL_MIN_ROW_COUNT, RTPANEL_MIN_COL_COUNT);
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
		m_pShareEditPtr->SetClientBound(m_position, NULL, TRUE);
		m_pDrawCanvas->InvalObject(this);
	}
}

void CTextObject::_OnTextSelectChangeNotify(SELCHANGE* pSelChange)
{
	DRAWOBJBASEINFO stDrawObjectInfo(CStringResouceMgr::Instance()->GetEditFontName().c_str());
	stDrawObjectInfo.dwDrawMask = 0;
	const int _cchStyleName = 64;
	TCHAR szFontName[_cchStyleName] = {0};
	int nFontSize = 0;

	HRESULT hr = E_FAIL;
	long nStart = pSelChange->chrg.cpMin;
	long nEnd = pSelChange->chrg.cpMax;

	COM_VERIFY_EXCEPT1(m_pShareEditPtr->GetTextFontName(nStart, nEnd,
		szFontName, _countof(szFontName)), S_FALSE);
	if (S_FALSE == hr)
	{
		stDrawObjectInfo.strFontName = _T("");
		stDrawObjectInfo.dwDrawMask |= DRAWOBJECT_BASE_FONTNAME;
	}
	else
	{
		stDrawObjectInfo.dwDrawMask |= DRAWOBJECT_BASE_FONTNAME;
	}
	if (szFontName[0] == _T('\0'))
	{
		stDrawObjectInfo.strFontName = _T("");
	}
	else
	{
		stDrawObjectInfo.strFontName = szFontName;
	}
	
	COM_VERIFY_EXCEPT1(m_pShareEditPtr->GetTextFontSize(nStart, nEnd, &nFontSize), S_FALSE)
	if (S_FALSE == hr)
	{
		//multi select , nFontSize is -1
		stDrawObjectInfo.nFontSize = -1;
		stDrawObjectInfo.dwDrawMask |= DRAWOBJECT_BASE_FONTSIZE;
	}
	else
	{
		stDrawObjectInfo.dwDrawMask |= DRAWOBJECT_BASE_FONTSIZE;
		stDrawObjectInfo.nFontSize = nFontSize;
	}

	COLORREF clrText = RGB(0, 0, 0);
	COM_VERIFY_EXCEPT1(m_pShareEditPtr->GetTextForeColor(nStart, nEnd, &clrText), S_FALSE);
	if (S_FALSE == hr)
	{
		stDrawObjectInfo.clrFontFore = clrText;

	}
	else
	{
		stDrawObjectInfo.clrFontFore = clrText;
		stDrawObjectInfo.dwDrawMask |= DRAWOBJECT_BASE_FONTCLR;
	}
	
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
			COM_VERIFY(m_pShareEditPtr->GetTextRange(0, -1, spRange));
			long nLength = 0;
			COM_VERIFY(spRange->GetStoryLength(&nLength));
			_OnKillFocus(nLength);
			m_pShareEditPtr->ClearUndoRedoInfo(TRUE, TRUE);
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
	m_pShareEditPtr->SetClientBound(&m_position, NULL, TRUE);
}

BOOL CTextObject::CheckAvailObject()
{
	return m_bAvailObject;
}

BOOL CTextObject::Undo()
{
	if (m_pShareEditPtr.get() && m_pShareEditPtr->Undo())
	{
		//m_bIsCanRedo = TRUE;
		return TRUE;
	}
	//m_bIsCanUndo = FALSE;
	return FALSE;
}

BOOL CTextObject::Redo()
{
	if (m_pShareEditPtr.get() && m_pShareEditPtr->Redo())
	{
		//m_bIsCanUndo = TRUE;
		return TRUE;
	}
	//m_bIsCanRedo = FALSE;
	return FALSE;
}

BOOL CTextObject::IsCanUndo()
{
	if (m_pShareEditPtr)
	{
		return m_pShareEditPtr->CanUndo();
	}
	return FALSE;
}
BOOL CTextObject::IsCanRedo()
{
	if (m_pShareEditPtr)
	{
		return m_pShareEditPtr->CanRedo();
	}
	return FALSE;
	//return m_bIsCanRedo;
}

BOOL CTextObject::BeginEdit()
{
	if (m_pShareEditPtr.get())
	{
		if(m_pShareEditPtr->BeginEdit() == S_OK)
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CTextObject::EndEdit(BOOL bIsPushUndo)
{
	if (m_pShareEditPtr.get())
	{
		if(m_pShareEditPtr->EndEdit(bIsPushUndo) == S_OK)
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CTextObject::UpdateDrawInfo(const DRAWOBJBASEINFO& stDrawObjInfo)
{
	if (m_pShareEditPtr.get())
	{
		if (stDrawObjInfo.dwDrawMask & DRAWOBJECT_BASE_FONTNAME)
		{
			m_pShareEditPtr->SetTextFontName(0, 0, stDrawObjInfo.strFontName);
		}

		if (stDrawObjInfo.dwDrawMask & DRAWOBJECT_BASE_FONTCLR)
		{
			m_pShareEditPtr->SetTextForeColor(0, 0, stDrawObjInfo.clrFontFore);
		}

		if (stDrawObjInfo.dwDrawMask & DRAWOBJECT_BASE_FONTSIZE)
		{
			m_pShareEditPtr->SetTextFontSize(0, 0, stDrawObjInfo.nFontSize);
		}
		return TRUE;
	}
	return FALSE;
}

void CTextObject::_InitDrawInfo(const DRAWOBJBASEINFO& stDrawObjInfo)
{
	if (m_pShareEditPtr.get())
	{
		if (stDrawObjInfo.dwDrawMask & DRAWOBJECT_BASE_FONTNAME)
		{
			m_pShareEditPtr->SetTextFontName(0, -1, stDrawObjInfo.strFontName);
		}

		if (stDrawObjInfo.dwDrawMask & DRAWOBJECT_BASE_FONTCLR)
		{
			m_pShareEditPtr->SetTextForeColor(0, -1, stDrawObjInfo.clrFontFore);
		}

		if (stDrawObjInfo.dwDrawMask & DRAWOBJECT_BASE_FONTSIZE)
		{
			m_pShareEditPtr->SetTextFontSize(0, -1, stDrawObjInfo.nFontSize);
		}
	}
}