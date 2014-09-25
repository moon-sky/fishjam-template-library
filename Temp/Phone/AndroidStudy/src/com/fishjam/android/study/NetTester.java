package com.fishjam.android.study;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.URL;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.util.EntityUtils;

import android.graphics.BitmapFactory;
import android.test.AndroidTestCase;
import android.text.Html.TagHandler;
import android.util.Log;

/***************************************************************************************************************************************
 * 注意： 
 *   1.网络相关的操作必须在线程中调用(主线程中调用的话，会抛出 xxx 异常 )
 * 
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
 * 
 * 
***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * Json
 *   JSONArray
 *   JSONObject
 *   JSONStringer
 *   JSONTokener
***************************************************************************************************************************************/


public class NetTester extends AndroidTestCase {
	private static final String TAG = NetTester.class.getName();
	

	public void testGetCityListFromWebService(){
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
	
	public void testHttpClientGet() throws Exception{
		final String GET_URL = "http://www.baidu.com/";

		// 创建HttpClient对象
		HttpClient httpClient = new DefaultHttpClient();

		// 创建HttpGet对象。
		HttpGet get = new HttpGet(GET_URL);
		// 发送GET请求
		HttpResponse httpResponse = httpClient.execute(get);
		if (httpResponse.getStatusLine().getStatusCode() == 200) {
			// 获取服务器响应字符串
			String result = EntityUtils.toString(httpResponse.getEntity());
			Log.i(TAG, result);
		}
	}
	
	public void testHttpClientPost() throws Exception {
		final String POST_URL = "http://www.baidu.com/";
		Map<String ,String> rawParams = new HashMap<String, String>();  //请求参数
		rawParams.put("user", "fishjam");
		rawParams.put("pass", "password");
		
		HttpClient httpClient = new DefaultHttpClient();		// 创建HttpClient对象
		HttpPost post = new HttpPost(POST_URL);		// 创建HttpPost对象。
		
		// 如果传递参数个数比较多的话可以对传递的参数进行封装
		List<NameValuePair> params = new ArrayList<NameValuePair>();
		for(String key : rawParams.keySet())
		{
			params.add(new BasicNameValuePair(key, rawParams.get(key)));	//封装请求参数
		}
		post.setEntity(new UrlEncodedFormEntity(params, "UTF-8"));		// 设置请求参数
		HttpResponse httpResponse = httpClient.execute(post);		// 发送POST请求
		
		// 如果服务器成功地返回响应
		if (httpResponse.getStatusLine().getStatusCode() == 200)
		{
			// 获取服务器响应字符串
			String result = EntityUtils.toString(httpResponse.getEntity());
			Log.i(TAG, result);
		}
	}
}