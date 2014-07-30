package com.fishjam.utility.thread.tester;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;

import com.sun.xml.internal.bind.v2.runtime.reflect.opt.Const;

import junit.framework.TestCase;

public class ThreadTester extends TestCase {
	class TestRunnable implements Runnable{
		@Override
		public void run() {
			for (int i = 0; i < 5; i++) {
				System.out.println("TestRunnable " + i);
			}
			System.out.println("After all thread pool quit");
		}
	}
	
	class TestCallable implements Callable<String>
	{
		private int id;
		private String strType;
		TestCallable(String strType, int id){
			this.strType = strType;
			this.id = id;
		}
		@Override
		public String call() throws Exception {
			String strThreadName  = String.format("%s[%d] in \"%s\"",  strType, id, Thread.currentThread().getName());
			System.out.println(strType + " in " + strThreadName);
			return strThreadName;
		}
	}
	public void testExecutor(){
		
	}
	public void testThreadPool() throws Exception {
		int cpuNums = Runtime.getRuntime().availableProcessors();   //获取当前系统的CPU 数目
		//ExecutorService executorService = Executors.newCachedThreadPool();
		ExecutorService executorService = Executors.newFixedThreadPool(cpuNums * 2);
		
		List<Future<String>> resultList = new ArrayList<Future<String>>();
		
		 //创建10个任务并执行 -- TODO: 有多少个线程？
		for(int i = 0; i < 10; i++)
		{
			Future<String> futureReturn =  executorService.submit(new TestCallable("Cached", i));
			resultList.add(futureReturn);
		}
		for (Future<String> future : resultList) {
			System.out.println("Result: " + future.get());
		}

		executorService.execute(new TestRunnable());
		System.out.println("After newCachedThreadPool end");

		executorService.shutdown();
		executorService.awaitTermination(10, TimeUnit.SECONDS);
	}
}
