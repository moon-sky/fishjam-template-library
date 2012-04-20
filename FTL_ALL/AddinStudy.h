#ifndef ADD_IN_STUDY_H
#define ADD_IN_STUDY_H
#pragma once

//开发文档
//Visual Studio 2008 可扩展性开发 -- http://www.cnblogs.com/anderslly/archive/2009/02/23/visualstudio-overview.html
//LearnVSXNow -- http://www.cnblogs.com/default/archive/2010/02/26/1674582.html

//CodeBlocks -- 开源的跨平台 C++ IDE，可以学习相关的知识
//  其中有 ICodeCompletionItemsProvider 接口?

//Visual Assist(us$249) -- http://www.wholetomato.com
//VSAid(Freeware) -- http://www.brocksoft.co.uk/vsaid.php 
//PhaStudio(OpenSource,C#) -- http://phatstudio.sourceforge.net
//Visual Studio Flat Solution Explorer(OpenSource, C++) http://vsflatsolutionexp.codeplex.com/
//DPack(Freeware) -- http://www.usysware.com/dpack

//DTE中 ProjectItem -> FileCodeModel -> CodeElement->CodeElementChildren ?
//CAtlDllModuleT 子类中 DECLARE_LIBID(idl中library的UUID) -- 作用 ?
//   需要先通过 DECLSPEC_UUID 生命对应的UUID
//idl 中 library(1) -> coclass(n),可通过 CoCreateInstance 创建 -> interface(n)
//最简单的COM组件：
//  1.需要从 CComObjectRootEx<线程模型>, CComCoClass<类名,CLSID_XXX> 继承
//  2.DECLARE_REGISTRY_RESOURCEID(.rgs文件对应的ResourceID)
//  3.BEGIN_COM_MAP/END_COM_MAP 设置接口映射
//  4.OBJECT_ENTRY_AUTO(CLSID_XXX, 类名) 映射

//  COM_INTERFACE_ENTRY2 -- 宏的使用(多个IDispatch? ) COM_INTERFACE_ENTRY2(IDispatch, IDTExtensibility2)
//
//SetScrollPos(m_handles.vert, SB_CTL, line, TRUE); -- 使用 SB_CTL 是什么意思? 
/*********************************************************************************************************
* 问题
*   1.C# 中有一个 CommandBar.resx 文件， C++/ATL 对应的是什么? 使用 CultureInfo 来管理多语言，C++/ATL如何? 
*   2.命令目标(IDTCommandTarget)的执行(Exec)和状态获取是通过比较字符串的方式来执行的,有没有更好的方式？
*
*
* 常见的需要 import 的 libid
*   AC0714F2-3D04-11D1-AE7D-00A0C90F26F4(1.0) -- AddInDesignerObjects(MSADDNDR.tlh)
*     主要定义 _IDTExtensibility2(DTE Addin 的基本父接口) 等接口
*     public IDispatchImpl<_IDTExtensibility2, &IID__IDTExtensibility2, &LIBID_AddInDesignerObjects, 1, 0>,
*   80CC9F66-E7D8-4DDD-85B6-D9E6CD0E93E2(8.0) -- EnvDTE(Environment Development Tools Extensibility)
*     开发环境工具扩展, 对应VS2005之前的DTE版本(如 DTE 接口), EnvDTE[x0]定义了自动化对象模型(AOM--Automation Object Model)中的类型
*       Solution(解决方案和项目)
*       Command(命令)，分三层结构
*         触发者，人机接口，如 菜单/工具栏/快捷键 等
*           => 命令(Command), 检查状态(如是否可见、可用等)，可直接执行或转给命令目标
*             => 命令目标(IDTCommandTarget),实现具体的动作，如文本编辑器中合解决方案上的 Edit.Copy 命令执行内容不相同
*         DTE -> get_CommandBars(获取主菜单接口) -> get_Item("MenuBar") -> get_Controls -> get_Item("Tools")获取CommandBarPopup
*         VS内置的菜单项按照菜单的嵌套关系进行命名，如 "Edit.Copy" 表示Edit下的Copy项(有空格的话需要去掉)，
*           自定义Addin的菜单命令为：ProjectName.ClassName.CommandName(如 FtlLogAddin.Connect.DoLogAddin)
*         VS 上面的主菜单：MenuBar； 
*         Doc 上右键菜单：Easy MDI Document Window(类名为 EzMdiTabs)
*         通过 DTE::ExecuteCommand(名字",xxx) 即可执行VS已有的标准命令(如 Window.CloseAllDocuments )
*       事件
*       文档
*       调试器
*   1A31287A-4D7D-413E-8E32-3B374931BD89(8.0) -- EnvDTE80, 对应于VS2005的DTE(只包含变化了的版本，如 DTE2 接口)，相应的还有 EnvDTE90 等
*   1CBA492E-7263-47BB-87FE-639000619B15(8.0) -- Microsoft_VisualStudio_CommandBars
*   xxxxx -- Extensibility (C#中才有?) -- 定义了IDTExtensibility2使用的类型
*
* ContextUIGUIDs -- VS定义了一些GUID值来标识其状态的改变。通过函数中的该参数，可指定是否在指定的状态下(vsContextGuidDebugging)启用等
*********************************************************************************************************/

