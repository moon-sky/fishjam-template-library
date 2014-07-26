package com.fishjam.study;

/**************************************************************************************************************************************
* 命令行生成POM工程: mvn archetype:create -DgroupId=com.fishjam.tester -DartifactId=JavaTester
* 
* Eclipse 中的插件 -- m2eclipse
* 
* 安装:
*    0. 安装配置JDK
*    1.下载解压后，设置 M2_HOME 环境变量; 
*    2.将 %M2_HOME%\bin 加入 %PATH%; 
*    3.命令行下使用 mvn Xxx 即可
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * 书籍： Maven实战.pdf -- P17
 *            http://www.yiibai.com/maven/
 *            http://blog.csdn.net/anialy/article/details/8226887
 *            
 * 项目管理工具的进化：make -> Ant(跨平台构建任务，过程式的) -> Maven(声明式的)
 * 
 * Maven -- 基于项目对象模型(Project Object Model, POM)的项目管理工具，可以通过一小段描述信息来管理项目的构建，报告和文档。
 *   是Ant的替代产品，可以完成程序构建，高级项目管理等，其中包括了
 *   1.一个项目生命周期(Project Lifecycle) -- 该模型吸取了大量其他的构建脚本和构建工具的优点，总结了大量项目的实际需求。
 *   2.一个依赖管理系统(Dependency Management System) -- 通过一个坐标系统准确定位每一个构建(artifact)，并可自动从中央仓库下载依赖，
 *   3.管理原本分散在项目各个角落的项目信息，包括项目描述、开发者列表、缺陷管理系统地址等。
 *   4.用来运行定义在生命周期阶段中插件目标(goal)的逻辑 -- 这些goal完成具体的编译工作，如编译、创建JAR等
 *   
 * 相关概念
 *   artifacts -- 在maven中，把由项目生成的包都叫作artifact
 *   goal --
 *   phase -- 构建声明周期的一个阶段，如 compile、test、package 等，maven执行时会先自动执行指定阶段之前的所有阶段
 *    
 * 安装：
 *    解压后设置 M2_HOME, 并将 %M2_HOME%\bin 加入 path， 验证： mvn -v
 *    可选配置 -  MAVEN_OPTS : -Xms256m -Xmx512m
 *    首次执行命令时会自动下载工具包(artifacts) -- 默认是放在 %HOMEPATH%\.m2\repository 目录下
 *    Eclipse中安装好 "Maven Integration for Eclipse" 插件后，右键项目-> Maven -> Add Dependency 可搜索并导入依赖 
 *    
 * 命令: mvn(Windows 下是 mvn.bat)
 *    Phase(按照生命周期阶段排序)， 每个阶段对应着潜在的goals,如项目类型是 jar, package阶段将默认执行jar:jar, 如项目类型是 war， 则执行war:war
 *      validate -- 验证工程是否正确，所有需要的资源是否可用
 *      compile -- 编译项目的源代码
 *      test -- 运行应用程序中的单元测试
 *      package -- 编译打包生成可发布格式(如 jar)，一般放在 target 目录下，然后可通过如下命令运行:
 *         java -cp target\test-maven-1.0-SNAPSHOT.jar com.fishjam.app.App
 *      integration-test -- 如有需要，将包处理和发布到一个能够进行集成测试的环境
 *      verify -- 运行所有检查，验证包是否有效且达到质量标准。
 *      install -- 把包安装在本地的repository中，可以被其他工程作为依赖来使用
 *      deploy -- 在集成或者发布环境下执行，将最终版本的包拷贝到远程的repository，使得其他的开发者或者工程可以共享。
 *      clean -- 清除先前构建的artifacts(即 删除 target 及其所有子目录内容 ?)
 *      site -- 生成基于pom.xml配置的项目信息，可以在target/site目录下看到生成的文档
 * 
 *    插件和插件目标(Goal)
 *      archetype:create -DartifactId=test-maven -- 创建名为test-maven的Maven项目
 *      dependency:copy-dependencies -- 拷贝依赖
 *      eclipse:eclipse -- 生成eclipse项目文件 ???
 *      help:system -- 
 *      install:install-file -- 安装
 *      jetty:run -- 启动jetty服务
 *      tomcat:run -- 启动tomcat服务( tomcat6:run ?)
 *    其他
 *      -version -- 显示版本、路径、Java 等信息
 *    参数说明 -- -D<property>=<value> -- 运行时指定属性来覆盖生成文件中指定的属性值
 *      -DartifactId=test-maven -- 生成 test-maven 的目录，里面会自动生成 src\main\* + src\test\* + pom.xml 的标准项目结构
 *      -DgroupId=com.fishjam.app  -- 会自动生成 src\main\java\com\fishjam\app\  的结构( 里面有 App.java 文件 )

 *    组合示例：clean package -Dmaven.test.skip=true:清除以前的包后重新打包，跳过测试类
 *  
 * 配置文件：基于 Jelly 的可执行 XML，构建规则动态，类似于编程语言，可以管理项目的整个生命周期，包括编译、构建、测试、发布等。
 *     pom.xml -- 项目对象模型配置文件
 *        <artifactId>test-maven -- 定义了当前Maven项目在组中唯一的ID，一般是各个子模块的名字
 *        <build> --  编译设置
 *        <dependencies><dependency> -- 定义依赖关系，第一次使用时会自动下载
 *        <groupId>com.fishjam.app -- 项目或者组织的唯一标志，会生成java的名称空间
 *        <modules><module> -- 定义项目中的多个模块 
 *        <name>test-maven -- 声明了一个对于用户更为友好的项目名称
 *        <packaging> -- 打包机制，如pom, jar, war 等
 *        <parent> -- 项目继承
 *        <plugins><plugin> -- build时执行的插件
 *        <reporting> --
 *        <resources><resource> -- 定义项目中需要的资源 
 *        <version>1.0-SNAPSHOT -- 当前版本，SNAPSHOT表示该项目还处于开发中，是不稳定的版本
 *          <scope> -- 指定依赖范围，如 [compile], test, runtime, system 等
 *          <exclusions><exclusion> -- 告诉maven只包括指定的项目，不包括相关的依赖，一般用于解决版本冲突问题
 *     settings.xml -- 有两个
 *       %M2_HOME%\conf\settings.xml --  maven应用程序的配置文件，用于所有项目，配置mvn本地仓库、自动下载的服务器地址等信息
 *       %HOMEPATH%\.m2\setting.xml -- 用户的本地配置，可以配置一些特殊的自定义信息，如用户信息等 
 *         <localRepository> -- 本地库的保存位置，默认在 %HOMEPATH%\.m2\repository
 *         <offline> -- 如果不想每次编译，都去查找远程中心库，那就设置为true，前提是已经下载了所有必须的依赖包
 *         <mirrors><mirror> -- 镜像库
 *         <proxies><proxy> -- 指定代理
 *         <repositories><repository> -- 开发团队自己的开发库和插件库信息
 *         <servers><server> --  
 *     m2.conf -- classworlds配置文件, 配置plexus(类加载器框架 ?比默认的java类加载器有更丰富的语法?) 
 *     
 * 规则
 *     <preGoal> + <postGoal> -- 使用goal可扩展
 *     
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * 中央仓库的配置 -- 可通过 http://search.maven.org/ 等搜索并手工下载对应的 .pom 和 .jar
 *    默认 -- http://repo.maven.apache.org/maven2/ 
 *               http://repo1.maven.org/maven2/
 *    使用本地仓库代替远程仓库
 *       1.下载 maven-proxy ?
 *       2.可以使用Nexus(仓库管理器软件) 建立私服?
 * 
 * 插件(工具包 ?artifacts) -- 有着相同目的的goals命令的集合
 *    使用方式为 “插件名:命令名 -D参数"
 *    archetype --   
**************************************************************************************************************************************/

/**************************************************************************************************************************************
* 常见的<dependency>, 在pom.xml中配置，排版按照 <groupId><artifactId>[version] 的方式
*    MySQL的JDBC驱动 -- <mysql><mysql-connector-java>[5.1.10]
*    JMail -- <javax.mail><mail>[1.4.4]  <== TODO: 需要确认，这个到底是JavaMail还是SpringMail?似乎是JavaMail
**************************************************************************************************************************************/

public class MavenStudy {

}
