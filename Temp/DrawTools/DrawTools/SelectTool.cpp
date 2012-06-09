#include "stdafx.h"
#include "SelectTool.h"
#include "DrawCanvas.h"
#include "DrawRect.h"

#include "SelectObject.h"

#include "../resource.h"
#ifndef DRAW_TOOL_TEST
#include <SilverlightCpp.h>
using namespace SilverlightCpp;
#endif 

//#include <SilverlightExCpp.h>
//using namespace SilverlightExCpp;

CSelectTool::CSelectTool(LPDRAWOBJBASEINFO pInfo)
: CDrawTool(pInfo, ttSelection, _T("Selection"))
{
	//TODO: IDrawCanvas is better ?
	

}

BOOL CSelectTool::OnLButtonDown(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	//CPoint ptLogical = point;
	//pView->ClientToDoc(&ptLogical);

	//CDrawObject* pObj = NULL;
	//pView->SetCurrentSelectMode(smNone);

	//// Check for resizing (only allowed on single selections)
	//if (pView->GetSelection().size() == 1)
	//{
	//	pObj = pView->GetSelection().front();
	//	int nDragHandle = pObj->HitTest(point, TRUE);
	//	pView->SetDragHandle(nDragHandle);
	//	if (nDragHandle != 0)
	//	{
	//		pView->SetCurrentSelectMode(smSize);
	//	}
	//		//g_selectMode = size;
	//}

	//// See if the click was on an object, select and start move if so
	//if (pView->GetCurrentSelectMode() == smNone)
	//{
	//	pObj = pView->ObjectAt(ptLogical);

	//	if (pObj != NULL)
	//	{
	//		//g_selectMode = move;
	//		pView->SetCurrentSelectMode(smMove);
	//		pView->SetActive(pObj, TRUE);
	//		if (!pView->IsSelected(pObj))
	//		{
	//			pView->Select(pObj, (nFlags & MK_SHIFT) != 0);
	//		}
	//		// Ctrl+Click clones the selection...
	//		if ((nFlags & MK_CONTROL) != 0)
	//		{
	//			pView->CloneSelection();
	//		}
	//	}
	//	else
	//	{
	//		CRect rcSelect;
	//		rcSelect.SetRect(ptLogical.x, ptLogical.y, ptLogical.x + 1, ptLogical.y + 1);
	//		
	//		CSelectObject* pSelectObject = new CSelectObject(pView, rcSelect, dotSelectRect, *m_pDrawObjInfo);
	//		//CDrawRect* pSelectObject = new CDrawRect(pView, rcSelect, dotSelectRect);
	//		pView->Add(pSelectObject);
	//		pView->SetDragHandle(5);
	//		pView->SetCurrentSelectMode(smSize);
	//		pView->Select(pSelectObject);
	//	}
	//}
	//return CDrawTool::OnLButtonDown(pView, nFlags, point);

	if (CDrawTool::OnLButtonDown(pView, nFlags, point))
	{
		return TRUE;
	}
	CPoint ptLogical = point;
	pView->ClientToDoc(&ptLogical);
	CRect rcSelect;
	rcSelect.SetRect(ptLogical.x, ptLogical.y, ptLogical.x + 1, ptLogical.y + 1);
	
	CSelectObject* pSelectObject = new CSelectObject(pView, rcSelect, dotSelectRect, *m_pDrawObjInfo);
	//CDrawRect* pSelectObject = new CDrawRect(pView, rcSelect, dotSelectRect);
	//pView->RemoveUnavialObect();
	pView->Add(pSelectObject);
	pView->SetDragHandle(5);
	pView->SetCurrentSelectMode(smSize);
	pView->Select(pSelectObject);
	return TRUE;
}

BOOL CSelectTool::OnLButtonDblClk(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
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

	return CDrawTool::OnLButtonDblClk(pView, nFlags, point);
}

void CSelectTool::OnEditProperties(IDrawCanvas* pView)
{
	if (pView->GetSelection().size() == 1)
	{
		pView->GetSelection().front()->OnEditProperties();
	}
}

