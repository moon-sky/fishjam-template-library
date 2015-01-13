package com.fishjam.springtester.framework;

//轻量级Java_EE企业应用实战_Struts_2+Spring_3+Hibernate整合开发 -- P241(国际化、资源)
//  使用的是 Eclipse-jee-helios 版( Eclipse 3.6 for Java EE Developers )
// 官方的在线文档：http://docs.oracle.com/javaee/7/api/index.html?overview-summary.html

/**************************************************************************************************************************************
* 选择MVC框架时需要考虑的
*    1.简单易用，以提高开发效率 -- 使小部分的精力在框架上，大部分的精力放在业务上
*    2.性能优秀
*    3.尽量使用大众的框架(避免使用小众的、私有的框架)，人员有技术积累
*    
*  常用的MVC框架
*     1.Spring 中自带的 Spring MVC -- 3.0以后更加完善，实现了对Struts 2的超越
**************************************************************************************************************************************/
/**************************************************************************************************************************************
 * JavaEE 项目的目录结构
 * 
 * WebRoot/
 *   index.jsp
 *   content/
 *   css/
 *   images/
 *   js/
 *   META-INF/ 
 *   WEB-INF/  -- 此目录下的内容客户端不能直接访问，只能服务器访问。一般放置部署描述符(web.xml) 和 Java 类(因此不放 css/js/images 一类的文件)。
 *                       但是(不好的习惯？)：可以通过转向(forward)的方式来访问该目录下的 jsp 文件。该目录下的文件访问其他目录的文件时可以忽略 WEB-INF 目录的层次关系
 *     web.xml
 *     build.xml
 *     struts-config.xml
 *     classes/
 *     lib/
 *     src/  -- 保存Servlet的Java源码等
**************************************************************************************************************************************/


/**************************************************************************************************************************************
 * 
 * TODO:
 *   1.项目代码分包的时候，可以先分为 MVC 对应的几个大包
 *     model -- dao/dao.hibernate/entity/service/service.spring
 *     view -- struts.action/struts.actionform/taglib
 *     controller -- servlet/utils
 *   2.DAO 和 DO 哪个在上面？书上说 DO 在下，但DAO需要和数据库打交道，感觉应该更下？
 *   3.Web应用中，Web容器会包含 WEB-INF 文件夹下的内容，客户端浏览器无法访问该路径下的任何内容
 *    
 * 开发工具：   MyEclipse 
 * Web服务器：TomCat
 * J2EE
 * Struts -- MVC框架
 * Spring
 * Hibernate

**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * JavaEE提供的 跨平台、开放、各种远程访问技术等，为异构系统的良好整合提供了保证。
 * JavaEE开发大致分为两种方式：
 *   1.以 Spring 为核心的轻量级JavaEE企业开发平台
 *   2.以 EJB3+JPA 为核心的经典JavaEE开发平台
 *   
 * 搭建 -- http://jingyan.baidu.com/article/6181c3e054a42d152ff1535c.html
 * 
 * SSH框架(WebProject) -- Struts + Spring + Hibernate
 *   目前较流行的一种Web应用程序开源集成框架，用于构建灵活、易于扩展的多层Web应用程序
 *   Struts -- 采用 Servlet/JSP 技术，系统的整体基础架构，负责MVC的分离，控制业务跳转
 *   Spring -- 轻量级的控制反转IoC容器(管理服务组件)和面向切面(AOP)的容器框架，负责查找、定位、创建和管理对象及对象之间的依赖关系
 *   Hibernate -- 对持久层提供支持
 *   
 * JavaEE从职责上分为以下几层 ( 各层的组件之间以松耦合的方式耦合，利于扩展 )：
 *   表示层(UI Layer) -- 负责收集用户请求，并显示处理结果。
 *     Struts Action + ActionForm + JSP + struts.xml 
 *   控制器层 -- 由系列控制器组成，用于拦截用户请求，将请求转发给用户实现的控制器组件，由其调用业务逻辑组件的业务逻辑方法处理，并根据处理结果转发到不同的表现层组件.
 *     目前大部分采用 MVC 框架
 *   业务逻辑层(Business Layer)-- 由系列的业务逻辑对象组成，实现了系统所需要的业务逻辑方法，
 *     该层不应该进行数据库访问(保证业务逻辑方法的实现与具体的持久层访问技术分离)，通过接口和DAO进行交互
 *     Spring + Transactions + Hibernate Session Management + Business Service Classes
 *   数据持久层(Persistence Layer)-- 提供数据访问服务, Hibernate + DataSource/Connection Pool + SQL + Hibernate Services
 *     数据访问对象层(DAO|EAO) -- 由系列的DAO组件组成，实现了对数据库的 CRUD 等原子操作，向上通过接口提供服务(更好的解耦，以便更换数据库)
 *   领域模块层(Domain Object)--由系列的POJO组成，这些对象是该系统的  Domain Model Business Objects 
 *   数据库层(Database Layer) -- 提供持久化服务
 *   
 * 业务流程：
 *   UI  层(Web层): JSP用户交互  -> Request/Respone -> Structs/Structs2根据 struts-config.xml(1)/struts.xml(2) 将ActionServlet/ActionSupport接收到的Request委派给响应的Action
 *   业务层(Service层): Spring IoC容器负责向Action提供业务模型(Model)组件 和 对应的DAO组件 完成业务逻辑，并提供事务处理、缓冲池等容器组件以提升系统性能和保证数据的完整性
 *   持久层(Dao层)：依赖于Hibernate的对象化映射和数据库交互，处理DAO组件请求的数据，并返回处理结果
**************************************************************************************************************************************/

/**************************************************************************************************************************************
* 各种常用功能对应的包
*   Mail(收发邮件) -- activation.jar + mail.jar
*   Oracle(数据库) -- classes12
*   Json -- 好几种实现（http://www.json.org/）
*     org.json.jar; 
*     json-lib -- http://sourceforge.net/projects/json-lib/
*     google-gson -- 
* 
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * 热部署 -- 应用正在运行的时候升级软件，却不需要重新启动应用，开发阶段可以实时编辑、调试Java文件 等。
 *   能否支持热部署，除了服务器(Tomcat, Jetty 等) 外，也受限于第三方软件包(Struts, Spring, Hibernate 等) -- 必须把.class复制到对应的目录下
 *   容器
 *      Tomcat -- server.xml 中 <Context ... reloadable="true" />, 参见 TomcatStudy.java, 可以检测修改的类文件？
 *        TODO: <WatchedResource>WEB-INF/web.xml</WatchedResource>  《==可以检测配置文件的更改？
 *      JBoss --
 *      Jetty -- 使用maven插件"jetty-maven-plugin" 运行项目，更改pom.xml中jetty的配置: <configuration><scanIntervalSeconds> 为 0 
 *        TODO: 实际上还需要 JRebel 检视指定的classes目录？
 *          Eclipse 中在线安装JRebel -- http://www.zeroturnaround.com/update-site/
 *   开发工具：
 *      Eclipse -- 
 *  
 *   Struts -- <constant name="struts.devMode" value="true" /> ？
 *   Mule -- ？
 ***************************************************************************************************************************************/
public class JavaEETest {

}
