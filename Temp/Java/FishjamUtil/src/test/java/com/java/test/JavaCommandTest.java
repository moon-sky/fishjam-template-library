package com.java.test;
/**
* <br/>Copyright(C), fishjam
* @author fishjam
* @version 1.0
*/

import static org.junit.Assert.*;

import org.junit.Test;

/************************************************************************************************************************
 * 运行java的方式(可制作成批处理或脚本)：
 *   1.java MyPackage.MainClass
 *   2.(Windows下)start javaw MyPackage.MainClass -- 不保留运行java程序的命令行窗口
 *   3.将应用制作成可执行的jar包(Windows下安装JRE时，会将 *.jar文件映射成由 javaw.exe 打开，双击jar包即可自行 或 java -jar myTest.jar ) 
 *      在自定义清单文件中增加如下行: Main-Class:<空格>主类<回车>(如 MyPackage.MainClass);然后在 jar 时使用 m 选项指定该文件
 *      注意：自定义清单文件的格式有严格限制：一行一个键值对，格式为 "key:<空格>value<回车>; 文件必须以一行空行结束(即最后一个键值对后必须由回车)
 *   
 * apt.exe(Annotation Processing Tool)--注释处理工具，提供了处理Annotation的框架：
 *   启动后扫描源代码中的annotation，并调用自定义的annotation处理器完成我们所要完成的工作
 * java.exe -- 运行环境， 语法为 java Java类名，注意，需要是全路径，如 packageName.className.subClassName
 *   -classpath %CLASSPATH%;目录列表 <== 运行时临时指定JRE搜索Java类的路径
 *   -verbose:gc <== 查看 GC 的提示信息, 如 [Full GC MemBeforeGC -> MemAfterC(HeapSize), elapseTime]
 * javac.exe -- 编译工具
 *   -d . <== 指定编译后按照package在当前目录下生成目录结构(推荐总是使用，因为会自动创建对应的目录结构)
 *   -Xlint <== 对代码进行检查?
 * javadoc.exe -- 将说明文档和程序代码统一维护，通过处理/xx ... x/包含的注释生成文档，由 内嵌的HTML 或 文档标签(@命令)组成
 *   javadoc -d 目的目录 [其他参数] 源文件或包
 *   包注释信息不是直接包含在Java源文件中，而是通过名为 package.html 的HTML 文件提供
 *   <br /> -- 换行
 *   @author -- 作者, 若要提取需要使用 -author 参数
 *   @deprecated -- 建议不再使用的
 *   @exception -- 抛出异常的类型
 *   @param 参数名 描述
 *   @return 方法的返回值的说明信息
 *   @see 类全名#方法名 -- 参考内容(生成超链接)
 *   @since -- 开始使用的版本
 *   @throws 可抛出的异常 描述
 *   @version -- 源文件的版本，若要提取需要使用 -version 参数
 *   -private -- 提取 private 修饰的内容，默认只处理 public/protected 修饰的内容
 *   -header <html-code> -- 指定一个HTML格式的文本，包含每个页面的页眉
 * javah.exe -- 创建 C 的头文件和Stub文件，用于编写原生代码(native methods)
 * javap.exe -- Java 类的反编译工具
 * jar.exe -- jar -cvf myPackage.jar 目录(可以使用*.class指定打包文件)
 *   c(压缩)、t(查看文件列表)、x(解压)、v(erbose)、f(指定文件名)、m(指定manifest文件)、M(不产生manifest), i(产生索引信息)
 *   O(只存储文件，不压缩，用来建立一个可置于classpath中的JAR文件--lib文件不能压缩？)
 *   C(更改为指定的目录并包含其中的文件,如果有任何目录文件，则对其进行递归处理)
 * jdb -- Java调试器
 * native2ascii.exe -- TODO: 处理包含非西欧字符的资源文件,转换成 UNICODE 编码方式 ? 
************************************************************************************************************************/

public class JavaCommandTest {

	@Test
	public void testCommand() {
		//fail("Not yet implemented");
		assertEquals(1, 1);
	}

	@Test
	public void testCommand2(){
		assertEquals(1, 1);
	}
}
