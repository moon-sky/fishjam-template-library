#ifndef FTL_IE_HELPER_H
#define FTL_IE_HELPER_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlIEHelper.h requires ftlbase.h to be included first
#endif

#ifndef FTL_COM_H
#  error ftlIEHelper.h requires ftlCom.h to be included first
#endif


/********************************************************************************************
* IE 7 的保护模式(Protected Mode) -- 也叫IE低权利(Low Rights)，通过 Process Explorer 查看进程的"安全"属性，可看到有
*   "Mandatory Label\Low Mandatory Level"的SID(相应的标志位是"Integrity")，这表明IE进程的完整性级别是"Low"。
*   "Virtualized" 为 Yes
*   保护模式下访问的文件目录(完整性级别都是"Low")：
*     1.IE临时文件: %UserProfile%\AppData\Local\Microsoft\Windows\Temporary Internet Files\Low
*     2.系统临时目录: %UserProfile%\AppData\Local\Temp\Low
*     3.Cookies: %UserProfile%\AppData\Roaming\Microsoft\Windows\Cookies\Low
*     4.历史: %UserProfile%\AppData\Local\Microsoft\Windows\History\Low
*     5.收藏夹: 收藏夹默认允许低级别的IE进程访问，因此不需要独立的目录？
*   解决兼容性问题：
*     1.通过建立NTFS软链接(Soft Link)的方法来解决IE保护模式的兼容性问题：linkd 源目录 低权限的目的目录
*       实质：给NTFS文件增加一个重解析点(REPARSE)的属性
*       Linkd 可执行程序在 Windows Server 2003 Resource Kit Tools工具包 中
*     2.SHGetSpecialFolderPath(NULL, szSavePath, CSIDL_LOCAL_APPDATA , TRUE); //取得 %UserProfile%\AppData\Local
*       CString strFilePath(szSavePath); 
*       strFilePath += TEXT("\\Temp\\Low\\nCapture\\IEToobar\\");  -- 之后的访问都在具有 Low 完整性等级的目录下
*   
* 代理进程(特权分离思想) -- 把传统的IE进程一分为三，分别负责不同安全等级的任务.
*   IE进程本身 -- 完成最常用的、无需特权的网页浏览任务
*   IeUser.exe(用户级代理进程) -- 完成中等特权的任务(例如保存图片等)
*   IeInstal.exe(管理级代理进程) -- 完成高特权的任务(例如安装加载项)。
* 
* IE中的多进程控制
*   HKLM或HKCR \SOFTWARE\Microsoft\Internet Explorer\MAIN\TabProcGrowth  -- REG_SZ, 为 0 表示， 为 Medium 表示
*
********************************************************************************************/

namespace FTL
{
    class CFIEUtils
    {
    public:
		//从Window窗体句柄获取IE内的Docuemnt接口
		FTLINLINE static HRESULT GetIEDocumentFromHWnd(HWND hWnd, IHTMLDocument** ppDocument);

		FTLINLINE static BOOL IsProtectedModeProcess();
    };

}//namespace FTL

#ifndef USE_EXPORT
#  include "ftlIEHelper.hpp"
#endif

#endif //FTL_IE_HELPER_H
