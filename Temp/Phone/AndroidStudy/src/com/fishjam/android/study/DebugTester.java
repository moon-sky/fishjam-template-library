package com.fishjam.android.study;
import android.test.AndroidTestCase;


/***************************************************************************************************************************************
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
 * 调试日志:
 *   1.util.Log 中可使用类似 Log.i(TAG, 消息); 的方式;  
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
 **************************************************************************************************************************************/

public class DebugTester  extends AndroidTestCase{

}
