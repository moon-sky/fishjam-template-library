"use strict";

module("MathTester", {
    setup: function() {
        //dump("in MathTester::setup\n");
    },
    teardown: function() {
        //dump("in MathTester::teardown\n");
    }
});

test("MathTester", function() {
	//将 时间(125S) 转换为 MM:SS 对应的字符串
	var mm = Math.floor(125 / 60.0);
	var ss = parseInt(125 % 60);
	ss = (ss < 10) ? '0' + ss : ss;
	var result = mm + ':' + ss;
	equal(result, "2:05", "时间转换");
});
