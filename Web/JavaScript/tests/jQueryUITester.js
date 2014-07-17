"use strict";

/******************************************************************************************************************************************
* 官方演示: http://jqueryui.com/demos/
* 
* jQueryUI -- 在jQuery 基础上开发的一套界面工具,免费开源,几乎包括了网页上你所能想到和用到的插件以及动画特效
* 使用方法：
*   0.通过 <script src="/js/jquery-x.y.z.js" type="text/javascript" /> 引入jQuery 库
*   1.通过 <link href="/css/jquery-ui-x.y.z.xxxx.css type="text/css" rel="stylesheet" /> 引入样式表;
*   2.通过 <script src="/js/jquery-ui-x.y.z.xxxx.js" type="text/javascript" /> 引入jQueryUI库
*   3.定义一个div，将需要应用jQueryUI的内容放入其中；
*   4.利用 jQuery 在 $(document).ready(function (){ 启用jQueryUI }); -- 注意:启用时可以传入参数，
*     如: $("#myAccordion").accordion({ collapsible: true, autoHeight: false });
*
* Interactions(互动元素)
*   Draggable&Droppable(拖拽)
*   Resizable(调整大小)
*   Selectable(通过鼠标选择，支持Ctrl)
*   Sortable(通过鼠标排序)
* 小工具(Widget)
*   Accordion(折叠窗口), 同时只能打开一个标签
*   Autocomplete(自动完成)
*   Button(各种按钮)
*   Datepicker(日期选择)
*   Dialog(对话框)
*   Menu
*   Progressbar
*   Selectmenu(类似Commobox的下拉选择菜单)
*   Slider
*   Spinner -- TODO: 其中演示了googlemap 的用法
*   Tabs(选项卡)
*   Tooltip
* Effects(各种动画效果)
* Utilities
*   Position(动态布局)，包括一个点击鼠标后自动切换图片的Demo
*   Widget Factory -- 
*
* Theme Roller(可以设置主题) -- http://jqueryui.com/themeroller/
*   
******************************************************************************************************************************************/

module("jQueryUITester", {
    setup: function() {
    //dump("in jQueryUITester::setup\n");
    },
    teardown: function() {
    //dump("in jQueryUITester::teardown\n");
    }
});

test("jQuery UI的测试", function() {
    equal(1, 1, "TODO:jQueryUI");
});
