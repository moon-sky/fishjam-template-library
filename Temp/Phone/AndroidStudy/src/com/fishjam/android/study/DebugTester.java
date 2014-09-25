package com.fishjam.android.study;
import android.test.AndroidTestCase;


/***************************************************************************************************************************************
 * 模拟器切换 纵(port)、横屏(land) -- 快捷键F12或 Ctrl+F11， TODO: 在小键盘上按数字键7也可以切换 ?
 *   默认情况下屏幕切换时程序会重启Activity，应在销毁前保存当前状态，再次Create时载入配置。通过 onConfigurationChanged 可以捕获切换消息？
 *     然后获取当前状态并处理： if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE){ }
 *     Manifest中设置Activity的属性：
 *       1. 指定横屏、竖屏(禁止切换) -- android:screenOrientation="landscape"(横屏)、"portrait" 竖屏 。
 *       2.屏幕切换时不重启Activity  --  android:configChanges="orientation|keyboardHidden"，之后只会调用onConfigurationChanged方法
 *           
 * 部署到手机 
 *   1.通过USB联机至手机
 *   2.安装Android的USB Driver -- %ANDROID_SDK%\ usb_driver
 *     不要搜索，我要自己选择要安装的驱动程序 -> 显示所有设备-> 选择 android_usb.inf 文件 -> HTC Dream Composite ADB Interface
 *     安装驱动后才能把Android手机整合成运行、调试环境
 *   3.打开手机上的USB调试(Debug)模式：应用程序->开发->USB调试，一般可以选中 "不锁定屏幕"、"USB调试"、"允许模拟位置" 等选项
 *
 * 
 * 每一个Android应用程序运行在自己的进程中,都拥有一个独立的Dalvik虚拟机实例
 *  
 * 调试日志， 通过 Logcat 进行查看
 *   1.android.util.Log 中可使用类似 Log.i(TAG, 消息); 的方式;  
 *     .v() -- ERBOSE
 *     .d() -- DEBUG
 *     .i() -- INFO
 *     .w() -- WARN
 *     .e() -- ERROR
 *   2.System.out.println("xxx"); -- 这种方式?
 *   
 * 使用模拟器测试位置服务(GPS)
 *   1.将Eclipse视图模式切换到 DDMS 模式;
 *   2.在 Emulator Control 中找到位置服务选项，提供了手动发送经纬度、GPX和KML格式数据来测试位置服务等
 *   
 * 手机上调试
 *   1.建议使用驱动精灵，自动检测手机驱动，手机连接后安装驱动
 *   2.设备管理中 -> Android Device -> Android Composite ADB Interface
 *   3.手机中打开 "USB调试", 
 *   4.Eclipse中 Window->Show View->Devices，查看是否已经连接
 *   
 **************************************************************************************************************************************/

/**************************************************************************************************************************************
 * 常见错误和分析
 *   1.启动时无法启动，显示
 *     "Unable to execute dex: java.nio.BufferOverflowException. Check the Eclipse log for stack trace."
 *     "Conversion to Dalvik format failed: Unable to execute dex: java.nio.BufferOverflowException. Check the Eclipse log for stack trace."
 *     原因：似乎是高版本的Android SDK 不再需要低版本的一些文件？
 *     解决方案：eclipse中右键 "Android Dependencies" --->Build Path--> remove it from build path
 *   2.eglSurfaceAttrib not implemented
 *     可能是模拟器未实现功能，用真机测试确认
 *   3."Hot code replace failed" -- 调试过程中编辑源码
 *     结束掉被调试的进程或断开连接？ 
 *   4.内存问题 
 *     OOM(Out Of Memory)
 *     GC_FOR_ALLOC --  发生在堆被占满不能进行内存分配时，在分配新对象之前必须进行内存回收
 *     GC_CONCURRENT-- 发生在（可能是部分的）垃圾可供回收时，通常有很多对象可以回收。
 *     GC_EXPLICIT -- 显式调用System.gc()产生的垃圾收集。
 *     GC_EXTERNAL_ALLOC -- Honeycomb及以上版本不会出现（一切都已在堆中分配）
 *     GC_HPRPF_DUMP_HEAP -- 发生在创建HPROF文件时
 **************************************************************************************************************************************/

public class DebugTester  extends AndroidTestCase{

}
