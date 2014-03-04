package com.fishjam.study.javaee;

//轻量级Java_EE企业应用实战_Struts_2+Spring_3+Hibernate整合开发 -- P

/**************************************************************************************************************************************
 * TODO:
 *   1.项目代码分包的时候，可以先分为 MVC 对应的几个大包
 *     model -- dao/dao.hibernate/entity/service/service.spring
 *     view -- struts.action/struts.actionform/taglib
 *     controller -- servlet/utils
 * 
 *
 * 开发工具：   MyEclipse 
 * Web服务器：TomCat
 * J2EE
 * Struts -- MVC框架
 * Spring
 * Hibernate

**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * JavaEE开发大致分为两种方式：
 *   1.以Spring为核心的轻量级JavaEE企业开发平台
 *   2.以EJB3 + JPA 为核心的经典JavaEE开发平台
 *   
 * 搭建 -- http://jingyan.baidu.com/article/6181c3e054a42d152ff1535c.html
 * 
 * SSH框架(WebProject) -- Struts + Spring + Hibernate
 *   目前较流行的一种Web应用程序开源集成框架，用于构建灵活、易于扩展的多层Web应用程序
 *   Struts -- 采用 Servlet/JSP 技术，系统的整体基础架构，负责MVC的分离，控制业务跳转
 *   Spring -- 轻量级的控制反转IoC容器(管理服务组件)和面向切面(AOP)的容器框架，负责查找、定位、创建和管理对象及对象之间的依赖关系
 *   Hibernate -- 对持久层提供支持
 *   
 * 从职责上分为四层：
 *   表示层(UI Layer)--Struts Action + ActionForm + JSP + struts.xml 
 *   业务逻辑层(Business Layer)--Spring + Transactions + Hibernate Session Management + Business Service Classes
 *   数据持久层(Persistence Layer)--Hibernate + DataSource/Connection Pool + SQL + Hibernate Services
 *   域模块层(实体层)-- Domain Model Business Objects 
 *   
 * 业务流程：
 *   UI  层: JSP用户交互  -> Request/Respone -> Structs/Structs2根据 struts-config.xml(1)/struts.xml(2) 将ActionServlet/ActionSupport接收到的Request委派给响应的Action
 *   业务层: Spring IoC容器负责向Action提供业务模型(Model)组件 和 对应的DAO组件 完成业务逻辑，并提供事务处理、缓冲池等容器组件以提升系统性能和保证数据的完整性
 *   持久层：依赖于Hibernate的对象化映射和数据库交互，处理DAO组件请求的数据，并返回处理结果
**************************************************************************************************************************************/

/**************************************************************************************************************************************
* 各种常用功能对应的包
*   Mail(收发邮件) -- activation + mail
*   Oracle(数据库) -- classes12
*   Json -- 好几种实现（http://www.json.org/）
*     org.json.jar; 
*     json-lib -- http://sourceforge.net/projects/json-lib/
*     google-gson -- 
* 
**************************************************************************************************************************************/


public class JavaEEStudy {

}
