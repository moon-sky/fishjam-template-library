package com.fishjam.android.study;
import java.util.Calendar;
import android.util.Log;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.test.AndroidTestCase;

/***************************************************************************************************************************************
 * os --
 *   Bundle -- 属性对，通常用于状态值保存和恢复。也可用于数据传递。
 *     new Bundle 后通过 Intent 的 putExtras 方法进行设置。接收端的 Activity.getIntent (不等于savedInstanceState).getExtras 进行获取。
 *   Context--Context可以访问Android应用环境的系统调用, 它提供了诸如资源解析, 访问数据库等，Activity、Service等都是其子类。
 *     getResources() -- 得到Resources对象，从而继续访问各种类型资源，如 r.getDimension(尺寸资源)，r.getXML(文件名), getDrawable(图片资源)
 *       引用资源的一般格式为: @[包名:]资源类型/资源名
 *     getString() -- TODO: 究竟是 Context.getString 还是 Resources.getString ?
 *   Environment
 *     getExternalStorageState -- 获取SD卡的状态，如 MEDIA_MOUNTED 
 *   Handler + Message -- (异步处理机制？) 可重载 Handler::handleMessage 方法，并在其中 switch(Message.whtat) { } 进行处理
 *     content--
 *
 * Intent(动作 + URI格式的数据) -- Android中引入的新的设计元素，不同组件之间相互导航的纽带，封装了不同组件之间导航查找的条件。
 *   应用程序可以通过它发出请求，就像是发出求助信号。应用程序可以按照相似或互补的方式进行注册，表明他们有能力或有兴趣执行各种请求或intent。
 *   主要部分： 
 *     动作(Action) -- 要完成的动作，Intent 类中预定义了大量的Action常量， 如 ACTION_CALL，ACTION_EDIT, ACTION_BATTERY_LOW 等
 *       访问系统Action: 如调用系统电话本来查找电话号码可直接使用 intent.setAction(Intent.ACTION_GET_CONTENT); intent.setType("vnd.android.cursor.item/phone"); 
 *       自定义Action: 1.定义对应的 static final 字符串; 2.在目标组件的AndroidManifest.xml对应的 <intent-filter> 中指定 action 属性
 *     数据(Data) -- 执行动作的URI和MIME类型等信息，不同的Action有不同的Data数据指定
 *     分类(Category) -- 执行Action的附加信息，如 CATEGORY_LAUNCHER 表示加载程序时该Activity出现在最上面。
 *     类型(Type),
 *     组件(Component) -- 可以通过组件名称直接指定目标
 *     扩展信息(Extra) -- 额外的附加信息，如通过Activity发送邮件时，可通过Extras属性来添加subject和body等
 *     setClass(源, 目的) -- 设置跳转的源(如 MainActivity.this)和目的class(ResultActivity.class)
 *   通过Intent可以：
 *       启动Activity( startActivity, startActivityForResult)
 *       启动Service( startService, bindService  )
 *       发起Broadcasts( sendBroadcast, sendOrderedBroadcast, sendStickyBroadcast )
 *   寻找目标组件的方法：
 *     1.通过组件名称直接指定(  intent.setComponent(new ComponetName(MyActivity.this, "com.fishjam.targetApp.MainActivity")) ) ;
 *     2.通过 AndroidManifest.xml 文件中的 <intent-filter> 指定能处理的Intent属性，系统解析后映射到对应的 Activity、IntentReceiver、Service 等。
 *       如系统中注册了多个能处理对应Intent的 <intent-filter>，则系统会提示用户选择一个组件来运行。
 *       使用时需要考虑三个属性：
 *          <action> -- 如 Intent 指定了，则IntentFilter中的必须匹配; 如 <action android:name="android.intent.action.VIEW" />
 *          <data> -- 指定要访问数据的URI(格式为: scheme://host:port/path )和MIME类型，如 <data android:scheme="http" />
 *          <category> -- 必须出现， 如 <category android:name="android.intent.category.BROWSABLE" />
 *        createChooser
 *   常见函数
 * PendingIntent -- 用于 Alarm、Notification 等中的延迟 Intent ?
 *   PendingIntent pi = PendingIntent.getBroadcast(MainActivity.this, 0, intent, 0);     
***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * BroadcastReceiver -- 广播接收器，应用程序中继承该类来实现监听和响应这些广播的Intent， 需要向系统注册(配置文件或 Context.registerReceiver )。
 *   onReceive -- 重载该方法，响应事件
 * ? 客户端通过 Context::sendBroadcast  发起一个系统级别的事件广播来传递消息
 * 系统预定义的系统广播事件:
 *   ACTION_BOOT_COMPLETED -- 系统启动完成
 *   ACTION_BATTERY_LOW -- 电量低
 *   ACTION_MEDIA_BUTTON -- 按下媒体按钮
 *   ACTION_MEDIA_EJECT -- 插入或拔出外部媒体
 *   ACTION_PACKAGE_ADDED -- 添加包
 *   ACTION_TIME_CHANGED -- 时间改变
***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * AlarmManager -- 系统级的提示服务，通过 Context.getSystemService(Context.ALARM_SERVICE) 获得
 *   setRepeating(AlarmManager.RTC_WAKEUP, time, 8*1000, pi);
 *   
 * 通告管理器(NotificationManager)--允许所有应用程序显示特定的警告信息在状态条上
 *   可以显示广播信息的内容、图标及振动等信息
 *   使用步骤：
 *     1.获得系统服务 NotificationManager -- (NotificationManager)Context.getSystemService(NOTIFICATION_SERVICE);
 *     2.实例化 Notification 并设置属性
 *        notice = new Notification(); notice.tickerText = "xxx";  notice.when = System.currentTimeMillis();
 *        Intent intent = new Intent(MainActivity.this, MainActivity.class);
 *        PendingIntent pi = PendingIntent.getActivity(MainActivity.this, 0, intent, 0); 
 *        notice.setLatestEventInfo(this, "MyTitile", "MyContent", pi); // 设置事件信息
 *     3.标示该通知的ID并发出通知
 *        nm.notify(nID, n);
 *     
 * 任务管理器(ActivityManager)--管理应用程序的活动周期，并且提供通用的导航支持（navigation backstack）
 * 
 * TODO: 反射部分?
 * 包管理器(PackageManager)--管理应用程序包。通过 getPackageManager()方法获得
 *   可以获取应用程序的信息(如名字,CacheSize、DataSize等),本质是从所有包的 AndroidManifest.xml 中解析数据?
 *   PackageItemInfo -- AndroidManifest.xml文件中所有节点的基类，有 ActivityInfo、ApplicationInfo、ServiceInfo 等多种子类
 *   示例：queryIntentActivities 可查询所有具备ACTION_MAIN和CATEGORY_LAUNCHER的Intent的应用程序，然后可让用户点击时启动应用(类似Home程序的Launcher)
***************************************************************************************************************************************/

