// MainFrm.cpp : implmentation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MapMakerView.h"
#include "ChildFrm.h"
#include "MainFrm.h"
#include "OptionsDlg.h"

CMainFrame* g_pMainFrame;

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if(CMDIFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
		return TRUE;

	HWND hWnd = MDIGetActive();
	if(hWnd != NULL)
		return (BOOL)::SendMessage(hWnd, WM_FORWARDMSG, 0, (LPARAM)pMsg);

	return FALSE;
}

BOOL CMainFrame::OnIdle()
{
	UIUpdateToolBar();
    //UISetText ( 1, m_strPromptInfo);
    UIUpdateStatusBar();

	return FALSE;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    g_pMainFrame = this;

	// create command bar window
	HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
	// attach menu
	m_CmdBar.AttachMenu(GetMenu());
	// load command bar images
	m_CmdBar.LoadImages(IDR_MAINFRAME);
	// remove old menu
	SetMenu(NULL);

	HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndCmdBar);
	AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

	//CreateSimpleStatusBar();
    int anPanes[] = { ID_DEFAULT_PANE, ID_PANE_TILE_GRID };//, IDPANE_STATUS, IDPANE_CAPS_INDICATOR };

    m_hWndStatusBar = m_wndStatusBar.Create ( *this );
    UIAddStatusBar ( m_hWndStatusBar );
    m_wndStatusBar.SetPanes(anPanes, _countof(anPanes), false);

	CreateMDIClient();
	m_CmdBar.SetMDIClient(m_hWndMDIClient);

	UIAddToolBar(hWndToolBar);
	UISetCheck(ID_VIEW_TOOLBAR, 1);
	UISetCheck(ID_VIEW_STATUS_BAR, 1);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	return 0;
}

void CMainFrame::OnDestroy()
{
		m_CmdBar.AttachMenu(NULL);

	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

    SetMsgHandled(FALSE);
	//bHandled = FALSE;
}

void CMainFrame::OnFileExit(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	PostMessage(WM_CLOSE);
}

void CMainFrame::OnFileNew(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	CChildFrame* pChild = new CChildFrame;
	pChild->CreateEx(m_hWndClient);

	// TODO: add code to initialize document
}

void CMainFrame::OnFileOpen(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    CString strFilter = _T("ImageFile(*.bmp;*.jpg;*.jpeg;*.png)|*.bmp;*.jpg;*.jpeg;*.png|All Files(*.*)|*.*||");
    strFilter.Replace(TEXT('|'), TEXT('\0'));

    CFileDialog dlgOpen(TRUE, NULL, NULL, OFN_PATHMUSTEXIST, strFilter, m_hWnd);

    if (dlgOpen.DoModal() == IDOK)
    {
        CChildFrame* pChild = new CChildFrame;
        pChild->CreateEx(m_hWndClient);
        pChild->SetImagePath(dlgOpen.m_szFileName);
    }
}


void CMainFrame::OnViewToolBar(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	static BOOL bVisible = TRUE;	// initially visible
	bVisible = !bVisible;
	CReBarCtrl rebar = m_hWndToolBar;
	int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
	rebar.ShowBand(nBandIndex, bVisible);
	UISetCheck(ID_VIEW_TOOLBAR, bVisible);
	UpdateLayout();
}

void CMainFrame::OnViewStatusBar(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
	::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
	UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
	UpdateLayout();
}

void CMainFrame::OnAppAbout(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	CAboutDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnWindowCascade(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	MDICascade();
}

void CMainFrame::OnWindowTile(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	MDITile();
}

void CMainFrame::OnWindowArrangeIcons(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	MDIIconArrange();
}

void CMainFrame::SetPromptInfo(const CString& strPromptInfo)
{
    m_strPromptInfo = strPromptInfo;
    UISetText(1, m_strPromptInfo);
    UIUpdateStatusBar(TRUE);
}