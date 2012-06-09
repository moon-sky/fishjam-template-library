#include "StdAfx.h"
#include "MoveTool.h"

CMoveTool::CMoveTool(LPDRAWOBJBASEINFO pDrawObjInfo) : CDrawTool(pDrawObjInfo, ttMove, _T("Move"))
{
	m_hCursor = NULL;
}

CMoveTool::~CMoveTool(void)
{
}

BOOL CMoveTool::OnLButtonUp(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	return FALSE;
}

BOOL CMoveTool::OnLButtonDown(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	m_ptBegin = point;
	return CDrawTool::OnLButtonDown(pView, nFlags, point);
}


void CMoveTool::OnMouseMove(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	if (nFlags & MK_LBUTTON)
	{
		CRect rcClient;
		CPoint ptOffset(0, 0);
		GetClientRect(pView->GetHWnd(), &rcClient);

		pView->GetCurrentOffsetPoint(&ptOffset);
		if (rcClient.PtInRect(point))
		{
			//CPoint ptOffset1 = pView->GetOffset();
			//ATLTRACE(_T("ptOffset11.x %d, ptOffset11.y %d   "), ptOffset1.x, ptOffset1.y);
			ptOffset.x = max(0, ptOffset.x - point.x + m_ptBegin.x);
			ptOffset.y = max(0, ptOffset.y - point.y + m_ptBegin.y);

			pView->SetCurrentOffsetPoint(&ptOffset);
			m_ptBegin = point;
		}
		//m_hCursor = ::LoadCursor(NULL, IDC_HAND);
		//CDrawTool::OnMouseMove(pView, nFlags, point);
	}
}