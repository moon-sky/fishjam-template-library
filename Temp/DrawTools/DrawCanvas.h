#pragma once

#include <list>
#include <algorithm>
#include <ftlFunctional.h>

#include "DrawObject.h"
#include "DrawTool.h"
#include "DrawRect.h"
#include "SelectTool.h"
#include "RectTool.h"
#include "TextTool.h"
#include "SelectTool.h"
#include "MoveTool.h"
#include "PolyTool.h"
#include "FreeDrawTool.h"

#include "DrawTypeDefine.h"

static DRAWOBJBASEINFO s_stDrawObjInfo;
static CMoveTool   s_MoveTool(&s_stDrawObjInfo);
static CSelectTool s_SelectTool(&s_stDrawObjInfo);
static CRectTool   s_RectTool(&s_stDrawObjInfo, ttRect, _T("Rect"));
static CRectTool   s_LineTool(&s_stDrawObjInfo, ttLine, _T("Line"));
static CRectTool   s_RoundRectTool(&s_stDrawObjInfo, ttRoundRect, _T("RoundRect"));
static CRectTool   s_EllipseTool(&s_stDrawObjInfo, ttEllipse, _T("Ellipse"));
static CRectTool   s_ArrowTool(&s_stDrawObjInfo, ttArrow, _T("Arrow"));
static CRectTool   s_BalloonTool(&s_stDrawObjInfo, ttBalloon, _T("Balloon"));
//static CRectTool   s_TextTool(ttText, _T("Text"));
static CTextTool   s_TextTool(&s_stDrawObjInfo, _T("Text"));
static CRectTool   s_LineArrowTool(&s_stDrawObjInfo, ttLineArrow, _T("LineArrow"));
static CFreeDrawTool   s_FreeLineTool(&s_stDrawObjInfo, ttFreeObject, _T("FreeLine"));
static CPolyTool       s_PolyTool(&s_stDrawObjInfo, ttPoly, _T("Poly"));

template <typename T>
class CDrawCanvas : public IDrawCanvas
{
protected:
	CPoint			m_ptMouseDownLogical;
	CPoint			m_ptMouseLastLogical;
	DrawObjectList	m_allObjects;
	DrawObjectList	m_selection;

	//CSelectTool*	m_pSelectTool;
	//CDrawRect*		m_pSelectRect;
	ToolType		m_nCurToolType;
	
	DrawToolList	m_tools;
	//BOOL			m_bActive;
	//BOOL			m_bCaptured;
	int				m_nDragHandle;
	float			m_zoomFactor;

	SelectMode      m_nCurrentSelectMode;

	LPDRAWOBJBASEINFO m_pDrawInfo;
public:
	CDrawCanvas()
	{
		m_ptMouseDownLogical.SetPoint(INVALID_POS_X, INVALID_POS_Y);
		m_ptMouseLastLogical.SetPoint(INVALID_POS_X, INVALID_POS_Y);

		//m_bActive = FALSE;
		m_nCurToolType = ttNone;
		m_nDragHandle = 0;
		m_zoomFactor = 0;
		m_nCurrentSelectMode = smNone;
		//m_pSelectRect = NULL;
		//m_bCaptured = FALSE;
		//m_pSelectTool = new CSelectTool();

		m_tools.push_back(&s_MoveTool);
		s_SelectTool.InitResource();
		m_tools.push_back(&s_SelectTool);
		m_tools.push_back(&s_RectTool);
		m_tools.push_back(&s_LineTool);
		m_tools.push_back(&s_RoundRectTool);
		m_tools.push_back(&s_EllipseTool);
		m_tools.push_back(&s_PolyTool);
		m_tools.push_back(&s_ArrowTool);
		m_tools.push_back(&s_FreeLineTool);
		m_tools.push_back(&s_TextTool);
		m_tools.push_back(&s_LineArrowTool);
		m_tools.push_back(&s_BalloonTool);
		m_pDrawInfo = &s_stDrawObjInfo;
	}
	virtual ~CDrawCanvas()
	{
		m_selection.clear();
		//std::for_each(m_allObjects.begin(), m_allObjects.end(), FTL::ObjecteDeleter<CDrawObject*>());
		m_allObjects.clear();

		//m_pSelectTool = NULL;
		//std::for_each(m_tools.begin(), m_tools.end(), FTL::ObjecteDeleter<CDrawTool*>() );
		m_tools.clear();
	}

