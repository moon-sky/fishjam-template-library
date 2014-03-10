<!-- HTML 注释，会发送到客户端，注意对比 JSP 注释 -->
<%-- JSP 注释，不会被发送到客户端 --%>
<!------------------------------------------------------------------------------------------------------------------------------------ 
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
    
  使用 response 在客户端生成图片(如验证码)
    1.设置 page 的 contentType="image/jpeg" 
    2.设置 page 的 import ="java.awt.image.*, javax.imageio.*, java.io.*, java.awt.* "
    3.生成图片数据： BufferedImage image = new BufferedImage(xxx); Graphics g = image.getGraphics();  绘图; g.dispose(); 
    4.将图像输出到页面响应： ImageIO.write(image, "JPEG", response.getOutputStream());
    5.客户端使用: 《img src="img.jsp" 》 或 通过 Action + HttpServlet 的设置方式 ?
    
  转发(forward)和重定向(redirect)对比
    1.执行 forward 后依然是上一次请求，执行 redirect 后生成第二次请求;
    2.forward 的请求参数和request范围的属性全部保留； redirect 后的原请求参数和request范围的属性全部丢失;
    3.forward 后地址栏里的URL不变； redirect 后地址栏改为重定向的目标URL
    4. ? forward 相当于服务器进行处理； redirect 相当于客户端进行处理
 --- --------------------------------------------------------------------------------------------------------------------------------->
 
<!-- --------------------------------------------------------------------------------------------------------------------------------
  自定义标签库(*.tld) -- JSTL 是Sun提供的一套标准标签库，Appache 也提供了一套用于生成页面并显示的 DisplayTag
    1.编写标签处理类：
       a.从 SimpleTagSupport  继承， 重写 doTag() 方法，负责生成页面内容( getJspContext(). getOut().write(...) )
         也可以从 TagSupport 继承，然后重写 doEndTag 方法? 可直接使用 pageContext.getOut().write(...)
       b.如标签类包含属性，每个属性都有对应的 getter/setter 方法;
    2.编写标签库定义文件(*.tld) , 格式为 taglib -> tag。编写完毕后复制到 WEB-INF 或其子目录下，Web容器会自动加载该文件 
       a. 定义标签库的 <uri>http://www.fishjam.com/mytaglib</uri>, 相当于该标签库的唯一标识，JSP页面中使用时根据该URI属性来定位标签库
       b.定义一到多个标签 <tag>, 其中可设置的子元素:
          name : 标签名
          body-content: 指定标签体内容，其值可为： 
            tagdpendent(标签处理类自己负责处理标签体)
            empty(该标签只能作为空标签使用)，
            scriptless(标签体可以是静态HTML元素、表达式语言，但不允许出现JSP脚本)
            JSP(标签体可以使用JSP脚本， 已不推荐使用)
            dynamic-attributes(指定是否支持动态属性，只有当定义动态属性标签时才需要该子元素)
          fragment: true|false
          required: 是否必须提供
          tag-class: 标签处理类
       c.如果标签有属性，则需要增加 《attribute》子元素定义标签属性
    3.使用标签
      a.通过URI导入标签库并指定前缀: 《%@ taglib uri="http://www.fishjam.com/mytaglib" prefix="fjtag" %》
      b.使用标签的语法: 《fjtag:tagName tagAttr="tagValue"》标签体《/fjtag:tagName》
      
    带标签体的标签，可内嵌其他内容，通常用于完成一些逻辑运算，如 判断和循环等
      1.在 doTag() 中 getJspContext().setAttribute("对象属性名", 属性对象);  getJspBody().invoke(null);  调用标签体中的invoke方法来输出标签体内容
      2.标签定义文件中指定  《body-content》 为 scriptless
      3.在JSP 中，标签体里写上  ${pageScope.对象属性名} ， 则会自动根据 "对象属性名" 查找对应的值进行显示(如迭代容器对象)  
  JspFragment -- 页面片段
   动态属性标签：属性个数、属性名不确定。标签处理类需要实现 DynamicAttributes 接口，实现setDynamicAttribute来设置属性名的键值对。
      配置文件中通过《dynamic-attributes》子元素指定该标签支持动态属性。使用时通过 dynaAttr 设置任意的属性键值对
   
 表达式语言(Expression Language)-- 一种简化的数据访问方式，可以方便的访问JSP的隐含对象和JavaBeans组件。
 ------------------------------------------------------------------------------------------------------------------------------------>

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
	<h1>JSP基础语法知识</h1>
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
			<td>jsp:forward -- 执行页面跳转，将请求的处理转发到下一个页面，跳转时用户显示的请求地址不变，请求参数和request范围的属性不会丢失<br>
				jsp:param -- 用于传递参数，必须与其他支持参数的标签一起使用<br> 
				jsp:include --用于动态引入一个JSP页面，不会导入被include页面的编译指令，仅将被导入页面的body内容插入本页面<br>
				jsp:plugin -- 用于下载JavaBean或Applet到客户端执行，客户端必须安装虚拟机，现在已很少使用<br>
				jsp:useBean -- 创建一个JavaBean的实例，代码重用<br> 
				jsp:setProperty -- 设置JavaBean实例的属性值(通过 类的 setXxx 方法设置)<br> 
				jsp:getProperty -- 获取JavaBean实例的属性值(通过类的 getXxx 方法获取)
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
			<td>该对象封装了一次请求，客户端的请求参数都被封装在该对象里。<br/>
					如果POST请求的参数里包含非西欧字符，则必须在获取参数前先调用 setCharacterEncoding 设置编码的字符集(不能自动检测 contentType？ )<br/>
					如GET请求的参数包含，则需要用 URLDecoder.decode() 进行处理。<br/> 
					代码中如要 URL转发则需要通过 getRequestDispatcher 进行
					</td>
			<td>TODO</td>
		</tr>		
		<tr>
			<td>response(HttpServletResponse)</td>
			<td>代表服务器对客户端的响应，通常只用于生成二进制响应(getOutputStream)，重定向(sendRedirect, 会丢失所有的请求参数和request范围的属性)，
			addCookie 等，否则直接使用out对象更方便</td>
			<td>TODO</td>
		</tr>		
		<tr>
			<td>session(HttpSession)</td>
			<td>代表一次会话(浏览器与站点建立连接时会话开始，客户端关闭浏览器时会话结束)，通常用户跟踪用户的会话信息(是否登录、购物车中的商品等)<br/>
			注意：session中的属性值必须是可序列化的，否则将引发不可序列化的异常</td>
			<td>TODO</td>
		</tr>		
		<tr>
			<td>Cell</td>
			<td>Cell</td>
			<td>TODO</td>
		</tr>		
	</table>

	<br>JSP标签库(*.tld), 其中可以包含多个标签，由标签处理类( SimpleTagSupport 的子类) 提供支持 
	<table border="1">
		<tr>
			<td>Cell</td>
			<td>Cell</td>
		</tr>
		<tr>
			<td>Cell</td>
			<td>Cell</td>
		</tr>
		<tr>
			<td>Cell</td>
			<td>Cell</td>
		</tr>
		<tr>
			<td>Cell</td>
			<td>Cell</td>
		</tr>
		<tr>
			<td>Cell</td>
			<td>Cell</td>
		</tr>
		<tr>
			<td>Cell</td>
			<td>Cell</td>
		</tr>
		<tr>
			<td>Cell</td>
			<td>Cell</td>
		</tr>
	</table>
</body>
</html>