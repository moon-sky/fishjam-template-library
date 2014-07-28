package com.java.tester;

import static org.junit.Assert.*;

import org.junit.Test;

/**********************************************************************************************************************************
 * JUnit -- Java 下单元测试的标准
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
 
public class JavaUnitTester {

	@Test
	public void test() {
		fail("Not yet implemented");
	}

}
