package com.fishjam.study.javaee;

/**************************************************************************************************************************************
 * TODO:
 *   1.多个客户端共享同一个Servlet实例？
 *   2.每个请求由一个Java线程处理?
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

public class ServletStudy {

}
