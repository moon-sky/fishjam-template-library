#ifndef FTL_WEB_H
#define FTL_WEB_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlWeb.h requires ftlbase.h to be included first
#endif


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
* WebService
*   各应用程序通过网络协议和规定的一些标准数据格式（Http,XML,Soap)来访问WebService,通过其内部执行得到所需结果
*   基本的 Web Services 平台是 XML+HTTP，使用 XML 来编解码数据，并使用 SOAP 来传输数据
*   SOAP(Simple Object Access Protocol) -- 简单对象访问协议，是基于XML的协议，主要用于应用程序之间的通信
*   UDDI(Universal Description, Discovery and Integration) -- 通用描述、发现及整合，是一种存储WebService的目录服务(只在.NET中?)
*   WSDL(Web Services Description Language) -- 网络服务描述语言，基于XML，用于描述和定位WebServices
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
* 浏览器嵌入窗口程序解决方案
*   IE -- 通过ActiveX方式，将IWebBrowser2对象嵌入到窗口中，使用连接点与浏览器内部连接，获取和处理事件
*     缺点：用户的操作系统各种各样，且内核也是多个版本且不兼容(IE9+)才支持HTML5，程序不稳定，不能跨平台。
*   Firefox -- XULRunner是一个跨平台的浏览器开源应用框架，也是Firefox和ThunderBird的核心，使用XPCOM方式实现，已有第三方项目将其封装称控件
*     https://developer.mozilla.org/en-US/docs/XULRunner
*   Chrome(CEF:Chromium Embedded Framework) --  https://code.google.com/p/chromiumembedded/
*     最好的解决方案。支持HTML5，跨平台。
*     分了几个版本：
*       CEF1 -- 使用Chromium WebKit API 的单进程实现
*       CEF2 -- 使用Chromium browser 的多进程实现
*       CEF3 -- 使用Chromium Content API 的多进程实现
*     http://blog.csdn.net/daoxwj/article/details/8786911
*     http://blog.csdn.net/milado_nju/article/details/7455373
*************************************************************************************************************************/
namespace FTL
{
}
#endif //FTL_WEB_H

#ifndef USE_EXPORT
#  include "ftlWeb.hpp"
#endif 