package com.fishjam.study.javaee;

/**************************************************************************************************************************************
 * TODO:
 *   1.需要使用 applicationContext.xml 文件来设置spring核心包？
 *   2.Struts1中的Action必需是thread－safe方式，仅允许一个实例去处理所有的请求，其用到的所有的资源都必需统一同步。
 *   3.从 http://struts.apache.org/ 下载 Structs, 建议下载 Full Distribution(完整版)，
 *     若需要使用 Ant 等编译，则需要将 struts2-core-x.y.z.jar 和 xwork-core-x.y.z.jar 添加到系统的 CLASSPATH 环境变量 
 *   
 *   Convention(约定)支持 -- 如：核心控制器接收到 regist.action 请求后，会调用 RegistAction 类来处理用户请求
 *     1.将 struts2-convention-plugin-x.y.z.jar 文件复制到 WEB-INF\lib 目录下
 *     2.Convention插件会自动搜索位于 action,actions,struts,struts2 包下的所有Java类，并将如下两种Java类当成Action处理：
 *       a.实现了 com.opensymphony.xwork2.Action 的Java类
 *       b.所有类名以 Action 结尾的Java类
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
 *   该框架下，用户请求不再直接向JSP页面发送，而是由核心控制器 StrutsPreparedAndExecuteFilter 根据 Action 的返回值来"forward"JSP页面生成相应
 *   
 * Struts2 -- 在 Webwork 基础上构建起来的MVC框架，其他的MVC框架有 Spring MVC, JSF 等
 *   struts.apache.org 下载安装包，包括 struts2-core, xwork, commons-logging, freemarker, ognl)
 *   MVC模式， 动作驱动：
 *    Model -- Action/ActionForm/JavaBean/EJB，实现核心商业逻辑，最后把控制权传给后续的JSP文件生成视图
 *    View -- Struts taglib/JSP，也可使用 XSLT 等其他表现层产品，taglib中包括了 Html,Bean,Logic,Template 等
 *    Control -- ActionServlet(1)/ActionSupport(2)，提供了处理所有发送到Struts的HTTP请求的入口点，截取和分发这些请求到相应的动作(Action)类
 *    配置文件：struts-config.xml(1)/struts.xml(2) -- 描述模型、视图、控制器对应关系，转发视图(View)的请求，组装响应数据模型（Model）
 *    
 * 
 * Struts2的使用步骤：
 *   1.在 web.xml 中配置 Filter(核心控制器)，让 Structs2 的核心Filter拦截用户请求
 *     <filter>
 *     		<filter-name>struts2</filter-name>
 *     		<filter-class>org.apache.struts2.dispatcher.ng.filter.StrutsPreparedAndExecuteFilter</filter-class>
 *     </filter>
 *     <filter-mapping>
 *       <filter-name>struts2</filter-name>
 *       <url-pattern>/*</url-pattern>
 *     </filter-mapping>
 *     补充说明：
 *     可选的Filter -- 
 *       StrutsExecuteFilter
 *       StrutsPrepareFilter
 *       (推荐)StrutsPreparedAndExecuteFilter
 *   2.从 com.opensymphony.xwork2.ActionSupport 继承实现自定义的 Action 类(业务控制器)，重写 execute 方法，返回表示执行结果的标志
 *     public class MyAction extends ActionSupport{
 *       private int operand1;  //注意：有对应的 get/set 函数，TODO：这些值从何而来？
 *       private int operand2;
 *       public String execute() throws Exception{
 *         if ( getSum() >= 0){  // 如果和是非负整数，跳到positive.jsp页面, getSum() 的实现为 return (operand1 + operand2);
 *           return "positive";
 *         }else // 如果和是负整数，跳到negative.jsp页面
 *         {
 *           return "negative";
 *         }
 *       }
 *      补充：
 *        a.ActionContext.getContext().getSession();  //获取当前Session对应的Map
 *        b.预定义的返回值： SUCCESS, ERROR, 
 *        c.Action类可以使一个普通的POJO，只需要包含一个 public String execute() 方法？
 *        d.Action类里定义的属性 可以由structs2框架根据 参数名字 自动 get/set    
 *   4.在 struts.xml 中配置 Action(动作类)的代码. struts(1) > package(n) > action(n)
 *     <package name="struts2" namespace="/mystruts" extends="struts-default"> 
 *       <action name="sum" class="action.MyAction"> //指定名为 sum 的Action，将会由对应的类进行处理(用户可向该Action发送请求 -- s:form 的 action )
			//下面通过 result 元素，指定逻辑视图和物理资源视图之间的映射
 *         <result name="positive">/positive.jsp</result>    
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
 *   该文件应该放在 Web 应用的类加载路径下(如 src/struts.xml )，部署时Eclipse会自动拷贝到 WEB-INF/classes 路径下
 *   为避免struts.xml 文件过于庞大、臃肿，可分解成多个配置文件(内容为标准的Struts2配置文件 ), 然后通过 <include file="myStruts1.xml"/> 包含
 * struts-default.xml -- Struts2框架自带的配置文件
 * struts-plugin.xml -- Struts2插件的默认配置文件
 * 
 * 配置内容
 *   <struts>
 *     <constant name="xxx" value="yyy /> -- 配置常量
 *     <bean type="xxx" name="yyy" class="zzz" scope="aaa" static="bbb" optinal="ccc" /> --
 *     <include file="aaa" /> --
 *     <package name="包名" extends="struts-default">
 *       <action name="xxx" class="yyy" method="POST" converter="zzz">
 *         <result name="SUCCESS" type="xxx">映射资源.jsp</result>
 *       </action>
 *   </struts>
 * 
 *   配置Action 
 *     1.指定 用户请求 和 处理Action 的对应关系	<action>
 *     2.指定 处理结果 和 物理视图资源 的对应关系 < result >
 *   
 *   拦截器( interceptor )
 *   异常处理
 *     异常映射: <exception-mapping -- 配置发生指定的 exception 时返回 result 属性对应的结果 >
 *       映射映射可以放在 <action> 内作为局部异常映射，也可以放在 <global-exception-mappings> 中作为全局异常映射
 *       全局映射结果需要通过 <global-results> 定义
 *     异常显示： <s:property value="exception"/> 和 <s:property value="exceptionStack" />
 *     
 *   输入校验：校验文件命名遵循如下规则： <ActionName>-validation.xml 
 *   
 * 使用 struts.properties 文件来管理常量 -- 推荐在 struts.xml 中 用 <constant name="xxx" value="yyy" /> 的方式
 *   (查看 struts2-core-x.y.z.jar\org\apache\struts2\default.properties 文件可知预定义参数名和默认值)
 *   struts.multipart.maxSize -- 指定Struts2文件上传中整个请求内容允许的最大字节数
 *   struts.action.extension[action] -- 指定需要Struts2处理的请求后缀，默认值是 *.action
 *   struts.devMode[false] -- 设置为true时，当应用出错时会显示更多、更友好的出错信息
 *   struts.el.throwExceptionOnFailure[false] -- 当表达式计算失败时，或表达式里某个常量不存在时是否抛出一个 RuntimeException 
**************************************************************************************************************************************/

