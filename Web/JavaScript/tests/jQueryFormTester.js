"use strict";

/******************************************************************************************************************************************
* jQueryForm(jquery.form.js) -- 大大简化了使用ajax技术提交表单时的数据传递问题(不需要逐个以JS的方式获取表单属性的值并拼接成查询字符串格式)
*   $("#myForm'). 方法:
*     ajaxForm; -- 增加所有需要的事件监听器，为AJAX提交表单做好准备，该函数不提交表单,一般放在 ready 中。
*       可有0或1个参数( function(response){} 的成功回调函数  或 Options 对象)
*     ajaxSubmit -- 提交表单，参数同ajaxForm。依赖于事件机制(只要有事件存在就可以使用，不需要提供submit按钮).
*       注意：一般在调用的地方为了避免浏览器进行表单提交造成刷新，需要 return false;
*     formToArray
*     formSerialize -- 将表单序列化成一个查询字符串并返回(即 & 分隔的 name=value )，然后可用 $.get、$.post、$.ajax等来提交数据
*     fieldSerialize -- 将表单的字段元素序列化成一个查询字符串并返回,通常用于只对部分表单字段进行序列化时
*       使用方式: $("#myForm .specialFields").fieldSerialize();
*     fieldValue -- 返回匹配插入数组中的表单元素值, 总是以数组的形式返回数据
*     clearForm -- 清除表单元素(将所有的 text, password, textarea, select, radio, checkbox 等置空)
*     clearFields -- 清除指定的字段元素
*     resetForm -- 通过调用表单元素原有的DOM方法，将表单恢复到初始状态
*  
*   Options对象
*     clearForm[] -- 布尔标志，表示如果表单提交成功是否清除表单数据
*     dataType[null] -- 期待返回的数据类型，如 null(表纯文本)/xml/script/json 等
*     resetForm[false] -- 布尔标志，表示如果表单提交成功是否进行重置
*     semantic[false] -- 布尔标志，表示数据是否必须严格按照语义顺序来进行提交，TODO:一般用于有 <input type="image"> 的元素?
*     target[null] -- 指明页面中由服务器响应进行更新的元素 -- TODO:什么意思？示例: '#divToUpdate'
*     type[form.method] -- 指定提交表单数据的方法(method):"GET"或"POST"
*     url[form.action] -- 指定提交表单数据的URL
*     beforeSubmit(数组形式的表单数据, jQuery表单对象, Options对象){ return boolean; }[null] -- 
*       表单提交前被调用的回调函数,作为钩子被提供来运行预提交逻辑或者校验表单数据，若该函数返回false则表单不被提交
*       表单数组接受以下方式的数据(TODO:是否能简化?)：[ { name: 'username', value: 'fish' }, { name: 'password', value: 'secret' } ]
*     success(response){} -- 表单成功提交后调用的回调函数,服务器返回响应后被调用,response 的类型由dataType决定
*   
******************************************************************************************************************************************/

module("jQueryFormTester", {
    setup: function() {
    //dump("in jQueryFormTester::setup\n");
    },
    teardown: function() {
    //dump("in jQueryFormTester::teardown\n");
    }
});

test("jQueryForm 的测试", function() {
    equal(1, 1, "TODO:jQueryForm");
});
