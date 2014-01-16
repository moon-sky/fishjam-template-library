package com.fishjam.android.study;
import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.Executor;

import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.test.AndroidTestCase;
import android.text.StaticLayout;
import android.util.Log;

/**************************************************************************************************************************************
 * Android 的View和UI组件不是线程安全的，不允许直接在线程中访问UI。一般通过 Handler 接收消息后更新
 * 
 * Thread -- 实现 Runnable 接口支持多线程，  new Thread(Runable).start();
 *   while(!Thread.currentThread().isInterrupted()) { ... } 
 *   
 *   AsyncTask<byte[], String, String> -- 异步任务类？参数为模版形式? 可用于照相后异步保存图片 ?
 *     doInBackground(byte[]...params) -- 继承类中需要重载的具体工作方法 
 *     execute(参数) -- 调用开始异步执行
 *
 *   Handler + Message -- (异步处理机制？) 可重载 Handler::handleMessage 方法，并在其中 switch(Message.what) { } 进行处理
 *     content--
 *     sendEmptyMessage -- 发送消息
 *     TODO: HandlerLeak -- ? 同一个线程下的handler共享一个looper对象，消息中保留了对handler的引用，只要有消息在队列中，那么handler便无法被回收
 *     
 *   Timer -- 定时器
 *     schedule(new TimerTask() { run }, 0, 1000 ); -- 周期性执行任务
 *     
 * 并发编程
 *   Executor框架 -- 包括线程池，Executor，Executors，ExecutorService，CompletionService，Future，Callable等
 *   Executor -- 接口
**************************************************************************************************************************************/

public class ThreadTester extends AndroidTestCase{
	private final static String TAG = ThreadTester.class.getName();
	
	public void testHandleMessage(){
		Handler handler = new Handler(){
			@Override
			public void handleMessage(Message msg) {
				switch (msg.what) {
				case 100:
					Log.i(TAG,  "receive 100 message");  //通常可以更新UI，如 view.setBackgroundResource(xxx)
					break;
				default:
					break;
				}
				super.handleMessage(msg);
			}
		};
		handler.sendEmptyMessage(100);
	}
	
	public void testTimer(){
		new Timer().schedule(new TimerTask() {
			@Override
			public void run() {
				Log.i(TAG,  "In Timer shcedule " + new Date().toString() );
			}
		}, 0, 1000);
	}
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
