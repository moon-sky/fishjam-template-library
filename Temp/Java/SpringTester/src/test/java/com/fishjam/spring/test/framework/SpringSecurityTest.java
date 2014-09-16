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
**********************************************************************************************************/
public class SpringSecurityTest extends TestCase {
	
	@Test
	public void testSecurity(){
		assertEquals(1, 1);
	}
}
