package com.fishjam.android.study;
import android.Manifest;
import android.test.AndroidTestCase;
import junit.framework.Assert;

/**************************************************************************************************************************************
 * AndroidManifest.xml -- 系统清单文件，控制应用的名称、图标、访问权限等属性，也可以声明其他程序调用该程序所需的权限声明？ 
 *   也包含这个Android应用程序具有哪些Activity、Service、Provider、Receiver 等，所有组件必须在其中声明后才能使用。
 *   
 *   versionCode="1" -- 
 *   versionName="1.0" -- 
 * <application>
 *   <activity android:name=".MainActivity" android:label="@string/app_name> -- 声明Activity
 *   <provider android:name="EmployeeProvider" android:authorities="com.fishjam.android.study.Employees" /> -- 声明 ContentProvider
 *   <receiver android:name="MyReceiver"> -- 声明 BroadcastReceiver
 *   <service android:name="MyService"> -- 声明Service
 *     <intent-filter> -- 指定访问能力。action指定程序入口?; category 指定 LAUNCHER(加载程序时运行),  等
 *       <action android:name="android.intent.action.MAIN" /> -- MAIN指定该Activity是程序的入口，
 *       <category android:name="android.intent.category.LAUNCHER" /> -- LAUNCHER指定加载该应用时运行该Activity(即主Activity)
 *       <data>
 *     </intent-filter>
 *     <uses-permission xxx/> -- 声明调用该应用所需的权限，写在应用(如 Activity、Service等)的内部
 * </application>
 * <uses-sdk> -- 指定SDk的版本信息
 *   minSdkVersion -- 可以支持的最低版本等级
 *   targetSdkVersion -- ? 指定该版本即允许平台禁用不需要的兼容性代码或者能使新的功能运行在旧版本的程序里 
 * <uses-permission android:name= "android.permission.xxxx"/> -- 程序本身需要的权限, 如 READ_CONTACTS(读取联系人)
 * 
 *  
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * 权限列表( uses-permission android:name="android.permission.xxxx" )
 *   ACCESS_CHECKIN_PROPERTIES
 *   ACCESS_COARSE_LOCATION
 *   ACCESS_FINE_LOCATION
 *   ACCESS_LOCATION_EXTRA_COMMANDS
 *   ACCESS_MOCK_LOCATION
 *   ACCESS_NETWORK_STATE -- 获取网络状态信息
 *   ACCESS_SURFACE_FLINGER
 *   ACCESS_WIFI_STATE -- 获取Wi-Fi网络状态信息
 *   ACCOUNT_MANAGER
 *   AUTHENTICATE_ACCOUNTS
 *   BATTERY_STATS	-- 获取电池状态信息
 *   BIND_APPWIDGET
 *   BIND_INPUT_METHOD
 *   BLUETOOTH		-- 连接匹配的蓝牙设备
 *   BLUETOOTH_ADMIN	-- 管理(发现、删除?)匹配的蓝牙设备
 *   BRICK
 *   BROADCAST_PACKAGE_REMOVED
 *   BROADCAST_SMS	-- 广播收到短信提醒
 *   BROADCAST_STICKY
 *   BROADCAST_WAP_PUSH
 *   CALL_PHONE		-- 拨打电话
 *   CALL_PRIVILEGED
 *   CAMERA				-- 使用照相机
 *   CHANGE_COMPONENT_ENABLED_STATE
 *   CHANGE_CONFIGURATION
 *   CHANGE_NETWORK_STATE	-- 改变网络连接状态
 *   CHANGE_WIFI_MULTICAST_STATE	-- 改变Wi-Fi网络连接状态
 *   CHANGE_WIFI_STATE
 *   CLEAR_APP_CACHE
 *   CLEAR_APP_USER_DATA
 *   CONTROL_LOCATION_UPDATES
 *   DELETE_CACHE_FILES		-- 删除缓存文件
 *   DELETE_PACKAGES			-- 删除安装包
 *   DEVICE_POWER
 *   DIAGNOSTIC
 *   DISABLE_KEYGUARD
 *   DUMP
 *   EXPAND_STATUS_BAR
 *   FACTORY_TEST
 *   FLASHLIGHT			-- 访问闪光灯
 *   FORCE_BACK
 *   GET_ACCOUNTS
 *   GET_PACKAGE_SIZE
 *   GET_TASKS
 *   GLOBAL_SEARCH
 *   HARDWARE_TEST
 *   INJECT_EVENTS
 *   INSTALL_LOCATION_PROVIDER
 *   INSTALL_PACKAGES
 *   INTERNAL_SYSTEM_WINDOW
 *   MANAGE_ACCOUNTS
 *   INTERNET		-- 打开网络Socket(即访问网络?)
 *   MANAGE_APP_TOKENS
 *   MASTER_CLEAR
 *   MODIFY_AUDIO_SETTINGS	-- 修改全局声音设置
 *   MODIFY_PHONE_STATE
 *   MOUNT_FORMAT_FILESYSTEMS
 *   MOUNT_UNMOUNT_FILESYSTEMS
 *   PERSISTENT_ACTIVITY
 *   PROCESS_OUTGOING_CALLS	-- 监听、控制、取消呼出电话
 *   READ_CALENDAR
 *   READ_CONTACTS	-- 读取用户的联系人数据
 *   READ_FRAME_BUFFER
 *   READ_HISTORY_BOOKMARKS	-- 读取历史书签
 *   READ_INPUT_STATE
 *   READ_LOGS
 *   READ_OWNER_DATA	-- 读取用户数据(具体指哪些?)
 *   READ_PHONE_STATE	-- 读取电话状态
 *   READ_PHONE_SMS(在 4.4里面没有找到，已经没有了?) -- 读取短信
 *   READ_SYNC_SETTINGS
 *   READ_SYNC_STATS
 *   REBOOT	-- 重启系统
 *   RECEIVE_BOOT_COMPLETED
 *   RECEIVE_MMS	-- 接收、监控、处理彩信
 *   RECEIVE_SMS	-- 接收、监控、处理短信
 *   RECEIVE_WAP_PUSH
 *   RECORD_AUDIO	-- 录音
 *   REORDER_TASKS
 *   RESTART_PACKAGES
 *   SEND_SMS	-- 发送短信
 *   SET_ACTIVITY_WATCHER
 *   SET_ALWAYS_FINISH
 *   SET_ANIMATION_SCALE
 *   SET_DEBUG_APP
 *   SET_ORIENTATION	-- 旋转屏幕
 *   SET_PREFERRED_APPLICATIONS
 *   SET_PROCESS_LIMIT
 *   SET_TIME -- 设置时间
 *   SET_TIME_ZONE	-- 设置时区
 *   SET_WALLPAPER	-- 设置桌面壁纸 
 *   SET_WALLPAPER_HINTS
 *   SIGNAL_PERSISTENT_PROCESSES
 *   STATUS_BAR
 *   SUBSCRIBED_FEEDS_READ
 *   SUBSCRIBED_FEEDS_WRITE
 *   SYSTEM_ALERT_WINDOW
 *   UPDATE_DEVICE_STATS
 *   USE_CREDENTIALS
 *   VIBRATE	-- 控制振动器, (Vibrator) context.getSystemService(Service.VIBRATOR_SERVICE); 
 *   WAKE_LOCK
 *   WRITE_APN_SETTINGS
 *   WRITE_CALENDAR
 *   WRITE_CONTACTS	-- 写入用户联系人
 *   WRITE_EXTERNAL_STORAGE -- 写SD卡
 *   WRITE_GSERVICES
 *   WRITE_HISTORY_BOOKMARKS	-- 写历史书签
 *   WRITE_OWNER_DATA	-- 写用户数据
 *   WRITE_SECURE_SETTINGS
 *   WRITE_SETTINGS
 *   WRITE_SMS	-- 写短信
 *   WRITE_SYNC_SETTINGS
 *   
**************************************************************************************************************************************/

public class AndroidManifest  extends AndroidTestCase{
	public void testSave() throws Throwable
	{
		int i=4+8;
		Assert.assertEquals(12,i);
	}
}

