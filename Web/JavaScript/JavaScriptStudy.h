
//<a href="javascript:self.close()"> 关闭 </a>

/***********************************************************************************************
* 帮助：MSDN -> Development Tools and Languages -> Visual Studio -> JScript
*
* 语法：
*   1.变量区分大小写，使用前需要通过 var <变量名> [ = <初始值> ] 的方式声明，默认赋值为"null"。JavaScript 对数据类型的要求不严格。
*     null -- 一个特殊的空值，当变量未定义，或者定义之后没有对其进行任何赋值操作时的值；
*     Number.NaN(Not a Number) -- 当运算无法返回正确的数值时，返回该值(如 parseInt("字母"))，任何数(包括NaN)跟它都不相等
*     Infinity -- 无穷大，对应的有 Number.NEGATIVE_INFINITY 和 Number.POSITIVE_INFINITY
*   2.数组(Array) -- 对象的集合，里边的对象可以是不同类型的，通过 [下标] 访问, var myArray = new Array(可选的初始列表);
*       JS只支持一维数组。如想模拟多维，需要类似 new Array(new Array(), xxx) 的方式，访问时通过 myArray[2][3] = xxx;
*       join(分隔符) -- 使用分隔符把数组中的各个元素串起来并返回相应的字符串，通常可用于调试输出?
*       sort([排序函数]) -- 按指定的排序函数进行排序，如按升序排列数字的排序函数： function myGreater(a, b){return a-b;}
*   3.Date -- 可存储 0001到9999的任意一个日期，精确到毫秒，内部实现是一个整数(1970-01-01 00:00:00:000 开始的毫秒数)，
*     如不指定时区，默认采用"UTC"。有 get/set[UTC]XXXX 的方法，来获取或设置对应的时间值。
*     getTimezoneOffset() -- 返回日期对象采用的时区与格林威治时间所差的分钟数，中国(GMT+0800)返回 -480。
*   4.异常处理：
*     try { xxxx } catch(e) { alert('some error'); }
*   5.一些常用的特殊函数
*     eval() -- 把括号内的字符串当作标准语句或表达式来运行
*     escape() -- 返回括号中的字符串经过编码后的新字符串，该编码应用于URL，也就是把空格写成"%20"这种格式
*     parseInt()/parseFloat() -- 返回把括号内的内容转换成整数/浮点数之后的值，参数中最好指定 radix 以防止出现特殊字符串(如0xxx);
***********************************************************************************************/

/***********************************************************************************************
* 对象化编程
*   调用时：className.funName1();
*   var className = {
*     funName1 : function(param1, param2) {
*       this.initialized = true;
*       return [返回值];
*     },
*     funName2 : function(param..) {
*     }
*   };
*
*   属性
*   方法
*     任何对象都有 toString() 方法，返回可读的字符串形式
*   事件--响应发生在对象上的事情
*
*   
* 文档接收更多事件 <== document.captureEvent()
***********************************************************************************************/



//Jscript提供SendKeys方法，可用于将一个或多个键击发送到活动窗口。

var CFUtil = {
    //打开一个新的窗口，或是使得已经创建的窗口获得焦点
    openWindow: function(windowName, url, flags, params) {
        var windowsMediator = Components.classes["@mozilla.org/appshell/window-mediator;1"].getService(Components.interfaces.nsIWindowMediator);
        var aWindow = windowsMediator.getMostRecentWindow(windowName);
        if (aWindow) {
            aWindow.focus();
        }
        else {
            aWindow = window.openDialog(url, windowName, flags, params);
        }
        return aWindow;
    },
    
    
   openProgram : function () {
   	    var WshShell = WScript.CreateObject("Wscript.Shell");	//创建WshShell对象
	    WshShell.Run("notepad",3); //利用WshShell对象来启动记事本,并把它最大化
	    WshShell.AppActivate("记事本"); //启动完应用程序后最好能先调用一下AppActivate方法，确保应用程序的窗口被激活
	    WScript.Sleep(1000);//有些GUI的界面响应速度会比较慢，因此在各操作之间最好能插入时间缓冲

	    WshShell.SendKeys("ABCD...");	//发送按键给窗口来操作应用程序的功能
	    WScript.Sleep(1000);

	    WshShell.SendKeys("%OF");   //发送组合功能键(ALT+O+F键调出记事本的字体设置窗口)
   }
};

//CFUtil.openProgram();
