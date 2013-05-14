module("BrowserTester", {
    setup: function() {
        //dump("in BrowserTester::setup\n");
    },
    teardown: function() {
        //dump("in BrowserTester::teardown\n");
    }
});

test("TODO: Browser", function() {

    //一种判断浏览器的方式 -- 似乎不全面
    if (document.all) {
        //MSIE
        equal(navigator.appName, "Microsoft Internet Explorer", "MSIE");
    }
    else {
        equal(navigator.appName, "Netscape", "Navigator");
    }
    
    equal(1, 1, "TODO: 浏览器对象，包含了正在使用的 Navigator 的版本信息。反映了当前使用的浏览器的资料");
    //JavaScript 客户端运行时刻引擎自动创建 navigator对象


    equal(navigator.appCodeName, "Mozilla", "返回浏览器的码名--IE、FF、Chrome全都返回 Mozilla");
    equal(navigator.appName, "Netscape", "navigator.appName 返回浏览器名, FF/Chrome--Netscape, IE -- Microsoft Internet Explorer");

    equal(navigator.appVersion, // "5.0 (Windows)");       //浏览器版本
     "5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.31 (KHTML, like Gecko) Chrome/26.0.1410.64 Safari/537.31");  //Chrome -- 5.0 
    //FF --
    //IE -- 4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0;  <== 注意：还可能有其他的一下信息

    equal(navigator.language, "zh-CN", "语言")
    equal(navigator.platform, "Win32", "平台");
    equal(navigator.userAgent,
		"Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.31 (KHTML, like Gecko) Chrome/26.0.1410.64 Safari/537.31", 
		"返回浏览器对象的全部信息，可以查找是否包含指定字符串来判断浏览器类型？");
    //equal(navigator.plugins, "a");    //以数组表示已安装的外挂程序

    //ok(true, "tmp");
});

test("TODO: Screen", function() {
    with (screen) { //全局屏幕对象 反映了当前用户的屏幕设置
        //TODO:在双屏显示器时，使用 equal 有的时候会失败，F5刷新后又变好，为什么？
        ok(width >= 1280, "屏幕宽度");
        ok(height >= 1024, "屏幕高度"); //960
        ok(availWidth >= 1280, "屏幕的可用宽度");
        ok(availHeight >= 1024, "屏幕的可用高度（除去了一些不自动隐藏的类似任务栏的东西所占用的高度"); //960
        equal(colorDepth, 32, "当前颜色设置所用的位数");
    }
});

