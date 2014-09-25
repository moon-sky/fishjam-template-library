package com.fishjam.android.study;
import java.io.FileDescriptor;

import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;
import android.os.IInterface;
import android.os.Parcel;
import android.os.RemoteException;
import android.test.AndroidTestCase;
import android.test.ServiceTestCase;
import android.util.Log;

/**************************************************************************************************************************************
 * AIDL(Android Interface Definition Language) -- Android接口 定义语言。
 *   客户端和被调用实现之间是通过代理模式的轻量级实现，代理类和被代理类都实现 IBinder 接口
 *   Stub -- 存根类，通过其 asInterface 返回目标接口(通过代理模式返回Java接口的实现)
 *   Proxy -- 代理类
 * 
 * 使用RPC的流程
 *   1.使用AIDL定义接口(如 IMath)， 扩展名为 .aidl;
 *   2.使用 aidl.exe  工具产生Java接口实现代码(ADT 会自动在gen目录下生成)
 *   3.定义实现类, IMathImpl extends IMath.Stub { 实现具体的方法 }
 *   4.将接口暴露给客户端 -- Service::onBind 中返回该接口的实例
 *   5.客户端 在 ServiceConnection::onServiceConnected 中 IMath iMath = IMath.Stub.asInterface(ibinder);     
 *   AIDL的例子:   
 *     interface IMath{   //会生成 public interface IPerson extends android.os.IInterface { ... } 的接口实现基类
 *       int Add(int  a, int b);
 *     }
 *
 * 
 *   注意：
 *     1.必须导入除内建类型外的任何其他类型。
 *     2.远程调用时可能抛出 RemoteException 异常，需要处理
**************************************************************************************************************************************/
/**************************************************************************************************************************************
 * 
 * 编写Service -- 继承 Service 类，重载其生命周期中的方法，并在 AndroidManifest.xml 中通过 <service> 元素声明。
 *   Service
 *     onBind -- 必须实现，返回一个绑定的接口给Service
 *     onCreate -- 第一次创建时由系统调用
 *     onStart -- 当通过 startService 方法启动时，该方法被调用
 *     onDestroy -- 服务退出时由系统调用
 *     stopSelf -- 服务自己主动退出?
 * 使用Service (使用 Context 类的方法)
 *   startService/stopService -- 启动/停止 Service
 *   bindService/unbindService -- 绑定/解绑定 Service，一般用在远程Service调用
 *   可通过 RPC 机制来实现不同进程间 Service 的调用
 *   
 *   ServiceConnection -- 进行远程Service调用时的回调接口？其 onServiceConnected/onServiceDisconnected 可判断连接成功或断开。
 *   IBinder -- 
***************************************************************************************************************************************/

class MyService extends Service{
	private final static String TAG = MyService.class.getSimpleName();
	
	 private final IBinder mBinder = new Binder(){
		 
	 };
	 
	@Override
	public IBinder onBind(Intent intent) {
		return mBinder;
	}

	@Override
	public void onCreate() {
		Log.i(TAG, "onCreate");
		super.onCreate();
	}

	@Override
	public void onDestroy() {
		Log.i(TAG, "onDestroy");
		super.onDestroy();
	}

	@Override
	public boolean onUnbind(Intent intent) {
		Log.i(TAG, "onUnbind");
		return super.onUnbind(intent);
	}

	@Override
	public void onRebind(Intent intent) {
		Log.i(TAG, "onRebind");
		super.onRebind(intent);
	}
}


public class ServiceTester  extends ServiceTestCase<MyService> {

	public ServiceTester() {
		super(MyService.class);
	}

	public void testMyService(){
		  Intent startIntent = new Intent();
	      startIntent.setClass(getContext(), MyService.class);
	      startService(startIntent);
	      
	      MyService myService = getService();
	      assertNotNull(myService);
	      
	      IBinder serviceBinder = bindService(startIntent); 
	      assertNotNull(serviceBinder);
	}
}