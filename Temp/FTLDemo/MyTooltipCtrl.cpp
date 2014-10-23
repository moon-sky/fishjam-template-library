// MyTooltipCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "FTLDemo.h"
#include "MyTooltipCtrl.h"
#include <ftlWindow.h>

// CMyTooltipCtrl

IMPLEMENT_DYNAMIC(CMyTooltipCtrl, CToolTipCtrl)

CMyTooltipCtrl::CMyTooltipCtrl()
{

}

CMyTooltipCtrl::~CMyTooltipCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyTooltipCtrl, CToolTipCtrl)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CMyTooltipCtrl message handlers



void CMyTooltipCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rcClient;
	GetClientRect(&rcClient);

	dc.FillSolidRect(rcClient, RGB(255, 0, 0));
}

BOOL CMyTooltipCtrl::MyAddTool(CWnd* pWnd, LPCTSTR lpszText, LPCRECT lpRectTool,
							   UINT_PTR nIDTool)
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(pWnd != NULL);
	ASSERT(lpszText != NULL);
	// the toolrect and toolid must both be zero or both valid
	ASSERT((lpRectTool != NULL && nIDTool != 0) ||
		(lpRectTool == NULL) && (nIDTool == 0));

	TOOLINFO ti;
	FillInToolInfo(ti, pWnd, nIDTool);
	ti.uFlags |= TTF_SUBCLASS;
	if (lpRectTool != NULL)
	{
		Checked::memcpy_s(&ti.rect, sizeof(RECT), lpRectTool, sizeof(RECT));
	}

	ti.lpszText = (LPTSTR)lpszText;
	return (BOOL) ::SendMessage(m_hWnd, TTM_ADDTOOL, 0, (LPARAM)&ti);
}

LRESULT CMyTooltipCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//DUMP_WINDOWS_MSG(__FILE__LINE__, NULL, 0, message, wParam, lParam);
	return CToolTipCtrl::WindowProc(message, wParam, lParam);
}
