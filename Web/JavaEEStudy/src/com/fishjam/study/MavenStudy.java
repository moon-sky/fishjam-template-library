package com.fishjam.study;

/**************************************************************************************************************************************
 * 书籍： Maven实战
 * 
 * 项目管理工具的进化：make -> Ant(跨平台构建任务) -> Maven
 * 
 * Maven -- 基于项目对象模型(Project Object Model, POM)的项目管理工具，可以通过一小段描述信息来管理项目的构建，报告和文档。
 *   是Ant的替代产品，可以完成程序构建，高级项目管理等，其中包括了
 *   1.一个项目生命周期(Project Lifecycle)
 *   2.一个依赖管理系统(Dependency Management System) 
 *   3.用来运行定义在生命周期阶段中插件目标(goal)的逻辑 -- 这些goal完成具体的编译工作，如编译、创建JAR等
 * 
 * 解压后设置 MAVEN_HOME, 并将 %MAVEN_HOME%\bin 加入 path
 * 
 * 命令: mvn(Windows 下是 mvn.bat)
 *    -D<property>=<value> -- 运行时指定属性来覆盖生成文件中指定的属性值
 *    archetype -- create 创建Maven项目
 *    compile -- 编译源代码
 *    deploy -- 发布项目
 *    test -- 运行应用程序中的单元测试
 *    site -- 生成项目相关信息的网站
 *    clean -- 清除项目目录中的生成结果
 *    package -- 根据项目生成ar
 *    install -- 在本地Repository中安装jar
 *    eclipse:eclipse -- 生成eclipse项目文件 ???
 *    jetty:run -- 启动jetty服务
 *    tomcat:run -- 启动tomcat服务
 *    组合示例：clean package -Dmaven.test.skip=true:清除以前的包后重新打包，跳过测试类
 *  
 *   配置文件：基于 Jelly 的可执行 XML，构建规则动态，类似于编程语言
 *     maven.xml
 *     project.xml 
 *     settings.xml
 *     
 *   规则
 *     <preGoal> + <postGoal> -- 使用goal可扩展
 * 
**************************************************************************************************************************************/


public class MavenStudy {

}
