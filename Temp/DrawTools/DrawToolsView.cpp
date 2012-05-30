// DrawToolsView.cpp : implementation of the CDrawToolsView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "DrawToolsView.h"

BOOL CDrawToolsView::PreTranslateMessage(MSG* pMsg)
{
	if (NULL != this->m_pFoucsTextObject && this->m_pFoucsTextObject->IsActive())
	{
		if (m_pFoucsTextObject->PreTranslateMessage(pMsg))
		{
			return TRUE;
		}
	}
	return FALSE;
}

LRESULT CDrawToolsView::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CPaintDC dc(m_hWnd);
	{
		CRect rcClip;
		GetClientRect(rcClip);
		//GetClientRect(&rcClient);

		CMemoryDC memDC(dc, rcClip);
		memDC.FillSolidRect(rcClip, RGB(0, 0, 255));
		DrawLogicalInfo(memDC);
		DrawDeviceInfo(memDC);
	}
	//TODO: Add your drawing code here

	return 0;
}

int CDrawToolsView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	SetCurrentToolType(ttText);
	SetMsgHandled(FALSE);
	return 0;
}

void CDrawToolsView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDrawCanvas<CDrawToolsView>::OnLButtonDown(nFlags, point);
	//_SetSelectRectClipInfo(point);
	SetMsgHandled(FALSE);
}

void CDrawToolsView::OnMouseMove(UINT nFlags, CPoint point)
{

	CDrawCanvas<CDrawToolsView>::OnMouseMove(nFlags, point);
	SetMsgHandled(FALSE);
}

void CDrawToolsView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CDrawCanvas<CDrawToolsView>::OnLButtonUp(nFlags, point);
	_RefreshDrawCanvasInfo();
	SetMsgHandled(FALSE);
}

void CDrawToolsView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CDrawCanvas<CDrawToolsView>::OnKeyDown(nChar, nRepCnt, nFlags);
	
	_RefreshDrawCanvasInfo();
	SetMsgHandled(FALSE);
}

void CDrawToolsView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CDrawCanvas<CDrawToolsView>::OnChar(nChar, nRepCnt, nFlags);
	SetMsgHandled(FALSE);
}

void CDrawToolsView::_RefreshDrawCanvasInfo()
{
	CString strObjectSize;
	strObjectSize.Format(TEXT("Objects:%d"), m_allObjects.size());
	GetParent().SetWindowText(strObjectSize);
}

BOOL CDrawToolsView::OnEraseBkgnd(CDCHandle dc)
{
	//SetMsgHandled(FALSE);
	return FALSE;
}