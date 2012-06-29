#include "StdAfx.h"
#include "FreeDrawTool.h"
#include "DrawCanvas.h"

#include <SilverlightCpp.h>
using namespace SilverlightCpp;


CFreeDrawTool::CFreeDrawTool(LPDRAWOBJBASEINFO pInfo, ToolType nToolType, LPCTSTR strName)
: CDrawTool(pInfo, nToolType, strName)
{
	m_hFreeDrawCur = NULL;
}

BOOL CFreeDrawTool::OnLButtonDown(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	CDrawTool::OnLButtonDown(pView, nFlags, point);

	CPoint local = point;
	pView->ClientToDoc(&local);

	pView->BeginCapture();
	CDrawFreeObject *pObject = new CDrawFreeObject(pView, CRect(local, CSize(0, 0)), dotFreeObject, *m_pDrawObjInfo);
	pView->Add(pObject);
	pView->Select(pObject);
	pObject->AddPoint(local);
	pView->InvalObject(pObject);

	local.x += 1; // adjacent points can't be the same!
	pObject->AddPoint(local);
	pView->SetCurrentSelectMode(smSize);
	pView->SetDragHandle(pObject->GetHandleCount());
	pView->SetMouseDownLogicalPoint(local);
	return TRUE;
}

BOOL CFreeDrawTool::OnLButtonUp(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	if (pView->IsCapture())
	{
		pView->EndCapture();
		if (pView->GetCurrentSelectMode() == smSize && !pView->GetSelection().empty())
		{
			CDrawFreeObject* pObject = (CDrawFreeObject*)pView->GetSelection().front();
			int nPoints = pObject->GetHandleCount();

			CPoint ptPoint1 = pObject->GetHandle(nPoints);
			CPoint ptPoint2 = pObject->GetHandle(nPoints - 1);
			if (nPoints > 2 &&
				(ptPoint1 == ptPoint2 ||
				pObject->GetHandle(nPoints).x - 1 == pObject->GetHandle(nPoints - 1).x &&
				pObject->GetHandle(nPoints).y == pObject->GetHandle(nPoints - 1).y))
			{
				// Nuke the last point if it's the same as the next to last...
				pView->InvalObject(pObject);
			}
			return TRUE;
		}
	}
	return CDrawTool::OnLButtonUp(pView, nFlags, point);
}

void CFreeDrawTool::OnMouseMove(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	//if (m_pCurObject != NULL && (nFlags & MK_LBUTTON) != 0)
	//{
	//	CPoint local = point;
	//	pView->ClientToDoc(&local);
	//	m_pCurObject->AddPoint(local);
	//	pView->SetDragHandle(m_pCurObject->GetHandleCount());
	//	pView->SetMouseLastLogicalPoint(local);
	//	pView->InvalObject(m_pCurObject);
	//}
	if (pView->IsCapture())
	{
		CPoint local = point;
		pView->ClientToDoc(&local);
		if (pView->GetCurrentSelectMode() == smSize && !pView->GetSelection().empty())
		{
			CDrawFreeObject *pObject = (CDrawFreeObject*)pView->GetSelection().front();
			pObject->AddPoint(local);
			pView->SetDragHandle(pObject->GetHandleCount());
			pView->SetMouseLastLogicalPoint(local);
			pView->InvalObject(pObject);
		}
	}
}

void CFreeDrawTool::InitResource()
{
	NDGraphics::CGDIPImage imgCursor;
	imgCursor.Load( SilverlightCpp::ZipManager::get_Current()->LoadCImage(
		_T( "/Assets/Images/Main/CaptureView/draw_curve.png" ) ), ImageFormatPNG ); // NS
	m_hFreeDrawCur = (HCURSOR)imgCursor.GetHICON();
	m_hCursor = m_hFreeDrawCur;
}