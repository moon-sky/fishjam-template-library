/************************************************************************
* Ajax(Asynchronous JavaScript and XML) -- 异步JavaScript与XML，加快用户响应(如输入时的自动补全?)
*   在 Ajax开发中，还可以利用隐藏框架实现各种技巧 -- 如无刷新上传文件
*   其核心是 XMLHttpRequest -- 发起远程HTTP访问
*   其一个技术标志是可以局部刷新Web页面，而保持页面的其他部分不受任何影响
* 缺点：
*   复杂性 -- 可以通过 ASP.NET Ajax 控件来降低，组件将自动呈现所需的JS代码以实现所需的Ajax效果
*   浏览器支持 -- 如低版本或禁用浏览器JavaScript功能的用户
*************************************************************************/

module("AjaxTester", {
    setup: function() {
        //dump("in AjaxTester::setup\n");
    },
    teardown: function() {
        //dump("in AjaxTester::teardown\n");
    }
});

test("TODO:Ajax", function() {
	 equal(1024, 1024, "Ajax");
});
