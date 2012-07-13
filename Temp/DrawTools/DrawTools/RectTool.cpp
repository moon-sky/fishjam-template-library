#include "stdafx.h"
#include "RectTool.h"
#include "DrawCanvas.h"
#include "DrawRect.h"
//#include "DrawRect.h"
#include "SelectTool.h"
#include "TextObject.h"


#include <SilverlightCpp.h>
using namespace SilverlightCpp;

// CRectTool (does rectangles, round-rectangles, and ellipses)

CRectTool::CRectTool(LPDRAWOBJBASEINFO pInfo, ToolType nToolType, LPCTSTR strName)
: CDrawTool(pInfo, nToolType, strName)
{
	m_hDrawCursor = NULL;
	m_hStamplistCursor = NULL;
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
	CRect rcRectObject(0, 0, 0, 0);
	rcRectObject.left = ptLogical.x;
	rcRectObject.top  = ptLogical.y;
	//DrawObjectType objType = dotNone;
	CDrawObject* pObj = NULL;
	switch (m_nToolType)
	{
		case ttRect:
			//objType = dotRect;
			{
				rcRectObject.right = ptLogical.x + 1;	//40
				rcRectObject.bottom = ptLogical.y + 1;	//40
				pObj = new CDrawRect(pView, rcRectObject, dotRect, *m_pDrawObjInfo);
				break;
			}
		case ttRoundRect:
			//objType = dotRoundRect;
			{
				rcRectObject.right = ptLogical.x + 1;	//40
				rcRectObject.bottom = ptLogical.y + 1;	//40
				pObj = new CDrawRect(pView, rcRectObject, dotRoundRect, *m_pDrawObjInfo);
				break;
			}
		case ttEllipse:
			{	
				//objType = dotEllipse;
				rcRectObject.right = ptLogical.x + 1;	//40
				rcRectObject.bottom = ptLogical.y + 1;	//40
				pObj = new CDrawRect(pView, rcRectObject, dotEllipse, *m_pDrawObjInfo);
				break;
			}
		case ttArrow:
			//objType = dotArrow;
			{
				rcRectObject.right = ptLogical.x + 1;	//40
				rcRectObject.bottom = ptLogical.y + 1;	//40
				pObj = new CDrawArrow(pView, rcRectObject, dotArrow, *m_pDrawObjInfo);
				break;
			}

		case ttLine:
			{
				//objType = dotLine;
				pObj = new CDrawRect(pView, CRect(ptLogical, CSize(0, 0)), dotLine, *m_pDrawObjInfo);
				break;
			}
		case ttLineArrow:
			{
				pObj = new CDrawRect(pView, CRect(ptLogical, CSize(0, 0)), dotLineArrow, *m_pDrawObjInfo);
				break;
			}
		case ttBalloon:
			{
				rcRectObject.right = ptLogical.x + 1;	//40
				rcRectObject.bottom = ptLogical.y + 1;	//40
				pObj = new CDrawBalloon(pView, rcRectObject, dotBalloon, *m_pDrawObjInfo);
				break;
			}
		case ttImage:
			{
				rcRectObject.right = ptLogical.x + 1;	//30
				rcRectObject.bottom = ptLogical.y + 1;	//30
				pObj = new CDrawImage(pView, rcRectObject, dotImage, *m_pDrawObjInfo);
				break;
			}

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

	switch (m_nToolType)
	{
		case ttRect:
		case ttRoundRect:
		case ttEllipse:
		case ttArrow:
		case ttLine:
		case ttLineArrow:
		case ttBalloon:
			{
				m_hCursor = m_hDrawCursor;
				break;
			}
		case ttImage:
			{
				m_hCursor = m_hStamplistCursor;
				break;
			}
		default:
			FTLASSERT(FALSE); // unsupported shape!
			break;    
	}
	return CDrawTool::OnMouseMove(pView, nFlags, point);
}

void CRectTool::InitResource()
{
	NDGraphics::CGDIPImage imgCursor1;
	imgCursor1.Load( SilverlightCpp::ZipManager::get_Current()->LoadCImage(
		_T( "/Assets/Images/Main/CaptureView/draw_image.png" ) ), ImageFormatPNG ); // NS
	m_hDrawCursor = (HCURSOR)imgCursor1.GetHICON();

	NDGraphics::CGDIPImage imgCursor2;
	imgCursor2.Load( SilverlightCpp::ZipManager::get_Current()->LoadCImage(
		_T( "/Assets/Images/Main/CaptureView/cursor_stamp.png" ) ), ImageFormatPNG ); // NS
	m_hStamplistCursor = (HCURSOR)imgCursor2.GetHICON();
}