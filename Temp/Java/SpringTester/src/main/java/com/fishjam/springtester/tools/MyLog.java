package com.fishjam.springtester.tools;

import javax.sql.rowset.JoinRowSet;

import org.aspectj.lang.ProceedingJoinPoint;
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
	public void logElapseTime(ProceedingJoinPoint joinPoint){
		//AOP 中环绕通知 aop:around 的方法
		try {
			long start = System.currentTimeMillis();
			joinPoint.proceed();		//把控制转给被通知的方法
			long end = System.currentTimeMillis();
			System.out.println( "\"" + joinPoint.getSignature().toString() +  "\" Elapse " + (end - start) +" milliseconds");
		} catch (Throwable e) {
			e.printStackTrace();
		}
	}
	public void logAfterAction(){
		System.out.println("Log after action");
	}
	
	public void LogInfo(String strInfo){
		System.out.println(strInfo);
	}
}