/*********************************************************************************************************
* 编写 DTE时，出现 error C2872: 'ULONG_PTR' : ambiguous symbol 错误
*   解决：在引入DTE 的类型库(libid:80cc9f66-e7d8-4ddd-85b6-d9e6cd0e93e2)时，将ULONG_PTR改名，
*   后面增加 rename("ULONG_PTR","ULONG_PTRDTE") ，
*   然后还需要注意头文件的包含顺序，不要在 Connect.h 中 using namespace XXX 后再包含那些会冲突的文件(尽量往前放)
* 
* 调试
*   C#的 Addin项目中有一个 ${ProjectName}-For Testing.AddIn 文件，这个文件存放的位置是
*     ${My Documents}\Visual Studio 2008\Addins，按F5测试时，VS使用该文件进行加载。
*     开发完毕后，将 Testing.AddIn 删除，将生成的程序集和结果文件放到 {$My Documents} 对应目录下即完成最简单的部署?
*
* Add-in 中的多线程
*   1遵循COM的线程传递接口规则，需要将DTE的接口 Marshal 到子线程中，然后即可正常使用
*   2.多线程同时操作同一个接口时的同步问题？自动同步？
*********************************************************************************************************/


/*********************************************************************************************************
* http://www.cnblogs.com/sifang2004/archive/2006/06/26/436076.html
*
* Visual Studio 提供了多种不同级别的扩展：
*   Macro(宏)--将VS中的一些重复任务自动化，可录制和手工编写宏。
*     注意：当前只能用VB.NET(VBA?)来开发宏，和 Add-In 访问相同的API，通过录制宏并分析可帮助Add-In开发
*   Add-In(插件)--访问IDE的底层API，得以将一些编码或部署任务自动化，从而扩展IDE。
*     可以操作解决方案、项目、文件、编辑器和VS中的各个窗口等。
*   VsPackage--通过VS SDK创建，可以达到IDE和编程语言之间更深层次的集成
*   Visualizer--
*   VS Shell--
*   Code Snippet--
*   Project/Item Template
*   MSBuild--
*   Visual Studio行业合作伙伴 (VSIP) 计划。
*   外接程序
*
* Developer Studio Objects 
*
* IDE中的重要窗口
*   Solution Explorer(解决方案资源管理器)--查看和管理解决方案内的各个项目、项目所包含的文件s
*   Property Window(属性窗口)--查看当前选中项的属性
*   Toolbox(工具箱)--包含了若干控件的列表
*   Server Explorer(服务器管理器)--查看和管理本地或远程的服务器
*   Class View(类浏览器)--显示了解决方案内的所有可用类型,并将它们按命名空间分组
*   Error List(错误列表)--显示错误、警告和信息等
*   Output Window(输出窗口)--
*   Task List(任务列表)--可添加两种任务, 一是 "//TODO:xxx"; 另一种是用户自定义任务
*   Object Browser(对象浏览器)--查看所有相关的组件内所包含的类型信息
* 
* 外接程序
*   创建一个插件工程:Other Project Types => Extensibility => Visual Studio Add-in => "选择应用程序宿主"。
*   是 COM 对象，它们实现 IDTExtensibility2 接口，并通过 EnvDTE 和 EnvDTE80 程序集中包含的核心自动化对象模型与 IDE 通信。
*   工具栏模型包含在 Microsoft.VisualStudio.CommandBars 中。使用到的主要对象就是DTE对象，利用它我们能编程控制在
*   Visual Studio中的任何东西，从工具栏，装卸工具窗口，甚至编辑文件和初始化编译。
*
*   接口定义文件 
*     AddIn.idl
*
*   配置文件(C#中是XML格式的 ${ProjectName}.AddIn ),Add-In通过这个文件向VS进行注册, <Version> 节点中"*"表示支持所有版本
*     AddIn.rgs -- 其中的 LoadBehavior 等配置信息保存想到中选择的 宿主环境、加载方式等
*       LoadBehavior -- 0(不自动加载),1(VS启动时加载),4(通过命令行加载)
*       CommandPreload -- 指定Add-In是否应当预先加载
*       CommandLineSafe -- 指定Add-In是否是命令行安全的以及是否显示用户界面
*   
*
**********************************************************************************************************/
 
