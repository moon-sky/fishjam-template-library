#include "stdafx.h"
#include "DrawObject.h"
#include "DrawCanvas.h"

CDrawObject::CDrawObject(IDrawCanvas* pDrawCanvas, const CRect& position, DrawObjectType objType)
{
	m_pDrawCanvas = pDrawCanvas;
	m_position = position;
	m_originalPos = m_position;
	m_zoomFactor = 1.0f;

	m_objType = objType;
	m_bActive = FALSE;

	m_bPen = TRUE;
	m_logpen.lopnStyle = PS_INSIDEFRAME;
	m_logpen.lopnWidth.x = 1;
	m_logpen.lopnWidth.y = 1;
	m_logpen.lopnColor = RGB(0, 0, 0);

	m_bBrush = TRUE;
	m_logbrush.lbStyle = BS_SOLID;
	m_logbrush.lbColor = RGB(192, 192, 192);
	m_logbrush.lbHatch = HS_HORIZONTAL;
}

CDrawObject::~CDrawObject()
{
}

void CDrawObject::Remove()
{
	delete this;
}

void CDrawObject::SetActive(BOOL bActive)
{
	m_bActive = TRUE;
}

BOOL CDrawObject::IsActive()
{
	return m_bActive;
}

void CDrawObject::Draw(HDC hDC, BOOL bOriginal)
{
}

void CDrawObject::DrawTracker(HDC hDC, TrackerState state, BOOL bDrawSelectTool)
{
	FTLASSERT(this);

	switch (state)
	{
		case normal:
			break;

		case selected:
		case active:
			{
				int nHandleCount = GetHandleCount();
				for (int nHandle = 1; nHandle <= nHandleCount; nHandle += 1)
				{
					CPoint handle = GetHandle(nHandle);
					CRect rect(handle.x - TRACK_MARGIN, handle.y - TRACK_MARGIN, 
						handle.x + TRACK_MARGIN + 1, handle.y + TRACK_MARGIN + 1);
					::Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
				}
			}
			break;
	}
}

// position is in logical
void CDrawObject::MoveTo(const CRect& position)
{
	FTLASSERT(this);

	if (position == m_position)
	{
		return;
	}

	m_pDrawCanvas->InvalObject(this);
	m_position = position;
	//CalcZoomRect(m_position, 1.0f / m_zoomFactor, m_originalPos);
	m_pDrawCanvas->InvalObject(this);
	//m_pDocument->SetModifiedFlag();
}

void CDrawObject::SetZoomFactor(float zoomFactor)
{
	m_zoomFactor = zoomFactor;
	//CalcZoomRect(m_originalPos, m_zoomFactor, m_position);
}

// Note: if bSelected, hit-codes start at one for the top-left
// and increment clockwise, 0 means no hit.
// If !bSelected, 0 = no hit, 1 = hit (anywhere)

// point is in device coordinates
int CDrawObject::HitTest(CPoint point, BOOL bSelected)
{
	FTLASSERT(this);

	if (bSelected)
	{
		int nHandleCount = GetHandleCount();
		for (int nHandle = 1; nHandle <= nHandleCount; nHandle += 1)
		{
			// GetHandleRect returns in device coords
			CRect rc = GetHandleRect(nHandle);
			rc.NormalizeRect();
			//if (1 == nHandle || 2 == nHandle)
			//{
			//	FTLTRACE(TEXT("rcHandle[%d]=[%d,%d]x[%d,%d], point =[%d,%d]\n"),
			//		nHandle, rc.left, rc.top, rc.right, rc.bottom, point.x, point.y);
			//}
			if (rc.PtInRect(point))
			{
				return nHandle;
			}
		}
	}
	else
	{
		FTLASSERT(FALSE);
		CRect rcPosition = m_position;
		rcPosition.NormalizeRect();
		if (rcPosition.PtInRect(point))
		{
			return 1;
		}
	}
	return 0;
}

