#ifndef FTL_SHELL_HPP
#define FTL_SHELL_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlShell.h"
#endif

#include "ftlConversion.h"

namespace FTL
{
#define FTL_SHELL_CHANGE_MONITOR_CLASS_NAME     TEXT("FTL_SHELL_CHANGE_MONITOR_CLASS")

    CFShellChangeMonitor::CFShellChangeMonitor()
    {
		m_pChangeObserver = NULL;
        m_hWndNotify = NULL;
        m_uiNotifyMsg = 0;
		m_uiChangeNotifyID = 0;
        m_pShellFolder = NULL;
    }

    CFShellChangeMonitor::~CFShellChangeMonitor()
    {
        Destroy();
    }

    BOOL CFShellChangeMonitor::Create(LPCTSTR pszMonitorPath /* = NULL */, 
		LONG nEvent /* = SHCNE_ALLEVENTS | SHCNE_INTERRUPT */,
		BOOL bRecursive /* = TRUE */)
    {
        BOOL bRet = FALSE;
        HRESULT hr = E_FAIL;
		if (!m_hWndNotify)
		{
			API_VERIFY(_CreateNotifyWinows());
			FTLASSERT(m_hWndNotify);
		}

		if(NULL == m_pShellFolder)
		{
        	COM_VERIFY(SHGetDesktopFolder(&m_pShellFolder));
		}
		
        m_uiNotifyMsg = RegisterWindowMessage(TEXT("FTL_SHELL_CHANGE_MONITOR"));

		LPITEMIDLIST pItemMonitor = NULL;
        if (NULL == pszMonitorPath)
        {
            COM_VERIFY(::SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOPDIRECTORY, &pItemMonitor)); //CSIDL_DESKTOP
        }
		else
		{
			//pItemMonitor = GetPidlFromPath(pszMonitorPath);
		}
		if (pItemMonitor)
		{
			SHChangeNotifyEntry notifyEntry = {0};
			notifyEntry.fRecursive = bRecursive;
			notifyEntry.pidl = pItemMonitor;

			m_uiChangeNotifyID = ::SHChangeNotifyRegister(m_hWndNotify, 
				SHCNF_IDLIST | SHCNF_TYPE, 
				nEvent,
				m_uiNotifyMsg,   
				1, 
				&notifyEntry);
			API_VERIFY(m_uiChangeNotifyID != 0);
		}


