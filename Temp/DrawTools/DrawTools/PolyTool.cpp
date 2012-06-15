#include "StdAfx.h"
#include "PolyTool.h"
#include "SelectTool.h"
#include "DrawCanvas.h"
#include "DrawTool.h"

CPolyTool::CPolyTool(LPDRAWOBJBASEINFO pInfo, ToolType nToolType, LPCTSTR strName) : CDrawTool(pInfo, nToolType, strName)
{
	m_pCurPolyObject = NULL;
}

BOOL CPolyTool::OnLButtonDown(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	if(CDrawTool::OnLButtonDown(pView, nFlags, point))
	{
		if (m_pCurPolyObject == NULL)
		{
			return TRUE;
		}
	}

	CPoint local = point;
	pView->ClientToDoc(&local);

	if (m_pCurPolyObject == NULL)
	{
		pView->BeginCapture();
		m_pCurPolyObject = new CDrawPoly(pView, CRect(local, CSize(0, 0)), dotPoly, *m_pDrawObjInfo);
		pView->Add(m_pCurPolyObject);
		pView->Select(m_pCurPolyObject);
		m_pCurPolyObject->AddPoint(local);
		pView->InvalObject(m_pCurPolyObject);
	}
	else if (local == m_pCurPolyObject->GetHandle(1))
	{
		// Stop when the first point is repeated...
		pView->EndCapture();
		m_pCurPolyObject->RemoveHandle();
		if (m_pCurPolyObject->GetHandleCount() < 2)
		{
			pView->Remove(m_pCurPolyObject, TRUE);
			m_pCurPolyObject->Remove();
		}
		m_pCurPolyObject = NULL;
		pView->SetCurrentSelectMode(smNone);
		return TRUE;
	}

	local.x += 1; // adjacent points can't be the same!
	m_pCurPolyObject->AddPoint(local);
	pView->SetCurrentSelectMode(smSize);
	pView->SetDragHandle(m_pCurPolyObject->GetHandleCount());
	pView->SetMouseDownLogicalPoint(local);
	return TRUE;
}

BOOL CPolyTool::OnLButtonDblClk(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	if (m_pCurPolyObject)
	{
		pView->EndCapture();
		int nPoints = m_pCurPolyObject->GetHandleCount();

		CPoint ptPoint1 = m_pCurPolyObject->GetHandle(nPoints);
		CPoint ptPoint2 = m_pCurPolyObject->GetHandle(nPoints - 1);
		if (nPoints > 2 &&
			(ptPoint1 == ptPoint2 ||
			m_pCurPolyObject->GetHandle(nPoints).x - 1 == m_pCurPolyObject->GetHandle(nPoints - 1).x &&
			m_pCurPolyObject->GetHandle(nPoints).y == m_pCurPolyObject->GetHandle(nPoints - 1).y))

		{
			// Nuke the last point if it's the same as the next to last...
			m_pCurPolyObject->RemoveHandle();
			pView->InvalObject(m_pCurPolyObject);
		}

		m_pCurPolyObject = NULL;
		return TRUE;
	}
	return FALSE;
	//c_drawShape = selection;
}

BOOL CPolyTool::OnLButtonUp(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	if(pView->IsCapture() && m_pCurPolyObject == NULL)
	{
		CPoint ptDevice = pView->GetMouseDownLogicalPoint();
		pView->DocToClient(&ptDevice);
		if (point == ptDevice && pView->GetCurrentSelectMode() == smSize)
		{
			CDrawObject* pObj = pView->GetSelection().front();
			pView->Remove(pObj, TRUE);
			pObj->Remove();
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
		}
		return CDrawTool::OnLButtonUp(pView, nFlags, point);
	}
	else
	{
		return TRUE;
	}
}

void CPolyTool::OnMouseMove(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	if (m_pCurPolyObject != NULL && (nFlags & MK_LBUTTON) != 0)
	{
		OutputDebugString(_T("CPolyTool::OnMouseMove\r\n"));
		CPoint local = point;
		pView->ClientToDoc(&local);
		m_pCurPolyObject->AddPoint(local, TRUE);
		pView->SetDragHandle(m_pCurPolyObject->GetHandleCount());
		pView->SetMouseLastLogicalPoint(local);
		pView->InvalObject(m_pCurPolyObject);
	}
	else
	{
		CDrawTool::OnMouseMove(pView, nFlags, point);
	}
}

void CPolyTool::OnEditProperties(IDrawCanvas* pView)
{

}

void CPolyTool::OnCancel(IDrawCanvas* pView)
{
	CDrawTool::OnCancel(pView);

	m_pCurPolyObject = NULL;
}