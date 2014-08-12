package com.fishjam.study.javaee;

import javax.servlet.http.HttpSession;

import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.servlet.ModelAndView;


/**************************************************************************************************************************************
 * 下载地址 -- http://www.springsource.org/download 
 *
 * SpringMVC 优点
 *   1.使用简单，学习成本低 -- 学习难度小于Struts2
 *   2.很容易就可以写出性能优秀的程序 -- Struts2要处处小心才可以写出性能优秀的程序
 *   3.非常灵活
 *   4. SpringMVC 3.0 后更加完善，实现了对 Struts 2 的超越
 * 
 * 常见类和接口(org.springframework.web.servlet.*)
 *     Controller -- 控制器(Action)，因为添加了 "@Controller" 注解表示该类可以担任控制器的职责，所以一般不直接使用该接口
 *     DispatcherServlet -- 前置控制器，在web.xml 的 <servlet> 部分配置，拦截匹配的请求：接收Http请求，并转发给HandlerMapping进行处理
 *     HandlerAdapter -- 处理请求映射的接口
 *     HandlerExceptionResolver -- 异常处理的接口
 *     HandlerInterceptor  -- 拦截器接口，用户可以实现该接口来完成拦截器的工作， preHandle -> postHandle -> afterCompletion
 *     HandlerMapping -- 处理请求映射的接口，将各个请求映射到Controller进行处理
 *     LocalResolver --
 *     ModelAndView --  
 *     ViewResolver -- View解析的接口，有 UrlBasedViewResolver、InternalResourceViewResolver 等实现类，生成http响应
 * 流程和关系(具体关系参见 SpringMVC.png)
 *     DispatcherServlet -> 
 *     
 *     
 * 常用的注解 -- 配置中需要通过 <context:component-scan 配置参数/> 启用Bean的自动扫描功能，否则不能扫描类上的注解
 *    通过配置注解，Spring会自动创建相应的 BeanDefinition 对象，并注册到 ApplicationContext 中，这些类就成了Spring受管组件，
 *    配置参数： <include-filter> 或 <exclude-filter> + type(过滤器类型) + expression(表达式)
 *       如 : <include-filter type="assignable" expression="com.myInterface"> -- 自动注册所有 myInterface 的实现类 
 *       过滤器类型：
 *          annotation(指定注解所标注的类);
 *          assignable(派生于指定类型的类);
 *          aspectj(指定AspectJ表达式匹配的类);
 *          custom(自定义的 TypeFilter 实现类);
 *          regex(正则表达式匹配的类);
 *    @Autowired 用于注入，(srping提供的) 默认按类型装配，要求必须存在，若允许null值，可以设置其required属性为false
 *       若想按名称装配，则可以结合 @Qualifier 注解一起使用
 *    @Component -- 泛指组件(Bean), 当不好归类时使用
 *    @Controller -- 声明Action组件，通常用在控制层
 *    @Configuration -- 声明Spring的配置类，等价于XML配置中的 <beans> 元素，然后可在其中使用 @Bean 声明返回Bean对象的函数(函数名即为其id)
 *    @CookieValue -- 
 *    @PostConstruct -- 
 *    @PreDestroy -- 
 *    @Qualifier("beanName") -- 优先配置指定名字的Bean，也可以用来创建自己的限定器注解
 *    @Repository -- 数据仓库，将数据访问层 (DAO层)的类标识为 Spring Bean，然后就不再需要在XML中通过<bean/>显示配置。
 *       可自动提供一些数据访问相关的功能。如 数据访问异常类型.
 *    @RequestHeader -- 从请求中获取值
 *    @RequestMapping("/login/login.do") -- 请求映射, 用在方法前
 *    @RequestParam(参数) 类型 变量名 -- 在 @RequestMapping 中的参数
 *    @Resource -- 用于命名资源依赖注入，( j2ee提供的 ) 默认按名称装配，对比：@Autowired默认按类型.
 *       例：@Resource(name="beanName")，若未提供name，则先取字段的名称作为bean名称寻找对象，若找不到，则使用类型装配方式 
 *    @ResponseBody -- 
 *    @Scope("prototype") -- 设定bean的作用域 
 *    @Service -- 声明Service组件，通常用在业务层
 *    @Transactional( rollbackFor={Exception.class}) -- 事务管理
 *  TODO(可能支持的，未确定)
 *    @NumberFormatannotation
 *    @DateTimeFormat
 *    @Valid
 *    
 * 单元测试
 *    JUnitActionBase
**************************************************************************************************************************************/

public class SpringMVCStudy {
	
	
	@RequestMapping("/login/login.do")
	 public ModelAndView listBuyerProduct(HttpSession session,
			 @RequestParam(required = false, defaultValue = "1" )  Integer curPage
			 )
	{
        ModelAndView modelAndView = new ModelAndView("login");
//        modelAndView.addObject("listavailablebuyerproducts", buyerProductService.listAvailableBuyerProducts());
//        if (session.getAttribute("userinfo") != null) {
//            modelAndView.addObject("myid1", JSONArray.fromObject(quotedpriceOneService.listMyQuotedpriceOneIdByUserId(((User) session.getAttribute("userinfo")).getId())).toString());
//            modelAndView.addObject("myid2", JSONArray.fromObject(quotedpriceTwoService.listMyQuotedpriceTwoIdByUserId(((User) session.getAttribute("userinfo")).getId())).toString());
//        }
        return modelAndView;
    }
}
