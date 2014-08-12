package com.fishjam.spring.test.framework;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

import javax.inject.Inject;

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

//SPRING in action(第三版中文版).pdf -- P100
/***********************************************************************************************************************************************
 * Spring开发中的最佳实践
 *   1.设置 <beans> 的 default-autowire="byName" 启用 name=>bean.id 的自动装配;
 *   2.? 使用Java提供的 @Inject 注解进行注入(Java提供的通用依赖注入模型)
***********************************************************************************************************************************************/


/***********************************************************************************************************************************************
 * 为了降低Java开发的复杂性，Spring采取了以下关键策略：
 *   1.基于POJO的轻量级和最小侵入性编程 -- 不用继承框架的特定接口或类，能轻松切换框架
 *   2.通过依赖注入(DI)和面向接口实现松耦合；
 *   3.基于切面(AOP)和惯例进行声明式编程
 *     通过 <aop:config><aop:aspect><asp:pointcut> 等进行配置(切面、切入点), <aop:before>(前置通知); <aop:after>(后置通知)，
 *     这样可以在指定的方法调用前后，指定调用特殊的方法(比如日志记录等)，而对原有的方法不产生影响。
 *   4.通过切面和模版减少样板式代码(如使用 JdbcTemplate 封装JDBC数据库操作的代码);
 *    
 *  
************************************************************************************************************************************************/

/***********************************************************************************************************************************************
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
 *  
 * Spring容器提供了两种装配(Wiring)Bean的方式:
 *   XML配置 -- 
 *   注解(Annotation)
 *   
 * 内部Bean -- 定义在其他Bean内部的Bean。
 *   定义方法：声明一个<bean>元素作为<property> 或 <constructor-arg> 元素的子节点。
 *      如：<property name="myInnerBean"><bean class="xxxx"/></property>
***********************************************************************************************************************************************/ 

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations={"classpath:DemoBeans.xml"})
public class FramewrokTest extends AbstractJUnit4SpringContextTests {

	
	
	@Autowired
	private Student student;
	
	@Autowired
	private MyLog	mylog;
	
	/**********************************************************************************************************
	 * Spring通过依赖注入模式，将依赖关系从编码中脱离出来，从而大大降低了组件之间的耦合，实现了组件真正意义上的即插即用。这也是Spring最具价值的特性之一。
	 * 依赖注入(DI)
	 *   依赖注入的几种实现类型：
	 *   1.接口注入 -- 常常借助接口来将调用者与实现者分离，如 Context.lookup(ServletContext.getXXX);
	 *   2.构造器注入 -- 即通过构造函数完成依赖关系的设定
	 *   3.设值注入(使用最广泛) -- 通过配置属性，使用类的setter方法完成依赖关系的设置
	 *   
	 * 自动装配(autowiring) -- Spring自动识别如何装配Bean的依赖关系(减少  <property> 和 <constructor-arg> 等元素)
	 *    四种类型( autowire="xxx", 也可在 <beans> 中通过 default-autowire="xxx" 来设置缺省匹配类型 ): 
	 *       [none] -- 缺省值，表示所有Bean都不是用自动装配
	 *       byName -- 为属性自动装配Id与该属性的名字相同的Bean, 如 name="datasource" 会自动匹配到 id="datasource" 的bean
	 *       byType(不好) -- 类型相同即自动匹配，注意：当有多个匹配的类型时，会抛出异常(NoSuchBeanDefinitionException)，
	 *         可通过设置首选(primary[true])或取消资格(autowire-candidate="false")的方式解决
	 *       constructor(不好) -- 构造函数的参数类型匹配，当有多个匹配的类型时，会抛出异常
	 *       autodetect -- 先用constructor,如失败再byType
	 * 自动检测(autodiscovery) -- 自动检测和定义Bean(减少 <bean> 元素)，默认情况下，查找使用构造型(stereotype, JSR-250)注解所标注的类：
	 *  
	 * 创建应用对象之间协作关系的行为通常被称为装配(wiring)，使用流程为:
	 *    1.声明Bean，使得容器能够加载--
	 *      a. xml中通过 <beans><bean> 的方式显示声明;可通过 <property> 等指定内部Bean(inner bean)，其实例仅使用于当前的Bean，不能被其他的bean共享
	 *        <bean><property name="xxx"> <bean class="内部类的路径" /></property></bean>
	 *         也可以利用自动装配(autowire 或 default-autowire[none] 设置类型) -- id和属性名相同(byName, 典型场景就是整个应用中唯一的 DataSource); 属性类型相同(byType);构造函数参数类型相同(constructor);
	 *      b.使用注解(Annotation)进行配置 (默认禁用，需要通过 <context:annotation-config> 或 <context:component-scan> 启用)
	 *         如换成<context:component-scan base-package="xxx.*">则允许Spring扫描指定包及其子包，找出能自动注册为Bean的类，而且也支持自动装配。
	 *         支持类型：
	 *           1).Spring自带的@Autowired注解 -- 用于setter方法等处，Spring会自动尝试byType的自动匹配,可通过 required=false 设置为可选
	 *           2).Java 提供的@Inject注解(JSR-330)，用于统一各种依赖注入框架的编程模型，其依赖关系必须存在，
	 *              可注入一个Provider接口来实现Bean引用的延迟注入以及注入Bean的多个实例等功能；通过 @Named 注解来标识可选择的Bean
	 *           3).JSR-250的@Resource注解,bean的ID默认为无限定类名，也可以指定
	 *             @Component, @Controller, @Repository, @Service  -- 详情参见SpringMVCStudy.java
	 *             <context:include-filter | exclude-filter> 中的 type + expression 协作定义组件扫描策略,可选type和expression:
	 *             annotation -- 扫描使用指定注解所标注的类，expression属性指定要扫描的注解
	 *             assignable -- 扫描派生于expression所指定类型的类
	 *             aspectj -- 扫描与expression属性所指定的AspectJ表达式所匹配的那些类
	 *             custom -- 使用自定义的 org.springframework.core.type.TypeFilter实现类
	 *             regex -- 扫描类的名称与expression属性所指定的正则表达式所匹配的那些类
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
		
		assertEquals(applicationContext.getBeanDefinitionCount(), 8);
		
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
