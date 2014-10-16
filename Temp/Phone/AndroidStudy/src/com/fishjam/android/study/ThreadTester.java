package com.fishjam.android.study;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.URL;
import java.net.URLConnection;
import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.Callable;
import java.util.concurrent.FutureTask;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import android.app.ProgressDialog;
import android.content.Context;
import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.test.AndroidTestCase;
import android.util.Log;

/**************************************************************************************************************************************
 * Android解决工作线程不能更新UI组件的方案：
 *   1.使用 Handler 实现线程之间的通信;
 *   2.Activity.runOnUiThread(Runnable);
 *   3.View.post(Runnable); 或 View.postDelayed(Runnable, long);
 *   4.AsyncTask<> -- 
 * 
 * Java线程池：http://www.cnblogs.com/jersey/archive/2011/03/30/2000231.html
 * 
 * Android 的View和UI组件不是线程安全的，不允许直接在线程中访问UI。一般通过 Handler 接收消息后更新
 * 
 * Thread -- 实现 Runnable 接口支持多线程，  new Thread(Runable).start();
 *   while(!Thread.currentThread().isInterrupted()) { ... } 
 *   
 *   AsyncTask<Params, Progress, Result> -- 后台的异步任务类，参数都是模版，如不需要可设置为void。
 *     http://blog.csdn.net/hitlion2008/article/details/7560878 -- 用AsyncTask到网络上下载图片，同时显示进度，下载完图片更新UI
 *     执行后台操作并将结果发送给前台线程，适用于短时间且与UI有交互的操作。
 *     规则：1.必须在 UI 线程中创建实例，调用 execute 方法启动异步执行; 2.每个 AsyncTask 只能被执行一次，多次调用将会引发异常. 
 *     注意：一个进程中所有的 AsyncTask 会通过线程池顺序执行，与主线程通过Handler交互
 *     子类需要重载的回调函数:
 *       doInBackground(xxx...params) -- 继承类中需要重载的具体工作方法，在用户调用 execute 后自动执行该方法，在其中可调用 publishProgress 通知进度更新
 *       onPreExecute() -- 初始化准备
 *       onProgressUpdate(xxx) -- 进度更新的回调，此时可以直接更新UI
 *       onPostExecute() -- doInBackground() 完成后自动调用该回调，进行结束处理
 *     execute(参数) -- 调用开始异步执行， 2.3以前同时最多5个线程(THREAD_POOL_EXECUTOR), 3.0以后最多1个线程(SERIAL_EXECUTOR)
 *     executeOnExecutor -- 3.0后新增函数，可提供自定义的线程池来运行和调度
 *       executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR) 等价于2.3以前的 execute()
 *
 *   Handler + Message -- 异步处理机制。Android中禁止线程中直接更新UI，因此需要通过发送 Message 到主线程中，由Handle获取到Message后更新。
 *     1.工作线程处理后 mHandler.sendEmptyMessage(0x1111) | sendMessageDelayed(...) 等 发送消息到 Handler; 
 *     2. mHandler = new Handler(){  @Override void handleMessage(msg){ switch(msg.what) { case 0x1111: 可直接更新UI控件 } };
 *     content--
 *     sendEmptyMessage -- 发送简单的空消息
 *     TODO: HandlerLeak -- ? 同一个线程下的handler共享一个looper对象，消息中保留了对handler的引用，只要有消息在队列中，那么handler便无法被回收
 *   工作原理： Thread ->Looper(0~1) -> MessageQueue(1) -> Handler(0~1)
 *     Looper -- 每个线程只有一个Looper，其loop方法负责读取其管理的 MessageQueue 中的消息，读到后交给发送该消息的 Handler 进行处理。
 *                    主线程由系统初始化Looper对象，创建 Handler 后即可直接使用 Looper + MessageQueue
 *                    子线程如要使用，创建并启动: Looper.prepare(); +  Looper.loop();   
 *     MessageQueue -- 先进先出的消息队列，由 Looper 管理和维护
 *     Handler -- 其作用为 把消息发送给Looper管理的MessageQueue，并负责处理Looper分给他的消息。
 *                     TODO: 在哪个线程中构造，就属于哪个线程? 但子线程中创建的Handler，其 handleMessage 为什么能直接操作UI（Toast) ?
 *     
 *   Timer + TimerTask -- 定时器
 *     schedule(new TimerTask() { run }, 0, 1000 ); -- 周期性执行任务
 *     TODO: TimerTask 的本质就是启动一条新线程?
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
 *   ThreadPoolExecutor -- 线程池？可以设置 corePoolSize/maximumPoolSize 等，其任务为 Runnable 实例
 *     execute() -- 将指定的任务放入线程池，等待执行
 *     remove() -- 移除指定任务
 *     shutdownNow() -- 
 *     awaitTermination() --
 *   FutureTask -- 可以取消的异步的计算任务，等价于可以携带结果的Runnable，三个状态：等待、运行和完成(正常结束、取消、异常)
 *     get() -- 获取结果，会阻塞直到任务转入完成状态，但可以指定超时
 *     cancel() -- 取消正在执行的任务
 *     isDone() -- 判断任务是否已经完成，之后用get会立即得到结果
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
	
	public void testThreadPool() throws InterruptedException{
		ThreadPoolExecutor executor = new ThreadPoolExecutor(1, 1, 0L,TimeUnit.MILLISECONDS, 
				new LinkedBlockingQueue<Runnable>());
		
		Callable<String> taskcCallable = new Callable<String>() {
			@Override
			public String call() throws Exception {
					Log.i(TAG, "taskcCallable running");
				return null;
			}
		};
		FutureTask<String> futureTask = new FutureTask<String>(taskcCallable);
		
		executor.execute(futureTask);
		
		executor.wait();
	}
	
	//利用 AsyncTask 进行下载的类
	class DownloadTask extends AsyncTask<URL, Integer, String>{

		ProgressDialog 	mProgressDialog;
		Context 			mContext;
		int 					mHasRead = 0;
		
		public DownloadTask(Context context){
			mContext = context;
		}
		
		@Override
		protected String doInBackground(URL... params) {	//完成实际的下载任务
			StringBuilder sBuilder = new StringBuilder();
			try {
				URLConnection conn = params[0].openConnection();
				BufferedReader bReader = new BufferedReader(new InputStreamReader(conn.getInputStream(), "utf-8" ));
				String line = null;
				while ((line = bReader.readLine()) != null) {
					sBuilder.append(line + "\n");
					mHasRead++;
					publishProgress(mHasRead);		//通知进度更新
				}
				return sBuilder.toString();
			} catch (Exception e) {
				e.printStackTrace();
			}
			return null;
		}

		@Override
		protected void onCancelled() {
			if (mProgressDialog != null) {
				mProgressDialog.dismiss();
			}
			super.onCancelled();
		}

		@Override
		protected void onPostExecute(String result) {
			mProgressDialog.dismiss();
			super.onPostExecute(result);
		}

		@Override
		protected void onPreExecute() {	//下载开始时显示一个进度条
			mProgressDialog = new ProgressDialog(mContext);
			mProgressDialog.setTitle("正在下载中...");
			mProgressDialog.setMessage("任务正在执行中，请等待");
			mProgressDialog.setCancelable(false); //不能用 "取消"按钮关闭
			mProgressDialog.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL); //设置进度条的风格
			mProgressDialog.setMax(100); 
			mProgressDialog.setIndeterminate(false); //设置对话框的进度条是否显示进度
			mProgressDialog.show();
			super.onPreExecute();
		}

		@Override
		protected void onProgressUpdate(Integer... values) {
			mProgressDialog.setProgress(values[0]);
			//super.onProgressUpdate(values);
		}
		
	}
}
