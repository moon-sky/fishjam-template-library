package com.java.test;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import static org.junit.Assert.*;
import org.junit.Test;


/**********************************************************************************************************************************************
 * StringTokenizer -- 一个处理字符串的工具类，但功能不如正则强大。
 * 
 * 正则表达式 -- String类增加了正则表达式支持,如 matches, replaceAll, split 等方法
 *   圆括号 -- 将多个表达式组成子表达式，其中可以使用 或("|") 运算符;
 *   
 *  边界匹配符:
 *    ^ -- 行开头
 *    $ -- 行结尾
 *    。。。
 *  
 *  数量标识符, 有几种模式
 *     Greedy(贪婪模式) -- 默认模式，会一直匹配下去，直到无法匹配为止;
 *     Reluctant(勉强模式) -- 用问号后缀(?)表示，只会匹配最少的字符
 *     Possessive(占有模式) -- 用加号后缀(+)表示，目前只有Java支持，较少使用
 *     
***********************************************************************************************************************************************/

/***********************************************************************************************************************************************
 * Pattern -- 正则表达式编译后在内存中的表示形式。执行匹配所涉及的状态保留在 Matcher 对象中。是不可变类，可供多个并发线程安全使用。
 * Matcher
 *   find -- 返回目标字符串中是否包含与Pattern匹配的子串
 *   group -- 返回上一次与Pattern匹配的子串
 *   start -- 返回上一次与Pattern匹配的子串在目标字符串中的开始位置
 *   lookingAt -- 返回目标字符串前面部分与Pattern是否匹配
 *   matches -- 返回整个目标字符串与Pattern是否匹配
 *   reset -- 可将现有的Matcher对象应用于一个新的字符序列
***********************************************************************************************************************************************/
/***********************************************************************************************************************************************
 * 常见正则表达式字符串:  参见 CommonRegex.java 
***********************************************************************************************************************************************/

public class RegexTest {
	public static final String REGEX_MAIL_STRING = "\\w{3,20}@\\w+\\.(com|org|cn|net|gov)";
	
	@Test
	public void testStringRegex(){
		assertTrue(true);
		//TODO: 把预设值的检测结果放入 checkMails -- 支持错误邮件地址的确认
		String [] checkMails = {
				"fishjam@163.com",
				"fishjam@163.net"
		};
		
		Pattern mailPattern = Pattern.compile(REGEX_MAIL_STRING);
		for (String strMail : checkMails) {
			Matcher matcher = mailPattern.matcher(strMail);
			assertTrue(matcher.matches());
		}
		
	}
	@Test
	public void testPattern(){
		//内部自动把字符串编译成匿名的Pattern对象，并执行匹配，通常用在只使用一次的情况
		assertTrue(Pattern.matches("a*b", "aaaaab"));

		Pattern	pattern = Pattern.compile("a*b");
		Matcher matcher = pattern.matcher("aaaaab");
		assertTrue(matcher.matches());
		
	}
	
	@Test
	public void testMatcher(){
		List<String> listMatcher = new ArrayList<String>();
		Matcher matcher = Pattern.compile("\\w+").matcher("Java is very easy!"); 		//按词进行匹配
		while (matcher.find()) {
			listMatcher.add(matcher.group());
		}
		assertEquals(4, listMatcher.size());
		assertEquals("[Java, is, very, easy]", listMatcher.toString());
	}
}
