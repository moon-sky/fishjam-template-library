#include "stdafx.h"
#include "TextTool.h"
#include "DrawCanvas.h"
#include "DrawRect.h"
//#include "DrawRect.h"
#include "SelectTool.h"
#include "TextObject.h"

// CTextTool (does rectangles, round-rectangles, and ellipses)

CTextTool::CTextTool(LPDRAWOBJBASEINFO pDrawObjInfo, LPCTSTR strName)
: CDrawTool(pDrawObjInfo, ttText, strName)
{
}

BOOL CTextTool::OnLButtonDown(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	//return s_SelectTool.OnLButtonDown(pView, nFlags, point);
	if (CDrawTool::OnLButtonDown(pView, nFlags, point))
	{
		return TRUE;
	}
	
	CPoint ptLogical = point;
	pView->ClientToDoc(&ptLogical);
	CRect rcPosition = CRect(ptLogical, CSize(1,1));

	CTextObject* pObj = new CTextObject(pView, rcPosition, dotText, *m_pDrawObjInfo);
	pView->Add(pObj);
	pView->Select(pObj);


	pView->SetCurrentSelectMode(smSize);
	pView->SetDragHandle(5);
	pView->SetMouseLastLogicalPoint(ptLogical);
	return TRUE;
}

BOOL CTextTool::OnLButtonDblClk(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	return CDrawTool::OnLButtonDblClk(pView, nFlags, point);
}

BOOL CTextTool::OnLButtonUp(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	//BOOL bRet = s_SelectTool.OnLButtonUp(pView, nFlags, point);
	//
	//CPoint ptLogical = point;
	//pView->ClientToDoc(&ptLogical);

	//CDrawObject* pObj = NULL;
	//switch (m_nToolType)
	//{
	//case ttText:
	//	pObj = new CTextObject(pView, CRect(ptLogical, pView->GetMouseDownLogicalPoint() ), dotText, *m_pDrawObjInfo);
	//	break;
	//default:
	//	FTLASSERT(FALSE); // unsupported shape!
	//	break;    
	//}
	////CDrawRect* pObj = new CDrawRect(pView, CRect(ptLogical, CSize(0, 0)), objType);

	//pView->Add(pObj);
	//pView->Select(pObj);
	//pView->SetActive(pObj, TRUE);

	//pView->SetCurrentSelectMode(smSize);
	//pView->SetDragHandle(5);
	//g_nDragHandle = 5;
	//g_lastPoint = local;
	//pView->SetMouseLastLogicalPoint(ptLogical);

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
	
	//if (!pView->GetSelection().empty())
	//{
	//	CDrawObject* pDrawObject = pView->GetSelection().front();
	//	pView->SetActive(pDrawObject, TRUE);
	//	//pDrawObject->SetActive(TRUE);
	//}
	//return bRet;

	DrawObjectList SelectList = pView->GetSelection();
	if (SelectList.size() == 1)
	{
		CDrawObject* pObject = *SelectList.begin();
		if (pObject != NULL && pObject->GetDrawObjType() == dotText)
		{
			CTextObject* pText = (CTextObject*)pObject;
			pText->CheckTextRect();
			pView->SetActive(pObject, TRUE);
		}
	}
	return CDrawTool::OnLButtonUp(pView, nFlags, point);
}

void CTextTool::OnMouseMove(IDrawCanvas* pView, UINT nFlags, const CPoint& point)
{
	//pView->GetSelectTool()->OnMouseMove(pView, nFlags, point);
	//s_SelectTool.OnMouseMove(pView, nFlags, point);
	return CDrawTool::OnMouseMove(pView, nFlags, point);
}
