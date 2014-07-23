package com.fishjam.study.javaee;

import org.springframework.context.support.ClassPathXmlApplicationContext;


//SPRING in action(第三版中文版).pdf (Spring实战) -- P122 注解切面
//   http://www.manning.com/walls4/ -- 下载Sample

/**************************************************************************************************************************************
* 常见问题及解决方案
*    1.当拦截"/" 时(REST 风格)，无法访问静态文件
*      解决方案一：激活Tomcat的defaultServlet(必须在最前面)来处理静态文件，<servlet-name>default</servlet-name><url-pattern>*.jpg</url-pattern> 
*      补充：Tomcat, Jetty, JBoss, and GlassFish 自带的默认Servlet的名字 -- "default"
*               Google App Engine 自带的 默认Servlet的名字 -- "_ah_default"
*               Resin 自带的 默认Servlet的名字 -- "resin-file"
*               WebLogic 自带的 默认Servlet的名字  -- "FileServlet"
*               WebSphere  自带的 默认Servlet的名字 -- "SimpleFileServlet"
*      解决方案二：Spring 3.0.4 后映射到ResourceHttpRequestHandler： <mvc:resources mapping="/images/**" location="/images/" />  
*    2. 所有的Bean默认都是单例(singleton)，可通过 scope 属性配置Bean的作用域来更改, 
*        如 prototype(每次调用时生成实例), request(一次HTTP请求), session, global-session  
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * 非侵入式设计(Spring) -- 无需继承框架提供的类(但通常需要使用Reflection机制，动态调用的方式来避免硬编码的约束)
 *   Class.forName("xxxx").newInstance();
 * 侵入设计(EJB) -- 需要继承框架类，如果以后想更换框架，之前写过的代码几乎无法重用
 * 
 * 为了降低Java开发的复杂性，Spring采取了以下关键策略：
 *   1.基于POJO的轻量级和最小侵入性编程 -- 不用继承框架的特定接口或类，能轻松切换框架
 *   2.通过依赖注入(DI)和面向接口实现松耦合；
 *   3.基于切面(AOP)和惯例进行声明式编程
 *     通过 <aop:config><aop:aspect><asp:pointcut> 等进行配置(切面、切入点), <aop:before>(前置通知); <aop:after>(后置通知)，
 *     这样可以在指定的方法调用前后，指定调用特殊的方法(比如日志记录等)，而对原有的方法不产生影响。
 *   4.通过切面和模版减少样板式代码(如使用 JdbcTemplate 封装JDBC数据库操作的代码); 
 *   
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * Spring -- 基于IoC和AOP的构架多层j2ee系统的开发应用框架，用于简化企业级应用程序开发。
 *   1.通过配置文件来创建对象，管理对象之间依赖关系(不用修改源码或重新编译)
 *   2.还提供 通用日志记录、性能统计、安全控制、异常处理等“面向切面”的能力
 *   3.管理 数据库事务，供了一套简单的JDBC访问实现，对不同的数据访问技术提供了统一的接口,提供DAO（数据访问对象）支持。
 *     也可与第三方数据访问框架(如Hibernate)集成(如 提供的 HibernateDaoSupport ?)。
 *     可通过Spring实现基于容器的事务管理，其本质上是基于动态AOP。
 *       <bean id="transactionManager" class="org.springframework.jdbc.datasource.DataSourceTransactionManager"> ...
 *       <bean id="userDAOProxy" class="org.springframework.transaction.interceptor.TransactionProxyFactoryBean"> ...
 *     JDBC模版库(spring-dao.jar) -- 
 *       JdbcTemplate -- 只需简单的编写业务相关的SQL，连接、打开、关闭数据库，异常处理等都由模版类处理
 *         JdbcTemplate jdbcTemplate = new JdbcTemplate(dataSource);
 *         jdbcTemplate.update("UPDATE user SET age = 10 WHERE id = 'erica'");
 *       TransactionTemplate + TransactionCallback -- -- 事务模版，手动控制原子事务
 *       回调(Callback)机制，具有极强的灵活性和扩展性。:
 *         在执行Update等时，可通过 PreparedStatementSetter 避免SQL注入漏洞；
 *         在执行Select等时，可通过 RowCallbackHandler 来获取查询结果
 *   4.Spring自身提供了一套非常强大的MVC框架(SpringMVC)，也可以非常容易的与第三方MVC框架(如 Struts,JF)集成，
 *     org.springframework.web.servlet.DispatcherServlet
 *   5.简化各种技术集成：提供对Java Mail、任务调度、JMX、JMS、JNDI、EJB、动态语言、远程访问、Web Service等的集成
 *   其真正的精华是 Ioc模式 实现的 BeanFactory 和 AOP 
 * Spring通过依赖注入模式，将依赖关系从编码中脱离出来，从而大大降低了组件之间的耦合，实现了组件真正意义上的即插即用。这也是Spring最具价值的特性之一。
 *    创建应用对象之间协作关系的行为通常被称为装配(wiring)，使用流程为:
 *    1.声明Bean，使得容器能够加载--
 *      a. xml中通过 <beans><bean> 的方式显示声明;可通过 <property> 等指定， 
 *        内部Bean(inner bean)，其实例仅使用于当前的Bean，不能被其他的bean共享
 *        <bean><property name="xxx"> <bean class="内部类的路径" /></property></bean>
 *         也可以利用自动装配(autowire 或 default-autowire[none] 设置类型) -- id和属性名相同(byName, 典型场景就是整个应用中唯一的 DataSource); 属性类型相同(byType);构造函数参数类型相同(constructor);
 *      b.使用注解(Annotation)进行配置 (默认禁用，需要通过 <context:annotation-config> 启用)
 *         如换成<context:component-scan base-package="xxx">则允许Spring扫描指定包及其子包，找出能自动注册为Bean的类。
 *         自动检测依据：@Component, @Controller, @Repository, @Service  -- 详情参见SpringMVCStudy.java
 *
 * 依赖注入的几种实现类型：
 *   1.接口注入 -- 常常借助接口来将调用者与实现者分离，如 Context.lookup(ServletContext.getXXX);
 *   2.构造子注入 -- 即通过构造函数完成依赖关系的设定，
 *   3.设值注入(使用最广泛) -- 通过配置属性，使用类的setter方法完成依赖关系的设置
 * 
 * 缺点：
 *   1.使用人数不多、jsp中要写很多代码、控制器过于灵活，缺少一个公用控制器
 * 
 * 输入验证 -- org.springframework.validation.Validator 接口，两个方法：supports, validate
 * 数据绑定 -- <spring:bind>
 * 统一的异常处理 -- <bean id="exceptionResolver" class="org.springframework.web.servlet.handler.SimpleMappingExceptionResolver">
 *   可通过 exceptionMappings 的属性配置将不同的异常映射到不同的jsp页面，
 *   可通过 defaultErrorView 的属性配置指定一个默认的异常提示页面
 **************************************************************************************************************************************/

