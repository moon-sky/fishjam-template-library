package com.fishjam.springtester.service.test;

import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;
import org.springframework.test.context.junit4.AbstractJUnit4SpringContextTests;

import com.fishjam.springtester.service.StudentService;

//@RunWith(SpringJUnit4ClassRunner.class)
//@ContextConfiguration({"classpath:SpringTester-servlet.xml"})
public class StudentServiceTest extends AbstractJUnit4SpringContextTests{
	
	private static StudentService service;
	@BeforeClass
	public static void init(){
		//执行整个测试类需要一次性初始化的环境
		//ApplicationContext context = new ClassPathXmlApplicationContext("config/Spring-db-old.xml"); 
		//service = (StudentService)context.getBean("studentService");
	}
	
	@AfterClass
	public static void fina(){
		
	}
	
	@Test
	public void testStudent(){
		//assetEqual(1)
	}
	
	@Test
	public void testStudent2(){
		
	}
}
