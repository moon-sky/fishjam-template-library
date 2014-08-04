"use strict";

//JS 采用对象化编程（或面向对象编程），每个对象有它自己的属性、方法和事件，不是完全的面向对象式语言，只是基于对象(Object-Based)的方式
//JS 没有类的概念？ -- 采用“原型化继承(prototypal inheritance)”的概念，对象本身可以用来创建新对象，对象也可以继承自其他对象
//   任何函数都可以被实例化为一个对象 

//有两种面向对象的编程方式？
//1. 类似 MyMathClass ? 每一个定义之间用逗号"," 分隔
//2. 类似CUser的原型方式(★推荐★): 
//   构造函数: function ClassName(参数列表) { 实现 };  ClassName.prototype.方法名 = function(参数列表) { 方法实现; }, 
//   使用： var 变量名 = new 类名(参数);
//   1.公共方法：prototype(原型) 属性包含了可以作为所有新副本的基引用(base reference)
//   2.私有方法(private method)：在函数内部定义的函数，这样只能在函数内部调用。问题：除了定义的函数外，该类的其他方法都不能使用，那基本没有什么用处了？
//   3.特权方法(privileged method)：查看并处理对象中私有变量的同时允许用户以公共方法访问的方法。也在函数内定义，但增加 this. 前缀。
//     说明信息:http://javascript.crockford.com/private.html
//     动态生成代码(特权方法在新对象实例化时才创建)比 prototype 上绑定一个简单的方法开销更大，但功能更强大和灵活(参见 "CUser(properties)" )
//   4.静态方法 -- 
// 
//?匿名对象(通过大括号包起来的一些属性值集合?) -- { 属性名: 属性值 }

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
    
    //定义的特权方法(注意前面的 this. )
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