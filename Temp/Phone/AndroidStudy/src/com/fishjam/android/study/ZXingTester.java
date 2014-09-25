package com.fishjam.android.study;
import android.test.AndroidTestCase;

/**************************************************************************************************************************************
 * 
 * ZXing(http://code.google.com/p/zxing/) -- 开源的二维码、条形码处理库，包含了多种语言的接口。如微信中的扫一扫。
 *    主要分为：
 *      core -- 核心编解码库
 *      android -- Android客户端？缺点：要求用户必须安装BarcodeScanner?用户体验不好。
 *    缺点：
 *       实现太过复杂多余东西太多(指 2.1 以前的版本?)。
 *  
 * 编译：
 *   ZXing2.1 以前版本(使用ant编译, 使用 build.xml)
 *     1.B.下载 Ant -- http://ant.apache.org/, 解压后设置 ANT_HOME 环境变量，并将bin加入%PATH%
 *     2.下载 ProGuard -- http://proguard.sourceforge.net/ -- 压缩、优化和混淆Java字节码文件的免费工具，可以删除无用的类、字段、方法和属性等
 *   ZXing2.1 以后版本(使用maven编译, 使用 pom.xml ?)
 *     1.下载 maven -- https://maven.apache.org/download.cgi， 解压后设置 MAVEN_HOME 环境变量，并将bin加入%PATH%
 *     2.进入 core 目录后，执行  mvn -DskipTests -Dgpg.skip=true install 进行编译，第一次使用时会下载很多依赖库。编译后在 target 找到生成的jar包
 *     3.更新本地Android工程配置(增加 local.properties 文件设置SDK的路径): android.bat update project --path <工程目录，如 android>
 *     3.导入源码后，"Build path" -> "Add External Archives" ，加入 core.jar
 *   
 *  源码分析
 *    CaptureActivity -- 启动Activity(扫描器的主界面?)
 *    CaptureActivityHandler -- 解码处理类，负责调用另外的线程进行解码
 *    DecodeThread -- 解码的线程
 *    ViewfinderView -- 自定义的View，即拍摄时中间的框框
 * 工具类
 *    IntentIntegrator + IntentResult -- 通过 Intents 方式使用二维码(但要求用户安装BarcodeScanner)
 *    MatrixToImageWriter -- 生成二维码
 *    MultiFormatReader -- 读取和分析二维码图片
**************************************************************************************************************************************/

public class ZXingTester extends AndroidTestCase {

	public void testGenerateCode(){
		//生成二维码文件
		/*
		 BitMatrix bitMatrix = new MultiFormatWriter().encode(text, BarcodeFormat.QR_CODE, width, height,hints);  //生成二维码 
		 File outputFile = new File("d://new.png"); //定义图片存储的位置（D区根目录）和文件名(new.png)
		 MatrixToImageWriter.writeToFile(bitMatrix, format, outputFile);  //将二维码写入图片文件
		 */ 		
	}
}
