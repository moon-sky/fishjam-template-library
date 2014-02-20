package com.fishjam.storeserver.study;

/**************************************************************************************************************************************
 * Struts2 -- 在 Webwork 基础上构建起来的MVC框架( Struts1.x的动作一般都以.do结尾，而Struts2是以.action结尾 )
 *   struts.apache.org 下载安装包，包括 struts2-core, xwork, commons-logging, freemarker, ognl)
 *   MVC模式， 动作驱动：
 *    Model -- Action/ActionForm
 *    View -- Struts taglib/JSP，也可使用 XSLT 等其他表现层产品
 *    Control -- ActionServlet(1)/ActionSupport(2)
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
 * struts.xml -- 通过该配置文件即可把握整个系统各部分之间的联系
 *   
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * ActionProxy -- 
 * FilterDispatcher -- 核心控制器? 调用ActionMapper确定请求那个Action
 * HttpServletRequest
 * HttpServletResponse 
 * ServletDispatcher
***************************************************************************************************************************************/
 
public class StrutsStudy {

}
