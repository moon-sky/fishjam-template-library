//properties 文件必须重启FF才能显示更改


//这种写法会采用全局变量的方式加载？会造成修改 properties 文件后必须重启 FF 才能生效

//*
//注意：每一项(变量或函数)之间通过逗号(,)分隔，而不是分号，结尾采用分号
//  这种写法实际上定义了一个全局的变量，而且会在js文件加载时就执行
var localize =
{
    _bundle: Cc["@mozilla.org/intl/stringbundle;1"]
    .getService(Ci.nsIStringBundleService)
    .createBundle("chrome://fishjamTest/locale/res.properties"),

    getLocalizedMessage: function(strKey) {
        var strValue = this._bundle.GetStringFromName(strKey);
        return strValue;
    }
};

/*/

//这种写法似乎也不能刷新 properties 文件
function Localize() {}
    Localize.prototype = {
    getLocalizedMessage: function(strKey) {
        var bundleService = Cc["@mozilla.org/intl/stringbundle;1"].getService(Ci.nsIStringBundleService);
        var bundle = bundleService.createBundle("chrome://fishjamTest/locale/res.properties");
        var strValue = bundle.GetStringFromName(strKey);
        return strValue;
    },
    anotherFun : function() {
    }
};
var localize = new Localize();

//*/
