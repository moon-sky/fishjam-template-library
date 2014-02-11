package com.fishjam.android.study;
import java.util.ArrayList;
import java.util.Date;

import android.R.string;
import android.test.AndroidTestCase;
import android.util.FloatMath;
import android.util.Log;
import junit.framework.Assert;
import java.util.Collection;

/***************************************************************************************************************************************
 * Android 的条件编译
 *   Java 语言本身没有引入条件编译的功能，根据Java编译器的优化和布尔常量(final Boolean)的机制，对于条件表达式中永远为false的语句，
 *   编译器将部队条件覆盖的代码段生成字节码(不编译进结果文件) 。如：
 *      final boolean bDebug = false;
 *      if(bDebug) { xxxxx -- 编译时会被优化掉 }
 *   可以定义一个纯数据的配置类来控制，如：
 *      public class AppConfig { public static final boolean bDebug = true; .... }
 *      然后在代码中直接使用这些 final 变量 -- if(AppConfig.bDebug) { ... }
 *
 *  TODO:使用类似方法，可以使用其他API读取配置来条件编译？ 
 *     系统会生成一个 BuildConfig 类，其中有 public final static boolean DEBUG = true;
 *     
 *   system.prop(Android源码编译时使用？) -- 通过 Key=Value 键值对 的方式配置系统属性，
 *   通过 android.os.SystemProperties 类读取其中内容，可实现根据配置文件来条件编译。
 *   注意：SystemProperties 是非标准的SDK接口，如要使用，在 Android.mk 文件中不能定义 LOCAL_SDK_VERION 变量(该变量表示应用只能使用标准的SDK接口)
 *     如：./build/target/board/generic/system.prop 文件(编译时，编译脚本会根据该文件生成  build.prop 文件)
 *     private static final boolean bSupportBluetooth = SystemProperties.getBoolean("ro.Gallery.bSupportBluetooth", false);
 *     if(bSupportBluetooth) { ... } 
***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * Android语法
 *  1.Android中的命名规则采用 mXXXX 的方式（没有下划线）
 *  
 *  HashMap<String, String> -- 不同步、空键值、效率高
 *  HashTable -- 同步、非空键值、效率略低
 * 
 * 数组
 *    private int[] mColors = new int[] { Color.BLACK, Color.RED, Color.BLUE,Color.GREEN, Color.MAGENTA, Color.YELLOW };
 *    ArrayAdapter -- 数组适配器
 *      String[] strs = {"a", "b", "c"}; ArrayAdapter aa = new ArrayAdapter(this, android.R.layout.simple_spinner_item, strs);
 *      spinner.setAdapter(aa); 
 *    
 * super 调用基类的同名方法，如: super.onCreate(xxxx);
 *
 * annotation
 * 
 * Android 使用Java中的事件处理机制，包括( TODO ):
 *   事件 -- (extends EventObject), 通过 FireEvent 激发事件 
 *   事件源
 *   事件监听器 -- 从EventListener 继承来定义监听接口，具体的监听器 implements 该接口  
 *     Android系统中常见的事件监听器：
 *       View 中的 OnClickListener, OnFocusChangeListener, OnKeyListener, OnTouchListener, OnCheckedChangeListener 等 
 **************************************************************************************************************************************/

/**************************************************************************************************************************************
 *  Android 包含一套C/C++库，可以被Android系统的各种组件使用，这些特性通过Android 应用程序框架开放给开发者，下面列出了核心库。
 *    系统C函数库 从BSD标准C系统库继承的，基于嵌入式linux设备进行优化的版本。
 *    多媒体库 -- 基于PacketVideo公司的OpenCORE库，该库支持录音回放，录制流行的声音和视频格式文件，以及静态图像文件，这些文件格式包括MPEG4,H.264,MP3,AAC,AMR,JPG,和PNG.
 *    界面管理(Surface Manager) -- 管理对显示子系统的访问，并且无缝合成不同的应用程序中的2D和3D图像层
 *    LibWebCord – 一个流行的网页浏览器引擎，增强Android浏览器和嵌入式网页浏览的能力
 *    SGL – 底层的2D 图像引擎
 *    3D 库 – 一个基于OpenGL ES 1.0 APIs的版本，这个库既可以使用硬件3D加速（硬件支持）或者内置的，高度优化的3D软加速
 *    字体类型(FreeType) -- 位图(bitmap)或者矢量(vector)字体
 *    SQLite – 一个强大的轻量级关系数据库引擎，允许所有的应用程序使用
 *    其他(?): 蓝牙(Bluetooth), EDGE, 3G, WiFi(依赖于硬件)，照相机, GPS, 指南针, 加速度计
 *    
**************************************************************************************************************************************/

public class LanguageTester  extends AndroidTestCase{
	private static final String TAG = LanguageTester.class.getName();
	
	public void testParseDouble() throws Throwable 
	{
		double dValue = Double.parseDouble("1.05");
		assertEquals(dValue, 1.05, 0.001);
		//将Double进行四舍五入
		//NumberFormat formatter = new DecimalFormat("0.00");
	    //String s=formatter.format(num);
		
		assertEquals(FloatMath.sqrt(100), 10.0);  //平方根
	}
	
	public void testTemplate(){
		Collection<String> colStrings = new ArrayList<String>();
		colStrings.add("fishjam");
		Log.i("Language", new Integer(colStrings.size()).toString());
		assertEquals(colStrings.size(), 1);
	}
	
	public void testDateTime(){
		Log.i(TAG, new Date().toString());
	}
}
