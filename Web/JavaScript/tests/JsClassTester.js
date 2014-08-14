"use strict";

//JavaScript继承详解系列 -- http://www.cnblogs.com/sanshi/archive/2009/07/08/1519036.html

//JS 本身不是面向对象的语言，而是基于对象的语言,所有事物都是对象 -- 因此其面向对象的概念和C++等不一致
//JS 采用对象化编程（或面向对象编程），每个对象有它自己的属性、方法和事件，不是完全的面向对象式语言，只是基于对象(Object-Based)的方式
//JS 没有类的概念？ -- 采用“原型化继承(prototypal inheritance)”的概念，对象本身可以用来创建新对象，对象也可以继承自其他对象
//   任何函数都可以被实例化为一个对象 
//   原型(prototype) -- 把prototype看做是一个模版，新创建的自定义对象都是这个模版(prototype)的一个链接


//有两种面向对象的编程方式？
//1. 类似 MyMathClass ? 每一个定义之间用逗号"," 分隔
//2. 类似CShape的原型prototyp方式(★推荐★) -- 可以实现继承，每个类都有一个 __proto__ 的原型链，最顶层是 Object: 
//   构造函数: function ClassName(参数列表) { 实现 };  ClassName.prototype.方法名 = function(参数列表) { 方法实现; }, 
//     注意：constructor始终指向创建当前对象的构造函数
//   使用： var 变量名 = new 类名(参数);
//   1.公共方法：prototype(原型) 属性包含了可以作为所有新副本的基引用(base reference)
//   2.私有方法(private method)：在函数内部定义的函数，这样只能在函数内部调用。问题：除了定义的函数外，该类的其他方法都不能使用，那基本没有什么用处了？
//   3.特权方法(privileged method)：查看并处理对象中私有变量的同时允许用户以公共方法访问的方法。也在函数内定义，但增加 this. 前缀。
//     说明信息:http://javascript.crockford.com/private.html
//     动态生成代码(特权方法在新对象实例化时才创建)比 prototype 上绑定一个简单的方法开销更大，但功能更强大和灵活(参见 "CUser(properties)" )
//   4.静态方法 -- 
//   5.使用 uber 来访问 parent
//?匿名对象(通过大括号包起来的一些属性值集合?) -- { 属性名: 属性值 }

//注意写法:
//MyClass.prototype.xxx = function(){ ... };    -- 这种写法是修改prototype, ok(myObj.constructor == MyClass);
//MyClass.prototype = { xxx : function(){...} } -- 这种写法是覆盖prototype，等价于 MyClass.prototype = new Object({...}); ok(myObj.constructor == Object)

var MyMathClass = { //TODO: 这个到底定义的是类还是对象？
    //构造函数中：this.<属性名> = <初始值>;
	 someValue: 10, 
	 init : function(value) {
		 this.m_value = value;
	 },
     add : function(num1, num2) {
       return num1 + num2;
     },
	//其他定义?	 
};

//注意：有两种不同的构造方式 -- 但似乎不支持重载 ?
//*
//通过原型(prototype)定义类 -- JS可以使用 prototype 属性在已经定义好的可实例化对象类型(即定义好构造函数)中加入属性和方法.
function CUser(name, birthday){      //构造函数
    this.name = name;
    this.birthday = birthday;
    
    //定义只能在内部使用的私有方法
    function dumpInfo() {
        //TODO:这里用 this.name/this.age 会出错，为什么?
        //console.log("CUser Init: name=%s, age=%s", this.name, this.age); 
        console.log("CUser Init: name=%s, birthday=%s", name, birthday); 
    }
    dumpInfo();
    
    //定义的特权方法(注意前面的 this. ) -- 即不能被继承的方法,通过 log 可发现其方法不在 __proto__ 里面
    this.privilegedName = function() {
        return this.birthday;
    };
    
    //JavaScript从入门到精通P124左右介绍的旧方法 -- 未测试是否有效
    //1.定义全局的 CUser_xxx 方法，实现中使用 this.name 等方式来操作内部变量
    //2.在构造中将方法赋值并生成对应的函数, 如 Xxx = CUser_xxx; 然后外界即可用 user.Xxx() 方法来操作
}

CUser.prototype.getName = function(){  //定义的公有方法 -- 本质是将函数(getName)添加到此对象的 prototype 对象中(作为其属性)
    //dumpInfo();  //不能在这些函数内使用, 私有方法似乎没有什么用处？
    return this.name;
};
CUser.prototype.getBirthday = function() {
    return this.birthday;
};
CUser.prototype.getAge = function() {
    var age = (new Date()).getFullYear() - this.birthday.getFullYear();
    return age;
};

