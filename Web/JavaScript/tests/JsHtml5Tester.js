"use strict";
/*************************************************************************************************************************
* HTML5 中引入了很多新的元素
*  canvas -- 
*    getContext("2d") -- 获取2D上下文环境的函数
*    width/height -- 高宽属性
*    toDataURL() -- 把Canvas中的数据转换成URL，可以直接赋值给 img.src 
* 
*  context
*    createImageData
*    getImageData(left, top, width, height) -- 获取当前图像的imageData
*    putImageData(imgData, left, top)
*    fillStyle -- 填充颜色的属性,如 #ff0000 表示红色
*
*  imageData -- Canvas元素2D上下文环境里定义的数据类型
*    data -- 二进制的RGBA字节数组对象? 其中有 length 属性，可通过 [] 获取每一个字节的值
* 
*  文件拖拽支持(drag and drop)
* 
*  WebSocket
*    connection.send(byteArray.buffer);
*************************************************************************************************************************/


module("HTML5Tester", {
    setup: function() {
        //dump("in MathTester::setup\n");
    },
    teardown: function() {
        //dump("in MathTester::teardown\n");
    }
});

test("CanvasTester", function() {
    var canvasWidth = 100;
    var canvasHeight = 100;

    var c = document.createElement("canvas");
    var ctx = c.getContext("2d");
    var imgData = ctx.createImageData(canvasWidth, canvasHeight);

    equal(imgData.data.length, canvasWidth * canvasHeight * 4, "data 属性是一个字节数组，每一个像素都是 RGBA");

    //把imageData转换为类型数组以换取更快的执行速度
    var buf = new ArrayBuffer(imgData.data.length);
    var data = new Uint32Array(buf);
    for (var y = 0; y < canvasHeight; ++y) {
        for (var x = 0; x < canvasWidth; ++x) {
        
            //TODO: 似乎不对？
            var value = x * y & 0xff;
            data[y * canvasWidth + x] =
                (255 << 24)|        // alpha
                (value << 16)|      // blue
                (value <<  8)|      // green
                value;              // red
        }
    }
});

test("DragDropTester", function() {
    /*
    document.addEventListener("drop", function(e) {
    e.preventDefault();
    e.stopPropagation();
    var files = e.dataTransfer.files;
    if (files.length > 0) selected(files[0]);
    }, false);
    //然后 selected 中即可处理File, File 有 size 等属性
    */
    equal(1, 1, "TODO: DragDrop Tester");
});