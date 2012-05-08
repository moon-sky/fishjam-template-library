#include "stdafx.h"
#include "SelectTool.h"
#include "DrawCanvas.h"
#include "DrawRect.h"
#include "../resource.h"

#include <SilverlightCpp.h>
using namespace SilverlightCpp;
//#include <SilverlightExCpp.h>
//using namespace SilverlightExCpp;

CSelectTool::CSelectTool()
: CDrawTool(ttSelection)
{
	//TODO: IDrawCanvas is better ?
	
	NDGraphics::CGDIPImage imgCursorSelect;
	imgCursorSelect.Load( SilverlightCpp::ZipManager::get_Current()->LoadCImage(
		_T( "/Assets/Images/Main/CaptureView/select_cursor_1.png" ) ), ImageFormatPNG ); // NS
	m_hCursorSelect = (HCURSOR)imgCursorSelect.GetHICON();
	FTLASSERT(m_hCursorSelect);
	//POINT ptPhysicalCur = { 0 };
	//BOOL bRet = FALSE;
	//API_VERIFY(GetPhysicalCursorPos(&ptPhysicalCur));
	//API_VERIFY(SetPhysicalCursorPos(ptPhysicalCur.x + 2, ptPhysicalCur.y + 2));

	NDGraphics::CGDIPImage imgCursorSelecting;
	imgCursorSelecting.Load( SilverlightCpp::ZipManager::get_Current()->LoadCImage(
		_T( "/Assets/Images/Main/CaptureView/select_cursor_2.png" ) ), ImageFormatPNG ); // NS
	m_hCursorSelecting = (HCURSOR)imgCursorSelecting.GetHICON();
	FTLASSERT(m_hCursorSelecting);
	m_hCursor = m_hCursorSelect;
}

void CSelectTool::OnLButtonDown(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	CPoint ptLogical = point;
	pView->ClientToDoc(&ptLogical);

	CDrawObject* pObj = NULL;
	pView->SetCurrentSelectMode(smNone);

	// Check for resizing (only allowed on single selections)
	if (pView->GetSelection().size() == 1)
	{
		pObj = pView->GetSelection().front();
		int nDragHandle = pObj->HitTest(point, TRUE);
		pView->SetDragHandle(nDragHandle);
		if (nDragHandle != 0)
		{
			pView->SetCurrentSelectMode(smSize);
		}
			//g_selectMode = size;
	}

	// See if the click was on an object, select and start move if so
	if (pView->GetCurrentSelectMode() == smNone)
	{
		pObj = pView->ObjectAt(ptLogical);

		if (pObj != NULL)
		{
			//g_selectMode = move;
			pView->SetCurrentSelectMode(smMove);

			if (!pView->IsSelected(pObj))
			{
				pView->Select(pObj, (nFlags & MK_SHIFT) != 0);
			}

			// Ctrl+Click clones the selection...
			if ((nFlags & MK_CONTROL) != 0)
			{
				pView->CloneSelection();
			}
		}
		else
		{
			pView->ReleaseSelectRect();
			CRect rcSelect;
			rcSelect.SetRect(ptLogical.x, ptLogical.y, ptLogical.x + 1, ptLogical.y + 1);
			pView->CreateSelectRect(rcSelect, FALSE);
			pView->SetDragHandle(5);
			pView->SetCurrentSelectMode(smNetSelectSize);
		}
	}

	// Click on background, start a net-selection
	if (pView->GetCurrentSelectMode() == smNone)
	{
		m_hCursor = m_hCursorSelecting; // m_hCursorSelect;
		SetCursor(m_hCursor);

		if (pView->GetSelectRect())
		{
			pView->ReleaseSelectRect();
		}

		if ((nFlags & MK_SHIFT) == 0)
		{
			pView->Select(NULL);
		}

		pView->SetCurrentSelectMode(smNetSelect);
		
		//CPoint ptOriginalDevice(0,0);
		//pView->DocToClient(&ptOriginalDevice);

		//CPoint ptOffset = pView->GetOffset();
		//pView->DocToClient(&ptOffset);

		CRect rect(point, point);
		//CRect rect(ptLogical,ptLogical);
		//pView->DocToClient(&rect);
		//rect.OffsetRect(-ptOriginalDevice);
		//rect.OffsetRect(ptOffset);

		//rect.NormalizeRect();
		//CClientDC dc(pView->GetHWnd());
		//dc.DrawFocusRect(rect);
	}
	CDrawTool::OnLButtonDown(pView, nFlags, point);
}

