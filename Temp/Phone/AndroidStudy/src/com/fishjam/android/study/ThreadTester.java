package com.fishjam.android.study;
import java.util.concurrent.Executor;

import android.os.AsyncTask;
import android.test.AndroidTestCase;

/**************************************************************************************************************************************
 * Thread -- 实现 Runnable 接口支持多线程，  new Thread(Runable).start();
 *   while(!Thread.currentThread().isInterrupted()) { ... } 
 *   
 *   AsyncTask<byte[], String, String> -- 异步任务类？参数为模版形式? 可用于照相后异步保存图片 ?
 *     doInBackground(byte[]...params) -- 继承类中需要重载的具体工作方法 
 *     execute(参数) -- 调用开始异步执行
 *     
 * 并发编程
 *   Executor框架 -- 包括线程池，Executor，Executors，ExecutorService，CompletionService，Future，Callable等
 *   Executor -- 接口
**************************************************************************************************************************************/

public class ThreadTester extends AndroidTestCase{
	
	public void testExecutor(){

		//Executor.execute(Runnalbe) 。Executor在执行时使用内部的线程池完成操作。
	}
	public void ThreadDemo(){
		//线程后台执行，前台显示进度对话框
		/**************************************************************************************************************************************
		 m_myDialog = ProgressDialog.show(EX03_18.this,strDialogTitle,strDialogBody, true);
		 new Thread()
		  {
		    public void run()
		    {
		      try
		      {
		        sleep(3000); //后台运行的程序片段，此处以暂停3秒作为示范
		      }
		      catch(Exception e)
		      {
		        e.printStackTrace();
		      }
		      finally
		      {
		         myDialog.dismiss(); //卸载所创建的m_myDialog对象
		      }
		    }
		  }.start();//开始运行线程
		//**************************************************************************************************************************************/
	}
}
