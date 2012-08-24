/*
MyMath.prototype =
{
    AddNumber: function(num1, num2) {
        var result = num1 + num2;
        return result;
    }
};

/*/

//模拟类 -- 另外一种语法呢?
function MyMath() {
    dump("in MyMath constructor\n");
}

MyMath.prototype = {
    property1: true,
    AddNumber: function(num1, num2) {
        var result = num1 + num2;
        alert("in AddNumber, result " + result);
        return result;
    }
};
//*/

function myShowErrorInfo() {
    var promptService = Components.classes["@mozilla.org/embedcomp/prompt-service;1"].getService(Components.interfaces.nsIPromptService);
    promptService.alert(window, "title", "msg");
    alert("in myShowErrorInfo:" + promptService.name);
}
