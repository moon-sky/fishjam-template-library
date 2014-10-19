package com.java.test.res;

import static org.junit.Assert.*;

import java.util.PropertyResourceBundle;
import java.util.ResourceBundle;

import org.junit.Test;

/*********************************************************************************************************************
 * 加载 config.properties 文件，然后通过 getString 等方法获取具体的配置信息
 *   ResourceBundle bundle=PropertyResourceBundle.getBundle("config"); 
*********************************************************************************************************************/

public class JavaResourceTest {
	
	@Test
	public void testResourceMgr(){
		assertEquals(1, 1);
	}
}
