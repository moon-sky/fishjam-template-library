
#ifndef FTL_SHELL_H
#define FTL_SHELL_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlShell.h requires ftlbase.h to be included first
#endif

/*****************************************************************************************************
* DropFile -- WM_DROPFILES 消息，需要设置 WS_EX_ACCEPTFILES 风格
*   nCount = DragQueryFile(hDropInfo, -1, NULL, 0);
*   for( i < nCount; ) { 
*     nSize = DragQueryFile(hDropInfo, i NULL, 0); 
*     pBuf=new char[++nSize]; 
*     DragQueryFile(hDropInfo, i, pBuf, nSize);
*   }
*
* SHGetSpecialFolderPath/SHGetSpecialFolderLocation -- 获取系统中指定类型的目录位置信息
* ShellBrowser 窗体现在只需要用 SHBrowseForFolder 和 SHGetPathFromIDList 函数即可
* SHGetDesktopFolder -- 获取Shell中桌面对应的IShellFolder接口
*
* LPITEMIDLIST -- Shell 中管理对象的唯一标识? 可以通过 ILCreateFromPath、ILClone、ILGetNext、ILCombine 等函数操纵，
*   使用 IShellFolder::BindToObject 绑定到目标目录上， EnumObjects 枚举其下的所有子对象
* LPENUMIDLIST
* 
* 
*   SHILCreateFromPath -- 从字符串的PATH中获得 LPITEMIDLIST
*   SHGetPathFromIDList-- 从LPITEMIDLIST中获得字符串的PATH
*
* 获取Shell的图标
*   SHFILEINFO shfi = {0};
*   UINT flags = SHGFI_ICON |SHGFI_SYSICONINDEX|SHGFI_SMALLICON; //SHGFI_LARGEICON
*   HIMAGELIST hi=(HIMAGELIST)SHGetFileInfo(NULL,0,&shfi,sizeof(SHFILEINFO),flags); //CImageList::FromHandle(hi
*
* 使用Shell打开指定文件(LPITEMIDLIST) -- 可以避免参数带空格的问题
*	SHELLEXECUTEINFO ShExecInfo = {0};
*   ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
*   ShExecInfo.fMask = SEE_MASK_IDLIST;
*   ShExecInfo.lpIDList= pidlItemTarget;
*   ShExecInfo.nShow = SW_MAXIMIZE;
*   ShellExecuteEx(&ShExecInfo);
*
* 获取文件版本信息
*   GetFileVersionInfoSize -> GetFileVersionInfo -> VerQueryValue(lpData, TEXT("\\"), &pFileInfo, (PUINT)&bufLen);
*   HIWORD(MS),LOWORD(MS),wBuild, HIWORD(LS)
*
*****************************************************************************************************/

/*****************************************************************************************************
* 文件系统变化监控
*   1.SHChangeNotifyRegister -- 把指定的窗口添加到系统的消息监视链中，接收并处理来自文件系统或者Shell的通知消息
*   2.FindFirstChangeNotification  -- 创建一个更改通知句柄并设置初始更改通知过滤条件，然后进行Wait
*   3.ReadDirectoryChangesW -- 
*   4.实现ICopyHook接口的Shell扩展对象，并注册到 HKCR\Directory\shellex\CopyHookHandlers 下
* 注册表系统变化监控
*   1.RegNotifyChangeKeyValue
*****************************************************************************************************/

namespace FTL
{
	
    class IShellChangeObserver
    {
    public:
		virtual void OnFileRename( LPCITEMIDLIST /* pIdl_Src */, LPCITEMIDLIST /* pIdl_Dst */ ) {};
		virtual void OnFileCreate( LPCITEMIDLIST /* pIdl */ ){};
		virtual void OnFileDelete( LPCITEMIDLIST /* pIdl */  ) {};
		virtual void OnFileUpdated( LPCITEMIDLIST /* pIdl */  ) {};
		virtual void OnFreeSpace(LPCITEMIDLIST /* pIdl */  ) {};
		virtual void OnDirRename( LPCITEMIDLIST /* pIdl_Src */, LPCITEMIDLIST /* pIdl_Dst */ ){
            FTLTRACE(TEXT("OnDirRename: \n"));
        };

		virtual void OnDirCreate( LPCITEMIDLIST /* pIdl */  ) {};
		virtual void OnDirDelete( LPCITEMIDLIST /* pIdl */  ) {};
		virtual void OnDirUpdated( LPCITEMIDLIST /* pIdl */  ){};
		virtual void OnMediaInserted( LPCITEMIDLIST /* pIdl */  ){};
		virtual void OnMediaRemoved( LPCITEMIDLIST /* pIdl */  ){};
		virtual void OnNetShare(LPCITEMIDLIST /* pIdl */ ) {};
		virtual void OnNetUnShare(LPCITEMIDLIST /* pIdl */ ) {};
		virtual void OnDriveAdded( LPCITEMIDLIST /* pIdl */  ) {};
		virtual void OnDriveAddGUI( LPCITEMIDLIST /* pIdl */  ) {};
		virtual void OnDriveRemoved( LPCITEMIDLIST /* pIdl */  ) {};
		virtual void OnChangeAttributes( LPCITEMIDLIST /* pIdl */  ) {};
		virtual void OnServerDisconnect(LPCITEMIDLIST /* pIdl */  ) {};
    };

