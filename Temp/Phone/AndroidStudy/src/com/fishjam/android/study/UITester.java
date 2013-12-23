package com.fishjam.android.study;
import junit.framework.TestCase;

/***************************************************************************************************************************************
 * 风格(Style)
 *   自定义Style(指定颜色、大小等)
 *     1.编写style.xml文件,放在values目录下
 *       <?xml version="1.0" encoding="utf-8"?>
 *       <resources>
 *         <style name="DavidStyleText1" [parent=“父Style名”]>
 *           <item name="android:textSize">18sp</item>
 *           <item name="android:textColor">#EC9237</item>
 *         </style>    可以使用 <style></style> 定义多个,
 *       </resources>
 *     2.main.xml 中 <TextView style="@style/DavidStyleText1" ...>
 *     
 * 主题(Theme)
 * 
 * view  -- 所有UI类都建立在View和ViewGroup两个类的基础之上，采用组合(Composite)设计模式
 *   View --基本控件,  其子类称为 Widget
 *     Button( setOnClickListener(new Button.OnClickListener(){public void onClick(View v) {...}}); )
 *     EditText
 *     TextView
 *     RelativeLayout
 *     lists,grids,text boxes,buttons,
 *   ViewGroup -- 布局容器，子类称为Layout
 *   
 * 自绘
 *   在View的子类中重载 onDraw(Canvas canvas) 函数，并在其中使用 Canvas 和 Paint进行绘制。
***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * View
 *   BaseAdapter -- 
 *   Button
 *     text
 *   EditText
 *     textSize -- 18sp
 *     password -- true
 *     numeric -- decimal
 *   CheckBox
 *   Gallery(图库)
 *   ImageButton
 *     src -- "@drawable/iconempty"
 *     setImageResource(R.drawable.iconempty);
 *   ImageView -- 显示图片
 *     setImageDrawable(getResources().getDrawable(R.drawable.right));   //设置显示的图片, 或通过 src 属性设置
 *   ListView
 *   RadioGroup(管理一组RadioButton)
 *     checkedButton -- "@+id/sex1"
 *     orientation -- vertical,horizontal
 *   RadioButton
 *   TextView -- 文本视图，显示字符串
 *     text -- @string/str_id
 *     textColor -- @drawable/darkgray
 *     gravity="center_vertical|center_horizontal", bottom
 *     autoLink -- all(可以显示网址，如 http://)
 *   ProgressDialog -- 进度显示
 *   Spinner --
 *   
***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * Layout -- Android 通过 LayoutInflater 类将 XML 格式的布局文件中的组件解析为可视化的视图组件。
 *   布局文件中的 <requestFocus/> 项代表什么意思? 
 *   AbsoluteLayout -- 绝对位置定位，降低兼容性，维护高
 *   RelativeLayout -- 可以拉伸自动适应，但会造成图像变形。
 *   LinearLayout -- 线型布局
 *     orientation -- vertical,horizontal
 *     layout_width -- fill_parent,wrap_content(根据内容指定高宽),320px,80dip
 *     layout_height -- fill_parent,wrap_content
 *     layout_alignParentRight -- true
 *     layout_alignParentTop -- true
 *     layout_alignParentLeft -- true
 *     layout_marginRight -- 80dip, 
 *     layout_below -- "@+id/ExitButton", 
 *     layout_alignTop -- "@+id/sizebutton"
 *     layout_toLeftOf -- "@+id/sizebutton"
 *     layout_x -- 30px
 *     layout_y
 *     layout_centerHorizontal -- true
 *     id -- "@+id/ResultText"
   *   background -- "@drawable/testpic",
 *   TableLayout + TableRow + 
 *     stretchColumns -- "数字"
 *   TableRow
 *     gravity -- right
 *
***************************************************************************************************************************************/
 
/***************************************************************************************************************************************
 * Activity  :  表示用户界面中的一个屏幕，显示由几个Views控件组成的用户接口，并对事件做出响应，可以在xml文件中定义所需要的Views或在代码中定义，
 *     Context <- ContextWrapper <- ContextThemeWrapper <- Activity
 *   每个Activity的状态由它所在Activity栈中的位置所决定，所有当前正在运行的Activity将遵循后进先出的原则。
 *   当一个新的Activity启动，当前的Activity将移至堆栈的顶部，如果用户使用Back按钮，或在前台Activity被关闭，下一个Activity将被激活并且移至到堆栈的顶部。
 *   Activity状态 -- 状态过渡具有不确定性并且由OPhone Runtime完全管理
 *     Active状态：这时候Activity处于栈顶，且是可见的，有焦点的，能够接收用户输入前景Activity。
 *     Paused状态：Activity是可见的，但没有焦点。例如，如果有一个透明或非全屏幕上的Activity在你的Activity上面。
 *     Stopped状态：完全被遮住（不可见）时，将继续保留在内存中保持当前的所有状态和成员信息，但会是被回收对象的主要候选。
 *     Inactive状态：Activity被杀掉以后或者被启动以前，已被从Activity堆栈中移除，需要重新启动才可以显示和使用。
 *   监测Activity的状态变化
 *     onCreate -- 在Activity生命周期开始时被调用，初始化用户界面，分配引用类变量，绑定数据控件，并创建服务和线程 -- 通常需要findViewById将成员变量和控件建立关联。
 *     onRestore -- onCreate完成后被调用，用来回复UI状态
 *     onRestart -- 当activity从停止状态重新启动时调用(onStop->onRestart->onStart)
 *     onStart -- 当activity对用户即将可见的时候调用，可注册更新用户界面Intent接收者
 *     onResume -- 当activity将要与用户交互时调用此方法，此时activity在activity栈的栈顶，用户输入已经可以传递给它 
 *     onSave -- Activity即将移出栈顶保留UI状态时调用此方法
 *     onPause -- 当系统要启动一个其他的activity时调用，这个方法被用来提交那些持久数据的改变、停止动画、和其他占用 CPU资源的东西。
 *     onStop -- 当另外一个activity恢复并遮盖住此activity,导致其对用户不再可见时调用。一个新activity启动、其它activity被切换至前景、当前activity被销毁时都会发生这种场景。可注销更新用户界面Intent接收者
 *     onDestroy -- 在activity被销毁前所调用的最后一个方法，有可能在某些情况下，一个Activity被终止时并不调用onDestroy方法。
 *   常见方法
 *     findViewById
 *     setContentView -- 可将Activity与Views绑定在一起来实现用户交互的功能
 *     startActivity -- 从一个屏幕导航到另一个屏幕，打开Activity的条件被封装在Intent中
 **************************************************************************************************************************************/

