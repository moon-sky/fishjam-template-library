#pragma once
/**************************************************************************************************************
* 广告类型：
*   百度: tn=xxxx
*   
**************************************************************************************************************/

/************************************************************************************************
* 开发工具
*   Internet Explorer Developer Toolbar(http://www.microsoft.com/en-us/download/details.aspx?id=18359)
*     微软推出的开发工具条，可以 浏览和修改Web页的文档对象模型 等。
*     安装好以后：Tools-> Developer Tool，快捷键是 F12
*   
* 
* mshtml.h(IE 引擎，Trident)
*
*
* IE浏览器提供了可编程的对外接口，用于增加自定义的功能的界面接口 
*   使用 RBDeskBand Wizard可以加速开发 -- http://www.codeproject.com/KB/atl/rbdeskband.aspx
*
* 基本的扩展界面接口：
*   1.添加定制菜单项到IE上下文菜单 -- 如 “添加到QQ表情”
*     添加注册表项 -- HKCU\Software\Microsoft\Internet Explorer\MenuExt\ 
*       下添加一个子键(子键名即显示在上下文菜单上的文字)，子键的默认值为一个网页文件的路径，
*       其中包含了菜单项命令激发时将执行操作的脚本程序，可以利用 External.menuArguments 收到当前IE运行状态的参数
*       如 External.menuArguments.document.links 
*       如果要控制定制菜单项的在网页上的显示范围，可以在该子键下添加一个Context二进制值(逻辑或):
*         默认(0x1);图象(0x2);控件(0x4);表(0x8);被选文字(0x10);锚点(0x20)
*   2.添加定制菜单项到<工具>菜单下，如 NetAnts, FalshGet，MSN Message Service 
*     a.创建唯一的GUID，并以其为名称加入到 HKLM\SOFTWARE\Microsoft\Internet Explorer\Extensions\ 下,
*     b.在该子键下创建一个名为 CLSID 的字符串值“{1FBA04EE-3024-11D2-8F1F-0000F87ABD16}”-- IE工具条的类标识码
*     c.MenuText--菜单项显示字串；MenuStatusBar--菜单高亮时状态栏显示的帮助字串
*     d.要响应菜单命令有三种实现方式: Exe文件，脚本文件，COM对象
*       (1).Exe -- 在子键下添加名为Exec的值为Exe文件的路径；
*       (2).脚本文件 -- 添加名为script的值为脚本文件的路径；
*       (3).COM对象 -- 添加名为 ClsidExtension 字符串值为你为此扩展所创建的COM的 GUID,COM 必须实现 IOleCommandTarget 接口,
*           COm对象中每个菜单或按钮命令都以整数标识(OLECMDID_XXX)存在于数组中，该数组有一个唯一的GUID标识，依靠命令的整数标识执行相应的操作。
*   3.添加定制按钮到标准工具栏，如 NetAnts, FalshGet -- HKLM\SOFTWARE\Microsoft\Internet Explorer\Extensions
*     a.基本注册项同定制菜单，但 CLSID的字符串值不同
*     b.BandCLSID -- 关联的Toolbar的CLSID
*       ButtonText--按钮的文本标签
*       Clsid -- 固定为 {E0DD6CAB-2D10-11D2-8F1A-0000F87ABD16} -- 
*       Hot Icon --Hot 图标的路径;
*       Icon--灰色图标路径;
*       'Default Visible'--是否默认可见(YES/NO);
*       MenuStatusBar-- 显示的文本
*       MenuText -- 显示的文本
*   4.添加定制的浏览栏，工具栏 -- 实现三个基本接口 IDeskBand, IObjectWithSite, IpersistStream，可以扩展实现IContextMenu
*     有四种可定制的栏(区别在于组件分类注册，可以通过 IMPLEMENTED_CATEGORY 宏注册？)：
*       水平浏览栏 -- CATID_CommBand
*       垂直浏览栏 -- CATID_InfoBand
*         <IE5以后，CommBand 和 InfoBand 统称为 Explorer Bars，可用于IE或资源管理器>
*       桌面工具栏 -- CATID_DeskBand
*       IE工具栏 -- 在 HKLM\Software\Microsoft\Internet Explorer\Toolbar\ 下以COM类的CLSID为名创建一个字符串值
*
*   5.浏览器插件(ATL DLL) -- Browser Helper Objects(BHO)，可以监测到IE的各种事件(由于采用连接点的方式通知，因此必须实现 IDispatch 接口，
*     并重载Invoke, 响应IE发送的事件)，BHO适用于Windows系统下的IE浏览器(CLSID_InternetExplorer)和文件浏览器(CLSID_ShellDesktop)
*     a.ATL DLL => New ATL Object => Internet Explorer Object(或 Simple Object, 属性时选中 ConnectionPoint 和 IObjectWithSite)

*     a.HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Browser Helper Objects 下以COM类的GUID为名称创建一个子键
*     b.必须实现 IObjectWithSite 接口，在 SetSite 中，可以获取IE实例的 IWebBrowser2 接口，并获取当前IE的所有信息。
*       要截获事件，需获取 IConnectionPointerContainer 接口，再 FindConnectionPoint (如 DIID_DWebBrowserEvents2),
*       找到某一事件对应的出接口的连接点对象，并用Advise注册(然后可截获浏览器的大量事件，如浏览新地址，前进、后退、生成新窗口等)
*       SetSite 中传入的 IUnknown 接口可以QI出: 
*         IBandSite -- 
*         IDropTarget -- 
*         IInputObject/IInputObjectSite -- , 
*         IOleWindow -- 可获取IE的主窗体, 
*         IOleCommandTarget -- ,
*         IPersist/IPersistStream
*         IServiceProvider -- 可进一步获取的接口:
*            IWebBrowserApp, IShellBrowser, IHTMLWindow2, IWebBrowserEventsService, IHTMLOMWindowServices, 
*            IShdocvwBroker, ITargetFrame, IProfferService
*         
*  
************************************************************************************************/

