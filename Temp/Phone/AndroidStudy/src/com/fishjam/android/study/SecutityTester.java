package com.fishjam.android.study;
import java.io.ByteArrayInputStream;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;

import android.content.ComponentName;
import android.content.pm.PackageManager;
import android.test.AndroidTestCase;

/**************************************************************************************************************************************
 * 爱加密的Blog系列(很多加解密的文章) -- http://www.eoeandroid.com/thread-314366-1-1.html
 * 
 * Google加密方式
 *    Google对Android APP的安全处理是给APK简单加密、java层源码加壳保护，对核心so库、资源文件、主配文件、
 *    第三方架包却没有任何保护处理。所做的保护也早已被黑客攻破但并没有做任何升级维护导致Android APP现在没有任何安全性。
 * 爱加密 软件 -- 以源码加壳保护、so库核心代码加壳保护、资源文件签名保护、APK防二次打包保护等方面来对APK进行全方面保护
 * 内存修改器 -- 如 八门神器, 通过修改正在运行的游戏的内存数据，可方便的修改游戏的HP、MP、生命数等等参数.
 *                    防护：多数据关联来保护应用的重要数据不被修改
 * 
 * 
 * Apk反编译(https://code.google.com/p/innlab/downloads/list)
 *   1.将 .apk 文件后缀改为 .zip 并解压，解压后的目录结构
 *     assets\ -- 存放xml配置文件和图片文件，一般经过加密无法直接查看
 *     META-INF\ -- 存放签名的文件夹
 *     res\
 *     classes.dex -- 源码的加密文件
 *     resources.arsc --
 *   2.使用 dex2jar ( http://code.google.com/p/dex2jar/ ) 把 classes.dex 文件转为 .jar 文件
 *      dex2jar.bat classes.dex  后会生成 classes_dex2jar.jar
 *   3.将 jar 解压 或使用 jd-gui 查看文件 
 * 
 * ApkTool(https://code.google.com/p/android-apktool/) -- 反编译生成程序的源代码和图片、XML配置、语言资源
 *   反编译： apktool.bat d -f <apk文件> [输出文件夹] -- 
 *                反编译后其内部的 .smali 文件就是 davlik 源码? 
 *   重新打包:  apktool.bat  b <文件夹>  -- 通常可用于修改后重新打包？
 * 
 * APK改之理(ApkIde) -- http://pan.baidu.com/share/link?shareid=164149&uk=3291471913#dir/path=%2Fmysoft%2Fapkide
 *   
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * 签名
 *   
 * 对APK文件进行签名(Jarsigner.exe)
 *   如：Jarsigner.exe -keystore [签名文件路径, 如fishjam.keystore] -storepass [密码] -verbose [需要签名文件路径, 如 MyApp.apk] [签名文件别名] 
 *   参数说明： 
 *      -keystore <url> -- 密钥库位置
 *      -storepass <口令> -- 用于密钥库完整性的口令
 *      -storetype <类型> -- 密钥库类型
 *      -keypass <口令> -- 专用密钥的口令（如果不同）
 *      -signedjar <文件> --  已签名的 JAR 文件的名称 
 *      -tsa <url> -- 时间戳机构的位置
 *      -tsacert <别名> -- 时间戳机构的公共密钥证书
 *      -altsigner <类> -- 替代的签名机制的类名
 *      -providerName <名称> -- 提供者名称
 *      -providerClass <类> -- 加密服务提供者的名称
 *      
 * 签名保护 -- 程序运行时获取当前应用的签名并进行比对判断，如不一致则终止程序，防止二次打包。
 *   APK的唯一识别是依靠包名和签名来做鉴定的。
 *   
 * 反编译后植入有米广告(其有 SDK)
 *   1.下载SDK， 如 http://static.youmi.net/files/sdk/YoumiSdk_v4.04_2013-07-12.zip
 *   2.在 AndroidManifest.xml 中加入 SYSTEM_ALERT_WINDOW 和 GET_TASKS 的权限
 *   3.在 AndroidManifest.xml 中加入 对应的代码(参看SDK，有 Activity, Service, Receiver 等 )
 *   
 * 源码混淆 -- 将代码中的所有变量、函数、类的名称加密为简短的英文字母代号，在APP被破解后增加破解者对代码的阅读难度
 * 
 * 伪加密(Android 4.2.x 之前) -- 通过java代码对APK(压缩文件)进行伪加密，其修改原理是修改连续4位字节标记为"P K 01 02"的后第5位字节，奇数表示不加密偶数表示加密。
 *   伪加密后的APK不但可以防止PC端对它的解压和查看，也同样能防止反编译工具编译。
 *   但是伪加密对其APK加密后市场也无法对其进行安全检测，部分市场会拒绝这类APK上传市场(Android 4.2.x后的系统无法安装)
 *   
**************************************************************************************************************************************/

public class SecutityTester extends AndroidTestCase {
	public void testRemoteHideOrShowAppIcon(){
		//远程控制应用图标的显示与隐藏
		
		/*
		private void setComponentEnabled(Class<?> clazz, boolean enabled) {
			final ComponentName c = new ComponentName(this, clazz.getName());
			getPackageManager().setComponentEnabledSetting(c, enabled? PackageManager.COMPONENT_ENABLED_STATE_ENABLED:PackageManager.COMPONENT_ENABLED_STATE_DISABLED,PackageManager.DONT_KILL_APP)
		}
		*/
	}
	
	public void testSignProtect(){
		//获取当前程序的签名
		/*
		PackageInfo packageInfo = getPackageManager().getPackageInfo(getPackageName(), PackageManager.GET_SIGNATURES);
		Signature[] signs = packageInfo.signatures;
		Signature sign = signs[0];
		CertificateFactory certFactory = CertificateFactory.getInstance("X.509");
		X509Certificate cert = (X509Certificate)certFactory.generateCertificate(new ByteArrayInputStream(sign.toByteArray()));
		byte[] buffer =cert.getEncoded();  //获得APK签名的MD5值，通过比对该值和正确签名的MD5值进行对比，即可识别是否被盗版
		*/
		//对应的 smali代码如下(可通过 ApkIde 搜索并修改调用逻辑，即可破解掉签名保护 )：  
		//  Landroid/content/pm/PackageInfo;->signatures:[Landroid/content/pm/Signature”
		
	}
}