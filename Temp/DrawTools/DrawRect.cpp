#include "stdafx.h"
#include "DrawRect.h"
#include "DrawCanvas.h"
#include <ftlGdi.h>

CDrawRect::CDrawRect(IDrawCanvas* pDrawCanvas, const CRect& position, DrawObjectType objType)
: CDrawObject(pDrawCanvas, position, objType)
{
	if (dotSelectRect == m_objType)
	{
		m_bBrush = FALSE;
		m_logpen.lopnStyle = PS_DOT;
	}
	m_roundness.x = 16;
	m_roundness.y = 16;
}

void CDrawRect::Draw(HDC hDC, BOOL bOriginal)
{
	BOOL bRet = FALSE;
	CDCHandle dc(hDC);

	CBrush brush;
	CPen pen;

	API_VERIFY(NULL != brush.CreateBrushIndirect(&m_logbrush));
	API_VERIFY(NULL != pen.CreatePenIndirect(&m_logpen));

	CBrushHandle pOldBrush;
	CPenHandle pOldPen;

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

	CRect rect;
	if (bOriginal)
	{
		rect = m_originalPos;
	}
	else
	{
		rect = m_position;
	}
	
	switch (m_objType)
	{
		case dotSelectRect:
		{
			//draw on device units
			m_pDrawCanvas->DocToClient(&rect);

			//FTLTRACE(TEXT("On Draw SelectRect, rcOld=[%d,%d]x[%d,%d], rect=[%d,%d]x[%d,%d]\n"),
			//	rcOld.left, rcOld.top, rcOld.right, rcOld.bottom,
			//	rect.left, rect.top, rect.right, rect.bottom);

			HBRUSH hOldBrush = dc.SelectStockBrush(NULL_BRUSH);
			dc.Rectangle(rect);
			dc.SelectBrush(hOldBrush);
			break;
		}
		case dotRect:
			dc.Rectangle(rect);
			break;

		case dotRoundRect:
			dc.RoundRect(rect, m_roundness);
			break;

		case dotEllipse:
			dc.Ellipse(rect);
			break;

		case dotLine:
		{
			if (rect.top > rect.bottom)
			{
				rect.top -= m_logpen.lopnWidth.y / 2;
				rect.bottom += (m_logpen.lopnWidth.y + 1) / 2;
			}
			else
			{
				rect.top += (m_logpen.lopnWidth.y + 1) / 2;
				rect.bottom -= m_logpen.lopnWidth.y / 2;
			}

			if (rect.left > rect.right)
			{
				rect.left -= m_logpen.lopnWidth.x / 2;
				rect.right += (m_logpen.lopnWidth.x + 1) / 2;
			}
			else
			{
				rect.left += (m_logpen.lopnWidth.x + 1) / 2;
				rect.right -= m_logpen.lopnWidth.x / 2;
			}

			dc.MoveTo(rect.TopLeft());
			dc.LineTo(rect.BottomRight());
			break;
		}
		default:
			FTLASSERT(FALSE);
			break;
	}

	dc.SelectBrush(pOldBrush);
	dc.SelectPen(pOldPen);
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
	return nHandleCount;
}

// returns center of handle in device coordinates
CPoint CDrawRect::GetHandle(int nHandle)
{
	if ( dotLine == m_objType && 2 == nHandle )
	{
		nHandle = 5;
	}
	else if (dotRoundRect == m_objType && 9 == nHandle )
	{
		FTLASSERT(FALSE); //change to device units
		CRect rect = m_position;
		rect.NormalizeRect();
		CPoint point = rect.BottomRight();
		point.x -= m_roundness.x / 2;
		point.y -= m_roundness.y / 2;
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
		case dotSelectRect:
		case dotRect:
			return TRUE;

		case dotRoundRect:
			rgn.CreateRoundRectRgn(fixed.left, fixed.top, fixed.right, fixed.bottom,
				m_roundness.x, m_roundness.y);
			break;

		case dotEllipse:
			rgn.CreateEllipticRgnIndirect(fixed);
			break;

		case dotLine:
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
	CDrawRect* pClone = new CDrawRect(m_pDrawCanvas, m_position, m_objType);
	pClone->m_bPen = m_bPen;
	pClone->m_logpen = m_logpen;
	pClone->m_bBrush = m_bBrush;
	pClone->m_logbrush = m_logbrush;
	pClone->m_roundness = m_roundness;

	return pClone;
}
