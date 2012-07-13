#include "stdafx.h"
#include "DrawRect.h"
#include "DrawCanvas.h"
#include <ftlGdi.h>
#include <gdiplus.h>

#include <SilverlightCpp.h>
using namespace SilverlightCpp;
#include <SilverlightExCpp.h>
using namespace SilverlightExCpp;

CDrawRect::CDrawRect(IDrawCanvas* pDrawCanvas, const CRect& position, 
					 DrawObjectType objType, const DRAWOBJBASEINFO& stDrawObjInfo)
: CDrawObject(pDrawCanvas, position, objType, stDrawObjInfo)
{
	//if (dotSelectRect == m_objType)
	//{
	//	m_bBrush = FALSE;
	//	m_logpen.lopnStyle = PS_DOT;
	//}
	m_roundness.x = 16;
	m_roundness.y = 16;

	m_stArrowInfo.fTheta = 0.5f;
}

CSize CDrawRect::GetMinSize()
{
	switch (m_objType)
	{
	case dotRect:
	case dotRoundRect:
	case dotEllipse:
	case dotArrow:
	case dotBalloon:
		return CSize(40, 40);
	case dotLine:
	case dotLineArrow:
		return CSize(0, 0);
	case dotImage:
		return CSize(30, 30);
	default:
		ATLASSERT(FALSE);
		break;
	}
	return CDrawObject::GetMinSize();
}

//void CDrawRect::ArrowTo(HDC hDC, const CPoint& ptFrom,const CPoint& ptTo, LPARROWINFO pA) 
//{
//	POINT pFrom;
//	POINT pBase;
//	POINT aptPoly[3];
//	float vecLine[2];
//	float vecLeft[2];
//	float fLength;
//	float th;
//	float ta;
//
//	// get from point
//	MoveToEx(hDC, 0, 0, &pFrom);
//
//	// set to point
//	aptPoly[0].x = ptTo.x;
//	aptPoly[0].y = ptTo.y;
//
//	// build the line vector
//	vecLine[0] = (float) aptPoly[0].x - pFrom.x;
//	vecLine[1] = (float) aptPoly[0].y - pFrom.y;
//
//	// build the arrow base vector - normal to the line
//	vecLeft[0] = -vecLine[1];
//	vecLeft[1] = vecLine[0];
//
//	// setup length parameters
//	fLength = (float) sqrt(vecLine[0] * vecLine[0] + vecLine[1] * vecLine[1]);
//	th = pA->nWidth / (2.0f * fLength);
//	ta = pA->nWidth / (2.0f * (tanf(pA->fTheta) / 2.0f) * fLength);
//
//	// find the base of the arrow
//	pBase.x = (int) (aptPoly[0].x + -ta * vecLine[0]);
//	pBase.y = (int) (aptPoly[0].y + -ta * vecLine[1]);
//
//	// build the points on the sides of the arrow
//	aptPoly[1].x = (int) (pBase.x + th * vecLeft[0]);
//	aptPoly[1].y = (int) (pBase.y + th * vecLeft[1]);
//	aptPoly[2].x = (int) (pBase.x + -th * vecLeft[0]);
//	aptPoly[2].y = (int) (pBase.y + -th * vecLeft[1]);
//
//	MoveToEx(hDC, pFrom.x, pFrom.y, NULL);
//
//	// draw we're fillin'...
//	//if(pA->bFill) 
//	{
//		LineTo(hDC, pBase.x, pBase.y);
//
//		LOGPEN logArrowPen = m_logpen;
//		logArrowPen.lopnWidth.x = 1;
//		logArrowPen.lopnWidth.y = 1;
//		CPen ArrowPen;
//		ArrowPen.CreatePenIndirect(&logArrowPen);
//		CDCHandle dc(hDC);
//		HPEN hOldPen = dc.SelectPen(ArrowPen);
//		Polygon(hDC, aptPoly, 3);
//		dc.SelectPen(hOldPen);
//	}
//
//	// ... or even jes chillin'...
//	//else 
//	//{
//	//	CDCHandle dc(hDC);
//	//	dc.LineTo(aptPoly[0]);
//	//	dc.MoveTo(aptPoly[0]);
//	//	dc.LineTo(aptPoly[1]);
//	//	
//	//	dc.MoveTo(aptPoly[0]);
//	//	dc.LineTo(aptPoly[2]);
//	//	//LineTo(hDC, pBase.x, pBase.y);
//	//	//MoveToEx(hDC, aptPoly[0].x, aptPoly[0].y, NULL);
//	//}
//}