void CSelectTool::OnLButtonDblClk(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	if ((nFlags & MK_SHIFT) != 0)
	{
		// Shift+DblClk deselects object...
		CPoint local = point;
		pView->ClientToDoc(&local);
		CDrawObject* pObj = pView->ObjectAt(local);
		if (NULL != pObj)
		{
			pView->Deselect(pObj);
		}
	}
	else
	{
		// "Normal" DblClk opens properties, or OLE server...
		if (pView->GetSelection().size() == 1)
		{
			pView->GetSelection().front()->OnOpen();
		}
	}

	CDrawTool::OnLButtonDblClk(pView, nFlags, point);
}

void CSelectTool::OnEditProperties(IDrawCanvas* pView)
{
	if (pView->GetSelection().size() == 1)
	{
		pView->GetSelection().front()->OnEditProperties();
	}
}

void CSelectTool::OnCancel(IDrawCanvas* pView)
{
	//if (pView->IsCapture())
	{
		SelectMode curSelectMode = pView->GetCurrentSelectMode();
		if (smNetSelectSize == curSelectMode)
		{
			pView->ReleaseSelectRect();

			//CRect rcSelectBox;
			//rcSelectBox.SetRect(pView->GetMouseDownLogicalPoint(), pView->GetMouseLastLogicalPoint());
			//rcSelectBox.NormalizeRect();
			//pView->CreateSelectRect(rcSelectBox);
			//pView->SelectWithinRect(rcSelectBox, TRUE);
		}
		else if (smSize == curSelectMode && smMove == curSelectMode)
		{
			//do nothing ?
			//pView->SetCurrentToolType(ttSelection);
		}
		else if(smNetSelect == curSelectMode)
		{
			FTLASSERT(FALSE); //do not use smNetSelect now
		}
	}
}

void CSelectTool::OnLButtonUp(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	if(pView->IsCapture())
	{
		if (pView->GetCurrentSelectMode() == smNetSelect)
		{
			CPoint ptLogical = point;
			pView->ClientToDoc(&ptLogical);
			//FTLTRACE(TEXT("OnLButtonUp, ptDevice=[%d,%d], ptLogical=[%d,%d]\n"), 
			//	point.x, point.y,
			//	ptLogical.x, ptLogical.y);

			CPoint ptOriginalDevice(0, 0);
			pView->DocToClient(&ptOriginalDevice);

			//CPoint ptOffset = pView->GetOffset();
			//pView->DocToClient(&ptOffset);

			CRect rectBefore(pView->GetMouseDownLogicalPoint(), pView->GetMouseLastLogicalPoint());
			pView->DocToClient(&rectBefore);
			//rectBefore.OffsetRect(-ptOriginalDevice);
			//rectBefore.OffsetRect(ptOffset);
			rectBefore.NormalizeRect();

			CRect rectAfter(pView->GetMouseDownLogicalPoint(), ptLogical);
			pView->DocToClient(&rectAfter);
			//rectAfter.OffsetRect(-ptOriginalDevice);
			//rectAfter.OffsetRect(ptOffset);
			rectAfter.NormalizeRect();

			CRect rcSelectBox;
			rcSelectBox.SetRect(pView->GetMouseDownLogicalPoint(), ptLogical);
			ptLogical.Offset(pView->GetOffset());
			rcSelectBox.NormalizeRect();

			//CPoint ptOriginalLogical(0,0);
			//pView->DocToClient(&ptOriginalLogical);
			//rcSelectBox.OffsetRect(ptOriginalLogical);

			pView->CreateSelectRect(rcSelectBox);
			pView->SelectWithinRect(rcSelectBox, TRUE);

			//CClientDC dc(pView->GetHWnd());
			////dc.OffsetViewportOrg(-ptOriginal.x, -ptOriginal.y);
			//dc.DrawFocusRect(rectBefore);
			//dc.DrawFocusRect(rectAfter);
		}
		else if (pView->GetCurrentSelectMode() == smNetSelectSize)
		{
			CDrawObject *pObj = pView->GetSelectRect();
			if (pObj)
			{
				pView->Select(pObj);
			}
		}
		else
		{
			FTLTRACE(TEXT("TODO: pView->GetCurrentSelectMode()=%d\n"), pView->GetCurrentSelectMode());
			//::InvalidateRect(pView->GetHWnd(), NULL, TRUE);
			//::UpdateWindow(pView->GetHWnd());
		}
		//else if (m_selectMode != none)
		//{
		//    pView->UpdateAllViews(pView);
		//}

		m_hCursor = m_hCursorSelect;// ::LoadCursor(NULL, IDC_ARROW);

		CDrawTool::OnLButtonUp(pView, nFlags, point);
	}
}

