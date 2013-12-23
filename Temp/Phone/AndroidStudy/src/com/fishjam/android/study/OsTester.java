package com.fishjam.android.study;
import junit.framework.TestCase;

/***************************************************************************************************************************************
 * os --
 *   Bundle -- 属性对，通常用于状态值保存和恢复。也可用于数据传递。
 *     new Bundle 后通过 Intent 的 putExtras 方法进行设置。接收端的 Activity.getIntent (不等于savedInstanceState).getExtras 进行获取。
 *   Handler
 *   Message
 *     content--
 *   Context--Context可以控制系统调用, 它提供了诸如资源解析, 访问数据库等
 *     getResources() -- 得到Resources对象，从而继续访问各种类型资源，如 r.getDimension(尺寸资源)，r.getXML(文件名), getDrawable(图片资源)
 *       引用资源的一般格式为: @[包名:]资源类型/资源名
 *     getString() -- TODO: 究竟是 Context.getString 还是 Resources.getString ?
 *
 * intent(动作 + URI格式的数据) -- Android中引入的新的设计元素，不同组件之间相互导航的纽带，封装了不同组件之间导航查找的条件。
 *   应用程序可以通过它发出请求，就像是发出求助信号。应用程序可以按照相似或互补的方式进行注册，表明他们有能力或有兴趣执行各种请求或intent。
 *   
***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * 通告管理器(NotificationManager)--允许所有应用程序显示特定的警告信息在状态条上
 * 任务管理器(ActivityManager)--管理应用程序的活动周期，并且提供通用的导航支持（navigation backstack）
***************************************************************************************************************************************/

public class OsTester  extends TestCase {
	public OsTester(){
		
	}
	
	public void CheckPhoneConfig() {
		//DisplayMetrics dm = new DisplayMetrics();
		//getWindowManager().getDefaultDisplay().getMetrics(dm);
		//String strOpt = "手机屏幕分辨率为：" +  dm.widthPixels + " × " + dm.heightPixels;  //320 x 480
	}
}