		//HRESULT hr = E_FAIL;
		//COM_VERIFY(SHGetDesktopFolder(&m_pShellFolder));
        return bRet;
    }

    BOOL CFShellChangeMonitor::Destroy()
    {
		BOOL bRet = TRUE;
		if (m_uiChangeNotifyID)
		{
			API_VERIFY(::SHChangeNotifyDeregister(m_uiChangeNotifyID));
			m_uiChangeNotifyID = 0;
		}
        if(m_hWndNotify)
        {
            API_VERIFY(::DestroyWindow(m_hWndNotify));
            m_hWndNotify = NULL;
        }
		SAFE_RELEASE(m_pShellFolder);
		return bRet;
    }

    BOOL CFShellChangeMonitor::_CreateNotifyWinows()
    {
		FTLASSERT(NULL == m_hWndNotify);

		BOOL bRet = FALSE;
		HINSTANCE hInstance = GetModuleHandle(NULL);

        WNDCLASSEX wndClass = {0};
		wndClass.cbSize = sizeof(WNDCLASSEX);

		wndClass.style			= 0; //CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc	= _MainMonitorWndProc;
		wndClass.cbClsExtra		= 0;
		wndClass.cbWndExtra		= 0;
		wndClass.hInstance		= hInstance;
		wndClass.hIcon			= NULL; //LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTWNDCLASS));
		wndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wndClass.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
		wndClass.lpszMenuName	= NULL;//MAKEINTRESOURCE(IDC_TESTWNDCLASS);
		wndClass.lpszClassName	= FTL_SHELL_CHANGE_MONITOR_CLASS_NAME;
		wndClass.hIconSm		= NULL; //LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
		
		ATOM atom = ::RegisterClassEx(&wndClass);
		API_VERIFY(atom != 0);
		if (atom != 0)
		{
			m_hWndNotify = ::CreateWindow(FTL_SHELL_CHANGE_MONITOR_CLASS_NAME, TEXT("FtlShellMonitor"), 
				WS_OVERLAPPED, -2, -2, 1, 1, NULL, NULL, hInstance, NULL);
			API_VERIFY(m_hWndNotify != NULL);
			if (m_hWndNotify)
			{
				::SetWindowLongPtr(m_hWndNotify, GWLP_USERDATA, (LONG_PTR)this);
			}
		}
		
        return bRet;
    }

    LRESULT CALLBACK CFShellChangeMonitor::_MainMonitorWndProc(HWND hwnd,UINT uMsg, 
        WPARAM wParam, LPARAM lParam)
    {
		LONG_PTR nUserData = GetWindowLongPtr(hwnd, GWLP_USERDATA);
		CFShellChangeMonitor* pThis = (CFShellChangeMonitor*)nUserData;

		if (pThis)
		{
			if (wParam && pThis->m_uiNotifyMsg == uMsg)
			{
				LRESULT nResult = 0;
				const LPCITEMIDLIST* pidls = ( LPCITEMIDLIST* )wParam;
				const LONG wEventId = ( LONG )lParam;
				LPCITEMIDLIST pIdlDst = pidls[0];
				LPCITEMIDLIST pIdlSrc = pidls[1];
				nResult = pThis->_HandleMonitorEvent(wEventId, pIdlDst, pIdlSrc);
				return nResult;
			}
		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam); 
    }
	
	LRESULT CFShellChangeMonitor::_HandleMonitorEvent(LONG wEventId, LPCITEMIDLIST pIdlDst, LPCITEMIDLIST pIdlSrc)
	{
		HRESULT hr = E_FAIL;

#ifdef FTL_DEBUG
		TCHAR szSrcPath[MAX_PATH] = {0};
		TCHAR szDstPath[MAX_PATH] = {0};
		
		if (pIdlDst)
		{
			COM_VERIFY(CFShellUtil::GetItemIdName(pIdlDst, szDstPath, _countof(szDstPath), SHGDN_FORPARSING, m_pShellFolder));
		}
		if (pIdlSrc)
		{
			COM_VERIFY(CFShellUtil::GetItemIdName(pIdlSrc, szSrcPath, _countof(szSrcPath), SHGDN_FORPARSING, m_pShellFolder));
		}
		CFStringFormater formaterChangeNotify;
		FTLTRACEEX(tlTrace, TEXT("_HandleMonitorEvent: event=%s(0x%x), srcPath=%s, dstPath=%s\n"), 
			CFShellUtil::GetShellChangeNotifyString(wEventId, formaterChangeNotify),
			wEventId, szSrcPath, szDstPath);
#endif 

		if (m_pChangeObserver)
		{
			switch (wEventId)
			{
			case SHCNE_RENAMEITEM:
				m_pChangeObserver->OnFileRename(pIdlSrc, pIdlDst);
				break;
			case SHCNE_CREATE:
				m_pChangeObserver->OnFileCreate(pIdlDst);
				break;
			case SHCNE_DELETE:
				m_pChangeObserver->OnFileDelete(pIdlDst);
				break;
			case SHCNE_MKDIR:
				m_pChangeObserver->OnDirCreate(pIdlDst);
				break;
			case SHCNE_RMDIR:
				m_pChangeObserver->OnDirDelete(pIdlDst);
				break;
			case SHCNE_MEDIAINSERTED:
				m_pChangeObserver->OnMediaInserted(pIdlDst);
				break;
			case SHCNE_MEDIAREMOVED:
				m_pChangeObserver->OnMediaRemoved(pIdlDst);
				break;
			case SHCNE_DRIVEREMOVED:
				m_pChangeObserver->OnDriveRemoved(pIdlDst);
				break;
			case SHCNE_DRIVEADD:
				m_pChangeObserver->OnDriveAdded(pIdlDst);
				break;
			case SHCNE_NETSHARE:
				m_pChangeObserver->OnNetShare(pIdlDst);
				break;
			case SHCNE_NETUNSHARE:
				m_pChangeObserver->OnNetUnShare(pIdlDst);
				break;
			case SHCNE_ATTRIBUTES:
				m_pChangeObserver->OnChangeAttributes(pIdlDst);
				break;
			case SHCNE_UPDATEDIR:
				m_pChangeObserver->OnDirUpdated(pIdlDst);
				break;
			case SHCNE_UPDATEITEM:
				m_pChangeObserver->OnFileUpdated(pIdlDst);
				break;
			case SHCNE_SERVERDISCONNECT:
				m_pChangeObserver->OnServerDisconnect(pIdlDst);
				break;
			case SHCNE_DRIVEADDGUI:
				m_pChangeObserver->OnDriveAddGUI(pIdlDst);
				break;
			case SHCNE_RENAMEFOLDER:
				m_pChangeObserver->OnDirRename(pIdlSrc, pIdlDst);
				break;
			case SHCNE_FREESPACE:
				m_pChangeObserver->OnFreeSpace(pIdlDst);
				break;
			default:
				FTLTRACEEX(tlWarning, TEXT("Unknown Shell Change Notify Event:0x%x\n"),	wEventId);
				break;
			}
		}
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////

	LPCTSTR CFShellUtil::GetShellChangeNotifyString(LONG nEvent, 
		CFStringFormater& formater, LPCTSTR pszDivide /* = TEXT("|") */)
	{
		LONG nOldEvent = nEvent;

		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nEvent, SHCNE_RENAMEITEM, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nEvent, SHCNE_CREATE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nEvent, SHCNE_DELETE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nEvent, SHCNE_MKDIR, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nEvent, SHCNE_RMDIR, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nEvent, SHCNE_MEDIAINSERTED, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nEvent, SHCNE_MEDIAREMOVED, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nEvent, SHCNE_DRIVEREMOVED, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nEvent, SHCNE_DRIVEADD, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nEvent, SHCNE_NETSHARE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nEvent, SHCNE_NETUNSHARE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nEvent, SHCNE_ATTRIBUTES, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nEvent, SHCNE_UPDATEDIR, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nEvent, SHCNE_UPDATEITEM, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nEvent, SHCNE_SERVERDISCONNECT, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nEvent, SHCNE_UPDATEIMAGE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nEvent, SHCNE_DRIVEADDGUI, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nEvent, SHCNE_RENAMEFOLDER, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nEvent, SHCNE_FREESPACE, pszDivide);
#if (_WIN32_IE >= 0x0400)
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nEvent, SHCNE_EXTENDED_EVENT, pszDivide);
#endif 
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nEvent, SHCNE_ASSOCCHANGED, pszDivide);

		FTLASSERT( 0 == nEvent);
		if (0 != nEvent)
		{
			FTLTRACEEX(FTL::tlWarning, TEXT("%s: GetShellChangeNotifyString Not Complete, old=0x%x, remain=0x%x\n"),
				__FILE__LINE__, nOldEvent, nEvent);
		}

		return formater.GetString();
	}

	HRESULT CFShellUtil::GetItemIdName( LPCITEMIDLIST  pItemIdList, LPTSTR pFriendlyName, UINT cchBuf, 
		DWORD dwFlags, IShellFolder* pSF )
	{
		HRESULT hr = S_OK;
		STRRET strRet = {0};
		IShellFolder* pShellFolder  = pSF;

		if ( pShellFolder == NULL )
		{
			COM_VERIFY(SHGetDesktopFolder( &pShellFolder ));
			if ( !pShellFolder )
			{
				return hr;
			}
		}
		COM_VERIFY(pShellFolder->GetDisplayNameOf( pItemIdList, dwFlags, &strRet));
		if (SUCCEEDED(hr))
		{
			COM_VERIFY(StrRetToBuf( &strRet, pItemIdList, pFriendlyName, cchBuf)); 
		}

		if ( NULL == pSF )
		{
			pShellFolder->Release();
			pShellFolder = NULL;
		}
		return hr;
	}

    HRESULT CFShellUtil::GetFileShellInfo(LPCTSTR pszPath, ShellFileInfo& outInfo)
    {
        HRESULT hr = E_NOTIMPL;
#pragma TODO(Use SHGetFileInfo get all info)
        
		CComPtr<IMalloc> spMalloc;
		CComPtr<IShellFolder> spShellFolder;
		COM_VERIFY(SHGetMalloc(&spMalloc));
		COM_VERIFY(SHGetDesktopFolder(&spShellFolder));
		
		if (spShellFolder && spMalloc)
		{

		}
		
        return hr;
    }

	HRESULT CFShellUtil::GetShellIconImageList(__out HIMAGELIST& rSmallIconList, __out HIMAGELIST& rLargeIconList)
	{
		FTLASSERT( NULL == rSmallIconList );
		FTLASSERT( NULL == rLargeIconList );

		HRESULT hr = E_FAIL;

		SHFILEINFO shfi = {0};
		rSmallIconList = (HIMAGELIST) SHGetFileInfo(NULL, 0, &shfi, sizeof(SHFILEINFO),
			SHGFI_ICON | SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
		rLargeIconList = (HIMAGELIST) SHGetFileInfo(NULL, 0, &shfi, sizeof(SHFILEINFO),
			SHGFI_ICON | SHGFI_SYSICONINDEX | SHGFI_LARGEICON);
		
		if (rSmallIconList != NULL && rLargeIconList != NULL)
		{
			hr = S_OK;
		}
		else
		{
			ImageList_Destroy(rSmallIconList);
			rSmallIconList = NULL;
			ImageList_Destroy(rLargeIconList);
			rLargeIconList = NULL;

			hr = E_FAIL;
		}
		return hr;
	}

	HRESULT CFShellUtil::CreateLink(LPCTSTR szPathObj, LPCTSTR szPathLink, LPCTSTR szDesc, LPCTSTR szIconPath /* = NULL */, int iIcon /* = -1 */)
	{
		HRESULT hr = E_FAIL;
		CComPtr<IShellLink> spShellLink;

		COM_VERIFY(CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
			IID_IShellLink, (void**)&spShellLink));
		if (SUCCEEDED(hr)) 
		{
			CComPtr<IPersistFile> spPersistFile;
			COM_VERIFY(spShellLink->QueryInterface(IID_IPersistFile, (void**)&spPersistFile));
			if (SUCCEEDED(hr))
			{
				COM_VERIFY(spShellLink->SetPath(szPathObj));		// 全路径程序名
				COM_VERIFY(spShellLink->SetDescription(szDesc));	// 备注
				//spShellLink->SetArguments();						// 命令行参数
				//spShellLink->SetHotkey();							// 快捷键
				//spShellLink->SetIconLocation();					// 图标
				//spShellLink->SetShowCmd();						// 窗口尺寸
				//spShellLink->SetWorkingDirectory(...);			//设置 EXE 程序的默认工作目录
				if (szIconPath && iIcon >= 0)
				{
					COM_VERIFY(spShellLink->SetIconLocation(szIconPath, iIcon));
				}
                CFConversion conv;
				COM_VERIFY(spPersistFile->Save(conv.TCHAR_TO_UTF16(szPathLink), TRUE));
			}
		}

		return hr;
	}

	HRESULT CFShellUtil::ExecuteOrOpenWithDialog(LPCTSTR pszFile, HWND hWndParent)
	{
		HRESULT hr = S_OK;
		BOOL bRet = FALSE;
		//先使用 ShellExecute 执行，如果有错误不会弹出错误信息
		DWORD dwResult = (DWORD) ::ShellExecute(hWndParent, TEXT("open") , pszFile , NULL, NULL, SW_NORMAL);
		if (dwResult < 32)
		{
			hr = HRESULT_FROM_WIN32(dwResult);

			if (SE_ERR_NOASSOC == dwResult)
			{
				//如果是未建立关联程序的错误
				HMODULE hShell32 = GetModuleHandle(TEXT("Shell32.dll"));
				FTLASSERT(hShell32);

				typedef HRESULT (_stdcall *SHOpenWithDialogProc)(HWND hwndParent, const OPENASINFO* poainfo);
				SHOpenWithDialogProc pShOpenWithDialog = (SHOpenWithDialogProc)GetProcAddress(hShell32, "SHOpenWithDialog");
				if (pShOpenWithDialog)
				{
					//Vista or Win7 above
					OPENASINFO openAsInfo = {0};
					openAsInfo.pcszFile = pszFile;
					openAsInfo.oaifInFlags = OAIF_ALLOW_REGISTRATION | OAIF_EXEC;

					COM_VERIFY_EXCEPT2((*pShOpenWithDialog)(hWndParent, &openAsInfo), S_FALSE, ERROR_CANCELLED);
					//Notice: Does not need call FreeLibrary
				}
				else
				{
					//WinXP
					TCHAR szSysDir[MAX_PATH] = {0};
					GetSystemDirectory(szSysDir, _countof(szSysDir));

					//CPath pathRundll32(szSysDir);
					//pathRundll32.Append(TEXT("Rundll32.exe"));
					TCHAR pathRundll32[MAX_PATH] = {0};
					StringCchCopy(pathRundll32, _countof(pathRundll32), szSysDir);
					PathAppend(pathRundll32, TEXT("Rundll32.exe"));

					//CPath pathRunParam(szSysDir);
					//pathRunParam.Append(TEXT("Shell32.dll,OpenAs_RunDLL"));
					TCHAR pathRunParam[MAX_PATH] = {0};
					StringCchCopy(pathRunParam, _countof(pathRunParam), szSysDir);
					PathAppend(pathRunParam, TEXT("Shell32.dll,OpenAs_RunDLL"));

					CString strCmd;  
					strCmd.Format(_T("%s %s %s"), pathRundll32, pathRunParam, pszFile);
					FTLTRACE(TEXT("ExecuteOpenWithDialog, Cmd=%s\n"), strCmd);
					//MessageBox( strCmd, TEXT("OpenAs_RunDLL in XP"), MB_OK);

#pragma TODO(CreateProcess cannot work)

					//STARTUPINFO startupInfo = {0};
					//startupInfo.cb = sizeof(startupInfo);

					//PROCESS_INFORMATION processInfo = {0};
					//API_VERIFY(CreateProcess(pathRundll32.m_strPath, (LPTSTR)(LPCTSTR)strCmd, NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo));
					//if (bRet)
					//{
					//	CloseHandle(processInfo.hProcess);
					//	CloseHandle(processInfo.hThread);
					//}
					//dwResult = (DWORD)ShellExecute(m_hWnd, TEXT("open"), strCmd, NULL, NULL, SW_SHOWNORMAL);
					//if (dwResult != 0)
					//{
					//	CString strRet;
					//	strRet.Format(TEXT("ErrorReturn:%d"), dwResult);
					//	MessageBox(strRet, TEXT(""), MB_OK);
					//}

					USES_CONVERSION;
					LPSTR pszCmd = T2A(strCmd);
					API_VERIFY(::WinExec( pszCmd, SW_SHOWNORMAL ));
				}
			}
			else
			{
				//other error -- use ShellExecuteEx and show system error message
				SHELLEXECUTEINFO ExecuteInfo= {0};
				ExecuteInfo.cbSize = sizeof(ExecuteInfo);
				ExecuteInfo.fMask = 0;// SEE_MASK_NOCLOSEPROCESS;
				ExecuteInfo.hwnd = hWndParent;
				ExecuteInfo.lpVerb = TEXT("open");
				ExecuteInfo.lpFile =  pszFile;
				ExecuteInfo.nShow = SW_NORMAL;
				API_VERIFY(ShellExecuteEx(&ExecuteInfo));
				if (!bRet)
				{
					hr = HRESULT_FROM_WIN32(GetLastError());
				}
			}
		}

		return hr;
	}

	HRESULT CFShellUtil::ExplorerToSpecialFile(LPCTSTR pszFilePath)
	{
		HRESULT hr = E_FAIL;

		PIDLIST_RELATIVE pidl = NULL;
		ULONG attributes = 0;
		SFGAOF sfgaofIn = 0, sfgaofOut = 0;

		COM_VERIFY(SHParseDisplayName(pszFilePath, NULL,	&pidl, sfgaofIn, &sfgaofOut));
		if (SUCCEEDED(hr))
		{
			COM_VERIFY(SHOpenFolderAndSelectItems(pidl, 0, NULL, 0));
			//ILFree(pidl);
			CoTaskMemFree(pidl);
			pidl = NULL;
		}
		return hr;
	}

    CFDirBrowser::CFDirBrowser(LPCTSTR lpszTitle /* = NULL */, HWND hWndOwner /* = NULL */, LPCTSTR pszInit /* = NULL */, 
        UINT nFlags)
    {
        ZeroMemory(&m_browseInfo, sizeof(m_browseInfo));
        m_szPath[0] = NULL;
        m_szInitPath[0] = NULL;
        m_hTreeView = NULL;
        m_bFirstEnsureSelectVisible = TRUE;

        m_browseInfo.lpszTitle = lpszTitle;
        m_browseInfo.hwndOwner = hWndOwner;
        m_browseInfo.pszDisplayName = m_szPath;
        m_browseInfo.ulFlags = nFlags;
        m_browseInfo.lpfn = DirBrowseCallbackProc;
        m_browseInfo.lParam = (LPARAM)this;
        
        if (pszInit && PathFileExists(pszInit))
        {
            lstrcpyn(m_szInitPath, pszInit, _countof(m_szInitPath));
        }
     }

    BOOL CFDirBrowser::DoModal()
    {
        BOOL bRet = FALSE;
        LPITEMIDLIST pIDList = SHBrowseForFolder(&m_browseInfo);    //调用显示选择对话框
        if (pIDList)
        {
            API_VERIFY(SHGetPathFromIDList(pIDList, m_szPath)); //取得文件夹路径到m_szPath里
            CoTaskMemFree(pIDList);
        }
        return bRet;
    }

    HWND CFDirBrowser::FindTreeViewCtrl(HWND hWnd)
    {
        BOOL bRet = FALSE;
        TCHAR szClassName[FTL_MAX_CLASS_NAME_LENGTH] = {0}; //The maximum length for lpszClassName is 256.
        HWND hWndItem = ::GetWindow(hWnd, GW_CHILD);
        BOOL bFound = FALSE;
        while (hWndItem && !bFound)
        {
            API_VERIFY(::GetClassName(hWndItem, szClassName, _countof(szClassName)) > 0);
            if (bRet)
            {
                if (lstrcmpi(szClassName, WC_TREEVIEW) == 0)
                {
                    //normal style
                    //BrowserDlg -> SysTreeView32
                    bFound = TRUE;
                    break;
                }
                if (lstrcmpi(szClassName, _T("SHBrowseForFolder ShellNameSpace Control")) == 0)
                {
                    //BIF_NEWDIALOGSTYLE
                    //BrowserDlg -> Select Dest Folder(SHBrowseForFolder ShellNameSpace Control) -> SysTreeView32
                    HWND hWndItemInControl = ::GetWindow(hWndItem, GW_CHILD);
                    while (hWndItemInControl)
                    {
                        API_VERIFY(::GetClassName(hWndItemInControl, szClassName, _countof(szClassName)) > 0);
                        if (bRet)
                        {
                            if (lstrcmpi(szClassName, WC_TREEVIEW) == 0)
                            {
                                bFound = TRUE;
                                hWndItem = hWndItemInControl;
                                break;
                            }
                        }
                        hWndItemInControl = ::GetWindow(hWndItemInControl, GW_HWNDNEXT);
                    }
                }
            }
            if (!bFound)
            {
                hWndItem = ::GetWindow(hWndItem, GW_HWNDNEXT);
            }
        }
        return hWndItem;
    }

    BOOL CFDirBrowser::EnsureSelectVisible()
    {
        BOOL bRet = FALSE;
        if (m_hTreeView)
        {
            HTREEITEM hSelected= (HTREEITEM)::SendMessage(m_hTreeView, TVM_GETNEXTITEM, TVGN_CARET, NULL);
            //TreeView_GetSelection(m_hTreeView);
            if (hSelected)
            {
#ifdef _DEBUG
                TCHAR szSelectPath[MAX_PATH] = {0};
                TVITEM item = {0};
                item.hItem = hSelected;
                item.mask = TVIF_TEXT | TVIF_STATE;
                item.pszText = szSelectPath;
                item.cchTextMax = _countof(szSelectPath);
                //TreeView_GetItem(m_hTreeView, &item);
                ::SendMessage(m_hTreeView, TVM_GETITEM, 0, (LPARAM)&item);
                FTLTRACE(TEXT("Select Item Text=%s\n"), szSelectPath);
#endif
                ::SendMessage(m_hTreeView, TVM_ENSUREVISIBLE, 0, (LPARAM)hSelected);
                //TreeView_EnsureVisible(m_hTreeView, hSelected);

                bRet = TRUE;
            }
        }
        return bRet;
    }
    int CFDirBrowser::DirBrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
    {
        HRESULT hr = E_FAIL;
        BOOL bRet = FALSE;

        //FTLTRACE(TEXT("DirBrowseCallbackProc, uMsg=%d\n"), uMsg);
        CFDirBrowser* pThis = reinterpret_cast<CFDirBrowser*>(lpData);

        switch (uMsg)
        {
        case BFFM_INITIALIZED:
            {
                FTLTRACE(TEXT("DirBrowseCallbackProc - BFFM_INITIALIZED\n"));
                if (pThis->m_szInitPath[0])
                {
                    SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)pThis->m_szInitPath);
                    pThis->m_hTreeView = pThis->FindTreeViewCtrl(hwnd);
                }
                break;
            }
        case BFFM_SELCHANGED:
            {
                LPITEMIDLIST pItemList = (LPITEMIDLIST)lParam;
                TCHAR szSelPath[MAX_PATH] = {0};
                bRet = SHGetPathFromIDList(pItemList, szSelPath);
                //COM_VERIFY(CFShellUtil::GetItemIdName(pItemList, szSelPath, _countof(szSelPath), SHGDN_FORPARSING));
                if(bRet) //SUCCEEDED(hr))
                {
                    FTLTRACE(TEXT("DirBrowseCallbackProc - BFFM_SELCHANGED, %s\n"), szSelPath);
                    SendMessage(hwnd, BFFM_SETSTATUSTEXT, NULL, (LPARAM)szSelPath);
                }

                if (pThis->m_browseInfo.ulFlags & BIF_NEWDIALOGSTYLE
                    && pThis->m_bFirstEnsureSelectVisible)
                {
                    pThis->m_bFirstEnsureSelectVisible = !pThis->EnsureSelectVisible();
                }
                break;
            }
        case BFFM_VALIDATEFAILED:
            {
                FTLTRACE(TEXT("DirBrowseCallbackProc - BFFM_VALIDATEFAILED, %s\n"), (LPCTSTR)lParam);
                break;
            }
        case BFFM_IUNKNOWN:
            {
                //经测试，至少支持如下接口： IFolderFilter、IFolderFilterSite
                IUnknown *pUnknown = reinterpret_cast<IUnknown*>(lParam);
                FTLTRACE(TEXT("DirBrowseCallbackProc - BFFM_IUNKNOWN, pUnknown=0x%p\n"), pUnknown);

                //COM_DETECT_INTERFACE_FROM_REGISTER(pUnknown);
                break;
            }
        default:
            FTLASSERT(FALSE);
            break;
        }
        return 1;
    }
}

#endif //FTL_SHELL_HPP