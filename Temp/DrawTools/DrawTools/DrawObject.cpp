#include "stdafx.h"
#include "DrawObject.h"
#include "DrawCanvas.h"
#include "ftlDefine.h"

CDrawObject::CDrawObject(IDrawCanvas* pDrawCanvas, const CRect& position, 
						 DrawObjectType objType, const DRAWOBJBASEINFO& stDrawObjInfo)
{
	m_pDrawCanvas = pDrawCanvas;
	SetPosition(position, FALSE);
	//m_position = position;
	m_originalPos = m_position;
	m_zoomFactor = 1.0f;

	m_objType = objType;

	m_bPen = stDrawObjInfo.bPen;
	//m_logpen.lopnStyle = PS_INSIDEFRAME;
	//m_logpen.lopnWidth.x = 1;
	//m_logpen.lopnWidth.y = 1;
	//m_logpen.lopnColor = RGB(0, 0, 0);
	m_logpen = stDrawObjInfo.logpen;

	m_bBrush = stDrawObjInfo.bBrush;
	m_logbrush = stDrawObjInfo.logbrush;
	//m_logbrush.lbStyle = BS_SOLID;
	//m_logbrush.lbColor = RGB(192, 192, 192);
	//m_logbrush.lbHatch = HS_HORIZONTAL;
	m_bAvailObject = TRUE;
	m_bHasBeenSized = FALSE;
}

CDrawObject::~CDrawObject()
{
}

CRect CDrawObject::GetInvalidRect()
{
	return m_position;
}

void CDrawObject::SetPosition(const CRect & pos, BOOL bCheckSize)
{
	m_position = pos;

	if (!m_bHasBeenSized)
	{
		//check new width or height
		int minMargin = 2 * TRACK_MARGIN + 1;
		if (abs(pos.Width()) >= minMargin || abs(pos.Height()) >= minMargin )
		{
			m_bHasBeenSized = TRUE;
		}
	}
	
	if (bCheckSize && ! HasBeenSized())
	{
		BOOL bWillInvalidate = FALSE;

		CSize szMin = GetMinSize();
		if (abs(m_position.Width()) < szMin.cx )
		{
			bWillInvalidate = TRUE;
			if (m_position.left < m_position.right)
			{
				m_position.right = m_position.left + szMin.cx;
			}
			else 
			{
				m_position.left = m_position.right;
				m_position.right =m_position.left + szMin.cx;
			}
		}
		if (abs(m_position.Height()) < szMin.cy)
		{
			bWillInvalidate = TRUE;
			if (m_position.top < m_position.bottom)
			{
				m_position.bottom = m_position.top + szMin.cy;
			}
			else
			{
				m_position.top = m_position.bottom;
				m_position.bottom = m_position.top + szMin.cy;
			}
		}

		if (bWillInvalidate)
		{
			m_pDrawCanvas->InvalObject(this);
		}
	}
}

CSize CDrawObject::GetMinSize()
{
	return CSize(0, 0);
}