test("TODO: Window", function() {
    //JavaScript 中最大的对象，它描述的是一个浏览器窗口，一个框架页面也是一个窗口
    //在一个页面内定义的所有全局变量和全局函数都是作为 window 对象的成员 -- 但为什么FF中jstrace不好用？

    //equal(window.name, "test", "窗口的名称");

    equal(window.closed, false, "当前窗体尚未关闭");
    equal(window.opener, null, "如果窗口不是由其他窗口打开的，Netscape中返回null;IE中返回undefined"); //
    equal(window.self, window, "self 指窗口本身，它返回的对象跟 window 对象是一模一样的,最常用的是\"self.close()\"");

    equal(window.parent, parent, "parent 返回窗口所属的框架页对象，如一个frame中找获取frameset的其他frame -- parent.frames[n]");
    equal(window.top, top, "top 返回占据整个浏览器窗口的最顶端的框架页对象");
    equal(window.parent, window.top, "此处 parent 和 top 是同一个");

    ok(window.parent != null, "window.parent != null");
    ok(window.parent.top == window.self, "顶层框架的 parent/top 还表示该框架本身。if(self==top){是顶层框架}");

    //每个框架都对应一个HTML 页面
    equal(window.frames.length == 0, true, "TODO:frames -- 该页面内所有框架的集合，可通过frames[\"名字\"] 或 frames[index] 的方式引用");

    equal(window.history, history, "history -- 历史对象");
    equal(window.location, location, "location -- 地址对象，有 reload() 和 repalce() 方法");
    equal(window.document, document, "document -- 文档对象");
    //equal(window.Link, Link, "超链接或图形的位置点"); //在Chrome中实测没有这个属性
    //equal(window.elements, elements, "访问窗体中的所有元素"); //在Chrome中实测没有这个属性

    //open 打开窗口 <== open(<URL字符串>, <窗口名称window.name,可以使用 _top/_blank 等内建名称>, <参数字符串>);
    //     参数(多个参数用逗号隔开)：
    //      top,left,width,height <== 窗口的位置、大小信息
    //      menubar     <== 窗口有没有菜单，取值yes 或no
    //      toolbar     <== 窗口有没有工具条，取值yes 或 no
    //      location    <== 窗口有没有地址栏，取值yes 或no
    //      directories <== 窗口有没有连接区，取值yes 或no
    //      scrollbars  <== 窗口有没有滚动条，取值yes 或no
    //      status      <== 窗口有没有状态栏，取值yes 或no
    //      resizable   <== 窗口给不给调整大小，取值yes 或no
    //var winBaidu = window.open("http://www.baidu.com", "_blank", 
    //  "top=100,left=200,width=400,height=100,menubar=no,toolbar=no,location=no,directories=no,status=no, scrollbars=yes,resizable=yes");
    //if (winBaidu) {
    //    winBaidu.close();
    //}
    ok(window.location.href.indexOf("QUnitTestFramework.html") > 0, "location.href 是完整的URL");
    equal(window.location.protocol, "file:", "location.protocol");
    equal(window.location.hostname, "", "location.hostname");

    //window.scroll(x, y); //将窗口滚动到坐标指定的位置
    //window.moveTo(x, y); //将窗口移动到指定的位置
    //window.resizeTo(cx, cy);  //将窗口调整到指定的大小

    window.defaultStatus = "缺省状态栏消息";
    window.status = "指定用户将鼠标移到某个窗口等操作时，出现的瞬时状态消息"

    with (window.document) {
        //document.open([MIMEType]) -- 以指定的MimeType(缺省是text/html)打开文档，然后可通过 write/writeln 等方法写入，写完通过close关闭

        //如果在 form 中指定其 name="xxx", 则可以通过 window.document.xxx 的方式引用?

        equal(characterSet, "GBK", "字符集");
        equal(referrer, "", "referrer -- 如果当前文档是通过点击连接打开的,返回原来的URL");

        equal(anchors.length, 0, "文档中所有锚标记(包含name属性的<a>标记)的数组");
        equal(links.length > 0, true, "文档中所有连接标记(包含href属性的<a>标记和<map>标记段里的<area>标记的数组");
        equal(applets.length, 0, "文档中所有的Applet对象(Java 小程序)的数组");
        equal(embeds.length, 0, "文档中所有的插件<embed>标记的数组");
        equal(forms.length, 0, "文档中所有表单<form>的数组");
        equal(cookie.length, 0, "文档中的Cookie");
        
        equal(images.length, 0, "文档中的所有图像");
        /*
        //在 QUnitTestFramework.html 中加入 <img src="..\HTML\img\merglobe.gif"/> 后测试这些
        equal(images.length, 1, "文档中的所有图像");
        ok(images[0].src.indexOf("merglobe.gif") > 0, "img src");
        equal(images[0].height, "100", "img src");
        equal(images[0].width, "100", "img src");
        //*/
    }

    //form 对象 -- windows.document.forms[n]
    //其提供了 submit()/reset() 等方法
    //         action/elements(可细化为 button/radio/text 等)/encoding/target 等属性
    //         提交时(form.OnSubmit中): document.submitForm.result.value = submitValue; document.submitForm.submit();
});

