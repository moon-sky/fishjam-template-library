#include "StdAfx.h"
#include "DrawPoly.h"
#include "DrawTypeDefine.h"
#include <ftlGdi.h>

CDrawPoly::CDrawPoly(IDrawCanvas* pDrawCanvas, const CRect& position, DrawObjectType objType, const DRAWOBJBASEINFO& stDrawObjInfo)
: CDrawObject(pDrawCanvas, position, objType, stDrawObjInfo)
{
	m_points = NULL;
	m_nPoints = 0;
	m_nAllocPoints = 0;

}

CDrawPoly::~CDrawPoly(void)
{
	if (m_points != NULL)
		delete[] m_points;
}

void CDrawPoly::AddPoint(const CPoint& point)
{
	if (m_nPoints == m_nAllocPoints)
	{
		CPoint* newPoints = new CPoint[m_nAllocPoints + 10];
		if (m_points != NULL)
		{
			memcpy_s(newPoints, sizeof(CPoint) * (m_nAllocPoints + 10), m_points, sizeof(CPoint) * m_nAllocPoints);
			delete[] m_points;
		}
		m_points = newPoints;
		m_nAllocPoints += 10;
	}

	if (m_nPoints == 0 || m_points[m_nPoints - 1] != point)
	{
		m_points[m_nPoints++] = point;
		//if(RecalcBounds())
		//{
			m_pDrawCanvas->InvalObject(this);
		//}
	}
}


void CDrawPoly::Draw(HDC hDC, BOOL bOriginal)
{
	CDCHandle dc(hDC);
	CBrush brush;
	if (!brush.CreateBrushIndirect(&m_logbrush))
		return;
	CPen pen;
	if (!pen.CreatePenIndirect(&m_logpen))
		return;

	CBrush pOldBrush;
	CPen pOldPen;

	if (m_bBrush)
		pOldBrush = dc.SelectBrush(brush);
	else
		pOldBrush = dc.SelectStockBrush(NULL_BRUSH);

	if (m_bPen)
		pOldPen = dc.SelectPen(pen);
	else
		pOldPen = dc.SelectStockPen(NULL_PEN);

	dc.Polygon(m_points, m_nPoints);

	dc.SelectBrush(pOldBrush);
	dc.SelectPen(pOldPen);
}

int CDrawPoly::GetHandleCount()
{
	return m_nPoints;
}

CPoint CDrawPoly::GetHandle(int nHandle)
{
	ATLASSERT(nHandle >= 1 && nHandle <= m_nPoints);
	CPoint ptRet = m_points[nHandle - 1];
	m_pDrawCanvas->DocToClient(&ptRet);
	return ptRet;
}


HCURSOR CDrawPoly::GetHandleCursor(int nHandle)
{
	return ::LoadCursor(NULL, IDC_SIZEALL);
}

// position must be in logical coordinates
void CDrawPoly::MoveTo(const CRect& position)
{
	if (position == m_position)
		return;

	for (int i = 0; i < m_nPoints; i += 1)
	{
		m_points[i].x += position.left - m_position.left;
		m_points[i].y += position.top - m_position.top;
	}

	m_position = position;
	m_pDrawCanvas->InvalObject(this);
}

void CDrawPoly::MoveHandleTo(int nHandle, CPoint point)
{	
	ATLASSERT(nHandle >= 1 && nHandle <= m_nPoints);
	if (m_points[nHandle - 1] == point)
		return;

	m_points[nHandle - 1] = point;
	m_pDrawCanvas->InvalObject(this);
}

// rect must be in logical coordinates
BOOL CDrawPoly::Intersects(const CRect& rect)
{
	CRgn rgn;
	rgn.CreatePolygonRgn(m_points, m_nPoints, ALTERNATE);
	return rgn.RectInRegion(rect);
}


void CDrawPoly::RemoveHandle()
{
	m_nPoints--;
}

CDrawObject* CDrawPoly::Clone()
{
	DRAWOBJBASEINFO stDrawInfo;
	stDrawInfo.logpen = m_logpen;
	stDrawInfo.logbrush = m_logbrush;
	CDrawPoly* pClone = new CDrawPoly(m_pDrawCanvas, m_position, m_objType, stDrawInfo);
	pClone->m_bPen = m_bPen;
	//pClone->m_logpen = m_logpen;
	pClone->m_bBrush = m_bBrush;
	//pClone->m_logbrush = m_logbrush;
	pClone->m_points = new CPoint[m_nAllocPoints];

	memcpy_s(pClone->m_points, sizeof(CPoint) * m_nAllocPoints, m_points, sizeof(CPoint) * m_nPoints);
	pClone->m_nAllocPoints = m_nAllocPoints;
	pClone->m_nPoints = m_nPoints;

	return pClone;
}

BOOL CDrawPoly::RecalcBounds()
{
	if (m_nPoints == 0)
		return FALSE;

	CRect bounds(m_points[0], CSize(0, 0));
	for (int i = 1; i < m_nPoints; ++i)
	{
		if (m_points[i].x < bounds.left)
			bounds.left = m_points[i].x;
		if (m_points[i].x > bounds.right)
			bounds.right = m_points[i].x;
		if (m_points[i].y < bounds.top)
			bounds.top = m_points[i].y;
		if (m_points[i].y > bounds.bottom)
			bounds.bottom = m_points[i].y;
	}

	if (bounds == m_position)
	{
		return FALSE;
	}

	return TRUE;
}