/**************************************************************************************************************************************
 * Dialog
 *   AlertDialog
 *     示例: 
 *       AlertDialog.Builder dlgBuilder = new AlertDialog.Builder(this);  //创建Builder，然后可以依次设置各种参数
 *       dlgBuilder.setMessage(msg).setCancelable(false).setPositiveButton("确定", new DialogInterface.OnClickListener() { ...onClick(...){finish();} }); //设置builder
 *       AlertDialog alert = dlgBuilder.create();  //创建
 *       alert.show(); //显示
 * 
 * Toast -- 提示窗口(没有交互功能，只是提示)
 *    Toast.makeText(EX04_03.this,"提示信息", Toast.LENGTH_LONG).show;
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * 按钮交互
 *   1. implements OnClickListener
 *      Button ResolutionTest = (Button)findViewById(R.id.ResolutionTest); 
 *      ResolutionTest.setOnClickListener(this);  //必须在 onClick中比较 getId才能处理多个按钮的点击情况
 *      public void onClick(View arg0) { 
 *          if (arg0.getId() == R.id.ResolutionTest)  { startTest(); } 
 *      } 
 *   2. 使用成员变量
 *       ResolutionTest.setOnClickListener(m_myButtonClick);
 *       Button.OnClickListener m_myButtonClick =  new Button.OnClickListener() {...}
 *   3. 使用匿名事件
 *       ResolutionTest.setOnClickListener(new Button.OnClickListener() {
 *           //@Override -- 是否需要这行
 *           public void onClick(View v) {
 *              startTest();  //不用比较getId,每个Button都可以有自己的OnClick
 *           }
 *       });
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * 1. 隐藏TitleBar 和 Status Bar -- TODO：需要在setContentView(R.layout.start_test)之前调用，否则会出现force close 错误。
 *    在当前Activity的Java源文件引入android.view.Window和android.view.WindowManager，并在oncreate()方法中调用:
 *    requestWindowFeature(Window.FEATURE_NO_TITLE);   		//Hide title bar;
 *    getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);  //Hide status bar;
 *    //getWindow().setBackgroundDrawableResource(R.color.red_bg); //设置背景色
 * 2. 通过资源中定义的id找到对应的对象 -- findViewById
 *    TextView ResultText = (TextView)findViewById(R.id.ResultText);
**************************************************************************************************************************************/

public class UITester extends TestCase {
	public UITester()	{
		
	}
	protected void setUp() throws Exception {
		super.setUp();
	}

	protected void tearDown() throws Exception {
		super.tearDown();
	}
	
	//界面切换 -- 有什么区别?标准应该是使用第二种方法（虽然麻烦，但更合理和）
	//  1.切换layout -- 定义不同的layout xml文件,通过 setContentView方法 进行切换; 还在同一个Activity中，成员变量不变。
	//  2.切换Activity -- 定义不同的layout xml，实现不同的Activity子类(显示对应的layout),通过 Intent类 进行切换。
	//  注意需要在AndroidManifest.xml中定义新的activity，并设置那么属性，否则无法编译。
	public void TestSwitchUI() {
		/*************************************************************************************************
		 * Intent intent = new Intent();// new一个Intent对象
		 * intent.setClass(EX03_09.this, EX03_09_1.class); //指定要启动的class
		 * startActivity(intent);// 调用一个新的Activity -- 类似非模态？
		 * //startActivityForResult(intent,0); // 被调用Activity要返回值的依据,
		 * 启动新的Activity并等待其结束， // 并通过重载 onActivityResult 函数判断返回值(resultCode ==
		 * RESULT_OK), 可以通过 data.getExtras 获取数据， 具有层次关系 // 子Activity中通过如下方式返回结果:
		 * // EX03_09_1.this.setResult(RESULT_OK, intent); //
		 * 返回result回上一个activity // EX03_09_1.this.finish();// 结束这个activity
		 * EX03_09.this.finish(); // 关闭原本的Activity(根据具体情况)
		 **************************************************************************************************/
	}
}

