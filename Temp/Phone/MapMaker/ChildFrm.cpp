// ChildFrm.cpp : implementation of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MapMakerView.h"
#include "ChildFrm.h"
#include "OptionsDlg.h"

CChildFrame::CChildFrame()
{

}

CChildFrame::~CChildFrame()
{

}

void CChildFrame::OnFinalMessage(HWND /*hWnd*/)
{
	delete this;
}

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL, WS_EX_CLIENTEDGE);
	// replace with appropriate values for the app
	m_view.SetScrollSize(1024, 768);
    
    SetMsgHandled(FALSE);
	return 1;
}

BOOL CChildFrame::OnForwardMsg(LPMSG pMsg, DWORD nUserData)
{
	if(CMDIChildWindowImpl<CChildFrame>::PreTranslateMessage(pMsg))
		return TRUE;

	return m_view.PreTranslateMessage(pMsg);
}

BOOL CChildFrame::SetImagePath(const CString& strImagePath)
{
    return m_view.SetImagePath(strImagePath);
}


void CChildFrame::OnOptinos(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    COptionsDlg dlgOptions;
    dlgOptions.m_nTileHeight = m_view.m_nGridHeight;
    dlgOptions.m_nTileWidth = m_view.m_nGridWidth;
    if (dlgOptions.DoModal() == IDOK)
    {
        m_view.SetTileParams(dlgOptions.m_nTileWidth, dlgOptions.m_nTileHeight);
    }
}

void CChildFrame::OnSelectDrawEmpty(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    m_view.SetDrawToolType(dttEmpty);
}

void CChildFrame::OnSelectDrawWall(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    m_view.SetDrawToolType(dttWall);
}
void CChildFrame::OnSelectDrawElevator(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    m_view.SetDrawToolType(dttElevator);
}