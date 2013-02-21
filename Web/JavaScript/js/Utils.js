function getCookie(cookieName) {
    var cookieString = document.cookie;

    var start = cookieString.indexOf(cookieName + '=');
    // 加上等号的原因是避免在某些 Cookie 的值里有与 cookieName 一样的字符串。
    if (start == -1) // 找不到
    {
        return null;
    }

    //获取到cookie值的开始位置
    start += cookieName.length + 1;
    var end = cookieString.indexOf(';', start);  //得到值的结束位置

    if (end == -1) {
        end = cookieString.length;　
    }

    return unescape(cookieString.substring(start, end));
}

//使用方法: setCookie('username','fishjam', 30)
function setCookie(c_name, value, expiredays) {
　  var exdate=new Date();
    exdate.setDate(exdate.getDate() + expiredays);
    document.cookie = c_name + "=" + escape(value) + ((expiredays==null) ? "" : ";expires="+exdate.toGMTString());
}