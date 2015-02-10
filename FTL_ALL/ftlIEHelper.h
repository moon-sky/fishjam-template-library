#ifndef FTL_IE_HELPER_H
#define FTL_IE_HELPER_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlIEHelper.h requires ftlbase.h to be included first
#endif

#ifndef FTL_COM_H
#  error ftlIEHelper.h requires ftlCom.h to be included first
#endif

//理解在保护模式下Internet Explorer的工作方式 --  http://blog.csdn.net/xt_xiaotian/article/details/5336809
//  原文:https://msdn.microsoft.com/zh-cn/library/bb250462(en-us,VS.85).aspx
//IEBlog 简体中文 http://blogs.msdn.com/b/ie_cn/
//允许通过Fiddle调试Metro风格程序的工具: http://blogs.msdn.com/b/fiddler/archive/2011/12/10/fiddler-windows-8-apps-enable-loopback-network-isolation-exemption.aspx

//Cross-Site-Request-Forgery (CSRF)

/****************************************************************************************************************************
* AppContainers (沙盒) -- 定义允许应用使用的特性(TODO:怎么定义? 扩展DLL能定义吗?)
* App capability declarations -- https://msdn.microsoft.com/en-us/library/windows/apps/hh464936.aspx
*   internetClient
*   location
*   sharedUserCertificates
*   
*   internetClientServer -- 可以作为服务器,能接收连接请求
*   privateNetworkClientServer -- 可以访问Intranet资源, (EPM时没有声明改能力,因此可避免 CSRF(Cross-Site-Request-Forgery) 和 Intranet Port Scanning) 
*   enterpriseAuthentication
*   musicLibrary
*   picturesLibrary
*   videosLibrary
*   documentsLibrary
*   
* 
****************************************************************************************************************************/

/****************************************************************************************************************************
* TODO: 
*   1.Protected Mode 和 Enhanced Protected Mode  的区别?
*     PM -- 可以针对四种类型的Sites分别启用或禁用
*     EPM -- 针对整个IE浏览器进行配置
*   2.MotleyFool目前的配置, 在同时启用 PM + EPM 后无法显示，为什么?签名?
*   3.通过 Delete Browser History 可以删除EPM相关的配置信息, 重新提示?
* 
* 保护模式(Protected Mode): Internet选项=>安全=>选择区域后"启用保护模式"
*   受限制的站点 -- 限制最多
*   Internet -- 限制较多, 包括通过 127.0.0.1 访问时
*   本地Intranet(Local Intranet) -- 限制较少(Medium), 包括通过 localhost 访问时
*   受信任的站点(Trusted Zones) -- 限制最少
* 增强保护模式(Enhanced Protected Mode):Internet选项=>高级=>安全=>启用增强保护模式
*   所有的Content Process 为64Bit, 且运行在保护模式, Win8以后运行在AppContainer中.
*   不能接收连接请求，不能连接本地服务(loopback) -- 不能通过Fiddler调试
* AppContainer(windows_ie_ac_001) -- Win8以后, 启用EPM时 Content Process的一种沙盒(sandboxed)表现形式?
*   各个AppContainer等的资源独立 -- 如 Cookies/Cache/Web Storage/IndexedDB 等, 可能造成同一个网站有多份 cookies 实例(Low/Medium/AppContainer)
*   声明了 internetClient, location, sharedUserCertificates 的能力?
*   AppContainer 的限制能力和很多方面都有关系: OS(32/64); Win版本; IE版本和类型(Desktop|Metro); PM|EPM 的设置; 访问的网址类型(Internet|Intranet|Trusted); 
*      网络类型(家庭|公用|工作); 防火墙 等
* LowIL
****************************************************************************************************************************/


/****************************************************************************************************************************
* TODO:
* 1.IELowutil.exe -- Internet Low-Mic Utility Tool, 似乎可以通过COM调用?
* 2.应用程序兼容性工具包(ACT) -- 似乎只针对XP?
*
* Mandatory Label
*   AppContainer
*   mandatory --
*   Medium
*   
****************************************************************************************************************************/

