<!-- HTML 注释，会发送到客户端，注意对比 JSP 注释 -->
<%-- JSP 注释，不会被发送到客户端 --%>
<!-- 
  编译指令 %@ xxx %
    page指令:
      autoFlush: 当输出缓冲区(buffer指定) 即将溢出时，是否需要强制输出缓冲区的内容。如为false则在溢出时会产生一个异常。
      contentType:设定生成网页的文件格式和编码字符集，默认 MIME 类型是( ="text/html" ),  默认字符集类型( charset="ISO-8859-1")
      errorPage: 指定错误处理页面，如果本页面产生了异常或错误且该JSP页面没有对应的处理代码，则自动调用指定的JSP页面
      extends: 指定JSP页面编译所产生的Java类所继承的父类或所实现的接口，默认是 HttpJspBase？
      import="导入的包或类名", 如 import="java.sql.*"
      info: 设置该JSP程序的信息，可通过 Servlet.getServletInfo()  方法获得 
      isErrorPage: 设置本JSP页面是否为错误处理页面，可调用特殊的方法？
      pageEncoding: 指定生成网页的编码字符集，默认是 "???"
      session="true|false": 设定这个JSP页面是否需要HTTP Session
    include 指令(静态include，编译时加入) <==> 对应 jsp:include 的动态include( 运行时通过 JspRuntimeLibrary.include() 加入)
      file="被包含文件的相对URL:"
  动作指令
    forward：《jsp:forward page="{relativeURL | %=expression % }"》《jsp:param name="xxx" value="yyy"/》《/jsp:forward》，
             参数可用 HttpServletRequest.getParameter() 方法获取
    include： 《jsp:include page="{relativeURL | 《%=expression %》}" flush="true"》《jsp:param name="xxx" value="yyy"/》
             flush属性指定输出缓存是否转移到被导入文件中。true(包含在被导入文件中)，false(包含在原文件中)
    useBean: 《jsp:useBean id="beanName" class="classname" scope="page|request|session|application" /》
    setProperty：《jsp:setProperty name="beanName" property="prop1" value="value1" /》
    getProperty：《jsp:getProperty name="beanName" property="prop1" /》
      说明: 直接使用JavaBean的类似乎更方便，new 出的对象可通过 pageContext.setAttribute("名字"， 实例) 的方式放置到指定范围(如Page)中 
    
 -->