void CDrawRect::ArrowTo(HDC hDC, const CPoint& ptFrom, const CPoint& ptTo, LPARROWINFO pA) 
{
	Point pBase;
	Point aptPoly[3];
	float vecLine[2];
	float vecLeft[2];
	float fLength;
	float th;
	float ta;

	// get from point
	// set to point
	aptPoly[0].X = ptTo.x;
	aptPoly[0].Y = ptTo.y;

	// build the line vector
	vecLine[0] = (float) aptPoly[0].X - ptFrom.x;
	vecLine[1] = (float) aptPoly[0].Y - ptFrom.y;

	// build the arrow base vector - normal to the line
	vecLeft[0] = -vecLine[1];
	vecLeft[1] = vecLine[0];

	// setup length parameters
	fLength = (float) sqrt(vecLine[0] * vecLine[0] + vecLine[1] * vecLine[1]);
	if (fLength <= pA->nWidth)
	{
		m_bAvailObject = FALSE;
		return;
	}
	th = pA->nWidth / (2.0f * fLength);
	ta = pA->nWidth / (2.0f * (tanf(pA->fTheta) / 2.0f) * fLength);

	// find the base of the arrow
	pBase.X = (int) (aptPoly[0].X + -ta * vecLine[0]);
	pBase.Y = (int) (aptPoly[0].Y + -ta * vecLine[1]);

	// build the points on the sides of the arrow
	aptPoly[1].X = (int) (pBase.X + th * vecLeft[0]);
	aptPoly[1].Y = (int) (pBase.Y + th * vecLeft[1]);
	aptPoly[2].X = (int) (pBase.X + -th * vecLeft[0]);
	aptPoly[2].Y = (int) (pBase.Y + -th * vecLeft[1]);

	Graphics graphic(hDC);
	graphic.SetSmoothingMode(SmoothingModeAntiAlias);
	Color clrArrow;
	clrArrow.SetFromCOLORREF(m_logpen.lopnColor);
	Pen ArrowPen(clrArrow, m_logpen.lopnWidth.x);
	graphic.DrawLine(&ArrowPen, ptFrom.x, ptFrom.y, pBase.X, pBase.Y);

	SolidBrush ArrowBrush(clrArrow);
	graphic.FillPolygon(&ArrowBrush, aptPoly, 3, FillModeAlternate);
}

