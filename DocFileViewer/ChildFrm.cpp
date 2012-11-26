// ChildFrm.cpp : implementation of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "mainfrm.h" 
#include "ChildFrm.h" 

CChildFrame::CChildFrame(CUpdateUIBase* pUpdate) 
:m_pUpdateUI(pUpdate) 
{ 
} 


void CChildFrame::OnFinalMessage(HWND /*hWnd*/)
{
    delete this;
} 

void CChildFrame::SetFileName(LPCTSTR lpszFileName) 
{ 
    lstrcpyn(m_szFileName, lpszFileName, _countof(m_szFileName)); 
} 


BOOL CChildFrame::OpenCompositeDocument(LPCTSTR lpszFileName) 
{ 
    USES_CONVERSION; 

    CWaitCursor wait; 

    HRESULT hResult = E_FAIL; 
    IStorage * ptrRootStg = NULL; // root storage 
    HWND hWnd = (HWND)m_hWnd; 

    hResult = ::StgIsStorageFile( T2COLE(m_szFileName) ); 
    if( S_OK != hResult ) 
    { 
        TCHAR szFmt[MAX_PATH] = { 0 }; 
        AtlLoadString(IDS_OPEN_ARCHIVE_ERROR, szFmt, _countof(szFmt)); 

        TCHAR szOut[MAX_PATH*2] = { 0 }; 
        wnsprintf(szOut, _countof(szOut), szFmt, lpszFileName); 

        AtlMessageBox(m_hWnd, szOut, IDS_ERROR, MB_OK|MB_ICONSTOP); 
        return FALSE; 
    }

    // open the Compound document 
    hResult = ::StgOpenStorage( T2COLE(m_szFileName), NULL, 
        STGM_READWRITE | STGM_SHARE_EXCLUSIVE, 
        0, 0, &ptrRootStg ); 
    if( FAILED(hResult) )
    { 
        TCHAR szFmt[MAX_PATH] = { 0 }; 
        AtlLoadString(IDS_OPEN_FILE_ERROR, szFmt, _countof(szFmt)); 

        TCHAR szError[MAX_PATH] = { 0 }; 

        TCHAR szOut[MAX_PATH*2] = { 0 }; 
        wnsprintf(szOut, _countof(szOut), szFmt, 
            lpszFileName, GetErrorString(S_OK, szError, _countof(szError))); 

        AtlMessageBox(m_hWnd, szOut, IDS_ERROR, MB_OK|MB_ICONSTOP); 
        return FALSE;
    } 

    TREE_ITEM_DATA * pRootData = new TREE_ITEM_DATA(ptrRootStg, STGTY_STORAGE); 

    HTREEITEM hRoot = m_wndCatalog.InsertItem ( 
        TVIF_IMAGE|TVIF_TEXT|TVIF_PARAM|TVIF_SELECTEDIMAGE, 
        PathFindFileName(m_szFileName), 
        0, 1, TVIS_EXPANDED, 0, 
        reinterpret_cast<LPARAM>(pRootData), 
        NULL, NULL); 


    std::stack < StgInfo > FolderStack; 
    HTREEITEM htiParent = hRoot; 
    IEnumSTATSTG * ptrEnum = NULL; 

    hResult = ptrRootStg->EnumElements( 0, NULL, 0, &ptrEnum ); 
    if( FAILED(hResult) ) { 
        ptrRootStg->Release(); 
        return 0; 
    } 

    TCHAR szSwap[MAX_PATH] = { 0 }; 

	LARGE_INTEGER nStorageLength;
	nStorageLength.QuadPart = 0;
    STATSTG StatStg = { 0 }; 
    while( S_OK == hResult ) 
    { 
        hResult = ptrEnum->Next( 1, &StatStg, NULL ); 
        if( S_FALSE == hResult ) 
        { 
            ptrRootStg->Release(); 
            ptrEnum->Release(); 

            // m_wndCatalog.Expand(strFolder); 

            if( !FolderStack.empty() ) 
            { 
				TCHAR szCurText[MAX_PATH] = {0};
				m_wndCatalog.GetItemText(htiParent, szCurText, _countof(szCurText));
				CString strTotal;
				strTotal.Format(TEXT("%s(%ld)"), szCurText, nStorageLength);
				m_wndCatalog.SetItemText(htiParent, strTotal);

				ptrRootStg  = FolderStack.top().pStg; 
                ptrEnum     = FolderStack.top().pEnum; 
                htiParent   = FolderStack.top().hParent; 
                FolderStack.pop(); 

                hResult = S_OK; 
            } 

            continue; 
        } 

        switch(StatStg.type) 
        { 
        case STGTY_STORAGE: // 是存储对象, "文件夹" 
            { 
				//先清零
				nStorageLength.QuadPart = 0;
                IStorage * ptrChildStg = NULL;

                HRESULT hr = ptrRootStg->OpenStorage( StatStg.pwcsName, NULL, 
                    STGM_READWRITE|STGM_SHARE_EXCLUSIVE, 
                    0, 0, &ptrChildStg ); 
                if( SUCCEEDED(hr) ) 
                { 
                    TREE_ITEM_DATA * pData = new TREE_ITEM_DATA(ptrChildStg, StatStg.type); 

                    HTREEITEM hFolder = m_wndCatalog.InsertItem ( 
                        TVIF_IMAGE|TVIF_TEXT|TVIF_PARAM|TVIF_SELECTEDIMAGE, 
                        WCHAR2TCHAR(StatStg.pwcsName, szSwap, _countof(szSwap)), 
                        0, 1, TVIS_EXPANDED, 0, 
                        reinterpret_cast<LPARAM>(pData), 
                        htiParent, NULL); 

                    // 父存储入栈 
                    FolderStack.push( StgInfo(ptrRootStg, ptrEnum, htiParent) );

                    // 子存储替代父存储 
                    ptrRootStg = ptrChildStg; 
                    htiParent = hFolder; 
                    hr = ptrChildStg->EnumElements( 0, NULL, 0, &ptrEnum ); 
                } 
            } 
            break; 
        case STGTY_STREAM:  // 是流, "文件" 
            { 
                CComPtr<IStream> spStream; 
                HRESULT hr = ptrRootStg->OpenStream(StatStg.pwcsName, NULL, 
                    STGM_READWRITE|STGM_SHARE_EXCLUSIVE, 0, &spStream); 
                ATLASSERT(SUCCEEDED(hr)); 
				
				LARGE_INTEGER nSeekPos;
				nSeekPos.QuadPart = 0LL;
				ULARGE_INTEGER nLength = {0};
				hr = spStream->Seek(nSeekPos, STREAM_SEEK_END, &nLength);
				CString strStreamName;
				WCHAR2TCHAR(StatStg.pwcsName, szSwap, _countof(szSwap));
				if (SUCCEEDED(hr))
				{
					nStorageLength.QuadPart += nLength.QuadPart;
					strStreamName.Format(TEXT("%s(%ld)"), szSwap, nLength);
				}
				else
				{
					strStreamName.Format(TEXT("%s(0)"), szSwap);
				}
                TREE_ITEM_DATA * pData = new TREE_ITEM_DATA(spStream, StatStg.type); 
                m_wndCatalog.InsertItem ( 
                    TVIF_IMAGE|TVIF_TEXT|TVIF_PARAM|TVIF_SELECTEDIMAGE, 
                    strStreamName, 
                    2, 2, TVIS_EXPANDED, 0, 
                    reinterpret_cast<LPARAM>(pData), 
                    htiParent, NULL); 
            } 
            break; 
        case STGTY_LOCKBYTES: 
            ATLTRACE(_T("===== STGTY_LOCKBYTES %d ====="), StatStg.type); 
            break; 
        case STGTY_PROPERTY: 
            ATLTRACE(_T("===== STGTY_PROPERTY %d ====="), StatStg.type); 
            break; 
        default: 
            ATLASSERT(!_T("Unknown storage type!!!")); 
            break; 
        } 

        ::CoTaskMemFree( StatStg.pwcsName ); // 释放名称所使用的内存 
    }

    return 0;
}

