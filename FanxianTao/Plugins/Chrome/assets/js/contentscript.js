//内容脚本(Content scripts), 和网页交互, 在网页载入完成后调用
//可以和 background.html 交互 -- 内容脚本在发现一个RSS Feed地址后将会给background.html发送一个信息

$(function() {
    /*
    $("*").mouseup(function(event) {
        console.log("event.which=%d, ctrlKey=%d", event.which, event.ctrlKey);
        if (event.which == 2 && event.ctrlKey) //鼠标中键
            window.clipboardData.setData("Text", $(this).html());
    });
    */
    
    //alert("in contentscript.js");
    $(":text, :password").css("backgroundColor", "red");
    //alert("inputNum=" + inputNum);
    console.log("in contentscript.js");
});