/**************************************************************************************************************
* BHO(Browser Helper Object) -- 浏览器辅助对象，浏览器对第三方程序员开放交互接口的业界标准，
*   不能在BHO里面自己创建网络连接，这样会导致IE报错崩溃并供出你写的DLL 
*     要在BHO里实现Winsock大概只能在IE休息的时候才可以 ?
* 
* 编写BHO流程
*   1.实现 IObjectWithSite 接口，创建 COM 类型
*   2.在 HKLM\Software\Microsoft\Windows\CurrentVersion\Explorer\Browser Helper Objects\ 中注册BHO对象
*
* 浏览器劫持 -- 更改主页,弹窗(名弹、安弹、智能弹),挟持跳转用户输入的URL，网址字段替换,修改当前网页代码,统计后台
*   启用SetSite方法强制IE跳转到程序员设定的页面去
*   利用 about:blank 篡改主页的“浏览器劫持”方法
*
* IURLSearchHook -- 等待IE创建IURLSearchHook来启动，被浏览器用来转换一个未知的URL协议地址
*   当浏览器企图去打开一个未知协议的URL地址时，浏览器首先尝试从这个地址得到当前的协议，
*   如果不成功，浏览器将寻找系统里所有注册为“URL Search Hook”（资源搜索钩子，USH）的对象并把这个IE不能理解的地址发送过去
* 
* Winsock LSP(Windows Socket Layered Service Provider) -- 分层服务提供商, Winsock2.0中提供，
*   支持SPI(Service Provider Interface, 服务提供商接口)，依赖于系统商已经存在的基本协议提供商(如TCP/IP协议),
*   在这些协议上派分出的子协议即为“分层协议”，如SSL，通过 LSP，可以更简单的分析出网络数据(如 Sniffer )
*   
**************************************************************************************************************/
