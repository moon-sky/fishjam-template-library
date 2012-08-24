#pragma once

//AJAX(Asynchronous JavaScript and XML) -- Ajax是可以跨域的
//字符串可以通过 "UNICODE转义" 的方式进行硬编码， 如 var string='\u4e2d\u56fd'; 表示"中国"

/***********************************************************************************************************************
* 调试：
*   1.IE中有错误时弹出错误对话框:  Internet Options -> Advanced -> Browsing -> Disable script debugging
*   2.FF中的Javascript Debugger 中 Debug -> Error Trigger -> 可以选择出现JS错误时是否跟踪或停止调试(默认是忽略)
*
* HTML 的 <script> 标签用于把 JavaScript 插入 HTML 页面当中。
*   1.<head>或<body> 中定义和执行： <script type="application/javascript"> xxxx </script>
*   2.引用外部脚本: <script type="application/javascript" src="/js/example_externaljs.js"></script>
*
* 脚本位置（可以将定义放在 head, <body onload="myFun()" > 执行
*   <head> -- 定义或引入脚本：当被调用时，位于 head 部分的 JavaScript 才会被执行
*   <body> -- 页面载入时脚本就会被执行：当页面载入时，会执行位于 body 部分的 JavaScript
* 
* JavaScript 变量名称的规则:
*   1.变量对大小写敏感;
*   2.变量必须以字母或下划线开始
*
* 函数
*   function <funName>([param1] [, param2...]) {  xxxx;   return [retValue]; }
*   1.函数声明时只声明参数名字（不能确定类型）， 不声明返回值( 函数体内通过 return 直接返回 );
*   类：判断是否是某种类型的变量，用 if( obj instanceof SomeClass ) { ... }
*     function MyClassName() { 构造; }
*     MyClassName.prototype = {
*       var1Name : 类型,
*       Fun1Name : function() { xxxx; },
*       Fun2Name : function() { yyyy; }
*     };
*  
*
* 语法和关键字
*   typeof -- 求变量类型，var声明无初始值的变量后，其类型为 "undefined", 其他的有 "function", "object"等
*   消息框 -- 三种消息框, alert("警告框"); retBool=confirm("确认框"); retValue=prompt("提示框","默认值");
*   for(var in obj) -- for...in 语句用于遍历数组或者对象的属性
*   异常处理
*     方法1.try{ xxx } catch(err) { var errText = err.toLocaleString() + "\n" + err.description; throw err; } finally { xxxx; }
*       注意：a.捕获以后的异常，只要不再 throw，就不会再在 "错误控制台" 中显示。
*             b.系统发生的异常是一个 XXX 对象，有多种属性、方法获取详细信息，自己用 throw 抛出的异常，其类型是自己定义的。
*     方法2.在 script 中设置 onerror 事件处理程序: <script type="text/javascript"> onerror = myglobalerrhandle(); </script>
*           function myglobalerrhandle( msg, url, line) { .... } 
***********************************************************************************************************************/

/***********************************************************************************************************************
* 通过 js-ctypes 在addon中直接调用dll中的函数，参考URL： https://developer.mozilla.org/en/js-ctypes/Using_js-ctypes
* 
* JSON(JavaScript Object Notation)  -- 一种轻量级的数据交换格式。它基于JavaScript的一个子集，采用完全独立于语言的文本格式。
*   是理想的数据交换语言。易于人阅读和编写，同时也易于机器解析和生成。
*   如: XPathGenerator.jsm ?
***********************************************************************************************************************/

/***********************************************************************************************************************
* 检测访问者的浏览器
* 存取cookies
* 定时器 -- 
*   setTimeout ("回调函数", 时间间隔); -- 在指定的毫秒数后调用函数或计算表达式。默认只调用一次，如要多次，需要在"回调函数"末尾再次调用
*   setInterval("回调函数", 时间间隔); -- 按照指定的周期（以毫秒计）来调用函数或计算表达式（自动周期性）
*   
***********************************************************************************************************************/

/***********************************************************************************************************************
* JS是面向对象的编程语言(OOP)
* 
* 常用系统类
*   Array -- concat 合并两个数组，join 将数组中的所有元素组成一个字符串，sort([排序函数]) 排序， map(???)
*   Boolean -- 将非逻辑值(对象为 null 时表 false ?) 等 )转换为逻辑值（true 或者 false）
*     注意：字符串等于""时为false，否则为true，即使字符串的内容是 "false" 时逻辑值也为 true;
*           对象函数为 null 或 undefined 时 ?
*   Date
*   Math
*     random -- 返回 0 ~ 1.0 的随机数, 例：Math.floor(Math.random()*11) -- 返回一个介于 0 和 10 之间的随机数
*   RegExp("检测字符") -- 正则表达式检测，三个方法：
*     test <== 检索字符串中的指定值。返回值是 true 或 false
*     exec <== 检索字符串中的指定值。返回值是被找到的值。如果没有发现匹配，则返回 null
*     compile  <== ? 可以改变检索模式，也可以添加或删除第二个参数(参数值为 "g" 表示全局搜索)
***********************************************************************************************************************/

