function getCookie(cookieName) {
    var cookieString = document.cookie;
    var start = cookieString.indexOf(cookieName + '=');
    // 加上等号的原因是避免在某些 Cookie 的值里有与 cookieName 一样的字符串。
    if (start == -1) // 找不到
    {
        return null;
    }
    start += cookieName.length + 1;
    var end = cookieString.indexOf(';', start);
    if (end == -1) return unescape(cookieString.substring(start));
    return unescape(cookieString.substring(start, end));
}