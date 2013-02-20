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
* Win8 -- 两种不同的用户接口，需要分别运行不同类型的应用程序（如IE有对应的 Metro style 和 Desktop style)
*   1.经典 -- 桌面应用程序
*   2.Metro -- 专门的 Metro 程序或 Metro-style enabled desktop browsers (MEDB).Metro模式下的IE只有Flash插件，不能安装或运行其他的任何插件?
*     WinRT -- ARM平台上的Window8，只能运行纯粹的Metro程序，不能使用Win32API，需要使用专门的 WinRT API?
*
* <iepmapi.h> -- IE Protected Mode API, 其中有 IEIsProtectedModeProcess 等函数的定义(但考虑到 WinXP 等得运行，最好动态加载)
* EPM(Enhanced Protected Mode) -- 启用后很多插件都无法正确运行。Metro类型的始终运行于EPM, Desktop类型的可选(Internet Options->Advanced),
*   Web网站的开发者在 Http Header 中通过"X-UA-Compatible:requiresActiveX=true" 选项要求plugin(即要求经典模式?)

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
*   IeInstal.exe(管理级代理进程) -- 完成高特权的任务(例如安装加载项，需要写INF文件?)。
* 
* IE中的多进程控制
*   HKLM或HKCR \SOFTWARE\Microsoft\Internet Explorer\MAIN\TabProcGrowth  -- REG_SZ, 为 0 表示， 为 Medium 表示
*
********************************************************************************************/

/********************************************************************************************
* IWebBrowser2::Navigate 的 PostData 是个 VT_ARRAY 的 Variant，例子 MakeNavigatePosData（未确认）
*  
********************************************************************************************/

/********************************************************************************************
* 浏览网页时的事件顺序(DISPID_XXX)
*   DISPID_ONVISIBLE(OnVisible)
*   DISPID_BEFORENAVIGATE2(BeforeNavigate2)
*   DISPID_DOWNLOADBEGIN(DownloadBegin)
*   DISPID_COMMANDSTATECHANGE(CommandStateChange) -- 指示 Toolbar/Forward/Back 等按钮的状态改变
*   DISPID_STATUSTEXTCHANGE(StatusTextChange) -- 状态条文本改变,如"检测代理、连接信息"等
*   DISPID_PROGRESSCHANGE(ProgressChange) -- 进度改变
*   DISPID_FILEDOWNLOAD(FileDownload) -- 指示开始下载文件，可以通过 bCancel 取消
*   DISPID_DOWNLOADCOMPLETE(DownloadComplete) -- 浏览完成时（或挂起，失败时) -- 这个发生了两次，和 NavigateComplete2 的关系和顺序？
*   DISPID_SETSECURELOCKICON(SetSecureLockIcon) -- 加密等级变化
*   DISPID_TITLECHANGE(TitleChange) -- 标题改变
*   DISPID_NAVIGATECOMPLETE2(NavigateComplete2) -- 导航结束(为什么没有发生 NavigateComplete?)
*   DISPID_PROGRESSCHANGE(ProgressChange) -- 进度改变(ProgressMax/ProgressMax ，然后 -1/ProgressMax 表示完全结束 )
*   DISPID_DOCUMENTCOMPLETE(DocumentComplete) -- 文档完全加载(一般在这个事件中进行后续处理), 
*     Url 是规范化后的URL，也可能经过服务器重定位后的，在没有frame的页面中，加载完成后只触发一次，有多个frame时，每个frame均触发；
*     highest frame 触发最终的事件，此时 pDisp 指向 highest frame。
********************************************************************************************/

namespace FTL
{
    class CFIEUtils
    {
    public:
		//从Window窗体句柄获取IE内的Docuemnt接口
		FTLINLINE static HRESULT GetIEDocumentFromHWnd(HWND hWnd, IHTMLDocument** ppDocument);

		FTLINLINE static BOOL IsProtectedModeProcess();

		FTLINLINE static BOOL MakeNavigatePosData(LPCTSTR pszPostInfo)
		{
			if (!pszPostInfo)
			{
				return FALSE;
			}
			int nSrcLength = lstrlen(pszPostInfo);
			VARIANT vtPost = {0};
			::VariantInit(&vtPost);

			LPSAFEARRAY psa = SafeArrayCreateVector ( VT_UI1, 0, nSrcLength );
			if (psa)
			{
				LPSTR pszPostData = NULL;
				SafeArrayAccessData ( psa, ( void** ) &pszPostData );
#ifdef _UNICODE
				USES_CONVERSION;
				int nLength = WideCharToMultiByte ( CP_ACP, 0, pszPostInfo, 
					nSrcLength, NULL, 0, NULL, NULL );
				CopyMemory ( pszPostData, CT2A ( pszPostInfo ), nLength );
#else
				CopyMemory ( pszPostData, pszPostInfo, nSrcLength );
#endif
				SafeArrayUnaccessData ( psa );
				V_VT ( &vtPost ) = VT_ARRAY | VT_UI1;
				V_ARRAY ( &vtPost ) = psa;

				//CFVariantInfo varInfo(vtPost);
				//FTLTRACE(TEXT("vtPost=%s\n"), varInfo.GetConvertedInfo());

				//使用 vtPost
				 //pWebBrowser->Navigate ( bsURL, &varFlags, &varTarget, &vtPost, &varHeader );

				VariantClear ( &vtPost );
				SafeArrayDestroyData ( psa );
			}

		}
    };

}//namespace FTL

#ifndef USE_EXPORT
#  include "ftlIEHelper.hpp"
#endif

#endif //FTL_IE_HELPER_H
