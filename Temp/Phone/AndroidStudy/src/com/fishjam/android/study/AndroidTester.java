package com.fishjam.android.study;

import com.fishjam.util.CallbackTesterActivity;

import android.app.Activity;
import android.content.Intent;
import android.test.ActivityUnitTestCase;
import android.test.AndroidTestCase;
import android.util.Log;
import android.view.LayoutInflater;


/***************************************************************************************************************************************
 * 单元测试 --  http://tech.ddvip.com/2012-12/1356946984188084.html
 *   Android测试项目也是一个Android应用项目， 其基本使用方法和开发一般的Android应用非常类似。
 *   
 *   Android Testing Framework -- 可用来测试Android的各个方面，单元测试、框架测试、UI测试等。
 *   Android JUint 扩展 -- 提供了对Android特定组件（如Activity，Service）的测试支持
 *   
 * 自动测试
 *   SDK提供了一个monkeyrunner(一个python应用)，可以模拟用户按键事件来测试UI。
 *   可以编写Python程序来安装应用或测试包、运行、模拟发送按键消息，截屏等。
 *   包：
 *     MonkeyRunner -- 提供应用的辅助方法以及用来链接设备或是模拟器的方法，并提供UI支持等(注意时 Python 文件)
 *     MonkeyDevice -- 代表一个设备或是模拟器，提供安装，卸载应用的方法，启动一个Activity，发送按键或是Touch 事件等。
 *     MonkeyImage -- 代表一个截屏图像，可以截取不同格式的图像，比较两个MonkeyImage图像，保存图像等
 *        device = MonkeyRunner.waitForConnection() -- 连接到设备
 *        device.press('KEYCODE_MENU','DOWN_AND_UP')  -- 模拟按键
 *        device.takeSnapshot() -- 截图
 *        
 *   随机测试
 *      adb shell monkey -p com.fishjam.android.study  -v 500  -- 使用缺省配置，想应用发送500个随机事件(包括按键、touch事件、系统事件等)
 *      Monkey选项：
 *        1.基本配置 -- 如设置尝试的事件数量
 *        2.运行约束 -- 如设置只对单独的一个包进行测试
 *        3.事件类型和频率
 *           –throttle <milliseconds> -- 在事件之间插入固定延迟，通过该选项可以减缓 Monkey 的执行速度
 *        4.调试选项
***************************************************************************************************************************************/

/****************************************************************************************************************************************
 * 1.创建工程对应的Test工程( 如 ApiDemos 对应的 ApiDemos.tests)
 * 2.Java Build Path-> Projects 中引用对应的待测试工程(在Test项目中即可引用其中的类)
 * 3.增加一个 AllTests.java 文件，其内容为(参见 ApiDemos\tests 中的源码) -- 似乎是可选的？有什么用？
 *   public class AllTests extends TestSuite {
 *      public static Test suite() {
 *        // 指明所有该包和其子包中定义的TestCase都为最终TestSuite 的一部分
 *        return new TestSuiteBuilder(AllTests.class).includeAllPackagesUnderHere().build();
 *      }
 *   }
 *
 * 
 *   1.Menifest.xml 中加入:
 *     a.<application>中加入: 
 *       <uses-library android:name="android.test.runner" />
 *     b.<application>外加入:
 *       <uses-permission android:name="android.permission.RUN_INSTRUMENTATION" />
*        <instrumentation android:name="android.test.InstrumentationTestRunner" android:targetPackage="com.fishjam.android.study" android:label="Test for Android Study"/>
 *  2.编写单元测试代码，从 android.test.AndroidTestCase, ActivityUnitTestCase, ActivityInstrumentationTestCase2 等继承，
 *     可重载 setUp, tearDown 等函数，进行必要的初始化或资源释放
 *  3.在 public void testXxx() throws Throwable{ } 中通过 junit.framework.Assert 等进行断言判断
 *    JUnit 把以 test 开头的方法作为一个实例，也可以使用 annotation @Test 表示一个方法为测试方法。
 *  4.运行
 *    a. eclipse中 -- 右键 Run as "Android JUnit Test"
 *    b. adb shell am instrument -w com.fishjam.android.study/android.test.InstrumentationTestRunner
 * 
 * Activity Testing -- 侧重于Activity的测试，使用 Instrumentation 在正常Activity生命周期之外来控制Activity，然后测试Activity的特定的功能
 * Content Provider Testing
 * Service Testing
 **************************************************************************************************************************************/

