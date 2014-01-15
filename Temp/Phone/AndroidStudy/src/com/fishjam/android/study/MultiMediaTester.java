package com.fishjam.android.study;

import android.test.AndroidTestCase;

/***************************************************************************************************************************************
 * AudioManager
 * 
 * Android 对常见媒体格式提供了支持，
 *   图片：JPEG, GIF, PNG, BMP 等
 *   音频：3GP, MP3, WAV 等
 *   视频：3GP, MP4 等
 *   
 * 播放音视频
 *   MediaPlayer -- 静态的 create 方法返回实例，然后可通过 start/pause/stop 等方法控制
 *     setDataSrouce -- 设置想要播放文件的路径
 *     prepare -- 在start前调用
 *     OnCompletionListener -- 在媒体播放完成时会回调的接口，其有 onCompletion 方法。通过 setOnCompletionListener 设置
 *   JetPlayer
 * VideoView + MediaController -- 可很容易的实现视频播放器
 *   video.setMediaController 和 ctrl.setMediaPlayer 相互关联， 然后 video.requestFocus 获得焦点
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
 *   SurfaceView -- 实现照片预览的视图组件
 *   SurfaceHolder -- 界面控制接口，一般通过 SurfaceView.getHolder 获得，允许控制界面的尺寸、格式、像素，及监控界面尺寸的改变等
 *     Callback -- 内部回调接口，可获得界面改变消息(surfaceCreated, surfaceChanged, surfaceDestroyed )，通过 addCallback 设置
 *     setType() -- 设置类型，如 SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS
 *     
 * 2D图形处理  -- android.graphics, android.view.animation 等包中
 *   图片: Drawable / Bitmap / BitmapFactory
 *     Bitmap
 *       createBitmap -- 根据参数创建位图
 *       extractAlpha -- 根据位图信息生成只有透明度没有颜色(即Alpha通道)的位图
 *     Drawable -- 抽象基类，代表可以被绘制出来的某东西，常用于定制UI，其有多个子类: BitmapDrawable/ColorDrawable/ShapeDrawable
 *       使用方式:
 *         1.保存在工程中的图片文件, 如 ImageView.setImageResource(R.drawable.myImage); 
 *         2.XML文件定义Drawable属性, 如 <ImageView android:id=“@id/ImageView01" android:src="@drawable/myimage" ... />
 *         3.直接构造实例读取(SD) 
 *           a.Bitmap bmp = BitmapFactory.decodeFile("/sdcard/test.jpg"); 
 *             类似的有 decodeResource(getResources(), R.drawable.xxx);
 *                          decodeStream(context.getResources().openRawResource(xxx));
 *           b.Bitmap bmp2 = Bitmap.createBitmap(bmp, ...);  //根据原始位图创建新视图  
 *           
 *   
 *   动画 -- Animation(动画抽象类，有多个子类) 
 *       AnimationSet -- 定义动画属性集合类
 *       AnimationUtils -- 动画工具类
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
 *      onMeasure -- 检测View组件及它所包含的所有组件的大小
 *      onLayout -- 当组件需要分配其子组件的位置、大小时回调
 *      onTrackballEvent -- 当发生轨迹球事件时触发
 *      onTouchEvent -- 当发生触摸屏事件时触发， TODO:怎么处理手势(Gesture?)
 *      onAttachedToWindow/onDetachedFromWindow -- 当该组件 放入窗口/从窗口分离 时触发
 *   2.可以从 GraphicsActivity 继承
 *   
 *   Canvas -- 画布
 *   Color/Paint(画笔)/Path/ShapeDrawable及其子类，如 OvalShape(椭圆)、RectShape(矩形)
 *   
 * 图形特效
 *   Matrix -- 坐标变换矩阵，可以实现图形的 旋转(setRotate)、缩放(setScale)和平移(setTranslate)，图片的特效处理等
 *     invert() -- 
 *     setTranslate() -- 
 *   ColorMatrix -- 颜色矩阵(RGBA)。可用于图片特效处理，如 将图片做出黑白的，或者老照片的效果 等
 *   Shader -- 渲染效果，可通过 Paint.setShader(); 等方式来使用。
 *      其有很多子类，如 BitmapShader(位图渲染)、ComposeShader(混合渲染)、LinearGradient(线性渲染)、RadialGradient(光束渲染), SweepGradient(梯度渲染) 等 
 *   Xfermode -- 两张图交互时的覆盖模式，一般用法为 paint.setXfermode(new PorterDuffXfermode(Mode.SRC_IN 等模式枚举值 ));
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

public class MultiMediaTester extends AndroidTestCase {
    public void test(){
    }
}