void CDrawObject::Remove()
{
	delete this;
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
					//ATLTRACE(_T("rect.left %d, rect.top %d, rect.right %d, rect.bottom %d nHandle %d\n"), rect.left, rect.top, rect.right, rect.bottom, nHandle);
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
	//m_position = position;
	SetPosition(position, FALSE);
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
			FTLASSERT(!rc.IsRectEmpty());
			//rc.NormalizeRect();
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

BOOL CDrawObject::HitTestMove(CPoint point)
{
	CRect rcDevice = m_position;
	m_pDrawCanvas->DocToClient(&rcDevice);
	rcDevice.NormalizeRect();
	if (rcDevice.PtInRect(point))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawObject::HitTestActive(CPoint point)
{
	return FALSE;
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

	//rcLogical.NormalizeRect();
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

HCURSOR CDrawObject::GetActiveCursor()
{
	return ::LoadCursor(NULL, IDC_SIZEALL);
}

void CDrawObject::RegulateRectToSquare(CRect& rcPos, eTypePointInRect eType )
{
	switch(eType)
	{
		case PIRLEFT:
			 {

				 if(abs(rcPos.Width()) > abs(rcPos.Height()))
				 {
					 if(rcPos.right > rcPos.left)
					 {	
						 rcPos.left = rcPos.right - abs(rcPos.Height());
					 }
					 else
					 {
						 rcPos.left = rcPos.right + abs(rcPos.Height());
					 }
				 }
			 }break;

		case PIRRIGHT:
			{
				if(abs(rcPos.Width()) > abs(rcPos.Height()))
				{
					if(rcPos.right > rcPos.left)
					{	
						rcPos.right = rcPos.left + abs(rcPos.Height());
					}
					else
					{
						rcPos.right = rcPos.left - abs(rcPos.Height());
					}
				}
			}break;
		case PIRTOP:
			{
				if(abs(rcPos.Width()) < abs(rcPos.Height()))
				{
					if(rcPos.bottom > rcPos.top)
					{
						rcPos.top = rcPos.bottom - abs(rcPos.Width());
					}
					else
					{
						rcPos.top = rcPos.bottom + abs(rcPos.Width());
					}
				}
			}break;
		case PIRBUTTOM:
			{
				if(abs(rcPos.Width()) < abs(rcPos.Height()))
				{
					if(rcPos.bottom > rcPos.top)
					{
						rcPos.bottom = rcPos.top + abs(rcPos.Width());
					}
					else
					{
						rcPos.bottom = rcPos.top - abs(rcPos.Width());
					}
				}
			}break;
	}
}
void CDrawObject::RegulateLine( CRect& rcPos, eTypePointInRect eType )
{
	switch(eType)
	{
		case PIRLEFT:
			{	
				if(2 * abs(rcPos.Width()) < abs(rcPos.Height()))
				{
					rcPos.left = rcPos.right;
					break;
				}

				if( abs(rcPos.Width()) > 2 *  abs(rcPos.Height()))
				{
					break;
				}
				if(abs(rcPos.Width()) > abs(rcPos.Height()))
				{
					if(rcPos.right > rcPos.left)
					{	
						rcPos.left = rcPos.right - abs(rcPos.Height());
					}
					else
					{
						rcPos.left = rcPos.right + abs(rcPos.Height());
					}
				}
			}break;

		case PIRRIGHT:
			{
				if(2 * abs(rcPos.Width()) < abs(rcPos.Height()))
				{
					rcPos.right = rcPos.left;
					break;
				}

				if( abs(rcPos.Width()) > 2 *  abs(rcPos.Height()))
				{
					break;
				}
				if(abs(rcPos.Width()) > abs(rcPos.Height()))
				{
					if(rcPos.right > rcPos.left)
					{	
						rcPos.right = rcPos.left + abs(rcPos.Height());
					}
					else
					{
						rcPos.right = rcPos.left - abs(rcPos.Height());
					}
				}
			}break;
		case PIRTOP:
			{
				if(2 * abs(rcPos.Height()) < abs(rcPos.Width()))
				{
					rcPos.top = rcPos.bottom;
					break;
				}

				if(2* abs(rcPos.Width()) <  abs(rcPos.Height()))
				{
					break;
				}
				if(abs(rcPos.Width()) < abs(rcPos.Height()))
				{
					if(rcPos.bottom > rcPos.top)
					{
						rcPos.top = rcPos.bottom - abs(rcPos.Width());
					}
					else
					{
						rcPos.top = rcPos.bottom + abs(rcPos.Width());
					}
				}
			}break;
		case PIRBUTTOM:
			{
				ATLTRACE("Height:%d--Width:%d\n", rcPos.Height(), rcPos.Width());
				if(2 * abs(rcPos.Height()) < abs(rcPos.Width()))
				{
					rcPos.bottom = rcPos.top;
					break;
				}

				if(2 * abs(rcPos.Width()) < abs(rcPos.Height()))
				{
					break;
				}
				if(abs(rcPos.Width()) < abs(rcPos.Height()))
				{
					if(rcPos.bottom > rcPos.top)
					{
						rcPos.bottom = rcPos.top + abs(rcPos.Width());
					}
					else
					{
						rcPos.bottom = rcPos.top - abs(rcPos.Width());
					}
				}
			}break;
	}
}
void CDrawObject::MoveLineHandleToWithShift( int nHandle, CPoint point )
{
	FTLASSERT(this);

	CRect position = m_position;
	switch (nHandle)
	{
		case 1:
			{
				position.left = point.x;
				position.top = point.y;
				RegulateLine(position, PIRLEFT);
				RegulateLine(position, PIRTOP);
			}break;
		case 5:
			{
				position.right = point.x;
				position.bottom = point.y;
				RegulateLine(position, PIRRIGHT);
				RegulateLine(position, PIRBUTTOM);
			}break;
	}
	MoveTo(position);
}

void CDrawObject::MoveHandleToWithShift( int nHandle, CPoint point )
{
	FTLASSERT(this);

	CRect position = m_position;
	switch (nHandle)
	{
		case 2:
		case 4:
		case 6:
		case 8:
			{
				return;
			}
			break;

		case 1:
			{
				position.left = point.x;
				position.top = point.y;
				RegulateRectToSquare(position, PIRLEFT);
				RegulateRectToSquare(position, PIRTOP);
			}
			break;

		case 3:
			{
				position.right = point.x;
				position.top = point.y;
				RegulateRectToSquare(position, PIRRIGHT);
				RegulateRectToSquare(position, PIRTOP);
			}
			break;

		case 5:
			{
				position.right = point.x;
				position.bottom = point.y;
				RegulateRectToSquare(position, PIRRIGHT);
				RegulateRectToSquare(position, PIRBUTTOM);
			}
			break;

		case 7:
			{
				position.left = point.x;
				position.bottom = point.y;
				RegulateRectToSquare(position, PIRLEFT);
				RegulateRectToSquare(position, PIRBUTTOM);
			}
			
			break;
		default:
			FTLASSERT(FALSE);
			break;
	}

	MoveTo(position);
}

// point must be in logical
void CDrawObject::MoveHandleTo(int nHandle, CPoint point)
{
	FTLASSERT(this);

	if(KEY_DOWN(VK_SHIFT))
	{
		if(dotLine == m_objType || dotLineArrow == m_objType)
		{
			return MoveLineHandleToWithShift(nHandle, point);
		}
		else
		{
			return MoveHandleToWithShift(nHandle, point);
		}
		
	}
	CRect position = m_position;
	switch (nHandle)
	{
		case 1:
			{
				position.left = point.x;
				position.top = point.y;
			}
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
			{
				position.right = point.x;
				position.bottom = point.y;
			}
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

	DRAWOBJBASEINFO stDrawInfo(_T(""));
	stDrawInfo.logpen = m_logpen;
	stDrawInfo.logbrush = m_logbrush;
	CDrawObject* pClone = new CDrawObject(m_pDrawCanvas, m_position, m_objType, stDrawInfo);
	pClone->m_bPen = m_bPen;
	//pClone->m_logpen = m_logpen;
	pClone->m_bBrush = m_bBrush;
	//pClone->m_logbrush = m_logbrush;
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

BOOL CDrawObject::UpdateDrawInfo(const DRAWOBJBASEINFO& stDrawObjInfo)
{
	FTLASSERT(this);

	if (stDrawObjInfo.dwDrawMask & DRAWOBJECT_BASE_PENCLR)
	{
		m_logpen.lopnColor = stDrawObjInfo.logpen.lopnColor;
	}

	if (stDrawObjInfo.dwDrawMask & DRAWOBJECT_BASE_PENWIDTH)
	{
		m_logpen.lopnWidth.x = stDrawObjInfo.logpen.lopnWidth.x;
		m_logpen.lopnWidth.y = stDrawObjInfo.logpen.lopnWidth.y;
	}

	if (stDrawObjInfo.dwDrawMask & DRAWOBJECT_BASE_BRUSHCLR)
	{
		m_logbrush.lbColor = stDrawObjInfo.logbrush.lbColor;
	}


	if (stDrawObjInfo.dwDrawMask & DRAWOBJECT_BASE_BPEN)
	{
		m_bPen     = stDrawObjInfo.bPen;
	}

	if (stDrawObjInfo.dwDrawMask & DRAWOBJECT_BASE_BBRUSH)
	{
		m_bBrush   = stDrawObjInfo.bBrush;
	}
	return TRUE;
	//Invalidate();
	//m_pDocument->SetModifiedFlag();
}

//BOOL CDrawObject::PreTranslateMessage(MSG* pMsg)
//{
//	return FALSE;
//}

void CDrawObject::NormalizePosition()
{
	m_position.NormalizeRect();
	m_originalPos.NormalizeRect();
}

void CDrawObject::EndMoveHandle()
{
	SetPosition(m_position, TRUE);
}

BOOL CDrawObject::CheckAvailObject()
{
	return m_bAvailObject;
}

void CDrawObject::SetActive(BOOL bActive)
{
	BOOL bNeedUpdate = (m_bActive != bActive);
	m_bActive = bActive;
	if (bNeedUpdate)
	{
		m_pDrawCanvas->InvalObject(this);
	}
}

BOOL CDrawObject::IsActive()
{
	return m_bActive;
}

BOOL CDrawObject::Undo()
{
	return FALSE;
}

BOOL CDrawObject::Redo()
{
	return FALSE;
}

BOOL CDrawObject::IsCanUndo()
{
	return FALSE;
}

BOOL CDrawObject::IsCanRedo()
{
	return FALSE;
}

BOOL CDrawObject::BeginEdit()
{
	return FALSE;
}

BOOL CDrawObject::EndEdit(BOOL bIsPushUndo)
{
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////  CDrawFreeObject  ///////////////////////////////////////////////
CDrawFreeObject::CDrawFreeObject(IDrawCanvas* pDrawCanvas, const CRect& position, 
								 DrawObjectType objType, const DRAWOBJBASEINFO& stDrawInfo)
: CDrawObject(pDrawCanvas, position, objType, stDrawInfo)
{
	m_points = NULL;
	m_nPoints = 0;
	m_nAllocPoints = 0;
}

CDrawFreeObject::~CDrawFreeObject()
{
	if (m_points)
	{
		delete[] m_points;
		m_points = NULL;
	}
}


void CDrawFreeObject::AddPoint(const CPoint& point)
{
	if (m_nPoints == m_nAllocPoints)
	{
		CPoint* newPoints = new CPoint[m_nAllocPoints + 10];
		if (m_points != NULL)
		{
			memcpy_s(newPoints, sizeof(CPoint) * (m_nAllocPoints + 10), m_points, sizeof(CPoint) * m_nAllocPoints);
			delete[] m_points;
			m_points = NULL;
		}
		m_points = newPoints;
		m_nAllocPoints += 10;
	}

	if (m_nPoints == 0 || m_points[m_nPoints - 1] != point)
	{
		m_points[m_nPoints++] = point;
		if(!RecalcBounds())
		{
			m_pDrawCanvas->InvalObject(this);
		}
	}
}

BOOL CDrawFreeObject::RecalcBounds()
{
	if (m_nPoints == 0)
	{
		return FALSE;
	}

	CRect rect(m_points[0], CSize(0, 0));
	rect.top = m_points[0].y;
	rect.left = m_points[0].x;
	rect.bottom = m_points[0].y;
	rect.right = m_points[0].x;
	for (int i = 1; i < m_nPoints; ++i)
	{
		if (m_points[i].x < rect.left)
		{
			rect.left = m_points[i].x;
		}
		if (m_points[i].x > rect.right)
		{
			rect.right = m_points[i].x;
		}
		if (m_points[i].y < rect.top)
		{
			rect.top = m_points[i].y;
		}
		if (m_points[i].y > rect.bottom)
		{
			rect.bottom = m_points[i].y;
		}
	}

	if (rect == m_position)
	{
		return FALSE;
	}
	m_position = rect;
	return TRUE;
}


void CDrawFreeObject::Draw(HDC hDC, BOOL bOriginal)
{
	Graphics graphics(hDC);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	Color clrPen;
	clrPen.SetFromCOLORREF(m_logpen.lopnColor);
	Pen penObject(clrPen, m_logpen.lopnWidth.x);

	
	GraphicsPath FreePath;
	Point ptBegin;
	Point ptEnd;
	for (int i = 0; i < m_nPoints; i++)
	{
		if (i == 0)
		{
			//dc.MoveTo(m_points[i]);
			ptBegin.X = m_points[i].x;
			ptBegin.Y = m_points[i].y;
		}
		else
		{
			ptEnd.X = m_points[i].x;
			ptEnd.Y = m_points[i].y;
			//graphics.DrawLine(&penObject, ptBegin, ptEnd);
			FreePath.AddLine(ptBegin, ptEnd);
			ptBegin = ptEnd;
		}
	}
	graphics.DrawPath(&penObject, &FreePath);
}

int CDrawFreeObject::GetHandleCount()
{
	return 2;
}

CPoint CDrawFreeObject::GetHandle(int nHandle)
{
	ATLASSERT(nHandle >= 1 && nHandle <= m_nPoints);
	if (nHandle == 1)
	{
		CPoint ptRet = m_points[0];
		m_pDrawCanvas->DocToClient(&ptRet);
		return ptRet;
	}
	else
	{
		CPoint ptRet = m_points[m_nPoints - 1];
		m_pDrawCanvas->DocToClient(&ptRet);
		return ptRet;
	}
}

HCURSOR CDrawFreeObject::GetHandleCursor(int nHandle)
{
	return ::LoadCursor(NULL, IDC_CROSS);
}

void CDrawFreeObject::MoveTo(const CRect& position)
{
	if (position == m_position)
	{
		return;
	}

	for (int i = 0; i < m_nPoints; i += 1)
	{
		m_points[i].x += position.left - m_position.left;
		m_points[i].y += position.top - m_position.top;
	}

	//m_position = position;
	SetPosition(position, FALSE);
	m_pDrawCanvas->InvalObject(this);
}

void CDrawFreeObject::MoveHandleTo(int nHandle, CPoint point)
{
	return;
}

CDrawObject* CDrawFreeObject::Clone()
{
	DRAWOBJBASEINFO stDrawInfo(_T(""));
	stDrawInfo.logpen = m_logpen;
	stDrawInfo.logbrush = m_logbrush;
	CDrawFreeObject* pClone = new CDrawFreeObject(m_pDrawCanvas, m_position, m_objType, stDrawInfo);
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

BOOL CDrawFreeObject::Intersects(const CRect& rect)
{
	return FALSE;
}

void CDrawFreeObject::DrawTracker(HDC hDC, TrackerState state, BOOL bDrawSelectTool)
{
	//nothing to do
}

BOOL CDrawFreeObject::HitTestMove(CPoint point)
{
	return FALSE;
}