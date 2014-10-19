package com.fishjam.spring.test.framework;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.AbstractJUnit4SpringContextTests;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

/**************************************************************************************************************************************
* 命令行生成POM工程: mvn archetype:create -DgroupId=com.fishjam.tester -DartifactId=JavaTester, 生成如下目录结构：
*    pom.xml
*    src/main/java -- 默认的项目主代码目录，Maven会自动搜索该目录，并打包到最终构件(如 jar)中
*    src/test/java -- 默认的测试代码目录，只在运行测试时使用，不会被打包
*    target -- 默认的构建输出结果目录
*    
* Eclipse 中的插件 
*    m2eclipse -- Maven Integration for Eclipse, Maven 功能在Eclipse中的集成
*    m2e-extras -- 与其他工具(如 Subversion) 进行集成
*    
* Maven Project Wizard 
*    
* 常见错误:
*    1.Eclipse is running in a JRE, but a JDK is required Some Maven plugins may not work when importing projects or updating source folders.
*       原因：Eclipse默认是运行在JRE上的，而m2eclipse的一些功能要求使用JDK
*       解决？：配置Eclipse安装目录的eclipse.ini文件，添加vm配置指向JDK。如: -vm D:\java\jdk1.6.0_07\bin\javaw.exe
*    2.Maven 默认 Archetype 使用的 JUnit 版本比较老(3.xxx), 推荐手工更改 pom.xml，使用较新的 4.xxx 版本(如 4.7 )
*    3.TODO: 为什么有两种 jetty maven 的plugin
*           <org.mortbay.jetty> 下可以搜索到 <maven-jetty-plugin> 和 <jetty-maven-plugin>
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * 书籍：  
 *            maven权威指南
 *            Maven实战（高清完整带书签）.pdf
 *            http://www.yiibai.com/maven/
 *            http://blog.csdn.net/anialy/article/details/8226887
 *            
 * 项目管理工具的进化：make -> Ant(跨平台构建任务，过程式的) -> Maven(声明式的)
 * 
 * Maven -- 基于项目对象模型(POM, Project Object Model)的项目管理工具，可以通过一小段描述信息来管理项目的构建，报告和文档。
 *   是Ant的替代产品，可以完成程序构建，高级项目管理等，其中包括了
 *   1.一个项目生命周期(Project Lifecycle) -- 该模型吸取了大量其他的构建脚本和构建工具的优点，总结了大量项目的实际需求。
 *   2.一个依赖管理系统(Dependency Management System) -- 通过一个坐标系统(GAV -- groupId, artifactId, version)准确定位每一个构建(artifact)，并可自动从中央仓库下载依赖，
 *   3.管理原本分散在项目各个角落的项目信息，包括项目描述、开发者列表、缺陷管理系统地址等。
 *   4.用来运行定义在生命周期阶段中插件目标(goal)的逻辑 -- 这些goal完成具体的编译工作，如编译、创建JAR等
 *   
 * 相关概念
 *   artifacts -- 在maven中，把由项目生成的包都叫作artifact
 *   goal --
 *   phase -- 构建声明周期的一个阶段，如 compile、test、package 等，maven执行时会先自动执行指定阶段之前的所有阶段
 *    
 * 安装：
 *    先安装配置JDK，然后解压后设置 M2_HOME, 并将 %M2_HOME%\bin 加入 path， 验证： mvn -v
 *    可选配置 -  MAVEN_OPTS : -Xms256m -Xmx512m
 *    首次执行命令时会自动下载工具包(artifacts) -- 默认是放在 %HOMEPATH%\.m2\repository 目录下
 *    Eclipse中安装好 "Maven Integration for Eclipse" 插件后，右键项目-> Maven -> Add Dependency 可搜索并导入依赖 
 *    最佳实践：
 *       1.推荐将 %M2_HOME%\conf\settings.xml 复制到 %HOMEPATH%\.m2\settings.xml , 并进行个性化配置
 *       2.设置 MAVEN_OPTS 环境变量 -- 通常可设置为: -Xms128m -Xmx512m 来增加可用内存。
 *       3.不要使用IDE内嵌的Maven -- 保证和命令行使用的是同一个版本，避免版本不一致的问题。
 *       4.用Maven管理的项目最好都是分模块的，每个模块对应个一个pom.xml，之间通过继承和聚合相互关联。
 *         优点:
 *         缺点：导入Eclipse 后每个模块都会是一个项目，但可以利用 Working Sets 的方式来管理
 *       5.把Maven项目的目录结构转换成Eclipse工程的目录结构，方便Eclipse中开发: 
 *          项目Properties => ProjectFacets => Convert to faceted from… => 选中 "Dynamic Web Module" 和 Java
 *    
 * 命令: mvn(Windows 下是 mvn.bat)
 *    Phase(按照生命周期阶段排序)， 每个阶段对应着潜在的goals,如项目类型是 jar, package阶段将默认执行jar:jar, 如项目类型是 war， 则执行war:war
 *      validate -- 验证工程是否正确，所有需要的资源是否可用
 *      compile -- 编译项目的源代码
 *      test -- 运行应用程序中的单元测试
 *      package -- 编译打包生成可发布格式(如 jar)，一般放在 target 目录下，然后可通过如下命令运行:
 *         java -cp target\test-maven-1.0-SNAPSHOT.jar com.fishjam.app.App
 *         TODO: 通过在 MANIFEST.MF 文件中增加 Main-Class 的信息，可以生成可执行的 jar 文件(可配置 maven-shade-plugin 来完成),
 *                    然后可通过 java -jar target\test-maven-1.0-SNAPSHOT.jar 的方式运行
 *      integration-test -- 如有需要，将包处理和发布到一个能够进行集成测试的环境
 *      verify -- 运行所有检查，验证包是否有效且达到质量标准。
 *      install -- 把包安装在本地的repository中，可以被其他工程作为依赖来使用
 *        mvn install:install-file -Dfile=emayclient.jar -DgroupId=cn.emay.sdk -DartifactId=client -Dversion=1.5 -Dpackaging=jar
 *      deploy -- 在集成或者发布环境下执行，将最终版本的包拷贝到远程的repository，使得其他的开发者或者工程可以共享。
 *      site -- 生成基于pom.xml配置的项目信息，可以在target/site目录下看到生成的文档
 *      clean -- 清除先前构建的artifacts(即 删除 target 及其所有子目录内容 ?)
 * 
 *    插件和插件目标(Goal)
 *      archetype -- 工程骨架，相当于向导？有很多类型。
 *         :crawl -- 获取仓库上所有可用的archetype, 在本地生成一个 archetype-catalog.xml 文件，然后可进行裁剪，并通过 -DarchetypeCatalog=local 进行本地化定制 
 *         :create -DartifactId=test-maven -- 创建名为test-maven的Maven项目
 *         :generate --  通过命令行向导的方式，给用户列出所有可用的项目骨架, 让用户选择并输入信息生成基础代码
 *      clean:clean -- 
 *      compiler:compile -- 
 *      dependency:copy-dependencies -- 拷贝依赖
 *      eclipse:eclipse -- 生成eclipse项目文件 ???
 *      help:system -- 打印出所有的Java系统属性和环境变量
 *      install:install-file -- 安装到本地的maven仓库。 -Dfile=ojdbc14.jar -DgroupId=com.oracle
 *      jar:jar -- 
 *      jetty:run -- 启动jetty服务
 *      resources:resources -- 
 *      surefire -- 执行JUnit测试的插件 ?
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
 *        <dependencies><dependency> -- 定义依赖关系，第一次使用时会自动下载
 *        <groupId>com.fishjam.app -- 项目或者组织的唯一标志，会生成java的名称空间
 *        <modules><module> -- 定义项目中的多个模块 
 *        <name>test-maven -- 声明了一个对于用户更为友好的项目名称
 *        <packaging> -- 打包机制，如pom, [jar], war 等，当打包模式为pom时，需要通过 <modules> 定义子模块
 *        <parent> -- 项目继承，子模块(module)中通过该元素中的 GAV  指定父模块，子模块会从父模块继承一切东西，包括 groupId, version, 依赖等。
 *                           因此，一般来说，子模块只需要定义自己的 <artifactId> 和其他特殊信息即可。 <version>${project.version}</version> 表示指向POM的project/version的值
 *        <reporting> --
 *        <version>1.0-SNAPSHOT -- 当前版本，SNAPSHOT表示该项目还处于开发中，是不稳定的版本
 *          <scope> -- 指定依赖范围，如 [compile], provided(只在编译时使用), test(只对测试有效), runtime, system 等
 *          <exclusions><exclusion> -- 告诉maven只包括指定的项目，不包括相关的依赖，一般用于解决版本冲突问题
 *        <build> --  编译设置, 
 *          <plugins><plugin> -- build时执行的插件，常见的有 <maven-jetty-plugin><maven-compiler-plugin> 等
 *          <resources><resource> -- 定义项目中需要的资源，一般需要指定 <directory> + <includes> 等 
 *     settings.xml -- 可以有两个，最佳实践为：拷贝 %M2_HOME% 下的到 %HOMEPATH%\.m2\ 下，进行个性化配置
 *       %M2_HOME%\conf\settings.xml --  maven应用程序的配置文件，用于所有项目，配置mvn本地仓库、自动下载的服务器地址等信息
 *       %HOMEPATH%\.m2\setting.xml -- 用户的本地配置，可以配置一些特殊的自定义信息，如用户信息等 
 *         <localRepository> -- 本地仓库的保存位置，默认在 %HOMEPATH%\.m2\repository，可改为 D:/.m2/repository 等非系统盘
 *         <offline> -- 如果不想每次编译，都去查找远程中心库，那就设置为true，前提是已经下载了所有必须的依赖包
 *         <mirrors><mirror> -- 镜像库，通过 <id> + <mirrorOf>central</mirrorOf> +<url> 的方式指定镜像的地址
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
* 常见的<dependency>, 在pom.xml中配置，排版按照 <groupId><artifactId>[version] 的方式，这三个元素定义了一个项目基本的坐标。
*    注意：1.为了统一版本，一般可通过 <properties> 定义版本的属性字符串，然后通过 ${字符串名} 的方式引用
*    maven-shade-plugin(可用于生成可执行的jar文件) -- <org.apache.maven.plugins><maven-shade-plugin>[1.2.1]
*    服务器功能开发相关
*    	<javax.mail><mail>[1.4.4]  <== javaxmail， TODO: 需要确认，这个到底是JavaMail还是SpringMail?似乎是JavaMail
*       <commons-fileupload><commons-fileupload>[1.2.2] <== 文件上传
*    数据解析
*       <net.sf.json-lib><json-lib>[2.4] <== Json解析， 需要加上 <classifier>jdk15</classifier> ?
*       <com.belerweb><pinyin4j>[2.5.0] <== 汉字拼音
*    调试和测试：
*       <junit><junit>[4.7] <== JUnit单元测试，一般会加上 <scope>test</scope> 表示只用于 test
*       <org.springframework><spring-test>[3.0.5.RELEASE]
*       <log4j><log4j>[1.2.9] <== 
*       <org.mockito><mockito-core>[1.9.5]
*       <org.mockito><mockito-all>[1.9.5]
*    Servlet相关的: 
*       <javax.servlet><servlet-api>[2.5] <== 一般会加上 <scope>provided</scope> 表示只在编译时使用
*       <javax.servlet.jsp><jsp-api>[2.1] <== 一般会加上 <scope>provided</scope> 表示只在编译时使用
*       <javax.servlet><jstl>[1.2]  <== JStl
*    Spring开发时的核心包(各个包最好选同一个版本):
*       <org.springframework><spring-core>[3.0.5.RELEASE]
*       <org.springframework><spring-context>[3.0.5.RELEASE]
*       <org.springframework><spring-context-support>[3.0.5.RELEASE]
*       <org.springframework><spring-aop>[3.0.5.RELEASE]
*       <org.springframework><spring-aspects>[3.0.5.RELEASE]
*       <org.springframework><spring-asm>[3.0.5.RELEASE]
*       <org.springframework><spring-expression>[3.0.5.RELEASE]
*       <org.springframework><spring-beans>[3.0.5.RELEASE]
*       <org.springframework><spring-tx>[3.0.5.RELEASE]   <== 提供声明式事务配置
*       <org.springframework><spring-jdbc>[3.0.5.RELEASE]
*       <org.springframework><spring-jms>[3.0.5.RELEASE]  <== 消息驱动
*       <org.springframework><spring-orm>[3.0.5.RELEASE]
*       <org.springframework><spring-oxm>[3.0.5.RELEASE]
*       <org.springframework><spring-instrument>[3.0.5.RELEASE]
*       <org.springframework><spring-instrument-tomcat>[3.0.5.RELEASE]
*    Spring MVC框架
*       <org.springframework><spring-web>[3.0.5.RELEASE]
*       <org.springframework><spring-webmvc>[3.0.5.RELEASE]
*    Struts
*       <org.apache.struts><struts2-core>[]
*       <org.apache.struts><struts2-spring-plugin>[]]
*    数据库
*       <c3p0><c3p0>[0.9.1.2] <== 一个简单的数据库连接池
*       <mysql><mysql-connector-java>[5.1.10] <== MySQL的JDBC驱动
*       <net.sourceforge.jtds><jtds>[1.3.0] <== sql server数据库驱动
*       <com.oracle><ojdbc14>[10.2.0.4.0] <== oracle数据库驱动。TODO: maven仓库中的ojdbc下载需要oracle的官方授权，可通过本地仓库管理
*    数据库持久化层
*       <org.hibernate><hibernate-core>[] <== 
*       <org.hibernate><hibernate-ehcache>[] <== 
*       <javax.persistence><persistence-api>[1.0.2] <== 标准的数据库持久化API ?
*       <org.mybatis><mybatis>[3.2.2] <==  mybatis(ibatis) 的数据库持久化层
*       <org.mybatis><mybatis-spring>[1.2.2] <== 帮助将MyBatis代码和Spring进行无缝整合
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * 常见 plugin
 *    
**************************************************************************************************************************************/


