package com.fishjam.study.javaee;

/**************************************************************************************************************************************
 * 非侵入式设计(Spring) -- 无需继承框架提供的类(但通常需要使用Reflection机制，动态调用的方式来避免硬编码的约束)
 *   Class.forName("xxxx").newInstance();
 * 侵入设计(EJB) -- 需要继承框架类，如果以后想更换框架，之前写过的代码几乎无法重用
 * 
 * AOP(Aspect Oriented Programming) -- 面向切面编程，通过在编译期间、装载期间或运行期间实现在不修改源代码的情况下给程序动态添加功能的一种技术。
 *   即把可重用的功能(如 安全、日志记录、)提取出来，然后将这些通用功能在合适的时候织入到应用程序中
 * Bean -- 一般指容器管理对象，在Spring中指Spring IoC容器管理对象
 * Ioc(Inversion of Control) -- 控制反转(也称作 依赖注入：Dependency Injection，现实例子就是USB接口)。
 *   即由容器控制程序之间的关系，而非传统实现中的程序代码直接控制。即控制权由应用代码中转到了外部容器。
 * JSTL -- JSP Standard Tag Library,JSP标准标签库
 * ORM() -- 
 * POJO(Plain Old Java Objects) -- 简单的Java对象，它可以包含业务逻辑或持久化逻辑，但不担当任何特殊角色且不继承或不实现任何其它Java框架的类或接口
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
 *   4.Spring自身提供了一套非常强大的MVC框架，也可以非常容易的与第三方MVC框架(如 Struts,JF)集成，
 *     org.springframework.web.servlet.DispatcherServlet
 *   5.简化各种技术集成：提供对Java Mail、任务调度、JMX、JMS、JNDI、EJB、动态语言、远程访问、Web Service等的集成
 *   其真正的精华是 Ioc模式 实现的 BeanFactory 和 AOP 
 * Spring通过依赖注入模式，将依赖关系从编码中脱离出来，从而大大降低了组件之间的耦合，实现了组件真正意义上的即插即用。这也是Spring最具价值的特性之一。
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
 applicationContext.xml -- Bean工厂配置文件，ApplicationContext extends BeanFactory,
 <beans>
   <!-- id(唯一标识，可通过 Action myAction=(Action)context.getBean("MyAction")获得实例)；depends-on(设置依赖关系，会决定构建顺序)；-->
   <bean id="MyAction" class="com.fishjam.xxx.MyActionImpl" singleton="false" init-method="init" destroy-method="cleanup" depends-on="ActionManager"> 
     <property name="属性名">   -- Spring读取该属性后，对应到Action中的setXxx()
       <value>属性值</value>
     </property>
     <property name="dataSource">   -- 对应类中有一个名为 DataSource 的属性,引用到id为 dataSource 的另外一个bean
       <ref local="dataSource"/>    -- 指定了属性对BeanFactory中其他Bean的引用关系
     </property>
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
 *     BeanFactory -- 负责根据配置文件创建并维护Bean实例，是Bean的管理容器。从配置中读取类名、属性和值，然后使用Reflection机制进行加载和属性设定。
 *     BeanWrapper -- 实现了针对单个Bean的属性设定操作。
 *                    使用示例: Object obj = Class.forName("net.xiaxin.beans.User").newInstance(); BeanWrapper bw = new BeanWrapperImpl(obj); 
 *                    bw.setPropertyValue("name","value");   等价于: new User().setName("value");
 *    
 *   .Apache
 *     BeanUtils
 *       setProperty(obj,"属性名","属性值");
**************************************************************************************************************************************/

public class SpringStudy {

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