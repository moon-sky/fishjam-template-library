package com.java.test;

/***********************************************************************************************************************************************
 * Java语言的内核基于 UNICODE 2.1 编码集，提供了对不同国家、不同语言的支持。
 *   资源(key-value 对)的搜索顺序:
 *      baseName_language_country.class
 *      baseName_language_country.properties
 *      baseName_language.class
 *      baseName_language.properties
 *      baseName.class
 *      baseName.properties
 *      
 * Java允许以类文件来作为资源文件 -- 将资源的 key-value 对存入 class 文件。TODO: 有什么好处?
 *   Java资源类的要求:
 *   1.类的名字必须是 baseName_language_country, 与属性文件的命名相似。
 *   2.该类必须继承 ListResourceBundle，并重写 getContents 方法，返回 key-value 对的Object(TODO: 二维？)数组
 *     如: public class MyResource_zh_CN extends ListResourceBundle{
 *     		private final Object resData[][] == { "msg", "hello {0}" };
 *     		public Object[][] getContents() { return resData; }
 *     }
 *  
 *  
 *  辅助类
 *     ResourceBundle -- 用于加载各个国家对应的语言资源包,
 *     Locale -- 封装一个特定的国家、区域的语言环境
 *     <Format> -- 通过 format() 格式化字符串; 通过 parse() 解析字符串
 *       MessageFormat -- 格式化带占位符的字符串
         NumberFormat -- 格式化数字， 通过静态的 getXxxInstance 获得实例
         DateFormat
 *  辅助工具
 *     native2ascii -- 
***********************************************************************************************************************************************/
import static org.junit.Assert.*;

import java.text.MessageFormat;
import java.util.Locale;
import java.util.ResourceBundle;

import org.junit.Test;

public class I18NTest {

	@Test
	public void testInternationalization(){
		//取得系统默认的国家语言环境
		Locale myLocale = Locale.getDefault();

		//根据指定国家/语言环境加载资源文件， TODO:依次查找  res_zh_CN.properties , res.properties 等 ?
		//TODO: 找不到文件路径 -- 应该如何放置?
		/*   
		ResourceBundle bundle = ResourceBundle.getBundle("res", myLocale);
		String strValue = bundle.getString("hello");
		assertEquals("你好", strValue);
		//*/
		
		assertTrue(true);
		Locale[] avaliableLocales =  Locale.getAvailableLocales();		//获取支持的国家和地区
//		for (Locale locale : avaliableLocales) {
//			locale.getDisplayCountry();
//			locale.getCountry();
//			locale.getDisplayLanguage();
//			locale.getLanguage();
//		}
	}
	
	@Test
	public void testMessageFormat(){
		//带占位符的字符串 -- {0} 表示第一个  
		String strLocalValue = "Hello {0}, You want to study {1}";
		String strFormated  = MessageFormat.format(strLocalValue, "fishjam", "Java");
		assertEquals("Hello fishjam, You want to study Java", strFormated);
		
	}
}
