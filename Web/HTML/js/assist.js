  /*
	辅助例子
	1,自动重置
	2,给元素添加动画
  */

$(document).ready(function() {

    //自定义的Tooltip显示(系统的提示效果响应比较慢,而且没有动画效果)
    var tooltipOffsetX = 10;
    var tooltipOffsetY = 20;
    $("a.tooltip").mouseover(function(e) {
        this.myOldTitle = this.title;      //定义一个新的变量，保存原来的 title 值
        this.title = "";
        var tooltip = "<div id='tooltip'>" + this.myOldTitle + "<\/div>"; //创建 div 元素
        $("body").append(tooltip); //把它追加到文档中
        $("#tooltip")
			    .css({
			        "top": (e.pageY + tooltipOffsetY) + "px",
			        "left": (e.pageX + tooltipOffsetX) + "px"
			    }).show("fast");   //设置x坐标和y坐标，并且显示
    }).mouseout(function() {
        this.title = this.myOldTitle;
        $("#tooltip").remove();   //移除 
    }).mousemove(function(e) {
        $("#tooltip")
			    .css({
			        "top": (e.pageY + tooltipOffsetY) + "px",
			        "left": (e.pageX + tooltipOffsetX) + "px"
			    });
    });




    //地址框、密码框中的提示，TODO：最好再增加颜色控制(文字是灰色)
    //var addrPromptText = "请输入邮箱地址";
    //var passwdPromptText = "请输入邮箱密码";

    $("#jqueryEval, #address, #password").focus(function() {         //获得焦点
        var txt_value = $(this).val();   // 得到当前文本框的值
        if (txt_value == this.defaultValue) {   //注意使用的 DOM 对象属性
            $(this).val("").removeClass("translucence");              // 如果符合条件，则清空文本框内容
            //$(this).css("font-color", "red");
        }
    });
    $("#jqueryEval, #address, #password").blur(function() {		  // 失去焦点
        var txt_value = $(this).val();   // 得到当前文本框的值
        if (txt_value == "") {
            $(this).val(this.defaultValue).addClass("translucence"); // 如果符合条件，则设置内容
            // $(this).css("color", "#7f7f7f");
            //$(this).css("opacity", "0.5");
        }
    })

    $("#jqueryCheckBtn").click(function() {
        //alert("aaaaaaa");
        //alert($("#jqueryEval").val());
        alert($(this).offset().left + ", " + $(this).offset().top);
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