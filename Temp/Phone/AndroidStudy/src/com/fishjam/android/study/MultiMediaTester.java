package com.fishjam.android.study;
import android.view.SurfaceHolder;
import junit.framework.TestCase;

/***************************************************************************************************************************************
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
 *   Drawable --
 *   Bitmap / BitmapFactory 
 *   Tween / Frame -- 动画
 *   Canvas
 *   Color
 *   Paint
 *   Path
 *   Matrix / Shader -- 图形特效
 * 3D图形处理(OpenGL ES) -- 位于 javax.microedition.khronos.opengles, android.opengl 等包中
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

public class MultiMediaTester extends TestCase {
	SurfaceHolder holder;
}