CUser.prototype.whoAreYou = function(){
    return "I'm " + this.name + " and my birthday is " + this.birthday;
}

//静态方法
CUser.clone = function( user ) {
    return new CUser(user.getName(), user.getBirthday());
};
/*/
function CUser(properties) {  //创建一个新的用户对象，接收一个有许多属性的对象作为参数
    for (var i in properties) { (function(which) {  //遍历该对象的所有属性
          var p = i; 
          which["get" + p] = function() {       //创建此属性的一个新的读取器(getter)
            return properties[p];
          };
          which["set" + p] = function(val) {    //创建此属性的一个新的设置器(setter)
            properties[p] = val;
          };
      })(this);
   }
};
//*/

//function CShape(){
//    this.name = "unknown";
//}
function CShape(name){ //构造函数， 注意：JS没有重载的概念？所以增加一个默认构造似乎没有用处?
    this.name = name;
}

//定义CShape的原型(prototype是每个函数都有的属性)，原型中的属性可以被自定义对象引用
CShape.prototype = {
    getName : function(){
        return this.name;
    },
    toString : function(){
        return this.name + "(in my toString)";
    }
}


//CRect -- 从CShape继承原型prototype中的所有属性和方法
function CRect(width, height){  //构造函数
    CShape.call(this, "rect");
    this.width = width;
    this.height = height;
}
CRect.prototype = new CShape();  //将CRect的原型指向CShape的一个实例，因此可以调用CShape原型中的所有属性
CRect.prototype.constructor = CShape;
CRect.prototype.getSize = function(){
    return this.width * this.height;
}


//定义一个函数，可以当作函数或对象来使用
function User( name ){
	this.name = name;
}

function SomeConstructor(intParam1)
{
    this._value = intParam1;
};


module("JsClassTester", {
    setup: function() {
        //dump("in JsClassTester::setup\n");
    },
    teardown: function() {
        //dump("in JsClassTester::teardown\n");
    }
});


var someParamForCall = "in window param";

//定义一个从 this 中返回 someParamForCall 的函数
function returnParamForThis(){      //注意：因为启用了strict模式，this默认值将是 undefined(而非window)
   return this.someParamForCall;    //函数也是对象,也有this,在运行时决定this的具体指(而非函数定义时)
}

//全局函数 apply 和 call -- 可以改变函数中this的具体指向
//apply和call两个函数的作用相同，唯一的区别是两个函数的参数定义不同
//apply用法?: function xxx(){ if(...) { this.Yyy.apply(this, arguments) } } -- 根据条件调用该类的其他函数，并传递参数过去
test("call and apply", function(){
    
    //两种等价调用方式
    equal(window.returnParamForThis(), "in window param", "返回全局中的变量值"); //因为启用了strcit模式，this不再默认指向window，所以此处必须加上 window. 的方式调用
    equal(returnParamForThis.apply(window), "in window param", "在函数上使用apply");
    
    var paramObj = { //自定义一个对象，并将此对象的属性 someParamForCall 指向全局函数 someParamForCall
        someParamForCall : "in Object Param",
        returnParamForThis : window.returnParamForThis,
    };
    //两种等价调用方式
    equal(paramObj.returnParamForThis(), "in Object Param", "返回paramObj对象中的变量值");
    equal(returnParamForThis.apply(paramObj), "in Object Param", "在函数上使用apply改变this");
});

test("Class", function() {
	var obj1 = new Object(); //创建系统提供的Object对象
	obj1.val = 5;			//任意一个赋值语句都会设置其属性
	equal( obj1.val, 5, "赋值时自动增加或重设属性");
	//obj.val = "change string";
	//equal( typeof obj.val, "string");
	obj1.click = function() { alert("hello"); };
	
	//上面代码等价于 -- 不同的语法
	var obj2 = {
		val : 5,
		click : function() { alert("hello"); }
	};


    var someObj = new SomeConstructor(1);
    
    var addResult = MyMathClass.add(1 , 2);
    ok(addResult == 3, "Call Class Function");
});

test("funObj", function() {
	//equal(window.name, "test", "原来的window.name");
	var me = new User("fishjam");	//将函数实例化为一个对象

	equal(me.name , "fishjam", "简单的函数作为对象的方式"); 
	equal(me.constructor , User, "me.constructor 是User对象的一个实例");  //
	
	var you = new me.constructor(); //
	equal(typeof(me), typeof(you), "使用对象的 construcotr 可以创建相同类型的对象，类似拷贝构造 ?");

    //未启用严格模式时造成名称污染的bug	
	//User("setUserName");					//当成正常的函数来使用 -- 注意：严格模式下会抛出异常，必须通过new来使用构造
	// equal(window.name, "setUserName");		//但因为是在 window 里面调用的，会设置其name属性，造成名称污染,容易出现Bug?
	new User("setUserName");
	equal(window.name, "");  //启用严格模式后，不会造成名称污染
	   
	
	//函数可以作为对象来建立，并用 Function() 构造器将函数对象赋予变量，其语法为 "new Function([arg1,..,argn], functionBody);
	var triple = new Function("n", "return 3*n");  //创建将数值乘以3以后的函数
	equal( triple(100), 300, "函数对象");
});


