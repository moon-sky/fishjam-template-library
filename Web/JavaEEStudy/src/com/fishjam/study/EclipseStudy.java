package com.fishjam.study;

/**************************************************************************************************************************************
 * MyEclipse = Eclipse + 一堆商业插件, 适用于开发 Java/J2EE 等，
 *   2014版：完整支持 AJAX,CSS,Hibernate,HTML5,Javascript,Java Servlet,JDBC,JSF,JSP,JQuery,Spring,SQL,Struts
 *           内嵌MySql, tomcat 等服务？
 *
 * 2014 的破解程序: http://filemarkets.com/fs/9luj7unx1udc2628/ ， 下载地方注册的用户名/密码: fishjam/fishjam
 *  1.安装好 MyEclipse2014 ，先不要运行程序。
 *  2.1.以管理员身份，执行破解文件夹下的 run.bat
 *  2.2.选择 Professional 版本 -> usercode 随便输入 -> 通过 SystemId 生成序列号;
 *  2.3.Tools -> rebuildKey -> Active , 生成一串数字
 *  2.4.Tools -> replaceJarFiles, 放到 %APPDATA%\MyEclipse\Common\
 *  2.5.从 MyEclipse 目录中进行添加 ?
 *  
 * http://jingyan.baidu.com/article/e4d08ffdd684640fd2f60ddd.html
 * 
 * 向导:
 *   1.Web Project
 *   2.右键 -> Add struts Capabilities, Add Hibernate Capabilities(hibernate.cfg.xml) 等
 *   3.
 *
 * 项目发布(生成 WAR 包)
 *   1.将web项目打包: Export -> JavaEE 的WAR file(myeclipse)
 *   2.将war放到tomcat的webapps目录下, 设置 server.xml 文件
 *
 * 直接连接数据库
 *   Windows->Preferences->MyEclipse->Database Explorer->Database Drivers。配置数据库驱动参数。
 *   1.Derby -- MyEclipse自带的一个关系数据库
***************************************************************************************************************************************/

/**************************************************************************************************************************************
 * 七大类特征
 *   1.JavaEE模型
 *   2.WEB开发工具
 *   3.EJB开发工具
 *   4.应用程序服务器的连接器
 *   5.JavaEE项目部署服务
 *   6.数据库服务
 *   7.MyEclipse整合帮助
 *
 * 常用快捷键和配置 -- http://baike.baidu.com/link?url=GF0-5mxd1L_V5w-wo7K8w-3YOrNER4jjkI9yaE2tbPUr7IB4TwHk4EiuDUVt5wkZ
 *   Ctrl+O 快速显示 OutLine
 *   Ctrl+T 快速显示当前类的继承结构
 *   Ctrl+E 快速显示当前Editer的下拉列表
 *   Ctrl+J 正向增量查找
 *   Ctrl+Q 定位到最后一次编辑处
 *   Ctrl+I 格式化激活的元素Format Active Elements。
 *   Ctrl+F6 切换到下一个Editor
   * 　Ctrl+F7 切换到下一个Perspective
 *   Ctrl+F8 切换到下一个View
 *   
 *   代码助手的自动提示
 1.Code Assist -- 默认是 Alt+/(在 Window->Perferences->General->Keys 中设置)，可以更改为 Ctrl+Space;
 2.Word Completion -- 
 2.Window->Perferences->Java->Editor->Content Assist 中设置 "Auto activation triggers for java"，
   可以设置为 .和a-z,A-Z的所有字母（导出配置后修改，然后再导入，可以突破早期版本只能输入4个字符的限制）;


Eclipse -- 免费开源的IDE，http://www.eclipse.org/，由IBM捐献
MyEclipse -- 插件收费（把其中的plugins拷贝到Eclipse中）
OSGI协议 -- 

开源计划(Open Source Project)
Eclipse 设计美妙之处，在于除了底层的核心以外，所有东西都是外挂，具备强大扩充性
perspective -- Eclipse 提供数群预先选定的视图，并已事先定义好的方式排列。


Eclipse 项目由Project Management Committee(PMC)所管理，Eclipse 项目分成3 个子项目：
1.平台--Platform
2.开发工具箱--Java Development Toolkit(JDT)
3.外挂开发环境--Plug-in Development Environment(PDE)

通常有四种版本：
1.Release Builds -- 主要稳定版本,经过完整测试。
2.Stable builds -- 经由Eclipse 开发团队测试，新功能通常会在此过渡版本出现。
3.Integration builds -- 各个独立的组件已经过测试，但不保证整合后的稳定。
4.Nightly builds -- 每日AutoBuild产生的。

Eclipse 平台由数种组件组成：
  平台核心(platform kernel)--管理和加载各种插件
  工作台(workbench)--图形操作接口，用Eclipse自己的标准图形工具箱(Standard Widget Toolkit-SWT)和JFace构建出来的。
    Eclipse 使用操作系统的原生图形来建置,因此要等SWT移植到该平台，Eclipse 才能在那个平台执行。
    视图(View)--呈现工作台中的信息
    编辑器(Editor)--显示文件的编辑器，根据文件类型，有不同的编辑器
    透视图(Perspective)--对工作台中一组视图的设置和布局，以完成特定类型的任务(如Java开发、Debug)
  工作区(workspace)--管理使用者的资源(被组织成一个或多个项目)，可以保存更改的历史记录
  团队组件(tram component)
  说明组件(help)

Elcipse 为了方便支持各种操作系统，在文件系统上提供了一个抽象层级：项目 > 数据夹

插件
  ADT -- Android Development Toolkit
  CDT -- C Development Toolkit
  JDT -- Java Development Toolkit
  TPTP -- Test & Performance Tools Platform，可以进行程序执行时间的统计分析、内存的监控、对象调用的分析等
  VssPlug -- 支持VSS，解压到 dropins 目录下，主菜单会多一项“VSS”，通过 "Team->Share..."进行设置,对VSS的密码明文设置在
    Workspace\.metadata\.plugins\org.vssplugin\dialog_settings.xml 中
  WTP -- Web 开发用的插件，具体名字？

启用插件的方法
  1.解压到 plugins 或 dropins(如 org.vssplugin_1.6.2) 目录下，重启；
  2.在Eclipse目录下创建一个links目录，建立*.link 的文本文件，其中指向放在任意地方的插件。
    如 path=d:\\myplugin\\eclipse\\plugins\\vssplugin_1.6.1
  如果出现问题，可以尝试将configuration目录下除 config.ini 文件外的所有文件都删除(未测试，最好先备份)。

性能测试
  JBuilder中有 Optimize Suite，可以监测运行的并发线程数据、内存的使用情况。

常用快捷键
  Ctrl+F6 -- 切换编辑器
  Code Formatter(Window->Preferences->Java->Code Formatter)
  
插件
  JET(Java Emitter Templates) -- 一个开源的模板引擎，可以在EMF中自动生成代码。
     http://www.ibm.com/developerworks/cn/opensource/os-cn-ecl-jet/?ca=drs-tp4608

***************************************************************************************************************************************/

public class EclipseStudy {

}