test("Windows提示方式", function() {
    equal(1, 1, "TODO");
    //alert(提示信息) -- 提示信息，只有一个OK按钮
    
    //confirm -- 系统提供的确认对话框(分 确定/取消 两个按钮)，用户确定时返回 true
    //if (confirm("提示信息")) {
    //    alert("用户选择了\"确定\"");
    //}

    //prompt -- 显示要求用户输入的对话框，如用户取消返回 null，否则返回用户输入的内容
    //var s = prompt("提示信息", "缺省输入");
    
});

test("TODO: Firefox中的学习资料", function() {
    ok(1, "TODO:Info");
    //window.onload -- 发生在文档全部(HTML、图片、控件、小程序等)下载完毕的时候，onload/onunload 虽然是window事件，但是需要写在<body>中
    //History -- 历史对象包含了用户已浏览的 URL 的信息,鉴于安全性的需要，该对象收到很多限制，目前只有如下属性和方法：
    //   length, back(), forward(), go(pos);
    //location -- 地址对象, 描述的是某一个窗口对象所打开的完整URL,内部有 protocol、pathname、href 等多个属性
    //  属于不同协议或不同主机的两个地址之间不能互相引用对方的 location对象(安全性的需要)
    //  URL地址：协议//主机:端口/路径名称#哈希标识?搜索条件
    //    协议 -- 如 http, ftp, file, javascript(对JS表达式求值，如 JavaScript:void(0) ), about 等，
    //    哈希标识 描述了URL中的锚名称，包括哈希掩码(#)。此属性只应用于 HTTP 的 URL
    //    搜索条件 描述了URL中的任何查询信息，包括问号，是 变量=值 的对，多个对之间由 & 连接
    //Document -- 显示于窗口或框架内的一个文档。描述当前窗口或指定窗口对象的文档。它包含了文档从<head>到</body>的内容
    //  close() 关闭文档，停止写入数据。如果用了 write[ln]() 或 clear()方法，就一定要用 close() 方法来保证所做的更改能够显示出来。
    //Form -- 表单
    // action -- 返回/设定表单的提交地址,即<form action="...">属性
    // method -- 返回/设定表单的提交方法,即<form method="...">属性
    // target -- 返回/设定表单提交后返回的窗口,即<form target="...">属性
    // encoding -- 返回/设定表单提交内容的编码方式,即<form enctype="...">
    // onreset() -- 重置表单时发生
    // onsubmit() -- 提交表单时发生
});

//一个HTML 页面可以有一个或多个子框架，这些子框架以<iframe>来标记，用来显示一个独立的HTML 页面
test("TODO:iframe", function() {
    equal(1, 1, "iFrame");
});

test("图形映射Area", function() {
    //hash -- HREF属性的偏移量部分
    //host -- HREF属性的主机部分
    //href -- HREF属性全部
    //target -- TARGET属性
    equal(1, 1, "TODO:Area");
    //var myarea = new Area();
    //myarea.href = "javascript:void(0)"; //忽略用户单击

});

test("BrowserInfo", function() {
    equal(gBrowserInfo.IsNetscape, true, "是Netscape");
});

test("Event", function() {
    //JavaScript 的事件处理功能可改变浏览器响应用户操作的处理方式
    //  可以直接把代码写在 onxxx 后面；也可以写在 .js 文件后直接引用；也可以使用匿名函数 onxxx=function(){ callFun }
    //
    //事件处理函数的返回值
    //  false -- 中断系统默认处理(比如 <a onclick="return false;" xxx/> 就不会跳转
         
    //常见事件(TODO: 事件都没有参数 ? 比如 onerror 怎么知道具体的错误原因 )：
    //  onblur -- 失去输入焦点时
    //  onabort -- 用户中断(如图形装入)操作时
    //  onclick -- 点击对象时
    //  onerror -- 出现错误
    //  onfocus -- 获得输入焦点时
    //  onload -- 装入图形、文档或frames(帧组)时
    //  onselect -- 文本区或文本字段的文本被选中时
    //  onsubmit -- 提交窗体时 
    equal(1, 1, "TODO:事件");
});