	//检测文件系统文件变化通知
    FTLEXPORT class CFShellChangeMonitor
    {
    public:
        FTLINLINE CFShellChangeMonitor();
        FTLINLINE ~CFShellChangeMonitor();
		FTLINLINE BOOL SetChangeObserver(IShellChangeObserver* pChangeObserver)
		{
			m_pChangeObserver = pChangeObserver;
			return TRUE;
		}
        FTLINLINE BOOL Create(LPCTSTR pszMonitorPath = NULL, 
			LONG nEvent = SHCNE_ALLEVENTS | SHCNE_INTERRUPT,
			BOOL bRecursive = TRUE);
        FTLINLINE BOOL Destroy();
    private:
		IShellChangeObserver*	m_pChangeObserver;
        HWND m_hWndNotify;
        UINT m_uiNotifyMsg;
		ULONG m_uiChangeNotifyID;
        IShellFolder*  m_pShellFolder;
        FTLINLINE BOOL _CreateNotifyWinows();
        FTLINLINE static LRESULT CALLBACK _MainMonitorWndProc(HWND hwnd,UINT uMsg, WPARAM wParam,LPARAM lParam);
		FTLINLINE LRESULT _HandleMonitorEvent(LONG wEventId, LPCITEMIDLIST pIdlDst, LPCITEMIDLIST pIdlSrc);

        //FTLINLINE BOOL _RegisterMonitor()   
    };

    struct ShellFileInfo
    {
        //PIDLIST_ABSOLUTE    pIdl;
    };
    FTLEXPORT class CFShellUtil
    {
    public:
		//获取 SHCNE_XXX( 如 SHCNE_RENAMEITEM ) 等对应的字符串信息
		FTLINLINE static LPCTSTR GetShellChangeNotifyString(LONG nEvent, CFStringFormater& formater, LPCTSTR pszDivide = TEXT("|"));

		//获取 pItemIdList 对应的字符串
		FTLINLINE static HRESULT GetItemIdName(  LPCITEMIDLIST  pItemIdList, LPTSTR pFriendlyName, UINT cchBuf, 
			DWORD dwFlags = SHGDN_FORPARSING, IShellFolder* pSF = NULL );

        //获取路径对应的 LPITEMIDLIST
        FTLINLINE static HRESULT GetItemIDListFromPath( LPCTSTR szFullPath, LPITEMIDLIST* ppidl, IShellFolder* pSF = NULL);

        //TODO: 系统已经提供了 SHGetFileInfo 函数
        FTLINLINE static HRESULT GetFileShellInfo(LPCTSTR pszPath, ShellFileInfo& outInfo);

		//获取Shell的系统图标列表，之后可以通过 GetListCtrl().SetImageList(CImageList::FromHandle(hi),LVSIL_SMALL 或 LVSIL_NORMAL) 的方式使用
		//获取文件信息时，可以获取到其在系统图标中的s索引 ?
		FTLINLINE static HRESULT GetShellIconImageList(__out HIMAGELIST& rSmallIconList, __out HIMAGELIST& rLargeIconList);

		//CLSID_ShellLink 组件有 IShellLink(提供快捷方式的参数读写功能)、IPersistFile(提供快捷方式持续性文件的读写功能)等接口。
		FTLINLINE static HRESULT CreateLink(LPCTSTR szPathObj, LPCTSTR szPathLink, LPCTSTR szDesc, LPCTSTR szIconPath = NULL, int iIcon = -1);

		//使用ShellExecute的方法打开文件执行，但如果没有建立连接，则弹出"OpenWith"的对话框
		//  TODO: 是否有其他标准的函数来完成该功能
		FTLINLINE static HRESULT ExecuteOrOpenWithDialog(LPCTSTR pszFile, HWND hWndParent);

		FTLINLINE static HRESULT ExplorerToSpecialFile(LPCTSTR pszFilePath);

        FTLINLINE static HRESULT LaunchIE(LPCTSTR szURL, int nCmdShow = SW_SHOW);

        //FTLINLINE static BOOL BrowserDirectory(CFStringFormater& strResult, LPCTSTR strTitle = NULL, UINT nFlags = BIF_NEWDIALOGSTYLE);
    };


    //BUG -- Win7 下，当有 BIF_NEWDIALOGSTYLE 时，无法自动定位显示 pszInit 的目录
    //https://connect.microsoft.com/VisualStudio/feedback/details/518103/bffm-setselection-does-not-work-with-shbrowseforfolder-on-windows-7
    //atldlgs.h 中有 CFolderDialog -- CFolderDialog dlg ( hWnd, NULL, BIF_USENEWUI | BIF_RETURNONLYFSDIRS ); dlg.SetInitialFolder( szDefaultPath ); if (IDOK ==dlg.DoModal() ){ ... }
    class CFDirBrowser
    {
    public:
        FTLINLINE CFDirBrowser(LPCTSTR lpszTitle = NULL, HWND hWndOwner = NULL, LPCTSTR pszInit = NULL, 
            UINT nFlags = BIF_NEWDIALOGSTYLE | 
            BIF_EDITBOX |
            BIF_STATUSTEXT |        //可包含状态区域，通过发送消息可以设置其文本
            BIF_RETURNONLYFSDIRS |  //仅返回文件系统的目录，选中“我的电脑”等时确认按钮为禁用状态
            BIF_BROWSEINCLUDEURLS 
            );
        FTLINLINE BOOL DoModal();
        FTLINLINE LPCTSTR GetSelectPath() { return m_szPath; }
        BROWSEINFO  m_browseInfo;
    protected:
        TCHAR       m_szPath[MAX_PATH];
        TCHAR       m_szInitPath[MAX_PATH];
    protected:
        FTLINLINE static int CALLBACK DirBrowseCallbackProc(HWND hwnd, UINT uMsg,LPARAM lParam, LPARAM lpData);
        HWND        m_hTreeView;
        BOOL        m_bFirstEnsureSelectVisible;
        FTLINLINE HWND FindTreeViewCtrl(HWND hWnd);
        FTLINLINE BOOL EnsureSelectVisible();
    };
}

#endif //FTL_SHELL_H

#ifndef USE_EXPORT
#  include "ftlShell.hpp"
#endif