LRESULT CChildFrame::OnForwardMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) 
{
    LPMSG pMsg = (LPMSG)lParam;

    if (CMDIChildWindowImpl<CChildFrame>::PreTranslateMessage(pMsg) ) 
        return TRUE; 

    CWindow wndActive = GetSplitterPane(SPLIT_PANE_RIGHT); 
    if (wndActive.IsWindow() ) 
    { 
        if ((wndActive == m_wndHexEdit) && m_wndHexEdit.PreTranslateMessage(pMsg) ) 
        { 
            return TRUE; 
        } 
    } 

    return FALSE; 
}

LRESULT CChildFrame::OnEraseBackground(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) 
{ 
    // handled, no background painting needed 
    return 1; 
} 

LRESULT CChildFrame::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{ 
    m_wndCatalog.Create(m_hWnd, rcDefault, NULL, 0, 0, IDC_ORGANIZE); 

    CImageList imageList; 
    // normal tree images 
    imageList.Create(IDB_TYPE, 16, 1, RGB( 255, 0 ,255) ); 
    m_wndCatalog.SetImageList(imageList, TVSIL_NORMAL); // TVSIL_STATE 
    imageList.Detach(); 

    m_wndHexEdit.Create(m_hWnd, rcDefault, NULL, 0, 0, IDC_HEXED_HOST, (LPVOID)this); 
    m_wndHexEdit.SetReadOnly(TRUE); 

    m_wndFileDetails.Create(m_hWnd, rcDefault); 

    SetSplitterPanes(m_wndCatalog, m_wndFileDetails); 

    SetWindowText(m_szFileName); 
    PostMessage(WM_CUSTOM_POSTCREATE); 
    return 0;
}

