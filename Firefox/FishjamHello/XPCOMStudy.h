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
*     设置权限(插件不需要)：netscape.security.PrivilegeManager.enablePrivilege('UniversalXPConnect');
*
* 接口(接口命名通常为 nsIxxx 的形式)
*   nsIDOMWindowInternal -- 基本的窗体? nsIWindowMediator.getEnumerator 返回的都是这种?(进行QI)
*   nsIFile -- 最基本的File接口
*     initWithPath("文件路径URL") <== 使用文件路径初始化对象，之后才能使用
*   nsIFileInputStream -- 输入文件流
*     init <== 关联nsIFile对象，参数详见 https://developer.mozilla.org/en-US/docs/PR_Open#Parameters
*   nsILocalFile -- 处理本地文件，XPCOM中既可以使用远程资源也可以使用本地文件，基本都需要使用URI作为路径(本地文件需要转换为 file:///C/temp/temp.txt 的格式)
*   nsISupports -- 基础接口，类似 IUnknown, 需要 #include "nsISupports.idl"
*   nsISimpleEnumerator -- 枚举器, 用法： while(enum.hasMoreElements()) { var obj = getNext().QueryInterface(Ci.nsIxxxx); obj.someMethod(); }
*   nsIPrefBranch -- 读写FF的首选项(about:config)
*   nsIPrefService -- FF的首选项， getBoolPref() 和 getIntPref 等方法
*   nsIProperties -- 根据名字获取XPCOM对象，比如 propDir = get("ProfD", Components.interfaces.nsILocalFile); 获取 Profiles 的目录
*     有没有能枚举所有 "名字-对象" 对的方法?
*   nsIWebNavigation -- 如下对象的类型。 document.getElementById('browser').webNavigation;
*   nsIWindowMediator -- 可以访问FF所有窗口(克服JS只能获取与父窗口相关的窗口的限制)
*     getMostRecentWindow('navigator:browser');  -- 获取活动浏览器窗口的枚举器
*     getEnumerator('navigator:browser') -- 获取所有具有特定类型的窗口(浏览器，如参数为null则获取包括对话框在内的所有窗口)，然后可以枚举
* 
* 字符编码转换(内部文本编码都是Unicode)
*   0.确认数据的编码方式，如果无法获取，可以采用缺省的 "intl.charset.default"
*     a.nsIChannel -- 网络中可以获取 contentCharset 属性;
*     b.文件 -- 通常使用系统缺省的编码或"preferences"中的缺省编码
*   1.使用 nsIConverterInputStream 从指定编码的文本文件中读入
*     var charset = 'Shift_JIS';
*     var converterStream = Cc['@mozilla.org/intl/converter-input-stream;1'].createInstance(Ci.nsIConverterInputStream);
*     converterStream.init(fileInputStream, charset, fileInputStream.available(), converterStream.DEFAULT_REPLACEMENT_CHARACTER);
*     var out = {};
*     var numChars = converterStream.readString(fileInputStream.available(), out);
*     if(numChars != 0) //EOF
*     { alert(out.value); }
*   2.使用 nsIUnicharLineInputStream 直接读入Unicode编码的流 ? 
*   3.使用 nsIScriptableUnicodeConverter 在内存中的各种编码之间转换
*     var converter = Cc["@mozilla.org/intl/scriptableunicodeconverter"].createInstance(Ci.nsIScriptableUnicodeConverter);
*     converter.charset = "UTF-8";  //读入流的编码格式
*     var unicode_str = converter.ConvertToUnicode("SomeUTF8String");    
*     var utf8_str = converter.ConvertFromUnicode("SomeUnicodeString");
*   4.使用 nsIUTF8ConverterService
*   5.使用 JS 函数 ( 用于 路径 )：
*     decodeURIComponent(escape("UTF-8String"))    <== 将 UTF-8  转换为 UTF-16
*     unescape(encodeURIComponent("UTF-16String")) <== 将 UTF-16 转换为 UTF-8
* 
* 文件IO（注意：使用完毕后需要调用 close 关闭）
*   分为两种模式，
*   1.二进制的流模式
*     输入：nsILocalFile + nsIFileInputStream(init) + nsIBinaryInputStream(setInputStream+readByteArray)
*     输出：nsILocalFile(create) + nsIFileOutputStream(init) + nsIBinaryOutputStream(setOutputStream+writeByteArray)
*   2.文本文件的字符模式(内部使用UTF-16编码，在读写时需要根据文件的编码对字符进行转换)
*     输入：nsILocalFile + nsIFileInputStream(init) + nsIConverterInputStream(init+readString)
*     输出：nsILocalFile(create) + nsIFileOutputStream(init) + nsIConverterOutputStream(init+writeString)
*           注意：nsIConverterOutputStream::writeString 的参数是Unicode编码的字符串(例子中是UNICODE转义，是否能直接用字符串?)
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
* 调用方式
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