//void CDrawRect::Draw(HDC hDC, BOOL bOriginal)
//{
//	BOOL bRet = FALSE;
//	CDCHandle dc(hDC);
//
//	CBrush brush;
//	CPen pen;
//
//	API_VERIFY(NULL != brush.CreateBrushIndirect(&m_logbrush));
//	API_VERIFY(NULL != pen.CreatePenIndirect(&m_logpen));
//
//	CBrushHandle pOldBrush;
//	CPenHandle pOldPen;
//
//	if (m_bBrush)
//	{
//		pOldBrush = dc.SelectBrush(brush);
//	}
//	else
//	{
//		pOldBrush = dc.SelectStockBrush(NULL_BRUSH);
//	}
//
//	if (m_bPen)
//	{
//		pOldPen = dc.SelectPen(pen);
//	}
//	else
//	{
//		pOldPen = dc.SelectStockPen(NULL_PEN);
//	}
//
//	CRect rect;
//	if (bOriginal)
//	{
//		rect = m_originalPos;
//	}
//	else
//	{
//		rect = m_position;
//	}
//	
//	switch (m_objType)
//	{
//		//case dotArrow:
//		//{
//			////draw on device units
//			//m_pDrawCanvas->DocToClient(&rect);
//
//			////FTLTRACE(TEXT("On Draw SelectRect, rcOld=[%d,%d]x[%d,%d], rect=[%d,%d]x[%d,%d]\n"),
//			////	rcOld.left, rcOld.top, rcOld.right, rcOld.bottom,
//			////	rect.left, rect.top, rect.right, rect.bottom);
//
//			//HBRUSH hOldBrush = dc.SelectStockBrush(NULL_BRUSH);
//			//dc.Rectangle(rect);
//			//dc.SelectBrush(hOldBrush);
//		//	break;
//		//}
//		case dotRect:
//			dc.Rectangle(rect);
//			break;
//
//		case dotRoundRect:
//			dc.RoundRect(rect, m_roundness);
//			break;
//
//		case dotEllipse:
//			dc.Ellipse(rect);
//			break;
//
//		case dotLine:
//		{
//			if (rect.top > rect.bottom)
//			{
//				rect.top -= m_logpen.lopnWidth.y / 2;
//				rect.bottom += (m_logpen.lopnWidth.y + 1) / 2;
//			}
//			else
//			{
//				rect.top += (m_logpen.lopnWidth.y + 1) / 2;
//				rect.bottom -= m_logpen.lopnWidth.y / 2;
//			}
//
//			if (rect.left > rect.right)
//			{
//				rect.left -= m_logpen.lopnWidth.x / 2;
//				rect.right += (m_logpen.lopnWidth.x + 1) / 2;
//			}
//			else
//			{
//				rect.left += (m_logpen.lopnWidth.x + 1) / 2;
//				rect.right -= m_logpen.lopnWidth.x / 2;
//			}
//
//			Graphics graphics(hDC);
//			graphics.SetSmoothingMode(SmoothingModeAntiAlias);
//			Color clrPen;
//			clrPen.SetFromCOLORREF(m_logpen.lopnColor);
//			Pen LinePen(clrPen, m_logpen.lopnWidth.x);
//			graphics.DrawLine(&LinePen, rect.TopLeft().x, rect.TopLeft().y, rect.BottomRight().x, rect.BottomRight().y);
//
//			break;
//		}
//		case dotLineArrow:
//		{
//			if (rect.top > rect.bottom)
//			{
//				rect.top -= m_logpen.lopnWidth.y / 2;
//				rect.bottom += (m_logpen.lopnWidth.y + 1) / 2;
//			}
//			else
//			{
//				rect.top += (m_logpen.lopnWidth.y + 1) / 2;
//				rect.bottom -= m_logpen.lopnWidth.y / 2;
//			}
//
//			if (rect.left > rect.right)
//			{
//				rect.left -= m_logpen.lopnWidth.x / 2;
//				rect.right += (m_logpen.lopnWidth.x + 1) / 2;
//			}
//			else
//			{
//				rect.left += (m_logpen.lopnWidth.x + 1) / 2;
//				rect.right -= m_logpen.lopnWidth.x / 2;
//			}
//			//CPoint ptBottomRight = rect.BottomRight();
//			//CPoint ptTopLeft     = rect.TopLeft();
//			//dc.MoveTo(ptTopLeft);
//			//
//			//if (m_bPen)
//			//{
//			//	LOGBRUSH logbrush = m_logbrush;
//			//	logbrush.lbColor   = m_logpen.lopnColor;
//			//	CBrush Arrowbrush;
//			//	API_VERIFY(NULL != Arrowbrush.CreateBrushIndirect(&logbrush));
//			//	HBRUSH OldBrush = dc.SelectBrush(Arrowbrush);
//			//	m_stArrowInfo.nWidth = max(3 * m_logpen.lopnWidth.x, 5);
//			//	ArrowTo(hDC, ptBottomRight, &m_stArrowInfo);
//			//	dc.SelectBrush(OldBrush);
//			//}
//			ArrowTo(hDC, rect.TopLeft(), rect.BottomRight(), &m_stArrowInfo);
//
//			break;
//		}
//		default:
//			FTLASSERT(FALSE);
//			break;
//	}
//
//	dc.SelectBrush(pOldBrush);
//	dc.SelectPen(pOldPen);
//}
//


