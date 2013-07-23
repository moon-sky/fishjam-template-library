"use strict";

/*************************************************************************************************************************
* TODO: 
*   1.imageData使用二进制字节的数组，具有和TypedArray接近的性能，
*     可以先把imageData转换为类型数组以换取更快的执行速度
*   
*
* Typed Array -- 类型数组(http://www.kuqin.com/webpagedesign/20120407/319733.html)
*   JS可以操作原始二进制数据(以前必须把原始数据当作字符串来处理，并且使用charCodeAt方法来从数据缓冲区中读取字节)
*   类型数组在初始化后不能改变大小， 
*   常见的有： IntNArray/UintNArray(N 为 8/16/32), Float32Array/Float64Array 等
*     构造函数(buf [, 开始位置] [,长度])
*     
*     本质是各种类型的 ArrayBuffer 的视图,多个类型数组视图可以指向同一个arrayBuffer，采用不同的类型、不同的长度以及不同的位移。
*     TODO:为什么一个里面设置了值，另一个视图里体现不出来？

*   优点：
*   1.类型数组(一个固定的内存块)比传统数组(使用Hash查找方式)快的多，具有非常好的性能; http://blog.n01se.net/blog-n01se-net-p-248.html
*   2.类型数组使得Web应用可以使用多种二进制文件格式和直接操作文件的二进制内容，例如从现有的媒体文件中提取数据
*     显示二进制文件的内容：http://ie.microsoft.com/testdrive/HTML5/TypedArrays/
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
    var buf = new ArrayBuffer(8);
    equal(buf.byteLength, 8, "ArrayBuffer::ByteLength");

    // 创建一个指向buf的视图v1，采用Int32类型，开始于默认的字节索引0，直到缓冲区的末尾
    var v1 = new Int32Array(buf);
    equal(v1.byteLength, 8, "Int32Array::ByteLength");
    equal(v1.length, 2, "Int32Array::length");
    v1[0] = 0x01020304;
    v1[1] = 0x05060708;
    //设置后的内存结构: 04 03 02 01 | 08 07 06 05

    // 创建一个指向buf的视图v2，采用Uint8类型，开始于字节索引2，直到缓冲区的末尾
    var v2 = new Uint8Array(buf, 2);

    // 创建一个指向buf的视图v3，采用Int16类型，开始于字节索引2，长度为2(即第 3~6 字节范围 )
    var v3 = new Int16Array(buf, 2, 2);
    equal(v3.byteLength, 4, "Int16Array::ByteLength");
    equal(v3.length, 2, "Int16Array::length");
    equal(v3[0], 0x0102, "从Int16数组中取出第一个数字");
    equal(v3[1], 0x0708, "从Int16数组中取出第二个数字");
});