LRESULT CChildFrame::OnCustomPostCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{ 
    SetSplitterPos(300); 

    OpenCompositeDocument(m_szFileName); 
    return 0;
} 

LRESULT CChildFrame::OnCustomActiveItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{ 
    m_wndCatalog.SelectItem((HTREEITEM)wParam); 
    m_wndCatalog.SetFocus(); 

    return 0; 
} 

LRESULT CChildFrame::OnContextMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{ 
    uMsg, wParam, lParam, bHandled; 

    bHandled = FALSE; 

    POINT ptScr = { 0 }; 

    ptScr.x = GET_X_LPARAM(lParam); 
    ptScr.y = GET_Y_LPARAM(lParam); 

    { 
        POINT ptLoc = ptScr; 

        DWORD dwStyle = m_wndCatalog.GetStyle(); 
        if(WS_VISIBLE != (dwStyle & WS_VISIBLE)) 
            return S_OK; 

        RECT rc; 
        m_wndCatalog.GetClientRect(&rc); 

        // 将屏幕坐标转换成指定窗口的相对坐标 
        m_wndCatalog.ScreenToClient(&ptLoc); 

        // 鼠标指针是否在 TreeView 窗口内 
        if (::PtInRect(&rc, ptLoc)) 
        { 
            UINT flags = 0;
            m_pData = m_wndCatalog.HitTest(ptLoc, &flags); 

            if (FALSE == m_pData.IsNull()) 
            { 
				TREE_ITEM_DATA * pTmp = (TREE_ITEM_DATA *)m_pData.GetData();

                CMenuHandle mnuPopUp; 
                CMenu mnuTemp; 
                
                mnuTemp.LoadMenu(IDR_MENU_EXPORT); 
                mnuPopUp = mnuTemp.GetSubMenu(0); 
				if (pTmp->dwStgType == STGTY_STORAGE)
				{
					mnuPopUp.EnableMenuItem(ID_FILE_ADD_STG, MF_BYCOMMAND|MF_ENABLED);
					mnuPopUp.EnableMenuItem(ID_FILE_ADD_STM, MF_BYCOMMAND|MF_ENABLED);
				}

                mnuPopUp.TrackPopupMenu(0, ptScr.x, ptScr.y, *this); 
                return 0; 
            } 
        } 
    } 

    return 0; 
} 

LRESULT CChildFrame::OnWmMDIActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) 
{ 
    LRESULT lReturn = CMDIChildWindowImpl<CChildFrame>::OnMDIActivate(uMsg, wParam, lParam, bHandled); 
    
    if ((m_hWnd == (HWND)lParam) && 
        (NULL != m_pUpdateUI) && 
        TRUE) 
    { 
        CMainFrame * pMainFrame = static_cast<CMainFrame *>(m_pUpdateUI); 
        if (pMainFrame) 
        { 
            // Init MRU list 
            CMenuHandle mainMenu = pMainFrame->m_wndCmdBar.GetMenu(); 
            CMenuHandle fileMenu = mainMenu.GetSubMenu(0); 
            
            pMainFrame->m_mru.SetMenuHandle ( fileMenu ); 
            pMainFrame->m_mru.ReadFromRegistry( APP_SETTINGS_KEY ); 
        } 
    } 
    
    bHandled = FALSE; 
    return lReturn; 
} 

