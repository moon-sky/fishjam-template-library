package com.fishjam.study.javaee;

/**************************************************************************************************************************************
 * TODO:
 *     Apache -- 为HTML页面服务
 *     Tomcat -- 运行JSP页面和Servlet，是一个Servlet和JSP容器。可以处理HTML页面，但能力不如Apache
 *     
 * 其他Web服务器
 *     JBoss -- 开源的专业 Jave EE 服务器
 *     Jetty -- 特点：可作为一个嵌入式服务器(即 可在应用中加入其 JAR 文件， 应用可在代码中对外提供Web服务 )
 *     Resin -- 目前最快的 JSP、Servlet 运行平台，支持EJB，个人学习免费，但商用收费
 *     WebLogic |  WebSphere  -- 专业的商用 Jave EE 服务器
 *     
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * Tomcat -- 开源且免费的jsp服务器，在中小型系统和并发访问用户不是很多的场合下被普遍使用，是Apache服务器的扩展
 * 
 * 使用方式，推荐使用解压缩的方式( 可以看到启动、运行时的控制台输出，利于开发调试)
 *   环境配置:
 *     1. 设置 TOMCAT_HOME 环境变量；
 *     2. 将 lib 目录下的 jsp-api.jar 和 servlet-api.jar 添加到 CLASSPATH 环境变量中
 *     3. 将Web应用 (整个目录 或 WAR文件) 复制到 webapps 目录下，即可自动部署到容器中，
 *   启动服务( 默认为 http://localhost:8080 )
 *     1.解压方式 : startup.bat 
 *     2.安装 : 开始 ->Apache Tomcat -> Configure Tomcat -> Start( bin\TomcatX.exe ?)
 * 
 * 配置
 *   1. bin\catalina.bat 中设置Java虚拟机的内存参数，Xms和PermSize只对性能有影响，Xmx代表虚拟机可以使用的内存，
 *     set JAVA_OPTS=-Xms1024m -Xmx1024m -Xmn512m -XX:PermSize=128m -XX:MaxPermSize=256m
 *   2. 配置DBCP数据源 -- 可配置为 全局数据源(server.xml) 或 局部数据源(各个Web应用的配置文件)。
 *      参见 myProject.xml 文件 
 *     
 *   
 * 设置通过调试目录运行(这样可在Tomcat中边测试边开发)
 *   方法1.在 server.xml 的 <Host> 中，增加:
 *     <Context path="/myProject" docBase="D:\\workspaces\\myProject\\WebRoot" debug="0" reloadable="false" privileged="true"></Context>
 *     path 为虚拟目录，docBase 为实际目录，可设置虚拟目录为 "/", 这样就可以直接访问
 *   方法2.在 conf\Catalina\localhost 目录中新建 myProject.xml 文件( 文件名将作为Web应用的虚拟路径 )，其内容为：
 *     <Context docBase=D:\\workspaces\\myProject\\WebRoot" debug="1" privileged="true">
 *       <Resource />  --  其他的各种参数，如 配置DBCP数据源
 *     </Context> 
 *   
 * 管理
 *   负载平衡 -- 
 *   邮件服务 -- 
 *   
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * tomcat-users.xml -- 用户权限管理。Tomcat默认采用文件安全域，可修改成其他方式（具体？）保存用户名、密码
 *  设置允许通过网页方式管理( http://localhost/manager/status，默认情况下任何人都不能访问 ) -- 增加用户 admin, 并属于 admin-gui 等角色
 *    <role rolename="admin-gui"/>
 *    <role rolename="manager-gui"/>
 *    <role rolename="manager"/>
 *    <role rolename="tomcat"/>
 *    <role rolename="admin"/>
 *    <user username="admin" password="admin" roles="admin,manager,admin-gui,manager-gui"/>
 *
 * web.xml -- 核心配置文件
 *   <error-page> -- 配置特定异常情况的显示页面
 *   列出Web应用路径下所有页面，方便直接选取文件调试 -- /web-app/servlet/init-para[param-name='listings'] -- 将对应的 param-value 改为 'true'
 *     
 *   </servlet>
 *   
 *   
 *   
 * server.xml --
 *   服务的监听端口和超时等 -- /Server/Service/Connector[@port] -- 默认端口为"8080", 超时为 "20000"(即 20s)
 *   同时提供多个服务 -- 复制并修改 /Server/Service 节点
 *   connnectionTimeout -- 网络连接超时(毫秒)。设置为0表示永不超时(一般仅在调试时使用)
 *   keepAliveTimeout -- 长连接最大保持时间(毫秒)
 *   disableUploadTimeout -- 传时是否使用超时机制
 *   enableLookups -- 是否反查域名，为了提高处理能力，一般应设置为false
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * 性能优化 -- 
 *   1.修改 %TOMCAT_HOME%\bin\catalina.bat 文件。 设置 JAVA_OPTS 参数 等?
 *     -Xms<size> -- 表示JVM初始化堆的大小
 *     -Xmx<size> -- 表示JVM堆的最大值
 *     -Xmn<size> -- 
 *      注意: 当应用程序需要的内存超出堆的最大值时虚拟机就会提示内存溢出(OutOfMemoryError)，并且导致应用服务崩溃
 *      Heap Size 最大不要超过可用物理内存的80％，一般的要将-Xms和-Xmx选项设置为相同，而-Xmn为1/4的-Xmx值。
 *   2.修改 server.xml， 设置 maxThreads,  maxSpareThreads 等
 *   3.使用 APR 库(tcnative-1.dl) -- 在Tomcat中使用JNI的方式来读取文件以及进行网络传输， 可以大大提升对静态文件的处理性能
**************************************************************************************************************************************/
public class TomcatStudy {

}
