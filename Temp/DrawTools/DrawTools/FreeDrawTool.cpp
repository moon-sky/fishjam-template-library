#include "StdAfx.h"
#include "FreeDrawTool.h"
#include "DrawCanvas.h"


CFreeDrawTool::CFreeDrawTool(LPDRAWOBJBASEINFO pInfo, ToolType nToolType, LPCTSTR strName)
: CDrawTool(pInfo, nToolType, strName)
{
	m_pCurObject = NULL;
}

BOOL CFreeDrawTool::OnLButtonDown(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	CDrawTool::OnLButtonDown(pView, nFlags, point);

	CPoint local = point;
	pView->ClientToDoc(&local);

	if (m_pCurObject == NULL)
	{
		pView->BeginCapture();
		m_pCurObject = new CDrawFreeObject(pView, CRect(local, CSize(0, 0)), dotFreeObject, *m_pDrawObjInfo);
		pView->Add(m_pCurObject);
		pView->Select(m_pCurObject);
		m_pCurObject->AddPoint(local);
		pView->InvalObject(m_pCurObject);
	}
	else if (local == m_pCurObject->GetHandle(1))
	{
		// Stop when the first point is repeated...
		pView->EndCapture();
		if (m_pCurObject->GetHandleCount() < 2)
		{
			pView->Remove(m_pCurObject, TRUE);
			m_pCurObject->Remove();
		}
		m_pCurObject = NULL;
		pView->SetCurrentSelectMode(smNone);
		return TRUE;
	}

	local.x += 1; // adjacent points can't be the same!
	m_pCurObject->AddPoint(local);
	pView->SetCurrentSelectMode(smSize);
	pView->SetDragHandle(m_pCurObject->GetHandleCount());
	pView->SetMouseDownLogicalPoint(local);
	return TRUE;
}

BOOL CFreeDrawTool::OnLButtonUp(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	if (m_pCurObject)
	{
		pView->EndCapture();
		int nPoints = m_pCurObject->GetHandleCount();

		CPoint ptPoint1 = m_pCurObject->GetHandle(nPoints);
		CPoint ptPoint2 = m_pCurObject->GetHandle(nPoints - 1);
		if (nPoints > 2 &&
			(ptPoint1 == ptPoint2 ||
			m_pCurObject->GetHandle(nPoints).x - 1 == m_pCurObject->GetHandle(nPoints - 1).x &&
			m_pCurObject->GetHandle(nPoints).y == m_pCurObject->GetHandle(nPoints - 1).y))

		{
			// Nuke the last point if it's the same as the next to last...
			pView->InvalObject(m_pCurObject);
		}

		m_pCurObject = NULL;
		return TRUE;
	}
	return CDrawTool::OnLButtonUp(pView, nFlags, point);
}

void CFreeDrawTool::OnMouseMove(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	if (m_pCurObject != NULL && (nFlags & MK_LBUTTON) != 0)
	{
		CPoint local = point;
		pView->ClientToDoc(&local);
		m_pCurObject->AddPoint(local);
		pView->SetDragHandle(m_pCurObject->GetHandleCount());
		pView->SetMouseLastLogicalPoint(local);
		pView->InvalObject(m_pCurObject);
	}
}