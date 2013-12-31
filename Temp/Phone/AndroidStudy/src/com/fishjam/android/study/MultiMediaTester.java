package com.fishjam.android.study;
import junit.framework.TestCase;

/***************************************************************************************************************************************
 * 压缩图片成缩略图(向MediaStore中插入图片数据)
 *   Uri uri = getContentResolver().insert(Media.EXTERNAL_CONTENT_URI, values); 
 *   Bitmap sourceBmp = BitmapFactory.decodeFile("文件全路径.jpg");             //获得Bitmap实例
 *   OutputStream outStream = getContentResolver().openOutputStream(uri);  //获得输出流
 *   sourceBmp.compress(Bitmap.CompressFormat.JPEG, 50, outStream);      //压缩输出流 
 *   outStream.close();
***************************************************************************************************************************************/

public class MultiMediaTester extends TestCase {

}
