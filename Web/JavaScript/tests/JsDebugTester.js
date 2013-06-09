/*************************************************************************************************************************
* 调试 -- Firefox 中的 Firebug
* TODO：写一个自己的封装类，来提供各个浏览器中都可以使用的日志功能，且能很好的切换各种实现(如是否启用、是否网络发送等?)
*
* Console -- 用于显示 JS和 DOM 对象信息的单独窗口，js中有全局的 console 对象，可以输出信息到Console窗口
*   方法：共分5中等级(debug < info < log < warn < error), 各种的颜色不一样
*     log(object[,object,...]) -- 输出语法，支持格式化(%s,%d,%o 等)
*     assert(expression[, object, ...])  -- 断言输出，当 expression 表达式为 false 的时候使用 error 方式输出
*     clear(); -- 清空 console 中的所有信息
*     count([title]) -- 统计当前代码被执行过多少次，title 参数可以在次数前面输出额外的标题以帮助阅读
*     dirxml(obj); -- 把 html 元素的html 代码打印出来(实际测试类似 %o)
*     group|groupCollapsed|groupEnd (object[, object, ...]) -- 把 log 等输出的信息进行分组，方便阅读查看 
*     trace() -- 打印调用堆栈
*     time|timeEnd (name) -- 测试js函数的执行时间,time(name)根据name创建1个新的计时器。timeEnd(name)停止给定name的计时器，并显示时间。
*     table(data) -- (只在FireBug1.6+以后才支持) 把data 对象用表格的方式显示出来，这在显示数组或者JSON 对象的时候非常有用
*     profile|profileEnd (name) -- 打开浏览器的分析器，用于分析这组函数之间的 js 执行情况
* 
*   浏览器的支持：
*     支持：Chrome、FireFox(FireBug) 
*     部分支持：IE8的开发工具(但功能比较单调,显示对象时不好), 其他版本的IE可以安装 Developer Toolbar
*     Firebuglite(https://getfirebug.com/) -- Firebug 推出的工具，可以让所有浏览器都支持Console功能? 
*************************************************************************************************************************/


module("JsDebugTester", {
    setup: function() {
        //dump("in DebugTester::setup\n");
    },
    teardown: function() {
        //dump("in DebugTester::teardown\n");
    }
});

test("ConsoleTester", function() {
    ok(1 == 1, "TODO: Console Tester");

    console.groupCollapsed("ConsoleTester");  //groupCollapsed 默认是折叠的， group 默认是展开的
    {
        console.time("consoleTime");        //开始计时
        //console.profile("consoleProfile");  //开始分析

        console.log("simple formatted text log, s-字符串:%s, d-整数:%d, f-浮点数:%f",
        "SomeString", 100, 3.1415926);

        console.log("o-对象: %o", this);

        console.log("自定义的匿名对象：%o", { AA: "OK", BB: "No" }); //采用的是类似 Json 的方式
        //console.table({ AA: "OK", BB: "No" });  //Chrome 不支持，尚未测试FireBug

        console.debug("some debug string");
        console.info("some info string");
        console.log("some log string");
        console.warn("some warn string");
        console.error("some error string");   //Chrome中会多输出调用堆栈(trace)

        console.assert(1 == 1, "不会输出这行语句(1==1)");
        console.assert(1 == 0, "会输出这行语句(1==0)");

        console.trace();    //打印调用堆栈

        //console.profileEnd("consoleProfile");   //结束分析，每一次都会生成一个 profile 文件，因此在Tester里面关闭

        console.count("consoleCount");          //执行次数
        console.timeEnd("consoleTime");         //结束计时并显示所用时间
    }
    console.groupEnd();

    console.log("some other info out of group");    //打印调用堆栈
});
