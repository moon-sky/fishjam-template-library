package com.fishjam.study;

import java.io.IOException;

import com.fishjam.utility.thread.FThread;

/************************************************************************************************************
 * JSR(Java Spec. Request)--Java 规格需求
 * 
 * 安装好JDK以后，要设置 JAVA_HOME 和 CLSS_PATH 环境变量
 *   CLASS_PATH -- 含一个或多个目录，每个目录被视为.class 文件的查找起点，在此下通过展开package名称的路径查找class。
 *     安装第三方package后需要加入CLASS_PATH。jar文件被认为是目录，也可以放于该变量中。
 * 
 * 一次编译,到处运行 -- java字节码(Code-Byte).class，在JVM虚拟机下运行
 * ClassPath 环境变量设置 java 程序查找class的路径
 * 
 * 
 * JVM的运行过程：载入类(ClassLoader)->字节码校验(ByteCodeVerifier)->解释器->运行时环境
 * JDK(Java Development Kit)按Java的应用环境划分
 *   J2EE(Java 2 Platform Enterprise Edition) -- 企业Web应用开发
 *   J2SE(Java 2 Platform Stand Edition) -- 普通应用程序
 *   J2ME(Java 2 Platform Micro Edition) -- 手机等消费电子产品
 *
 * java中的char占用两个字节(unicode编码)
 * Obfuscator(代码混淆器)--将字节码转换为一个逻辑上的对等物。常用的有：JODE、JavaGuard 等
 * JAD/FrontEnd--Java反编译工具，可以得到源代码。
 *
 * Java性能优化：
 *   1.尽量减少临时对象的使用
 *   2.对象不用时最好显式置为Null
 *   3.尽量使用StringBuffer,而不用String来累加字符串
 *   4.能用基本类型如Int,Long,就不用Integer,Long对象
 *   5.尽量少用静态对象变量--静态变量属于全局变量,不会被GC回收,它们会一直占用内存
 *   6.分散对象创建或删除的时间
 * 
 * Java中检测密码(必须有数字、大、小写、特殊符号等，长度8-30)的正则表达式:
 *   "((?=.*\\d)(?=.*[a-z])(?=.*[A-Z]).{8,30})"
 ***********************************************************************************************************/
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
public class JavaEnv {
	
	public void testEnv() throws InterruptedException
	{
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
	}
}
