#pragma once

#include <list>
#include <algorithm>
#include <ftlFunctional.h>

#include "DrawObject.h"
#include "DrawTool.h"
#include "DrawRect.h"
#include "SelectTool.h"
#include "RectTool.h"
#include "SelectTool.h"
#include "TextObject.h"

#include "DrawTypeDefine.h"

template <typename T>
class CDrawCanvas : public IDrawCanvas
{
protected:
	CPoint			m_ptMouseDownLogical;
	CPoint			m_ptMouseLastLogical;
	DrawObjectList	m_allObjects;
	DrawObjectList	m_selection;

	CSelectTool*	m_pSelectTool;
	CDrawRect*		m_pSelectRect;
	CTextObject*	m_pFoucsTextObject;
	ToolType		m_nCurToolType;
	SelectMode		m_nCurrentSelectMode;
	DrawToolList	m_tools;
	//BOOL			m_bActive;
	//BOOL			m_bCaptured;
	int				m_nDragHandle;
	float			m_zoomFactor;
public:
	CDrawCanvas()
	{
		m_ptMouseDownLogical.SetPoint(INVALID_POS_X, INVALID_POS_Y);
		m_ptMouseLastLogical.SetPoint(INVALID_POS_X, INVALID_POS_Y);

		//m_bActive = FALSE;
		m_nCurToolType = ttNone;
		m_nCurrentSelectMode = smNone;
		m_pSelectRect = NULL;
		m_pFoucsTextObject = NULL;

		//m_bCaptured = FALSE;
		m_pSelectTool = new CSelectTool();
		m_tools.push_back(m_pSelectTool);
		m_tools.push_back(new CRectTool(ttRect));
		m_tools.push_back(new CRectTool(ttText));
	}
	virtual ~CDrawCanvas()
	{
		m_selection.clear();
		std::for_each(m_allObjects.begin(), m_allObjects.end(), FTL::ObjecteDeleter<CDrawObject*>());
		m_allObjects.clear();

		m_pSelectTool = NULL;
		std::for_each(m_tools.begin(), m_tools.end(), FTL::ObjecteDeleter<CDrawTool*>() );
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

	void OnLButtonDown(UINT nFlags, const CPoint& point)
	{
		//fix Win+D lost focus
		SetFocus(GetHWnd());

		BOOL bHandle = FALSE;
		if (m_pFoucsTextObject && m_pFoucsTextObject->IsActive())
		{
			MSG msg = {0};
			msg.hwnd = this->GetHWnd();
			msg.message = WM_LBUTTONDOWN;
			msg.wParam = (WPARAM)nFlags;
			msg.lParam = MAKELPARAM(point.x, point.y);
			GetCursorPos(&(msg.pt));
			if(m_pFoucsTextObject->PreTranslateMessage(&msg))
			{
				bHandle = TRUE;
			}
		}
		if (!bHandle)
		{
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
	}

	void OnMouseMove(UINT nFlags, const CPoint& point)
	{
		BOOL bHandled = FALSE;
		if (m_pFoucsTextObject && m_pFoucsTextObject->IsActive())
		{
			MSG msg = {0};
			msg.hwnd = this->GetHWnd();
			msg.message = WM_MOUSEMOVE;
			msg.wParam = (WPARAM)nFlags;
			msg.lParam = MAKELPARAM(point.x, point.y);
			GetCursorPos(&(msg.pt));
			if(m_pFoucsTextObject->PreTranslateMessage(&msg))
			{
				bHandled = TRUE;
			}
		}
		if (!bHandled)
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
	}

	void OnCancelMode()
	{
		CDrawTool* pTool = GetCurrentTool();
		if (pTool)
		{
			pTool->OnCancel(this);
		}
	}

	void OnLButtonUp(UINT nFlags, const CPoint& point)
	{
		BOOL bHandled = FALSE;
		if (m_pFoucsTextObject && m_pFoucsTextObject->IsActive())
		{
			MSG msg = {0};
			msg.hwnd = this->GetHWnd();
			msg.message = WM_LBUTTONUP;
			msg.wParam = (WPARAM)nFlags;
			msg.lParam = MAKELPARAM(point.x, point.y);
			GetCursorPos(&(msg.pt));
			if(m_pFoucsTextObject->PreTranslateMessage(&msg))
			{
				bHandled = TRUE;
			}
		}

		if (!bHandled)
		{
			CDrawTool* pTool = GetCurrentTool();
			if (pTool)
			{
				if (_IsValidateScreenPoint(point))
				{
					pTool->OnLButtonUp(this, nFlags, point);
				}
				//else
				//{
				//	pTool->OnLButtonUp(this, nFlags, m_ptLastValidateDevice);
				//}
			}
		}
	}

	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if (VK_ESCAPE == nChar)
		{
			this->Select(NULL, FALSE);
			this->m_pFoucsTextObject = NULL;
		}
		CDrawTool* pTool = GetCurrentTool();
		if (pTool)
		{
			pTool->OnKeyDown(this, nChar, nRepCnt, nFlags);
		}
	}

	void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		CDrawTool* pTool = GetCurrentTool();
		if (pTool)
		{
			pTool->OnChar(this, nChar, nRepCnt, nFlags);
		}
	}

	//BOOL OnSetCursor(HWND hWnd, UINT nHitTest, UINT message)
	//{
	//	CDrawTool* pTool = GetCurrentTool();
	//	if (pTool)
	//	{
	//		return pTool->OnSetCursor(hWnd, nHitTest, message);
	//	}
	//	return FALSE;
	//}

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