void CSelectTool::_OnNotCapturedMouseMove(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	CPoint ptLogical = point;
	pView->ClientToDoc(&ptLogical);

	if (pView->GetCurrentToolType() == ttSelection && pView->GetSelection().size() == 1)
	{
		CDrawObject* pObj = pView->GetSelection().front();

		int nHandle = pObj->HitTest(point, TRUE);
		if (nHandle != 0)
		{
			m_hCursor = pObj->GetHandleCursor(nHandle);
			return; // bypass CDrawTool
		}

		if (pObj->GetPosition().PtInRect(ptLogical))
		{
			m_hCursor = ::LoadCursor(NULL, IDC_SIZEALL);
		}
		////if (pView->GetCurrentToolType() == ttSelection)
		//{
		//	//SetCursor(::LoadCursor(NULL, IDC_CROSS));
		//	CDrawTool::OnMouseMove(pView, nFlags, point);
		//	return;
		//}
	}
}

void CSelectTool::_LimitSelectPoint(IDrawCanvas* pView, CPoint& ptClient)
{
	CRect rcClient;
	GetClientRect(pView->GetHWnd(), &rcClient);

	ptClient.x = max(ptClient.x, rcClient.left);
	ptClient.x = min(ptClient.x, rcClient.right);
	ptClient.y = max(ptClient.y, rcClient.top);
	ptClient.y = min(ptClient.y, rcClient.bottom);
}

void CSelectTool::_ScrollForSelect(IDrawCanvas* pView, const CPoint& point)
{
	HWND hWndOwner = pView->GetHWnd();

	CRect rcClient;
	GetClientRect(hWndOwner, &rcClient);

	if (point.x > rcClient.right)
	{
		SendMessage(hWndOwner, WM_HSCROLL, MAKEWPARAM(SB_LINERIGHT, 1), NULL);
	}
	else if (point.x < rcClient.left)
	{
		SendMessage(hWndOwner, WM_HSCROLL, MAKEWPARAM(SB_LINELEFT, 1), NULL);
	}
	if (point.y > rcClient.bottom)
	{
		SendMessage(hWndOwner, WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 1), NULL);
	}
	else if (point.y < rcClient.top)
	{
		SendMessage(hWndOwner, WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 1), NULL);
	}
}

BOOL CSelectTool::_OffView(IDrawCanvas* pView, const CPoint& point)
{
	HWND hWndOwner = pView->GetHWnd();

	CRect rcClient;
	GetClientRect(hWndOwner, &rcClient);

	return !PtInRect(&rcClient, point);
}

