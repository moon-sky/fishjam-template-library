//JS 采用对象化编程（或面向对象编程），每个对象有它自己的属性、方法和事件
var MyMathClass = {
    //构造函数中：this.<属性名> = <初始值>;
    
     add : function(num1, num2) {
       return num1 + num2;
     },
};

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
    var someObj = new SomeConstructor(1);
    
    var addResult = MyMathClass.add(1 , 2);
    ok(addResult == 3, "Call Class Function");
    
    ok(1, "with 为一个或一组语句指定默认对象, 参见 window.document 测试");
    ok(1, "this 返回\"当前\"对象。在不同的地方，this 代表不同的对象, 全局代表 window对象");
});