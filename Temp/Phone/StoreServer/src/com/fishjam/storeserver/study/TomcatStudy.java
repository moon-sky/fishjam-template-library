package com.fishjam.storeserver.study;

/**************************************************************************************************************************************
 * TODO:
 *     Apache -- 为HTML页面服务
 *     Tomcat -- 运行JSP 页面和Servlet，是一个Servlet和JSP容器。可以处理HTML页面，但能力不如Apache
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * Tomcat -- 开源且免费的jsp服务器，在中小型系统和并发访问用户不是很多的场合下被普遍使用，是Apache 服务器的扩展
 * 
 * 使用方式(好像有好几种安装包? 如 apache-tomcat-6.0.35 中的启动是 bin\startup.bat )：
 *   1.解压 : bin\TomcatX.exe 
 *   2.安装 : 开始 ->Apache Tomcat -> Configure Tomcat -> Start
 * 
 * 设置通过调试目录运行(这样可在Tomcat中边测试边开发)
 *   1.在 server.config 的 <Host> 中，增加:
 *     <Context path="/myProject" docBase="D:\\workspaces\\myProject" reloadable="false" privileged="true"></Context>
 *     path 为虚拟目录，docBase 为实际目录
 *
 * 管理
 *   负载平衡 -- 
 *   邮件服务 -- 
 *   
 * Servlet 
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * tomcat-users.xml -- 设置允许通过网页方式管理( http://localhost/manager/status )
 *  <role rolename="admin-gui"/>
 *  <role rolename="manager-gui"/>
 *  <role rolename="manager"/>
 *  <role rolename="tomcat"/>
 *  <role rolename="admin"/>
 *  <user username="admin" password="admin" roles="admin,manager,admin-gui,manager-gui"/>
 * web.xml -- 核心配置文件
 *   
**************************************************************************************************************************************/
public class TomcatStudy {

}