	BOOL OnSetCursor(CWindow wnd, UINT nHitTest, UINT message)
	{
		BOOL bRet = FALSE;
		CDrawTool* pTool = GetCurrentTool();
		if (pTool)
		{
			bRet = pTool->OnSetCursor(wnd, nHitTest, message);
		}
		return bRet;
	}

	BOOL OnLButtonDblClk(UINT nFlags, const CPoint& point)
	{
		CDrawTool* pTool = GetCurrentTool();
		if (pTool)
		{
			if (_IsValidateScreenPoint(point))
			{
				return pTool->OnLButtonDblClk(this, nFlags, point);
			}
		}
		return FALSE;;
	}

	void OnLButtonDown(UINT nFlags, const CPoint& point)
	{
		//fix Win+D lost focus
		SetFocus(GetHWnd());
		CDrawTool* pTool = GetCurrentTool();
		if (pTool)
		{
			if (_IsValidateScreenPoint(point))
			{
				pTool->OnLButtonDown(this, nFlags, point);
			}
			//else
			//{
			//	pTool->OnLButtonDown(this, nFlags, m_ptLastValidateDevice);
			//}
		}
	}

	void OnMouseMove(UINT nFlags, const CPoint& point)
	{
		CDrawTool* pTool = GetCurrentTool();
		if (pTool)
		{
			if (_IsValidateScreenPoint(point))
			{
				pTool->OnMouseMove(this, nFlags, point);
			}
			//else
			//{
			//	CPoint ptDownDevice = m_ptMouseDownLogical;
			//	this->DocToClient(&ptDownDevice);
			//	//If this happen, must be Win + D when drag mouse in view
			//	pTool->OnCancel(this);
			//}
		}
	}

	void OnCancelMode()
	{
		CDrawTool* pTool = GetCurrentTool();
		if (pTool)
		{
			pTool->OnCancel(this);
		}
	}

	BOOL OnLButtonUp(UINT nFlags, const CPoint& point)
	{
		CDrawTool* pTool = GetCurrentTool();
		if (pTool)
		{
			if (_IsValidateScreenPoint(point))
			{
				return pTool->OnLButtonUp(this, nFlags, point);
			}
			//else
			//{
			//	pTool->OnLButtonUp(this, nFlags, m_ptLastValidateDevice);
			//}
		}
		return FALSE;
	}

	BOOL OnSetCursor(HWND hWnd, UINT nHitTest, UINT message)
	{
		CDrawTool* pTool = GetCurrentTool();
		if (pTool)
		{
			return pTool->OnSetCursor(hWnd, nHitTest, message);
		}
		return FALSE;
	}

	virtual CPoint GetMouseDownLogicalPoint()
	{
		return m_ptMouseDownLogical;
	}
	virtual VOID SetMouseDownLogicalPoint(const CPoint& ptDown)
	{
		m_ptMouseDownLogical = ptDown;
	}
	virtual CPoint GetMouseLastLogicalPoint()
	{
		return m_ptMouseLastLogical;
	}
	virtual VOID SetMouseLastLogicalPoint(const CPoint& ptLast)
	{
		m_ptMouseLastLogical = ptLast;
	}

	CDrawTool* FindTool(ToolType toolType)
	{
		for (DrawToolList::iterator iter = m_tools.begin();
			iter != m_tools.end();
			++iter)
		{
			CDrawTool* pTool = *iter;
			if (pTool->GetToolType() == toolType)
			{
				return pTool;
			}
		}

		return NULL;
	}

	virtual ToolType   GetCurrentToolType()
	{
		return m_nCurToolType;
	}

