package com.fishjam.android.study;
import java.util.ArrayList;
import android.test.AndroidTestCase;
import android.util.Log;
import junit.framework.Assert;
import java.util.Collection;

/***************************************************************************************************************************************
 * Android语法
 *  1.Android中的命名规则采用 mXXXX 的方式（没有下划线）
 *  
 *  HashMap<String, String>
 * 
 * 数组
 *    private int[] mColors = new int[] { Color.BLACK, Color.RED, Color.BLUE,Color.GREEN, Color.MAGENTA, Color.YELLOW };
 *    ArrayAdapter -- 数组适配器
 *      String[] strs = {"a", "b", "c"}; ArrayAdapter aa = new ArrayAdapter(this, android.R.layout.simple_spinner_item, strs);
 *      spinner.setAdapter(aa); 
 *    
 * super 调用基类的同名方法，如: super.onCreate(xxxx);
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

	public void testParseDouble() throws Throwable 
	{
		double dValue = Double.parseDouble("1.05");
		assertEquals(dValue, 1.05, 0.001);
		//将Double进行四舍五入
		//NumberFormat formatter = new DecimalFormat("0.00");
	    //String s=formatter.format(num);
	}
	
	public void testTemplate(){
		Collection<String> colStrings = new ArrayList<String>();
		colStrings.add("fishjam");
		Log.i("Language", new Integer(colStrings.size()).toString());
		assertEquals(colStrings.size(), 1);
	}
}