void CSelectTool::_OnCapturedMouseMove(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	_ScrollForSelect(pView, point);
	CPoint ptLogical = point;
	pView->ClientToDoc(&ptLogical);

	if (pView->GetCurrentSelectMode() == smNetSelect && _OffView(pView, point))
	{
		CRect rcSelectBox;
		rcSelectBox.SetRect(pView->GetMouseDownLogicalPoint(), ptLogical);
		ptLogical.Offset(pView->GetOffset());
		rcSelectBox.NormalizeRect();

		if (rcSelectBox.IsRectEmpty())
		{
			return;
		}
		pView->CreateSelectRect(rcSelectBox, FALSE);
		pView->SelectWithinRect(rcSelectBox, TRUE);
		pView->SetCurrentSelectMode(smNetSelectSize);

		CDrawObject* pObj = pView->GetSelectRect();
		if (pObj)
		{
			int nDragHandle = pObj->SizingHitTest(ptLogical, pView->GetMouseDownLogicalPoint());
			pView->SetDragHandle(nDragHandle);
		}
	}
	
	if (pView->GetCurrentSelectMode() == smNetSelect)
	{
		CPoint ptOffset = pView->GetOffset();
		pView->DocToClient(&ptOffset);
		
		//CPoint ptOriginalDevice(0,0);
		//pView->DocToClient(&ptOriginalDevice);

		CRect rectBefore(pView->GetMouseDownLogicalPoint(), pView->GetMouseLastLogicalPoint());
		pView->DocToClient(&rectBefore);
		//rectBefore.OffsetRect(-ptOriginalDevice);
		//rectBefore.OffsetRect(ptOffset);
		rectBefore.NormalizeRect();

		CRect rectAfter(pView->GetMouseDownLogicalPoint(), ptLogical);
		pView->DocToClient(&rectAfter);
		//rectAfter.OffsetRect(-ptOriginalDevice);
		//rectAfter.OffsetRect(ptOffset);
		rectAfter.NormalizeRect();

		//CClientDC dc(pView->GetHWnd());
		//dc.DrawFocusRect(rectBefore);
		//dc.DrawFocusRect(rectAfter);
		//FTLTRACE(TEXT("_OnCapturedMouseMove, ptLogical=[%d,%d]\n"), ptLogical.x, ptLogical.y);

		//m_hCursor = m_hCursorSelect;// ::LoadCursor(NULL, IDC_CROSS);
		//::SetCursor(m_hCursor); //refresh current cursor
		CDrawTool::OnMouseMove(pView, nFlags, point);
		return;
	}

	CPoint ptLastLogical = pView->GetMouseLastLogicalPoint();
	CPoint delta = (CPoint)(ptLogical - ptLastLogical);

	if (smNetSelectSize == pView->GetCurrentSelectMode())
	{
		CDrawObject* pObj = pView->GetSelectRect();
		if (pObj)
		{
			pObj->MoveHandleTo(pView->GetDragHandle(), ptLogical);
		}
	}

	for(DrawObjectList::iterator iter = pView->GetSelection().begin();
		iter != pView->GetSelection().end();
		++iter)
	{
		CDrawObject* pObj = *iter;
		CRect position = pObj->GetPosition();

		if (smMove == pView->GetCurrentSelectMode())
		{
			position += delta;
			pObj->MoveTo(position);
		}
		else if (pView->GetDragHandle()!= 0)
		{
			pObj->MoveHandleTo(pView->GetDragHandle(), ptLogical);
		}

		//FTLTRACE(TEXT("pObj Pos=[%d,%d] X [%d,%d]\n"), pObj->m_position.left,
		//    pObj->m_position.top,
		//    pObj->m_position.right,
		//    pObj->m_position.bottom);
	}

	pView->SetMouseLastLogicalPoint(ptLogical);

	if (pView->GetCurrentSelectMode() == smMove)
	{
		m_hCursor = ::LoadCursor(NULL, IDC_SIZEALL);
	}
	if (pView->GetCurrentSelectMode() == smSize && pView->GetCurrentToolType() == ttSelection)
	{
		m_hCursor = pView->GetSelection().front()->GetHandleCursor(pView->GetDragHandle());
		return; // bypass CDrawTool
	}

	if (pView->GetCurrentToolType() == ttSelection)
	{
		CDrawTool::OnMouseMove(pView, nFlags, point);
	}
}

void CSelectTool::OnMouseMove(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	CPoint ptLogical = point;
	pView->ClientToDoc(&ptLogical);

	//init cursor for default value
	CRect rcDrawTarget = pView->GetDrawTarget();
	rcDrawTarget.OffsetRect(-rcDrawTarget.TopLeft());

	if (rcDrawTarget.PtInRect(ptLogical))
	{
		m_hCursor = m_hCursorSelect;
	}
	else
	{
		m_hCursor = ::LoadCursor(NULL, IDC_ARROW);
	}

	if (!pView->IsCapture())
	{
		_OnNotCapturedMouseMove(pView, nFlags, point);
	}
	else
	{
		_OnCapturedMouseMove(pView, nFlags, point);		
	}
}