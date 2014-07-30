package com.java.tester;

import static org.junit.Assert.*;

import org.junit.Test;

/************************************************************************************************************************
 * apt.exe(Annotation Processing Tool)--注释处理工具，提供了处理Annotation的框架：
 *   启动后扫描源代码中的annotation，并调用自定义的annotation处理器完成我们所要完成的工作
 * java.exe -- 运行环境
 * javac.exe -- 编译工具
 *   -d . <== 指定编译后按照package在当前目录下生成目录结构
 *   -Xlint <== 对代码进行检查?
 * javadoc.exe -- 将说明文档和程序代码统一维护，通过处理/xx ... x/包含的注释生成文档，由 内嵌的HTML 或 文档标签(@命令)组成
 *   <br> -- 换行
 *   @author
 *   @deprecated -- 建议不再使用的
 *   @param 参数名 描述
 *   @return
 *   @see 类全名#方法名 -- 参考(生成超链接)
 *   @since -- 开始使用的版本
 *   @throws 可抛出的异常 描述
 *   @version
 *   javadoc -d 目的目录 [其他参数] 源文件
 * javah.exe -- 创建 C 的头文件和Stub文件，用于编写原生代码(native methods)
 * javap.exe -- Java 类的反编译工具
 * jar.exe -- jar -cvf myPackage.jar 目录(可以使用*.class指定打包文件)
 *   c(压缩)、t(查看文件列表)、x(解压)、v(erbose)、f(指定文件名)、m(指定manifest文件)、
 *   O(只存储文件，不压缩，用来建立一个可置于classpath中的JAR文件--lib文件不能压缩？)
 * jdb -- Java调试器
 * native2ascii.exe -- 
************************************************************************************************************************/

public class JavaCommandTester {

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
