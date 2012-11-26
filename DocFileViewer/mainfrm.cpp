// MainFrm.cpp : implmentation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "ChildFrm.h"
#include "MainFrm.h"

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
	return FALSE;
}

LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// create command bar window
	HWND hWndCmdBar = m_wndCmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
	// attach menu
	m_wndCmdBar.AttachMenu(GetMenu());
	// load command bar images
	m_wndCmdBar.LoadImages(IDR_MAINFRAME);
	// remove old menu
	SetMenu(NULL);

	HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndCmdBar);
	AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

	CreateSimpleStatusBar();

	CreateMDIClient();
	m_wndCmdBar.SetMDIClient(m_hWndMDIClient);

	UIAddToolBar(hWndToolBar);
	UISetCheck(ID_VIEW_TOOLBAR, 1);
	UISetCheck(ID_VIEW_STATUS_BAR, 1); 

    { 
        // Init MRU list 
        CMenuHandle mainMenu = m_wndCmdBar.GetMenu(); 
        CMenuHandle fileMenu = mainMenu.GetSubMenu(0); 

        m_mru.SetMaxEntries(MRU_MAX_ENTRIES); 
        m_mru.SetMenuHandle ( fileMenu ); 
        m_mru.ReadFromRegistry ( APP_SETTINGS_KEY ); 
    } 

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

    DragAcceptFiles(); 

	return 0;
} 

LRESULT CMainFrame::OnWmDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) 
{ 
    uMsg, wParam, lParam, bHandled; 

    // Save the MRU 
    m_mru.WriteToRegistry ( APP_SETTINGS_KEY ); 

    bHandled = FALSE; 
    return S_OK; 
} 


LRESULT CMainFrame::OnWmMDISetMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) 
{ 
    LRESULT lReturn = CMDIFrameWindowImpl<CMainFrame>::OnMDISetMenu(uMsg, wParam, lParam, bHandled); 

    { 
        // Init MRU list 
        CMenuHandle mainMenu = m_wndCmdBar.GetMenu(); 
        CMenuHandle fileMenu = mainMenu.GetSubMenu(0); 

        m_mru.SetMenuHandle ( fileMenu ); 
        m_mru.ReadFromRegistry( APP_SETTINGS_KEY ); 
    } 

    bHandled = TRUE; 
    return lReturn; 
} 

LRESULT CMainFrame::OnCmdMRUMenuItem(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) 
{ 
    wNotifyCode, wID, hWndCtl, bHandled; 

    TCHAR sFile[MAX_PATH] = { 0 }; 
    // Read the selected filename from the MRU list, and open that file.
    if ( m_mru.GetFromList ( wID, sFile, _countof(sFile) ) ) 
    { 
        return _DoOpenFile(sFile, wID); 
    } 

    bHandled = FALSE; 
    return FALSE; 
} 

LRESULT CMainFrame::OnWmDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) 
{ 
    uMsg, wParam, lParam, bHandled; 

    HDROP hDrop = (HDROP)wParam;

    if (IsIconic()) 
        ShowWindow(SW_RESTORE); 

    ::SetForegroundWindow(m_hWnd); 

    TCHAR szFile[MAX_PATH] = { 0 }; 

    // Determine how many files are involved in this operation. 
    UINT uNumFiles = DragQueryFile ( hDrop, 0xFFFFFFFF, NULL, 0 ); 

    for ( UINT uFile = 0; uFile < uNumFiles; uFile++ ) 
    { 
        // Get the next filename. 
        if ( DragQueryFile ( hDrop, uFile, szFile, _countof(szFile) )) { 
            _DoOpenFile(szFile); 
        } 
    } // end for 

    DragFinish(hDrop); 

    bHandled = FALSE; 
    return S_OK; 
} 

LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PostMessage(WM_CLOSE);
	return 0;
}