//int CDrawObject::HitTest2(CPoint point)
//{
//	CRect rcPosition = m_position;
//	m_pDrawCanvas->DocToClient(&rcPosition);
//	rcPosition.NormalizeRect();
//
//	if (point.x <= rcPosition.left)
//	{
//		if (point.y <= rcPosition.top)
//		{
//			// left top
//			return 1;
//		}
//		else if (point.y >= rcPosition.bottom)
//		{
//			// left bottom
//			return 7;
//		}
//		else
//		{
//			// left center
//			return 8;
//		}
//	}
//	else if (point.x >= rcPosition.right)
//	{
//		if (point.y <= rcPosition.top)
//		{
//			// right top
//			return 3;
//		}
//		else if (point.y >= rcPosition.bottom)
//		{
//			// right bottom
//			return 5;
//		}
//		else
//		{
//			// right center
//			return 4;
//		}
//	}
//	else
//	{
//		if (point.y <= rcPosition.top)
//		{
//			// center top
//			return 2;
//		}
//		else if (point.y >= rcPosition.bottom)
//		{
//			// center bottom
//			return 6;
//		}
//		else
//		{
//			// center
//			ATLTRACE("point(%d, %d), rect(%d, %d, %d, %d)\n", point.x, point.y, rcPosition.left, rcPosition.top, rcPosition.right, rcPosition.bottom);
//			FTLASSERT(FALSE);
//			return 0;
//		}
//	}
//}

int CDrawObject::SizingHitTest(CPoint ptLogic, CPoint ptOrigLogic)
{
	CRect rcPosition = m_position;
	m_pDrawCanvas->DocToClient(&rcPosition);
	rcPosition.NormalizeRect();

	if (ptLogic.x > ptOrigLogic.x)
	{
		if (ptLogic.y > ptOrigLogic.y)
		{
			// right bottom
			return 5;
		}
		else
		{
			// right top
			return 3;
		}
	}
	else
	{
		if (ptLogic.y > ptOrigLogic.y)
		{
			// left bottom
			return 7;
		}
		else
		{
			// left top
			return 1;
		}
	}
}

// rect must be in logical coordinates
BOOL CDrawObject::Intersects(const CRect& rect)
{
	FTLASSERT(this);

	CRect fixed = m_position;
	fixed.NormalizeRect();
	CRect rectT = rect;
	rectT.NormalizeRect();
	return !(rectT & fixed).IsRectEmpty();
}

//void CDrawObject::CalcZoomRect(const CRect& rcOriginal, float zoomFactor, CRect& rcTarget)
//{
//	rcTarget.left = (LONG)(zoomFactor * rcOriginal.left);
//	rcTarget.top = (LONG)(zoomFactor * rcOriginal.top);
//	rcTarget.right = (LONG)(zoomFactor * rcOriginal.right);
//	rcTarget.bottom = (LONG)(zoomFactor * rcOriginal.bottom);
//}


int CDrawObject::GetHandleCount()
{
	FTLASSERT(this);
	return 8;
}

// returns device coords of center of handle
CPoint CDrawObject::GetHandle(int nHandle)
{
	FTLASSERT(this);
	int x = 0, y = 0, xCenter, yCenter;
	CRect rcLogical = m_position;

	//rcLogical.OffsetRect(m_pDrawCanvas->GetOffset());
	m_pDrawCanvas->DocToClient(&rcLogical);

	// this gets the center regardless of left/right and top/bottom ordering
	xCenter = rcLogical.left + rcLogical.Width() / 2;
	yCenter = rcLogical.top + rcLogical.Height() / 2;

	switch (nHandle)
	{
		case 1:
			x = rcLogical.left;
			y = rcLogical.top;
			break;

		case 2:
			x = xCenter;
			y = rcLogical.top;
			break;

		case 3:
			x = rcLogical.right - 1;
			y = rcLogical.top;
			break;

		case 4:
			x = rcLogical.right - 1;
			y = yCenter;
			break;

		case 5:
			x = rcLogical.right - 1;
			y = rcLogical.bottom - 1;
			break;

		case 6:
			x = xCenter;
			y = rcLogical.bottom - 1;
			break;

		case 7:
			x = rcLogical.left;
			y = rcLogical.bottom - 1;
			break;

		case 8:
			x = rcLogical.left;
			y = yCenter;
			break;

		default:
			FTLASSERT(FALSE);
			break;
	}

	return CPoint(x, y);
}