HRESULT CChildFrame::_RecursionExtractData(LPCTSTR lpszRoot, HTREEITEM hCurrItem, CTreeViewCtrlEx * pTreeView) 
{ 
    HRESULT hr = S_OK; 
    CTreeItem tiCurr(hCurrItem, pTreeView); 
    TREE_ITEM_DATA * pTmp = (TREE_ITEM_DATA *)tiCurr.GetData(); 
    if (NULL == pTmp) { 
        return hr; 
    } 

    TCHAR szPathCopy[MAX_PATH] = { 0 }; 
    lstrcpyn(szPathCopy, lpszRoot, _countof(szPathCopy)); 

    PathAddBackslash(szPathCopy); 

    TCHAR szCurrName[MAX_PATH/2] = { 0 }; 
    tiCurr.GetText(szCurrName, _countof(szCurrName)); 
    AdjustFileName(szCurrName); 
    
    if (STGTY_STORAGE == pTmp->dwStgType) 
    { 
        TCHAR *pIter = szCurrName; 
        while (*pIter && pIter<szCurrName+lstrlen(szCurrName)) 
        { 
            if(*pIter == '.') { *pIter = '_'; } 
            pIter ++; 
        } 

        TCHAR szNowPath[MAX_PATH] = { 0 }; 
        wnsprintf(szNowPath, _countof(szNowPath), _T("%s%s"), szPathCopy, szCurrName); 
        ::SHCreateDirectoryEx(*pTreeView, szNowPath, NULL); 
    } 
    else if (STGTY_STREAM == pTmp->dwStgType)
    { 
        TCHAR szFileName[MAX_PATH] = { 0 }; 
        wnsprintf(szFileName, _countof(szFileName), _T("%s%s"), szPathCopy, szCurrName); 

        CComPtr<IStream> spFile; 
        hr = pTmp->spStgOrStrm->QueryInterface(&spFile); 
        if (FAILED(hr)) return hr; 

        hr = WriteStreamToFile(szFileName, spFile); 
    } 

    lstrcat(szPathCopy, szCurrName); 

    HTREEITEM hChild = tiCurr.GetChild(); 

    while (NULL != hChild) 
    { 
        _RecursionExtractData(szPathCopy, hChild, pTreeView); 
        hChild = CTreeItem(hChild, pTreeView).GetNextSibling(); 
    } 

    return hr; 
} 

