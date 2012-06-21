#pragma once

#include "DrawTypeDefine.h"

class CDrawObject// : public CMessageFilter
{
public:
	CDrawObject(IDrawCanvas* pDrawCanvas, const CRect& position, DrawObjectType objType, const DRAWOBJBASEINFO& stDrawObjInfo);
	virtual ~CDrawObject();

	CRect GetPosition() const
	{
		return m_position;
	}

	virtual void SetPosition(const CRect & pos);
	
	CRect GetOriginalPosition() const
	{
		return m_originalPos;
	}
	DrawObjectType GetDrawObjType() const
	{
		return m_objType;
	}


	void SetZoomFactor(float zoomFactor);
	//void CalcZoomRect( const CRect& rcOriginal, float zoomFactor, CRect& rcTarget);

	virtual int GetHandleCount();
	virtual CPoint GetHandle(int nHandle);
	CRect GetHandleRect(int nHandleID);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual HCURSOR GetActiveCursor();
	//virtual void SetLineColor(COLORREF color);
	virtual BOOL UpdateDrawInfo(const DRAWOBJBASEINFO& stDrawObjInfo);

	// Operations
	virtual void Draw(HDC hDC, BOOL bOriginal);
	enum TrackerState 
	{
		normal = 0, 
		selected, 
		active 
	};

	//virtual BOOL PreTranslateMessage(MSG* pMsg);

	virtual void NormalizePosition();
	virtual void DrawTracker(HDC hDC, TrackerState state, BOOL bDrawSelectTool);
	virtual void MoveTo(const CRect& position);
	virtual int  HitTest(CPoint point, BOOL bSelected);
	virtual BOOL HitTestMove(CPoint point);
	virtual BOOL HitTestActive(CPoint point);
	// virtual int HitTest2(CPoint point);
	virtual int SizingHitTest(CPoint ptLogic, CPoint ptOrigLogic);
	virtual BOOL Intersects(const CRect& rect);
	virtual void MoveHandleTo(int nHandle, CPoint point);
	virtual void OnOpen();
	virtual void OnEditProperties();
	virtual CDrawObject* Clone();
	virtual void Remove();
	virtual void EndMoveHandle();
	virtual BOOL CheckAvailObject();
	virtual void SetActive(BOOL bActive);
	virtual BOOL IsActive();
	//void Invalidate();

protected:
	// Attributes
	CRect	m_position; //logical
	CRect	m_originalPos; //logical
	float	m_zoomFactor;
	DrawObjectType m_objType;

	IDrawCanvas*	m_pDrawCanvas;
	BOOL			m_bPen;
	BOOL			m_bBrush;
	LOGPEN			m_logpen;
	LOGBRUSH		m_logbrush;
	BOOL			m_bActive;
};



class CDrawFreeObject : public CDrawObject
{
public:
	CDrawFreeObject(IDrawCanvas* pDrawCanvas, const CRect& position, DrawObjectType objType, const DRAWOBJBASEINFO& stDrawInfo);
	virtual ~CDrawFreeObject();


	void AddPoint(const CPoint& point);
	BOOL RecalcBounds();

	virtual void Draw(HDC hDC, BOOL bOriginal);
	virtual int GetHandleCount();
	virtual CPoint GetHandle(int nHandle);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual void MoveTo(const CRect& position);
	virtual void MoveHandleTo(int nHandle, CPoint point);
	virtual CDrawObject* Clone();
	virtual BOOL Intersects(const CRect& rect);
	virtual void DrawTracker(HDC hDC, TrackerState state, BOOL bDrawSelectTool);
	virtual BOOL HitTestMove(CPoint point);
	void RemoveHandle();
protected:
	int m_nPoints;
	int m_nAllocPoints;
	CPoint* m_points;
};