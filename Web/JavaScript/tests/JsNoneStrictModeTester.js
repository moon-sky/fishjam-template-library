//测试非 Strict Mode的一些功能
//"use strict";

/******************************************************************************************************************
* 严谨模式(Strict Mode) -- 通过 "use strict;" 声明
* 目标：能更快更方便的调试，严格模式会抛出更多错误，引起注意并修复很多以前很难被发现的潜在问题
*   1.去除了with语句 -- 若使用会抛出异常
*   2.局部变量在赋值前必须先进行申明 -- 未启用时为一个未申明的局部变量复制时会自动创建一个同名全局变量。
*   3.函数中的this不再默认指向全局window对象 -- this在被赋值之前会一直保持为undefined，因此调用构造时必须使用 new
* 注意：
*   1.不要在全局环境下启用严格模式 -- 放在函数内部或立即执行函数表达式(IIFE)中，否则第三方等的代码会出问题
*     示例: (function() { "use strict"; function doSomeFun(){}} ())
******************************************************************************************************************/


module("NoneStrictModeTester", {
    setup: function() {
        //dump("in NoneStrictModeTester::setup\n");
    },
    teardown: function() {
        //dump("in NoneStrictModeTester::teardown\n");
    }
});

test("with", function() {
    with (window.document) {
        equal(links.length > 0, true, "非严格模式下可以通过with简化代码，但可能出错");
    }
});

(function() {
    //"use strict";
    console.log("启用Strict模式的时候，使用了with的代码会报错 'Strict mode code may not include a with statement'");
    test("with", function() {
        with (window.document) {
            equal(links.length > 0, true, "Chrome的Console中会出错");
        }
    });
}());
 