LRESULT CChildFrame::OnFileExtractData(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{ 
    HRESULT hr = E_FAIL; 

    TREE_ITEM_DATA * pTmp = (TREE_ITEM_DATA *)m_pData.GetData(); 

    if (NULL == pTmp) return 0; 

    TCHAR szText[MAX_PATH] = { 0 }; 
    m_pData.GetText(szText, _countof(szText)); 
    ATLTRACE(_T("---------%s\n"), szText); 

    if (STGTY_STREAM == pTmp->dwStgType) 
    { 
        TCHAR szFileName[MAX_PATH] = { 0 }; 

        if (NULL == _tcschr(szText, '.')) 
        { 
            _tcscat(szText, _T(".bin")); 
        } 

        AdjustFileName(szText); 

        TCHAR szFilter[MAX_PATH] = { 0 }; 
        { 
            // assembling filter string 
            TCHAR szAllFiles[MAX_PATH] = { 0 }; 
            AtlLoadString(IDS_ALL_FILES, szAllFiles, _countof(szAllFiles)); 

            wnsprintf(szFilter, _countof(szFilter), _T("%s(*.*)|*.*||"), szAllFiles); 
        } 

        CModifiedFileDialog fd ( FALSE, _T(""), szText, OFN_HIDEREADONLY, szFilter, m_hWnd ); 

        
        if ( IDOK != fd.DoModal(this->m_hWnd) ) 
            return 0; 
        
        lstrcpyn(szFileName, fd.m_szFileName, _countof(szFileName)); 

        if (PathFileExists(szFileName)) { 
            TCHAR szFmt[MAX_PATH] = { 0 }; 
            AtlLoadString(IDS_FILE_EXISTS, szFmt, _countof(szFmt)); 
            TCHAR szMsg[MAX_PATH*2] = { 0 }; 
            wnsprintf(szMsg, _countof(szMsg), szFmt, szFileName); 
            if ( IDYES != AtlMessageBox(m_hWnd, szMsg, IDS_WARNING, MB_YESNO|MB_ICONWARNING)) { 
                return 0; 
            } 
        } 

        CComPtr<IStream> spFile; 
        hr = pTmp->spStgOrStrm->QueryInterface(&spFile); 
        if (FAILED(hr)) return hr; 

        hr = WriteStreamToFile(szFileName, spFile); 
    } 

    else if (STGTY_STORAGE == pTmp->dwStgType) 
    { 
        CModifiedFolderDialog fd ( GetTopLevelParent(), IDS_SELECT_DIR, 
            BIF_RETURNONLYFSDIRS|BIF_NEWDIALOGSTYLE ); 

        TCHAR szCurrPath[MAX_PATH] = { 0 }; 
        GetCurrentDirectory(_countof(szCurrPath), szCurrPath); 

        fd.SetInitialDir( szCurrPath ); 

        if ( IDOK != fd.DoModal() ) 
            return 0; 

        lstrcpyn(szCurrPath, fd.GetFolderPath(), _countof(szCurrPath)); 
        PathAddBackslash(szCurrPath); 

        SHCreateDirectoryEx(*this, szCurrPath, NULL); 

        // recursion 
        _RecursionExtractData(szCurrPath, m_pData, &m_wndCatalog); 
    } 

    return 0; 
} 

#include "dlgstgname.h"

LRESULT CChildFrame::OnFileAddStorage(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	USES_CONVERSION;

	wNotifyCode, wID, hWndCtl, bHandled;
	HRESULT hr = E_FAIL;

	ATLTRACE(_T("CChildFrame::OnFileAddStorage\r\n"));

    TREE_ITEM_DATA * pTmp = (TREE_ITEM_DATA *)m_pData.GetData(); 
	
    if (NULL == pTmp) return 0; 

	CComPtr<IStorage> spStg; 
	hr = pTmp->spStgOrStrm->QueryInterface(&spStg);
	if (FAILED(hr)) { 
		return 0;
	}

	CDlgStgName dlg;
	if ( IDOK != dlg.DoModal()) { return 0; }
	
	CComPtr<IStorage> spNew;
	hr = spStg->CreateStorage(T2CW(dlg.m_szName),
		STGM_CREATE|STGM_WRITE|STGM_SHARE_EXCLUSIVE, 0, 0, &spNew);
	if (FAILED(hr)) {
		return 0;
	}

	TREE_ITEM_DATA * pData = new TREE_ITEM_DATA(spNew, STGTY_STORAGE); 
	
	HTREEITEM hFolder = m_wndCatalog.InsertItem ( 
		TVIF_IMAGE|TVIF_TEXT|TVIF_PARAM|TVIF_SELECTEDIMAGE, 
		dlg.m_szName, 
		0, 1, TVIS_EXPANDED, 0, 
		reinterpret_cast<LPARAM>(pData), 
		m_pData, NULL); 

	return 0;
}


#include "dlgstmname.h"

LRESULT CChildFrame::OnFileAddStream(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	wNotifyCode, wID, hWndCtl, bHandled;
	ATLTRACE(_T("CChildFrame::OnFileAddStream\r\n"));
	HRESULT hr = E_FAIL;
	TREE_ITEM_DATA * pTmp = NULL;
	LPBYTE pFileData = NULL;
	HANDLE hFile = NULL;
	
	do 
	{
		pTmp = (TREE_ITEM_DATA *)m_pData.GetData(); 
		
		if (NULL == pTmp) { break; }
		
		CComPtr<IStorage> spStg; 
		hr = pTmp->spStgOrStrm->QueryInterface(&spStg);
		if (FAILED(hr)) { 
			break;
		}
		
		CDlgStmName dlg;
		if ( IDOK != dlg.DoModal()) { break; }
		
		hFile = CreateFile(dlg.m_szFilePath, GENERIC_READ, FILE_SHARE_READ, 
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE == hFile) {
			break;
		}
		DWORD dwSize = GetFileSize(hFile, NULL);
		if (0 == dwSize) { 
			break;
		}
		pFileData = (LPBYTE) malloc(dwSize);
		if(FALSE == ReadFile(hFile, pFileData, dwSize, &dwSize, NULL))
		{ break; }
		
		CComPtr<IStream> spNew;
		DWORD dwGrfMode = STGM_CREATE|STGM_READWRITE|STGM_SHARE_EXCLUSIVE;
		hr = spStg->CreateStream(T2CW(dlg.m_szName), dwGrfMode, 0, 0, &spNew);
		if (FAILED(hr)) {
			break;
		}
		
		ULONG ulSize = 0;
		hr = spNew->Write(pFileData, dwSize, &ulSize);
		
		TREE_ITEM_DATA * pData = new TREE_ITEM_DATA(spNew, STGTY_STREAM); 
		
		HTREEITEM hFolder = m_wndCatalog.InsertItem ( 
			TVIF_IMAGE|TVIF_TEXT|TVIF_PARAM|TVIF_SELECTEDIMAGE, 
			dlg.m_szName, 
			2, 2, TVIS_EXPANDED, 0, 
			reinterpret_cast<LPARAM>(pData), 
			m_pData, NULL); 
	} while (FALSE);

	if (pFileData) { free(pFileData); }
	CloseHandle(hFile);

	return 0;
}

LRESULT CChildFrame::OnFileDelNode(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	USES_CONVERSION;

	wNotifyCode, wID, hWndCtl, bHandled;
	HRESULT hr = E_FAIL;
	ATLTRACE(_T("CChildFrame::OnFileDelNode\r\n"));

	TREE_ITEM_DATA * pTmp = NULL;
	TREE_ITEM_DATA * pParent = NULL;
	LPBYTE pFileData = NULL;
	HANDLE hFile = NULL;
	
	do 
	{
		pTmp = (TREE_ITEM_DATA *)m_pData.GetData(); 
		
		if (NULL == pTmp) { break; }

		pParent = (TREE_ITEM_DATA *) m_pData.GetParent().GetData();
		if (NULL == pParent) {
			AtlMessageBox(*this, IDS_DEL_NODE, IDS_WARNING);
			break;
		}

		if (IDOK != AtlMessageBox(*this, IDS_CONFORM, IDS_WARNING, MB_OKCANCEL))
		{ break; }

		ATLASSERT(pParent->dwStgType == STGTY_STORAGE);

		CComPtr<IStorage> spParent;
		hr = pParent->spStgOrStrm->QueryInterface(&spParent);
		if (FAILED(hr)) { break; }

		TCHAR szName[MAX_PATH] = { 0 };
		m_pData.GetText(szName, _countof(szName));
		hr = spParent->DestroyElement( T2CW(szName) );
		if (FAILED(hr)) { break; }

		m_wndCatalog.DeleteItem(m_pData.m_hTreeItem);
	} while (FALSE);
	return 0;
}


void CChildFrame::UpdateLayout(BOOL bResizeBars) 
{ 
    RECT rect = { 0 }; 
    GetClientRect(&rect); 
    // position bars and offset their dimensions 
    UpdateBarsPosition(rect, bResizeBars); 
    SetSplitterRect(&rect); 
} 

LRESULT CChildFrame::OnTvnDeleteItem(int idCtrl, LPNMHDR pNMHDR, BOOL& bHandled) 
{ 
    idCtrl, pNMHDR, bHandled; 
    NMTREEVIEW* pnmtv = (NMTREEVIEW*) pNMHDR; 

    TREE_ITEM_DATA * pTmp = (TREE_ITEM_DATA *)( pnmtv->itemOld.lParam ); 
    if( pTmp ) { 
        delete pTmp; 
    } 

    bHandled = FALSE; 

    return S_OK; 
} 

LRESULT CChildFrame::OnTvnSelChanged(int idCtrl, LPNMHDR pNMHDR, BOOL& bHandled)
{ 
    USES_CONVERSION; 
    HRESULT hr = E_FAIL; 

    idCtrl, pNMHDR, bHandled; 
    CTreeViewCtrl ctrlTree = m_wndCatalog; 

    bHandled = FALSE; 

    NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR; 
    ATLASSERT( pNMTreeView ); 

    { 
        m_pData.m_hTreeItem = pNMTreeView->itemNew.hItem; 
        m_pData.m_pTreeView = &m_wndCatalog; 
    } 

    CTreeItem hItem(pNMTreeView->itemNew.hItem, &m_wndCatalog); 

    TREE_ITEM_DATA * pCurrSel = NULL; 
    pCurrSel = (TREE_ITEM_DATA *) ctrlTree.GetItemData(hItem); 

    if (pCurrSel && STGTY_STREAM == pCurrSel->dwStgType) 
    { 
        CComPtr<IStream> spStrm; 
        hr = pCurrSel->spStgOrStrm->QueryInterface(&spStrm); 
        if (FAILED(hr)) return hr; 

        STATSTG st = { 0 }; 
        hr = spStrm->Stat(&st, STATFLAG_NONAME); 
        if (FAILED(hr)) return hr; 

        LARGE_INTEGER liBegin = { 0 }; 
        hr = spStrm->Seek(liBegin, STREAM_SEEK_SET, NULL); 

        BYTE * byBuff = new BYTE[st.cbSize.QuadPart+1]; 
        ZeroMemory(byBuff, (st.cbSize.QuadPart+1)*sizeof(BYTE)); 
        ULONG ulSize = 0; 
        hr = spStrm->Read(byBuff, st.cbSize.QuadPart, &ulSize); 

		ATLASSERT(SUCCEEDED(hr));

        if(st.cbSize.QuadPart) 
        { 
            CComSafeArray data; 
            data.CreateOneDim(VT_UI1, st.cbSize.QuadPart, (const void *)byBuff); 
            m_wndHexEdit.SetData(data); 
        } 
        else 
        {
            CComVariant data; 
            m_wndHexEdit.SetData(data); 
        }

        { 
            m_wndFileDetails.ShowWindow(SW_HIDE); 
            m_wndHexEdit.ShowWindow(SW_NORMAL); 
            SetSplitterPane(SPLIT_PANE_RIGHT, m_wndHexEdit); 
        } 

        delete [] byBuff; 
    } 
    else if (pCurrSel && STGTY_STORAGE == pCurrSel->dwStgType) 
    { 
        CComVariant data; 
        m_wndHexEdit.SetData(data); 
        m_wndHexEdit.ShowWindow(SW_HIDE); 

        std::vector<STATSTG_EX> vecFileInfo; 

        CTreeItem tiChild = hItem.GetChild(); 
        while (FALSE == tiChild.IsNull()) 
        { 
            TREE_ITEM_DATA * pData = (TREE_ITEM_DATA *)tiChild.GetData(); 

            STATSTG_EX statstgex; 

            do 
            { 
                if (pData->dwStgType == STGTY_STREAM) 
                { 
                    CComPtr<IStream> spStrm; 
                    hr = pData->spStgOrStrm->QueryInterface(&spStrm); 
                    if (FAILED(hr)) break; 
                    
                    hr = spStrm->Stat(&statstgex, STATFLAG_NONAME); 
                    if (FAILED(hr)) break; 
                } 
                else if (pData->dwStgType == STGTY_STORAGE) 
                { 
                    CComPtr<IStorage> spStg; 
                    hr = pData->spStgOrStrm->QueryInterface(&spStg); 
                    if (FAILED(hr)) break; 
                    
                    hr = spStg->Stat(&statstgex, STATFLAG_NONAME); 
                    if (FAILED(hr)) break; 
                } 
            } while(0); 
            
            statstgex.htiHost = tiChild.m_hTreeItem; 
            
            TCHAR szSwap[MAX_PATH] = { 0 }; 
            tiChild.GetText(szSwap, _countof(szSwap)); 
            TCHAR2WCHAR(szSwap, statstgex.szwName, _countof(statstgex.szwName)); 
            
            vecFileInfo.push_back(statstgex); 

            tiChild = tiChild.GetNextSibling(); 
        } 

        m_wndFileDetails.SetFileInfo(vecFileInfo); 

        m_wndFileDetails.ShowWindow(SW_NORMAL); 
        SetSplitterPane(SPLIT_PANE_RIGHT, m_wndFileDetails); 
    } 

    return 0;
} 
