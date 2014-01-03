package com.fishjam.android.study;
import junit.framework.TestCase;

/***************************************************************************************************************************************
* GoogleMap
*   使用步骤:
*     1.创建项目时 Build Target 要选择"GoogleAPIs", 即添加 maps.jar
*     2.选择"AVD"时要使"Target"为"GoogleAPIs", 
*     3.AndroidManifest.xml 中
*       添加 <uses-library android:name="com.google.android.maps" />
*       声明权限: <uses-permission android:name="android.permission.INTERNET" />
*          是否需要这个? <uses-permission android:name="android.permission.ACCESS_MOCK_LOCATION" />
*     4.获得 "Google Map API key"：使用 JDK的keytool生成MD5 key，到google网站申请 ApiKey
*       http://code.google.com/intl/zh-cn/android/maps-api-signup.html
*     5.布局文件 <com.google.android.maps.MapView android:apiKey="xxxxxxx" />
*     6.继承 MapActivity, 里面使用 MapView 进行显示和控制
*   
*   要显示地图时，也可使用 ACTION_VIEW + Uri="geo：进度,维度" 的Intent方式
*
*   GPS(Global Position System) -- 24颗卫星在离地1.2W公里的高空，以12小时的周期环绕地球运行。
*     任意时刻，在地面上的任意一点都至少可以同时观测到4颗以上的卫星。根据卫星到接收机的距离解出观测点的位置。
*     LocationManager -- 提供了系统位置访问的方法，如定位、跟踪(requestLocationUpdates)、趋近告警(addProximityAlert)等.
*       locationManager = (LocationManager)Context.getSystemService(Context.LOCATION_SERVICE);
*       getLastKnownLocation() -- 根据Provider获得位置信息
*     LocationProvider -- 定义了位置服务的提供方法(如 GPS设备或网络提供 ),设置相关属性，可通过 Criterial 类来设置条件
*       
*     LocationListener -- 位置变化回调接口，可以发出 onLocationChanged、onProviderDisabled、onStatusChanged 等回调方法
*     Location -- 位置信息
*       getLatitue -- 获得经度
*       getLongitude -- 获得维度
*   
*   邮件
*     1.内置 Gmail 引擎收发邮件
*     2.使用 SMTP 来收发
 **************************************************************************************************************************************/

public class GoogleAPITester  extends TestCase{

}
