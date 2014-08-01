package com.fishjam.springtester.tools;

/*****************************************************************************************************************
 * 测试AOP的例子类 
 *****************************************************************************************************************/

public class MyLog {
	
	public void logBeforeAction(){
		System.out.println("Log before action");
	}
	
	public void logAfterAction(){
		System.out.println("Log after action");
	}
}