void CDrawRect::Draw(HDC hDC, BOOL bOriginal)
{
	BOOL bRet = FALSE;

	CRect rect;
	if (bOriginal)
	{
		rect = m_originalPos;
	}
	else
	{
		rect = m_position;
	}
	
	Graphics graphics(hDC);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	Color clrPen;
	clrPen.SetFromCOLORREF(m_logpen.lopnColor);
	Pen penObject(clrPen, m_logpen.lopnWidth.x);

	Color clrBrush;
	if(m_bBrush)
	{
		clrBrush.SetFromCOLORREF(m_logbrush.lbColor);
	}
	else
	{
		clrBrush.SetValue(Color::MakeARGB(0, GetRValue(m_logbrush.lbColor), 
			GetGValue(m_logbrush.lbColor), GetBValue(m_logbrush.lbColor)));
	}
	SolidBrush brhObject(clrBrush);
	switch (m_objType)
	{
		case dotRect:
			{
				rect.NormalizeRect();
				
				//Rect rcRect(rect.left, rect.top, rect.Width(), rect.Height());
				rect.top  += (m_logpen.lopnWidth.x / 2 - (m_logpen.lopnWidth.x - 1) % 2);
				rect.left += (m_logpen.lopnWidth.x / 2 - (m_logpen.lopnWidth.x - 1) % 2);
				rect.right -= (m_logpen.lopnWidth.x / 2 + m_logpen.lopnWidth.x % 2);
				rect.bottom -= (m_logpen.lopnWidth.x / 2 + m_logpen.lopnWidth.x % 2);
				Rect rcFrame(rect.left, rect.top, rect.Width(), rect.Height());
				graphics.FillRectangle(&brhObject, rcFrame);
				//rcRect.Inflate(-m_logpen.lopnWidth.x / 2 - m_logpen.lopnWidth.x % 2, -m_logpen.lopnWidth.x / 2 - m_logpen.lopnWidth.x % 2);
				graphics.DrawRectangle(&penObject, rcFrame);
				//dc.Rectangle(rect);
				break;
			}
		case dotRoundRect:
			{
				//dc.RoundRect(rect, m_roundness);
				break;
			}
		case dotEllipse:
			{
				//dc.Ellipse(rect);
				rect.NormalizeRect();
				Rect rcEllipse(rect.left, rect.top, rect.Width(), rect.Height());
				

				rect.top  += (m_logpen.lopnWidth.x / 2 - (m_logpen.lopnWidth.x - 1) % 2);
				rect.left += (m_logpen.lopnWidth.x / 2 - (m_logpen.lopnWidth.x - 1) % 2);
				rect.right -= (m_logpen.lopnWidth.x / 2 + m_logpen.lopnWidth.x % 2);
				rect.bottom -= (m_logpen.lopnWidth.x / 2 + m_logpen.lopnWidth.x % 2);
				Rect rcFrame(rect.left, rect.top, rect.Width(), rect.Height());
				graphics.FillEllipse(&brhObject, rcFrame);
				graphics.DrawEllipse(&penObject, rcFrame);
				break;
			}
		case dotLine:
		{
			//if (rect.top > rect.bottom)
			//{
			//	rect.top -= m_logpen.lopnWidth.y / 2;
			//	rect.bottom += (m_logpen.lopnWidth.y + 1) / 2;
			//}
			//else
			//{
			//	rect.top += (m_logpen.lopnWidth.y + 1) / 2;
			//	rect.bottom -= m_logpen.lopnWidth.y / 2;
			//}

			//if (rect.left > rect.right)
			//{
			//	rect.left -= m_logpen.lopnWidth.x / 2;
			//	rect.right += (m_logpen.lopnWidth.x + 1) / 2;
			//}
			//else
			//{
			//	rect.left += (m_logpen.lopnWidth.x + 1) / 2;
			//	rect.right -= m_logpen.lopnWidth.x / 2;
			//}
			Point ptBegin;
			Point ptEnd;
			ptBegin.X = rect.TopLeft().x;
			ptBegin.Y = rect.TopLeft().y;

			ptEnd.X = rect.BottomRight().x;
			ptEnd.Y = rect.BottomRight().y;

			if (CheckAvailObject())
			{
				graphics.DrawLine(&penObject, ptBegin, ptEnd);
			}
			break;
		}
		case dotLineArrow:
		{
			//if (rect.top > rect.bottom)
			//{
			//	rect.top -= m_logpen.lopnWidth.y / 2;
			//	rect.bottom += (m_logpen.lopnWidth.y + 1) / 2;
			//}
			//else
			//{
			//	rect.top += (m_logpen.lopnWidth.y + 1) / 2;
			//	rect.bottom -= m_logpen.lopnWidth.y / 2;
			//}

			//if (rect.left > rect.right)
			//{
			//	rect.left -= m_logpen.lopnWidth.x / 2;
			//	rect.right += (m_logpen.lopnWidth.x + 1) / 2;
			//}
			//else
			//{
			//	rect.left += (m_logpen.lopnWidth.x + 1) / 2;
			//	rect.right -= m_logpen.lopnWidth.x / 2;
			//}
			ArrowTo(hDC, rect.TopLeft(), rect.BottomRight(), &m_stArrowInfo);
			break;
		}
		default:
			FTLASSERT(FALSE);
			break;
	}
}


int CDrawRect::GetHandleCount()
{
	int nHandleCount = CDrawObject::GetHandleCount();
	if (dotRoundRect == m_objType)
	{
		nHandleCount++;
	}
	else if (dotLine == m_objType)
	{
		nHandleCount = 2;
	}
	else if (dotLineArrow == m_objType)
	{
		nHandleCount = 2;
	}
	return nHandleCount;
}

// returns center of handle in device coordinates
CPoint CDrawRect::GetHandle(int nHandle)
{
	//if ( dotLine == m_objType && 2 == nHandle )
	//{
	//	nHandle = 5;
	//}
	//else if (dotLineArrow == m_objType && 2 == nHandle )
	//{
	//	nHandle = 5;
	//}
	if (dotLineArrow == m_objType || dotLine == m_objType)
	{
		if (1 == nHandle)
		{
			CPoint point = m_position.TopLeft();
			m_pDrawCanvas->DocToClient(&point);
			return point;
		}
		else if (2 == nHandle)
		{
			CPoint point = m_position.BottomRight();
			m_pDrawCanvas->DocToClient(&point);
			return point;
		}
	}
	else if (dotRoundRect == m_objType && 9 == nHandle )
	{
		//FTLASSERT(FALSE); //change to device units
		CRect rect = m_position;
		rect.NormalizeRect();
		CPoint point = rect.BottomRight();
		point.x -= m_roundness.x / 2;
		point.y -= m_roundness.y / 2;
		m_pDrawCanvas->DocToClient(&point);
		return point;
	}

	return CDrawObject::GetHandle(nHandle);
}