	virtual CSelectTool* GetSelectTool()
	{
		FTLASSERT(m_pSelectTool);
		return m_pSelectTool;
	}

	virtual ToolType   GetCurrentToolType()
	{
		return m_nCurToolType;
	}

	virtual VOID SetCurrentToolType(ToolType nToolType)
	{
		if (m_nCurToolType == ttSelection && nToolType != ttSelection)
		{
			ReleaseSelectRect();
		}
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

	virtual BOOL CreateSelectRect(const CRect& rcPosition, BOOL bSelect = TRUE)
	{
		FTLASSERT(NULL == m_pSelectRect);
		//FTLTRACE(TEXT("CreateSelectRect, pos=[%d,%d]x[%d,%d]\n"),
		//	rcPosition.left, rcPosition.top, rcPosition.right, rcPosition.bottom);

		ReleaseSelectRect();
		m_pSelectRect = new CDrawRect(this, rcPosition, dotSelectRect);
		Add(m_pSelectRect);
		if (bSelect)
		{
			Select(m_pSelectRect, TRUE);
		}
		InvalObject(m_pSelectRect);
		return TRUE;
	}
	virtual BOOL ReleaseSelectRect()
	{
		if (m_pSelectRect)
		{
			//CRect rcSelect = m_pSelectRect->GetPosition();
			//FTLTRACE(TEXT("ReleaseSelectRect For:[%d,%d]x[%d,%d]\n"), rcSelect.left,
			//	rcSelect.top, rcSelect.right, rcSelect.bottom);

			InvalObject(m_pSelectRect);
			Remove(m_pSelectRect);
			SAFE_DELETE(m_pSelectRect);
		}
		return TRUE;
	}
	virtual CDrawRect* GetSelectRect()
	{
		return m_pSelectRect;
	}

	virtual void Add(CDrawObject* pObj)
	{
		m_allObjects.push_back(pObj);
	}

	virtual void Remove(CDrawObject* pObj)
	{
		if (m_pFoucsTextObject == pObj)
		{
			m_pFoucsTextObject = NULL;
		}

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
	}

	void InvalObject(CDrawObject* pObj)
	{
		CRect rect = pObj->GetPosition();
		rect.NormalizeRect();
		rect.InflateRect(1, 1);

		DocToClient(&rect);
		rect.InflateRect(TRACK_MARGIN, TRACK_MARGIN);
		//if (IsSelected(pObj)) //m_bActive && 
		//{
		//	rect.left -= 4;
		//	rect.top -= 5;
		//	rect.right += 5;
		//	rect.bottom += 4;
		//}
		//rect.InflateRect(1, 1); // handles CDrawOleObj objects
		//rect.InflateRect(4, 4); // handles CDrawOleObj objects

		T* pThis = static_cast<T*>(this);
		//pThis->Invalidate();
		::InvalidateRect(pThis->m_hWnd, rect, FALSE);
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
		if (pObj->GetDrawObjType() == dotText)
		{
			m_pFoucsTextObject = dynamic_cast<CTextObject*>(pObj);
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
		if (::GetCapture() == pt->m_hWnd)
		{
			bRet = TRUE;
		}
		return bRet;
	}
	void BeginCapture() 
	{
		//m_bCaptured = TRUE;
		T* pt = static_cast<T*>(this);
		pt->SetCapture();
	}
	void EndCapture()
	{
		//m_bCaptured = FALSE;
		FTLASSERT(IsCapture());
		BOOL bRet = FALSE;
		API_VERIFY(::ReleaseCapture());
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
		for (DrawObjectList::iterator iter = m_allObjects.begin();
			iter != m_allObjects.end();
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

	virtual void DeleteSelectObjects()
	{
		for (DrawObjectList::iterator iter = m_selection.begin(); iter != m_selection.end(); ++iter)
		{
			CDrawObject* pObj = *iter;
			DrawObjectList::iterator iterInAll = std::find(m_allObjects.begin(), m_allObjects.end(), pObj);
			
			if (m_pFoucsTextObject == pObj)
			{
				m_pFoucsTextObject = NULL;
			}

			FTLASSERT(iterInAll != m_allObjects.end());
			if (iterInAll != m_allObjects.end())
			{
				m_allObjects.erase(iterInAll);
			}
			InvalObject(pObj);

			//delete Object self
			pObj->Remove();
		}
		m_selection.clear();
	}

	//draw info(object/select tracker) on device unit, this will not be effect by ZoomIn or ZoomOut
	//TODO: position ?
	void DrawDeviceInfo(HDC hDC)
	{
		for (DrawObjectList::iterator iter = m_allObjects.begin();
			iter != m_allObjects.end();
			++iter)
		{
			CDrawObject* pObj = *iter;
			//select object -- draw on device 
			if (pObj->GetDrawObjType() == dotSelectRect)
			{
				pObj->Draw(hDC, FALSE);
			}

			//select tracker will draw on device
			if (IsSelected(pObj))
			{
				pObj->DrawTracker(hDC, CDrawObject::selected, TRUE);
			}
		}
	}

	//draw info(objects) on logical unit, will be effect by ZoomIn and ZoomOut
	void DrawLogicalInfo(HDC hDC)
	{
		for (DrawObjectList::iterator iter = m_allObjects.begin();
			iter != m_allObjects.end();
			++iter)
		{
			CDrawObject* pObj = *iter;
			//normal object -- draw on logical device
			if (pObj->GetDrawObjType() != dotSelectRect)
			{
				pObj->Draw(hDC, FALSE);
			}
		}
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
};