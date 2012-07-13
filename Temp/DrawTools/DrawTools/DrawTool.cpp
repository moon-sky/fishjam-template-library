#include "stdafx.h"
#include "DrawTool.h"
#include "DrawCanvas.h"
#include "TextObject.h"

CDrawTool::CDrawTool(LPDRAWOBJBASEINFO pDrawObjInfo, ToolType nToolType, LPCTSTR strName)
{
	m_nToolType = nToolType;
	m_hCursor = NULL;
	m_strToolName = strName;
	m_pDrawObjInfo = pDrawObjInfo;
}

BOOL CDrawTool::OnSetCursor(HWND hWnd, UINT nHitTest, UINT message)
{
	if (NULL != m_hCursor)
	{
		::SetCursor(m_hCursor);
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawTool::OnLButtonDown(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	BOOL bRet = FALSE;
	pView->BeginCapture();
	CPoint ptLogical = point;
	pView->ClientToDoc(&ptLogical);

	CDrawObject* pObj = NULL;
	pView->SetCurrentSelectMode(smNone);

	// Check for resizing (only allowed on single selections)
	if (pView->GetSelection().size() == 1)
	{
		CDrawObject* pOldActiveObject = pView->ObjectAt(ptLogical);
		BOOL bSelectChanage = FALSE;
		pObj = pView->GetSelection().front();
		if (pObj != pOldActiveObject)
		{
			//if is not the same, then clear old active
			pView->SetActive(NULL, FALSE);
			bSelectChanage = TRUE;
		}

		int nDragHandle = pObj->HitTest(point, TRUE);
		pView->SetDragHandle(nDragHandle);
		if (nDragHandle != 0)
		{
			pView->SetCurrentSelectMode(smSize);
			bRet = TRUE;
			bSelectChanage = FALSE;
		}
		if(bSelectChanage)
		{
			pView->Select(NULL); // add for temp
		}
	}

	// See if the click was on an object, select and start move if so
	if (pView->GetCurrentSelectMode() == smNone)
	{
		pObj = pView->ObjectAt(ptLogical);

		if (pObj != NULL && pView->IsSelected(pObj))
		{
			if (pObj->HitTestMove(point))
			{
				pView->SetActive(pObj, FALSE);
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
				bRet = TRUE;
			}
			else if (pObj->HitTestActive(point))
			{
				pView->SetActive(pObj, TRUE);
				pView->SetCurrentSelectMode(smNone);
				bRet = TRUE;
			}
		}
	}
	pView->SetMouseDownLogicalPoint(ptLogical);
	pView->SetMouseLastLogicalPoint(ptLogical);
	return bRet;
}

BOOL CDrawTool::OnLButtonDblClk(IDrawCanvas* /*pView*/, UINT /*nFlags*/, const CPoint& /*point*/)
{
	return FALSE;
}

BOOL CDrawTool::OnLButtonUp(IDrawCanvas* pView, UINT /*nFlags*/, const CPoint& point)
{
	if(pView->IsCapture())
	{
		CPoint ptDevice = pView->GetMouseDownLogicalPoint();
		pView->DocToClient(&ptDevice);
		BOOL bBackupData = TRUE;
		if (point == ptDevice && pView->GetCurrentSelectMode() == smSize)
		{
			CDrawObject* pObj = pView->GetSelection().front();
			if (pObj->GetDrawObjType() == dotSelectRect || 
				(!pObj->CheckAvailObject() && pObj->GetDrawObjType() != dotText && pObj->IsActive())
				&& ((pObj->GetDrawObjType() == dotLineArrow || pObj->GetDrawObjType() == dotLine) && !pObj->CheckAvailObject()))
			{
				pView->Remove(pObj, TRUE);
				pObj->Remove();
				bBackupData = FALSE;
			}
		}
		
		for(DrawObjectList::iterator iter = pView->GetSelection().begin();
			iter != pView->GetSelection().end();
			++iter)
		{
			CDrawObject* pObj = *iter;
			if (smSize == pView->GetCurrentSelectMode())
			{
				pObj->EndMoveHandle();
			}

			if (pObj->GetDrawObjType() == dotText && pObj->IsActive())
			{
				bBackupData = FALSE;
			}
		}
		pView->EndCapture(bBackupData);
	}
	return FALSE;
}

void CDrawTool::OnMouseMove(IDrawCanvas* pView, UINT /*nFlags*/, const CPoint& point)
{
	CPoint ptLogical = point;
	pView->ClientToDoc(&ptLogical);
	if (!pView->IsCapture())
	{
		CDrawObject* pObj = pView->ObjectAt(ptLogical);
		if (pObj != NULL && pView->IsSelected(pObj))
		{
			int nHandle = pObj->HitTest(point, TRUE);
			if (nHandle != 0)
			{
				m_hCursor = pObj->GetHandleCursor(nHandle);
				return; // bypass CDrawTool
			}
			if (pObj->HitTestActive(point))
			{
				m_hCursor = pObj->GetActiveCursor();
			}
			else if (pObj->HitTestMove(point))
			{
				m_hCursor = ::LoadCursor(NULL, IDC_SIZEALL);
			}
		}
	}
	else
	{
		_ScrollForSelect(pView, point);
		//CPoint ptLogical = point;
		//pView->ClientToDoc(&ptLogical);

		CPoint ptLastLogical = pView->GetMouseLastLogicalPoint();
		CPoint delta = (CPoint)(ptLogical - ptLastLogical);

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
		}
		pView->SetMouseLastLogicalPoint(ptLogical);

		if (pView->GetCurrentSelectMode() == smMove)
		{
			m_hCursor = ::LoadCursor(NULL, IDC_SIZEALL);
		}
		if (pView->GetCurrentSelectMode() == smSize && pView->GetCurrentToolType() == ttSelection
			&& !pView->GetSelection().empty())
		{
			m_hCursor = pView->GetSelection().front()->GetHandleCursor(pView->GetDragHandle());
			return; // bypass CDrawTool
		}
		//m_hCursor = ::LoadCursor(NULL, IDC_ARROW);
		if (pView->GetCurrentToolType() == ttSelection)
		{
			pView->SetMouseLastLogicalPoint(ptLogical);
		}
	}
}

BOOL CDrawTool::HandleControlMessage(IDrawCanvas* pView, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
{
	BOOL bRet = FALSE;
	if (!pView->GetSelection().empty())
	{
		CDrawObject* pActiveObject = pView->GetSelection().front();
		if (pActiveObject && pActiveObject->GetDrawObjType() == dotText)
		{
			CTextObject* pActiveTexObject = dynamic_cast<CTextObject*>(pActiveObject);
			if (pActiveTexObject)
			{
				bRet = pActiveTexObject->HandleControlMessage(pView, uMsg, wParam, lParam, lResult);
			}
		}
	}
	return bRet;
}

void CDrawTool::InitResource()
{

}

void CDrawTool::OnEditProperties(IDrawCanvas* /*pView*/)
{
}

void CDrawTool::OnCancel(IDrawCanvas* pView)
{
	//pView->SetCurrentToolType(ttSelection);
}

BOOL CDrawTool::IsNeedClip()
{
	return TRUE;
}

void CDrawTool::_CheckSelectPostion(IDrawCanvas* pView)
{
	if (pView->GetSelection().size() == 1)
	{
		CDrawObject* pObj = pView->GetSelection().front();
		if (dotSelectRect == pObj->GetDrawObjType())
		{
			CRect rcSelectPos = pObj->GetPosition();
			CRect rcOldSelectPos = rcSelectPos;
			rcSelectPos.NormalizeRect();

			CRect rcDrawTarget = pView->GetDrawTarget();
			rcDrawTarget.OffsetRect(-rcDrawTarget.TopLeft());

			rcSelectPos.IntersectRect(rcSelectPos, rcDrawTarget);
			if (rcSelectPos.IsRectEmpty())
			{
				pView->SetCurrentToolType(ttSelection);
				//pView->ReleaseSelectRect();
			}
			else if(rcOldSelectPos != rcSelectPos)
			{
				pObj->SetPosition(rcSelectPos, FALSE);
				pView->InvalObject(pObj);
			}
		}
	}
}

void CDrawTool::_ScrollForSelect(IDrawCanvas* pView, const CPoint& point)
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