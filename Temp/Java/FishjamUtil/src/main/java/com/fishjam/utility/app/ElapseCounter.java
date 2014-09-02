package com.fishjam.utility.app;

//简单的性能计数器
public class ElapseCounter {
	private long startCounter;
	private long stopCounter;
	
	public ElapseCounter(){
		startCounter = System.currentTimeMillis(); 
	}
	
	public long Stop(){
		stopCounter = System.currentTimeMillis();
		return (stopCounter - startCounter);
	}
	
	public long GetElapse(){
		long elapse = stopCounter - startCounter;
		
		return elapse;
	}
}