class MyReceiver extends BroadcastReceiver{
	@Override
	public void onReceive(Context context, Intent intent) {
		Log.i("Receiver", "Receive BOOT_COMPLETED broadcast information");
	}
}

public class OsTester  extends AndroidTestCase {
	public OsTester(){
		//Intent intent = new Intent();
	}
	
	public void CheckPhoneConfig() {
		//DisplayMetrics dm = new DisplayMetrics();
		//getWindowManager().getDefaultDisplay().getMetrics(dm);
		//String strOpt = "手机屏幕分辨率为：" +  dm.widthPixels + " × " + dm.heightPixels;  //320 x 480
	}
	public void IntentTester(){
		//以下代码在源Activity的子类(MainActivity) 中执行，启动并给 ResultActivity 传递参数
		/*
		Bundle bundle = new Bundle();
		bundle.putString("username", "userNameString");
		bundle.putString("passwd", "passwdString");
		Intent intent = new Intent(MainActivity.this, ResultActivity.class);
		intent.putExtra("data", bundle);
		startActivity(intent);
		*/
		
		//以下代码在目的Activity的子类(ResultActivity) 中执行，接收传入的参数
		/*
		Intent intent = this.getIntent();
		Bundle bundle = intent.getBundleExtra("data");
		List list = new ArrayList();
		list.add(bundle.getString("username"));
		ArrayAdapter adapter = new ArrayAdapter(this, android.R.layout.simple_list_item_checked, list);
		listView.setAdapter(adapter);
		*/
	}
	
	public void CalendarTester()
	{
		 //Calendar -- 日历(TODO: 是不是UI控件 ?)
		Calendar calendar = Calendar.getInstance();
		if (calendar != null) {
			int  year 	= calendar.get(Calendar.YEAR);
			int month = calendar.get(Calendar.MONTH);
			int day 		= calendar.get(Calendar.DAY_OF_MONTH) ;
		}
		
	}
}
