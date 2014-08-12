"use strict";

/**************************************************************************************************************************
* Ajax(Asynchronous JavaScript and XML) -- 异步JavaScript与XML，加快用户响应(如输入时的自动补全?GoogleMap等)
*   在 Ajax开发中，还可以利用隐藏框架实现各种技巧 -- 如无刷新上传文件
*   其核心是 XMLHttpRequest -- 发起远程HTTP访问
*   其一个技术标志是可以局部刷新Web页面，而保持页面的其他部分不受任何影响
* 缺点：
*   复杂性 -- 可以通过 ASP.NET Ajax 控件来降低，组件将自动呈现所需的JS代码以实现所需的Ajax效果
*   浏览器支持 -- 如低版本或禁用浏览器JavaScript功能的用户
*   可能破坏浏览器后退按钮的正常行为
* TODO:
*   1.因为使用ajxa传递参数的时候需要head支持，必须放在WebServer(Apache/IIS)下测试
*
*
***************************************************************************************************************************/

/***************************************************************************************************************************
* XMLHttpRequest -- 核心对象，发送异步请求、接收响应、执行回调
*   属性
*     readyState -- 状态整数。0(尚未初始化),1(已调用open准备发送),2(已调用send发送),3(正在接收),4(完成响应，此时才可以处理responseText等信息)
*     response -- 字符串
*     responseText -- 客户端接收到的HTTP响应的文本内容，当readyState为4时才有意义
*     responseType
*     responseXML -- MIME 类型被指定为 text/xml或application/xml时，XML格式的响应
*     status -- 描述HTTP状态码(如 200 表示成功)，只有当 readyState 为3或4 时才能访问，否则会引发异常
*     statusText -- HTTP状态码文本(success, error,notmodified,timeout)
*     withCredentials -- 是否发送验证信息，通常用于跨域访问时(设置为 true) -- 会发送cookie过去?
*   回调事件
*     onabort
*     onerror
*     onreadystatechange -- 当readyState值改变的回调函数, 通常需要在其中判断 readyState(4) 和 status(200) 后才能继续处理
*     onprogress
*   方法
*     abort() -- 暂停操作(请求或接收),并将对象设置为初始化状态
*     getAllResponseHeaders() -- 获取所有的HttpResponse的头部信息
*     getResponseHeader() -- 检索响应的头部值，仅当 readyState 为 3或4 时才可调用
*     open(METHOD, uri, async, username, passwd) -- 初始化，设置需要打开的 URI 及 METHOD(GET/POST)、是否异步(默认为true) 等
*     send({包含可变类型的参数数据}) -- 发送请求,GET时使用null作为参数，POST时使用指定参数发送附加数据
*     setRequestHeader(header, value) -- 设置请求的 Content-Type 头部信息，在open后调用，如 send(data)中的参数类型为DOMString,
*       则数据被编码为UTF-8；如数据是Document类型，则由data.XmlEncoding指定的编码串行化该数据
* 模式
*   同步模式
*   异步模式
***************************************************************************************************************************/

module("AjaxTester", {
    setup: function() {
        //dump("in AjaxTester::setup\n");
    },
    teardown: function() {
        //dump("in AjaxTester::teardown\n");
    }
});


test("TODO:Ajax", function() {
    var bCreateByActiveX = false;
    var xmlHttpReq = null;          //声明一个空对象用来装入XMLHttpRequest
    if (window.ActiveXObject) {     //IE5 IE6是以ActiveXObject的方式引入XMLHttpRequest的
        xmlHttpReq = new ActiveXObject("Microsoft.XMLHTTP");
        bCreateByActiveX = true;
    }
    else if (window.XMLHttpRequest) {//除IE5 IE6 以外的浏览器XMLHttpRequest是window的子对象
        xmlHttpReq = new XMLHttpRequest(); //实例化一个XMLHttpRequest
        bCreateByActiveX = false;
    }
    
    //或者简写成如下方式
    //  xmlHttpReq = window.ActiveXObject ? new ActiveXObject("Microsoft.XMLHTTP") : new XMLHttpRequest();
    
    equal(bCreateByActiveX, false, "现在的浏览器中XMLHttpRequest是window的子对象");
    ok(xmlHttpReq != null, "创建 XMLHttpRequest 对象");


//*
    if (xmlHttpReq != null) {//如果对象实例化成功
        //xmlHttpReq.open("GET", "http://www.baidu.com", false);      //调用open()方法并采用同步方式
        xmlHttpReq.onreadystatechange = RequestCallBack;    //设置回调函数
        //xmlHttpReq.send(null);                              //因为使用get方式提交，所以可以使用null参调用
    }

    console.log("%o", xmlHttpReq);

    function RequestCallBack() {
        if (xmlHttpReq.readyState == 4) {
            if (xmlHttpReq.status == 200) {
                //将xmlHttpReq.responseText的值赋给ID为 resText 的元素
                alert(xmlHttpReq.responseText);
                //document.getElementById("resText").innerHTML = xmlHttpReq.responseText;
            }
        }
    }
//*/

});
