/********************************************************************************************
* http://jquery.com/
* jQuery JQuery 免费、开源(使用MIT许可协议)，是一个轻量级的JavaScript库(精简后只有20多K)，兼容各种浏览器,极大地简化了 JavaScript 编程
*   能方便地处理HTML元素、事件等，实现动画效果，为网站提供AJAX交互，使得HTML页保持代码和页面分离(不用插入js进行调用，只需定义id即可)
*
* 使用：head中包含 <script type="text/javascript" src="jquery.js"></script>
*       jquery.js 可以保存到本地或者服务器直接引用，也可以直接使用CDN提供的(优点:用户已经访问过时会缓存;缺点:万一连不上?)
*       Google CDN: http://ajax.googleapis.com/ajax/libs/jquery/1.8.0/jquery.min.js  -- 注：1.8.0 可以省略成 1.8 或 1 等(自动选择满足要求的最高版本)
*   两个版本：Production version -- 已被精简和压缩，用于实际的网站
*             Development version -- 未压缩，用于测试和开发
*     
* 基础语法：$(selector).action() -- 美元符号定义 jQuery; 选择符(selector)查询和查找HTML 元素; action() 执行对元素的操作
*   $(document).ready(function(){ xxx }); -- 
*   .ready(function() { xxx });
*   .click(function() { xxx });
*  
* 插件(模块化的使用方式)：
*       
* 选择器(Selectors)
*   $(this). -- 选择当前元素
*   $("#test") -- 选择  id="test" 的元素
*   $(".test") -- 选择所有 class="test" 的元素
*   $("类型") -- 选择所有该类型的元素， 如类型 <p> 的 $("p")
*   
    

********************************************************************************************/

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
});
