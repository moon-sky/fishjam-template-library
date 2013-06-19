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
* XMLHttpRequest
*   属性
*     readyState -- 整数,
*     response -- 字符串
*     responseText -- 返回的结果字符串
*     responseType
*     responseXML
*     status -- 
*     statusText -- ? 请求状态字符串， 如 success，error
*   回调
*     onabort
*     onerror
*     onreadystatechange -- 当 readyState 值改变的回调函数, 通常需要在其中判断 readyState(4) 和 status(200) 
*     onprogress
*   方法
*     open(METHOD, url, isAsync) -- 设置需要打开的 URL 及 METHOD(GET/POST)、是否异步 等
*     send(?) -- 发送数据
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
