// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__F2F4E2DE_34D0_4352_99C9_7B2247509478__INCLUDED_)
#define AFX_MAINFRM_H__F2F4E2DE_34D0_4352_99C9_7B2247509478__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000 

class CChildFrame; 

class CMainFrame 
    : public CMDIFrameWindowImpl<CMainFrame> 
    , public CUpdateUI<CMainFrame> 
    , public CMessageFilter 
    , public CIdleHandler 
{ 
public: 
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME) 

	CMDICommandBarCtrl m_wndCmdBar; 
    CRecentDocumentList m_mru; 

    std::map<HWND, CChildFrame *> m_mapChildFrame; 

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnWmDestroy) 
        MESSAGE_HANDLER(WM_MDISETMENU,      OnWmMDISetMenu) 
        MESSAGE_HANDLER(WM_DROPFILES,       OnWmDropFiles) 
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_FILE_OPEN_DOC, OnFileOpen)
        COMMAND_RANGE_HANDLER(ID_FILE_MRU_FIRST, ID_FILE_MRU_LAST, OnCmdMRUMenuItem) 
		COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
		COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(ID_WINDOW_CASCADE, OnWindowCascade)
		COMMAND_ID_HANDLER(ID_WINDOW_TILE_HORZ, OnWindowTile)
		COMMAND_ID_HANDLER(ID_WINDOW_ARRANGE, OnWindowArrangeIcons)
        CHAIN_MDI_CHILD_COMMANDS() 
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CMDIFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled); 
	LRESULT OnWmDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled); 
    LRESULT OnWmMDISetMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled); 
    LRESULT OnWmDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled); 
	LRESULT OnFileExit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnFileOpen(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnCmdMRUMenuItem(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled); 
	LRESULT OnViewToolBar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnViewStatusBar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnAppAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnWindowCascade(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnWindowTile(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnWindowArrangeIcons(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled); 

    BOOL _DoOpenFile(LPCTSTR lpszFileName, int nMRUID = 0); 

};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_MAINFRM_H__F2F4E2DE_34D0_4352_99C9_7B2247509478__INCLUDED_)
