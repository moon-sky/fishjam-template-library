#pragma once

/*******************************************************************************************
* TODO:
*   1.Firefox 启动时访问的内容是 FireFox\omni.ja(zip格式的压缩包)，可以查看确认系统的一些实现方式?
*   2.可以在 DOM Inspector 中输入 chrome://xxx/xxx.xul 进行调试(可以看到xul中的对象树和运行效果)
*
* Firefox 启动时的命令参数( firefox.exe -h | more -- 必须通过重定向来生成，也可以 > help.txt)
*   -console    <== 打开调试Console，可以显示 dump 等的信息
*   -jsconsole  <== 启动时即打开"错误控制台",也可启动后从"工具->Web开发者"中打开
*   -no-remote  <== 不接收或发送远程命令(remote commands), 即可以启动多个实例，互不干扰
*   -private    <== 进入隐私浏览模(不记录本次会话的任何历史记录)
*   -ProfileManager <== 以配置管理器的方式启动，可以选择配置文件，Windows 下等价于 -P ?
*   -safe-mode  <== 
*
* 设置扩展代理文件(文本文件，其内容为指向扩展文件的目录 -- 修改代码后，重启Firefox即可生效，不用打包压缩成xpi)
*   1.定位到扩展安装的路径： %USERPROFILE%\Application Data\Mozilla\Firefox\Profiles\<profile_id>\extensions\
*     如 Win7: C:\Users\%USERNAME%\AppData\Roaming\Mozilla\Firefox\Profiles\XXXX\extensions\
*       WinXP: C:\Documents and Settings\%USERNAME%\Application Data\Mozilla\Firefox\Profiles\XXXX\extensions
*   2.创建一个文本文件，用扩展ID做为文件名来保存文件，没有文件扩展名。如果使用ID是GUID值，那么文件名也必须包含‘{}’。
*   3.文本文件中输入开发目录的全路径(如：c:\extensions\my_extension\)，路径必须以‘\’结尾并且之后不能有空格。
*   4.启动Firefox，Firefox将检测到文本链接扩展目录，并安装扩展。
*   补充：
*     a.有jar(如日本Toolbar，FFJpToolbar.jar)的话，可以把jar解压，并更改 chrome.manifest 中相关的路径(content,skin,locale等)
*     b.(未确认)安装 名为 Remote XUL Manager 的扩展，FF菜单中 Web Developer -> Remote XUL Manager -> 添加LocalFiles, 双击xul文件，使用FF打开，就可执行XUL的布局和功能了
*       https://addons.mozilla.org/en-US/firefox/addon/remote-xul-manager/
*
* FF配置(地址栏输入 about:config )
*   已有选项:
*     browser.dom.window.dump.enabled　	= true //允许使用 dump() 语句向标准控制台输出信息，脚本里可以通过 Interface("nsIConsoleService")
*     devtools.chrome.enabled			= true //(ToolsMenu中增加相关功能按钮?)可以通过"Tools menu"中的"Scratchpad"运行JavaScript代码片段
*     extensions.logging.enabled		= true //允许扩展日志
*     javascript.options.showInConsole	= true //把 JavaScript 的出错信息显示在错误控制台
*     javascript.options.strict　　　　	= true //在错误控制台中启用严格的 JavaScript 警告信息，过多的信息可以通过 Console2 插件过滤
*     network.http.use-cache			= false //对网页不使用缓存
*   为了通过断点调试plugin(NPAPI)，一般需要设置如下配置
*     dom.ipc.plugins.timeoutSecs		= 999	//xx超时?
*     dom.ipc.plugins.processLaunchTimeoutSecs	= 999;	//加载时超时
*     dom.ipc.plugins.parentTimeoutSecs	= 999	//xx超时?
*   右键->新建选项:
*     dom.report_all_js_exceptions		= true  //查看JavaScript异常的详细信息
*     extensions.checkCompatibility		= false //不检测兼容性(似乎不好?)
*     nglayout.debug.disable_xul_cache	= true  //禁用 XUL 缓存，使得对窗口和对话框的修改不需要重启(如 更改 dtd等)
*     nglayout.debug.disable_xul_fastload = true
*
* 帮助开发、调试的扩展 
*   DOM Inspector <== 查看和编辑网页文档和XUL应用程序的DOM，可以通过工具条中的"Find a node to inspect by clicking on it"来允许点击元素后定位。
*     或安装"Element Inspector"插件后 通过 Shift + 右键 直接定位。
*     问题：定位到一个元素后怎么确认是哪个扩展提供的？即怎么知道对应的 xul 等文件？
*   Venkman(更名为 JavaScript Debugger) <== JavaScript 的调试环境，可以设置断点、查看调用堆栈等
*     https://addons.mozilla.org/en-US/firefox/addon/javascript-debugger/
*     注意：需要取消 "Debug -> Exclude Application Files" 的选择，否则断点不会生效
*   XPCOMViewer <== 查看当前安装的XPCOM控件，查看到对应的控件后，可通过右键的"Generate Code"自动生成相关的代码。
*   Extension Developer 
*   Console2 
*   Chrome List
*   Firebug -- 一个极为复杂的用于监视和调试CSS,HTML 以及javascript 的网络开发环境
*   Greasemonkey -- 让使用者安装一些脚本使大部分HTML为主的网页于使用者端直接改变得更方便易用。
*   Scrapbook -- 一个网页剪辑管理器
*
* 调试方法和技巧
*   JavaScript
*     1.dump("string") 函数，可在终端中输出信息, 启动 FF 时需要增加参数 -console
*       console.log("xxxx") ?
*   XPCOM
*******************************************************************************************/
