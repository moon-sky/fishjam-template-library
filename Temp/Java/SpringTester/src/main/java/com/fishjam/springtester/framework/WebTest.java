package com.fishjam.springtester.framework;

/**************************************************************************************************************************************
 * Web开发中需要了解的东西(★强烈推荐★) -- http://www.open-open.com/news/view/1ca444
 * 
 * 注意事项
 *   1.浏览器兼容 -- 通常需要测试 Firefox(Gecko引擎)、Safari|Chrome(Webkit引擎)、Opera浏览器、
 *     IE浏览器(使用微软的Application Compatibility VPC Images, 下载地址: http://www.microsoft.com/en-us/download/details.aspx?id=11575)
 *     可通过 http://browsershots.org/ 查看网页在不同的浏览器下是怎么显示出来的(输入网址、勾选浏览器，指定分辨率、JS、Flash版本等)。
 *       启动对应的虚拟机访问网站后，截图并上传，然后用户可以浏览确认。
 *   
 *   
 * 最佳实践
 *   0.TODO: 一些需要的共有库文件可以通过CDN(比如  百度静态资源库 http://cdn.code.baidu.com/ )加速。
 *     缺点: 这样的话不能更改库中bug，似乎也不好?
 *   1.不要把用户的邮件地址以明文显示出来，这样会被爬虫爬走并被让用户的邮箱被垃圾邮件搞死
 *   2.为用户的链接加上 rel="nofollow" 的属性以 避免垃圾网站的干扰 -- 这样搜索引擎不会再访问这个链接
 *   3.为网站建立一些的限制(安全性的范畴),如不能注册两个以上的邮箱、账号等；限制搜索的时间间隔等。
 *     使用 CAPTCHA 来保证是人为的操作等等
 *   4.如果 POST 成功，要在 POST 方法后重定向网址，这样可以阻止用户通过刷新页面重复提交
 *   5.只要需要，请实现 cache 机制，了解并合理地使用 HTTP caching 以及 HTML5 Manifest.
 *   6.优化页面性能时可以使用 firebug 或是 chrome 内置的开发人员的工具来查看网页装载的性能
 *   7.学习如何 gzip/deflate 网页 (deflate 更好)
 *   8.把多个 CSS 文件和 Javascript 文件合并成一个，这样可以减少浏览器的网络连数，并且使用 gzip 压缩被反复用到的文件
 *   9.繁忙的网络应该考虑把网页的内容分开存放在不同的域名下 -- 如专门的图片服务器、Ajax服务器 等
 *   10.静态网页(图片,CSS,JavaScript 等)应该放在一个不使用 cookies 的独立的域名下
 *   11.为 Javascript 使用 Google Closure Compiler 或是 其它压缩工具，从而减少网络传输
 *   12.确保网站有一个 /favicon.ico 文件放在网站的根下, 因为 浏览器会自动请求这个文件，如没有的话，会产生大量404错误
 *   
 * SEO(搜索引擎优化)
 *   1.做一个 XML sitemap，并放在网站的根下(/sitemap.xml), 可以让搜索引擎了解网站图
 *   2.安装 Google Analytics  (或是别的开源的网站分析工具，如： Piwik)
 *   3.了解 robots.txt 和搜索引擎爬虫是如何工作的
**************************************************************************************************************************************/

public class WebTest {

}