void CSelectTool::InitResource()
{
#ifdef DRAW_TOOL_TEST
	m_hCursorSelect = LoadCursor(NULL, IDC_SIZEALL);
	m_hCursorSelecting = LoadCursor(NULL, IDC_HAND);
	m_hCursor = m_hCursor;
#else
	NDGraphics::CGDIPImage imgCursorSelect;
	imgCursorSelect.Load( SilverlightCpp::ZipManager::get_Current()->LoadCImage(
		_T( "/Assets/Images/Main/CaptureView/select_cursor_1.png" ) ), ImageFormatPNG ); // NS
	m_hCursorSelect = (HCURSOR)imgCursorSelect.GetHICON();
	FTLASSERT(m_hCursorSelect);


	NDGraphics::CGDIPImage imgCursorSelecting;
	imgCursorSelecting.Load( SilverlightCpp::ZipManager::get_Current()->LoadCImage(
		_T( "/Assets/Images/Main/CaptureView/select_cursor_2.png" ) ), ImageFormatPNG ); // NS
	m_hCursorSelecting = (HCURSOR)imgCursorSelecting.GetHICON();
	FTLASSERT(m_hCursorSelecting);
	m_hCursor = m_hCursorSelect;
#endif 

}

BOOL CSelectTool::OnLButtonUp(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	if(pView->IsCapture())
	{
		//if (pView->GetCurrentSelectMode() == smNetSelect)
		//{
		//	CPoint ptLogical = point;
		//	pView->ClientToDoc(&ptLogical);
		//	//FTLTRACE(TEXT("OnLButtonUp, ptDevice=[%d,%d], ptLogical=[%d,%d]\n"), 
		//	//	point.x, point.y,
		//	//	ptLogical.x, ptLogical.y);

		//	CPoint ptOriginalDevice(0, 0);
		//	pView->DocToClient(&ptOriginalDevice);

		//	//CPoint ptOffset = pView->GetOffset();
		//	//pView->DocToClient(&ptOffset);

		//	CRect rectBefore(pView->GetMouseDownLogicalPoint(), pView->GetMouseLastLogicalPoint());
		//	pView->DocToClient(&rectBefore);
		//	//rectBefore.OffsetRect(-ptOriginalDevice);
		//	//rectBefore.OffsetRect(ptOffset);
		//	rectBefore.NormalizeRect();

		//	CRect rectAfter(pView->GetMouseDownLogicalPoint(), ptLogical);
		//	pView->DocToClient(&rectAfter);
		//	//rectAfter.OffsetRect(-ptOriginalDevice);
		//	//rectAfter.OffsetRect(ptOffset);
		//	rectAfter.NormalizeRect();

		//	CRect rcSelectBox;
		//	rcSelectBox.SetRect(pView->GetMouseDownLogicalPoint(), ptLogical);
		//	ptLogical.Offset(pView->GetOffset());
		//	rcSelectBox.NormalizeRect();

		//	//CPoint ptOriginalLogical(0,0);
		//	//pView->DocToClient(&ptOriginalLogical);
		//	//rcSelectBox.OffsetRect(ptOriginalLogical);

		//	//pView->CreateSelectRect(rcSelectBox);
		//	pView->SelectWithinRect(rcSelectBox, TRUE);

		//	//CClientDC dc(pView->GetHWnd());
		//	////dc.OffsetViewportOrg(-ptOriginal.x, -ptOriginal.y);
		//	//dc.DrawFocusRect(rectBefore);
		//	//dc.DrawFocusRect(rectAfter);
		//}
		//else 
		//if (pView->GetCurrentSelectMode() == smNetSelectSize)
		//{
		//	CDrawObject *pObj = pView->GetSelectRect();
		//	if (pObj)
		//	{
		//		pView->Select(pObj);
		//	}
		//}
		//else
		//{
		//	FTLTRACE(TEXT("TODO: pView->GetCurrentSelectMode()=%d\n"), pView->GetCurrentSelectMode());
			//::InvalidateRect(pView->GetHWnd(), NULL, TRUE);
			//::UpdateWindow(pView->GetHWnd());
		//}
		//else if (m_selectMode != none)
		//{
		//    pView->UpdateAllViews(pView);
		//}

		CPoint ptDevice = pView->GetMouseDownLogicalPoint();
		pView->DocToClient(&ptDevice);
		if (point == ptDevice && pView->GetCurrentSelectMode() == smSize)
		{
			CDrawObject* pObj = pView->GetSelection().front();
			pView->Remove(pObj, TRUE);
			pObj->Remove();
		}
		m_hCursor = m_hCursorSelect;// ::LoadCursor(NULL, IDC_ARROW);
		for(DrawObjectList::iterator iter = pView->GetSelection().begin();
			iter != pView->GetSelection().end();
			++iter)
		{
			CDrawObject* pObj = *iter;
			if (smSize == pView->GetCurrentSelectMode())
			{
				pObj->EndMoveHandle();
			}
		}
		pView->EndCapture(FALSE);
		//CDrawTool::OnLButtonUp(pView, nFlags, point);
	}
	return FALSE;
}