test("prototype", function(){
//*
	//使用 prototype 定义的类, 其属性都在对象中，且有合适的上下文
	var userObj1 = new CUser("fishjam", new Date(1979, 3, 30, 12, 0, 0, 0)); //1979-03-30 12:00:00:000
	equal(userObj1.constructor, CUser, "constructor始终指向创建当前对象的构造函数");
	equal(userObj1.constructor.constructor, Function, "构造函数的constructor是函数对象");
	
	console.log("prototype 创建的类CUser对象: %o", userObj1);
	equal(userObj1.whoAreYou(), "I'm fishjam and my birthday is Mon Apr 30 1979 12:00:00 GMT+0800 (中国标准时间)", "调用共有方法");
	equal(userObj1.getName(), "fishjam", "userObj1.getName() == fishjam");
	equal(userObj1.getAge(), 35, "userObj1.getAge() == 35, 老了。。。。");
	
	
	
	var userCloneObj = CUser.clone(userObj1);
	console.log("%s:%o", userCloneObj.toString(), userCloneObj);
	console.log("%s:%o", userObj1.toString(), userObj1);
	//ok(userCloneObj == userObj1, "静态方法");   //TODO: 两个对象的比较( == 操作符的重载)
/*/
	//创建一个新的用户对象实例，并把具有两个属性的一个对象传入进行初始化 -- 会自动生成对应的 getter/setter
	var userObj2 = new CUser({ 
	    Name:"fishjam", 
	    Birthday: new Date(1979, 3, 30, 12, 0, 0, 0) 
	});

	ok(userObj2.Name == null);   //注意：不存在名为Name的属性--它是属性对象的私有变量

	equal(userObj2.getName(), "fishjam");
	ok(userObj2.getBirthday().toString() == new Date(1979, 3, 30, 12, 0, 0, 0).toString());
	
	userObj2.setName("fujie");
	equal(userObj2.getName(), "fujie");
//*/

    equal(1, 1, "prototype");


});

test("继承", function(){
    equal(1, 1, "test");
    
    //new 时内部的实际流程:
    //  1.创建一个空白对象 -- new Object()
    //  2.拷贝 CShape.prototype 中的属性(键值对)到这个空对象中(内部实现时不是拷贝而是一个隐藏的链接)
    //  3.将这个对象通过this关键字传递到构造函数中并执行构造函数
    //  4.将这个对象赋值给变量unkShape
    var unkShape = new CShape("shape");
    console.log("unkShape=%o, toString=%s, getName=%s", unkShape, unkShape.toString(), unkShape.getName());

    var rectShape = new CRect(10, 20);
    console.log("rectShape=%o, toString=%s, getName=%s", rectShape, rectShape.toString(), rectShape.getName());
    equal(rectShape.getSize(), 200, "调用矩形的计算面积函数"); 
    
});

//向JavaScript固有类型Array扩展一个获取最小值的方法
//陷阱: 向Array的原型中添加扩展方法后，当使用for-in循环数组时，这个扩展方法也会被循环出来
//       解决方法：使用 hasOwnProperty 判断
Array.prototype.min = function() {
    var min = this[0];
    for (var i = 1; i < this.length; i++) {
        if (this[i] < min) {
            min = this[i];
        }
    }
    return min;
};

Array.prototype.sum = function(){
    var total = 0;
    for (var i = 0; i < this.length; i++) {
        total += this[i];
    }
    return total;
}

test("扩展系统提供的功能" , function(){
    var arrNumber = [ 2, 4, 6, 8, 10, 1, 3, 5, 7, 9];
    equal(arrNumber.min(), 1, "扩展系统的Array, 增加min方法");
    equal(arrNumber.sum(), 55, "扩展Array，增加sum方法");
    
    for( var attr in arrNumber){
        if (!arrNumber.hasOwnProperty(attr)) {
            console.log("%o", attr); //会打印出 "min" 和 "sum"， TODO:为什么没有打印出10?
            ok(attr== "min" || attr =="sum", "自定义的扩展会在for-in循环时被枚举出来");
        }
    }
});