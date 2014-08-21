package com.fishjam.spring.test.framework;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

import javax.naming.InitialContext;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import org.springframework.context.support.FileSystemXmlApplicationContext;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.AbstractJUnit4SpringContextTests;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import com.fishjam.springtester.domain.Student;
import com.fishjam.springtester.tools.MyLog;
import com.fishjam.utility.web.spring.ApplicationContextDumper;

//SPRING in action(第三版中文版).pdf -- P195
//Spring 的下载地址 -- http://www.springsource.org/download 
	
/***********************************************************************************************************************************************
 * Spring开发中的最佳实践
 *   1.设置 <beans> 的 default-autowire="byName" 启用 name=>bean.id 的自动装配;
 *   2.? 使用Java提供的 @Inject 注解进行注入(Java提供的通用依赖注入模型， 可用于变量、构造函数等地方)
 *   3.通过接口进行交互，如 服务对象 -> (DAO接口 <== DAO实现)
***********************************************************************************************************************************************/

/***********************************************************************************************************************************************
 * SpringMVC 优点
 *   1.使用简单，学习成本低 -- 学习难度小于Struts2
 *   2.很容易就可以写出性能优秀的程序 -- Struts2要处处小心才可以写出性能优秀的程序
 *   3.非常灵活
 *   4. SpringMVC 3.0 后更加完善，实现了对 Struts 2 的超越
 *
 * 为了降低Java开发的复杂性，Spring采取了以下关键策略：
 *   1.基于POJO的轻量级和最小侵入性编程 -- 不用继承框架的特定接口或类，能轻松切换框架
 *   2.通过依赖注入(DI)和面向接口实现松耦合；
 *   3.基于切面(AOP)和惯例进行声明式编程
 *     通过 <aop:config><aop:aspect><asp:pointcut> 等进行配置(切面、切入点), <aop:before>(前置通知); <aop:after>(后置通知)，
 *     这样可以在指定的方法调用前后，指定调用特殊的方法(比如日志记录等)，而对原有的方法不产生影响。
 *   4.通过切面和模版减少样板式代码(如使用 JdbcTemplate 封装JDBC数据库操作的代码);
 *    
 *  
***********************************************************************************************************************************************/

/***********************************************************************************************************************************************
* 常见类和接口(org.springframework.web.servlet.*)
*     Controller -- 控制器(Action)，因为添加了 "@Controller" 注解表示该类可以担任控制器的职责，所以一般不直接使用该接口
*     DispatcherServlet -- 前置控制器，在web.xml 的 <servlet> 部分配置，拦截匹配的请求：接收Http请求，并转发给HandlerMapping进行处理
*     HandlerAdapter -- 处理请求映射的接口
*     HandlerExceptionResolver -- 异常处理的接口
*     HandlerInterceptor  -- 拦截器接口，用户可以实现该接口来完成拦截器的工作， preHandle -> postHandle -> afterCompletion
*     HandlerMapping -- 处理请求映射的接口，将各个请求映射到Controller进行处理
*     LocalResolver --
*     ModelAndView --  
*     ViewResolver -- View解析的接口，有 UrlBasedViewResolver、InternalResourceViewResolver 等实现类，生成http响应
* 流程和关系(具体关系参见 SpringMVC.png)
*     DispatcherServlet -> 
***********************************************************************************************************************************************/

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

