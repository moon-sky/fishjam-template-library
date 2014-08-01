package com.fishjam.spring.tester.framework;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.FileSystemXmlApplicationContext;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.AbstractJUnit4SpringContextTests;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import com.fishjam.springtester.domain.Student;
import com.fishjam.utility.web.spring.ApplicationContextDumper;

/*********************************************************************************************************
 * 使用 spring-test 框架进行单元测试 
 *    步骤：
 *      1.引入 spring-test 包依赖;
 *      2.从AbstractJUnit4SpringContextTests继承子类;
 *      3.@RunWith + @ContextConfiguration 声明;
 *      4.通过 @Autowired 等指定需要注入的变量，会自动加载xml配置并进行依赖注入
 *   TODO:
 *      通过注解声明的Bean如何测试?
 * 
 *
 **********************************************************************************************************/

@RunWith(SpringJUnit4ClassRunner.class)
//@ContextConfiguration(locations={"classpath:DemoBeans.xml"})
@ContextConfiguration(locations={"file:src/test/resources/DemoBeans.xml"})
public class SpringTestTest extends AbstractJUnit4SpringContextTests{

	//注意：不需要设置对应的 setter 也可以，会自动使用spring-test提供的依赖注入功能
	@Autowired
	private Student student;

	//测试使用 spring-test 框架自动加载 classpath:DemoBeans.xml 配置文件
	@Test
	public void testLoadBeansFromXmlBySpringTest(){
		assertNotNull(student);
		assertEquals(student.getId(), 999);
		assertEquals(student.getName(), "fishjam");
		assertEquals(student.getSex(), Student.SEX_MALE);
	}
	
	
	
}
