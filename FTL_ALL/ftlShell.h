
#ifndef FTL_SHELL_H
#define FTL_SHELL_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlShell.h requires ftlbase.h to be included first
#endif

/*****************************************************************************************************
* DropFile -- WM_DROPFILES 消息
*   nCount = DragQueryFile(hDropInfo, -1, NULL, 0);
*   for( i < nCount; ) { 
*     nSize = DragQueryFile(hDropInfo, i NULL, 0); 
*     pBuf=new char[++nSize]; 
*     DragQueryFile(hDropInfo, i, pBuf, nSize);
*   }
*
* SHGetSpecialFolderLocation/SHGetSpecialFolderPath -- 获取系统中指定类型的目录位置信息
* ShellBrowser 窗体现在只需要用 SHBrowseForFolder 和 SHGetPathFromIDList 函数即可
* SHGetDesktopFolder -- 获取Shell中桌面对应的IShellFolder接口
*
* LPITEMIDLIST -- Shell 中管理对象的唯一标识? 可以通过 ILCombine 等函数操纵，
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
*****************************************************************************************************/

namespace FTL
{
	
    class IShellChangeObserver
    {
    public:

    };

	//检测文件系统文件变化通知
    FTLEXPORT class CFShellChangeMonitor
    {
    public:
        FTLINLINE CFShellChangeMonitor();
        FTLINLINE ~CFShellChangeMonitor();
        FTLINLINE BOOL Create(LPCTSTR pszMonitorPath = NULL, BOOL bRecursive = TRUE);
        FTLINLINE BOOL Destroy();
    private:
        HWND m_hWndNotify;
        UINT m_uiNotifyMsg;
		ULONG m_uiChangeNotifyID;
        IShellFolder*  m_pShellFolder;
        BOOL _CreateNotifyWinows();
        static FTLINLINE LRESULT CALLBACK MainMonitorWndProc(HWND hwnd,UINT uMsg, WPARAM wParam,LPARAM lParam);

        //FTLINLINE BOOL _RegisterMonitor()   
    };

    struct ShellFileInfo
    {

    };
    FTLEXPORT class CFShellUtil
    {
    public:
        FTLINLINE static HRESULT GetFileShellInfo(LPCTSTR pszPath, ShellFileInfo& outInfo);

		//使用ShellExecute的方法打开文件执行，但如果没有建立连接，则弹出"OpenWith"的对话框
		//  TODO: 是否有其他标准的函数来完成该功能
		FTLINLINE static HRESULT ExecuteOpenWithDialog(LPCTSTR pszFile, HWND hWndParent);
    };
}

#endif //FTL_SHELL_H

#ifndef USE_EXPORT
#  include "ftlShell.hpp"
#endif