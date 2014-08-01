package com.fishjam.springtester.tools;

import org.aspectj.weaver.NewConstructorTypeMunger;

/*****************************************************************************************************************
 * 测试AOP的例子类 
 *****************************************************************************************************************/

//配置为单例Bean -- 通过静态工厂方法创建
public class MyLog {
	
	private MyLog(){
	}
	
	//出于线程安全考虑，此处使用了被称为 "initialization on demand holder"的技术来创建单例类的实例
	private static class MyLogSingletonHolder{
		static MyLog instance = new MyLog();	//延迟加载实例
	}
	public static MyLog getInstance() {
		return MyLogSingletonHolder.instance;	//每次调用时返回相同的实例
	}
	
	public void logBeforeAction(){
		System.out.println("Log before action");
	}
	
	public void logAfterAction(){
		System.out.println("Log after action");
	}
}
