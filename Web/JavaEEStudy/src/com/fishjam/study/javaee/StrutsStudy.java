package com.fishjam.study.javaee;

/**************************************************************************************************************************************
 * TODO:
 *   1.需要使用 applicationContext.xml 文件来设置spring核心包？
 *   2.Struts1中的Action必需是thread－safe方式，仅允许一个实例去处理所有的请求，其用到的所有的资源都必需统一同步。
 *   
 *   Convention(约定)支持 -- ?
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * MVC解决的主要问题:
 *   1.将Web页面中的输入元素封装为一个(请求)数据对象 -- 一般是创建对应的实体类，然后通过对应属性的setter设置
 *   2.(Dispatcher)根据请求的不同，调度相应的逻辑处理单元，并将(请求)数据对象作为参数传入;
 *   3.逻辑处理单元（一般是Action的子类?）完成运算后，返回一个结果数据对象;
 *   4.（Resolver）将结果数据对象中的数据与预先设计的表现层相融合并展现给用户。
 * 
 * 其他的MVC框架
 *   JSF -- Sun 推荐的 Java EE 规范，Apache 也为 JSF 提供了 MyFaces 实现，设计理念上比Struts 2 更优秀( 传统 RAD )，但市场占有率不高
 *   Tapestry -- 完全脱离了传统Servlet API，是一种纯粹的、组件式的MVC框架，同时提供了控制器和页面模版的解决方案，
 *      无需使用 JSP 等其他表现层技术
**************************************************************************************************************************************/


/**************************************************************************************************************************************
 * Struts1.x的动作一般都以.do结尾，
 * Struts2是以.action结尾，可在 <struts>标签下恢复为.do方式, <constant name="struts.action.extension" value="do"></constant>
 *
 * Struts2 -- 在 Webwork 基础上构建起来的MVC框架，其他的MVC框架有 Spring MVC, JSF 等
 *   struts.apache.org 下载安装包，包括 struts2-core, xwork, commons-logging, freemarker, ognl)
 *   MVC模式， 动作驱动：
 *    Model -- Action/ActionForm/JavaBean/EJB，实现核心商业逻辑，最后把控制权传给后续的JSP文件生成视图
 *    View -- Struts taglib/JSP，也可使用 XSLT 等其他表现层产品，taglib中包括了 Html,Bean,Logic,Template 等
 *    Control -- ActionServlet(1)/ActionSupport(2)，提供了处理所有发送到Struts的HTTP请求的入口点，截取和分发这些请求到相应的动作(Action)类
 *    配置文件：struts-config.xml(1)/struts.xml(2) -- 描述模型、视图、控制器对应关系，转发视图(View)的请求，组装响应数据模型（Model）
 * 
 * Struts2的使用步骤：
 *   1.在 web.xml 中配置 Filter(过滤器)
 *     <filter>
 *       <filter-name>struts-prepare</filter-name>
 *       <filter-class>org.apache.struts2.dispatcher.FilterDispatcher</filter-class>
 *     </filter>
 *     <filter-mapping>
 *       <filter-name>struts2</filter-name>
 *       <url-pattern>/*</url-pattern>
 *     </filter-mapping>
 *   2.从 com.opensymphony.xwork2.ActionSupport 继承实现自定义的 Action 类，重写 execute 方法，返回表示执行结果的标志
 *     public class MyAction extends ActionSupport{
 *       private int operand1;  //注意：有对应的 get/set 函数
 *       private int operand2;
 *       public String execute() throws Exception{
 *         if ( getSum() >= 0){  // 如果和是非负整数，跳到positive.jsp页面, getSum() 的实现为 return (operand1 + operand2);
 *           return "positive";
 *         }else // 如果和是负整数，跳到negative.jsp页面
 *         {
 *           return "negative";
 *         }
 *       }        
 *   4.在 struts.xml 中配置 Action(动作类)的代码. struts(1) > package(n) > action(n)
 *     <package name="struts2" namespace="/mystruts" extends="struts-default"> 
 *       <action name="sum" class="action.MyAction">
 *         <result name="positive">/positive.jsp</result>    //name属性表示动作名(execute返回的字符串)，class表示动作类名(跳转的页面)
 *         <result name="negative">/negative.jsp</result>
 *       </action>
 *     </package>
 *     注意：支持通配符配置来自动匹配：<action name="*" class = "com.fishjam.{1}Action" method="show"></action>
 *   5.编写JSP(如 sum.jsp)页面，通过其中的form将数据提交给 Action, 其中使用 Struts2带的tag(在 标签库 "/struts-tags" 中)
 *     在<s:form>中最好都使用Struts2标签，尽量不要用HTML或普通文本.
 *     <%@ page language="java" import="java.util.*" pageEncoding="GBK"%>
 *     <%@ taglib prefix="s" uri="/struts-tags"%>
 *     <html>... 
 *       <s:form action="mystructs/sum.action>
 *         <s:textfield name="operand1" label="操作数1"/>
 *         <s:textfield name="operand2" label="操作数2"/>
 *         <s:submit value="代数和" />
 *       </s:form>
 *     </html>
 *     结果页面： positive.jsp，其中主要有  <s:property value="sum" />   -- <s:property>显示Action类中的sum属性值
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * struts.xml -- 通过该配置文件即可把握整个系统各部分之间的联系(页面导航使系统的脉络更加清晰)
 *   
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * ActionProxy -- 
 * FilterDispatcher -- 核心控制器? 调用ActionMapper确定请求那个Action
 * HttpServletRequest
 * HttpServletResponse 
 * ServletDispatcher
***************************************************************************************************************************************/
/***************************************************************************************************************************************
* JSTL -- JSP Standard Tag Library,JSP标准标签库
***************************************************************************************************************************************/ 	 
public class StrutsStudy {

}
