/*************************************************************************************************************************
* Typed Array -- 类型数组(http://www.khronos.org/registry/typedarray/specs/latest/)
*   JS可以操作原始二进制数据(以前必须把原始数据当作字符串来处理，并且使用charCodeAt方法来从数据缓冲区中读取字节)
*   类型数组在初始化后不能改变大小， 
*   常见的有： Int32Array
*     本质是各种类型的 ArrayBuffer 的视图，多个类型数组视图可以指向同一个arrayBuffer，采用不同的类型、不同的长度以及不同的位移。

*************************************************************************************************************************/


module("JsPerformanceTester", {
    setup: function() {
        //dump("in JsPerformanceTester::setup\n");
    },
    teardown: function() {
        //dump("in JsPerformanceTester::teardown\n");
    }
});

test("TypedArrayTester", function() {
    // 创建一个8字节的ArrayBuffer -- 然后下面的各种视图都指向该数组
    var b = new ArrayBuffer(8);

    // 创建一个指向b的视图v1，采用Int32类型，开始于默认的字节索引0，直到缓冲区的末尾
    var v1 = new Int32Array(b);

    // 创建一个指向b的视图v2，采用Uint8类型，开始于字节索引2，直到缓冲区的末尾
    var v2 = new Uint8Array(b, 2);

    // 创建一个指向b的视图v3，采用Int16类型，开始于字节索引2，长度为2
    var v3 = new Int16Array(b, 2, 2);
    
	equal(1, 1, "TODO: 性能问题");
});
