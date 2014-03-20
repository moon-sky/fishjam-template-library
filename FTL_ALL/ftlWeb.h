#ifndef FTL_WEB_H
#define FTL_WEB_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlWeb.h requires ftlbase.h to be included first
#endif

/*************************************************************************************************************************
* 浏览器引擎
*   Gecko -- Firefox使用,
*   Trident(也称MSHTML) -- IE使用
*   WebKit -- 用于Safari等，前身是KHTML
*     WebKit Ports(移植) -- 目前有 Safari/Chromium/QtWebKit/Android Browser 等多个Ports，
*        在2D绘图、GPU加速、JavaScript引擎、audio/video解码等，都有不同的实现。
*     各种Port的测试：http://trac.webkit.org/browser/trunk/LayoutTests  
*                     http://testthewebforward.org/
*   KHTML -- WebKit的前身?
*
* 现代浏览器的组件
*   HTML、XML、CSS、JavsScript解析器
*   Layout
*   文字和图形渲染
*   图像解码
*   GPU交互
*   网络访问
*   硬件加速
*************************************************************************************************************************/


/*************************************************************************************************************************
* DHTML -- Dynamic HTML
* Nginx -- 高性能的 HTTP 和 反向代理 服务器，也是一个 IMAP/POP3/SMTP 代理服务器，常作为负载均衡服务器。
*   不支持使用一致性哈希来选择多个Memcache后端(可使用第三方模块? 或使用 Membase替代MemCache ?)
* Rails -- 用于开发数据库驱动的网络应用程序的完整框架，基于MVC设计模式，使用Ruby开发？
* MemCache -- 高性能的分布式的内存对象缓存系统
*   优势：配置简单；nginx内建对memcache后端的支持，支持失效
*
*************************************************************************************************************************/

/*************************************************************************************************************************
* WebService -- 实现异构程序之间方法调用的一种机制
*   各应用程序通过网络协议和规定的一些标准数据格式（Http,XML,Soap)来访问WebService,通过其内部执行得到所需结果
*   基本的 Web Services 平台是 XML+HTTP，使用 XML 来编解码数据，并使用 SOAP 来传输数据
*   SOAP(Simple Object Access Protocol) -- 简单对象访问协议，是基于XML的协议，主要用于应用程序之间的通信
*     由信封(<soap:Envelope>), SOAP头(<soap:Header>), 消息体(<soap:Body>) 组成，如有异常，则在body中需要包含<soap:Fault>
*   UDDI(Universal Description, Discovery and Integration) -- 通用描述、发现及整合，是一种存储WebService的目录服务(只在.NET中?)
*   WSDL(Web Services Description Language) -- Web服务描述语言，基于XML，用于描述和定位WebServices
* 
* App Speed Index(全世界的网站数据分析?) -- http://newrelic.com/asi
* 
* Web 服务器的架构(重点需要考虑扩展)
*   1. Nginx + Memcache + N个Rails + Mysql  -- 
* 
* 用户行为分析(糗百等)和对策 -- https://github.com/qiushibaike/moumentei(糗百的旧源码)
*   a.50%~80%访客不登陆 -- 未登录用户直接返回缓存内容
*   b.首页等几个页面占了50%以上访问量
*   c.页面上80%内容是不变的，剩下的主要为用户相关信息 -- 先载入相对不常变得缓存内容，然后加载变化内容
*************************************************************************************************************************/


