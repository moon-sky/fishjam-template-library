module("CookieTester", {
    setup: function() {
        //dump("in MathTester::setup\n");
    },
    teardown: function() {
        //dump("in MathTester::teardown\n");
    }
});

test("读写Cookie", function() {
    var oldCookie = document.cookie;

    with (window.document) {
        //注意：本地磁盘中的页面，chrome的控制台是无法用JavaScript读写操作 cookie 的
        //一个网站可以创建多个cookie(通过 域 或 路径?)
        
        //cookie 的类型(typeof) 是字符串(string), 因此可以用字符串的方式来操作(如 indexOf, substring 等)
        //每个Cookie所存放的数据不能超过4K，格式为 <cookie名>=<值>;
        //路径(routing) -- 默认时，只有与创建 cookie 的页面在同一个目录或子目录下的网页才可以访问，通过设置 path=xxx 属性即可更改
        //域(domain) -- 不同的域(如www.qq.com和sports.qq.com)之间不能直接访问cookie，通过设置 domain=qq.com 且设置path=x 可以让同域的能访问。


        //每个 Cookie 都有失效日期，一旦电脑的时钟过了失效日期，这个 Cookie就会被删掉。默认是临时性质的，浏览器关闭即删除？
        //不能直接删掉一个 Cookie，但是可以用设定失效日期早于现在时刻的方法来间接删掉它(即不能通过 cookie="" 来清除所有的cookie )
        //  设定 Cookie 的失效日期都是用 GMT 格式的时间的，通过 expires=date 值来设定
        var expires = new Date();
        expires.setTime(expires.getTime() + 3 * 30 * 24 * 60 * 60 * 1000); //设定大约三个月后过期
        //expires.setDate(expires.getDate() + 30);  //表示设置30天以后

        //加密传输(设置 secure 属性，没有属性值 ) -- 通常 cookie 信息都是使用HTTP连接传递数据，不安全
        //  注意：只是网络传输时加密，本地保存未加密(需要自己加密)

        var strCookie = "name=fishjam,email=fishjam@163.com;secure;path=/;domain=jstest.com;expires=" + expires.toGMTString();
        cookie = escape(strCookie); //设置时最好使用 escape -- cookie信息时不能包含空格，分号，逗号等特殊符号
        equal(unescape(cookie), strCookie, "document.cookie， TODO: 多个cookie用什么分隔？普通是逗号, 失效时间前是分号?");
        equal(0, 0, "TODO: 怎么判断、设置失效时期? expires ?, 默认的生命周期是浏览器关闭时");
    }
    
    //恢复Cookie原来的值
    document.cookie = oldCookie;
    
    //ok(0144 == 100, "八进制");

});