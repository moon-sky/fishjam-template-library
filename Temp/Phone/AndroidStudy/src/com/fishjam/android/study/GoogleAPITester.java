package com.fishjam.android.study;
import junit.framework.TestCase;

/***************************************************************************************************************************************
* GoogleMap
*   使用步骤:
*     1.创建项目时 Build Target 要选择"GoogleAPIs", 即添加 map.jar
*     2.选择"AVD"时要使"Target"为"GoogleAPIs", 
*     3.AndroidManifest.xml 中
*       添加 <uses-library android:name="com.google.android.maps" />
*       声明权限: <uses-permission android:name="android.permission.INTERNET" />
*          是否需要这个? <uses-permission android:name="android.permission.ACCESS_MOCK_LOCATION" />
*     4.获得 "Google Map API key"：使用 JDK的keytool生成MD5 key，到google网站申请 ApiKey
*       http://code.google.com/intl/zh-cn/android/maps-api-signup.html
*     5.布局文件 <com.google.android.maps.MapView android:apiKey="xxxxxxx" />
*     5.继承 MapActivity, 里面使用 MapView 进行显示和控制
*      
 **************************************************************************************************************************************/

public class GoogleAPITester  extends TestCase{

}
