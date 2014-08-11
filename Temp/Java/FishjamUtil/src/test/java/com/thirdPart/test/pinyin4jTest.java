package com.thirdPart.test;

/**********************************************************************************************************************************************
 * Pinyin4j -- 将汉字编程对应的拼音，以方便数据的处理，支持:
 *    同一汉字有多个发音
 *    拼音的格式化输出，比如第几声之类的
 *    支持简体中文、繁体中文转换为拼音 
 ***********************************************************************************************************************************************/
import net.sourceforge.pinyin4j.PinyinHelper;
import net.sourceforge.pinyin4j.format.HanyuPinyinOutputFormat;
import net.sourceforge.pinyin4j.format.HanyuPinyinToneType;
import net.sourceforge.pinyin4j.format.HanyuPinyinVCharType;
import net.sourceforge.pinyin4j.format.exception.BadHanyuPinyinOutputFormatCombination;


import org.junit.Test;

import junit.framework.TestCase;

public class pinyin4jTest extends TestCase {


	@Test
	public void testBasicUsage(){
		String[] pinyinArray =PinyinHelper.toHanyuPinyinStringArray('傅');
		//for (String string : pinyinArray) {
		//	System.out.println(" =" + string);
		//}
		assertEquals(1, pinyinArray.length);
		assertEquals("fu4", pinyinArray[0] );
		
		//如果toHanyuPinyinStringArray中的参数不是汉字，那么它会返回null
		pinyinArray = PinyinHelper.toHanyuPinyinStringArray('A');
		assertEquals(null, pinyinArray);
	}
	
	@Test
	public void testPolyphone(){	//测试多音字
		String[] pinyinArray =PinyinHelper.toHanyuPinyinStringArray('重');
		//for (String string : pinyinArray) {
		//	System.out.println(" =" + string);
		//}
		assertEquals(2, pinyinArray.length);
		assertEquals("zhong4", pinyinArray[0]);
		assertEquals("chong2", pinyinArray[1]);
	}
	
	@Test
	public void testFormat(){
		//支持拼音输出的格式化，比如，“黄”可以输出为“huang”、“huang2”、“huáng”等等 -- 具体参见文档
		HanyuPinyinOutputFormat format= new HanyuPinyinOutputFormat();
		format.setToneType(HanyuPinyinToneType.WITH_TONE_MARK);
		format.setVCharType(HanyuPinyinVCharType.WITH_U_UNICODE);
		
		try {
			String[] pinyinArray = PinyinHelper.toHanyuPinyinStringArray('黄', format);
			//for(int i = 0; i < pinyinArray.length; ++i)
			//{
			//         System.out.println(pinyinArray[i]);
			//}
			assertEquals("huáng", pinyinArray[0]);
		} catch (BadHanyuPinyinOutputFormatCombination e) {
			e.printStackTrace();
		}
	}
	
	@Test
	public void testSentence(){
		//测试转换长的句子，里面有中英文、标点符号等 <== 对于非汉字，Pinyin4j会自动输出null
		  Hanyu hanyu = new Hanyu();

          // 中英文混合的一段文字
          String str = "这是一段简单的 hello world 实例，外加数字(1)和标点符号(!=?) 等";
          String strPinyin = hanyu.getStringPinYin(str);
          //System.out.println(strPinyin);
          assertEquals("zhei shi yi duan jian dan de  hello world shi li ，wai jia shu zi (1)he biao dian fu hao (!=?) deng ", strPinyin);
	}
	
	class Hanyu
	{
		private HanyuPinyinOutputFormat format = null;
		private String[] pinyin;
		public Hanyu()
		{
			format = new HanyuPinyinOutputFormat();
			format.setToneType(HanyuPinyinToneType.WITHOUT_TONE);
			pinyin = null;
		}

		// 转换单个字符
		public String getCharacterPinYin(char c)
		{
			try
			{
				pinyin = PinyinHelper.toHanyuPinyinStringArray(c, format);
			}
			catch (BadHanyuPinyinOutputFormatCombination e)
			{
				e.printStackTrace();
			}
			// 如果c不是汉字，toHanyuPinyinStringArray会返回null

			if (pinyin == null)
				return null;
			// 只取一个发音，如果是多音字，仅取第一个发音
			return pinyin[0];
		}

		// 转换一个字符串
		public String getStringPinYin(String str)
		{
			StringBuilder sb = new StringBuilder();
			String tempPinyin = null;
			for (int i = 0; i < str.length(); ++i)
			{
				tempPinyin = getCharacterPinYin(str.charAt(i));
				if (tempPinyin == null)
				{
					// 如果str.charAt(i)非汉字，则保持原样
					sb.append(str.charAt(i));
				}
				else
				{
					sb.append(tempPinyin + " ");
				}
			}
			return sb.toString();
		}
	}
}