/*********************************************************************************************************************************************
 * 常用的注解 -- 配置中需要通过 <context:component-scan 配置参数/> 启用Bean的自动扫描功能，否则不能扫描类上的注解
 *    通过配置注解，Spring会自动创建相应的 BeanDefinition 对象，并注册到 ApplicationContext 中，这些类就成了Spring受管组件，
 *    配置参数： <include-filter> 或 <exclude-filter> + type(过滤器类型) + expression(表达式)
 *       如 : <include-filter type="assignable" expression="com.myInterface"> -- 自动注册所有 myInterface 的实现类 
 *       过滤器类型：
 *          annotation(指定注解所标注的类);
 *          assignable(派生于指定类型的类);
 *          aspectj(指定AspectJ表达式匹配的类);
 *          custom(自定义的 TypeFilter 实现类);
 *          regex(正则表达式匹配的类);
 *    @Autowired 用于注入，(srping提供的) 默认按类型装配，要求必须存在，若允许null值，可以设置其required属性为false
 *       若想按名称装配，则可以结合 @Qualifier 注解一起使用
 *    @Component -- 泛指组件(Bean), 当不好归类时使用
 *    @Controller -- 声明Action组件，通常用在控制层
 *    @Configuration -- 声明Spring的配置类，等价于XML配置中的 <beans> 元素，然后可在其中使用 @Bean 声明返回Bean对象的函数(函数名即为其id)
 *    @CookieValue -
 *    @PersistenceContext -- ? 修饰 EntityManger 类型的变量 ,表示注入该类型的实例 
 *    @PostConstruct -- 
 *    @PreDestroy -- 
 *    @Qualifier("beanName") -- 优先配置指定名字的Bean，也可以用来创建自己的限定器注解
 *    @Repository -- 数据仓库，将数据访问层 (DAO层)的类标识为 Spring Bean，然后就不再需要在XML中通过<bean/>显示配置。
 *       可自动提供一些数据访问相关的功能。如 数据访问异常类型(需要添加 PersistenceExceptionTranslationPostProcessor 类型的Bean).
 *    @RequestHeader -- 从请求中获取值
 *    @RequestMapping( {"/", "/home", "/login/login.do"} ) -- 请求映射, 用在方法前, 其参数是一个数组，当自由一个URL时，可简化
 *    @RequestParam(参数) 类型 变量名 -- 在 @RequestMapping 中的参数
 *    @Resource -- 用于命名资源依赖注入，( j2ee提供的 ) 默认按名称装配，对比：@Autowired默认按类型.
 *       例：@Resource(name="beanName")，若未提供name，则先取字段的名称作为bean名称寻找对象，若找不到，则使用类型装配方式 
 *    @ResponseBody -- 
 *    @Scope("prototype") -- 设定bean的作用域 
 *    @Service -- 声明Service组件，通常用在业务层
 *    @Transactional( rollbackFor={Exception.class}) -- 事务管理
 *  TODO(可能支持的，未确定)
 *    @NumberFormatannotation
 *    @DateTimeFormat
 *    @Valid
*********************************************************************************************************************************************/

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations={"classpath:DemoBeans.xml"})
public class FrameworkTest extends AbstractJUnit4SpringContextTests {

	
	
	@Autowired
	private Student student;
	
	@Autowired
	private MyLog	mylog;
	
	/*******************************************************************************************************************************************
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
	 *           3).JSR-250 -- Spring 基于Java的配置
	 *             @Resource注解, bean的ID默认为无限定类名，也可以指定
	 *             @Component, @Controller, @Repository, @Service  -- 详情参见SpringMVCStudy.java
	 *             <context:include-filter | exclude-filter> 中的 type + expression 协作定义组件扫描策略,可选type和expression:
	 *             annotation -- 扫描使用指定注解所标注的类，expression属性指定要扫描的注解
	 *             assignable -- 扫描派生于expression所指定类型的类
	 *             aspectj -- 扫描与expression属性所指定的AspectJ表达式所匹配的那些类
	 *             custom -- 使用自定义的 org.springframework.core.type.TypeFilter实现类
	 *             regex -- 扫描类的名称与expression属性所指定的正则表达式所匹配的那些类
	 *             @Configuration -- 定义配置类，等价于XML中的<beans>配置，但仍然需要先在XML中通过 <context:component-scan>启用
	*********************************************************************************************************************************************/
	
	
	//测试手工加载Bean的配置XML文件，并获取其中的bean信息
	@Test
	public void testLoadBeansFromXmlManual(){
	
		ClassPathXmlApplicationContext xml;

		//加载放在类路径下面(也即编译后会进入到classes目录下)的Bean定义文件(src/test/resources/DemoBeans.xml)
		//下面的两种方式是等价的:
		ApplicationContext applicationContext = new FileSystemXmlApplicationContext("src/test/resources/DemoBeans.xml");
		//ApplicationContext applicationContext = new ClassPathXmlApplicationContext("DemoBeans.xml");
		System.out.println(ApplicationContextDumper.ApplicationContextToString(applicationContext, ApplicationContextDumper.DEFAULT_DIVIDE));
		
		assertEquals(applicationContext.getBeanDefinitionCount(), 20);
		Student studentBean = (Student)applicationContext.getBean("student");

		assertNotNull(studentBean);
		assertEquals(studentBean.getId(), 999);
		assertEquals(studentBean.getName(), "fishjam");
		assertEquals(studentBean.getSex(), Student.SEX_MALE);
	}
}
