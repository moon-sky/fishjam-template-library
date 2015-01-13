package com.fishjam.android.study;
import android.test.AndroidTestCase;

/**************************************************************************************************************************************
 * xUtils( https://github.com/wyouflf/xUtils ) -- 一个快速开发框架，里面包含了很多实用的工具，分为几大模块:
 *   DbUtils -- orm框架，一行代码就可以进行增删改查; 支持事务(默认关闭); 自动加载外键关联实体，支持延时加载 等
 *   ViewUtils -- ioc框架，完全注解方式就可以进行UI，资源和事件绑定
 *   HttpUtils -- 支持同步，异步方式的请求; 持大文件上传，上传大文件不会oom。支持断点续传，随时停止下载任务，开始任务
 *   BitmapUtils -- 支持加载网络图片和本地图片; 内存管理使用lru算法，更好的管理bitmap内存
 *   
***************************************************************************************************************************************/
public class xUtilsTest {
	public void testDbUtils(){
		/*
		
		DbUtils db = DbUtils.create(this);
		
		//保存
		User user = new User(); //这里需要注意的是User对象必须有id属性，或者有通过@ID注解的属性
		db.save(user); // 使用saveBindingId保存实体时会为实体的id赋值
		
		//查找
		Parent Parent = db.findFirst(Selector.from(Parent.class).where("name","=","test"));
		
		*/
	}
	
	public void testHttpUtils(){
		/*
		HttpUtils http = new HttpUtils();
		
		//普通 GET 方法
		http.send(HttpRequest.HttpMethod.GET,  "http://www.lidroid.com", new RequestCallBack<String>(){
    		//回调实现
    	}
    	
    	//上传文件或POST提交数据
    	RequestParams params = new RequestParams();
    	params.addHeader("name", "value");
		params.addQueryStringParameter("name", "value");
    	params.addBodyParameter("file", new File("path"));
    	
    	http.send(HttpRequest.HttpMethod.POST, "uploadUrl", params, new RequestCallBack<String>() {
    		//回调实现
		}
    	
    	//下载 -- 支持断点续传
    	HttpHandler handler = http.download("downloadUrl", "localsavepath", 
    		true, // 如果目标文件存在，接着未完成的部分继续下载。服务器不支持RANGE时将从新下载。
    		true, // 如果从请求返回信息中获取到文件名，下载完成后自动重命名。
    		new RequestCallBack<File>() {
    		//回调实现
    	} 
		
		*/
	}
	
	public void testBitmapUtils(){
		/*
		BitmapUtils bitmapUtils = new BitmapUtils(this);
		
		bitmapUtils.display(testImageView, "http://bbs.lidroid.com/static/image/common/logo.png");		//加载网络图片
		bitmapUtils.display(testImageView, "/sdcard/test.jpg");					// 加载本地图片(路径以/开头， 绝对路径)
		bitmapUtils.display(testImageView, "assets/img/wallpaper.jpg");	// 加载assets中的图片(路径以assets开头)

		
		 */
	}
}
