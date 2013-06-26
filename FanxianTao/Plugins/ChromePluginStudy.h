#pragma once

//如意淘的地址：C:\Documents and Settings\fujie\Local Settings\Application Data\Google\Chrome\User Data\Default\Extensions\keigpnkjljkelclbjbekcfnaomfodamj\3.5.5_0
//TODO: JSON 文档的格式 -- 所有的键值对都要用引号包起来? 每一对大括号里的最后一项后面都没有逗号，其余的每项之间必须用逗号间隔?
// 跨域访问 -- ?
// 图片：图标边缘应该使用alpha透明，这样的话可以融合到各种不同的浏览器主题里

/***************************************************************************************************************
* Chrome -- 基于Webkit的新一代浏览器
* 隐身模式 -- 保证在该窗口下浏览不会留下痕迹，缺省情况下，扩展是不会运行在该模式下的
*
***************************************************************************************************************/

/***************************************************************************************************************
* 
* 官方扩展文档：http://code.google.com/chrome/extensions/dev/devguide.html 
* 360翻译的文档：http://open.chrome.360.cn/html/dev_doc.html
*                http://open.chrome.360.cn/extension_dev/overview.html
* 开发IDE： Notepad++
* 
* Chrome扩展方式有多种类型
*   browser_action -- 浏览器行为扩展, 插件会在浏览器的右上角显示一个icon，这个icon可以响应用户点击操作(default_popup).
*   page_action -- 页面行为扩展，该插件会在匹配页面的浏览器地址栏最右边显示一个icon，表明只对特定的页面起作用
*   theme -- 主题插件，用于提供改变浏览器主题背景的一类插件，不包含HTML或JS代码，主要是各种图片、颜色等，可通过 crxtheme 工具制作
*   app -- 即WebApp，在chrome应用程序页面显示一个图标，点击后导航到指定的url，最简单的一个插件
*
* 扩展(Extension)、 WebApp、皮肤 等
*   
***************************************************************************************************************


/***************************************************************************************************************
* 
* browser action
* page action -- 当扩展的图标是否显示出来是取决于单个的页面时
* Content Script -- 内容脚本，插件和网页交互，通常由JS编写，会在网页载入完成后调用，可看作网页的一部分(而非扩展的一部分)，
*   有一些限制(如不能直接访问扩展中定义的变量)，可通过message机制解决。
* 
* 调试
*   打开插件页(chrome://settings/extensions) -> 勾选"开发者模式" -> 载入插件并定位到文件夹
*
* 打包
*   打包工具打包
*
* 安全限制
*   Content Security Policy(CSP)--
*
* 发布到Google的扩展中心(压缩成 zip 格式提交审核 )
*   https://chrome.google.com/webstore/developer/update
***************************************************************************************************************/


/***************************************************************************************************************
manifest.json -- 配置和控制整个插件的资源, JSON格式，必须以UTF-8格式保存。 http://open.chrome.360.cn/extension_dev/manifest.html
  "manifest_version": 2,     //现在开发的都是 2.0的，1.0 的已经过时了

  "app": {			//APP 的配置信息
	 "launch": {
	   "web_url": "http://www.xxxx"
	 },
	 "urls" : [ "启动的网址, 如 http://xxxx" ]
  },
  "default_locale" : "zh_CN",	//缺省的本地化目录，如 en 或 zh_CN，在 _locales 目录下需要有对应的子目录"
  "description" : "描述信息，不能超过132个字符，必须是纯文本。TODO: 显示在哪里",
  "icons" : {  "16":"icon16.png", "128":"icon128.png" },  //大小+图标文件的方式
  "name" : "FanxianTao",						//扩展的名字",
  "version" : "1.0",							//版本信息
  "permissions": "tabs, [http://xxx/xxx/]",		//权限信息，有权限能访问的地址(加入后才能跨域访问)，如果是星号(*)，表明可以访问任何网站

  //应用类型，多选一，或者不选
  "browser_action" : { "default_icon": "icon.png", "default_popup": "popup.html"}, 
     //注意：很多文档都是 popup, 实测要 default_popup 才行。但使用default_popup后，background脚本中注册的onclick事件将失效
  "page_action" : { },
  "theme" : { "images":{xxx}, "colors":{xxx}, "tints":{xxx}, "properties":{xxx} },
  "app" : {},

  //后台逻辑及事件处理程序，会在一个独立的后台环境运行，而且每次浏览器启动或者插件重新加载时都会自动执行，
  //最主要的作用是指定运行background时的脚本文件, 注册监听浏览器以及插件本身的事件操作，从而达到控制整个插件事务逻辑的作用；
  //同时也只有在这里指定的脚本中才可以使用chrome插件提供的API接口，也可用于页面间的通信
  //只在启动浏览器加载插件时载入一次，它不直接显示出来而是在后台运行。收集或处理的结果可以保存到全局变量localStorage中传递给popup页面
    "background": {
      "page": "assets/views/background.html"
      //"scripts": [ "jquery-1.8.0.min.js", "background.js" ],
      //"matches": [ "http://x/x", "https://x/x" ], 
      //"css": [ "plug_style.css" ]
    },
  "plugins": [ 二进制的插件 ],
  "update_url" : "http://path/to/updateInfo.xml"
  "homepage_url" : "主页",

  "content_scripts" : [ { all_frames, matches,run_at }, {} ]  -- 多个内容脚本信息的数组

  //使用自定义页面覆盖Chrome默认页面
  "chrome_url_overrides" : {   
	"newtab" : "newtab.html"		//新标签页
  },

popup.html -- 

***************************************************************************************************************/

/***************************************************************************************************************
扩展API(chrome.), 通常需要在 mainfest.json 中设置对应的权限(permissions)
  bookmarks -- BookmarkTreeNode(节点，书签或文件夹)
  extension
    .onMessage -- 事件, addListener(function (request, sender, sendResponse){ });
    .getURL('manifest.json') -- 返回指定的URL
    .sendMessage({json param}, function(response){ }); -- 发送消息，然后可在 onMessage 的处理事件中处理

  tab
    .url -- 
  tabs
    .create({url: "www.baidu.com"}) -- 打开URL页面
	.executeScript(null, {code:"document.body.style.backgroundColor='red'"});
    .getSelected(null, function(tab){ tab.url; }); -- 获取当前激活的页面?

BrowserPage 相关的函数(怎么调用?)
  setIcon,setTitle,setBadgeText(图标标记，如未读邮件数),setBadgeBackgroundColor,setPopup

不同页面间的交互
  chrome.extension methods such as getViews() and getBackgroundPage()
***************************************************************************************************************/