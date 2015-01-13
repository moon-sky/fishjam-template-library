package com.fishjam.springtester.framework;

/**************************************************************************************************************************************
 * 常见辅助调试的选项
 *   IE
 *     
 *   Struts2
 *     struts.xml
 *       /struts/constant[name="struts.devMode" value="true"] -- 设置为 true可使用开发模式，应用出错时显示更多、更友好的出错信息
 *       conststruts.devMode="true" -- 
 *       struts.configuration.xml.reload="true" -- 当struts.xml文件改变后，是否自动加载该文件
 *       struts.convention.classes.reload="true" -- 如果使用 Convention 自动映射时，可在Action或JSP页面改变时自动加载相关文件
 *     Config Browser插件(struts2-config-browser-plugin-x.y.z.jar 复制到 WEB-INF\lib 目录下) -- 查看Action等各种资源的映射情况
 *       访问以下地址： http://xxx/应用名/config-browser/index.action 
 *        
 *   Tomcat
 *     web.xml
 *       /web-app/servlet/init-para[param-name='listings'] -- 将对应的 param-value 改为 'true', 列出Web应用路径下所有页面，方便直接选取文件调试 
 *     动态配置或 server.xml 中 <Context docBase= > 里 reloadable="true"， 则更改 Servlet 后会自动重新加载
 *
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * TODO:
 *   1.网上说 "如果 Web 应用在启动时, WEB-INF 下的 classes 和 lib 目录不存在，则在启动应用后，新建 classes|lib  目录，动态添加的内容是没有用的，
 *      tomcat 不会进行监控这些目录”--  实测： 停止tomcat -> 删除 work -> 重启tomcat ，结果：无效  
 * 
 * JavaEE
 *   1.使用<jsp:useBean> 时报 "The value for the useBean class attribute XXXXXXX is invalid."
 *     原因：a.在编译 JSP 时(不是运行时)，找不到指定的 Bean 类; b.或Bean类不是public 的可实例化类; c.或没有public的默认构造
 *     解决：
 *       a. 是因为如下代码出现异常(？？？)： Class bean = ctxt.getClassLoader().loadClass(klass);
 *          使用ant等将Bean相关的源码先编译到 WEB-INF\classes\*.class; 可以设置 reloadable 为 true ? 或将 <jsp:useBean> 中的 "class" 改为 "type" ?
 *          注意(TODO:考虑描述方式?)：WEB-INF 必须在虚拟根目录的第一级之目录下，否则也会出现这种情况，或找不到 servlet 的问题，
 *                   WEB-INF 所在的目录即为 虚拟目录的根目录，会影响 servlet 时的设置 jsp-file 时指定的文件路径
 *          因为 Servlet 编译成 .class 后，tomcat只在第一次启动时才会加载(不监控)，所以每次更改后必须重启 tomcat ? 设置 reloadable 为 true 可不重启
 *   2.MyEclipse 中导入Spring后，启动Tomcat时 org.apache.catalina.core.StandardContext listenerStart 类中报错：
 *     Error configuring application listener of class org.springframework.web.context.ContextLoaderListener
 *     a.经过检查，springhibernate/spring.jar 和 org.springframework.web-3.1.1.RELEASE.jar 中都包含了这个类
 *     TODO: spring-web.jar ?
 *     也可能是缓存问题？
**************************************************************************************************************************************/

public class CommonErrors {

}