/************************************************************************************************************************
*   
* OSR(Off Screen Rendering) -- 离屏渲染，不创建真窗口，将整个页面渲染到一张位图上面。但目前不能使用GPU加速
*   通过离屏渲染 + Windows的Layered Window， 可制作不规则的网页窗口(网页不透明区域是什么形状，窗口就是什么形状)
*
* 浏览器嵌入窗口程序解决方案
*   IE -- 通过ActiveX方式，将IWebBrowser2对象嵌入到窗口中，使用连接点与浏览器内部连接，获取和处理事件
*     缺点：用户的操作系统各种各样，且内核也是多个版本且不兼容(IE9+)才支持HTML5，程序不稳定，不能跨平台。
*   Firefox -- XULRunner是一个跨平台的浏览器开源应用框架，也是Firefox和ThunderBird的核心，使用XPCOM方式实现，已有第三方项目将其封装称控件
*     https://developer.mozilla.org/en-US/docs/XULRunner
*   
*   Chrome(CEF:Chromium Embedded Framework) --  https://code.google.com/p/chromiumembedded/
*     最好的解决方案。支持Windows、Linux、Mac等平台，有多种语言的移植版本。
*     提供的如下特性：自定义插件、自定义协议(比如Q+中qplus://)、自定义JavaScript对象和扩展，支持HTML5，
*     缺点：体积太大，缓存设计成只能有一个进程读写(每个进程实例指定一个不同的缓存文件夹)，
*     分了几个版本：
*       CEF1 -- 使用Chromium WebKit API 的单进程实现，QQ/有道等都用的这个版本，但似乎会被停止，只支持NPAPI插件
*       CEF2 -- 使用Chromium browser 的多进程实现，目前已停止维护
*       CEF3 -- 使用Chromium Content API 的多进程实现，使用异步消息的方式来完成主进程和其他进程的通信，支持NPAPI和PPAPI插件
*         TODO：似乎很好，但为什么没有人使用？
*     http://blog.csdn.net/daoxwj/article/details/8786911
*     http://blog.csdn.net/milado_nju/article/details/7455373
*     CEF笔记：http://www.cnblogs.com/haippy/archive/2013/06/10/3131373.html
*   
*   Q+:在Web App的概念下，为Web页面提供了一个运行环境(简单来说就是：Client的一个框和一些可用的API)，支持IE和Webkit内核。
*      
* 
*   QtWebkit 和 CEF 对比： http://www.magpcss.org/ceforum/viewtopic.php?f=6&t=395
*************************************************************************************************************************/

/*************************************************************************************************************************
* CEF3源码分析
* 跨平台支持？ 是在 cef_win.h 文件中定义
*   CefWindowHandle -- 窗体句柄(HWnd), 
* 引用计数
*   采用CefRefPtr<CefBase子类>来实现生存期管理，各个 CefBase子类 具体实现类中通过 IMPLEMENT_REFCOUNTING 宏实现引用计数的函数
*  
* 常见类
*   CefApp -- 整体的进程类，通常要从该类继承?
*   CefBrowser -- 代表一个浏览器对象，可以获取其中的各个CefFrame,给指定browser/render进程发送特定消息。
*     在browser进程中该类的方法可以被任意线程调用。在render进程中只能在主线程被调用
*   CefBrowserHost -- 在浏览器窗口来看代表了browser进程，暴露了与浏览器窗口相关的接口
*     CreateBrowser -- 创建浏览器对象
*   CefCommandLine -- 命令行，通过其静态 CreateCommandLine 方法创建，InitFromString(::GetCommandLineW()) 初始化
*   CefDOMNode
*   CefDOMVisitor -- 
*   CefClient -- 回调管理类，可通过重载其函数提供自定义回调, CefCreateBrowser/CefCreateBrowserSync 创建实例
*   CefFrame -- 表示浏览器窗口中的一个frame，可加载URL，获取页面的源码和文本,URL,V8执行上下文,访问页面中的DOM 等
*   CefProcessMessage -- 
*   CefRequest -- 
*   CefSettings
*
* 各种处理回调(大量采用类似观察者的回调Handler来处理各种事件)
*   CefBrowserProcessHandler
*   CefRenderProcessHandler -- 实现Render处理的回调
*
* V8脚本引擎
*   CefRegisterExtension("v8/app", ) -- 注册可以和原生应用程序代码交互的Javascript扩展(
*   CefV8Handler -- 用户从该类中继承，并根据name等进行处理
*   CefV8Context -- 
*   CefV8ValueList
*   CefV8Value
*   
* 调试
*   命令行参数：
*     -single-process -- 单进程模式，这样才能够设置断点？
*   CefTraceClient -- 采集Trace通知
*   
* 线程&进程
*   1.当前线程运行环境判断 -- CefCurrentlyOn(TID_XX)， 如 TID_UI 表示在UI主线程内
*   2.CefPostTask -- 执行异步？
*************************************************************************************************************************/

namespace FTL
{
}
#endif //FTL_WEB_H

#ifndef USE_EXPORT
#  include "ftlWeb.hpp"
#endif 