/***************************************************************************************************************************************
 * 测试相关的包和类
 *   android.test -- 用于编写Android测试用例
 *   android.test.mock -- 定义了方便测试用的测试"桩"类, 如 MockApplication, MockContentProvider, MockContext, MockCursor 等 
 *   android.test.suitebuilder -- 运行测试用例的Test Runner类
 * 
 *  Instrumentation -- 提供了一些"钩子"方法连接到Android操作系统中，可独立控制Android组件(Activity,Service等)的生命周期,
 *    并可以控制Android如何调用一个应用。使用Instrumatation API时你可以直接调用 onCreate, onResume 等生存期 方法。
 *    也可以支持强制某个应用和另一个已经在运作的应用运行在同一个进程中(在测试代码中可以直接调用被测试应用的方法和访问其成员)，这在通常的情况下是不可能实现的。
 *  InstrumentationTestRunner 
 *   
 *  TestCase  -- JUnit 中的基础Case基类，可用于编写一些和平台无关的测试用例
 *   +-InstrumentationTestCase -- 提供了Instrumentation接口给子类，能直接控制Activity的生命周期，允许创建一些Mock对象来帮助测试，可向UI发送按键和触摸事件来模拟用户界面交互。
 *                                               如 getInstrumentation().callActivityOnStart(xxx);
 *      +-ActivityTestCase
 *         +-ActivityUnitTestCase<MyActivity> -- 通常用来测试单独Activity(业务逻辑等)，不运行在一般应用运行的环境中(没有UI交互)也不和其他Activity产生交互。
 *            在启动被测试的Activity之前，你可以Inject一个假的Context(setActivityContext)或是Application(setApplication)
 *            getActivity() -- 调用该方法后，被测试的Activity才会启动。
 *            使用方式：
 *               1.从ActivityUnitTestCase继承，指定Activity的模版参数; 设置无参构造，实现为 super(MyActivity.class);
 *               2. setUp() 中  mStartIntent = new Intent(Intent.ACTION_MAIN);
 *               3. testXxx() 函数中，一开始需要调用 startActivity(mStartIntent, null, null); 然后才能调用 getActivity() 获得实例，否则将会返回 null
 *               4. 然后即可使用 getActivity().findViewById(xxx) 等方法获取其他的元素进行测试
 *         +-ActivityInstrumentationTestCase2 -- 通常用于多个Activity的功能测试，使用正常的系统框架来运行Activity, 
 *               1.可以直接 getActivity 获取实例(会自动创建)
 *               2.可以使用 a.sendKeys(xxxx); b.getActivity().runOnUiThread(new Runnable() {xxxx});  getInstrumentation().waitForIdleSync(); 等方法进行UI类型的交互？
 *            使用方式：
 *               1.无参构造中 super("packagepath", MyActivity.class>);
 *               2.setUp 中可以直接 getActivity() 获取到实例
 *      +-SingleLaunchActivityTestCase -- 用于测试单个Activity，但只运行setUp和tearDown一次，可以保证运行多个测试之间fixture不会被重置
 *   +-AndroidTestCase -- 通常用于编写 Android 环境下通用的测试用例(可以直接通过 getContext() 获取到非空实例)，可测试permission，可以使用 FocusFinder
 *      可用 LayoutInflater.from(getContext()).inflate(R.layout.xxx, null) 的方式加载资源并测试
 *      +-ApplicationTestCase<MyApplication> -- 测试Application对象。测试用例调用createApplication()后才会执行Application的onCreate方法，可通过 setContext 来注入自定义的MockContext对象
 *      +-ProviderTestCase2 -- 测试Content Provider
 *      +-ServiceTestCase -- 测试Service对象，其中提供了 getService() 来取得当前被测试的对象
 *      
 * MoreAsserts -- 支持更多的比较方法，如 RegEx(正则)比较等
 * ViewAsserts -- 可以用来校验 UI View
 * 
 * 断言方法
 *   assertEquals -- 相等断言
***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * 标注 -- 标注测试方法，分类划分主要是根据测试访问数据的位置，如本地，数据库，SD卡，网络，支持多线程 等
 *   @SmallTest 
 *   @MediumTest
 *   @LargeTest 
***************************************************************************************************************************************/

//*
public class AndroidTester  extends ActivityUnitTestCase<CallbackTesterActivity>{
	private final static String TAG = AndroidTester.class.getSimpleName();
	private Intent mStartIntent;
	public AndroidTester() {
		super(CallbackTesterActivity.class);
	}

	
	@Override
	protected void setUp() throws Exception {
		super.setUp();
		mStartIntent = new Intent(Intent.ACTION_MAIN);
	}


	public void testActivityLife(){
		assertNull(getActivity()); 
		Log.i(TAG, "Before call startActivity");
		
		startActivity(mStartIntent, null, null);		//start 之后 getActivity 才不为null
		Activity activity1 = getActivity();
		Activity activity2 = getActivity();
		
		assertNotNull(activity1 != null);
		assertEquals("在一次测试函数中，多次调用getActivity() 会返回同一实例", activity1,  activity2);
		
		Log.i(TAG, "Before callActivityOnStart");
        // At this point, onCreate() has been called, but nothing else Complete the startup of the activity
        getInstrumentation().callActivityOnStart(activity1);
        Log.i(TAG, "Before callActivityOnResume");
        getInstrumentation().callActivityOnResume(activity1);
        
        Log.i(TAG, "Before callActivityOnStop");
        getInstrumentation().callActivityOnStop(activity1);
        
        Log.i(TAG, "Before testActivityLife function End");			//实测发现没有调用 onDestroy
        // ActivityUnitTestCase.tearDown(), which is always automatically called, will take care
        // of calling onDestroy().
	}
}

/*/
class AndroidTester extends AndroidTestCase{
	public void testGetContext(){
		assertNotNull("AndroidTestCase 中可以直接获取Context", getContext());
	}
}
//*/