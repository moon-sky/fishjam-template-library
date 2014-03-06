package com.fishjam.study.javaee;

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.ServletContext;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**************************************************************************************************************************************
 * TODO:
 *   1.要编译JaveEE(Ant?),需要将 SDK6 中的 javaee.jar 或 tomcat7中的 jsp-api.jar,servlet-api.jar 加到 %CLASSPATH% 中
 *   2.多个客户端共享同一个Servlet实例？
 *   3.每个请求由一个Java线程处理?
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 *  
 * 功能点
 *   异步请求
 *   Annotation标注
 *
 * 注意：
 *   1.Http有多种请求方式(GET/POST/PUT 等),HttpServlet中对应提供了 doGet/doPost/doPut 等服务方法分别对应。
 *     但如果想处理所有类型的请求，只需简单地实现 service 方法即可
 *     TODO:如果这样的话，还提供各种 doXxx 方法干什么？
 *   
 * Servlet(Server Applet) -- 在服务器上运行的小程序,主要用于交互式地浏览和修改数据，生成动态Web内容
 *   Servlet容器负责创建HttpServlet对象，并把Http请求直接封装到HttpServlet对象中
 
 *
 * javax.servlet.http.HttpServlet。
 *   init() -- 负责初始化Servlet对象。
 *   service() -- 核心，负责响应客户的请求。super.service()方法可能激活其它方法以处理请求，如doGet()或doPost()等
 *    a.通过 Request 获得请求，如 表单数据(getParameter)或URL上的查询字符串
 *    b.通过 Response 生成响应数据, getWriter 输出文本信息，getOutputStream() 输出二进制信息
 *   destory() -- 负责释放占有的资源
 * 
 * HttpServletRequest
 *   getParameter -- 
 * HttpServletResponse
 *   setContentType("text/html;charset=GB2312");
 * 
 *   
**************************************************************************************************************************************/

//使用 @WebServlet Annotation 进行部署
@WebServlet(name="ServletStudy", urlPatterns={ "/ServletDemo" })
public class ServletStudy extends HttpServlet {

	@Override
	protected void service(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		//super.service(request, response);
		
		response.setContentType("text/html;charset=utf-8");
		PrintWriter out = response.getWriter();
		out.println("<html><head><title>");
		out.println(ServletStudy.class.getName());
		out.println("</title></head><body>");
		ServletContext application = getServletConfig().getServletContext();
		
		//获取 web.xml 中使用 context-param 配置的参数
		String dbDriverString = application.getInitParameter("db_driver");
		String dbUrlString = application.getInitParameter("db_url");
		out.print("application init Params are:" + "db_driver=" + dbDriverString
				+ ", db_url=" + dbUrlString);
		out.println("</body></html>");
		
	}

}