HCURSOR CDrawRect::GetHandleCursor(int nHandle)
{
	if (dotLine == m_objType && 2 == nHandle )
	{
		nHandle = 5;
	}
	else if (dotLineArrow == m_objType && 2 == nHandle )
	{
		nHandle = 5;
	}
	else if (dotRoundRect == m_objType && 9 == nHandle )
	{
		return ::LoadCursor(NULL, IDC_SIZEALL);
	}
	return CDrawObject::GetHandleCursor(nHandle);
}

// point is in logical coordinates
void CDrawRect::MoveHandleTo(int nHandle, CPoint point)
{
	if (dotLine == m_objType && 2 == nHandle )
	{
		nHandle = 5;
	}
	else if (dotLineArrow == m_objType && 2 == nHandle )
	{
		nHandle = 5;
	}
	else if ( dotRoundRect == m_objType  && 9 == nHandle )
	{
		CRect rect = m_position;
		rect.NormalizeRect();
		if (point.x > rect.right - 1)
		{
			point.x = rect.right - 1;
		}
		else if (point.x < rect.left + rect.Width() / 2)
		{
			point.x = rect.left + rect.Width() / 2;
		}
		if (point.y > rect.bottom - 1)
		{
			point.y = rect.bottom - 1;
		}
		else if (point.y < rect.top + rect.Height() / 2)
		{
			point.y = rect.top + rect.Height() / 2;
		}
		m_roundness.x = 2 * (rect.right - point.x);
		m_roundness.y = 2 * (rect.bottom - point.y);
		//m_pDocument->SetModifiedFlag();
		//if (pView == NULL)
		//    Invalidate();
		//else
		m_pDrawCanvas->InvalObject(this);
		return;
	}
	CDrawObject::MoveHandleTo(nHandle, point);
}

// rect must be in logical coordinates
BOOL CDrawRect::Intersects(const CRect& rect)
{
	CRect rectT = rect;
	rectT.NormalizeRect();

	CRect fixed = m_position;
	fixed.NormalizeRect();
	if ((rectT & fixed).IsRectEmpty())
	{
		return FALSE;
	}

	CRgn rgn;
	switch (m_objType)
	{
		case dotRect:
		case dotArrow:
		case dotImage:
			return TRUE;

		case dotRoundRect:
			rgn.CreateRoundRectRgn(fixed.left, fixed.top, fixed.right, fixed.bottom,
				m_roundness.x, m_roundness.y);
			break;

		case dotEllipse:
			rgn.CreateEllipticRgnIndirect(fixed);
			break;

		case dotLine:
		case dotLineArrow:
			{
				int x = (m_logpen.lopnWidth.x + 5) / 2;
				int y = (m_logpen.lopnWidth.y + 5) / 2;
				POINT points[4];
				points[0].x = fixed.left;
				points[0].y = fixed.top;
				points[1].x = fixed.left;
				points[1].y = fixed.top;
				points[2].x = fixed.right;
				points[2].y = fixed.bottom;
				points[3].x = fixed.right;
				points[3].y = fixed.bottom;

				if (fixed.left < fixed.right)
				{
					points[0].x -= x;
					points[1].x += x;
					points[2].x += x;
					points[3].x -= x;
				}
				else
				{
					points[0].x += x;
					points[1].x -= x;
					points[2].x -= x;
					points[3].x += x;
				}

				if (fixed.top < fixed.bottom)
				{
					points[0].y -= y;
					points[1].y += y;
					points[2].y += y;
					points[3].y -= y;
				}
				else
				{
					points[0].y += y;
					points[1].y -= y;
					points[2].y -= y;
					points[3].y += y;
				}
				rgn.CreatePolygonRgn(points, 4, ALTERNATE);
			}
			break;
		default:
			FTLASSERT(FALSE);
			break;
	}
	return rgn.RectInRegion(fixed);
}

CDrawObject* CDrawRect::Clone()
{
	DRAWOBJBASEINFO stDrawInfo(_T(""));
	stDrawInfo.logbrush = m_logbrush;
	stDrawInfo.logpen   = m_logpen;
	CDrawRect* pClone = new CDrawRect(m_pDrawCanvas, m_position, m_objType, stDrawInfo);
	pClone->m_bPen = m_bPen;
	//pClone->m_logpen = m_logpen;
	pClone->m_bBrush = m_bBrush;
	//pClone->m_logbrush = m_logbrush;
	pClone->m_roundness = m_roundness;

	return pClone;
}


