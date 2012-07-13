#include "StdAfx.h"
#include "MoveTool.h"

#include <SilverlightCpp.h>
using namespace SilverlightCpp;

CMoveTool::CMoveTool(LPDRAWOBJBASEINFO pDrawObjInfo) : CDrawTool(pDrawObjInfo, ttMove, _T("Move"))
{
	m_hCursor = NULL;
	m_bDrag   = FALSE;

	m_hCursorMove = NULL;
	m_hCursorMoving = NULL;
}

CMoveTool::~CMoveTool(void)
{
}

BOOL CMoveTool::OnLButtonUp(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	//m_hCursor = m_hCursorMove;
	if (m_bDrag)
	{
		m_bDrag = FALSE;
		pView->SetCurrentSelectMode(smNone);
		m_hCursor = m_hCursorMoving;
		SetCursor(m_hCursor);
		return FALSE;
	}
	else 
	{
		return CDrawTool::OnLButtonUp(pView, nFlags, point);
	}

}

BOOL CMoveTool::OnLButtonDown(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	if (CDrawTool::OnLButtonDown(pView, nFlags, point))
	{
		return TRUE;
	}
	m_ptBegin = point;
	m_bDrag   = TRUE;
	m_hCursor = m_hCursorMoving;
	SetCursor(m_hCursor);
	return TRUE;
}


void CMoveTool::OnMouseMove(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	FUNCTION_BLOCK_TRACE(0);
	if (m_bDrag && (nFlags & MK_LBUTTON))
	{
		m_hCursor = m_hCursorMoving;
		CRect rcClient;
		CPoint ptOffset(0, 0);
		GetClientRect(pView->GetHWnd(), &rcClient);

		pView->GetCurrentOffsetPoint(&ptOffset);
	//	if (rcClient.PtInRect(point))
	//	{
			//CPoint ptOffset1 = pView->GetOffset();
			//ATLTRACE(_T("ptOffset11.x %d, ptOffset11.y %d   "), ptOffset1.x, ptOffset1.y);
			ptOffset.x = max(0, ptOffset.x - point.x + m_ptBegin.x);
			ptOffset.y = max(0, ptOffset.y - point.y + m_ptBegin.y);

			pView->SetCurrentOffsetPoint(&ptOffset);
			m_ptBegin = point;
	//	}
	}
	else
	{
		m_hCursor = m_hCursorMove;
		CDrawTool::OnMouseMove(pView, nFlags, point);
	}
}

void CMoveTool::InitResource()
{
	NDGraphics::CGDIPImage imgCursorMove;
	imgCursorMove.Load( SilverlightCpp::ZipManager::get_Current()->LoadCImage(
		_T( "/Assets/Images/Main/CaptureView/cursor_move.png" ) ), ImageFormatPNG ); // NS
	m_hCursorMove = (HCURSOR)imgCursorMove.GetHICON();

	NDGraphics::CGDIPImage imgCursorMoving;
	imgCursorMoving.Load( SilverlightCpp::ZipManager::get_Current()->LoadCImage(
		_T( "/Assets/Images/Main/CaptureView/cursor_moving.png" ) ), ImageFormatPNG ); // NS
	m_hCursorMoving = (HCURSOR)imgCursorMoving.GetHICON();

	m_hCursor = m_hCursorMove;
}