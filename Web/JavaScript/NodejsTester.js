"use strict";
/************************************************************************************************************
* NodeJs(http://nodejs.org/) -- 使用Google的V8虚拟机(Chrome浏览器)，来解释和执行JavaScript代码
*   npm(node packagemanager) -- node包管理器，第三方的package全是通过npm去安装的
*     npm安装uglify-js <== npm –g install uglify-js
************************************************************************************************************/


module("NodejsTester", {
    setup: function() {
        //dump("in NodejsTester::setup\n");
    },
    teardown: function() {
        //dump("in NodejsTester::teardown\n");
    }
});

test("NodeTester", function() {
    equal(1, 1, "NodeJs");
});