/****************************************************************************************************************************
* EPM -- (已读完)http://blogs.msdn.com/b/ieinternals/archive/2012/03/23/understanding-ie10-enhanced-protected-mode-network-security-addons-cookies-metro-desktop.aspx 
*        回复读到 tommcg1 1 Mar 2013 11:54 AM 
*        https://msdn.microsoft.com/en-us/library/bb250462(v=vs.85).aspx
*        https://msdn.microsoft.com/en-us/library/ie/dn519894(v=vs.85).aspx
* 
* Win8 -- 两种不同的用户接口，需要分别运行不同类型的应用程序（如IE有对应的 Metro style 和 Desktop style)
*   1.经典 -- 桌面应用程序, 64Bit上 64Bit Manager + 32Bit Content Process
*   2.Metro -- 专门的 Metro 程序或 Metro-style enabled desktop browsers (MEDB), 运行在AppContainer中。通过 HTML+JavaScript 开发.
*              Metro模式下的IE只有Flash,Silverlight,Java 插件?
*              不能安装或运行其他的任何插件, Toolbar, BHO, non-browser-platform COM objects(MIME Handler, URLMon Protocol Handler,ActiveX Controls) 等
*              64Bit上只有64Bit Content Process, 
*              注意：其上有"View on the Desktop" 的命令选项,会push 当前的session cookies(不会push persistent cookies)到桌面形式的IE实例，然后访问相同网站
*     WinRT -- ARM平台上的Window8，只能运行纯粹的Metro程序，不能使用Win32API，需要使用专门的 WinRT API?
*
* <iepmapi.h> -- IE Protected Mode API, 其中有 IEIsProtectedModeProcess 等函数的定义(但考虑到 WinXP 等得运行，最好动态加载)
* EPM(Enhanced Protected Mode) -- IE10加入, 启用后很多插件都无法正确运行。Metro类型的始终运行于EPM, Desktop类型的可选(Internet Options->Advanced),
*   Web网站的开发者在 Http Header 中通过"X-UA-Compatible:requiresActiveX=true" 选项要求plugin,用户会得到提示，允许插件运行在 "Low IL Compat" 进程中
*   Win8.0默认Disabled, Win8.1默认Enabled, IE11后来又改为默认禁用
*   Win7上的IE10,如果启用EPM,只会 打开64位Content Process(没有 AppContainer 的沙盒)
*   TODO:EPM 禁用时才能使用 AdminAPI ?
*   BHO|Toolbar和EPM不兼容时, 会提示 " 'Xxx' isn't compatible with Enhanced Protected Mode and has been disabled.
*
*   开启时: 父IE运行在 AppContainer 的 Integrity Level, 成为 Metro Style, 很多高权限的功能会被Deny(Create global event 等函数返回 Access denied)
*   关闭时: 子IE运行在 Low 的 Integrity Level, 会 
*   TODO: 父IE运行在 Medium 的 Integrity Level
*   原理:64位进程的高级内存保护(被crack前crash + 减少堆栈溢出的可能性?) + 完整性级别 + AppContainers(沙盒)
*        ASLR -- 地址空间布局随机化, 通过 /DYNAMICBASE 开启
*   解决方案(TODO:尚未实际测试):
*     0.安装 32 位和 64 位二进制文件 + 将组件注册为 CATID_AppContainerCompatible 类别 -- 即可在EPM启用时加载组件?
*       部分资源访问错误的解决: 可以通过更新资源的安全访问控制列表 (SACL) 来加入 IE AppContainer 的安全 ID (SID) 以解决此问题.
*     1.增加Local COM Server 类型的Broker, BHO(AppContainer) <=> Broker(Medium) <=> 高权限Process(?)
*       TODO: ? Broker及相关的dll需要设置ALL_APPLICATION_PACKAGES权限? 可通过 ConvertStringSidToSid("S-1-15-2-1") + AddAceToObjectsSecurityDescriptor 来增加?
*     2.对应的BHO,Broker 等需要签名;
*     3.Broker.exe 需要注册到 HKLM\SOFTWARE\Microsoft\Internet Explorer\Low Rights\ElevationPolicy 来提升进程到中等完整度
*       Policy: 3(中等完整性静默运行), 2(提示用户允许中等完整性运行),1(低完整性静默运行), 0(阻止运行)
*       如调用者是可执行文件: AppName(可执行调用者文件的文件名) + AppPath(安装路径)
*       如调用者是COM对象: CLSID
*     注意: 8.1的话，需要给CLSID注册AppContainer的category才能让它兼容，另外还要注册64位的ActiveX
*   允许从网页拖拽到指定程序(避免提示):
*     HKEY_LOCAL_MACHINE/SOFTWARE/Microsoft/Internet Explorer/Low Rights/DragDrop
*
* IE 7 的保护模式(Protected Mode) -- 也叫IE低权利(Low Rights) -- TODO: Win7上实际测试IE10, 和理论不同
*   通过 Process Explorer 查看进程的"安全"属性，可看到有
*   "Mandatory Label\Low Mandatory Level"的SID(相应的标志位是"Integrity" -- 完整)，这表明IE进程的完整性级别是"Low", "Virtualized" 为 Yes
*   普通进程是:"Mandatory Label\Medium Mandatory Level", "Virtualized" 为 No
*   保护模式下访问的文件目录(完整性级别都是"Low")：
*     1.IE临时文件: %UserProfile%\AppData\Local\Microsoft\Windows\Temporary Internet Files\Low
*     2.系统临时目录: %UserProfile%\AppData\Local\Temp\Low
*     3.Cookies: %UserProfile%\AppData\Roaming\Microsoft\Windows\Cookies\Low
*     4.历史: %UserProfile%\AppData\Local\Microsoft\Windows\History\Low
*     5.收藏夹: 收藏夹默认允许低级别的IE进程访问，因此不需要独立的目录？
*     6.注册表: HKEY_CURRENT_USER\Software\AppDataLow\Software 等
*     TODO: %userprofile%/AppData/LocalLow , 可通过 SHGetKnownFolderPath(FOLDERID_LocalAppDataLow) 获取,似乎不同的Tab会返回同的结果? 
*   解决兼容性问题：
*     1.通过建立NTFS软链接(Soft Link)的方法来解决IE保护模式的兼容性问题：linkd 源目录 低权限的目的目录
*       实质：给NTFS文件增加一个重解析点(FILE_ATTRIBUTE_REPARSE_POINT)的属性
*       Linkd 可执行程序在 Windows Server 2003 Resource Kit Tools工具包 中
*     2.SHGetSpecialFolderPath(NULL, szSavePath, CSIDL_LOCAL_APPDATA , TRUE); //取得 %UserProfile%\AppData\Local
*       CString strFilePath(szSavePath); 
*       strFilePath += TEXT("\\Temp\\Low\\nCapture\\IEToobar\\");  -- 之后的访问都在具有 Low 完整性等级的目录下
*   TODO:
*     1.PhotoViewer 会在 HKLM\SOFTWARE\Microsoft\Internet Explorer\Low Rights\ElevationPolicy 下写信息(Policy = 3 等)，似乎是为了发消息?
* 
* 代理进程(特权分离思想) -- 把传统的IE进程一分为三，分别负责不同安全等级的任务.
*   IE进程本身 -- 完成最常用的、无需特权的网页浏览任务
*   IeUser.exe(用户级代理进程, User Broker) -- 完成中等特权的任务(例如保存图片等)
*   IeInstal.exe(管理级代理进程 - Admin Broker) -- 完成高特权的任务(例如安装加载项，需要写INF文件?)。
* 
* IE兼容性视图(CV)列表 -- 维护系统下的网站与IE兼容的信息
*   %LocalAppData%\Microsoft\Internet Explorer\IECompatData\
*     ie9CompatViewList.xml IE9)
*     iecompatdata.xml (IE10)
*   桌面模式
*   Windows UI 模式下的IE
*   
*
* IE中的多进程控制: 
*   Manager|Frame process -- Medium(64Bit),不会运行任何第三方的内容
*   Content|Tab Process -- Low(Protected Mode) or Medium(32Bit, Not Protected Mode or Intranet) -- 
*     所有的 HTML, ActiveX, Toolbar(界面上似乎是Manager process) 等都是运行在 Content Process 中的
*     TODO:如果跟访问的站点有关的话，岂不是进程的权限会随时变?
*
*   HKCR\SOFTWARE\Microsoft\Internet Explorer\Main\TabProcGrowth  -- REG_SZ, 为 0 表示单进程? 为 Medium 表示多进程?
*     TODO:需要重启才能生效?
* 
* 相关函数
*   IEIsProtectedModeURL -- 
*   IEIsProtectedModeProcess  -- 判断是否运行在保护模式下
*   IEGetWriteableFolderPath -- 获取指定类型的目录地址。注意: 因为 EPM 开启和关闭时获得地址不一样，所以一般不能通过它来写文件。
*   IELaunchURL -- 使用合适的完整性启动IE访问URL(不要使用 CreateProcess 启动IE?)
*   IESaveFile -- 保存文件到IEShowSaveFileDialog返回的路径中? TODO: 需要现在  %userprofile%/AppData/LocalLow 中创建一个临时文件? 然后保存时指定该文件?
*   IEShowSaveFileDialog -- 示用户保存文件到的路径(高完整性上下文中?)
*   TODO: 有 AppContainer 的函数?
****************************************************************************************************************************/

/****************************************************************************************************************************
* IWebBrowser2::Navigate 的 PostData 是个 VT_ARRAY 的 Variant，例子 MakeNavigatePosData（未确认）
* IDownloadManager -- 微软提供的接口标准，用户可以实现该接口方法(通过 QueryService 提供给外界使用)，
*   从而实现定制化的下载管理器(如显示视频?)
*  
****************************************************************************************************************************/

/****************************************************************************************************************************
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
****************************************************************************************************************************/




namespace FTL
{
    enum IETabIntegrityLevel{
        ilAppContainer,
        ilLowCompat,  //网页要求 ActiveX control which is not EPM-compatible 时
        ilLow, 
    };

    class CFIEUtils
    {
    public:
		//从Window窗体句柄获取IE内的Docuemnt接口
		FTLINLINE static HRESULT GetIEDocumentFromHWnd(HWND hWnd, IHTMLDocument** ppDocument);

		FTLINLINE static BOOL IsProtectedModeProcess();
        
        FTLINLINE static IETabIntegrityLevel GetIETabProcessIntegrityLevel(HANDLE hProcess);

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
