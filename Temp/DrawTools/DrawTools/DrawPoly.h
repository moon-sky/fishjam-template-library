#pragma once
#include "drawobject.h"

class CDrawPoly : public CDrawObject
{
public:
	CDrawPoly(IDrawCanvas* pDrawCanvas, const CRect& position, DrawObjectType objType, const DRAWOBJBASEINFO& stDrawObjInfo);
	virtual ~CDrawPoly(void);

	void AddPoint(const CPoint& point, BOOL bInvalidate = FALSE);
	BOOL RecalcBounds();

	virtual void Draw(HDC hDC, BOOL bOriginal);
	virtual int GetHandleCount();
	virtual CPoint GetHandle(int nHandle);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual void MoveTo(const CRect& position);
	virtual void MoveHandleTo(int nHandle, CPoint point);
	virtual CDrawObject* Clone();
	virtual BOOL Intersects(const CRect& rect);

	void RemoveHandle();
protected:
	int m_nPoints;
	int m_nAllocPoints;
	CPoint* m_points;
};