LRESULT CMainFrame::OnFileOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{ 
    TCHAR szFilter[MAX_PATH] = { 0 }; 
    TCHAR szFileName[MAX_PATH] = { 0 }; 
    
    { 
        // assembling filter string 
        TCHAR szAllFiles[MAX_PATH] = { 0 }; 
        AtlLoadString(IDS_ALL_FILES, szAllFiles, _countof(szAllFiles)); 
        
        wnsprintf(szFilter, _countof(szFilter), _T("%s(*.*)|*.*||"), szAllFiles); 
    } 
    
    CModifiedFileDialog fd ( TRUE, _T(""), szFileName, OFN_HIDEREADONLY, szFilter, m_hWnd ); 
    
    if ( IDOK != fd.DoModal(this->m_hWnd) ) 
        return 0; 
    
    _DoOpenFile(fd.m_szFileName); 
    
    return 0;
}


LRESULT CMainFrame::OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	static BOOL bVisible = TRUE;	// initially visible
	bVisible = !bVisible;
	CReBarCtrl rebar = m_hWndToolBar;
	int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
	rebar.ShowBand(nBandIndex, bVisible);
	UISetCheck(ID_VIEW_TOOLBAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
	::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
	UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainFrame::OnWindowCascade(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	MDICascade();
	return 0;
}

LRESULT CMainFrame::OnWindowTile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	MDITile();
	return 0;
}

LRESULT CMainFrame::OnWindowArrangeIcons(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	MDIIconArrange();
	return 0;
}

BOOL CMainFrame::_DoOpenFile(LPCTSTR lpszFileName, int nMRUID) 
{ 
    USES_CONVERSION; 
    BOOL bReturn = FALSE; 
    if ( ::PathFileExists(lpszFileName) ) 
    { 
        DWORD dwAttr = ::GetFileAttributes(lpszFileName); 
        if(dwAttr&FILE_ATTRIBUTE_DIRECTORY) 
            return bReturn; 

        bool bFound = false; 
        CChildFrame * pTemp = NULL; 
        std::map<HWND, CChildFrame*>::iterator it; 
        for (it=m_mapChildFrame.begin(); it!=m_mapChildFrame.end(); it++) 
        { 
            if( ::IsWindow(it->first) ) 
            { 
                pTemp = it->second; 
                if(lstrcmpi(lpszFileName, pTemp->m_szFileName) == 0) { 
                    bFound = true; 
                    break; 
                } 
            } 
        } 

        if(bFound) { 
            if(pTemp) { 
                MDIActivate(pTemp->m_hWnd); 
            } 
            return TRUE; 
        } 

        { 
            HRESULT hr = ::StgIsStorageFile( T2COLE(lpszFileName) ); 
            if( S_OK != hr ) 
            { 
                TCHAR szFmt[MAX_PATH] = { 0 }; 
                AtlLoadString(IDS_OPEN_ARCHIVE_ERROR, szFmt, _countof(szFmt)); 
                
                TCHAR szOut[MAX_PATH*2] = { 0 }; 
                wnsprintf(szOut, _countof(szOut), szFmt, lpszFileName); 
                
                AtlMessageBox(m_hWnd, szOut, IDS_ERROR, MB_OK|MB_ICONSTOP); 
                return 0;
            } 
        } 
        
        CChildFrame* pChild = new CChildFrame((CUpdateUIBase*)this); 
        pChild->SetFileName(lpszFileName); 
        pChild->CreateEx(m_hWndMDIClient, rcDefault, NULL, 0, WS_EX_CLIENTEDGE); 

        m_mapChildFrame[pChild->m_hWnd] = pChild; 
        
        // If this file was already in the MRU list, move it to the top of the list. 
        // Otherwise, add it to the list. 
        if ( 0 == nMRUID ) { 
            m_mru.AddToList ( lpszFileName ); 
        } else { 
            m_mru.MoveToTop ( nMRUID ); 
        } 
        bReturn = TRUE; 
    } 
    else 
    { 
        // We couldn't read the contents of this file, 
        // so remove it from the MRU list if it was in there. 
        if ( 0 != nMRUID ) 
            m_mru.RemoveFromList ( nMRUID ); 
    } 
    m_mru.WriteToRegistry(APP_SETTINGS_KEY); 
    return bReturn; 
} 
