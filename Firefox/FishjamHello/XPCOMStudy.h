#pragma once

/*******************************************************************************************
* https://developer.mozilla.org/en-US/docs/How_to_build_a_binary_XPCOM_component_using_Visual_Studio
*
* XPCOM 可以通过 C/C++/JavaScript 创建，也可以在 C/C++/JavaScript 中使用
*   https://developer.mozilla.org/en-US/docs/How_to_Build_an_XPCOM_Component_in_Javascript
*   https://developer.mozilla.org/en-US/docs/How_to_build_a_binary_XPCOM_component_using_Visual_Studio
* 
* TODO:
*   1.XPCOM 的 组件管理器 ?
*   2.nsIServiceManager -- 
*   3.XPConnect -- Javascript 在XPConnect的支持下，Javascript也可以用来开发COM组件，可以实现任何功能
*     扩展通过XPCOM的XPConnect API与浏览器实现交互
*
* XPCOM(Cross Platform Component Object Module) -- 跨平台组件对象模型，仿MS的COM实现
*   Mozilla构建的跨平台组件结构，原理类似于微软的Com组件。编译性的，提供主要的逻辑处理。
*   XPCOM 并不直接支持 C++ 的异常处理. 在 XPCOM 中, 所有的异常必须在组件内部处理, 而不能跨越接口的边界，返回 nsresult(类似 HRESULT)
*   ？目前Mozilla 已不想支持XPCom
* 
* MXR(Mozilla Cross-Reference) -- 
*******************************************************************************************/

/*******************************************************************************************
* 安全性
*   1.基于安全性的原则，很多需要特权的XPCOM物件在无特权的XUL文件上都不能使用。唯有使用数码签署的脚本才享有此种特权 -- 签名怎么做？
*   2.不能加载远端的XUL、DTD和RDF文件，一般是通过 chrome:// 的方式使用安装包或系统中的
*   权限：如自动完成(autocomplete) 需要 XPConnect 权限 
*   为了使用 XPConnect，文件需要特殊的UniversalXPConnect权限(普通的网页以及本地文件没有该权限)
*     文件中设置权限(插件不需要)：netscape.security.PrivilegeManager.enablePrivilege('UniversalXPConnect');
*
* 接口(接口命名通常为 nsIxxx 的形式)
*   nsISupports -- 基础接口，类似 IUnknown, 需要 #include "nsISupports.idl"
*   
* 语法
*   契约ID(contractID,类似ProgID?) -- 用于访问组件的可读(to humen)的唯一标识字符串，
*     格式为"@domain_name/module_name/component_name;version_number"。
*     refp = do_CreateInstance( "@mozilla.org/accessibleRetrieval;1", &rv);
*   智能指针：nsCOMPtr、nsAutoPtr、nsAutoArrayPtr、nsRefPtr
*   服务(单一实例)：nsCOMPtr<nsIPrefService> prefService = do_GetService (NS_PREFSERVICE_CONTRACTID);
*     系统提供的服务
*     a.跨平台文件抽象：向 XPCOM 开发人员提供同一而强大的文件访问功能.
*     b.目录服务, 提供应用和特定系统定位信息.
*     c.内存管理, 保证所有对象使用同样的内存分配器. 
*     d.事件通知机制, 允许传递简单消息.
*
* 调用方式（可以通过 XPCOM Viewer 扩展的 "Generate Code"功能生成调用服务的代码)
*   普通组件(通过 createInstance()创建实例) <== 
*   系统服务(通过 getService() 获得唯一实例) <== var myObj = Components.classes["契约ID"].getService(接口类型);
*     说明：在对应的 idl 文件中，契约ID通常为 NS_XXXXX_CONTRACTID
*   自定义组件
*      1.var myObj = Components.classes["@fishjam.com/helloworld-http-request;1"].createInstance(Components.interfaces.nsIHelloHttpRequest);
*      2.var myObj = Components.classes["@fishjam.com/helloworld-http-request;1"].createInstance(); myObj = myObj.QueryInterface(Components.interfaces.nsIHelloHttpRequest);
* 组件的编写
*   1.定义接口文件(idl?)
*   2.编译接口文件为 XPT 格式
*   3.实现接口
*   4.完成组件的工厂方法及注册模块 -- 实现组件的注册及创建相关的功能
*   5.VS中需要定义 XP_WIN、XP_WIN32 宏；链接 nspr4.lib xpcom.lib mozalloc.lib xpcomglue_s_nomozalloc.lib 等库
*       xpcomglue_s.lib ?
*   6.在 chrome.manifest 中注册组件(interfaces、binary-component)
*******************************************************************************************/

/*******************************************************************************************
* IDL(编译 idl 的工具是 xpidl.exe，生成的头文件中定义了 NS_IXXX_IID_STR 和 NS_IXXX_IID 宏，生成的接口一般为)
*  -I 目录    <== 包含系统或第三方idl的目录，如 -I %XPSDK_DIR%idl
*  -m typelib <== 生成以 .xpt 为后缀的类型库，类型库可以通过XPConnect向类似JavaScript的语言提供组件访问的Wrapper
*  -m header  <== 生成头文件
*  -w         <== 打开警告
*  -v         <== verbose 模式
*  -e 输出文件名 <== 指定输出的文件名(如 XXX.xpt)
*
* Regxpcom <== 组件注册工具，
* xpt.py dump xxx.xpt <== 查看类型库的详细信息
*
* Python(?) -- 
*  从 idl 生成头文件 -- header.py -I  yourGecokSDKfolder \idl -o yourheader.h youridl.idl".
*  从 idl 生成 typelib(.xpt) -- typelib.py -I  yourGecokSDKfolder \idl -o yourtypelib.xpt youridl.idl
*******************************************************************************************/

/*******************************************************************************************
* Gecko SDK(XulRunner SDK) -- https://developer.mozilla.org/en/Gecko_SDK
*     https://developer.mozilla.org/en/XPCOM/XPCOM_changes_in_Gecko_2.0
*     https://developer.mozilla.org/en/How_to_build_a_binary_XPCOM_component_using_Visual_Studio
*   一个开源的, 遵循标准的, 嵌入式 web 浏览器和 工具包
*   解压需要使用如下命令: 7z.exe x  XXX.win32.sdk.zip -oXXX.win32.sdk -y
*
*
*******************************************************************************************/


/*******************************************************************************************
* 然后在实现文件中 -- 工具会自动生成 工厂和模块定义 ?
*******************************************************************************************/
