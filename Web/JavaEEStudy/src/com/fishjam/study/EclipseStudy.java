package com.fishjam.study;

/**************************************************************************************************************************************
 * Eclipse -- 免费开源的IDE，http://www.eclipse.org/，由IBM捐献
 *   Eclipse 设计美妙之处，在于除了底层的核心以外，所有东西都是外挂，具备强大扩充性
 *   perspective -- Eclipse 提供数群预先选定的视图，并已事先定义好的方式排列。
 *   
 *   Eclipse 项目由 Project Management Committee(PMC) 所管理，Eclipse 项目分成3 个子项目：
 *     1.平台--Platform
 *     2.开发工具箱--Java Development Toolkit(JDT)
 *     3.外挂开发环境--Plug-in Development Environment(PDE)
 *     
 *   通常有四种版本：
 *     1.Release Builds -- 主要稳定版本,经过完整测试。
 *     2.Stable builds -- 经由Eclipse 开发团队测试，新功能通常会在此过渡版本出现。
 *     3.Integration builds -- 各个独立的组件已经过测试，但不保证整合后的稳定。
 *     4.Nightly builds -- 每日AutoBuild产生的。
 *     
 *   Eclipse 平台由数种组件组成：
 *     平台核心(platform kernel)--管理和加载各种插件
 *     工作台(workbench)--图形操作接口，用Eclipse自己的标准图形工具箱(Standard Widget Toolkit-SWT)和JFace构建出来的。
 *        Eclipse 使用操作系统的原生图形来建置,因此要等SWT移植到该平台，Eclipse 才能在那个平台执行。
 *     视图(View)--呈现工作台中的信息
 *     编辑器(Editor)--显示文件的编辑器，根据文件类型，有不同的编辑器
 *     透视图(Perspective)--对工作台中一组视图的设置和布局，以完成特定类型的任务(如Java开发、Debug)
 *     工作区(workspace)--管理使用者的资源(被组织成一个或多个项目)，可以保存更改的历史记录
 *     团队组件(tram component)
 *     说明组件(help)
 *     
 *   Elcipse 为了方便支持各种操作系统，在文件系统上提供了一个抽象层级：项目(?) > 数据夹(?)
***************************************************************************************************************************************/

/**************************************************************************************************************************************
 * 更改默认字体 -- Preferences -> General -> Appearance -> Colors and Fonts， 更改 Basic -> Text Font 为默认(微软雅黑)
 * 更改文件默认编码为UTF-8 -- Preferences-> General -> Workspaces -> Text file encoding， 更改为 UTF-8
 *   注意：当使用 EmEditor 等工具将非UTF-8 编码的文件转换为UTF-8时，必须是不带签名的UTF-8，否则ant编译时会失败
 *   
 * 常用快捷键和配置 -- http://baike.baidu.com/link?url=GF0-5mxd1L_V5w-wo7K8w-3YOrNER4jjkI9yaE2tbPUr7IB4TwHk4EiuDUVt5wkZ
 *   Ctrl+O 快速显示 OutLine
 *   Ctrl+T 快速显示当前类的继承结构
 *   Ctrl+E 快速显示当前Editer的下拉列表
 *   Ctrl+J 正向增量查找
 *   Ctrl+Q 定位到最后一次编辑处
 *   Ctrl+I 格式化激活的元素Format Active Elements。
 *   Ctrl+F6 切换到下一个Editor(编辑器)
 *   Ctrl+F7 切换到下一个Perspective
 *   Ctrl+F8 切换到下一个View
 *   
 * 常用功能(可以设置自定义的快捷键)
 *   Code Formatter(Window->Preferences->Java->Code Formatter)
 *   Override/Implement Methods -- 重载基类的虚函数，可以更改为 Alt+Shift+O, When 为 "Editing Java Source"
 *    
 * 代码助手的自动提示
 *   1.Code Assist -- 默认是 Alt+/(在 Window->Perferences->General->Keys 中设置)，可以更改为 Ctrl+Space;
 *   2.Word Completion -- 
 *   2.Window->Perferences->Java->Editor->Content Assist 中设置 "Auto activation triggers for java"，
 *     可以设置为 .和a-z,A-Z的所有字母（导出配置后修改，然后再导入，可以突破早期版本只能输入4个字符的限制）;
 *     导出配置： File -> Export -> Perferences -> 导出后 搜索并替换 content_assist_autoactivation_triggers_java 的值
 *
***************************************************************************************************************************************/