/**************************************************************************************************************************************
* 国际化支持：
*   1.structs.xml 中增加 
*     <constant name="struts.custom.i18n.resources" value="messageResource"/>
*     <constant name="struts.i18n.encoding" value="GBK"/> 
*   2.编写 messageResource.properties, messageResource_en_US.properties 等多语言的键值对文件
*   3.注意使用 native2ascii 命令处理国际化资源 -- 如果编码是 UTF-8 是否还需要处理？
**************************************************************************************************************************************/


/**************************************************************************************************************************************
 * 常见的类和接口
 *   Action -- Strut2提供的接口，其中定义了 ERROR, INPUT,LOGIN,NONE,SUCCESS  等标准字符串 和 execute() 的方法原型
 *     +-ActionSuport -- Action的实现类，提供了 获取国际化信息、数据校验、默认的处理用户请求 等功能，一般从该类继承开发
 *          getText() -- 获取国际化消息
 *   ActionContext -- 访问Servlet API
 *     static getContext() -- 静态方法，获取系统的 ActionContext实例 
 *   ActionProxy --
 *   FilterDispatcher -- 核心控制器? 调用ActionMapper确定请求那个Action
 *   HttpServletRequest
 *   HttpServletResponse 
 *   ServletDispatcher
 *   ServletActionContext -- 其中有不少静态方法 getPageContext, getRequest  获得对应的Servlet对象
 * 
 * Action中直接访问 Servlet API -- Action实现指定接口即可直接访问Web应用的对应实例
 *   ServletContextAware  ==> ServletContext
 *   ServletRequestAware ==> HttpServletRequest
 *   ServletResponseAware ==>HttpServletResponse, 注意：不能使用respone直接生成响应代码(Action只是业务控制器，不负责UI)
 *   
***************************************************************************************************************************************/
/***************************************************************************************************************************************
* JSTL -- JSP Standard Tag Library,JSP标准标签库
*   structs2 的标签库 -- <%@ taglib prefix="s" uri="/struts-tags"%>
*     需要在 WEB-INF\lib 下考入 struts2-core-xxxx.jar, ognl-xxxx.jar 等
*   设置的属性：
*      name(Action 使用的属性名) 
*      key( 国际化时的键值对 )
*    标签：
*     <s:form> -- 
*     <s:param>
*     <s:property value="Action类属性名" > -- 字符串属性
*     <s:submit> -- 
*     <s:text> --  
*     <s:textfield name="username" key="user" > --
***************************************************************************************************************************************/ 	 
public class StrutsStudy {

	
}