/**************************************************************************************************************************************
 * Spring3 中使用的Annotation -- 注解驱动(配置中先启用 <context:annotation-config /> ) -- 括号中是在 xml 中的等价配置方式
 *   @Autowired -- 允许Spring自动装配，要求应用中只能有一个适合装配到注解所标注的属性或参数中，否则会抛出NoSuchBeanDefinitionException异常
 *   @Inject -- JSR-330 中定义的 Java依赖注入规范，
 *   @PostConstruct(init-method) -- 定义容器初始化bean之后进行的操作，一般放在自定义的 init() 方法前
 *   @PreDestroy(destory-method) -- 定义销毁bean之前进行的操作，一般放在自定义的 destroy() 方法前
 *   @Value(SpEL) -- 设置属性值，可以设置Spring表达式格式的值。如对 DataSource 的URL指定 "#{systemProperties.myDataSource"}
 *   允许<context:component-scan>自动检测为Bean的标注类型
 *   @Component -- 
 **************************************************************************************************************************************/

/**************************************************************************************************************************************
 applicationContext.xml -- Bean工厂配置文件，Spring容器根据这些信息创建和管理Bean，
   ApplicationContext extends BeanFactory,
 <beans>
   <!-- id(唯一标识，可通过 Action myAction=(Action)context.getBean("MyAction")获得实例)；depends-on(设置依赖关系，会决定构建顺序)；-->
   <bean id="MyAction" class="com.fishjam.xxx.MyActionImpl" singleton="false" init-method="init" destroy-method="cleanup" 
         depends-on="ActionManager" factory-method="class中的静态工厂方法(通常用于装备单例Bean)">
     <constructor-arg value="数值类型的值1" />  《== 设置构造函数的参数值
     <constructor-arg ref="引用类型(如其他的Bean)的值2" /> 
     <property name="属性名" value="xxx"></property>   -- Spring读取该属性后，对应到Action中的setXxx()，可通过 <null/> 语法给属性值赋空值
     <property name="dataSource" ref="dataSource">   -- 对应类中有一个名为 DataSource 的属性,引用到id为 dataSource 的另外一个bean
       <ref local="dataSource"/>   《== 指定了属性对BeanFactory中其他Bean的引用关系 , 
     </property>  《== 引用了 p 名称空间后( xmlns:p="http://www.springframework.org/schema/p" )等价于 -- p:dataSource-ref="dataSource"
	<property name="roles">  《装配 集合类型的Bean属性(list, set, map, props 等), 对应Java中的 Collection 子类等类型的成员变量
		<list>
			<ref bean="admin" />
			<ref bean="normal" />
		</list>
	</property>   《== <map><entry key="xxx" value-ref="XXX' /></map> 设置映射类型的属性(key/key-ref/value/value-ref) 等, <props><prop> 只支持字符串映射
   </bean>
   <bean id="dataSource" class="org.springframework.jndi.JndiObjectFactoryBean">
     <property name="xxxx"><value>yyyy</value></property>
   </bean>
 </beans>

 web.xml  --  Spring提供了可配置的ApplicationContext加载机制，可选加载器：ContextLoaderListener(新2.3以后)， ContextLoaderServlet(旧).
   设置好以后，Web容器会自动加载 /WEB-INF/applicationContext.xml 初始化ApplicationContext实例，代码中可通过 WebApplicationContextUtils.getWebApplicationContext 获取实例
     <listener><listener-class>org.springframework.web.context.ContextLoaderListener</listener-class></listener>
 或  <servlet>
       <servlet-name>context</servlet-name>
       <servlet-class>org.springframework.web.context.ContextLoaderServlet</servlet-class>
       <load-on-startup>1</load-on-startup>
     </servlet>


 TODO: bean.xml? 位于工作路径下(默认为项目根路径-- .project文件所在目录)，配置文件需要用 FileSystemXmlApplicationContext、XmlBeanFactory 等 加载?
***************************************************************************************************************************************/

