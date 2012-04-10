#ifndef ADD_IN_STUDY_H
#define ADD_IN_STUDY_H
#pragma once

//CodeBlocks -- 开源的跨平台 C++ IDE，可以学习相关的知识
//  其中有 ICodeCompletionItemsProvider 接口?

//DTE中 ProjectItem -> FileCodeModel -> CodeElement->CodeElementChildren ?

/*********************************************************************************************************
* 问题
*   1.C# 中有一个 CommandBar.resx 文件， C++/ATL 对应的是什么? 使用 CultureInfo 来管理多语言，C++/ATL如何? 
*   2.命令目标(IDTCommandTarget)的执行(Exec)和状态获取是通过比较字符串的方式来执行的,有没有更好的方式？
*
*
* 引用的命名空间 ，注意：EnvDTE[x0]定义了自动化对象模型(AOM--Automation Object Model)中的类型
*     AddInDesignerObjects;
*     EnvDTE(Environment Development Tools Extensibility)--开发环境工具扩展, 对应VS2005之前的DTE版本(如 DTE 接口)
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
*     EnvDTE80 -- 对应于VS2005的DTE(只包含变化了的版本，如 DTE2 接口)，相应的还有 EnvDTE90 等
*     Microsoft_VisualStudio_CommandBars;
*     Extensibility (C#中才有?) -- 定义了IDTExtensibility2使用的类型
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
* http://tortoisesvn.net/docs/nightly/TortoiseSVN_zh_CN/index.html
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
*   LearnVSXNow -- http://www.cnblogs.com/default/archive/2010/02/26/1674582.html
*   需要安装 Visual Studio 200X 的 SDK(VSX SDK) -- http://msdn.com/vsx 
* 
* 常用Service -- Visual Studio提供给Add-in和Package的功能，Package也能提供(Proffer)自己的Service
*   1 : IVsObjectManager
*   2 : _DTE
*   3 : IVsCommandWindow
*   4 : IVsRegisterFindScope
*   5 : IVsAddProjectItemDlg
*   6 : IVsStructuredFileIO
*   7 : IVsInvisibleEditorManager
*   8 : ICQryAutoFactory
*   9 : IVsHTMLConverter
*   10 : IVsLog
*   11 : IVsFileChange(SVsFileChange)
*   12 : IUIHostLocale
*   13 : IVsEnumHierarchyItemsFactory
*   14 : IHxSession
*   15 : IHxRegisterSession
*   16 : IVsUIShellOpenDocument
*   17 : IVsIntelliMouseHandler
*   18 : IVsParseCommandLine
*   19 : SVsProfilesManagerUI
*   20 : IVsFontAndColorStorage
*   21 : SVsOnlineHelpData
*   22 : SVsSearchService
*   23 : Help
*   24 : SVsHelpPage
*   25 : IVsOutputWindow
*   26 : IVsRegisterScciProvider
*   27 : IVsMonitorSelection
*   28 : SVsWebProxy
*   29 : IVsComponentSelectorDlg
*   30 : SVsHelpOnlineF1Service
*   31 : IVsSolutionPersistence
*   32 : IVsLibraryReferenceManager
*   33 : IVsCmdNameMapping
*   34 : IVsToolbox(SVsToolBox)
*   35 : IVsRegisterEditors(SVsRegisterEditors) -- 
*   36 : IInternetSecurityManager
*   37 : IVsSolution
*   38 : IVsdDeployment
*   39 : ObjectExtenders
*   40 : IVsSolutionBuildManager
*   41 : IVsFileChangeEx
*   42 : IVsMonitorUserContext
*   43 : IVsTaskList
*   44 : IVsBuildEventCommandLineDialogService
*   45 : SVsStartPageDownload
*   46 : IVsRunningDocumentTable
*   47 : IVsdPackageManager
*   48 : IVsUIShell(SVsUIShell) -- 提供了若干方法去实现与界面有关的功能
*   49 : IVsDiscoveryService
*   50 : IVsPropertyPageFrame
*   51 : IVsAddWebReferenceDlg
*   52 : IVsFilterKeys
*   53 : IVsTextImageUtilities
*   54 : IVsAppCommandLine
*   55 : IProfferService
*   56 : IVsXMLMemberIndexService
*   57 : IVsStatusbar
*   58 : IVsUIHierWinClipboardHelper
*   59 : IVsExternalFilesManager
*   60 : IVsMenuEditorFactory
*   61 : IVsdDeployment
*   62 : IVsRegisterProjectTypes
*   63 : SourceControl
*   64 : IVsCodeShareHandler
*   65 : SVsLinkedUndoTransactionManager
*   66 : IVsShell(SVsShell) --
*   67 : OleMenuCommandService(IMenuCommandService) -- 有 AddCommand 等方法 
*   68 : STrackSelection
*   69 : SVsWindowFrame
*
* 常见接口
*   IVsPackage -- 必须有 PackageRegistration 的属性？ VSX中有名为 Package 的基础实现？
*     CLR的Package必须强命名，需要Key文件。为了性能，Package只有在需要其功能的时候才会加载。
*     IVsPackage 的 Site 即是 IDE，Package可以通过 IServiceProvider 接口获取到各种Services
*   CommandSet? -- 命令集
*  
* 常用工具
*    $(RegitPath)\Regit.exe/regpkg.exe -- Package安装以及注册工具，读取 Package 子类中的各种 Attribute 来注册
*
* 注册/取消注册
*   VS通过PLK（Package Load Key -- 128个字符的数字散列码，从微软站点 http://msdn.microsoft.com/vstudio/extend/ 获得）
*   来检查发布版本的扩展包是否合法(开发时使用VSIP的调试许可证)，
*   通过 ProvideLoadKey(最小版本,版本号,名字,开发者名字,PLK的资源ID) 属性提供
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
*     /RANU 
*     /setup -- 可以在启动的splash screen 获得 logo and product name
*     /NoVsip -- 模拟没有安装 VS SDK 的机器的行为
*     /root:Software\Microsoft\VisualStudio\9.0Exp --指定注册的注册表根位置
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
**********************************************************************************************************/

#endif //ADD_IN_STUDY_H