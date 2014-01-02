package com.fishjam.android.study;
import junit.framework.TestCase;

/**************************************************************************************************************************************
 * AndroidManifest.xml -- 包含这个Android应用程序具有哪些Activity、Service或者Receiver，也有权限设置。所有组件必须在其中声明后才能使用。
 * <application>
 *   <activity android:name=".MainActivity" android:label="@string/app_name> -- 声明Activity
 *   <provider android:name="EmployeeProvider" android:authorities="com.fishjam.android.study.Employees" /> -- 声明 ContentProvider
 *   <receiver android:name="MyReceiver"> -- 声明 BroadcastReceiver
 *   <service android:name="MyService"> -- 声明Service
 *     <intent-filter> -- 指定访问能力。action指定程序入口?; category 指定 LAUNCHER(加载程序时运行),  等
 *       <action>
 *       <category>
 *       <data>
 * <uses-sdk> -- 指定SDk的版本信息，如 minSdkVersion  
 * 
 * 
 *  
**************************************************************************************************************************************/

public class AndroidManifest  extends TestCase{

}
