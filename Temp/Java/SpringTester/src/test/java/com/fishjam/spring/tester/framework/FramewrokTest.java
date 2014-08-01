package com.fishjam.spring.tester.framework;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import org.springframework.context.support.FileSystemXmlApplicationContext;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.AbstractJUnit4SpringContextTests;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.web.context.support.XmlWebApplicationContext;

import com.fishjam.springtester.domain.Student;
import com.fishjam.springtester.tools.MyLog;
import com.fishjam.utility.web.spring.ApplicationContextDumper;

import junit.framework.TestCase;

/*********************************************************************************************************
 * 为了降低Java开发的复杂性，Spring采取了以下关键策略：
 *   1.基于POJO的轻量级和最小侵入性编程 -- 不用继承框架的特定接口或类，能轻松切换框架
 *   2.通过依赖注入(DI)和面向接口实现松耦合；
 *   3.基于切面(AOP)和惯例进行声明式编程
 *     通过 <aop:config><aop:aspect><asp:pointcut> 等进行配置(切面、切入点), <aop:before>(前置通知); <aop:after>(后置通知)，
 *     这样可以在指定的方法调用前后，指定调用特殊的方法(比如日志记录等)，而对原有的方法不产生影响。
 *   4.通过切面和模版减少样板式代码(如使用 JdbcTemplate 封装JDBC数据库操作的代码);
 *    
 *  
**********************************************************************************************************/

/*********************************************************************************************************
 * 容器是Spring框架的核心 -- 使用依赖注入管理构成应用的组件，会创建相互协作的组件之间的关联。
 * 
 * Spring 自带的容器实现：
 *   BeanFactory -- 最简单的容器，提供基本的DI支持
 *   ApplicationContext -- 继承于BeanFactory，并提供面向应用的服务，如从属性文件解析文本信息,
 *      发布应用事件给感兴趣的事件监听者。
 *   
 *  ClassPathXmlApplicationContext -- 加载位于应用系统 classpat 下的一个或多个XML文件
 *     TODO:怎么知道应用的类路径是什么？好写相对路径来加载
 *  FileSystemXmlApplicationContext -- 读取文件系统下的XML配置文件并加载上下文定义
 *  XmlWebApplicationContext -- 读取Web应用下的XMl配置文件并装载上下文定义
*********************************************************************************************************/ 

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations={"classpath:DemoBeans.xml"})
public class FramewrokTest extends AbstractJUnit4SpringContextTests {

	
	
	@Autowired
	private Student student;
	
	@Autowired
	private MyLog	mylog;
	
	/**********************************************************************************************************
	 * 依赖注入(DI)
	 *   依赖注入的几种实现类型：
	 *   1.接口注入 -- 常常借助接口来将调用者与实现者分离，如 Context.lookup(ServletContext.getXXX);
	 *   2.构造器注入 -- 即通过构造函数完成依赖关系的设定
	 *   3.设值注入(使用最广泛) -- 通过配置属性，使用类的setter方法完成依赖关系的设置
	**********************************************************************************************************/
	//测试手工加载Bean的配置XML文件，并获取其中的bean信息
	@Test
	public void testLoadBeansFromXmlManual(){
		ClassPathXmlApplicationContext xml;

		//加载放在类路径下面(也即编译后会进入到classes目录下)的Bean定义文件(src/test/resources/DemoBeans.xml)
		//下面的两种方式是等价的:
		ApplicationContext applicationContext = new FileSystemXmlApplicationContext("src/test/resources/DemoBeans.xml");
		//ApplicationContext applicationContext = new ClassPathXmlApplicationContext("DemoBeans.xml");
		System.out.println(ApplicationContextDumper.ApplicationContextToString(applicationContext, ApplicationContextDumper.DEFAULT_DIVIDE));
		
		assertEquals(applicationContext.getBeanDefinitionCount(), 6);
		
		Student studentBean = (Student)applicationContext.getBean("student");

		assertNotNull(studentBean);
		assertEquals(studentBean.getId(), 999);
		assertEquals(studentBean.getName(), "fishjam");
		assertEquals(studentBean.getSex(), Student.SEX_MALE);
	}

	/**********************************************************************************************************
	 * AOP(切面编程) -- 确保POJO保持简单
	 *   通常用于将系统服务(如日志、安全控制、事务管理等)模块化，并从业务对象中分离出来，以声明的方式应用进去
	 *
	 * 示例：
	 *    在 Student.DoAction 的前后自动调用 MyLog 对应的方法记录日志，在Console中会看到 System.out 打印出的日志
	 *    TODO:  怎么给 logBeforeAction 传递参数（如 方法名)
	 *    
	 * 使用方法（参见 DemoBeans.xml）-- 
	 *    1.将对应的服务类 (MyLog) 声明为一个Bean;
	 *    2.在 <aop:aspect> 中通过 ref 引用该Bean;
	 *    3.通过 <aop:pointcut> 定义切入点和表达式(expression)属性来选择所应用的通知，其语法为 AspectJ 的切点表达式语言
	 *    4.声明需要调用的切面方法和时机 <aop:before>, <aop:after>, <aop:异常> 等
	 *    
	 * 常见错误
	 *    1.NoClassDefFoundError: org/aspectj/weaver/reflect/ReflectionWorld$ReflectionWorld
	 *      使用了AOP 和 AspectJ 语法，但没有加入依赖包 <org.aspectj><aspectjweaver>
	 ***********************************************************************************************************/
	@Test
	public void testAOP(){
		student.DoAction();
	}
}