	virtual VOID SetCurrentToolType(ToolType nToolType)
	{
		//if (m_nCurToolType == ttSelection && nToolType != ttSelection)
		//{
		//	ReleaseSelectRect();
		//}
		m_nCurToolType = nToolType;
	}

	virtual CDrawTool*  GetCurrentTool() 
	{
		CDrawTool* pTool = FindTool(m_nCurToolType);
		return pTool;
	}

	virtual SelectMode GetCurrentSelectMode()
	{
		return m_nCurrentSelectMode;
	}
	virtual VOID SetCurrentSelectMode(SelectMode selectMode)
	{
		m_nCurrentSelectMode = selectMode;
	}

	virtual int GetDragHandle()
	{
		return m_nDragHandle;
	}

	virtual VOID SetDragHandle(int nDragHandle)
	{
		m_nDragHandle = nDragHandle;
	}

	//virtual BOOL CreateSelectRect(const CRect& rcPosition, BOOL bSelect = TRUE)
	//{
	//	FTLASSERT(NULL == m_pSelectRect);
	//	//FTLTRACE(TEXT("CreateSelectRect, pos=[%d,%d]x[%d,%d]\n"),
	//	//	rcPosition.left, rcPosition.top, rcPosition.right, rcPosition.bottom);

	//	ReleaseSelectRect();
	//	m_pSelectRect = new CDrawRect(this, rcPosition, dotSelectRect);
	//	Add(m_pSelectRect);
	//	if (bSelect)
	//	{
	//		Select(m_pSelectRect, TRUE);
	//	}
	//	InvalObject(m_pSelectRect);
	//	return TRUE;
	//}
	//virtual BOOL ReleaseSelectRect()
	//{
	//	if (m_pSelectRect)
	//	{
	//		//CRect rcSelect = m_pSelectRect->GetPosition();
	//		//FTLTRACE(TEXT("ReleaseSelectRect For:[%d,%d]x[%d,%d]\n"), rcSelect.left,
	//		//	rcSelect.top, rcSelect.right, rcSelect.bottom);

	//		InvalObject(m_pSelectRect);
	//		Remove(m_pSelectRect);
	//		SAFE_DELETE(m_pSelectRect);
	//	}
	//	return TRUE;
	//}
	virtual CDrawObject* GetSelectRect()
	{
		//return m_pSelectRect;
		DrawObjectList::iterator iter  = m_selection.begin();
		while(iter != m_selection.end())
		{
			CDrawObject* pObject = *iter;
			if (pObject->GetDrawObjType() == dotSelectRect)
			{
				return *iter;
			}
			iter++;
		}
		return NULL;
	}

	virtual void Add(CDrawObject* pObj)
	{
		RemoveUnavialObect();
		m_allObjects.push_back(pObj);
	}

	virtual void Remove(CDrawObject* pObj, BOOL bPaint = FALSE)
	{
		DrawObjectList::iterator iter = std::find(m_allObjects.begin(), m_allObjects.end(), pObj);
		if (iter != m_allObjects.end())
		{
			m_allObjects.erase(iter);
		}

		DrawObjectList::iterator iter2 = std::find(m_selection.begin(), m_selection.end(), pObj);
		if (iter2 != m_selection.end())
		{
			pObj->SetActive(FALSE);
			m_selection.erase(iter2);
		}
		if (bPaint)
		{
			T* pThis = static_cast<T*>(this);
			pThis->Invalidate();
		}
	}

	virtual void DeleteSelectObjects(BOOL bPaint = FALSE)
	{
		DrawObjectList::iterator iter = m_selection.begin();
		while (iter != m_selection.end())
		{
			CDrawObject* pObject = *iter;
			if (pObject)
			{
				m_selection.erase(iter++);

				DrawObjectList::iterator iter2 = std::find(m_allObjects.begin(), m_allObjects.end(), pObject);
				if (iter2 != m_allObjects.end())
				{
					m_allObjects.erase(iter2);
				}
				delete pObject;
				pObject = NULL;
			}
		}
		if (bPaint)
		{
			T* pThis = static_cast<T*>(this);
			pThis->Invalidate();
		}
	}