//void CSelectTool::_OnNotCapturedMouseMove(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
//{
//	CPoint ptLogical = point;
//	pView->ClientToDoc(&ptLogical);
//
//	if (pView->GetCurrentToolType() == ttSelection && pView->GetSelection().size() == 1)
//	{
//		CDrawObject* pObj = pView->GetSelection().front();
//
//		int nHandle = pObj->HitTest(point, TRUE);
//		if (nHandle != 0)
//		{
//			m_hCursor = pObj->GetHandleCursor(nHandle);
//			return; // bypass CDrawTool
//		}
//
//		if (pObj->GetPosition().PtInRect(ptLogical))
//		{
//			m_hCursor = ::LoadCursor(NULL, IDC_SIZEALL);
//		}
//		////if (pView->GetCurrentToolType() == ttSelection)
//		//{
//		//	//SetCursor(::LoadCursor(NULL, IDC_CROSS));
//		//	CDrawTool::OnMouseMove(pView, nFlags, point);
//		//	return;
//		//}
//	}
//}

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

//void CSelectTool::_OnCapturedMouseMove(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
//{
//	_ScrollForSelect(pView, point);
//	CPoint ptLogical = point;
//	pView->ClientToDoc(&ptLogical);
//
//	CPoint ptLastLogical = pView->GetMouseLastLogicalPoint();
//	CPoint delta = (CPoint)(ptLogical - ptLastLogical);
//
//	for(DrawObjectList::iterator iter = pView->GetSelection().begin();
//		iter != pView->GetSelection().end();
//		++iter)
//	{
//		CDrawObject* pObj = *iter;
//		CRect position = pObj->GetPosition();
//
//		if (smMove == pView->GetCurrentSelectMode())
//		{
//			position += delta;
//			pObj->MoveTo(position);
//		}
//		else if (pView->GetDragHandle()!= 0)
//		{
//			pObj->MoveHandleTo(pView->GetDragHandle(), ptLogical);
//		}
//	}
//	pView->SetMouseLastLogicalPoint(ptLogical);
//
//	if (pView->GetCurrentSelectMode() == smMove)
//	{
//		m_hCursor = ::LoadCursor(NULL, IDC_SIZEALL);
//	}
//	if (pView->GetCurrentSelectMode() == smSize && pView->GetCurrentToolType() == ttSelection)
//	{
//		m_hCursor = pView->GetSelection().front()->GetHandleCursor(pView->GetDragHandle());
//		return; // bypass CDrawTool
//	}
//
//	if (pView->GetCurrentToolType() == ttSelection)
//	{
//		CDrawTool::OnMouseMove(pView, nFlags, point);
//	}
//}

void CSelectTool::OnMouseMove(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	//CPoint ptLogical = point;
	//pView->ClientToDoc(&ptLogical);

	//if (!pView->IsCapture())
	//{
	//	//ATLTRACE(_T("No Capture \n"));
	//	_OnNotCapturedMouseMove(pView, nFlags, point);
	//}
	//else
	//{
	//	//ATLTRACE(_T("Is Capture \n"));
	//	_OnCapturedMouseMove(pView, nFlags, point);		
	//}
	CDrawTool::OnMouseMove(pView, nFlags, point);
}