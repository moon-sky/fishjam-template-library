"use strict";

/*************************************************************************************************************************
* JavaScript相关的工具
*   TODO: 对JS文件进行合并，可以减少http请求优化网络耗时提升性能
* 
* 混淆 -- JS代码必须明文下载到客户端，其保护非常困难。一般通过 改名+压缩 的方式增加解析的难度。
*   Javascript 是一种基于原型的语言，没有严格的类型定义。
*     在自定义的类中，对于需要外部访问的属性和方法(如外部函数和全局变量等)，不能进行混淆；
*     对于内部访问的属性和方法，需要进行混淆
*     
* 混淆工具
*   jasob -- 能同时混淆HTML、CSS和JS，效果很好，但收费。
*   Javascript Obfuscator -- 收费，不支持同时混淆HTML
*   YUI Compressor(https://github.com/yui/yuicompressor) -- 免费,可以混淆JS，不支持同时混淆HTML。
*   JSCompress(个人作品) -- http://www.zhaixiaowai.com/JSCompress/
*   UglifyJS -- jQuery 1.5以后使用的工具。基于 NodeJS 的Javascript语法解析/压缩/格式化工具。
*
* 反混淆工具 -- 一般是源码格式化工具
*   Chrome中可以通过 Sources -> Pretty print 来格式化混淆后的代码。
*   WEBSTORM --  
*************************************************************************************************************************/


module("ToolsTester", {
    setup: function() {
    //dump("in ToolsTester::setup\n");
    },
    teardown: function() {
    //dump("in ToolsTester::teardown\n");
    }
});

test("JavaScript相关的工具", function() {
    equal(1, 1, "TODO:工具");
});


test("JUnit测试", function() {
    var objNull;
    equal(objNull, null, "jUnit的 equal 采用 == 进行判断");
    ok(objNull !== null, "证明 objNull !== null");
});
