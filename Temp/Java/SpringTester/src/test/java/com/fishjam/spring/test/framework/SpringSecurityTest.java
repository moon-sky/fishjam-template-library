package com.fishjam.spring.test.framework;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;

import org.junit.Test;
import org.springframework.web.bind.annotation.RequestMapping;

import junit.framework.TestCase;
import static org.junit.Assert.*;

/*********************************************************************************************************
 * 常见错误
 *   IE报验证码错误(IE10)
 *      现象：如果在 @RequestMapping 中直接使用 HttpSession session，并且想通过 session.getAttribute("vcode") 获取之前设置的
 *               验证码，有可能会得到null; 造成验证码验证错误。
 *      原因：未知
 *      更改：通过 HttpServletRequest 的 request.getSession(); 获取到 session, 然后就可以获取之前设置的验证码了
**********************************************************************************************************/

/**********************************************************************************************************
 * Spring Security -- 用来保护Spring应用程序，实现安全性
 * 
 * OWASP(Open Web Application Security Project, 开放Web应用安全项目) -- 涵盖了几乎所有关于 Web 站点安全的东西
 *   https://www.owasp.org/index.php/Category:OWASP_Guide_Project
 *   美国联邦贸易委员会(FTC)强烈建议所有企业需遵循 OWASP 十大 WEB 弱点防护守则:
 *   1.了解什么是 SQL 注入攻击 并知道怎么阻止这种攻击 -- http://en.wikipedia.org/wiki/SQL_injection
 *   2.永远不要相信用户的输入(包括 Cookies，因为那也是用户的输入)
 *   3.对用户的口令进行 Hash，并使用 salt(随机数)，以防止 Rainbow(与预先计算的hash值进行比较) 攻击
 *   4.不要试图自己去发明或创造一个自己的 fancy 的认证系统，可能会忽略到一些不容易让你查觉的东西而导致你的站点被 hack
 *   5.了解 处理信用卡的一些规则
 *   6.使用 SSL/HTTPS 来加密传输登录页面或是任可有敏感信息的页面，比如信用卡号等
 *   7.知道如何对付 session 劫持( Session Hijacking )
 *   8.避免 跨站脚本攻击(XSS)
 *   9.避免 跨站伪造请求攻击 cross site request forgeries (XSRF)
 *   10.确保你的数据库连接是安全的
**********************************************************************************************************/
public class SpringSecurityTest extends TestCase {
	
	@Test
	public void testSecurity(){
		assertEquals(1, 1);
	}
}
