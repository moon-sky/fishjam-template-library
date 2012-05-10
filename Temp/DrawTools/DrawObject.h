#pragma once

#include "DrawTypeDefine.h"

class CDrawObject
{
public:
	CDrawObject(IDrawCanvas* pDrawCanvas, const CRect& position, DrawObjectType objType);
	virtual ~CDrawObject();

	CRect GetPosition() const
	{
		return m_position;
	}

	void SetPosition(CRect & pos)
	{
		m_position = pos;
	}
	
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
	virtual void SetLineColor(COLORREF color);
	virtual void SetFillColor(COLORREF color);

	// Operations
	virtual void Draw(HDC hDC, BOOL bOriginal);
	enum TrackerState 
	{
		normal = 0, 
		selected, 
		active 
	};

	virtual void NormalizePosition();
	virtual void DrawTracker(HDC hDC, TrackerState state, BOOL bDrawSelectTool);
	virtual void MoveTo(const CRect& positon);
	virtual int HitTest(CPoint point, BOOL bSelected);
	// virtual int HitTest2(CPoint point);
	virtual int SizingHitTest(CPoint ptLogic, CPoint ptOrigLogic);
	virtual BOOL Intersects(const CRect& rect);
	virtual void MoveHandleTo(int nHandle, CPoint point);
	virtual void OnOpen();
	virtual void OnEditProperties();
	virtual CDrawObject* Clone();
	virtual void Remove();
	//void Invalidate();
	virtual void SetActive(BOOL bActive);
	virtual BOOL IsActive();

protected:
	// Attributes
	CRect	 m_position; //logical
	CRect	m_originalPos; //logical
	float	m_zoomFactor;
	DrawObjectType m_objType;

	IDrawCanvas*	m_pDrawCanvas;
	BOOL			m_bActive;
	BOOL			m_bPen;
	BOOL			m_bBrush;
	LOGPEN			m_logpen;
	LOGBRUSH		m_logbrush;
};