	void SetActive(CDrawObject* pObj, BOOL bActive = TRUE)
	{
		//if (bActive)
		{
			DrawObjectList::iterator iter = m_allObjects.begin();
			while (iter != m_allObjects.end())
			{
				if ((*iter) == pObj)
				{
					(*iter)->SetActive(bActive);
				}
				else
				{
					(*iter)->SetActive(FALSE);
				}
				iter ++;
			}
		}
		//else
		//{
		//	pObj->SetActive(FALSE);
		//}
	}

	void InvalObject(CDrawObject* pObj)
	{
		CRect rect = pObj->GetPosition();
		DocToClient(&rect);
		if (IsSelected(pObj)) //m_bActive && 
		{
			rect.left -= 4;
			rect.top -= 5;
			rect.right += 5;
			rect.bottom += 4;
		}
		rect.InflateRect(1, 1); // handles CDrawOleObj objects
		//rect.InflateRect(4, 4); // handles CDrawOleObj objects

		T* pThis = static_cast<T*>(this);
		pThis->Invalidate();
		//::InvalidateRect(pThis->m_hWnd, rect, FALSE);
	}

	virtual BOOL IsSelected(const CDrawObject* pDrawObj) const
	{
		DrawObjectList::const_iterator iter = std::find(m_selection.begin(), m_selection.end(), pDrawObj);
		if (iter != m_selection.end())
		{
			return TRUE;
		}
		return FALSE;
	}

	virtual void Select(CDrawObject* pObj, BOOL bAdd = FALSE)
	{
		if (!bAdd)
		{
			//OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);
			m_selection.clear();
		}

		if (pObj == NULL || IsSelected(pObj))
		{
			return;
		}
		m_selection.push_back(pObj);
		InvalObject(pObj);
	}

	void Deselect(CDrawObject* pObj) 
	{

	}
	void SelectWithinRect(CRect rect, BOOL bAdd = FALSE)
	{

	}

	BOOL IsCapture()
	{
		BOOL bRet = FALSE;
		T* pt = static_cast<T*>(this);
		HWND hCaptureWnd = ::GetCapture();
		//ATLTRACE(_T("CaptureWnd %x\n"), hCaptureWnd);
		if (hCaptureWnd == pt->m_hWnd)
		{
			bRet = TRUE;
		}
		return bRet;
	}
	void BeginCapture() 
	{
		//m_bCaptured = TRUE;
		//ATLTRACE(_T("BeginCapture \n"));
		T* pt = static_cast<T*>(this);
		pt->SetCapture();
	}
	void EndCapture(BOOL bBackupData = TRUE)
	{
		//m_bCaptured = FALSE;
		//ATLTRACE(_T("EndCapture \n"));
		//FTLASSERT(IsCapture());
		if (IsCapture())
		{
			BOOL bRet = FALSE;
			API_VERIFY(::ReleaseCapture());
			//RemoveUnavialObect();
			if (bBackupData)
			{
				BackupDrawObjectData(_T("Naver Capture"));
			}
		}
	}

	HWND GetHWnd()
	{
		T* pThis = static_cast<T*>(this);
		FTLASSERT(::IsWindow(pThis->m_hWnd));
		return pThis->m_hWnd;
	}
	//void UpdateAllViews(CDrawCanvas* pSender){}
	virtual CDrawObject* ObjectAt(const CPoint& point)
	{
		//logical unit
		CRect rect(point, CSize(1, 1));
		//for (DrawObjectList::iterator iter = m_allObjects.begin();
		//	iter != m_allObjects.end();
		//	++iter)
		for (DrawObjectList::reverse_iterator iter = m_allObjects.rbegin();
			iter != m_allObjects.rend();
			++iter)
		{
			CDrawObject* pObj = *iter;
			if (pObj->Intersects(rect))
			{
				return pObj;
			}
		}
		return NULL;
	}

	virtual void CloneSelection() {}
	virtual DrawObjectList& GetSelection() 
	{
		return m_selection;
	}