/*************************************************************************************************************************************** 
* 安装插件的方法
*   如果插件出现问题，可以尝试将configuration目录下除 config.ini 文件外的所有文件都删除(未测试，最好先备份)。
*   1.解压到 eclipse 的 plugins + features 或 dropins (如 org.vssplugin_1.6.2) 目录下，重启；
*     容易导致混乱，难以判断具体插件的来源
*   2.扩展安装(推荐方式)
*     在Eclipse目录下创建一个 links 目录，建立 *.link 的文本文件，其中指向放在任意地方的插件。
*     如 path=d:\\myplugin\\eclipse\\plugins\\vssplugin_1.6.1
*   3.在线安装(Eclipse中: Help->Eclipse Marketplace; MyEclipse中:  )
*   
*   
* 常用插件(网站找到对应的在线安装地址，然后通过 Help->Install New Software 安装)
*   ADT -- Android Development Toolkit
*   CDT -- C Development Toolkit（http://www.eclipse.org/cdt/downloads.php）
*   JDT -- Java Development Toolkit
*   JET -- Java Emitter Templates ,一个开源的模板引擎，可以在EMF中自动生成代码。
*   Maven -- Maven Integration for Eclipse,  安装好插件以后需要让eclipse中的maven重新定位成手动安装的外部版本:
*      Window -> Preference -> Maven -> Installation -> Add -> 选中安装的外部Maven路径
*   Subversive -- Subversion (SVN) Team Provider ( http://www.eclipse.org/subversive/ )
*   TPTP -- Test & Performance Tools Platform，可以进行程序执行时间的统计分析、内存的监控、对象调用的分析等
*   VssPlug -- 支持VSS，解压到 dropins 目录下，主菜单会多一项“VSS”，通过 "Team->Share..."进行设置,对VSS的密码明文设置在
*     Workspace\.metadata\.plugins\org.vssplugin\dialog_settings.xml 中
*   WTP -- Web 开发用的插件，具体名字？
* 
***************************************************************************************************************************************/ 

/***************************************************************************************************************************************
 * MyEclipse = Eclipse + 一堆商业插件, 适用于开发 Java/J2EE 等，
 *   2014版：完整支持 AJAX,CSS,Hibernate,HTML5,Javascript,Java Servlet,JDBC,JSF,JSP,JQuery,Spring,SQL,Struts
 *           内嵌MySql, tomcat 等服务？
 * 
 * 各种可视化开发插件
 *   JSP --  Window -> Show View -> Palette
 * 
 * 项目发布(生成 WAR 包)
 *   1.将web项目打包: Export -> JavaEE 的WAR file(myeclipse)
 *   2.将war放到tomcat的webapps目录下, 设置 server.xml 文件
 *
 * 配置Web服务器(Tomcat)
 *    Window->Preferences -> MyEclipse -> Servers -> Tomcat, 
 *    
 * 直接连接数据库
 *   Windows->Preferences->MyEclipse->Database Explorer->Database Drivers。配置数据库驱动参数。
 *   1.Derby -- MyEclipse自带的一个关系数据库
 *   
 * 常用向导:
 *   1.Web工程( 有什么区别 ?)
 *      a.Web Project
 *      b.Web Service Project
 *      c.Web Fragment Project
 *      d.Dynamic Web Project -- 可以指定 Tomcat/Servlet 版本, 
 *      
 *   2.右键 -> Add struts Capabilities, Add Hibernate Capabilities(hibernate.cfg.xml) 等
 *   3.
 *   
 * MyEclipse允许在线更新：
 *   1.Window--Preferences--General-Capabilities, 选中 "Classic Update", 然后 Apply
 *   2.此后才能看到如下菜单：Help > Software Updates > Find and Install...
 *
 * 部署
 *   当 Eclipse生成、部署Web项目时，会自动将 src 路径下除 *.java 外的所有文件都复制到Web应用的 WEB-INF\classes 路径下
***************************************************************************************************************************************/

public class EclipseStudy {

}
