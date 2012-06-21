#include "stdafx.h"
#include "RectTool.h"
#include "DrawCanvas.h"
#include "DrawRect.h"
//#include "DrawRect.h"
#include "SelectTool.h"
#include "TextObject.h"

// CRectTool (does rectangles, round-rectangles, and ellipses)

CRectTool::CRectTool(LPDRAWOBJBASEINFO pInfo, ToolType nToolType, LPCTSTR strName)
: CDrawTool(pInfo, nToolType, strName)
{
}

BOOL CRectTool::OnLButtonDown(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	//FTLASSERT(FALSE);
	if (CDrawTool::OnLButtonDown(pView, nFlags, point))
	{
		return TRUE;
	}

	CPoint ptLogical = point;
	pView->ClientToDoc(&ptLogical);

	//DrawObjectType objType = dotNone;
	CDrawObject* pObj = NULL;
	switch (m_nToolType)
	{
		case ttRect:
			//objType = dotRect;
			pObj = new CDrawRect(pView, CRect(ptLogical, CSize(40, 40)), dotRect, *m_pDrawObjInfo);
			break;

		case ttRoundRect:
			//objType = dotRoundRect;
			pObj = new CDrawRect(pView, CRect(ptLogical, CSize(40, 40)), dotRoundRect, *m_pDrawObjInfo);
			break;

		case ttEllipse:
			//objType = dotEllipse;
			pObj = new CDrawRect(pView, CRect(ptLogical, CSize(40, 40)), dotEllipse, *m_pDrawObjInfo);
			break;

		case ttArrow:
			//objType = dotArrow;
			pObj = new CDrawArrow(pView, CRect(ptLogical, CSize(40, 40)), dotArrow, *m_pDrawObjInfo);
			break;

		case ttLine:
			//objType = dotLine;
			pObj = new CDrawRect(pView, CRect(ptLogical, CSize(0, 0)), dotLine, *m_pDrawObjInfo);
			break;
		//case ttText:
		//	pObj = new CTextObject(pView, CRect(ptLogical, CSize(0, 0)), dotText, *m_pDrawObjInfo);
		//	break;
		case ttLineArrow:
			pObj = new CDrawRect(pView, CRect(ptLogical, CSize(0, 0)), dotLineArrow, *m_pDrawObjInfo);
			break;
		case ttBalloon:
			pObj = new CDrawBalloon(pView, CRect(ptLogical, CSize(40, 40)), dotBalloon, *m_pDrawObjInfo);
			break;
		case ttImage:
			pObj = new CDrawImage(pView, CRect(ptLogical, CSize(20, 20)), dotImage, *m_pDrawObjInfo);
			break;
		default:
			FTLASSERT(FALSE); // unsupported shape!
			break;    
	}
	//CDrawRect* pObj = new CDrawRect(pView, CRect(ptLogical, CSize(0, 0)), objType);

	pView->Add(pObj);
	pView->Select(pObj);

	pView->SetCurrentSelectMode(smSize);
	pView->SetDragHandle(5);
	//g_nDragHandle = 5;
	//g_lastPoint = local;
	pView->SetMouseLastLogicalPoint(ptLogical);
	return TRUE;
}

BOOL CRectTool::OnLButtonDblClk(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	return CDrawTool::OnLButtonDblClk(pView, nFlags, point);
}

BOOL CRectTool::OnLButtonUp(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	//FTLASSERT(FALSE);
	//CPoint ptDownDevice = pView->GetMouseDownLogicalPoint();
	//pView->DocToClient(&ptDownDevice);

	//if (point == ptDownDevice && pView->GetSelection().size() > 0)
	//{
	//	// Don't create empty objects...
	//	CDrawObject *pObj = pView->GetSelection().back();
	//	pView->Remove(pObj);
	//	pObj->Remove();
	//	pView->GetSelectTool()->OnLButtonDown(pView, nFlags, point); // try a select!
	//}
	//pView->GetSelectTool()->OnLButtonUp(pView, nFlags, point);
	//BOOL bRet = CDrawTool::OnLButtonUp(pView, nFlags, point);
	//if (!pView->GetSelection().empty())
	//{
	//	CDrawObject* pDrawObject = pView->GetSelection().front();
	//	pView->SetActive(pDrawObject, TRUE);
	//	//pDrawObject->SetActive(TRUE);
	//}
	
	return CDrawTool::OnLButtonUp(pView, nFlags, point);
}

void CRectTool::OnMouseMove(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	//pView->GetSelectTool()->OnMouseMove(pView, nFlags, point);
	//s_SelectTool.OnMouseMove(pView, nFlags, point);
	return CDrawTool::OnMouseMove(pView, nFlags, point);
}
