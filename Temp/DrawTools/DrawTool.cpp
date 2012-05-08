#include "stdafx.h"
#include "DrawTool.h"
#include "DrawCanvas.h"

CDrawTool::CDrawTool(ToolType nToolType)
{
	m_nToolType = nToolType;
	m_hCursor = NULL;
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

void CDrawTool::OnLButtonDown(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	pView->BeginCapture();
	CPoint ptLogical = point;
	pView->ClientToDoc(&ptLogical);
	pView->SetMouseDownLogicalPoint(ptLogical);
	pView->SetMouseLastLogicalPoint(ptLogical);
}

void CDrawTool::OnLButtonDblClk(IDrawCanvas* /*pView*/, UINT /*nFlags*/, const CPoint& /*point*/)
{
}

void CDrawTool::OnLButtonUp(IDrawCanvas* pView, UINT /*nFlags*/, const CPoint& point)
{
	pView->EndCapture();
	CPoint ptDownDevice = pView->GetMouseDownLogicalPoint();
	pView->DocToClient(&ptDownDevice);
	if (point == ptDownDevice)
	{
		SelectMode curSelectMode = pView->GetCurrentSelectMode();
		if (smSize != curSelectMode && smMove != curSelectMode)
		{
			pView->SetCurrentToolType(ttSelection);
			pView->ReleaseSelectRect();
		}
	}
	else
	{
		_CheckSelectPostion(pView);
	}
}

void CDrawTool::OnMouseMove(IDrawCanvas* pView, UINT /*nFlags*/, const CPoint& point)
{
	CPoint ptLogical = point;
	pView->ClientToDoc(&ptLogical);
	pView->SetMouseLastLogicalPoint(ptLogical);
}

void CDrawTool::OnEditProperties(IDrawCanvas* /*pView*/)
{
}

void CDrawTool::OnCancel(IDrawCanvas* pView)
{
	//pView->SetCurrentToolType(ttSelection);
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
				pView->ReleaseSelectRect();
			}
			else if(rcOldSelectPos != rcSelectPos)
			{
				pObj->SetPosition(rcSelectPos);
				pView->InvalObject(pObj);
			}
		}
	}
}