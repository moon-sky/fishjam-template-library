package com.java.test;

import static org.junit.Assert.*;

import org.junit.Test;

/**********************************************************************************************************************************
 * JUnit -- 目前 Java 下单元测试的标准
 * TestNG(Test Next Generation) -- 下一代的测试框架，基于J2SE5.0的注释特性而构建的轻量级的单元测试框架结构。
 *    相对JUnit更灵活。配置文件为 Xxx-suite.xml ?
 * Mock框架 -- 如 JMock, Easymock, PowerMock等
 * 
 * 使用
 *   import static org.junit.Assert.assertEquals;
 *   import org.junit.Test;
 *   
 *   JUnit3 -- 约定所有需要执行测试的方法都以 test 开头
 *   JUnit4 -- 需要执行的测试方法都应该以@Test进行标注
 *   
 *   最佳实践：
 *      1.需要执行测试的方法以 test 开头，并且在恰面都加上 @Test，即: @Test public void testXxxx() {}  
**********************************************************************************************************************************/

/**********************************************************************************************************************************
 * 目录结构
 *    src/test/java -- 存放测试用例源文件
 *    src/test/resources -- 存放配置文件和资源文件，如 TestNG的运行文件
**********************************************************************************************************************************/


public class JavaUnitTest {

	@Test
	public void test() {
		assertEquals(1, 1);
	}

}
