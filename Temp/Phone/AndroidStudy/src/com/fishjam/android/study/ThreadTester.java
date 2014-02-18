package com.fishjam.android.study;
import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;

import android.os.Handler;
import android.os.Message;
import android.test.AndroidTestCase;
import android.util.Log;

/**************************************************************************************************************************************
 * Android 的View和UI组件不是线程安全的，不允许直接在线程中访问UI。一般通过 Handler 接收消息后更新
 * 
 * Thread -- 实现 Runnable 接口支持多线程，  new Thread(Runable).start();
 *   while(!Thread.currentThread().isInterrupted()) { ... } 
 *   
 *   AsyncTask<Params, Progress, Result> -- 后台的异步任务类，参数都是模版，如不需要可设置为void。
 *     http://blog.csdn.net/hitlion2008/article/details/7560878 -- 用AsyncTask到网络上下载图片，同时显示进度，下载完图片更新UI
 *     执行后台操作并将结果发送给前台线程，适用于短时间且与UI有交互的操作。
 *     注意：一个进程中所有的 AsyncTask 会通过线程池顺序执行，与主线程通过Handler交互
 *     doInBackground(xxx...params) -- 继承类中需要重载的具体工作方法，在用户调用 execute 后自动执行该方法
 *     onProgressUpdate(xxx) -- 进度更新的回调，此时可以直接更新UI，在 doInBackground 中通过调用 publishProgress 进行统治 
 *     execute(参数) -- 调用开始异步执行， 2.3以前同时最多5个线程(THREAD_POOL_EXECUTOR), 3.0以后最多1个线程(SERIAL_EXECUTOR)
 *     executeOnExecutor -- 3.0后新增函数，可提供自定义的线程池来运行和调度
 *       executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR) 等价于2.3以前的 execute()
 *
 *   Handler + Message -- 异步处理机制。Android中禁止线程中直接更新UI，因此需要通过发送 Message 到主线程中，由Handle获取到Message后更新。 
 *     使用时只需 new Handler 并重载handleMessage 方法， 并在其中 switch(Message.what) { } 处理即可( 不需要 set 到Activity等 )
 *     content--
 *     sendEmptyMessage -- 发送消息
 *     TODO: HandlerLeak -- ? 同一个线程下的handler共享一个looper对象，消息中保留了对handler的引用，只要有消息在队列中，那么handler便无法被回收
 *     
 *   Timer -- 定时器
 *     schedule(new TimerTask() { run }, 0, 1000 ); -- 周期性执行任务
 *     
 * 并发编程(java.util.concurrent)
 *   Executor框架 -- 包括线程池，Executor，Executors，CompletionService，Future，Callable等
 *   Executor -- 接口
 *   Executors -- 主要提供一些静态方法的管理类
 *     newCachedThreadPool() -- 返回一个按需创建线程(不设上限)的线程池
 *     newFixedThreadPool(n) -- 指定线程个数的[固定?]线程池
 *     newScheduledThreadPool -- 
 *     newSingleThreadExecutor() -- 返回一个只有一个后台线程在执行所提交任务的线程池(3.0以后缺省？)
 *   ExecutorService -- 通过 Executors 获得系统提供的各种线程池
 *   ThreadPoolExecutor -- 线程池？可以设置 corePoolSize/maximumPoolSize 等
 *   FutureTask
**************************************************************************************************************************************/

public class ThreadTester extends AndroidTestCase{
	private final static String TAG = ThreadTester.class.getSimpleName();
	
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
