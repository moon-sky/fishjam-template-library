#pragma once

//代码片段：https://developer.mozilla.org/en-US/docs/Code_snippets
//Helloworld: http://kb.mozillazine.org/Getting_started_with_extension_development (搜索 "download")
//firefox extension tutorial
//旧版本的FF下载：ftp://ftp.mozilla.org/pub/firefox/releases/, 安装好后需要 工具->选项->更新->不检查更新, 否则查看About时会自动更新

//扩展是针对浏览器的，扩展随着浏览器启动，//插件是针对页面的，在页面遇到特殊数据格式时才启动
/************************************************************************
* 插件开发环境配置(https://developer.mozilla.org/en-US/docs/Setting_up_extension_development_environment)
* a.Firefox 多个版本共存 -- 配置文件分离(注意：较好的方式是通过 -no-remote -p 启动，然后通过对话框选择配置文件)
*   1.备份配置文件：%USERPROFILE%\Application Data\Mozilla，%USERPROFILE%\Local Settings\Application Data\Mozilla
*   2.安装新的FF版本到新的目录（不能采用升级安装的方式，否则会覆盖以前的版本?)
*   3.修改快捷方式：右键->属性->“目标”改为 "{安装 Firefox 的目录}\firefox.exe" -P {版本号} -no-remote
*     -no-remote <== 可以同时运行多个实例
*     -purgecaches <== 禁止JavaScript的缓存(fastload)
*   4.运行这个修改过的快捷方程，选择“创建配置文件”并命名为 {版本号}
*     Firefox 把用户的个人信息，包括设置、已安装的扩展等，都保存在一个配置文件中，默认是使用名为 default 的配置文件。
*   5.建议：一般会用最新版本的浏览器来读网页, 而老版本只用作测试 
*     所以备份配置后安装新版本可以覆盖原版本的配置 (因为我们要继承以前的资料, 以方便使用)，
*              然后再去安装一个老版本的 Firefox 并进行上述的多版本兼容操作
* 开发工具
*   aptana
*   spket <== http://spket.com/(开发 JS 和 XML，需要破解 com.spket.ui.internal.License )
*   XULBooster <== 
*   XPComViewer <== 可以检测注册到FF中的XPCom
*
************************************************************************/

/************************************************************************
* 两种扩展方式
*   1.插件（Plugins) -- 让Firefox能够显示特殊格式的文件而由相应的第三方应用程序提供的二进制文件
*     (如显示Flash的Adobe shockwave flash等) -- 使用自己的View来展示
*   2.扩展（Extensions）-- 修改或者增强Firefox本身的功能而提供的一种打包格式,通常由包含功能代码的js脚本、包含界面的xul
*     文件以及包含皮肤的css文件和各种图像文件组成。扩展名xpi的zip压缩包。
*   2.1.传统的基于JavaScript和XUL的方式，传统的，使用XUL的扩展，功能强大，但比较复杂，而且要求重启安装
*       CSS(Cascading Style Sheets) -- 层叠样式表，解决内容与表现分离的问题，让两者可以独立变化，
*         在FF中还可以把XBL和XUL关连起来，以完成对XUL的扩展。
*       DTD(Document Type Definition) -- 文档类型定义，借用DTD中的实体 (Entity)展开机制，完成字符串的本地化
*         引用方式：<!DOCTYPE window [<!ENTITY % brandDTD SYSTEM "chrome://branding/locale/brand.dtd" > %brandDTD; ]>
*         使用时： label="&backCmd.label;"
*         注意：DTD文件需要保存为UTF8格式(VS显示为 Unicode(UTF-8 without signature) - Codepage 65001)，否则可能出现乱码
*       JavaScript -- FF内核调用编译性程序的粘和剂，完成业务逻辑，通过它来调用编译性的程序（比如c++的动态库，API）
*         引用方式：<script type="application/x-javascript" src="chrome://global/content/nsDragAndDrop.js"/>
*       XBL(Extensible Binding Language) -- 扩展绑定语言，在XUL中可以只描述具有共性的部分，而由XBL对它进行扩展。
*          XBL 有组合和继承机制，大大提高了可重用性。
*       XUL(XML User Interface Language) -- 应用XML来描述使用者界面的标示语言，完成UI定制，既可以在远程执行，也可以安装到本地执行。
*       properties文件 -- 在 nsIStringBundle 接口的帮助下，javascript可以方便的从property文件中取到所要的字符串，处理JavaScript里字符串的本地化。
*          文件内每一行的内容是 "key=Value" 的格式
*          引用方式：<stringbundleset id="stringbundleset">
*                      <stringbundle id="minimo_properties" src="chrome://minimo/locale/minimo.properties"/>
*                    </stringbundleset>
*                    或通过 nsIStringBundleService/nsIStringBundle 使用
*       RDF(Resource Description Framework) -- 资源描述框架
*       SVG(Scalable Vector Graphics) -- 表示矢量图?
*
*   2.2. FF4 以后 Add-on Builder 和 Add-on SDK 的方式, 就是XPCOM，目前主推的方式，FF->JS->XpCom->C++ Dll
*        XPCOM(Cross Platform Component Object Module) -- 跨平台组件对象模型，仿MS的COM实现
*          Mozilla构建的跨平台组件结构，原理类似于微软的Com组件。编译性的，提供主要的逻辑处理。
*          引用方式：通常在JS中通过DOM接口取得 -- gBrowser = document.getElementById("content"));
*            使用：gBrowser.webNavigation.goForward();
*        注意：因为每一个FF版本使用不同版本的SDK，通常产品的策略是只支持最近的N个版本，避免维护上的问题
*        
*    此处可能有问题？
*        特点：restartless(bootstrapped extensions )，安装时不需要重启，但功能有限，使用 Add-on SDK 和 Add-on Builder 帮助开发
*        https://addons.mozilla.org/en-US/developers/tools/builder
*
*
* FF中本地化有两部分：
*   XUL 中的本地化通过DTD实现；
*   JavaScript 中的本地化通过 properties 文件实现
*
*
************************************************************************/

/************************************************************************
* theme -- 主题?

************************************************************************/

