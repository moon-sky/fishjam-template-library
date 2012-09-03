#pragma once

/*******************************************************************************************************
* 扩展的在线生成向导(可用吗？)：https://addons.mozilla.org/en-US/developers/tools/package/
* 经典扩展：
*   All-in-One Gestures -- 鼠标手势
*   GreaseMonkey -- 在Firefox 中提供了一个用于运行用户脚本(JavaScript)的环境，这里脚本可以指定目标网站。
*   
* 注意：
*   1.? -- dtd、properties 等资源文件必须存为 "without signature" 的UTF-8编码，否则可能无法显示
*   2. -- 为所写的代码授予权限以克服安全机制的限制
*   3. -- 不通过xpcom技术，使用 js-ctypes 在addon中直接调用dll中的函数(http://mozilla.com.cn/post/21692/)
*         可以摆脱xpcom技术带来的版本兼容性束缚，但不支持 Chrome(最好还是用 npapi ?)
*   
* 扩展的基本目录结构(扩展名xpi的zip压缩包) -- https://developer.mozilla.org/en/Building_an_Extension
*   chrome.manifest -- Chrome 注册的清单文件,设置各种XUL文件路径以及其他的一些资源文件路径，语法如下：
*     content 扩展别名 位置 <== 定义扩展别名，设置对应的Chrome包和物理地址(jar包或目录等，注意，最后要有斜线表示目录)的对应关系
*     overlay chrome://browser/content/browser.xul	chrome://helloworld/content/overlay.xul	<== 在FF提供的browser.xul上合并自定义的UI，通常是扩展的主UI
*       说明：1.FF通过"Chrome URIs"来存取相关的文件，如"chrome://browser/content/browser.xul", 可直接在地址栏输入确认,
*               FF会将其转换为磁盘或JAR包中实际的文件路径。
*             2.在扩展中可以使用覆盖（Overlay）来向已有的界面中添加元素(如系统 Tools菜单 <menupopup id="menu_ToolsPopup"> ),
*               其他可以覆盖的系统xul有 chrome://browser/content/preferences/preferences.xul 等
*             3.FF系统使用的 browser.xul 等文件在安装目录下的 omni.ja 压缩包内
*     locale 扩展别名 语言ID 位置 <== 设置不同语言ID(如zh-CN)对应的资源目录，问题：怎么切换语言?
*       如 locale helloworld zh-CN chrome/locale/zh-CN/
*     skin 扩展名 classic/1.0 位置 <== 
*     style chrome://global/content/customizeToolbar.xul xxx.css  <== 建立界面(xul)和界面风格(css)之间的关联 ?
*
*     XPCOM注册
*       C++类型的XPCOM注册：
*         interfaces 类型库(.xpt)的位置 <== 指定XPCOM所实现的接口类型库
*         binary-component XPCOM的路径 ABI=WINNT_x86-msvc appversion<15 <== 
*       JavaScript类型的XPCOM注册
*         component 组件ID xxx.js <CR> contract 契约ID 组件ID <== 建立 js 和 XPCOM 之间的连接(通过组件ID?)
*       Python类型的XPCOM注册(PyXPCOM)
*         怎么做???
* 
*   install.rdf -- 包含了扩展的注册信息和附加信息等，主要的有以下内容：
*     <em:id> -- 以 "{UUID}" 或 "name@地址" 的方式表示 
*     <em:name> -- 
*     <em:version> -- 
*     <em:targetApplication> -- 指定目标系统的信息，如
*        <em:id>：{ec8030f7-c20a-464f-9b0e-13a3a9e97384} -- Firefox App ID
*        <em:minVersion>,<em:maxVersion>
*    <em:updateKey> -- 自动升级时的"Public Key",
*
*   update.rdf -- 描述当前扩展的新版本信息(如 设计URL?),可以支持自动升级
*     0.下载并安装 HashTab 程序,可以在文件属性中查看其 CRC32/MD5/SHA-1 等哈希值;
*       下载并安装 McCoy 程序(https://developer.mozilla.org/en-US/docs/McCoy) -- 签名的GUI程序，对应 Uhura 等命令行的功能？
*     <em:updateURL> -- update.rdf 文件在Web服务器上的地址 ? 那客户端放什么?
*     <em:updateKey> -- 
*
*   [chrome] -- Chrome指的是应用程序窗口的内容区域之外的用户界面元素的集合，包括工具条，菜单，进度条和窗口的标题栏等。
*       contents.rdf -- (现在似乎不需要这个文件了？每个子目录下都有一个？现在统一成 chrome.manifest ？)描述相关目录的信息清单，Mozilla将读取并使用其内容来注册包，
*       并给这个包分配一个chrome URL，如果没有改文件，就不能通过 chrome:// 来访问？实测可以访问
*     三种基本的chrome提供者
*     [content] -- 扩展的描述界面的 XUL 文件和增加行为的 JavaScript 文件。
*       如：content	myExtension	chrome/content/ 或 content	myExtension	jar:chrome/myExtension.jar!/content/
*       主窗体文件名必须与包名相同(如editor包必须有一个叫做editor.xul的文件)？老的版本可能有这个限制，现在似乎已经不需要了？
*     [locale] -- 存放的是本地化相关的文件, 如  en-US、zh-CN、ko-KR、vi-VN 等目录，其下有 dtd、properties 等文件
*        注意：1.必须保存为"without signature"的UTF-8格式；2. 更改properties后必须重启FF才能生效(有没有配置项 ?)
*     [skin] -- 存放 CSS、PNG等资源文件，用来定义扩展的外观，可以包含多个以支持不同的主题?
*   [plugins] -- 
*   [components] -- 存放自定义的XPCOM组件(xpt和dll)，可以使用 C++、JavaScript、Python等实现。
*     cmdline.js
*   [defaults] -- 存放一些默认的设置数据
*   
* 安装方式：
*   1.xpi
*   2.调试时的扩展代理文件
*   3.注册表(类似扩展代理文件)：HKCR 或 HKLM 的 Software\Mozilla\Firefox\Extensions 
*
* 配置项(在 about:config 中增加自己的配置项，插件必须是目录的形式)
*   使用 pref("名字", 值) 的方式放置在自己目录下的 defaults\preferences\xxx.js 文件中
* 
*
* 打包签名发布，官方文档(不过比较旧) ：https://developer.mozilla.org/en/Signing_a_XPI
*   1.mccoy ???(携程的工具?) -> 打包成压缩文件，扩展名改为 xpi;
*   2.sha1sum.exe xxx.xpi 得到校验码，copy到 update.rdf;
*   3.sign 和 verify (mccopy 上的按钮，实际上怎么做?)
*   
*   签名结果说明( META-INF 目录下)
*     manifest.mf -- 包含XPI中每一个文件的 MD5/SHA1 验证码
*     zigbert.sf --  包含XPI中每一个文件的 MD5/SHA1 + XPI本身 验证码(值和 manifest.mf 中的不一样)
*     zigbert.rsa -- 签名信息(如公司、网址等），是二进制文件，用什么查看?
*
*   NSS(Network Security Services) -- 下载：https://ftp.mozilla.org/pub/mozilla.org/security/nss/releases/NSS_3_12_4_RTM/msvc9/WINNT5.1_OPT.OBJ/nss-3.12.4.zip)
*   NSPR(Netscape Portable Runtime) -- 使用 NSS 3.12.4 即可不用这个工具？
*     命令工具介绍( -H 显示详细帮助，注意：MS自己有 certutil.exe等工具，因此需要设置 %PATH% 环境变量 )
*       certutil.exe -- 证书操作工具，可以 新建证书数据库(-N), 导入证书(-A), 列出证书(-L)
*         -L 查看证书时的 "Trust Attributes -- SSL,S/MIME,JAR/XPI"
*            u, u, Cu -- ?
*       pk12util.exe -- 密钥操作工具？
*         -o "证书文件名" -n "证书昵称" -d 目录
*       pvk2pfx.exe -- .pvk + .spc(双证书文件) => .pfx() ， 如 mycredentials.spc + myprivatekey.pvk => cert.pfx
*       signtool.exe(nss版本) -- 签名工具， 可以使用指定的证书来签名特定的文件或目录
*         -l -d . <== 查看当前目录下已经导入的证书信息
*
*     步骤：
*       说明：虽然 PFX 格式证书中本身已经含有证书颁发者的根证书和中级根证书，但由于NSS签名工具 Signtool 不支持证书链(Bug),
*             所以必须也导入根证书中级证书(即第 2 步)，并保存在工作目录下(一般可以命名为 rootCA.cer? )
*       1.在当前目录建立nss证书数据库，需要输入"证书存储库管理密码(NSS Certificate DB)"，然后会生成 cert8.db/key3.db/secmod.db 三个文件
*         certutil -N -d .
*       1.5.生成测试用的自签名 pfx 证书(一般商业开发中应该用已经生成的)
*         最后会生成什么文件？ -- mycredentials.spc, myprivatekey.pvk, cert.cert, cert.pfx, cert2.cert,cert.pem, cert.p12
*         signtool -G myTestCert -d . -p"证书存储库管理密码"  <== 会生成 "x509.raw" and "x509.cacert"，
*           需要提供"certificate common name", "organization","organization unit","state or province","country","username","email address" 等证书信息.
*           生成后可通过 certutil -d . -L 查看, 类型为 "u,u,Cu"
*       1.6 将测试用的证书加入信任
*           x509.cacert 改名为 x509.cert => FF14中：工具->选项->高级->加密->查看证书->证书机构-->导入->编辑信任->编辑CA信任(软件)
*       2.将根证书( .cer/.pfx -- .pfx最好? )和XPI文件签名时使用的证书(不带密钥)导入到NSS数据库中
*         certutil -A -n "Intermediate CA Certificate" -t "TC,TC,TC" -d . -i "cert.cert"
*         certutil -A -n "Code Signing Certificate" -t "c,c,C" -d . -i "cert2.cert"
*           -A 加入证书
*             -n "证书的昵称"
*             -t "信任参数"
*             -i "证书文件"
*           -L 列出证书，可以查看导入结果
*       3.导入代码签名证书 [TODO:还是密钥?]( .pfx 或 pkcs12 等文件)，需要输入"证书存储库管理密码" + "导出PFX证书时设置的密码"
*         pk12util -i "XPICA.pfx" -d .
*       4.检查根证书和代码签名证书是否成功导入证书存储库中(即签名使用的证书链是否完整)
*         certutil -L -d .  <== 显示  "u,u,u"
*       5.使用证书来签名指定目录下的文件（会在指定目录下生成 META-INF 和相应的 zigbert.sf 等文件)
*         signtool -d . -k "签名证书的昵称" -p "建立数据库时的密码"  directory   //-X -Z "file.xpi"
*       6.压缩程序打包为 zip 包，并更改扩展名为 xpi，压缩包中第一个文件必须是含有签名信息的zigbert.rsa，因此需要分两步
*         zip  ..\xxxxx.xpi META-INF\zigbert.rsa				<== 先把 zigbert.rsa 压缩进xpi包
*         zip -r -D ..\xxxxx.xpi * -x META-INF\zigbert.rsa		<== 然后把除 zigbert.rsa 外的其他文件压缩进xpi包
*       7.将根证书导入到 Firefox 的证书系统中后，将 xpi 拖到 Firefox安装时，会由"未验证"更改为"已验证"
*       
* 证书：
*   类型:  
********************************************************************************************************/


