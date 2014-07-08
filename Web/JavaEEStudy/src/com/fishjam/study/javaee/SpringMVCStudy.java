package com.fishjam.study.javaee;

/**************************************************************************************************************************************
 * 下载地址 -- http://www.springsource.org/download 
 *    
 * SpringMVC 优点
 *   1.使用简单，学习成本低 -- 学习难度小于Struts2
 *   2.很容易就可以写出性能优秀的程序 -- Struts2要处处小心才可以写出性能优秀的程序
 *   3.非常灵活
 * 
 * 常见类和接口(org.springframework.web.servlet.*)
 *     Controller -- 控制器(Action)，因为添加了 "@Controller" 注解表示该类可以担任控制器的职责，所以一般不直接使用该接口
 *     DispatcherServlet -- 前置控制器，在web.xml 的 <servlet> 部分配置，拦截匹配的请求，接收Http请求，并转发给HandlerMapping进行处理
 *     HandlerAdapter -- 处理请求映射的接口
 *     HandlerExceptionResolver -- 异常处理的接口
 *     HandlerInterceptor  -- 拦截器接口，用户可以实现该接口来完成拦截器的工作
 *     HandlerMapping -- 处理请求映射的接口，将各个请求映射到Controller进行处理
 *     LocalResolver --
 *     ModelAndView --  
 *     ViewResolver -- View解析的接口，有 UrlBasedViewResolver、InternalResourceViewResolver 等实现类，生成http响应
 * 流程和关系(具体关系参见 SpringMVC.png)
 *     DispatcherServlet -> 
**************************************************************************************************************************************/

public class SpringMVCStudy {

}
