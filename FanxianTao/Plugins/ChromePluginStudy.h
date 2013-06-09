#pragma once

//如意淘的地址：C:\Documents and Settings\fujie\Local Settings\Application Data\Google\Chrome\User Data\Default\Extensions\keigpnkjljkelclbjbekcfnaomfodamj\3.5.5_0

/*****************************************************************************************************************************
* Chrome -- 基于Webkit的新一代浏览器
*   
* 官方扩展文档：http://code.google.com/chrome/extensions/dev/devguide.html 
* 360翻译的文档：http://open.chrome.360.cn/html/dev_doc.html   
* 开发IDE： Notepad++
* 
* browser action
* page action -- 当扩展的图标是否显示出来是取决于单个的页面时
* content script
* 
* 必要的文件
*   manifest.json -- UTF-8格式保存,所有的键值对都要用引号包起来? 每一对大括号里的最后一项后面都没有逗号，其余的每项之间必须用逗号间隔?
*     "browser_action" : {"default_icon": "icon.png", "popup": "popup.html"}
*     "permissions": [http://x/x] -- 注意：实际是星号(*)
*     "
*   popup.html -- 
*
* 调试
*   打开插件页(chrome://settings/extensions) -> 勾选"开发者模式" -> 载入插件并定位到文件夹
* 
* 发布到Google的扩展中心(压缩成 zip 格式提交审核 )
*   https://chrome.google.com/webstore/developer/update
*****************************************************************************************************************************/
