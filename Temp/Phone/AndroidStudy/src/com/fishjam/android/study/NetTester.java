package com.fishjam.android.study;
import java.net.URL;

import android.graphics.BitmapFactory;
import android.test.AndroidTestCase;

/***************************************************************************************************************************************
 * Socket/ServerSocket -- TCP/IP编程中使用
 *   Socket client = server.accept(); //获得客户端套接字，然后 client.getOutputStream() 获得输出流，即可进行交互了
 *   InputStream in = socket.getInputStream();  byte[] buffer = new byte[in.available()];  in.read(buffer); String msg = new String(buffer);
 * DatagramSocket/DatagramPackage -- UDP
 * URL/URLConnection/HttpURLConnection 
 *   1.InputStream in = new URL(myUrl).openStream();  //打开Url对应的输入流(如 文件、网络等?) 
 *   2.1. 网络解析:
 *       URLConnection conn = url.openConnection(); InputStream in = conn.getInputStream();
 *       HttpURLConnection httpConn = (HttpURLConnection)conn; in = httpConn.getInputStream();
 *   2.2 本地图片解析
 *       bitmap = BitmapFactory.decodeStream(in);
 *   3. in.close();  //使用完毕后注意关闭
 * URLEncoder 
 *   encode -- ?
 * Servlet 
 *   HttpServlet -- 继承子类，重载其中的 doGet /doPost 等方法
 *   HttpServletRequest/HttpServletResponse -- 
 * Apache Http 客户端
 *   HttpGet/HttpPost，使用 DefaultHttpClient 执行请求获得响应
 *     HttpPost request = new HttpPost(urlStr); ... HttpResponse response = new DefaultHttpClient().execute(request);
 * WebService -- 使用单独下载的 KSOAP.jar ?
 * WebView -- 视图组件直接显示网页，其使用了开源的 WebKit 引擎
***************************************************************************************************************************************/
public class NetTester extends AndroidTestCase {
	public void TestGetCityListFromWebService(){
		/****************************************************************************************************************************************
		//通过WebService获取天气预报的城市列表
		List<String > lstCiteStrings = new ArrayList<String>();
		SoapObject request = new SoapObject("http://webXml.com.cn/", "getRegionProvince");
		//获得序列化的 envelope
		SoapSerializationEnvelope envelope = new SoapSerializationEnvelope(SoapEnvelope.VER11);
		envelope.bodyOut = request;
		(new MarshalBase64()).register(envelope);
		AndroidHttpTransport ht = new AndroidHttpTransport("http://webservice.webxml.com.cn/WebServices/WeatherWS.asmx");
		ht.debug = true;
		ht.call("http://webXml.com.cn/getRegionProvince", envelope);  //调用
		if (envelope.getResponse() != null) {
			String str = envelope.bodyIn.toString();
			if (str.length() > 0) {
				//字符串解析并加入结果
				lstCiteStrings.add("xxxx");
			}
		}
    ****************************************************************************************************************************************/
	}
}