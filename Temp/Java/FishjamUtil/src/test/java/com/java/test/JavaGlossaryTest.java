package com.java.test;


/**********************************************************************************************************************************************
 * ACID(Atomic, Consistent, Isolated, Durable) -- 事务的4个特性，即原子性、一致性、隔离性(锁定表或行)、持久性
 * AOP(Aspect-Oriented Programming) -- 面向切面编程(详情参见 AopStudy.java )
 * Bean -- 一般指容器管理对象，在Spring中指Spring IoC容器管理对象
 * BMP(Bean-Managed Persistence) -- Bean管理持久化
 * BO(Business Object) -- 业务对象，可以理解为
 * CMP(Container-Managed Persistence) -- 容器管理持久化
 * Convention Over Configuration -- 约定优于配置，
 * CORBA(Common Object Request Broker Architecture) -- 一种和语言无关的分布式"对象交换标准框架"，
 * CRUD -- Create + Retrieve + Update + Delete
 * CVS(Concurrent Versions System) -- 版本控制工具CVS，已被SVN替代
 * DAO(Data Access Object) -- 数据访问对象
 * DBCP(Database Connection Pooling) -- 数据库连接池，是Apache的 Jakarta Commons 项目  
 * DI(Dependency Injection) -- 依赖注入，对象的依赖关系将由负责协调系统中各个对象的第三方组件在创建对象时设定，
 *    对象无需自行创建或管理它们的依赖关系 -- 依赖关系将被自动注入(通过 XML配置、注解、)到需要它们的对象中去。现实例子就是USB接口。
 *    依赖注入让相互协作的软件组件保持松散耦合。
 * DMI(Dynamic Method Invocation) -- 动态方法调用， struts2中指定action不同的请求为 "ActionName!methodName" 
 * DTO(Data Transfer Object) -- 数据传输对象，JavaEE中，中间层组件将应用底层的状态信息封装成名为DTO的JavaBean，并传送到JSP页面进行显示。
 * EJB(Enterprise Java Bean) -- (将事务逻辑和连接的问题隔离),已被 JPA 替代？
 * GC(Garbage Collector) -- 垃圾回收
 * Ioc(Inversion of Control) -- 控制反转(也称作 依赖注入：Dependency Injection。
 *   即由容器控制程序之间的关系，而非传统实现中的程序代码直接控制。即控制权由应用代码中转到了外部容器。
 * JAAS(Java Authentication Authorization Service) -- Java验证和授权服务，用于控制对 Java Web 应用的授权访问，
 *   在 web.xml 中的 <security-constraint>， <login-config> 等处体现
 * JAF(JavaBeans Activation Framework) -- 
 * JCP(Java Community Process) --  
 * JDK(Java Development Kit) -- 按Java的应用环境划分
 *   J2EE(Java 2 Platform Enterprise Edition) -- 企业Web应用开发
 *   J2SE(Java 2 Platform Stand Edition) -- 普通应用程序
 *   J2ME(Java 2 Platform Micro Edition) -- 手机等消费电子产品
 * JDO(Java Data Object) -- Java数据对象，是Java EE标准中除JPA外，另一个支持管理持久化数据的规范。 
 * JINI(Java Intelligent Network Infrastructure) -- 能轻松动态地在代表本机系统的网络中加入或移除硬件设备
 * JIT(Just-In-Time) -- JIT编译器，在运行时按需编译，可以减少编译时间和运行时的程序大小，但初次运行时较慢?
 * JMS -- (发送和接收异步消息的?)
 * JMX -- (管理应用程序对象? )
 * JNDI(Java Naming Directory Interface) -- Java命名和目录接口，为某个Java对象起一个名字，然后可通过该名字来访问。
 *    访问方式(JSP页面脚本)： Context ctx = new InitialContext(); DataSource ds = (DataSource)ctx.lookup("java:comp/env" + "自定义名字全路径");
 * JNI(Java Native Interface) -- Java原生接口，用以和C/C++程序接轨，获得高性能
 * JPA(Java Persistence API) -- 是sun公司官方提供的Java持久化的解决方案，基于POJO。推荐在基于Spring的应用程序中使用JPA实现持久化
 * JPA(Java Portlet API) -- ???
 * JSF() -
 * JSP(Java Server Page) -- 最终会编译成Servlet才能运行
 * JSR(Java Spec. Request) -- Java 规格需求，比如 JSR-250， JSR330 等
 * JSTL(JSP Standard Tag Library) -- JSP标准标签库， Sun提供的一套标签库，用于IO、流程控制、XML文件解析、数据库插叙及文字格式标准化等
 * JTA(Java Transaction API) -- Java事务API
 * OGNL() -- 
 * ORM(Object Relation Mapping) -- 对象-关系映射，完成对象数据到关系数据映射的机制, 如 Java中的User类 <==> 数据库中的Tbl_User表。
 *   最早的ORM是 Entity EJB，但非常麻烦。目前流行的ORM框架有Hibernate、iBatis、TopLink等
 * OSGi(Open Service Gateway Initiative) --
 * PO(Persistent Object) --  持久对象，可以理解为 数据库模型通过映射转化成的持久化类，通常和数据库的表对应 
 * POJO(Plain Old Java Object) -- 普通的传统的Java对象，其中有一些属性及其getter setter方法的类,没有业务逻辑。
 *   不允许有业务方法,也不能携带有connection之类的方法。实际就是普通JavaBeans，是为了避免和EJB混淆所创造的简称 。
 *   它可以包含业务逻辑或持久化逻辑，但不担当任何特殊角色且不继承或不实现任何其它Java框架的类或接口
 * RAD() -- 快速应用开发
 * REST(Representational State Transfer) -- 表述性状态转移, 针对网络应用的设计和开发方式的一种软件架构风格
 * RMI(Remote Method Invocation) -- 远程调用，通过存活于远程机器上的对象，发送消息并执行。 
 * RTTI(Run-time Type Information) -- 运行时类型信息
 * SOAP -- 
 * SpEL(Spring Expression Language) -- Spring表达式语言  
 * SSH(Struts + Spring + Hibernate) -- SSH组合是一种轻量级的JavaEE平台，可运行在普通Web容器中，无需EJB容器的支持
 * TLD(Tag Library Definition) -- 标签库定义，用于JSP中，文件扩展名也是 tld，一个标签库中可包含多个标签
 * VO(Value Object) -- 值对象, 可以理解为了便于使用而在PO的基础上进行又一次加工而产生的对象, 通常用于业务层之间的数据传递，可以不和数据库的表对应 
 * WAP(Wireless Application Protocol) -- 无线应用协议
***********************************************************************************************************************************************/
public class JavaGlossaryTest {
	public void testGlossary(){
		
	}
}
