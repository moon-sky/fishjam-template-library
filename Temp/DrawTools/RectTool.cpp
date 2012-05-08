#include "stdafx.h"
#include "RectTool.h"
#include "DrawCanvas.h"
#include "DrawRect.h"
#include "TextObject.h"
//#include "DrawRect.h"
//#include "SelectTool.h"

// CRectTool (does rectangles, round-rectangles, and ellipses)

CRectTool::CRectTool(ToolType nToolType)
: CDrawTool(nToolType)
{
}

void CRectTool::OnLButtonDown(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	CDrawTool::OnLButtonDown(pView, nFlags, point);

	CPoint ptLogical = point;
	pView->ClientToDoc(&ptLogical);

	DrawObjectType objType = dotNone;
	switch (m_nToolType)
	{
		//case selectRect:
		//    nShape = CDrawRect::selectRect;
		//    break;
		case ttRect:
			objType = dotRect;
			break;

		case ttRoundRect:
			objType = dotRoundRect;
			break;

		case ttEllipse:
			objType = dotEllipse;
			break;

		case ttLine:
			objType = dotLine;
			break;
		case ttText:
			objType = dotText;
			break;
		default:
			FTLASSERT(FALSE); // unsupported shape!
			break;    
	}
	CDrawObject* pObj = NULL;
	if (objType != dotText)
	{
		pObj = new CDrawRect(pView, CRect(ptLogical, CSize(0, 0)), objType);
	}
	else
	{
		pObj = new CTextObject(pView, CRect(ptLogical, CSize(100,100)), objType);
	}

	pView->Add(pObj);
	pView->Select(pObj);

	pView->SetCurrentSelectMode(smSize);
	pView->SetDragHandle(5);
	//g_nDragHandle = 5;
	//g_lastPoint = local;
	pView->SetMouseLastLogicalPoint(ptLogical);
}

void CRectTool::OnLButtonDblClk(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	CDrawTool::OnLButtonDblClk(pView, nFlags, point);
}

void CRectTool::OnLButtonUp(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	CPoint ptDownDevice = pView->GetMouseDownLogicalPoint();
	pView->DocToClient(&ptDownDevice);

	if (point == ptDownDevice)
	{
		// Don't create empty objects...
		CDrawObject *pObj = pView->GetSelection().back();
		pView->Remove(pObj);
		pObj->Remove();
		pView->GetSelectTool()->OnLButtonDown(pView, nFlags, point); // try a select!
	}
	pView->GetSelectTool()->OnLButtonUp(pView, nFlags, point);
}

void CRectTool::OnMouseMove(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	pView->GetSelectTool()->OnMouseMove(pView, nFlags, point);
}
