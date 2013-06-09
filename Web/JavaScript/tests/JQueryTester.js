/******************************************************************************************************************************************
* JQuery(jquery.com) 免费、开源(使用MIT许可协议)，是一个轻量级的JavaScript库(精简后只有20多K)，兼容各种浏览器,极大地简化了 JavaScript 编程
*   能方便地处理HTML元素、事件等，实现动画效果，为网站提供AJAX交互，使得HTML页保持代码和页面分离(不用插入js进行调用，只需定义id即可)
*
* 使用：head中包含 <script type="text/javascript" src="jquery.js"></script>
*       jquery.js 可以保存到本地或者服务器直接引用，也可以直接使用CDN提供的(优点:用户已经访问过时会缓存;缺点:万一连不上?)
*       Google CDN: http://ajax.googleapis.com/ajax/libs/jquery/1.8.0/jquery.min.js  -- 注：1.8.0 可以省略成 1.8 或 1 等(自动选择满足要求的最高版本)
*   两个版本：Production version -- 已被精简和压缩，用于实际的网站
*             Development version -- 完整无压缩，用于测试、学习和开发
*
*      
* 基础语法： $就是 jQuery的一个简写形式，如 $("#foo") 和 jQuery("#foo") 等价; $.ajax 和 jQuery.ajax 等价
*   $(selector).action() -- 美元符号定义 jQuery; 选择符(selector)查询和查找HTML 元素; action() 执行对元素的操作
*   $(document).ready(function(){ xxx }); -- 等待dom元素加载完毕执行指定语句
*   .ready(function() { xxx });
*   .click(function() { xxx });
*  
* 插件(模块化的使用方式)：
*       
* 选择器(Selectors) -- (基本语法同CSS选择器),允许开发者使用从CSS1到CSS3几乎所有的选择器及jQuery独创的高级而复杂的选择器，通过插件还可以支持XPath选择器，甚至可以编写属于自己的选择器
*   基本选择器
*     $("tagType") -- 选择所有指定类型的元素， 如类型 <p> 的 $("p")
*     $("#IdObj") -- 选择  id="IdObj" 的元素
*     $(".className") -- 选择所有 class="className" 的元素
*     $(this) -- 选择当前元素,通常用在事件处理代码中，表示激发该事件的对象
*     $("sel1,sel2) -- 将多个选择器匹配的元素合并后一起返回
*     $("*) -- 选取所有元素 
*  层次选择器()
*     $("ancestor descendant") -- 选取ancestor元素里的所有descendant元素(包括孙子?)
*     $("parent>child") -- 选取 parent 元素下的 child 子元素(只包括儿子?)
*     $("
*
* DOM操作封装 -- 封装了大量常用的DOM操作
* 可靠的事件处理机制 -- 
* 完善的Ajax -- 将所有的Ajax操作封装到一个函数 $.ajax() 里，开发者处理Ajax时能专心处理业务逻辑
* 不污染顶级变量 -- 只建立一个名为jQuery的对象，其所有的函数方法都在这个对象之下，其别名 $ 也可以随时交出控制权(如何?)，绝不会污染其他的对象(可和其他库共存而无冲突)
* 出色的浏览器兼容 -- 
* 链式操作方式 -- 对发生在同一个jQuery对象上的一组动作，可以直接连写而无需重复获取对象(代码优雅)
* 隐式迭代 -- 方法都被设计成自动操作对象集合，而不是单独的对象，不需要编写大量的循环结构代码
* 行为层与结构层的分离 -- 可以使用jQuery选择器选中元素，然后直接给元素添加事件，这种分离的是想可以是jQuery开发人员和页面开发人员各司其职
* 丰富的插件支持 -- 
******************************************************************************************************************************************/

/******************************************************************************************************************************************
* 函数
*   .ready(xxx) -- 网页中所有DOM结构绘制完毕就执行，可能DOM元素关联的东西没有加载完(比如图片?)；可以同时编写多个(window.onload只能有一个)
******************************************************************************************************************************************/

$(document).ready(function() {
    $("button").click(function() {
        $("p").hide();
    });
});

module("JQueryTester", {
    setup: function() {
        //dump("in JQueryTester::setup\n");
    },
    teardown: function() {
        //dump("in JQueryTester::teardown\n");
    }
});

test("TODO: JQuery", function() {
    equal(1, 1, "TODO:");
    
    $(document).ready(function() {
        alert("after document loaded");
    });
});
