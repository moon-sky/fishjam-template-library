package com.fishjam.study;

/************************************************************************************************************
 * springMVC3学习(一)--框架搭建 http://blog.csdn.net/itmyhome1990/article/details/18311467
 * 安装好JDK以后，要设置环境变量：
 *   JAVA_HOME -- 指向 JDK 的安装目录
 *   CLASSPATH -- %JAVA_HOME%/lib/tools.jar;%JAVA_HOME%/lib/dt.jar 
 *     含一个或多个目录，每个目录被视为.class 文件的查找起点，在此下通过展开package名称的路径查找class。
 *     安装第三方package后需要加入CLASSPATH。jar文件被认为是目录，也可以放于该变量中。
 *   验证JDK环境：java -version 和 javac -version
 *   
 * 一次编译,到处运行 -- java字节码(Code-Byte).class，在JVM虚拟机下运行
 * ClassPath 环境变量设置 java 程序查找class的路径
 * 
 * Java命令行运行
 *   java -cp Xxx.jar 运行类的全路径： 如 java -cp target/myApp-1.0-SNAPSHOT.jar com.fishjam.test.App
 * 
 * JVM的运行过程：载入类(ClassLoader)->字节码校验(ByteCodeVerifier)->解释器->运行时环境
 *
 * java中的char占用两个字节(unicode编码)
 * Obfuscator(代码混淆器)--将字节码转换为一个逻辑上的对等物。常用的有：JODE、JavaGuard 等
 * JAD/FrontEnd--Java反编译工具，可以得到源代码。
 *
 *
 * Javadoc转为CHM(微软公司发布的一种帮助文档格式,需要安装 HTML Help Workshop --
 *   http://msdn.microsoft.com/en-us/library/ms669985.aspx)
 *   1.Java代码格式最好统一，如使用GBK或UTF-8，在生成Javadoc时常需要对VM options进行编码设置
 *     -locale en_US -encoding UTF-8 -charset UTF-8
 *   2.使用jd2chm工具，能够生成hhc、hhk、hhp文件，方便我们使用HHW进行编辑。可以指定HTML的目录，但增加了一个About.html，
 *     http://download.csdn.net/source/1024968 
 *   3.使用 javadoc2chm-1.1.0.7.msi，指定  index.html 的路径即可，能自动生成目录和索引
 * 
 * 在线帮助文档：
 *   http://docs.oracle.com/javaee/7/api/
 ***********************************************************************************************************/
/*
class MyStack<T>
{
	private T ob;
	public MyStack(T ob)
	{
		this.ob = ob;
	}
	@Override
	public String toString() 
	{
		return ob.getClass().getName() + ":" + ob.toString();
	}
}

class MyThread extends FThread
{
    @Override
    public void run()
    {
        int count = 10;
        while(count >0 && this.getWaitType()==ThreadWaitType.twtContinue)
        {
            System.out.println("Count=" + count);
            count--;
            this.sleepAndCheckStop(1000);
        }
    }
}
*/
public class JavaEnv {
	
	public void testEnv() throws InterruptedException
	{
/*		
		MyStack<Integer> iStack = new MyStack<Integer>(10);
		System.out.println(iStack.toString());
		
		MyStack<String> sStack = new MyStack<String>("hello world");
		System.out.println(sStack.toString());
		
		//MyStack<Object> oStack = new MyStack<String>("必须使用通配符泛型");
		MyStack<?> oStack = new MyStack<String>("必须使用通配符泛型");
		System.out.println(oStack.toString());
		
		MyThread myThread = new MyThread();
		myThread.start();
		
		Thread.sleep(1000);
		myThread.pause();
		Thread.sleep(1000);
		myThread.resume();
		
		Thread.sleep(2000);
		myThread.stop();
		
		myThread.join();
		System.out.println("after thread end");
*/
	}
}