	//void DrawObjects(HDC hDC, BOOL bOriginal, BOOL bDrawSelectTool)
	//{
	//	for (DrawObjectList::iterator iter = m_allObjects.begin();
	//		iter != m_allObjects.end();
	//		++iter)
	//	{
	//		CDrawObject* pObj = *iter;

	//		BOOL bWillDraw = bDrawSelectTool ^ (pObj->GetDrawObjType() != dotSelectRect);
	//		if ( bWillDraw )
	//		{
	//			pObj->Draw(hDC, bOriginal);
	//			if (IsSelected(pObj))
	//			{
	//				pObj->DrawTracker(hDC, CDrawObject::selected, bDrawSelectTool);
	//			}
	//		}
	//	}
	//}

	//draw info(object/select tracker) on device unit, this will not be effect by ZoomIn or ZoomOut
	//TODO: position ?
	void DrawObjectsTracker(HDC hDC)
	{
		for (DrawObjectList::iterator iter = m_allObjects.begin();
			iter != m_allObjects.end();
			++iter)
		{
			CDrawObject* pObj = *iter;
			//select object -- draw on device 
			//if (pObj->GetDrawObjType() == dotSelectRect)
			//{
			//	pObj->Draw(hDC, FALSE);
			//}

			//select tracker will draw on device
			if (IsSelected(pObj))
			{
				pObj->DrawTracker(hDC, CDrawObject::selected, TRUE);
			}
		}
	}

	//draw info(objects) on logical unit, will be effect by ZoomIn and ZoomOut
	void DrawObjects(HDC hDC)
	{
		for (DrawObjectList::iterator iter = m_allObjects.begin();
			iter != m_allObjects.end();
			++iter)
		{
			CDrawObject* pObj = *iter;
			//normal object -- draw on logical device
			//if (pObj->GetDrawObjType() != dotSelectRect)
			//{
			//	pObj->Draw(hDC, FALSE);
			//}
			if (pObj)
			{
				pObj->Draw(hDC, FALSE);
			}
		}
	}

	BOOL _IsValidateScreenPoint(const CPoint& point)
	{
		BOOL bRet = FALSE;

		//after Win + D, ClientToScreen will return invalidate point

		const INT VALIDATE_MARGIN_X = 30000;	//10000
		const INT VALIDATE_MARGIN_Y = 30000;	//10000
		if ((point.x > -VALIDATE_MARGIN_X && point.x < VALIDATE_MARGIN_X)
			&& (point.y > -VALIDATE_MARGIN_Y && point.y < VALIDATE_MARGIN_Y))
		{
			bRet = TRUE;
		}

		return bRet;
	}

	void RemoveUnavialObect()
	{
		DrawObjectList::iterator it = m_allObjects.begin();
		while(it != m_allObjects.end())
		{
			CDrawObject* pObject = *it;
			if (!pObject->CheckAvailObject())
			{
				DrawObjectList::iterator selIt = std::find(m_selection.begin(), m_selection.end(), pObject);
				if (selIt != m_selection.end())
				{
					m_selection.erase(selIt);
				}

				m_allObjects.erase(it++);
				delete pObject;
				pObject = NULL;
			}
			else
			{
				it++;
			}
		}
	}

	void GetDrawObjectBaseInfo(DRAWOBJBASEINFO& stDrawInfo)
	{
		stDrawInfo = *m_pDrawInfo;
	}

	void SetDrawObjectBaseInfo(const DRAWOBJBASEINFO& stDrawInfo, BOOL bPaint = FALSE)
	{
		*m_pDrawInfo = stDrawInfo;
		for (DrawObjectList::iterator iter = m_selection.begin();
			iter != m_selection.end();
			++iter)
		{
			CDrawObject* pObj = *iter;
			if (pObj && pObj->CheckAvailObject())
			{
				pObj->UpdateDrawInfo(*m_pDrawInfo);
				if (bPaint)
				{
					InvalObject(pObj);
				}
			}
		}
	}
};