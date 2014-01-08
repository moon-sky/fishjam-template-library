package com.fishjam.android.study;
import android.test.AndroidTestCase;

/***************************************************************************************************************************************
 * HVGA( Half-size VGA) -- VGA(640x480)的一半，分辨率为480x320, iPhone,第一款gPhone等手机都是。
 * dip(density irrelevant pixels) -- 密度无关的像素,简称为 dp。基于屏幕密度的抽象单位，程序用它来定义界面元素。
 *   像素(px)=dip * (密度 / 160) , 如 密度为160dpi的屏幕上，1dip等于1x; 密度240dpi的屏幕上,1dip=1.5px
 *
 * 尺寸：屏幕的物理尺寸，指屏幕的对角线长度，如 2.8英寸、3.7英寸
 * 比例：屏幕的物理长度和物理宽度比，16:9或16:10的是宽屏，4:3的为窄屏
 * 分辨率：屏幕上拥有的像素的总数，常使用"宽度x长度"表达，？通常应用程序并不直接处理分辨率？
 * 密度：以屏幕分辨率为基础，沿屏幕长宽方向排列的像素，密度较低的屏幕，在长宽方向都只有比较少的像素。
 * 视网膜(Retina)显示屏：一种超高像素和越高密度的液晶屏，相对于以前的iPhone，增加了4倍的像素数量，
 *       将960*640的分辨率压缩到和前代iPhone相同物理尺寸(3.5英寸)的显示屏内。屏幕密度达到 326像素/英寸
 *  
 * 多分辨率规格的界面设计 -- 真正需要区别的是OS版本(1.5和1.6及以上)而不是屏幕分辨率
 *   各种屏幕和UI类型：
 *     HTC Sense UI, SamSung TouchWiz UI, Motorola MOTO blur UI, Xiaomi MIUI, OMS(Ophone), LEOS(LePhone), Alibaba云OS 等
 *   A.1.5以前不支持高分屏，所有图片放在同一个drawable目录下;
 *     1.6开始支持 高(drawable-hdpi)、中(drawable-mdpi)、低(drawable-ldpi)三种密度。
 *     3.0以后支持 超高(drawable-xhdpi)，
 *     为了电视? 支持 (drawable-xxhdpi)
 * 
 *  交互设计建议：
 *    1.以中密度(mdpi)的正常屏幕(HVGA, 320x480)为设计基准进行界面布局(此时 dip和px为1:1，能更充分理解空间并合理使用)；
 *    2.程序可以为各种尺寸的屏幕提供不同的资源(主要是布局),也可以为各种密度的屏幕提供不同的资源(主要是位图);
 *    3.使用与密度无关的像素(dip)来定义应用程序的界面布局，而不是px -- 这样可保证应用程序的界面能在各种分辨率的屏幕上都可以正常显示。
 *    4.执行时，平台会根据屏幕本身的尺寸与密度特性，自动载入对应的资源，并把它们从与密度无关的像素转换成屏幕上的物理像素
 *    5.在处理屏幕方向变化时，为其优化而不是单纯地改变方向
 *    6.挑选具有代表性的特性界面在真机预览效果
 *    7.处理按钮的放大缩小时，尽可能使用九宫格图片，而不是单纯绘图
***************************************************************************************************************************************/

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
 *   相关组件: Canvas + Bitmap + Paint 
 *   在View的子类中重载 onDraw(Canvas canvas) 函数，并在其中使用 进行绘制。
 *   
