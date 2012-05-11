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
		pObj = new CDrawRect(pView, CRect(ptLogical, CSize(1, 1)), objType);
	}
	else
	{
		pObj = new CTextObject(pView, CRect(ptLogical, CSize(1, 1)), objType);
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
		pView->InvalObject(pObj);
		pView->Remove(pObj);
		pObj->Remove();
		pView->GetSelectTool()->OnLButtonDown(pView, nFlags, point); // try a select!
	}
	pView->GetSelectTool()->OnLButtonUp(pView, nFlags, point);

	if (!pView->GetSelection().empty())
	{
		CDrawObject* pDrawObject = pView->GetSelection().front();
		pDrawObject->SetActive(TRUE);

		//if (pDrawObject->GetDrawObjType() == dotText)
		//{
		//	CTextObject* pTextObject = dynamic_cast<CTextObject*>(pDrawObject);
		//	//CComPtr<ITextServices> spTextService;
		//	
		//	
		//	//if (spTextService)
		//	//{ GetTextServices(&spTextService);
		//	//	spTextService->OnTxInPlaceActivate(pDrawObject->GetPosition());
		//	//	spTextService->TxSendMessage(WM_SETFOCUS, 0, 0, NULL);
		//	//}
		//}
	}
}

void CRectTool::OnMouseMove(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	pView->GetSelectTool()->OnMouseMove(pView, nFlags, point);
}
