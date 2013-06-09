/******************************************************************************************************************************************
* 开发IDE(代码自动提示)
*   1.Dreamweaver + jQuery_API.mxp 插件
*   2.Aptana -- 基于Eclipse，专注于JavaScript的Ajax开发IDE，(Windows -> Preferences -> Aptana ->Editors -> Javascript -> Code Assist)
*   3.Eclipse + jQueryWTP 或 Spket 插件
*   4.Visual Studio 2008 + KB958502 补丁 + jquery-vsdoc.js(该文件版本和jQuery一致，而且 "-" 前面的部分必须和jQuery文件名一样，放在相同目录)
*     测试似乎不行?

* JQuery(jquery.com) 免费、开源(使用MIT许可协议)，是一个轻量级的JavaScript库(精简后只有20多K)，兼容各种浏览器,极大地简化了 JavaScript 编程
*   能方便地处理HTML元素、事件等，实现动画效果，为网站提供AJAX交互，使得HTML页保持代码和页面分离(不用插入js进行调用，只需定义id即可)
*   
* 使用：head中包含 <script type="text/javascript" src="jquery.js"></script>
*       jquery.js 可以保存到本地或者服务器直接引用，也可以直接使用CDN提供的(优点:用户已经访问过时会缓存;缺点:万一连不上?)
*       Google CDN: http://ajax.googleapis.com/ajax/libs/jquery/1.8.0/jquery.min.js  -- 注：1.8.0 可以省略成 1.8 或 1 等(自动选择满足要求的最高版本)
*   两个版本：Production version -- 已被精简和压缩，用于实际的网站
*             Development version -- 完整无压缩，用于测试、学习和开发
*
* jQuery对象 和 DOM 对象对比
*   jQuery对象 -- 通过jQuery包装DOM对象后产生的对象(是一个数组)，只能使用jQuery的方法。命名时通常在变量名前面加上 "$"
*   DOM 对象 -- 通过 document.getXxx 返回的对象
*      $("#foo").html() <== 等价于 ==> document.getElementById("foo").innerHTML
*      jQuery变量转换为DOM: var domObj = $jObj[index] 或 $jObj.get(index), 本质是从数组里取元素
*      DOM变量转换为jQuery: var $jObj = $(domObj), 本质是通过 $(objName) 工厂函数生成, 如 $(document)
*
* 基础语法： $就是 jQuery的一个简写形式，如 $("#foo") 和 jQuery("#foo") 等价; $.ajax 和 jQuery.ajax 等价
*   $(selector).action() -- 美元符号定义 jQuery; 选择符(selector)查询和查找HTML 元素; action() 执行对元素的操作
*   $(document).ready(function(){ xxx }); -- 等待dom元素加载完毕执行指定语句
*   .ready(function() { xxx });
*   .click(function() { xxx });
*  
******************************************************************************************************************************************/