// return rectange of handle in device coords
CRect CDrawObject::GetHandleRect(int nHandleID)
{
  //  FTLTRACE(TEXT("CDrawObject(0x%p)::GetHandleRect,m_position=[%d,%d]x[%d,%d], nHandle=%d\n"), this,
		//m_position.left, m_position.top, m_position.right, m_position.bottom,
		//nHandleID);

	CRect rect;
	// get the center of the handle in device coords
	CPoint point = GetHandle(nHandleID);
	// convert to client/device coords
	//m_pDrawCanvas->DocToClient(&point);
	// return CRect of handle in device coords
	rect.SetRect(point.x - TRACK_MARGIN, point.y - TRACK_MARGIN, point.x + TRACK_MARGIN, point.y + TRACK_MARGIN);
	//m_pDrawCanvas->ClientToDoc(&rect);
	return rect;
}

HCURSOR CDrawObject::GetHandleCursor(int nHandle)
{
	FTLASSERT(this);

	LPCTSTR id = IDC_NO;
	switch (nHandle)
	{
		case 1:
		case 5:
			id = IDC_SIZENWSE;
			break;

		case 2:
		case 6:
			id = IDC_SIZENS;
			break;

		case 3:
		case 7:
			id = IDC_SIZENESW;
			break;

		case 4:
		case 8:
			id = IDC_SIZEWE;
			break;
		default:
			FTLASSERT(FALSE);
			break;
	}

	return ::LoadCursor(NULL, id);
}

// point must be in logical
void CDrawObject::MoveHandleTo(int nHandle, CPoint point)
{
	FTLASSERT(this);

	CRect position = m_position;
	switch (nHandle)
	{
		case 1:
			position.left = point.x;
			position.top = point.y;
			break;

		case 2:
			position.top = point.y;
			break;

		case 3:
			position.right = point.x;
			position.top = point.y;
			break;

		case 4:
			position.right = point.x;
			break;

		case 5:
			position.right = point.x;
			position.bottom = point.y;
			break;

		case 6:
			position.bottom = point.y;
			break;

		case 7:
			position.left = point.x;
			position.bottom = point.y;
			break;

		case 8:
			position.left = point.x;
			break;
		default:
			FTLASSERT(FALSE);
			break;
	}

	MoveTo(position);
}

//void CDrawObject::Invalidate()
//{
//    FTLASSERT(this);
//    //m_pDocument->UpdateAllViews(NULL, HINT_UPDATE_DRAWOBJ, this);
//}

CDrawObject* CDrawObject::Clone()
{
	FTLASSERT(this);

	CDrawObject* pClone = new CDrawObject(m_pDrawCanvas, m_position, m_objType);
	pClone->m_bPen = m_bPen;
	pClone->m_logpen = m_logpen;
	pClone->m_bBrush = m_bBrush;
	pClone->m_logbrush = m_logbrush;
	FTLASSERT(pClone);

	return pClone;
}

void CDrawObject::OnEditProperties()
{
	FTLASSERT(this);

#if 0
	CPropertySheet sheet( _T("Shape Properties") );
	CRectDlg dlg;
	dlg.m_bNoFill = !m_bBrush;
	dlg.m_penSize = m_bPen ? m_logpen.lopnWidth.x : 0;
	sheet.AddPage( &dlg );

	if (sheet.DoModal() != IDOK)
		return;

	m_bBrush = !dlg.m_bNoFill;
	m_bPen = dlg.m_penSize > 0;
	if (m_bPen)
	{
		m_logpen.lopnWidth.x = dlg.m_penSize;
		m_logpen.lopnWidth.y = dlg.m_penSize;
	}
#endif 

	//Invalidate();
	//m_pDocument->SetModifiedFlag();
}

void CDrawObject::OnOpen()
{
	OnEditProperties();
}

void CDrawObject::SetLineColor(COLORREF color)
{
	FTLASSERT(this);

	m_logpen.lopnColor = color;
	//Invalidate();
	//m_pDocument->SetModifiedFlag();
}

void CDrawObject::SetFillColor(COLORREF color)
{
	FTLASSERT(this);

	m_logbrush.lbColor = color;
	
	//Invalidate();
	//m_pDocument->SetModifiedFlag();
}

void CDrawObject::NormalizePosition()
{
	m_position.NormalizeRect();
	m_originalPos.NormalizeRect();
}
