package com.fishjam.spring.test.framework;

import javax.servlet.http.HttpSession;

import junit.framework.TestCase;

import org.junit.Test;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.servlet.ModelAndView;
import org.springframework.web.servlet.view.tiles3.TilesConfigurer;
import org.springframework.web.servlet.view.tiles3.TilesViewResolver;
import org.springframework.web.servlet.view.xslt.XsltViewResolver;


/***********************************************************************************************************************************************
 * Spring MVC Web框架， 解决状态管理、工作流、验证等
 *   网络数据流程：
 *     1. -> 网络请求， 携带用户所请求的URL和表单内容等信息
 *     2. -> DispatcherServlet(前端控制器，通常为单实例)，根据请求的URL查询处理器映射来确定具体的控制器，并发送给其进行处理 
 *     3. -> 控制器，通过将业务逻辑委托给一个或多个服务对象的方式，进行实际的业务逻辑处理( 控制器不应该直接参与渲染结果给客户端 )，
 *             处理完毕后的信息(model) 需要以用户友好的方式(view)返回给客户进行显示，由控制器将 请求+model+view逻辑名 发送回 DispatcherServlet
 *             注意：请求处理函数的签名可将任何事务作为参数， 如 HttpServletRequest, HttpServletResponse,  HttpSession ， 用户请求的查询参数 等
 *     4. -> 视图解析器(如 FreeMarkerViewResolver)，将 view逻辑名 匹配为一个特定的视图实现(JSP, FTL 等) 
 *     5. -> 视图使用模型数据渲染输出，传递会客户端进行呈现 
***********************************************************************************************************************************************/

/***********************************************************************************************************************************************      
 * 配置:
 *   1.配置 DispatcherServlet -- 在 web.xml 中配置  DispatcherServlet 的 <servlet>,
 *      默认情况下，会从 <servlet-name>-servlet.xml 文件中加载应用上下文
 *   2.处理器映射(Handler Mapping)
 *     Spring自带多个处理器映射实现:
 *       BeanNameUrlHandlerMapping -- 根据控制器Bean的名字将控制器映射到URL
 *       ControllerBeanNameHandlerMapping -- 根据控制器Bean的名字将控制器映射到URL，但Bean的名字不需要遵循URL的约定
 *       ControllerClassNameHandlerMapping -- 通过使用控制器的类名作为URL基础将控制器映射到URL
 *       [推荐, 默认值]DefaultAnnotationHandlerMapping -- 将请求映射给使用 @RequestMapping 注解的控制器和控制器方法
 *       SimpleUrlHandlerMapping -- 使用定义在Spring应用上下文的属性集合将控制器映射到URL
 *     注解驱动：<mvc:annotation-driven /> 表示使用 Spring MVC 提供的注解驱动特性, 注册了多个特性：包括 校验支持、信息转换、域格式化支持等
 *   控制器(Controller) -- 使用 @Controller 声明
 *   视图解析器(View Resolver) -- 将控制器返回的逻辑视图名称转换成渲染结果的实际视图，
 *      其路径一般是 前缀(prefix , 如 "/WEB-INF/views/") + 逻辑视图名 + 后缀(suffix, 如 ".jsp"), 在配置 bean 时通过 <property> 设置
 *      Spring自带多个视图解析器实现， 可通过 "viewClass" 属性更改View对象？
 *         ContentNegotiatingViewResolver -- 委托给一个或多个视图解析器，而选择哪一个取决于请求的内容类型
 *         FreeMarkerViewResolver -- 查找一个基于FreeMarker的模版
 *         InternalResourceViewResolver -- 在Web应用程序的WAR文件中查找视图模版
 *         ResourceBundleViewResolver -- 根据属性文件(properties file) 来查找View实现
 *         TilesViewResolver -- 查找通过 Tiles 模版定义的视图
 *         VelocityLayoutViewResolver -- 解析基于Velocity的视图
 *         XmlViewResolver -- 查找在 /WEB-INF/views.xml 中声明的View实现
 *         XsltViewResolver -- 解析基于 XSLT 的视图
***********************************************************************************************************************************************/

/***********************************************************************************************************************************************
 * Apache Tiles 
 *    页面模版框架，将页面分层片段并在运行时组装成完整的页面
***********************************************************************************************************************************************/
public class SpringMVCTest extends TestCase {

	@Test
	public void testMVC(){
		assertEquals(1, 1);
	}
	
	@RequestMapping("/login/login.do")
	 public ModelAndView listBuyerProduct(HttpSession session,
			 @RequestParam(required = false, defaultValue = "1" )  Integer curPage
			 )
	{
       ModelAndView modelAndView = new ModelAndView("login");
//       modelAndView.addObject("listavailablebuyerproducts", buyerProductService.listAvailableBuyerProducts());
//       if (session.getAttribute("userinfo") != null) {
//           modelAndView.addObject("myid1", JSONArray.fromObject(quotedpriceOneService.listMyQuotedpriceOneIdByUserId(((User) session.getAttribute("userinfo")).getId())).toString());
//           modelAndView.addObject("myid2", JSONArray.fromObject(quotedpriceTwoService.listMyQuotedpriceTwoIdByUserId(((User) session.getAttribute("userinfo")).getId())).toString());
//       }
       return modelAndView;
   }
}