/**************************************************************************************************************************************
 * Spring表达式语言(Spring Expression Language -- SpEL) : 通过运行期执行的表达式将值装配到Bean的属性或构造器参数中。
 *    注意：虽然功能很强大， 但是没有IDE的语法检查支持，很难调试和维护，因此不要把过多的逻辑放入SpEL表达式中
 *    语法：value = "#{表达式}"， 
 *       #{myBean.property1}  <== 获取属性值 
 *       #{myBean.getObj()?.fun()}<== 调用getObj()方法获得返回的对象，如不为null，则通过其fun()方法获得返回值
 *       T() <== 调用类作用域的方法和常量，通常可用于调用静态方法和常量， #{T(java.lang.Math).PI * circle.radius ^ 2 } 计算园的面积
 *       运算符的文本替代方式(textual alternatives): ==(eq), <(lt), <=(le), >(gt), >=(ge)
 *       逻辑表达式<== and / or / not(或 !)
 *       条件表达式<==  条件 ? 真时的值 : 假时的值， 常见的使用场景为检测null， 简化形式为 #{ 变量 ?: "默认值" }   
 *       正则表达式匹配 <== <property name="validEmail" value="#{admin.email matches '[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.com'}" /> 
 *       操作集合 <== 获取一个成员: #{集合名[index]};  或 #{集合名['keyName']}。
 *          特殊：#{systemEnvironment['HOME']} -- 访问名为 HOME 的环境变量;  
 *                   #{systemProperties['application.home']} -- Java启动时的属性(-D参数)
 *          查询集合成员： 
 *             .?[条件] -- 查询满足条件的所有元素，如 <property name="bigCities" value="#{cities.?[population gt  1000000]}" /> -- 选择所有人口数大于100万的城市
 *             .^[条件] -- 查询第一个匹配项;  
 *             .$[条件] -- 查询最后一个匹配项； 
 *             .![属性] -- 集合投影(从集合的每一个成员中选择特定的属性放入新的集合中)，如 <property name="cityNames" value="#{cities.![name]}" /> -- 获取所有城市的名字并放入新的集合中 
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * Spring.jar -- Spring 框架需要的包
 * log4j.properties -- Apache log4j作为日志输出组件,配置文件
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * 核心包和类
 *   .Spring
 *     ApplicationContext -- 从BeanFactory继承，并提供了更为框架化的实现：
 *        1.可在web.xml中对ApplicationContext进行配置;
 *        2.国际化支持; -- <bean id="messageSource" class="org.springframework.context.support.ReloadableResourceBundleMessageSource" xxx>
 *            配置好后，Spring会在指定目录下依次查找 _zh_CN, _zh 等后缀的 .properties 和 .class 文件，使用找到的第一个满足要求的资源文件；
 *            代码中可用 ctx.getMessage("key" [,arg, Locale.zh_CN]) 的方式加载文本; 
 *            JSP页面中通过 <spring:message code="key"/> 的方式使用。
 *            TODO:需要使用 native2ascii.exe 对资源文件进行转码(转换成 \uFFFF 的格式)，否则取出来的会是乱码？能否直接将文件保存成Unicode格式？
 *                 实际工作中可在Ant中设置Native2Ascii任务：<native2ascii encoding="GBK" src="${src}" dest="${build}"/>
 *            JVM启动时可通过 "-Duser.language=en" 来设定当前JVM语言类型
 *        3.支持对文件和URL的访问; -- ctx.getResource 加载资源文件(如 "classpath:config.properties")
 *        4.事件传播 -- 基于Observer模式提供了针对Bean的事件传播功能。为系统中状态改变时的检测提供了良好支持。
 *            可通过 publishEvent方法，将事件通知系统内所有的ApplicationListener。典型应用是异常处理
 *        5.可以在同一个应用中加载多个Context实例
 *        有多种子类(分别通过不同的方式读取xml配置信息): 
 *           ClassPathXmlApplicationContext -- 从应用的类路径(包含JAR文件)下加载
 *           FileSystemXmlApplicationContext -- 指定文件系统的路径进行加载
 *           XmlWebApplicationContext -- 读取Web应用下的XML配置文件进行加载
 *     BeanFactory -- 负责根据配置文件创建并维护Bean实例，是Bean的管理容器。从配置中读取类名、属性和值，然后使用Reflection机制进行加载和属性设定。
 *     BeanWrapper -- 实现了针对单个Bean的属性设定操作。
 *                    使用示例: Object obj = Class.forName("net.xiaxin.beans.User").newInstance(); BeanWrapper bw = new BeanWrapperImpl(obj); 
 *                    bw.setPropertyValue("name","value");   等价于: new User().setName("value");
 *    
 *   .Apache
 *     BeanUtils
 *       setProperty(obj,"属性名","属性值");
 *       
 * 测试
 *    Spring 提供了测试模块来测试Spring应用：
 *       为JNDI、Servlet等编写单元测试提供了一系列的模拟对象实现；
 *       对于继承测试，为加载Spring应用上下文中的Bean的集合及交互提供了支持。
**************************************************************************************************************************************/

