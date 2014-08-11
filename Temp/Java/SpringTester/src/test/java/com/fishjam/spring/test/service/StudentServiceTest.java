package com.fishjam.spring.test.service;

import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.AbstractJUnit4SpringContextTests;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import com.fishjam.springtester.service.StudentService;

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations={"file:src/test/resources/DemoBeans.xml"})
public class StudentServiceTest extends AbstractJUnit4SpringContextTests{
	
	@Autowired
	private static StudentService service;
	
	@BeforeClass
	public static void init(){
		//执行整个测试类需要一次性初始化的环境
		//ApplicationContext context = new FileSystemXmlApplicationContext(
		//		"F:\\Fujie\\FJCODE_GOOGLE\\Temp\\Java\\SpringTester\\src\\main\\webapp\\WEB-INF\\SpringTester-servlet.xml");
		//System.out.println(ApplicationContextDumper.ApplicationContextToString(context, BaseInfoDumper.DEFAULT_DIVIDE));
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
