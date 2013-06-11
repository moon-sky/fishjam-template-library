  /*
	辅助例子
	1,自动重置
	2,给元素添加动画
  */

$(document).ready(function() {
    //地址框、密码框中的提示，TODO：最好再增加颜色控制(文字是灰色)
    //var addrPromptText = "请输入邮箱地址";
    //var passwdPromptText = "请输入邮箱密码";
    
    $("#address").focus(function() {         // 地址框获得鼠标焦点
        var txt_value = $(this).val();   // 得到当前文本框的值
        if (txt_value == this.defaultValue) {
            $(this).val("");              // 如果符合条件，则清空文本框内容
            //$(this).css("color", "red");
        }
    });
    $("#address").blur(function() {		  // 地址框失去鼠标焦点
        var txt_value = $(this).val();   // 得到当前文本框的值
        if (txt_value == "") {
            $(this).val(this.defaultValue); // 如果符合条件，则设置内容
           // $(this).css("color", "#7f7f7f");
        }
    })

    //TODO: 似乎密码框不能显示文字? -- 怎么处理?
    $("#password").focus(function() {
        var txt_value = $(this).val();
        if (txt_value == this.defaultValue) {
            $(this).val("");
        }
    });
    $("#password").blur(function() {
        var txt_value = $(this).val();
        if (txt_value == "") {
            $(this).val(this.defaultValue);
        }
    })


    //手动重置页面元素
    $("#reset").click(function() {
        $("*").removeAttr("style");
        $("div[class=none]").css({ "display": "none" });
    });

    //判断是否自动重置
    $("input[type=button]").click(function() {
        //if($("#realodwindow").is(":checked")){
        //   window.location.reload();
        //}else 
        if ($("#isreset").is(":checked")) {
            $("#reset").click();
        }
    });

    //TODO: 新加的 -- 加载完毕后马上激发 click, 保证其默认为选中状态(在 HTML 里设置了 checked="checked" 无法设置初始值)
    $("#isreset").trigger('click');

    //给id为mover的元素添加动画.
    function animateIt() {
        $("#mover").slideToggle("slow", animateIt);
    }
    animateIt();
})