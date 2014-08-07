package com.fishjam.spring.tester.framework;

import org.junit.Test;

import junit.framework.TestCase;
import static org.junit.Assert.*;

/*********************************************************************************************************
 * Spring Security -- 用来保护Spring应用程序，实现安全性
 *  
**********************************************************************************************************/
public class SpringSecurityTest extends TestCase {
	
	@Test
	public void testSecurity(){
		assertEquals(1, 1);
	}
}