<%@page import="java.util.*" %>
<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>JSP的基础语法</title>
</head>
<body>
	JSP基础语法知识
	<br />

	<%! 
	private int mRowIndex = 1; 
	public void _myjspInit(){
		//super._jspInit();
		mRowIndex = 1;
		System.out.println("jsp 初始化了");
	}
	
	%>

	<table border="1" bgcolor="#9999dd">
		<thead>
			<tr>
				<th>索引</th>
				<th>描述</th>
				<th>示例</th>
			</tr>
		</thead>
		<tr>
			<td><%= mRowIndex++ %></td>
			<td>声明变量和函数：&lt;%! 声明部分 %&gt;<br>注意：不能直接使用JSP内置的out等变量</td>
			<td><%! private int mCount = 1; %> 此处定义了成员变量 mCount(初始值为
				1)，该变量在整个Web生存期中各客户端共享<br /></td>
		</tr>
		<tr>
			<td><%= mRowIndex++ %></td>
			<td>调用JSP语句： &lt;% 语句 %&gt;</td>
			<td>
				<% out.println("Count= " + mCount + ", 时间: \"" + new Date() + "\""); mCount++; %>
			</td>
		</tr>
		<tr>
			<td><%= mRowIndex++ %></td>
			<td>输出表达式：&lt;%= 表达式 %&gt;</td>
			<td><%= "Count= " + mCount + ", 时间: \"" +  new Date() +"\"" %>
				，等价于 out.println(表达式)</td>
		</tr>
		<tr>
			<td>Cell</td>
			<td>编译指令 -- 通知JSP引擎消息，不直接生成输出。<br>语法：&lt;%@ 编译指令
				属性名=&quot;属性值&quot; ...%&gt;
			</td>
			<td>page: 针对当前页面的指令<br>include(静态): 指定包含另一个页面嵌入到当前JSP文件中<br>taglib:
				定义和访问自定义标签
			</td>
		</tr>
		<tr>
			<td>2</td>
			<td>动作指令</td>
			<td>jsp:forward -- 执行页面跳转，将请求的处理转发到下一个页面，跳转时用户显示的请求地址不变<br>
				jsp:param -- 用于传递参数，必须与其他支持参数的标签一起使用<br> jsp:include --
				用于动态引入一个JSP页面，不会导入被include页面的编译指令，仅将被导入页面的body内容插入本页面<br>
				jsp:plugin -- 用于下载JavaBean或Applet到客户端执行，客户端必须安装虚拟机，现在已很少使用<br>
				jsp:useBean -- 创建一个JavaBean的实例，代码重用<br> jsp:setProperty --
				设置JavaBean实例的属性值(通过 类的 setXxx 方法设置)<br> jsp:getProperty --
				获取JavaBean实例的属性值(通过类的 getXxx 方法获取)
			</td>
		</tr>
		<tr>
			<td>Cell</td>
			<td></td>
			<td>
				
			</td>
		</tr>
	</table>

	<br>内置对象，在 _jspService() 方法内部初始化 <br>
	<table border="1">
		<thead>
			<tr>
				<th>实例名</th>
				<th>说明</th>
				<th>示例</th>
			</tr>
		</thead>
		<tr>
			<td>application(ServletContext)</td>
			<td>代表Web应用本身，可用于访问Web应用的配置参数(如 web.xml中的context-param)，或在JSP页面、Servlet之间交换信息，</td>
			<td>
				<% 
					//Enumeration<String> initParams = application.getInitParameterNames();
					out.println("ContextPath=" + application.getContextPath()); 
				%>
			</td>
		</tr>
		<tr>
			<td>config(ServletConfig)</td>
			<td>代表当前JSP的配置信息(但通常JSP页面都无需配置)，该对象在JSP中很少使用，但在Servlet中用处较大</td>
			<td>ServeltName=<%= config.getServletName() %></td>
		</tr>
		<tr>
			<td>exception(Throwable)</td>
			<td>代表其他页面中的异常和错误，只有当页面是错误处理页面(isErrorPage=true)时才有效</td>
			<td>TODO</td>
		</tr>
		<tr>
			<td>out(JspWriter)</td>
			<td>代表JSP页面的输出流，用于输出()文本?)内容，形成HTML页面</td>
			<td>Cell</td>
		</tr>
		<tr>
			<td>page(即 this)</td>
			<td>代表页面本身，一般直接用this</td>
			<td>Cell</td>
		</tr>
		<tr>
			<td>pageContext(PageContext)</td>
			<td>JSP页面上下文，主要访问共享数据(getAttribute(scope)),和 获取其他内置对象(getServletContext)等</td>
			<td>Cell</td>
		</tr>
		<tr>
			<td>request(HttpServletRequest)</td>
			<td>该对象封装了一次请求，客户端的请求参数都被封装在该对象里</td>
			<td>TODO</td>
		</tr>		
		<tr>
			<td>response(HttpServletResponse)</td>
			<td>代表服务器对客户端的响应，通常只用于生成二进制响应，重定向等，否则直接使用out对象更方便</td>
			<td>TODO</td>
		</tr>		
		<tr>
			<td>session(HttpSession)</td>
			<td>代表一次会话(浏览器与站点建立连接时会话开始，客户端关闭浏览器时会话结束)</td>
			<td>TODO</td>
		</tr>		
		<tr>
			<td>Cell</td>
			<td>Cell</td>
			<td>TODO</td>
		</tr>		
	</table>

	<br>类说明
	<table border="1">
		<tr>
			<td>HttpJspBase</td>
			<td>Cell</td>
		</tr>
		<tr>
			<td>PageContext(pageContext)</td>
			<td>Cell</td>
		</tr>
		<tr>
			<td>HttpSession(session)</td>
			<td>Cell</td>
		</tr>
		<tr>
			<td>ServletContext(application)</td>
			<td>Cell</td>
		</tr>
		<tr>
			<td>ServletConfig(config)</td>
			<td>Cell</td>
		</tr>
		<tr>
			<td>JspWriter(out, _jspx_out)</td>
			<td>Cell</td>
		</tr>
		<tr>
			<td>Object(page = this)</td>
			<td>Cell</td>
		</tr>
	</table>
</body>
</html>