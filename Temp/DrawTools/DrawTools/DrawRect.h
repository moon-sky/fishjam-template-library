#pragma once
#include "DrawObject.h"

typedef struct stu_ArrowInfo
{
	int nWidth;		// width (in pixels) of the full base of the arrowhead
	float fTheta;	// angle (in radians) at the arrow tip between the two
	//  sides of the arrowhead
	bool bFill;		// flag indicating whether or not the arrowhead should be
	//  filled
	stu_ArrowInfo()
	{
		nWidth = 10;
		fTheta = 1.0f;
		bFill  = true;
	}
}ARROWINFO, *LPARROWINFO;

class CDrawRect : public CDrawObject
{
public:	
	CDrawRect(IDrawCanvas* pDrawCanvas, const CRect& position, 
		DrawObjectType objType, const DRAWOBJBASEINFO& stDrawObjInfo);
	// Implementation
public:
	virtual CSize GetMinSize();
	virtual void Draw(HDC hDC, BOOL bOriginal);
	virtual int GetHandleCount();
	virtual CPoint GetHandle(int nHandle);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual void MoveHandleTo(int nHandle, CPoint point);
	virtual BOOL Intersects(const CRect& rect);
	virtual CDrawObject* Clone();
	virtual BOOL CheckAvailObject();
	void ArrowTo(HDC hDC, const CPoint& ptFrom, const CPoint& ptTo, LPARROWINFO pA);
protected:
	CPoint m_roundness; // for roundRect corners

	//CPoint m_ptLine[2];
	ARROWINFO m_stArrowInfo;
};


class CDrawArrow : public CDrawRect
{
public:
	CDrawArrow(IDrawCanvas* pDrawCanvas, const CRect& position, 
		DrawObjectType objType, const DRAWOBJBASEINFO& stDrawObjInfo);

public:
	virtual void Draw(HDC hDC, BOOL bOriginal);
	virtual int GetHandleCount();
	virtual CPoint GetHandle(int nHandle);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual void MoveHandleTo(int nHandle, CPoint point);
	//virtual BOOL Intersects(const CRect& rect);
	virtual CDrawObject* Clone();
	virtual void MoveTo(const CRect& positon);
	
protected:
	CPoint m_ptArrow[7];
	double m_dbVertical;
	double m_dbHorizontal1;
	double m_dbHorizontal2;

private:
	void _UpdateArrowPoint();
};

class CDrawBalloon : public CDrawRect
{
public:
	CDrawBalloon(IDrawCanvas* pDrawCanvas, const CRect& position, 
		DrawObjectType objType, const DRAWOBJBASEINFO& stDrawObjInfo);

public:
	virtual void Draw(HDC hDC, BOOL bOriginal);
	virtual int GetHandleCount();
	virtual CPoint GetHandle(int nHandle);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual void MoveHandleTo(int nHandle, CPoint point);
	virtual void MoveTo(const CRect& position);
	virtual CRect GetInvalidRect();
	virtual BOOL Intersects(const CRect& rect);
	virtual CDrawObject* Clone();
	virtual void EndMoveHandle();

	enum PointPosType
	{
		posTopRight = 0,
		posRightTop,
		posRightBottom,
		posBottomRight,
		posBottomLeft,
		posLeftBottom,
		posLeftTop,
		posTopLeft,

		//posTop,
		//posRight,
		//posBottom,
		//posLeft,
		//posCenter,
	};

protected:
	BOOL	m_bUserChangedArrowPos[3];
	BOOL	m_bJustCreated;
	CPoint m_ptArraow[3];
	float m_flRectScale;
	float m_flPolyScale;
	float m_flArrowScale;
	CRgn  m_rgnObject;
	PointPosType	m_ArrowPosType;
private:
	BOOL			_IsChangeSide(PointPosType posOld, PointPosType posNew);
	void			_CalcArrowPoint();
	PointPosType	_CalcArrowPos(const CRect& rect, const CPoint& ptArrow);
};

class CDrawImage : public CDrawRect
{
public:
	CDrawImage(IDrawCanvas* pDrawCanvas, const CRect& position, 
		DrawObjectType objType, const DRAWOBJBASEINFO& stDrawObjInfo);

public:
	virtual void Draw(HDC hDC, BOOL bOriginal);
	virtual CDrawObject* Clone();
	virtual BOOL UpdateDrawInfo(const DRAWOBJBASEINFO& stDrawObjInfo);

protected:
	CString m_strImageFile;
};