/******************************************************************************************************************************************
* 选择器(Selectors) -- (基本语法同CSS选择器),允许开发者使用从CSS1到CSS3几乎所有的选择器及jQuery独创的高级而复杂的选择器，通过插件还可以支持XPath选择器，甚至可以编写属于自己的选择器
*   如果没有满足条件的元素(如没有指定id的元素)，调用jQuery方法也不会报错(因为选择器的返回是一个数组，只是此时数组个数为 0)
*   基本选择器
*     tagType -- 选择所有指定类型的元素， 如类型 <p> 的 $("p")
*     #IdObj -- 选择  id="IdObj" 的元素
*     .className -- 选择所有 class="className" 的元素
*     this -- 选择当前元素,通常用在事件处理代码中，表示激发该事件的对象
*     sel1,sel2 -- 将多个选择器匹配的元素合并后一起返回
*     * -- 匹配所有元素 
*   层次选择器
*     ancestor descendant -- 选取ancestor元素里的所有descendant元素(包括所有儿孙), 如 $("div span")选取<div>里的所有<span>，注意选择的是 <span>
*     parent > child -- 选取 parent 元素下的 child 子元素(只包括儿子),如 $("div > span")选取<div>元素下元素名是<span>的子元素
*     prevSel + nextSel -- 选取紧接在 prevSel 元素后的 nextSel 元素，更常见的用法是 $("prevSel").next("nextSel")
*     prevSel ~ siblings -- 选取 prevSel 元素之后的所有 siblings 元素，更常见的用法是 $("prevSel").nextAll("siblings)
*  过滤选择器 -- 通过特定的过滤规则来筛选
*     :first -- 选取第一个元素，如 $("div:first")选取第一个<div>元素
*     :last -- 选取最后一个元素
*     :not(selector) -- 去除所有与给定选择器匹配的元素，如 $("input:not(.myClass)") 选取class不是myClass的<input>元素
*     :even -- 选取索引是偶数的所有元素，索引从0开始，常用于表格的交替颜色
*     :odd -- 选取索引是奇数的所有元素，索引从0开始，常用于表格的交替颜色
*     :eq(index) -- 选取选取索引是index的单个元素，索引从0开始，而且只匹配一个元素
*     :gt(index) -- 选取索引大于index的所有元素(大于 index 而不包含index)
*     :lt(index) -- 选取索引小于index的所有元素(小于 index 而不包含index)
*     :header -- 选取所有的标题元素(h1~h6)
*     :animated -- 选取当前正在执行动画的所有元素
*   内容过滤选择器 -- 根据包含的子元素或文本内容进行选择
*     :contains(text) -- 选取含有文本内容为"text"的元素， 如 $("div:contains('我')") 选取含有文本"我"的<div>元素
*     :empty -- 选取不包含子元素或文本的空元素, 和 :parent 相反？
*     :has(selector) -- 选取含有选择器所匹配的元素的元素，如 $("div:has(p)") 选取含有 <p>元素的<div>元素，注意选择的是 <div>
*     :parent -- 选取含有子元素或者文本的元素(即自身是父控件)，如 $("div:parent") 选取拥有子元素(包括文本元素)的<div>元素
*   可见性过滤选择器
*     :hidden -- 选取所有不可见的元素，包括 <input type="hidden"/>, style="display:none", style="visibility:hidden;" 等的元素
*     :visible -- 选取所有可见的元素
*   属性过滤选择器
*     [attribute] -- 选取拥有此属性的元素， 如 $("div[id]") 选取拥有id属性的<div>元素
*     [attribute=value] -- 选取属性值为 value 的元素，如 $("div[title=test]") 选取属性title为"test"的<div>元素
*        对应的有 !=value(不等于), ^=value(属性值以value开始), $=value(属性值以value结束), *=value(属性值含有value)
*     [selector1][selector2] -- 将属性按“与”的方式合并，同时满足多个条件，如 $("div[id][title*='test']") 选取有属性id，并且属性title中含有"test"值的<div>元素
*   子元素过滤选择器(注意和过滤选择器的区别：1.子元素过滤选择器的索引从开始; 2.子元素过滤选择器会对每个父元素都过滤)
*     :nth-child(index/even/odd/equation) -- 选取每个父元素下的第index个子元素/奇/偶/?? 元素(index从1开始算)，如 $("div.one :nth-child(3n)")选取class为one的div父元素中索引值是3的倍数的元素
*        注意：为每一个父元素匹配子元素，并且index是从1开始的
*     :first-child -- 选取每个父元素的第一个子元素，如 $("ul li:first-child") 选取每个<ul>中第一个<li>元素
*     :last-child -- 选取每个父元素的最后一个子元素
*     :only-child -- 如果某个元素是它父元素中唯一的子元素，将会匹配(即父元素中只有一个子元素)，如 $("ul li:only-child") 在<ul>中选取是唯一子元素的<li>元素
*   表单对象属性过滤器 -- 对所选择的表单元素进行过滤
*     :enabled/disabled -- 选取所有 可用/不可用 元素
*     :checked -- 选取所有被选中的元素(单选框/复选框)
*     :selected -- 选取所有被选中的选项元素(下拉列表) 
******************************************************************************************************************************************/

/******************************************************************************************************************************************
*
* 插件(模块化的使用方式)：
* DOM操作封装 -- 封装了大量常用的DOM操作
* 可靠的事件处理机制 -- 
* 完善的Ajax -- 将所有的Ajax操作封装到一个函数 $.ajax() 里，开发者处理Ajax时能专心处理业务逻辑
* 不污染顶级变量 -- 只建立一个名为jQuery的对象，其所有的函数方法都在这个对象之下，其别名 $ 也可以随时交出控制权( jQuery.noConflict() )，
*   绝不会污染其他的对象(可和其他库共存而无冲突)
* 出色的浏览器兼容 -- 
* 链式操作方式 -- 对发生在同一个jQuery对象上的一组动作，可以直接连写而无需重复获取对象(代码优雅)
* 隐式迭代 -- 方法都被设计成自动操作对象集合，而不是单独的对象，不需要编写大量的循环结构代码
* 行为层与结构层的分离 -- 可以使用jQuery选择器选中元素，然后直接给元素添加事件，这种分离的是想可以是jQuery开发人员和页面开发人员各司其职
* 丰富的插件支持 -- 
******************************************************************************************************************************************/

/******************************************************************************************************************************************
* 函数
*   .attr("属性") -- 返回指定的属性 ?
*   .css("属性", "新的值") -- 设置对应的css属性值
*   .html() --
*   .is(":checked") -- 判断是否被选中 ?
*   .next("xx)
*   .nextAll("xxx") -- 选取当前元素 后面的 同辈"xxx"元素
*   .siblings("xxx") -- 选取当前元素同辈的"xxx"元素
*   .show(毫秒数) -- 
*
* 事件( .xxx(function() { xxx });
*   .click() -- 设置click事件?
*   .ready() -- 如 $(document).ready(function(){ xxx });
*     网页中所有DOM结构绘制完毕就执行，可能DOM元素关联的东西没有加载完(比如图片?)；可以同时编写多个(window.onload只能有一个)
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
