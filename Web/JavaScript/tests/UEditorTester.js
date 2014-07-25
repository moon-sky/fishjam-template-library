"use strict";

/******************************************************************************************************************************************
* UEditor(http://ueditor.baidu.com/) -- 百度的开源免费编辑器，由百度开发的所见即所得富文本web编辑器，允许自由使用和修改代码，基于MIT协议
*   ueditor.config.js -- 配置文件
*   ueditor.all.js -- 编辑器源码文件
*
* 分层架构设计
*   核心层 -- 提供编辑器底层的一些方法和概念，如DOM树操作、Selection、Range等。
*   命令和插件层 -- 所有功能型实现都是通过该层中的命令(静态方法)和插件(随编辑器初始化)来完成
*   UI层 -- UI设计与核心层和命令插件层几乎完全解耦,
*
* 使用方式
*   1.<script id="container" name="content" type="text/plain">
*        这里写你的初始化内容
*     </script>
*   2.var ue = UE.getEditor('container');
******************************************************************************************************************************************/

module("UEditorTester", {
    setup: function() {
        //dump("in UEditorTester::setup\n");
    },
    teardown: function() {
        //dump("in UEditorTester::teardown\n");
    }
});

test("UEditor", function() {
    //var fr = new FileReader();
    //console.log("%o", fr);
    //equal(1, 1, "TODO:JS支持File吗");
});
