#pragma once
#include "DrawObject.h"

class CDrawRect : public CDrawObject
{
public:
	CDrawRect(IDrawCanvas* pDrawCanvas, const CRect& position, DrawObjectType objType);
	// Implementation
public:
	virtual void Draw(HDC hDC, BOOL bOriginal);
	virtual int GetHandleCount();
	virtual CPoint GetHandle(int nHandle);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual void MoveHandleTo(int nHandle, CPoint point);
	virtual BOOL Intersects(const CRect& rect);
	virtual CDrawObject* Clone();

protected:
	CPoint m_roundness; // for roundRect corners
};
