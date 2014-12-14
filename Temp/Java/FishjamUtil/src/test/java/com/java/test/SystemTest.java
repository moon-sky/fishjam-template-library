package com.java.test;

import static org.junit.Assert.*;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Map;
import java.util.Properties;

import org.junit.Test;

/**************************************************************************************
* System -- 代表当前Java程序的运行平台。全局唯一，因此不能创建实例
*   访问环境变量、系统属性
*   加载文件和动态链接库
*   
* Runtime -- 代表Java程序的运行时环境，每个Java程序都有一个与之对应的实例(getRuntime() 获得)。应用程序通过该对象与其运行时环境相连。
*            load -- 加载文件
*            loadLibray -- 加载动态链接库
**************************************************************************************/

public class SystemTest {
	@Test
	public void testSystem(){
		
		//返回当前时间与 UTC 19700101 的时间差
		System.currentTimeMillis();  //毫秒方式返回
		System.nanoTime();			 //纳秒方式返回 ?
		
		{
			String str1 = new String("test hash code");
			String str2 = new String("test hash code");
			
			//String重写了hashCode方法，根据字符串值计算hash值
			assertTrue(str1.hashCode() == str2.hashCode()); 
			
			//根据对象地址计算得到的hash值，不会因重写 hashCode 而改变,因为是 new 出来的对象，肯定是不同的
			assertTrue(System.identityHashCode(str1) != System.identityHashCode(str2));	
		}

		{
			//获取系统所有的环境变量
			Map<String, String> env = System.getenv();	
			assertTrue(env.size() > 0);
			
			//获取所有的系统属性
			Properties props = System.getProperties();
			assertTrue(props.size() > 0);
			
			try {
				//将所有系统属性保存到 props.txt 文件中
				props.store(new FileOutputStream("props.txt"), "System Properties");
			} catch (FileNotFoundException e) {
				e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	
	@Test
	public void testRuntime() throws IOException{
		Runtime runtime = Runtime.getRuntime();
		assertTrue(runtime.availableProcessors() > 0);	//处理器数量
		assertTrue(runtime.freeMemory() > 0);			//空闲内存数
		assertTrue(runtime.totalMemory() > 0);			//总内存数
		assertTrue(runtime.maxMemory() > 0);			//可用最大内存数
		
		runtime.exec("notepad.exe");					//执行操作系统命令
	}
}
