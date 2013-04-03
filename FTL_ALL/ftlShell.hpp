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
        m_hWndNotify = NULL;
        m_uiNotifyMsg = 0;
		m_uiChangeNotifyID = 0;
        m_pShellFolder = NULL;
        
        HRESULT hr = E_FAIL;
        COM_VERIFY(SHGetDesktopFolder(&m_pShellFolder));
    }

    CFShellChangeMonitor::~CFShellChangeMonitor()
    {
        if (m_hWndNotify)
        {
            Destroy();
        }
    }

    BOOL CFShellChangeMonitor::Create(LPCTSTR pszMonitorPath /* = NULL */, BOOL bRecursive /* = TRUE */)
    {
        BOOL bRet = FALSE;
        HRESULT hr = E_FAIL;
        m_uiNotifyMsg = RegisterWindowMessage(TEXT("FTL_SHELL_CHANGE_MONITOR"));

        if (NULL == pszMonitorPath)
        {
            LPITEMIDLIST pItemDesktop = NULL;
            COM_VERIFY(::SHGetSpecialFolderLocation(NULL,CSIDL_DESKTOPDIRECTORY, &pItemDesktop));
            if (SUCCEEDED(hr))
            {
            }
        }
        
        int fSources = SHCNF_TYPE |  //SHCNF_IDLIST
#if defined (UNICODE)  || defined (_UNICODE)
            SHCNF_PATHW;
#else
            SHCNF_PATHA;
#endif

#if 0
        SHChangeNotifyEntry notifyEntry = {0};
        notifyEntry.fRecursive = bRecursive;
        notifyEntry.pidl = GetPidlFromPath(pszMonitorPath);
        m_uiChangeNotifyID = ::SHChangeNotifyRegister(m_hWndNotify, fSources, SHCNE_ALLEVENTS | SHCNE_INTERRUPT, 
			m_uiNotifyMsg,   
			1, &notifyEntry);
#endif 
        return bRet;
    }

    BOOL CFShellChangeMonitor::Destroy()
    {
        if(m_hWndNotify)
        {
			SHChangeNotifyDeregister(m_uiChangeNotifyID);
            m_pShellFolder->Release();
            m_pShellFolder = NULL;

            ::DestroyWindow(m_hWndNotify);
            m_hWndNotify = NULL;
        }

    }

    BOOL CFShellChangeMonitor::_CreateNotifyWinows()
    {
        WNDCLASS wndClass = {0};
        
        //::RegisterClass()
        //::CreateWindow()
        return FALSE;
    }

    LRESULT CALLBACK CFShellChangeMonitor::MainMonitorWndProc(HWND hwnd,UINT uMsg, 
        WPARAM wParam,LPARAM lParam)
    {
        switch(uMsg)
        {
        case WM_CREATE:
            break;
		//case m_uiNotifyMsg:
		//	//处理这个消息获得变化通知
		//	{
		//		LPCITEMIDLIST* pItemList = (LPCITEMIDLIST*)wParam;
		//		pItemList[0]; // Dst
		//		pItemList[1]; // Src
		//		
		//		//然后调用 SHChangeNotify ?
		//		break;
		//	}
        default: 
            return DefWindowProc(hwnd, uMsg, wParam, lParam); 
        }
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

				COM_VERIFY(spPersistFile->Save(CFConversion().TCHAR_TO_UTF16(szPathLink), TRUE));
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

}

#endif //FTL_SHELL_HPP