/**********************************************************************************************************
* C++/ATL 形式的Addin插件框架
*   Connect.h -- 默认实现了 IDTCommandTarget 和 IDTExtensibility2 两个接口
* 
* 常用接口
*   IDTExtensibility2 -- 用于在Add-In和IDE之间进行通信，其参数中的 custom 总是空数组 ? 
*     OnConnection : 在加载Add-In时调用,其参数 Application 是AOM的根对象。
*       注意，此时可能有些需要的VS组件尚未加载完成
*     OnStartupComplete : 在宿主环境加载完成的时候，必须将AddIn设置为启动时加载才会触发该事件
*     OnAddInsUpdate : 在任何Add-In的集合状态发生改变(如AddIn Manager中改变任一插件的配置)的时候
*     OnBeginShutdown : 在宿主环境将被关闭的时候
*     OnDisconnection : 在卸载Add-In时调用
*   IDTCommandTarget -- 如果选择了向导中的UI选项，就需要实现它
*     QueryStatus
*     Exec
*   
**********************************************************************************************************/

/**********************************************************************************************************
* MPF(Managed Package Framework) -- 微软在Visual Studio的COM interoperability程序集之上创建的框架，
*   可以帮助我们用“本土化”的托管代码来创建VSPackage
*   
* VSIP(Visual Studio Integration Package，需要链接 vsguids.lib ) -- 扩展Visual studio最强大的工具，
*   整个visual studio的功能就是建立在以Visual studio外壳（shell）为核心的扩展包上。
*   Package可以在Visual Studio的启动界面里或在关于对话框里显示它自己的信息
*   所有的Language、编辑器、调试器、Project System以及其他很多的组件都是Package，扩展：
*     服务（Service） -- 提供功能供开发者或者其他package调用，如 C#语言服务
*     界面元素 -- 例如菜单、工具栏、窗口等
*     编辑器 -- VisualStudio的 核心编辑器 + 扩展的自定义编辑器
*     设计器 -- 通常是可视化工具，如 WinForm设计器
*     项目 -- 组织源文件和资源，编译、调试和发布由源文件创建的产品
*   Package的特点
*     按需加载 -- 只有服务被调用的Package才会加载
*     Site Package -- Package 需要 SetSite，才能通过服务的实例来和Visual Studio交互
*     Global Service -- 如有已经被site的IVsPackage对象，即可通过GetService方法去访问全局服务,
*       C#中有静态的 Package.GetGlobalService 方法，使用服务前需要注册服务(C#中是ProvideServiceAttribute)
* 
*   需要安装 Visual Studio 200X 的 SDK(VSX SDK) -- http://msdn.com/vsx 
* 
* 常用Service(IServiceProvider::QueryService) -- Visual Studio提供给Add-in和Package的功能，Package也能提供(Proffer)自己的Service
*   注意：因为一个Service可以提供多个接口，因此良好的习惯是单独定义一个 Sxxx(如 SVsResourceManager) 作为 Service 的类型
*   _DTE(SID_SDTE)
*   Help
*   ICQryAutoFactory
*   IHxRegisterSession
*   IHxSession
*   IInternetSecurityManager
*   ILocalRegistry/ILocalRegistry4(SID_SLocalRegistry) -- 
*   IOleComponentUIManager(SID_SOleComponentUIManager) --
*   OleMenuCommandService(IID_IMenuCommandService) -- 未测试
*   IProfferService(SID_SProfferService) -- 向VS提供服务，如自定义的 IVsTextMarkerTypeProvider
*     C# 的服务需要实现 Microsoft.VisualStudio.OLE.Interop.dll 中的IServiceProvider(注意：不是 System 中的),从 Package 继承即可。
*        通过 ProvideService 属性提供全局服务，[ProvideService(typeof(SMyService))]
*   IUIHostLocale
*   IVsAddProjectItemDlg
*   IVsAddWebReferenceDlg
*   IVsAppCommandLine
*   IVsBuildEventCommandLineDialogService
*   IVsCmdNameMapping
*   IVsCodeShareHandler
*   IVsCommandWindow
*   IVsComponentSelectorDlg
*   IVsdDeployment
*   IVsdDeployment
*   IVsDiscoveryService
*   IVsdPackageManager
*   IVsEnumHierarchyItemsFactory
*   IVsExternalFilesManager
*   IVsFileChange(SVsFileChange)
*   IVsFileChangeEx
*   IVsFilterKeys
*   IVsFontAndColorStorage
*   IVsHTMLConverter
*   IVsIntelliMouseHandler(SID_SVsIntelliMouseHandler) -- 
*   IVsInvisibleEditorManager
*   IVsLibraryReferenceManager
*   IVsLog
*   IVsMacroRecorder(SID_SVsMacroRecorder) -- 宏录制器
*   IVsMenuEditorFactory
*   IVSMDCodeDomProvider(SID_SVSMDCodeDomProvider)
*   IVsMonitorSelection
*   IVsMonitorUserContext
*   IVsObjectManager[0-2](SID_SVsObjectManager)
*   IVsOutputWindow
*   IVsOutputWindowPane(SID_SVsGeneralOutputWindowPane) -- 获取的是Output窗体中的 General，其他如何获取? QI?
*   IVsParseCommandLine
*   IVsPropertyPageFrame
*   IVsQueryEditQuerySave2(SID_SVsQueryEditQuerySave)
*   IVsRegisterEditors(SID_SVsRegisterEditors) -- 
*   IVsRegisterFindScope
*   IVsRegisterProjectTypes
*   IVsRegisterScciProvider
*   IVsResourceManager(SID_SVsResourceManager) -- 
*   IVsRunningDocumentTable(SID_SVsRunningDocumentTable)
*   IVsShell(SID_SVsShell) --
*   IVsSolution(SID_SVsSolution)
*   IVsSolutionBuildManager
*   IVsSolutionPersistence
*   IVsStatusbar(SID_SVsStatusbar)
*   IVsStructuredFileIO
*   IVsTaskList
*   IVsTextImageUtilities
*   IVsTextManager(SID_SVsTextManager) -- 支持 IVsTextManagerEvents 等连接点
*   IVsToolbox/IVsToolboxClipboardCycler(SID_SVsToolbox)
*   IVsUIHierWinClipboardHelper
*   IVsUIShell(SID_SVsUIShell) -- 提供了若干方法去实现与界面有关的功能
*   IVsUIShellOpenDocument(SID_SVsUIShellOpenDocument) -- 可通过其 IsDocumentOpen(NULL, 0, bstrDocumentPath, LOGVIEWID_TextView,
*      0, &spVsUIHierarchy, &itemId, &spVsWindowFrame, &isOpen) 的方式来获取已经打开的文档的控制接口。 bstrDocumentPath 可通过 Document 等获取
*   IVsWindowFrame(SID_SVsWindowFrame)
*   IVsXMLMemberIndexService
*   ObjectExtenders
*   OleMenuCommandService() -- 有 AddCommand 等方法
*   SourceControl
*   STrackSelection
*   SVsHelpOnlineF1Service
*   SVsHelpPage
*   SVsLinkedUndoTransactionManager
*   SVsOnlineHelpData
*   SVsProfilesManagerUI
*   SVsSearchService
*   SVsStartPageDownload
*   SVsWebProxy
*
* 常见接口
*   IVsPackage -- 必须有 PackageRegistration 的属性？ VSX中有名为 Package 的基础实现类 IVsPackageImpl。
*     CLR的Package必须强命名，需要Key文件。为了性能，Package只有在需要其功能的时候才会加载。
*     IVsPackage 的 Site 即是 IDE，Package可以通过 IServiceProvider 接口获取到各种Services
*   CommandSet? -- 命令集
*  
* 常见父类
*   IVsPackageImpl -- 
*     可重载的方法(不要直接重载 SetSite 方法): 
*       void PostSited(IVsPackageEnums::SetSiteResult result)
*       void PreClosing()
* 
* 常用工具
*    $(RegitPath)\Regit.exe/RegPkg.exe -- Package安装以及注册工具，读取 Package 子类中的各种
*      Attribute(如 DefaultRegistryRoot/PackageRegistration) 来注册
*
* 注册/取消注册
*   VS通过PLK（Package Load Key -- 128个字符的数字散列码，从微软站点 http://msdn.microsoft.com/vstudio/extend/ 获得）
*   来检查发布版本的扩展包是否合法(开发时使用VSIP的调试许可证)，
*   C# 通过 ProvideLoadKey(最小版本,版本号,名字,开发者名字,PLK的资源ID) 属性提供
*   PLK主要有三个部分组成：
*     1. .rgs 文件中的 CompanyName/ProductName/ProductVersion/MinEdition/ID(如 val ID  = d '%IDS_BASICPACKAGE_LOAD_KEY%') 等
*     2. VSL_REGISTRY_MAP_NUMBER_ENTRY(IDS_BASICPACKAGE_LOAD_KEY)
*     3. 关联资源的 IDS_BASICPACKAGE_LOAD_KEY(Hash的字符串)、IDS_BASICPACKAGE_REGISTRY_NAME 宏
* 
*   取消注册时使用 unregister 参数调用，会调用 VSDllUnregisterServer
*     0.定义GUID的 guids.h 文件中不要使用 #pragma once, 其 DEFINE_GUID 定义的 CLSID_XXX 等需要在 #include <initguid.h> 前后各包含一次
*     在 Package 的实现文件中：
*     1.#define DEFAULT_REGISTRY_ROOT XXXX 宏， 如 LREGKEY_VISUALSTUDIOROOT L"Exp"
*     2.#include <VSLPackageDllEntryPoints.cpp>
* 
*
*  Microsoft Visual Studio 2008 Experimental hive -- VS2008实验室，devenv.exe /rootSuffix Exp /RANU
*     会使用独立的注册表信息进行调试，防止影响VS的开发环境 -- 通过编译时的 regpkg 命令行?
*     /RANU -- run as normal user( 注册到 HKEY_CURRENT_USER ?)
*     /setup -- 让VS在启动时自动检测组件的 安装、卸载、升级等
*     /NoVsip -- 模拟没有安装 VS SDK 的机器的行为，主要用于验证 PLK ?
*     /rootSuffix Exp -- 告诉Visual Studio使用 experimental registry hive
*     /root:Software\Microsoft\VisualStudio\9.0Exp --指定注册的注册表根位置
*     /Splash -- 长时间显示Splash窗体，以进行确认
*  vsct -- Visual Studio Command Table, VS2008中定义用户界面的XML，编译成二进制资源后以1000作为资源ID添加到VSPackage.resx资源文件中，
*     regpkg.exe注册时会将相关内容注册到注册表中(位置?)，VS启动时只需读取注册表即可更新界面（无需加载DLL）。
*    CommandTable -> Commands -> Groups  -> Group  ->
*                             -> Buttons -> Button -> Strings 
*                             -> Bitmaps -> Bitmap
*                 -> Symbols  -> GuidSymbol(定义Guid的符号名)
* ctc -- Command Table Configuration, VS2005时保存UI信息的文本文件
* 
* 集成测试项目
* 单元测试项目
*
* ToolWindow -- IDE和界面之间的联系是通过重写Window属性实现的，Window属性是一个实现了IWin32Window接口的对象，它返回的就是MyControl的实例
*   安全性：using System.Security.Permissions;
*           UIPermission(SecurityAction.LinkDemand, Window = UIPermissionWindow.AllWindows)]
*
*
* ProcessMnemonic -- 处理"&"(助记符号)?
*
* Customizing the Text Editor
*   File extension --> Language Service
*   Features (IVsLanguageInfo)
*     Text colorization
*     Statement completion
*     Method tips
*     Special markers
*     Red squigglies
*     Custom commands (& context menus)
*     Hidden Text (concealed & expand/collapse)
*     CodeWindow dropdown bar
*
* 
* 资源映射宏
*   VSL_BEGIN_REGISTRY_MAP(IDR_REGISTRYSCRIPT)
*     VSL_REGISTRY_MAP_GUID_ENTRY -- 声明各种GUID，如 ToolWindow、TaskList 等
*     VSL_REGISTRY_MAP_GUID_ENTRY_EX
*     VSL_REGISTRY_MAP_NUMBER_ENTRY -- 可以指定 PLK 的 Resouce ID
*     VSL_REGISTRY_RESOURCEID_ENTRY -- 
*     VSL_REGISTRY_MAP_TEMPLATE_PATH_ENTRY
*   VSL_END_REGISTRY_MAP()
*
* 命令映射宏
*   VSL_BEGIN_COMMAND_MAP()
*     VSL_COMMAND_MAP_ENTRY(cmdSet的Guid, cmdId, 查询状态函数, 处理函数) --
*       系统提供的 cmdSet 有 CMDSETID_StandardCommandSet97、CMDSETID_StandardCommandSet2K 等
*   VSL_END_VSCOMMAND_MAP()
*
* 消息映射 -- 在ATL::BEGIN_MSG_MAP 的基础上增加了 TRY，理论上说也可以用 WTL
*   VSL_BEGIN_MSG_MAP(className)
*     CHAIN_MSG_MAP(基类)
*   VSL_END_MSG_MAP()
*
* IVsPackage::CreateTool 创建 ToolWindow 的映射宏
*   VSL_BEGIN_TOOL_MAP()
*     VSL_TOOL_ENTRY(ToolWindow的Guid, 创建函数)
*   VSL_END_TOOL_MAP()
*
**********************************************************************************************************/

#endif //ADD_IN_STUDY_H