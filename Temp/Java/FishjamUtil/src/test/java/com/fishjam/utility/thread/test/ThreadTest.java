package com.fishjam.utility.thread.test;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;

//import com.sun.xml.internal.bind.v2.runtime.reflect.opt.Const;

import junit.framework.TestCase;


/**********************************************************************************************************************
 * Java中实现多线程有两种途径：继承Thread类或者实现Runnable接口
 * 使用 synchronized 关键字同步，其关键是多个线程对象竞争同一个共享资源
 *   1.将函数声明为 synchronized,同步方法不会继承(父类是，子类不会自动是)
 *   2.使用特定变量，对指定代码块同步(关键区)
 * 
 * wait() 必须在synchronized 函数或者代码块里面,会让已经获得synchronized 函数或者
 *   代码块控制权的Thread暂时休息，并且自动释放synchronized锁，其他同步函数/block可被调用
 *     
 * 可能抛出 IllegalMonitorStateException 异常--只能对自己拥有锁的对象调用wait/notify 
 * 
 * daemon线程 -- 在背景提供通用性服务的线程，使用setDaemon切换。主线程结束后，所有的
 *   daemon线程会自动强制终止并结束程序。而如果有非daemon线程，则程序会等待其结束。
 * 
 * [Thread]
 *   currentThread() -- 获取当前线程的实例，然后可通过 getName() 等方法获取线程名
 *   sleep(毫秒数) -- 休眠指定的时间
 * [ThreadGroup] -- 线程组，所有的线程都隶属于某个线程组，缺省为系统线程组
 *********************************************************************************************************************/

/*********************************************************************************************************************
 * 任务(Task)  -- 
 *    Callable 接口 -- 以泛型的方式，通过 Future 支持任意类型的返回值，可抛出异常由调用者处理
 *    Runnable接口 -- 最原始的运行接口，无返回值，无法抛出经过检查的异常
 * 
 * 线程池 
 *    [Executors] -- 通过其静态方法返回线程池工厂的实例
 *       newCachedThreadPool -- 
 *       newFixedThreadPool(n) -- 
 *       newSingleThreadExecutor -- 
 *    当将一个任务添加到线程池中的时候，线程池会为每个任务创建一个线程(每个任务都会有一个线程？)，该线程会在之后的某个时刻自动执行。
 *    
 *    <Callable> 
 *    <ThreadFactory>
 *    <Executor>
 *    <ExecutorService> -- 线程池管理工具的接口，负责任务的管理，通过工具类Executors的静态方法来创建实例
 *         execute -- 最基本的执行功能, 无返回值。不进行异常处理
 *         submit --  扩展了 execute，通过创建和返回Future实例，用户可以取消或等待执行完成，会捕捉异常并在调用Future.get 时再抛出?
 *      +- <ScheduledExecutorService>
 *      +- [ThreadPoolExecutor]
 *     
 *********************************************************************************************************************/

/***************************************************************************************************
 * 常见类
 *   CountDownLatch -- 一个同步辅助类，在完成一组正在其他线程中执行的操作之前，它允许一个或多个线程一直等待
 *     通常可用来保证所有线程都完成主线程才退出 , 子线程结束时 countDown, 主线程 await,
 *
 ***************************************************************************************************/


public class ThreadTest extends TestCase {
	class TestRunnable implements Runnable{
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