/**************************************************************************************************************************************
 * 常见 ArcheType -- 注意：有些ArcheType比较老，对应的文件需要手工更改
 *    <org.apache.maven.archetypes><maven-archetype-webapp>[RELEASE], 一个简单的Java Web应用程序, SpringMVC
 *    
 *    
 * 开发自定义的 ArcheType(工程框架向导)
 *    
**************************************************************************************************************************************/

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations={"classpath:DemoBeans.xml"})
public class MavenTest extends AbstractJUnit4SpringContextTests {
	
	@Test
	public void testMavenDemo(){
		
	}
}

//使用Jetty插件啥子编译选项 -- 修改java后自动编译生效？还是因为 Properties -> Java Build Path -> Source -> Output folder ?
/*
<build>
<defaultGoal>package</defaultGoal>
<finalName>springtest1</finalName>
<plugins>
    <plugin>
        <groupId>org.mortbay.jetty</groupId>
        <artifactId>maven-jetty-plugin</artifactId>
        <version>6.1.25</version>
        <configuration>
            <reload>manual</reload>
            <scanIntervalSeconds>0</scanIntervalSeconds>
            <contextPath>/</contextPath>
            <!-- <webDefaultXml>src/main/resources/webdefault.xml</webDefaultXml> -->
            <connectors>
                <connector implementation="org.mortbay.jetty.nio.SelectChannelConnector">
                    <port>8080</port>
                    <maxIdleTime>60000</maxIdleTime>
                </connector>
            </connectors>
        </configuration>
    </plugin>
    <plugin>
        <groupId>org.apache.maven.plugins</groupId>
        <artifactId>maven-compiler-plugin</artifactId>
        <version>2.1</version>
        <configuration>
            <source>1.5</source>
            <target>1.5</target>
            <encoding>UTF-8</encoding>
            <!-- <failOnError>false</failOnError> -->
        </configuration>
    </plugin>
</plugins>
</build>
*/