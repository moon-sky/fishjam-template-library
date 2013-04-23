#ifndef NPAPI_STUDY_H
#define NPAPI_STUDY_H

//http://andylin02.iteye.com/blog/616098 -- Chrome的多线程模型
// ?? Plugin动态链接库名字必须为xxxWrapper.dll，否则webkit引擎无法识别
//     Plugin动态链接库必须和浏览器或者widget管理器在同一目录下
// ?? Plugin类从 nsPluginInstanceBase 继承? 包含 "pluginbase.h"文件
// FF中查看插件：about:pplugins
/*******************************************************************************************************
* http://mozilla.com.cn/post/21666/
* https://code.google.com/p/npapi-sdk/
* https://developer.mozilla.org/en-US/docs/Gecko_Plugin_API_Reference -- NPAPI
* 
* NPAPI(Netscape Plugin Application Programming Interface) -- 是 Netscape Navigator、Firefox、Google Chrome、
*   Apple Safari、Opera 等Gecko引擎浏览器所使用的插件接口。可以开发跨浏览器的插件，解决浏览器兼容问题。
*   代替旧的 XPCOM 和 LiveConnect-based API，项目名需要以 np 开头，为了适应不同的操作系统，建议全小写，长度控制在8字符内。
*   插件需要放置在指定的目录下？，每个插件可以处理一种或多种MIME格式的数据。
*   插件安装好以后，可以嵌入式、全屏幕和隐式三种可能的形式出现。
*
* NPAPI中的函数类型
*   NPN_XXX(Netscape Side API) -- 浏览器提供给plugin的函数，一般都在NPNetscapeFuncs结构体中
*     NPN_InvokeDefault、NPN_CreateObject、 NPN_RetainObject、NPN_ReleaseObject 
*     NPN_UTF8FromIdentifier、NPN_GetStringIdentifier、NPN_GetIntIdentifier、NPN_Status
*     NPN_GetURL -- 访问指定URL，可以建立新的数据流? 可以通过 "javascript:函数名()" 的方式调用JS的函数
*     NPN_GetURLNotify / NPN_PostURL/NPN_PostURLNotify
*     NPN_GetProperty/NPN_SetProperty -- 获取或设置指定对象的指定属性
*     NPN_GetValue/NPN_SetValue/NPN_InvalidateRect
*     NPN_PluginThreadAsyncCall -- 线程安全地通知浏览器调用插件函数的方式。通常用在多线程环境下，需要初始化 NPNFuncs.pluginthreadasynccall 。
*   NPP_XXX(Plug-in Side API) -- plugin提供给browser的函数，主要用于插件完成一些实际功能，实现后填充NPPluginFuncs到结构体中(供browser回调)
*     NPP_NEW/NPP_Destroy -- 针对Plugin对象的函数
*     NPP_SetWindow -- 设置窗口句柄及窗口相对位置
*     NPP_HandleEvent -- 处理平台相关的事件, 其参数可以转换为 NPEvent(在Windows平台中就是消息)，
*       TODO: NP_Initialize 中 NPNetscapeFuncs->handleevent 为 NULL ? 怎么更改?
*     NPP_URLNotify 
*     NPP_GetValue/NPP_SetValue -- 当plug-in加载后，浏览器调用JS的这个方法
*     NPP_NewStream/NPP_DestroyStream -- 创建及销毁一个 NPStream 的使用实例，plugin和浏览器交互数据时候需要此两个接口
*     NPP_WriteReady/NPP_Write -- 在需要传递数据给plugin的时候，需要此两个接口，分别为获得可传递数据大小和传递数据的接口。
*   NP_XXX() -- 插件提供给浏览器，默认调用的方法，主要用于识别、初始化、终止化插件等，需要在def中导出
*     NP_Initialize、NP_Shutdown 等 -- ? npapi的插件库提供给浏览器的最上层接口，针对Plugin类型的函数
*   NS_XXX() -- 
*
*
* 编写NPAPI插件(动态链接库)
*   a.预定义的编译宏：XP_WIN, _X86_
*   b.需要在def文件中导出如下函数(几乎都是框架提供)：
*     0.NP_GetMIMEDescription(测试无效?) -- browser通过返回值识别plugin， 返回值为 "mime类型:扩展名:描述[; 另外的mime类型 ]" 格式的字符串，
*       需要将版本信息(.rc)StringFileInfo中的 ，BLOCK 由 XXX 改为 040904E4(表明语言为英语,字符集为 "Windows Multilingual", 这样才会加载资源信息),
*        更改或增加如下项(会显示在 "about:plugins" 中)
*       "FileOpenName" -- MIME类型的描述部分(File Opent Template)，如 "Fishjam Template Log(*.ftl)" (多个时通过"|"分隔)
*       "FileExtents" -- MIME类型的后缀部分(多个时通过 "|" 分隔)
*       "FileDescription" -- 文件描述部分, 在"版本"下方
*       "FILEVERSION" -- 版本
*       "ProductName" -- 产品名称部分(最上面的大号黑体字)
*       "MIMEType" -- "MIME 类型" 部分，如 "application/x-yyy"(注意：必须全小写, 多个时通过"|"分隔)， 需要和 NP_GetMIMEDescription 返回值一样
*       "Language(未测试)" -- 使用的语言
*     1.NP_GetEntryPoints
*     2.NP_Initialize
*     3.NP_Shutdown
*     4.[NP_GetValue(测试无效?)], -- 返回插件的一些值(如 pluginName 和plugin Description 等)
*       其他一些规定的函数(?, NP_XXX, NPP_XXX )
*   c.注册：HKLM\Software\MozillaPlugins\任意名字的项(?)，字符串数据"Path"
*     TODO：具体每项的说明？
*
* 查看Plugin是否加载成功: FF(about:plugins), 
* Firefox 的 plugin 运行在 plugin-container.exe 进程中，attach 到该进程上
* Google 是多进程浏览器，设置了断点挨个试验？

*******************************************************************************************************/