BOOL CDrawRect::CheckAvailObject()
{
	if (m_objType == dotLine || m_objType == dotLineArrow)
	{
		if (((abs(m_position.Width()) < 5) && abs(m_position.Height()) < 5))
		{
			return FALSE;
		}
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
///////////////////          CDrawArrow                   ////////////////////////
//////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
//////////////            2
//                        |\
//         0_____________1| \
//         |                 \3
//        6|_____________5   /
//                        | /
//                        |/
//                        4
/////////////////////////////////////////////////////////////////////////////////

CDrawArrow::CDrawArrow(IDrawCanvas* pDrawCanvas, const CRect& position, 
					   DrawObjectType objType, const DRAWOBJBASEINFO& stDrawObjInfo)
: CDrawRect(pDrawCanvas, position, objType, stDrawObjInfo)
{
	m_dbVertical    = 0.25f;
	m_dbHorizontal1 = 0.75f;
	m_dbHorizontal2 = 0.75f;

	_UpdateArrowPoint();
}

void CDrawArrow::Draw(HDC hDC, BOOL bOriginal)
{
	CDCHandle dc(hDC);
	CBrush brush;
	if (!brush.CreateBrushIndirect(&m_logbrush))
	{
		return;
	}
	CPen pen;
	if (!pen.CreatePenIndirect(&m_logpen))
	{
		return;
	}

	CBrush pOldBrush;
	CPen pOldPen;

	if (m_bBrush)
	{
		pOldBrush = dc.SelectBrush(brush);
	}
	else
	{
		pOldBrush = dc.SelectStockBrush(NULL_BRUSH);
	}

	if (m_bPen)
	{
		pOldPen = dc.SelectPen(pen);
	}
	else
	{
		pOldPen = dc.SelectStockPen(NULL_PEN);
	}

	dc.Polygon(m_ptArrow, 7);

	dc.SelectBrush(pOldBrush);
	dc.SelectPen(pOldPen);
}

int CDrawArrow::GetHandleCount()
{
	return 11;
}

CPoint CDrawArrow::GetHandle(int nHandle)
{
	if (nHandle == 9)
	{
		CPoint ptHandle = m_ptArrow[0];
		m_pDrawCanvas->DocToClient(&ptHandle);
		return ptHandle;
	}
	else if (nHandle == 10)
	{
		CPoint ptHandle = m_ptArrow[1];
		m_pDrawCanvas->DocToClient(&ptHandle);
		return ptHandle;
	}
	else if (nHandle == 11)
	{
		CPoint ptHandle = m_ptArrow[2];
		m_pDrawCanvas->DocToClient(&ptHandle);
		return ptHandle;
	}
	else
	{
		return CDrawRect::GetHandle(nHandle);
	}
}

HCURSOR CDrawArrow::GetHandleCursor(int nHandle)
{
	if (9 == nHandle )
	{
		return ::LoadCursor(NULL, IDC_SIZENS);
	}
	else if (10 == nHandle)
	{
		return ::LoadCursor(NULL, IDC_SIZEWE);
	}
	else if (11 == nHandle)
	{
		return ::LoadCursor(NULL, IDC_SIZEWE);
	}
	else
	{
		return CDrawRect::GetHandleCursor(nHandle);
	}
}

void CDrawArrow::MoveHandleTo(int nHandle, CPoint point)
{
	if (9 == nHandle )
	{
		CRect rect = m_position;
		if (point.y > m_position.top && point.y < m_position.top + m_position.Height() / 2)
		{
			m_ptArrow[0].y = point.y;
			m_dbVertical = (double)(m_ptArrow[0].y - m_position.top) / (double)m_position.Height();
			_UpdateArrowPoint();
			m_pDrawCanvas->InvalObject(this);
		}
	}
	else if (10 == nHandle)
	{
		CRect rect = m_position;
		if (point.x > m_position.left && point.x < m_position.right)
		{
			m_ptArrow[1].x = point.x;
			m_dbHorizontal1 = (double)(m_ptArrow[1].x - m_position.left) / (double)m_position.Width();
			_UpdateArrowPoint();
			m_pDrawCanvas->InvalObject(this);
		}
	}
	else if (11 == nHandle)
	{
		CRect rect = m_position;
		if (point.x > m_position.left && point.x < m_position.right)
		{
			m_ptArrow[2].x = point.x;
			m_dbHorizontal2 = (double)(m_ptArrow[2].x - m_position.left) / (double)m_position.Width();
			_UpdateArrowPoint();
			m_pDrawCanvas->InvalObject(this);
		}
	}
	else
	{
		CDrawRect::MoveHandleTo(nHandle, point);
		_UpdateArrowPoint();
		m_pDrawCanvas->InvalObject(this);
	}
}


CDrawObject* CDrawArrow::Clone()
{
	DRAWOBJBASEINFO stDrawInfo(_T(""));
	stDrawInfo.logbrush = m_logbrush;
	stDrawInfo.logpen   = m_logpen;
	CDrawArrow* pClone = new CDrawArrow(m_pDrawCanvas, m_position, m_objType, stDrawInfo);
	pClone->m_bPen = m_bPen;
	//pClone->m_logpen = m_logpen;
	pClone->m_bBrush = m_bBrush;
	//pClone->m_logbrush = m_logbrush;
	pClone->m_roundness = m_roundness;

	for (int i = 0; i < 7; i++)
	{
		pClone->m_ptArrow[i] = m_ptArrow[i];
	}
	return pClone;
}

void CDrawArrow::MoveTo(const CRect& position)
{
	if (position == m_position)
	{
		return;
	}

	for (int i = 0; i < 7; i ++)
	{
		m_ptArrow[i].x += position.left - m_position.left;
		m_ptArrow[i].y += position.top - m_position.top;
	}
	m_pDrawCanvas->InvalObject(this);
	//m_position = position;
	SetPosition(position, FALSE);
	m_pDrawCanvas->InvalObject(this);
}


void CDrawArrow::_UpdateArrowPoint()
{
	CRect rcTmp = m_position;
	rcTmp.NormalizeRect(); 
	//ATLTRACE(_T("top%d bottom%d left%d right%d \r\n"), rcTmp.top, rcTmp.bottom, rcTmp.left, rcTmp.right);
	m_ptArrow[0].x = rcTmp.left;
	m_ptArrow[0].y = rcTmp.top + (int)(rcTmp.Height() * m_dbVertical);

	m_ptArrow[1].x = rcTmp.left + (int)(rcTmp.Width() * m_dbHorizontal1);
	m_ptArrow[1].y = m_ptArrow[0].y;

	m_ptArrow[2].x = rcTmp.left + (int)(rcTmp.Width() * m_dbHorizontal2);
	m_ptArrow[2].y = rcTmp.top;

	m_ptArrow[3].x = rcTmp.right;
	m_ptArrow[3].y = rcTmp.top + (int)(rcTmp.Height() / 2);

	m_ptArrow[4].x = m_ptArrow[2].x;
	m_ptArrow[4].y = rcTmp.bottom;

	m_ptArrow[5].x = m_ptArrow[1].x;
	m_ptArrow[5].y = rcTmp.bottom - (int)(rcTmp.Height() * m_dbVertical);

	m_ptArrow[6].x = rcTmp.left;
	m_ptArrow[6].y = rcTmp.bottom - (int)(rcTmp.Height() * m_dbVertical);
}

////////////////////////////////////////////////////////////////////////////////////
///////////////////////        CDrawBalloon                  ///////////////////////

CDrawBalloon::CDrawBalloon(IDrawCanvas* pDrawCanvas, const CRect& position, 
						   DrawObjectType objType, const DRAWOBJBASEINFO& stDrawObjInfo)
: CDrawRect(pDrawCanvas, position, objType, stDrawObjInfo)
{
	m_flRectScale = 0.125f;
	m_flPolyScale = 0.25f;
}

void CDrawBalloon::Draw(HDC hDC, BOOL bOriginal)
{
	BOOL bRet = FALSE;
	CDCHandle dc(hDC);

	CBrush brush;
	CBrush FrameBrush;

	API_VERIFY(NULL != brush.CreateBrushIndirect(&m_logbrush));
	LOGBRUSH logFrameBrush;
	logFrameBrush.lbColor = m_logpen.lopnColor;
	logFrameBrush.lbStyle = BS_SOLID;
	logFrameBrush.lbHatch =  HS_HORIZONTAL;
	API_VERIFY(NULL != FrameBrush.CreateBrushIndirect(&logFrameBrush));
	CBrushHandle pOldBrush;

	CRect rect;
	if (bOriginal)
	{
		rect = m_originalPos;
	}
	else
	{
		rect = m_position;
	}
	rect.NormalizeRect();
	rect.right += 1;
	CPoint ptPoly[3];

	ptPoly[0].x = rect.left + rect.Width() * m_flPolyScale;
	ptPoly[0].y = rect.bottom;
	int nRoundness = rect.Width() * m_flRectScale;
	rect.bottom = rect.top + rect.Height() * (1 - m_flRectScale);

	ptPoly[1].x = ptPoly[0].x - rect.Width() * m_flPolyScale / 2;
	ptPoly[1].y = rect.bottom - m_logpen.lopnWidth.x;

	ptPoly[2].x = ptPoly[0].x + rect.Width() * m_flPolyScale / 2;
	ptPoly[2].y = rect.bottom - m_logpen.lopnWidth.x;

	CRgn hRgnPoly;
	hRgnPoly.CreatePolygonRgn(ptPoly, 3, ALTERNATE);

	CRgn hClient;
	hClient.CreateRoundRectRgn(rect.TopLeft().x, rect.TopLeft().y, rect.BottomRight().x, 
		rect.BottomRight().y, nRoundness, nRoundness);
	
	if (m_rgnObject.IsNull())
	{
		m_rgnObject.CreateRectRgn(1, 1, 2, 2);
	}
	m_rgnObject.CombineRgn(hRgnPoly, hClient, RGN_OR);
	if (m_bBrush)
	{
		dc.FillRgn(m_rgnObject, brush);
	}
	else 
	{
		dc.FillRgn(m_rgnObject, (HBRUSH)::GetStockObject(NULL_BRUSH));
	}
	

	if (m_bPen)
	{
		dc.FrameRgn(m_rgnObject, FrameBrush, m_logpen.lopnWidth.x, m_logpen.lopnWidth.x);
	}
	else
	{
		dc.FrameRgn(m_rgnObject, (HBRUSH)::GetStockObject(NULL_BRUSH), m_logpen.lopnWidth.x, m_logpen.lopnWidth.x);
	}
	
	hRgnPoly.DeleteObject();
	hClient.DeleteObject();
}


BOOL CDrawBalloon::Intersects(const CRect& rect)
{
	CRect rectT = rect;
	rectT.NormalizeRect();

	CRect fixed = m_position;
	fixed.NormalizeRect();

	if ((rectT & fixed).IsRectEmpty())
	{
		return FALSE;
	}
	if (m_rgnObject.RectInRegion(fixed))
	{
		return TRUE;
	}
	return FALSE;
}

CDrawObject* CDrawBalloon::Clone()
{
	DRAWOBJBASEINFO stDrawInfo(_T(""));
	stDrawInfo.logpen   = m_logpen;
	stDrawInfo.logbrush = m_logbrush;
	stDrawInfo.bBrush   = m_bBrush;
	stDrawInfo.bPen     = m_bPen;
	CDrawBalloon* pClone = new CDrawBalloon(m_pDrawCanvas, m_position, m_objType, stDrawInfo);

	m_flPolyScale = pClone->m_flPolyScale;
	m_flRectScale = pClone->m_flRectScale;
	return pClone;
}



////////////////////////////////////////////////////////////////////////////////////
///////////////////////        CDrawImage                    ///////////////////////
CDrawImage::CDrawImage(IDrawCanvas* pDrawCanvas, const CRect& position, 
					   DrawObjectType objType, const DRAWOBJBASEINFO& stDrawObjInfo)
: CDrawRect(pDrawCanvas, position, objType, stDrawObjInfo)
{
	m_strImageFile = stDrawObjInfo.strImageFileName;
}	

void CDrawImage::Draw(HDC hDC, BOOL bOriginal)
{
	if (!m_strImageFile.IsEmpty())
	{
		NDGraphics::CGDIPImage GdiImage;
		SilverlightCpp::ZipManager* pZipManager = SilverlightCpp::ZipManager::get_Current();
		FTLASSERT(pZipManager); 
		CImage* pImage = pZipManager->LoadCImage(m_strImageFile);
		CRect rect = m_position;
		rect.NormalizeRect();
		if (pImage)
		{
			GdiImage.Load(pZipManager->LoadCImage(m_strImageFile), ImageFormatPNG);
			GdiImage.Draw(hDC, rect.left, rect.top, rect.Width(), rect.Height());
		}
	}
}

CDrawObject* CDrawImage::Clone()
{
	DRAWOBJBASEINFO stDrawInfo(_T(""));
	stDrawInfo.logpen   = m_logpen;
	stDrawInfo.logbrush = m_logbrush;
	stDrawInfo.bBrush   = m_bBrush;
	stDrawInfo.bPen     = m_bPen;
	stDrawInfo.strImageFileName = m_strImageFile;
	CDrawImage* pClone = new CDrawImage(m_pDrawCanvas, m_position, m_objType, stDrawInfo);
	return pClone;
}

BOOL CDrawImage::UpdateDrawInfo(const DRAWOBJBASEINFO& stDrawObjInfo)
{
	if ((stDrawObjInfo.dwDrawMask & DRAWOBJECT_BASE_IMAGEFILE) &&
		stDrawObjInfo.strImageFileName.CompareNoCase(m_strImageFile) != 0)
	{
		m_strImageFile = stDrawObjInfo.strImageFileName;
		return TRUE;
	}
	return FALSE;
}