/**************************************************************************************************************************************       
 * Spring Portfolio(http://spring.io/projects) -- 包括多个构建于核心Spring框架之上的框架和类库，几乎为每一个领域的Java开发都提供了Spring编程模型
 *   Spring Web Flow -- 建立与Spring MVC框架之上，并为基于流程的会话式Web应用(购物车、向导)提供支持。
 *   Spring Web Service -- 提供了契约优先的Web Service 模型
 *   Spring Security -- 为应用提供了声明式的安全机制
 *   Spring Integration -- 提供了几种通用的应用集成模式的Spring声明式风格的实现
 *   Spring Batch -- 对数据进行大量操作时，使用的批处理方式
 *   Spring Social --  社交网络扩展模块
 *   Spring Mobile/Android -- 移动Web应用开发
 *   Spring Dynamic Module -- 整合了Spring声明式依赖注入和OSGi的动态组件模型，可以采用模块化的方式构建应用
 *   Spring Rich Client -- 富应用工具箱，开发桌面应用程序
 *   Spring .NET -- 面向.NET平台的，提供了松耦合和面向切面的Spring特性。
**************************************************************************************************************************************/
public class SpringStudy {
	
	public void testSpring(){
		 ApplicationContext context = new ClassPathXmlApplicationContext("applicationContext_demo.xml"); //TODO: 路径
		 SpringStudy myBean = (MyBean)context.getBean("myBean");// SpringStudy.class);
	}
}

//典型的异常显示页面：
/**************************************************************************
<html>
<head><title>Exception!</title></head>
<body>
	<% Exception ex = (Exception)request.getAttribute("Exception"); %>
	<H2>Exception: <% ex.getMessage();%></H2>
	<P/>
	<% ex.printStackTrace(new java.io.PrintWriter(out)); %>
</body>
</html>
***************************************************************************/