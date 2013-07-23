"use strict";

/*************************************************************************************************************************
JavaScript中的正则表达式
  ? / xxxx / 中间包含， 后面可以加 g(表全句) ?
  RegExp 对象表示正则表达式, 创建语法为 new RegExp(pattern, attributes);
*************************************************************************************************************************/


module("RegularExprTester", {
    setup: function() {
        //dump("in MathTester::setup\n");
    },
    teardown: function() {
        //dump("in MathTester::teardown\n");
    }
});

test("正则表达式", function() {
    //判断邮件地址：
    var bEmail = /.+@.+\.[a-zA-Z]{2,4}$/.test("fishjam@163.com");
    equal(bEmail, true, "合法的邮件地址");

    bEmail = /.+@.+\.[a-zA-Z]{2,4}$/.test("fishjam");
    equal(bEmail, false, "非法邮件地址");
});

test("RegExp, TODO:", function() {
    var regExp = new RegExp();
    equal(1, 1, "TODO:")
});

