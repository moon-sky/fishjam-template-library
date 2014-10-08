package com.fishjam.android.study;

import java.io.InputStream;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.test.ActivityTestCase;
import android.test.ActivityUnitTestCase;

/***************************************************************************************************************************************
 * 最佳实践
 *   TODO: Google 的Sample -- http://developer.android.com/training/displaying-bitmaps/index.html
 *   1.加载图片
 *     BitmapFactory.decodeStream(getResources.openRawResource())  构造图片(不使用Java层的createBitmap，节约内存)
 *     TODO: 该方法不会根据机器的各种分辨率来自动适应 ？
 *   2.BitmapFactory.Options( inJustDecodeBounds-设置为true时只获取图片高宽信息, inSampleSize-压缩比例 )
 *     读取图片时加上图片的Config参数，可以更有效减少加载的内存,否则容易出现OOM问题
 *   3.显示图片 -- setImageBitmap ?
 *   4.用 LruCache 来缓存Drawable和图片，内存紧缺时清掉不必要的图
***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * 手机上的调节音量的按钮分两种情况：
 *   1.调整手机本身的铃声音量
 *   2.调整游戏，软件，音乐播放的音量， 需要先通过 Activity.setVolumeControlStream(AudioManager.STREAM_MUSIC) 设置一下，
 *      否则只有游戏中有声音在播放的时候才能调整游戏的音量。
 * 
 * AudioManager -- (AudioManager)getSystemService(Context.AUDIO_SERVICE); //通过服务获取音频管理器，可控制音量大小等
 *   getStreamMaxVolume(AudioManager.STREAM_MUSIC); -- 获取最大音量值(TODO: 注意是 15，而不是100)
 *   setStreamVolume(AudioManager.STREAM_MUSIC, currentVol, AudioManager.FLAG_PLAY_SOUND); --  设置当前的音量大小
 *   
 * Android 对常见媒体格式提供了支持，
 *   图片：JPEG, GIF, PNG, BMP 等
 *   音频：3GP, MP3, WAV 等
 *   视频：3GP, MP4 等
 *   
 * 播放音视频
 *   MediaPlayer -- 静态的 create 方法返回实例，然后可通过 start/pause/stop 等方法控制
 *       缺点：资源占用量较高、延迟时间较长、不支持多个音频同时播放
 *       优点： 
 *     setDataSrouce -- 设置想要播放文件的路径
 *     setVolue -- 调整左右声道的大小
 *     prepare -- 在start前调用
 *     OnCompletionListener -- 在媒体播放完成时会回调的接口，其有 onCompletion 方法。通过 setOnCompletionListener 设置
 * JetPlayer -- 可以播放比较长的游戏背景音乐, 参见 Samples 中的 JetBoy
 * VideoView + MediaController -- 可很容易的实现视频播放器
 *   video.setMediaController 和 ctrl.setMediaPlayer 相互关联， 然后 video.requestFocus 获得焦点
 * SoundPool -- 直接new实例： new SoundPool(4, AudioManager.STREAM_MUSIC, 100);  //允许4个声音流同时播放，声音品质为100
 *   优点：适用于游戏中同时播放多个音效， 
 *   缺点：只能播放较短的声音片段(最大只能申请1M的内存空间)，音频格式建议使用OGG，目前只对16bit的WAV文件有较好的支持
 *   load -- 加载声音资源，返回ID
 *   play() -- 根据ID播放已经加载的声音资源
 * 
 * 录制音视频
 *   MediaRecorder 
 *     setAudioSource -- 设置音频源，如 AudioSource.MIC
 *     setOutputFormat -- 设置输出格式，如 OutputFormat.THREEE_GPP
 *     setAudioEncoder -- 设置音频编码器，如 AudioEncoder.AMR_NB
 *     setOutputFile -- 设置输出文件路径
 *     
 * 手机拍照
 *   Camera -- 相机类，实现拍照功能
 *     PictureCallback -- 内部接口，处理照片准备好后的回调
 *       onPictureTaken() -- 照片回调，可通过 AsnycTask 子类进行异步保存
 *     open() -- 打开相机
 *     release() -- 释放非托管资源, 可以在 SurfaceHolder.Callback.surfaceDestroyed 中调用，注意先 stopPreview
 *     setPreviewDisplay(SurfaceHolder) -- 设置预览，可以在 SurfaceHolder.Callback.surfaceCreated 中调用
 *     setParameters() -> startPreview() -- 设置参数后进行预览，可以在 SurfaceHolder.Callback.surfaceChanged 中调用
 *     takePicture() -- 进行照相
 *   SurfaceView -- 实现自绘的视图组件，可以控制表面的格式，底层提供了GPU加速，具有很好的性能，
 *      其子类有 GLSurfaceView、VideoView 等. 通常用于主动持续更新的画面(如 酷跑游戏等)
 *      注意：SurfaceView可以在一个子线程中直接绘制，而View必须在UI的主线程中绘制。
 *   SurfaceHolder -- 界面控制接口，一般通过 SurfaceView.getHolder 获得，允许控制界面的尺寸、格式、像素，及监控界面尺寸的改变等
 *     Callback -- 内部回调接口，可获得界面改变消息(surfaceCreated, surfaceChanged, surfaceDestroyed )，通过 addCallback 设置
 *     lockCanvas/unlockCanvasAndPost --锁定surface(可锁定部分范围)并返回可绘图的Canvas；绘制完成后解锁canvas并等待主线程更新
 *       注意： unlockCanvasAndPost 后只是提交了更新请求，主线程可能尚未更新，因此绘制的内容会被覆盖。
 *                为了使提交立即生效，可以 lockCanvas(new Rect(0, 0, 0, 0)) + unlockCanvasAndPost() 一次。
 *     setType() -- 设置类型，如 SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS
 *   Surface -- 保存了当前窗口的像素数据，但一般不直接和Surface打交道。而是通过在Canvas上绘图来改变其中的数据 
 *     
 * 2D图形处理  -- android.graphics, android.view.animation 等包中
 *   图片: Drawable / Bitmap / BitmapFactory
 *     Bitmap
 *       createBitmap -- 根据参数创建位图，filter 参数?
 *       extractAlpha -- 根据位图信息生成只有透明度没有颜色(即Alpha通道)的位图
 *       isRecycle-- 判断图片内存是否已被回收
 *       recycle -- 回收图片所占的内存
 *     Drawable -- 抽象基类，代表可以被绘制出来的某东西，常用于定制UI，其有多个子类: BitmapDrawable/ColorDrawable/ShapeDrawable/StateListDrawable 等
 *       使用方式:
 *         1.保存在工程中的图片文件, 如 ImageView.setImageResource(R.drawable.myImage); 
 *         2.XML文件定义Drawable属性, 如 <ImageView android:id=“@id/ImageView01" android:src="@drawable/myimage" ... />
 *           注意：可以通过 XML 定义 drawable 对象, 使用方式:  android:background="@drawable/bg_border" 
 *              bg_border.xml(透明背景色 + 红色边框) : <shape xmlns:android="http://xxx"><solid android:color="#0000"/><stroke android:width="4px" android:color="#f00" /></shape>
 *              button_selector.xml(按钮按下、弹起时的界面) : <selector> <item state_pressed="true" drawable="xxx"/> <item state_pressed="false" drawable="xxx"/> </selector>
 *         3.直接构造实例读取(SD) 
 *           a.Bitmap bmp = BitmapFactory.decodeFile("/sdcard/test.jpg"); 
 *             类似的有 decodeResource(getResources(), R.drawable.xxx);
 *                          decodeStream(context.getResources().openRawResource(xxx));
 *           b.Bitmap bmp2 = Bitmap.createBitmap(bmp, ...);  //根据原始位图创建新视图  
 *     BitmapFactory -- Bitmap的工厂类，具有 decodeFile/decodeResource/decodeStream/decodeByteArray 等多种方式来构造Bitmap
 *     
 *   9Pitch图片 -- 特殊的PNG图片(扩展名为 .9.png，使用 draw9patch.bat 编辑)，
 *     在原始图片四周各添加一个宽度为1像素的线条，这4条线决定了图片的缩放、显示规则。
 *     代码中使用：
 *       Bitmap bmp_9path = BitmapFactory.decodeResource(getResources(), R.drawable.9path);  
 *       NinePatch np = new NinePatch(bmp_9path, bmp_9path.getNinePatchChunk(), null); //第二个参数就是.9.png图片文件中保存的处理拉伸方式的信息数据
 *       np.draw(canvas, xxxx); 
 *     
 *   动画 -- Animation(动画抽象类，有多个子类) 
 *       AnimationSet -- 定义动画属性集合类
 *       AnimationUtils -- 动画工具类
 *       ViewPropertyAnimator -- 
 *     1.Tween -- 可以使视图组件移动、放大、缩小、旋转、及透明度等变化；
 *       ScaleAnimation/AlphaAnimation/RotateAnimation/TranslateAnimation
 *       使用方式:
 *         1.硬编码 -- 构造Animation子类实例并设置好参数后，通过 ImageView.startAnimation 等函数启动指定动画
 *         2.配置文件(推荐) -- res\anim\ 目录中设置动画文件，然后通过 AnimationUtils.loadAnimation 方法获得动画实例，并通过组件的 startAnimation 开始，
 *           <set> <alpha android:fromAlpha="0.1" toAlpha="1.0" duration="3000" /></set>， 类似的有 <scale>, <translate>, <rotate> 等
 *     2.Frame -- 帧动画，通过顺序播放排列好的图片来实现，类似电影。
 *       AnimationDrawable -- 一般通过XML配置文件配置，<animation-list> <item/></animation-list>，然后在Activity的xml配置中可以通过 android:background 指定？
 *     
 * 自绘
 *   1.从 View/SurfaceView 等类继承，然后选择性重载以下方法：
 *      onDraw -- 使用Canvas进行绘制，通过 invalidate 或 postInvalidate 请求更新
 *      onFinishInflate -- 当应用从XML布局文件加载组件并利用它来构建界面之后调用
 *      onMeasure -- 检测View组件及它所包含的所有组件的大小，父元素要放置该View时调用。
 *        计算时先通过 MeasureSpec 类的静态方法getMode(AT_MOST 表示最大可获得的空间，对应wrap_content; EXACTLY 表示精确的尺寸，对应fill_parent )
 *        和getSize来分析传入的边界参数。然后通过 setMeasuredDimension 指定计算结果。
 *      onLayout -- 当组件需要分配其子组件的位置、大小时回调(即调用 layout 时)
 *      onTrackballEvent -- 当发生轨迹球事件时触发
 *      onTouchEvent -- 当发生触摸屏事件时触发， TODO:怎么处理手势(Gesture?)
 *      onAttachedToWindow/onDetachedFromWindow -- 当该组件 放入窗口/从窗口分离 时触发
 *   2.可以从 GraphicsActivity 继承
 *   
 *   Canvas -- 画布
 *     save/restore -- 保存和取出画布的状态，成对出现。通常用于对Canvas进行 旋转/缩放/平移等操作，来对其中的部分元素进行特殊绘制，绘制完毕后恢复Canvs
 *   Color/Paint(画笔)/Path/ShapeDrawable及其子类，如 OvalShape(椭圆)、RectShape(矩形)
 *   
 * 图形特效
 *   Matrix -- 3x3的坐标变换矩阵，通常用于图片的特效处理等。
 *     可以实现图形的 平移(Translate)、 旋转(Rotate)、缩放(Scale)、错切(Skew, x或y保持不变，而对应的y或x按比例平移: 如 矩形=>菱形，属于等面积变换 )等， (TODO: Persp 透视变换?)。
 *       每种变换有三种操作方式： set(直接设置), pre(先乘, 即矩阵的右乘)，post(后乘, 即矩阵的左乘) -- 矩阵的乘法不满足交换律，先乘后乘必须严格区分。
 *       旋转/缩放/错切 可以围绕一个中心点进行，如不指定，则围绕(0,0)进行变换。
 *       水平对称：float matrix_values[] = {1f, 0f, 0f, 0f, -1f, 0f, 0f, 0f, 1f}; matrix.setValues(matrix_values); 
 *       垂直对称：float matrix_values[] = {-1f, 0f, 0f, 0f, 1f, 0f, 0f, 0f, 1f}; matrix.setValues(matrix_values);
 *       x=y轴对称： float matrix_values[] = {0f, -1f, 0f, -1f, 0f, 0f, 0f, 0f, 1f};
 *     invert() -- 
 *     setTranslate() -- 
 *   ColorMatrix -- 颜色矩阵(RGBA)。可用于图片特效处理，如 将图片做出黑白的，或者老照片的效果 等
 *   Shader -- 渲染效果，可通过 Paint.setShader(); 等方式来使用。
 *      其有很多子类，如 BitmapShader(位图渲染)、ComposeShader(混合渲染)、LinearGradient(线性渲染)、RadialGradient(光束渲染), SweepGradient(梯度渲染) 等 
 *   Xfermode -- 两张图交互时的覆盖模式，一般用法为 paint.setXfermode(new PorterDuffXfermode(Mode.SRC_IN 等模式枚举值 ));
 *   EdgeEffect/EdgeEffectCompat -- 边缘效果
 *   
 * 3D图形处理(OpenGL ES) -- 位于 javax.microedition.khronos.opengles, android.opengl 等包中
 *   
 *   
 ***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * 压缩图片成缩略图(向MediaStore中插入图片数据)
 *   Uri uri = getContentResolver().insert(Media.EXTERNAL_CONTENT_URI, values); 
 *   Bitmap sourceBmp = BitmapFactory.decodeFile("文件全路径.jpg");             //获得Bitmap实例
 *   OutputStream outStream = getContentResolver().openOutputStream(uri);  //获得输出流
 *   sourceBmp.compress(Bitmap.CompressFormat.JPEG, 50, outStream);      //压缩输出流 
 *   outStream.close();
***************************************************************************************************************************************/

