//ReflectionWnd.cpp : Implementation of CReflectionWnd

#include "stdafx.h"
#include "ReflectionWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CReflectionWnd

CReflectionWnd::CReflectionWnd()
{
}

CReflectionWnd::~CReflectionWnd()
{
	if (IsWindow())	
	{
		DestroyWindow();
	}
}

const DWORD DEFAULT_TOOLBAR_STYLE = 
		/*Window styles:*/ WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | WS_TABSTOP |
		/*Toolbar styles:*/ TBSTYLE_TOOLTIPS | TBSTYLE_FLAT | TBSTYLE_TRANSPARENT | TBSTYLE_LIST | TBSTYLE_CUSTOMERASE |
							TBSTYLE_WRAPABLE |
		/*Common Control styles:*/ CCS_TOP | CCS_NODIVIDER | CCS_NOPARENTALIGN | CCS_NORESIZE;

LRESULT CReflectionWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	RECT rect;
	GetClientRect(&rect);
	m_ToolbarWnd.Create(m_hWnd, rect, NULL, DEFAULT_TOOLBAR_STYLE);
	return 0;
}
