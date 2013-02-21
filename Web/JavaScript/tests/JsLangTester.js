//★JavaScript区分大小写★
//Netscape中如果多次因为用 for 循环向文档重复写 HTML 代码会导致浏览器卡死

module("JSLanguageTester", {
    setup: function() {
    //dump("in JavaScriptLanguage::setup\n");
    },
    teardown : function () {
    //dump("in JavaScriptLanguage::teardown\n");
    }
});


//变量: 只包含字母、数字和/或下划线；要以字母开头；变量需要声明 没有声明的变量不能使用(undefined)
//      JS对数据类型的要求不严格，声明类型可以用赋予初始值的方法做到，但之后可以给变量赋予其他类型的值
//      通过 typeof(变量名) 或 typeof 变量名 的方式可以查询变量的类型，常见的类型有 string, number, array 等
test("变量", function() {
    var varNull;
    equal(varNull, null, "变量未定义或定义之后未赋值，则值为\"null\"");

    ok(0144 == 100, "八进制");
    ok(100 == 100, "十进制");
    ok(0x64 == 100, "十六进制");


    var varInfinity = 1 / 0;
    equal(varInfinity, Infinity, "Infinity");

    //NaN -- Not a Number
    var varNaN = parseInt("abc");
    equal(varNaN.toString(), NaN.toString(), "NaN 字符串比较");
    notEqual(varNaN, NaN, "NaN直接比较");  //注意：NaN 值非常特殊，因为它“不是数字”，所以任何数跟它都不相等，甚至 NaN 本身也不等于 NaN

    var iNumber = 100;
    ok(iNumber == 100, "数值");
    ok(typeof (iNumber) == "number", "typeof");
});

test("基础语法", function() {
    var iNumber = 2;
    var strNumber = "";
    switch (iNumber) {
        case 0:
            strNumber = "Zero";
            break;
        case 1:
            strNumber = "One";
            break;
        case 2:
            strNumber = "Two";
            break;
        default:
            strNumber = "Unknown";
            break;
    }
    equal(strNumber, "Two", "测试Switch");

    with (navigator) {
        var strAppName = appName;
        var strAppCodeName = appCodeName;

        ok(strAppName != "" && strAppCodeName != "", "测试 with() 语法，能取出对象的数据，不报错");
        //alert(strAppName);
    }

    equal(1, 1, "TODO: undefined 不是 JavaScript 常数, 不能直接使用 ?");
});

test("字符串", function() {
    var strInfo = "";
    for (var i = 1; i < 10; i++) {
        strInfo += i;
    }
    equal(strInfo, "123456789", "字符串和数字相加(+)会按字符串连接的方式");

    var strHelloworld = "hello world";
    ok(strHelloworld.length == 11, "String Length");

    ok(strHelloworld.charAt(0) == "h", "CharAt");       //返回该字符串位于第<位置>位的单个字符(Unicode)
    ok(strHelloworld.charAt(strHelloworld.length - 1) == "d", "CharAt(Length-1)");


    //charCodeAt -- 返回该字符串位于第<位置>位的单个字符的 ASCII 码。
    //var strChina = String.fromCharCode();   //返回一个字符串，该字符串每个字符的 ASCII 码由 a, b, c... 等来确定。

    var strHelloArray = strHelloworld.split(" "); //split返回一个数组，该数组是从<字符串对象>中分离开来的
    ok(strHelloArray.length == 2, "split length");
    ok(strHelloArray[0] == "hello", "Split Pos[0]");
    ok(strHelloArray[1] == "world", "Split Pos[1]");
});

test("运算符", function() {
    equal(100 == "100", true, "数字和字符串比较(==) -- 相等");
    equal(100 === "100", false, "数字和字符串比较(===) -- 不相等");

    equal(0xF1 & 0xE2, 0xE0, "与操作");
    equal(0xF1 | 0xE2, 0xF3, "或操作");
    equal(0xF1 ^ 0xE2, 0x13, "位异或操作");
    
});

test("数组", function() {
    var myArray = new Array();
    myArray[0] = "abc";
    myArray[1] = 123;

    var strArrayJoin = myArray.join(" "); //join方法把数组中的各个元素用<分隔符>串起来形成一个字符串，不影响数组内容
    ok(strArrayJoin == "abc 123");

    //注意：JavaScript 只有一维数组, 如果要模拟多维数组，需要将数组的元素设置为数组
    myArray[2] = new Array();
    myArray[2][0] = "1000";

    ok(myArray.length == 3, "array length");
    ok(parseInt(myArray[2][0]) == 1000, "array element");

    myArray.push("push info");
    equal(myArray.length, 4, "array push");
});

function sortLesser(a, b) {
    return (b - a);
}

test("数组排序", function() {
    var myArray = new Array();
    for (var i = 1; i < 10; i++) {
        myArray[i] = i;
    }
    equal(myArray.join("") ,"123456789");
    myArray.sort(sortLesser);       //按从大到小排序
    equal(myArray.join("") , "987654321");

});