public class MultiMediaTester extends ActivityUnitTestCase<Activity> {
	private Intent mStartIntent;
    public MultiMediaTester() {
		super(Activity.class);
	}
    

	@Override
	protected void setUp() throws Exception {
		super.setUp();
		mStartIntent = new Intent(Intent.ACTION_MAIN);
	}


	public void testLoadBitmap(){
    	startActivity(mStartIntent, null, null);
    	
    	InputStream is = getActivity(). getResources().openRawResource(R.drawable.ic_launcher);
        BitmapFactory.Options options=new BitmapFactory.Options();
        
        //第一步: 获取图片的高宽信息, 此时 decode的bitmap为null(节约内存)
        options.inJustDecodeBounds = true;
        BitmapFactory.decodeStream(is,null,options);
        int nWidth = options.outWidth;
        int nHeight = options.outHeight;
        
        //第二部, 根据图片的高宽和设备的范围设置压缩比例，从而以较小的内存消耗获得合适的bitmap
        options.inJustDecodeBounds = false;
        //缩放的比例，缩放是很难按准备的比例进行缩放的，其值表明缩放的倍数，SDK中建议其值是2的指数值,值越大会导致图片不清晰
        options.inSampleSize = 4;   //width，hight设为原来的四分一，即整个图是原来的 16 分之一
        
        
      //以最省内存的方式读取本地资源的图片
        options.inPurgeable = true;  
        options.inInputShareable = true;  //共享一个指向数据源的引用，否则会做深拷贝
        options.inPreferredConfig = Bitmap.Config.RGB_565;  //缺省是 ARGB_8888 
        
        Bitmap btp =BitmapFactory.decodeStream(is,null,options);
        if (btp != null) {
			btp.recycle();
		}
        
    }
}