/***********************************************************************************************************************
* isNaN -- 判断一个字符串是否是数字
***********************************************************************************************************************/

/***********************************************************************************************************************
* 每次事件发生时都会有一个 event 对象， 表示激发事件时的对象？
*   stopPropagation() -- 
*   [target]
*
* 常用事件(onXXX)
*   onabort -- 图像加载被中断
*   onblur -- 元素失去焦点
*   onchange -- 用户改变域的内容
*   onclick -- 鼠标点击某个对象
*   ondblclick -- 鼠标双击某个对象
*   onerror -- 当加载文档或图像时发生某个错误
*   onfocus -- 元素获得焦点
*   onkeydown -- 某个键盘的键被按下
*   onkeypress -- 某个键盘的键被按下或按住
*   onkeyup -- 某个键盘的键被松开
*   onload -- 某个页面或图像被完成加载, 通常可用于检测浏览器版本、处理cookies等
*   onmousedown -- 某个鼠标按键被按下 
*   onmousemove -- 鼠标被移动
*   onmouseout -- 鼠标从某元素移开
*   onmouseover -- 鼠标被移到某元素之上
*   onmouseup -- 某个鼠标按键被松开
*   onreset -- 重置按钮被点击
*   onresize -- 窗口或框架被调整尺寸 
*   onselect -- 文本被选定
*   onsubmit -- 提交按钮被点击，通常可以验证所有的表单域，如果不满足检测条件，则返回false会取消提交
*   onunload -- 用户退出页面，通常可以保存 cookies 等
***********************************************************************************************************************/

/***********************************************************************************************************************
* HTML DOM(Document Object Model for HTML， HTML 文档对象模型)
* Document	代表整个 HTML 文档，用来访问页面中的所有元素。
* Anchor	代表 <a> 元素。
* Area	代表图像地图中的 <area> 元素。
* Base	代表 <base> 元素。
* Body	代表图像地图中的 <body> 元素。
* Button	代表 <button> 元素。
* Event	代表事件的状态
* Form	代表 <form> 元素
* Frame	代表 <frame> 元素
* Frameset	代表 <frameset> 元素
* Iframe	代表 <iframe> 元素, 不是同域也是可以操控的
* Image	代表 <img> 元素
* Input button	代表 HTML 表单中的按钮
* Input checkbox	代表 HTML 表单中的选择框
* Input file	代表 HTML 表单中的 fileupload 。
* Input hidden	代表 HTML 表单中的隐藏域。
* Input password	代表 HTML 表单中的密码域。
* Input radio	代表 HTML 表单中的单选框。
* Input reset	代表 HTML 表单中的重置按钮。
* Input submit	代表 HTML 表单中的确认按钮。
* Input text	代表 HTML 表单中的文本输入域。
* Link	代表 <link> 元素
* Meta	代表 <meta> 元素
* Object	代表一个 <Object> 元素
* Option	代表 <option> 元素
* Select	代表 HTML 表单中的选择列表。
* Style	代表某个单独的样式声明。
* Table	代表 <table> 元素。
* TableData	代表 <td> 元素。
* TableRow	代表 <tr> 元素。
* Textarea	代表 <textarea> 元素。

* Document
*   write
*   createElement
*   getElementById
* <form> 
*   [method] -- "post", 通过 [action] 指定目标， [onsubmit]="return checkForm()" 进行检测(如函数返回false会取消提交)
* History -- 出于隐私方面的原因，History 对象不再允许脚本访问已经访问过的实际 URL。traceObj 会得到 "SecurityError: The operation is insecure" 错误。
* <input>
*   [type] -- 指定类型，典型的有
*      "button"  <== 对应 onclick 事件;
*      "text"    <== 对应 onchange 事件
* Location  -- 
* Navigator -- 包含客户端有关浏览器的信息
* Screen -- 
* <window> -- 如果文档包含框架（frame 或 iframe 标签），浏览器会为 HTML 文档创建一个 window 对象，并为每个框架创建一个额外的 window 对象。
*   open <== 打开一个不可调整大小的窗体 ?
*   openDialog <== 打开一个可以调整大小的对话框 ?
*   addEventListener("事件名,如load/unload", function() { 匿名函数体;}, false); //具体意义? 
***********************************************************************************************************************/