test("Math", function() {
    //alert("PI=" + Math.PI);
    equal(Math.ceil(1.5), 2, "ceil 返回大于等于 x 的最小整数");
    equal(Math.floor(1.5), 1, "floor 返回小于等于 x 的最大整数");
    equal(Math.round(1.5), 2, "round 返回 四舍五入后的值");

    var myRandomNumber = Math.random() * 40 + 60;
    ok(myRandomNumber >= 60 && myRandomNumber <= 100, "random 返回大于 0 小于 1 的一个随机数");
});

test("Number", function() {
    ok(Number.MAX_VALUE < Number.POSITIVE_INFINITY);    //最大值 < 正无穷大
    ok(Number.NEGATIVE_INFINITY < Number.MIN_VALUE); //负无穷大 < 最小值
});

test("Date", function() {
    //Date日期对象。这个对象可以储存任意一个日期，从 0001 年到 9999 年，并且可以精确到毫秒数（ 1/1000 秒）
    //所有日期时间，如果不指定时区，都采用“UTC”世界时时区
    var d = new Date(2012, 11, 21, 1, 2, 3, 400);
    equal(d.getTimezoneOffset(), -480, "中国是东八区，与格林威治时间所差的分钟数为 -480(东方为负)");
    equal(d.getMonth(), 11, "注意月份是从 0 开始的");
    equal(d.toGMTString(), "Thu, 20 Dec 2012 17:02:03 UTC", "获取GMT(格林威治时间)格式的日期型字符串");
    ok(d.getFullYear() == 2012);

    var dParser = Date.parse(d);
    equal(dParser, 1356022923000, "Date.parse 返回该日期对象的内部表达方式(从1970年1月1日零时正开始计算到日期对象所指的日期的毫秒数");
    //equal(d.toString(), dParser.toString(), "直接定义和采用parse 两种方式相等 -- TODO: 为什么不能直接用等的方式？");
});

test("TODO: 全局函数", function() {
    equal(1, 1, "TODO: eval");  //eval把括号内的字符串当作标准语句或表达式来运行

    var strSrcURL = "<>[]{}+-=,.;;'\"";
    var strTargetURL = "%3C%3E%5B%5D%7B%7D+-%3D%2C.%3B%3B%27%22";
    equal(escape(strSrcURL), strTargetURL, "escape 把字符串按URL编码方法来编码(如空格变为 %20 ),如要对'+'编码，需要加参数(1)");
    equal(unescape(strTargetURL), strSrcURL, "unescape 解码括号中字符串成为一般字符串");
    equal(isFinite(parseInt("abc")), false, "isFinite -- 如果括号内的数字是“有限”的(MIN_VALUE和MAX_VALUE之间)就返回true");

    equal(parseInt("123"), 123, "parseInt -- 把括号内的内容转换成整数之后的值");
    equal(parseInt("0123"), 83, "前面有0时默认会按照八进制分析，可能造成错误");
    equal(parseInt("0123", 10), 123, "parseInt(xxx, 10) -- 强制按照10进制方式分析，★推荐★");

    equal(isNaN(parseInt("abc")), true, "parseInt -- 如果以字母开头，则返回“NaN")

    equal(parseFloat("123.456"), 123.456, "parseFloat返回把括号内的字符串转换成浮点数之后的值");

    equal("123".toString(), "123", "toString把对象转换成字符串");
    
    equal("123".toString(16), "123", "0x7B -- TODO(为什么不行？):toString(N)可以转换成特定进制");
});

/*
function SomeFun(num1, num2, num3) {
    for (var i = 0; i < arguments.length; i++) {
        alert(arguments[i]);
    }
}
*/

test("自定义函数", function() {
    equal(1, 1, "TODO:函数内部可以用 arguments 数组属性来获得外部程序调用函数时指定的参数信息");
    //这样可以在函数中获取任意多个参数 -- 不过对开发来说，可读性降低，只在调试时使用?
    
    //SomeFun(100, "abc", new Date(), new String("abcde"));

});


//指定事件处理程序的方法
//1.直接在 HTML 标记中指定, onload="xxx" onunload="xxx"
//2.编写特定对象特定事件的 JavaScript，通常很少用。
//  如：<script language="JavaScript" for="对象" event="事件">事件处理程序代码 </script>
//3.在 JavaScript 中说明, <对象>.<事件> = <事件处理程序>; 如 window.onerror = globalError; 
test("TODO: 事件处理程序", function() {

    ok(1, "TODO:事件处理程序的返回值(类似beHandled)：false表明需要系统继续处理；true表明已经处理完毕");
});


//window.onerror = globalError; //注意没有最后的括号"()"
//  返回值：
//    true --
//    false -- 
test("TODO: 错误处理程序", function() {

    ok(1, "TODO");
});