***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * View 事件
 *     onTouchEvent() -- 当发生触摸屏事件时的回调，switch(event.getAction()) 来判断按键状态(down,move,up 等)
 * 
 * View
 *   AdapterView
 *   AutoCompleteTextView -- 自动完成文本框，通过设置想要显示资源的适配器(setAdapter)来实现
 *   BaseAdapter -- 
 *   Button
 *     text
 *   EditText -- 编辑框或密码框
 *     textSize -- 18sp
 *     password -- true
 *     numeric -- decimal
 *   CheckBox -- 复选按钮
 *   Gallery(图库) + ImageSwitcher -- 能水平方向显示其内容，一般用来浏览图片，被选中的项位于中间，且可以响应事件显示信息。
 *      通常使用 Gallery 显示缩略图，在点击时通过ImageSwitcher::setImageResource切换到大图
 *      ImageSwitcher::setOutAnimation(AnimationUtils.loadAnimation(this, android.R.anim.fade_out)); // 设置动画效果
 *   GridView -- 按照行列的方式显示内容，一般适合显示图片)
 *   ImageButton
 *     src -- "@drawable/iconempty"
 *     setImageResource(R.drawable.iconempty);
 *   ImageView -- 显示图片
 *     setImageDrawable(getResources().getDrawable(R.drawable.right));   //设置显示的图片, 或通过 src 属性设置
 *     setImageResource(xxx);
 *   ListView/ListActivity -- 列表视图，以垂直列表的方式列出需要显示的列表项(如联系人名单、系统设置项等)
 *     关键点：设置Adapter
 *   MapView -- 显示Google地图
 *   ProgressBar -- 进度条，有很多种:
 *     1.ProgressDialog(对话框进度条)： 覆盖Activity的onCreateDialog方法，并在其中创建进度条并返回；调用showDialog方法显示;
 *     2.ProgressBarIndeterminate(标题栏进度条)：调用 Activity.requestWindowFeature(Window.FEATURE_INDETERMINATE_PROGRESS) 方法设置窗口有进度条的特征;
 *       调用 Activity.setProgressBarIndeterminateVisibility 显示进度条
 *     3.ProgressBar(水平进度条) -- 布局文件中声明ProgressBar，获取到变量后调用 incrementProgressBy 等方法设置进度
 *   RadioGroup(管理一组RadioButton)
 *     checkedButton -- "@+id/sex1"
 *     orientation -- vertical,horizontal
 *   RadioButton
 *   Spinner -- 下拉列表
 *   TabHost / TabActivity -- 选项卡
 *   TextView -- 文本视图，显示字符串
 *     text -- @string/str_id
 *     textColor -- @drawable/darkgray
 *     gravity="center_vertical|center_horizontal", bottom
 *     autoLink -- all(可以显示网址，如 http://)
 *   ToggleButton -- 开关按钮
 *   VideoView -- 
 *   WebView -- 内置浏览器控件，可直接加载网页。为响应超链接功能，调用 setWebViewClient 方法设置自定义的 WebViewClient 子类实例
 *     getSettings().setJavaScriptEnabled(true) -- 更改设置
 *     loadUrl -- 加载指定的URL地址网页
 *   Window
 *     setFormat -- 设置窗口特征，如 PixelFormat.TRANSLUCENT(半透明)， 需要在 setContentView 之前调用
***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * Layout -- Android 通过 LayoutInflater 类将 XML 格式的布局文件中的组件解析为可视化的视图组件。
 *   布局文件中的 <requestFocus/> 项代表什么意思? 
 *   (TODO:已废弃 )AbsoluteLayout -- 绝对位置定位，降低兼容性，维护成本高
 *   FrameLayout -- 帧布局，组件从屏幕的左上角开始布局，多个组件层叠排序，后面的组件覆盖前面的组件。
 *   LinearLayout -- 线型布局，按照垂直或者水平方向布局组件
 *     gravity(对齐方式) -- top, bottom, left, right, center_vertical
 *     orientation(方向) -- vertical,horizontal
 *     layout_width -- fill_parent,wrap_content(根据内容指定高宽),320px,80dip
 *     layout_height -- fill_parent,wrap_content
 *     layout_alignParentRight -- true
 *     layout_alignParentTop -- true
 *     layout_alignParentLeft -- true
 *     layout_marginRight -- 80dip, 
 *     layout_below -- "@+id/ExitButton", 
 *     layout_alignTop -- "@+id/sizebutton"
 *     layout_toLeftOf -- "@+id/sizebutton"
 *     layout_x, layout_y -- 30px
 *     layout_centerHorizontal -- true
 *     id -- "@+id/ResultText"
 *    background -- "@drawable/testpic",
 *   RelativeLayout -- 相对布局，相对其他组件的布局方式(如在其上下左右等)。可以拉伸自动适应，但会造成图像变形。
 *     layout_below, layout_toLeftOf
 *   ScrollView 
 *   TableLayout  -- 表格布局，按照行列方式布局组件( 采用 > TableRow > 元素 的方式)
 *     stretchColumns -- "数字"
 *   TableRow
 *
 * LayoutInflater infater = (LayoutInflater)getSystemService(Context.LAYOUT_INFLATER_SERVICE); //或LayoutInflater.from(getApplicationContext())
 * convertView = infater.inflate(R.layout.baseadapter_provider,null);
***************************************************************************************************************************************/

/**************************************************************************************************************************************
 * 对话框(Dialog)
 *   AlertDialog(警告对话框)
 *     示例: 
 *       AlertDialog.Builder dlgBuilder = new AlertDialog.Builder(this);  //创建Builder，然后可以依次设置各种参数
 *       dlgBuilder.setMessage(msg).setCancelable(false).setPositiveButton("确定", new DialogInterface.OnClickListener() { ...onClick(...){finish();} }); //设置builder
 *       AlertDialog alert = dlgBuilder.create();  //创建对话框实例
 *       alert.show(); //显示
 *  ColorPickerDialog -- 选择颜色对话框
 *  DatePickerDialog/TimePickerDialog(日期/时间 选择对话框) --  
 *    1.重载 Activity.onCreateDialog(int id) { switch(id) {...}  } 方法创建对话框;
 *    2.在 OnDateSetListener 和 OnTimeSetListener 的对应事件方法中响应日期和时间的设置;
 *    3.调用 Activity.showDialog 显示对话框
 *  
 * Toast -- 提示信息(没有交互功能，只是提示), 静态的makeText设置显示文本和时长
 *    Toast.makeText(getApplicationContext(), "提示信息", Toast.LENGTH_LONG).show;
**************************************************************************************************************************************/

/***************************************************************************************************************************************
 * Activity  :  表示用户界面中的一个屏幕，显示由几个Views控件组成的用户接口，并对事件做出响应，可以在xml文件中定义所需要的Views或在代码中定义，
 *     Context <- ContextWrapper <- ContextThemeWrapper <- Activity
 *     注意：Activity必须在清单文件中声明才能使用
 *   每个Activity的状态由它所在Activity栈中的位置所决定，所有当前正在运行的Activity将遵循后进先出的原则。
 *   当一个新的Activity启动，当前的Activity将移至堆栈的顶部，如果用户使用Back按钮，或在前台Activity被关闭，下一个Activity将被激活并且移至到堆栈的顶部。
 *   Activity状态 -- 状态过渡具有不确定性并且由Runtime完全管理
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
 *   常见场景的执行顺序:
 *     初次启动:    onCreate -> onStart -> onResume
 *     程序退出:    onPause  -> onStop -> onDestroy
 *     后台到前台: onRestart ->onStart -> onResume
 *   常见方法
 *     findViewById -- 根据ID查找组件的实例
 *     finish() -- 结束Activity, 通常用法为 MyActivity.this.finish(); 
 *     setContentView -- 通常在重载的 onCreate 中调用来设置要显示的视图，从而实现与用户交互的功能
 *     startActivity -- 启动并导航到另一个Activity，参数被封装在Intent中
 *       Intent intent = new Intent(FirstActivity.this, SecondActivity.class);  通过Bundle设置参数;  startActivity(intent);  //在FirstActivity中启动SecondActivity
 *       在 SecondActivity::onCreate 中: Intent intent = getIntent(); Bundle bundle = intent.getExtras(); ...
 *     startActivityForResult -- 以等待返回结果的方式启动并导航到另一个Activity， 需要重载 onActivityResult 处理返回的 Intent 等结果。
 *                                       在SecondActivity中通过 SecondActivity.this.setResult(0, intent); 设置返回值， 然后 finish() 结束
 *   常见子类
 *     ListActivity, MapActivity 等
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
 *    requestWindowFeature(Window.FEATURE_NO_TITLE);   //Hide title bar;
 *    getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);  //Hide status bar;
 *    //getWindow().setBackgroundDrawableResource(R.color.red_bg); //设置背景色
 * 2. 通过资源中定义的id找到对应的对象 -- findViewById
 *    TextView ResultText = (TextView)findViewById(R.id.ResultText);
 * 3. 显示图片
 *    
**************************************************************************************************************************************/

public class UITester extends AndroidTestCase {
    public UITester()    {
        
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
    
    public void TabHostTester(){
        /********************************************************************************************
         * 方法1:
         *   1. 在布局文件中使用 FrameLayout 列出Tab组件及Tab中的内容组件
         *   2. Activity 要继承 TabActivity;
         *   3.调用 TabActivity::getTabHost 获得 TabHost 对象，然后创建 Tab 选项页
         * 方法2:
         *   1.实现接口 TabHost.TabContentFactory 的 createTabContent 方法来指定
         *     public class MyActivity extends TabActivity implements TabHost.TabContentFactory { 
         *          onCreate 中:  getTabHost().addTab(getTabHost().newTabSpec("all").setContent(this);
         *          public View createTabContent(String tag) { ...; if(tag.equals("all")) {  设置all页面对应的内容  }  } 
         ********************************************************************************************/
        
        /*******************************************************************************************
        //代码部分
            TabHost tabHost = getTabHost();
            LayoutInflater.from(this).infalte(R.layout.main, tabHost.getTabContentView(), true);
            tabHost.addTab(tabHost.newTabSpec("all").setIndicator("所有通话记录").setContent(R.id.TextViewAll));
            tabHost.addTab(tabHost.newTabSpec("ok").setIndicator("已接来电").setContent(R.id.TextViewReceived));
            tabHost.addTab(tabHost.newTabSpec("cancel").setIndicator("未接来电").setContent(R.id.TextViewUnreceived));

         //布局部分
           <FrameLayout ....>
                 <TabHost>
                   <TextView android:id="@+id/TextViewAll" android:layout_width="wrap_content" android.text="所有通话记录">
                   </TextView>
                 </TabHost>
           </FrameLayout>
         *******************************************************************************************/
    }
    

    public void GridViewDisplayImageTester(){
        //使用 BaseAdapter 类，重写其中的方法( 如 getView 来设置图片显示格式 )
        /********************************************************************************************
        //自定义的 BaseAdapter 类，根据需要返回并图片的数据信息
            class MyAdapter extends BaseAdapter{
                private Integer [] imgs = {
                        R.drawable.ic_launcher
                };
                Context context;
                MyAdapter(Context context){
                    this.context = context;
                }
                @Override
                public View getView(int position, View convertView, ViewGroup parent) {
                    ImageView imageView;
                    if(convertView == null){
                        imageView = new ImageView(context);
                        imageView.setLayoutParams(new GridView.LayoutParams(45, 45));    //设置ImageView对象布局
                        imageView.setAdjustViewBounds(false);         //设置边界对齐
                        imageView.setPadding(8,  8,  8,  8);                //设置间距
                    }
                    else{
                        imageView = (ImageView)convertView;
                    }
                    imageView.setImageResource(imgs[position]);    //为 ImageView 设置图片资源
                    return imageView;
                }
            }
        //Activity 子类中，使用自定义的 Adapter 在 GridView 中显示图片
            GridView gridView = (GridView)findViewById(R.id.xxx);
            gridView.setNumColumns(4);     //设置列数
            gridView.setAdapter(new MyAdapter(this));
        *********************************************************************************************/
    }
}