/*******************************************************************************************************
* 类型：
*   ScriptablePluginObject ?
*   NPClass -- 一个结构体，里面定义了很多名为 NPXXXFunctionPtr 的函数指针(如 Construct、HasMethod、Invoke、SetProperty等)
*     一套C方式实现的插件接口规范
*   NPError -- 定义的错误码，NPERR_NO_ERROR
*   NPIdentifier -- 属性或方法名等，一般通过 NPN_UTF8FromIdentifier 方法得到 UTF8 格式的 string
*   NPMIMEType --
*   NPP m_pNPInstance; //相当于插件的实例句柄，其内部有 pdata(CPlugin*) 和 ndata() 的两个指针，可以分别转换为插件和浏览器
*   NPStream -- 浏览器和插件通过流(代表了一个URL及其数据的对象)来交换彼此的数据。
*     浏览器创建(浏览器调用NPP_NewStream)的流有以下几种模式
*       1.NP_NORMAL  -- 当有数据可发送时浏览器就把数据发送给plug-in，这些数据可能是以非正常顺序到达的。
*         浏览器通过 NPP_WriteReady 和 NPP_Write 等发送数据，效率较高。
*       2.NP_SEEK -- 随即存取模式， seekable 为 true 时，流中的数据先由插件调用 NPN_RequestRead 加以指明所要获取的数据的范围，
*         然后浏览器通过 NPP_WriteReady 和 NPP_Write 等发送数据，需要远程服务器的支持或浏览器先将流数据存到本地的临时文件中。
*       3.NP_ASFILE -- 浏览器先将整个Url数据存到一个本地文件中，然后通过 NPP_StreamAsFile 将文件名传给插件
*     插件创建(插件调用NPN_NewStream)的流只有一种格式
*       1.NP_NORMAL -- 类似浏览器创建的普通流，但方向相反。在创建流时可以指明流数据的目的地（Web页面中的一个frame或一个新的浏览器窗口等)。
*         通过目的地的指明，插件可以达到和浏览器的无缝连接。
*   NPVariant -- 一般通过 NPN_ReleaseVariantValue 等函数操作
*   NPWindow -- 平台相关的窗体信息(句柄、大小、位置等)
* 
* Npruntime 接口 -- JavaScript脚本接口?
*   Browser =>   => Plugin
*   Browser <=   <= Plugin
*
* 
*
*
* 使用方式(HTML中)
*   是  <object id="myPluginID" type="application/x-yyy" > <param name="xxx" value="yyy"/></object>
*   还是 <embed id="myPluginID" type="application/x-yyy" color="FFFF0000"></embed>
*   然后在JS中即可通过 "myPluginID.xxx" 访问，内部过程：
*   1.NPP_GetValue 得到 NPClass;
*   2.hasMethod/hasProperty 确认是否有对应的函数/属性;
*   3.Invoke/GetProperty 进行触发;
*   4.插件内部的实现;
*  
*   动态创建：
*     var newNode = document.createElement("embed");
*     newNode.setAttribute("type","application/x-yyy");
*     newNode.setAttribute("id", xxx"); //width, height
*     
* 函数回调(myPluginID.somecallback = func): 通过 SetProperty(回调指针) 设置，插件内部通过 
*   NPN_DefaultInvoke(回调指针) 触发回调.回调函数是 NPObject* 类型的变量(m_playOver), 
*   调用:NPN_InvokeDefault(0, m_playOver, 0, 0, &result); 
*        NPN_ReleaseVariantValue(&result); 
*   析构时需要 :if(m_playOver){ NPN_ReleaseObject(m_playOver); }
*
* NPAPI Scriptable Plugin(JavaScript 可以将 Plugin 当作 JavaScript Object 来使用) -- 
*   需要添加plugin的函数与属性(static NPIdentifier 类型的?通过 NPN_GetStringIdentifier("名字") 初始化?)
*   
*   若要支持属性枚举(traceObj 或 DOM Inspector等), 需要支持以下方法或属性
*     __iterator__
*     QueryInterface
*   其他属性或方法(有没有基类能提供?)
*     nodeName
*     nodeValue
*     nodeType
*     parentNode
*     parentElement
*     childNodes
*     firstChild
*     lastChild
*     previousSibling
*     nextSibling
*     attributes
*     ownerDocument
*     insertBefore()
*     replaceChild()
*     removeChild()
*     appendChild()
*     hasChildNodes()
*     等
*******************************************************************************************************/

#endif //NPAPI_STUDY_H