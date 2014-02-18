// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CMainFrame : 
	public CMDIFrameWindowImpl<CMainFrame>, 
	public CUpdateUI<CMainFrame>,
	public CMessageFilter, public CIdleHandler
{
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

	CMDICommandBarCtrl m_CmdBar;

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_DESTROY(OnDestroy)
		COMMAND_ID_HANDLER_EX(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER_EX(ID_FILE_NEW, OnFileNew)
        COMMAND_ID_HANDLER_EX(ID_FILE_OPEN, OnFileOpen)
		COMMAND_ID_HANDLER_EX(ID_VIEW_TOOLBAR, OnViewToolBar)
		COMMAND_ID_HANDLER_EX(ID_VIEW_STATUS_BAR, OnViewStatusBar)
		COMMAND_ID_HANDLER_EX(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER_EX(ID_WINDOW_CASCADE, OnWindowCascade)
		COMMAND_ID_HANDLER_EX(ID_WINDOW_TILE_HORZ, OnWindowTile)
		COMMAND_ID_HANDLER_EX(ID_WINDOW_ARRANGE, OnWindowArrangeIcons)
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CMDIFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()

	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnDestroy();
    void OnFileExit(UINT uNotifyCode, int nID, CWindow wndCtl);
    void OnFileNew(UINT uNotifyCode, int nID, CWindow wndCtl);
    void OnFileOpen(UINT uNotifyCode, int nID, CWindow wndCtl);

    void OnViewToolBar(UINT uNotifyCode, int nID, CWindow wndCtl);
    void OnViewStatusBar(UINT uNotifyCode, int nID, CWindow wndCtl);
    void OnAppAbout(UINT uNotifyCode, int nID, CWindow wndCtl);
    void OnWindowCascade(UINT uNotifyCode, int nID, CWindow wndCtl);
    void OnWindowTile(UINT uNotifyCode, int nID, CWindow wndCtl);
    void OnWindowArrangeIcons(UINT uNotifyCode, int nID, CWindow wndCtl);
};
