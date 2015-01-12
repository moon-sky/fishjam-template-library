package com.fishjam.utility.common;

//常见的正则表达式
public class RegexUtil {
	/**
	 * 手机号
	 */
	private static final String REG_MOBILE = "^1[3|4|5|8][0-9]{9}$";
	/**
	 * 身份证号
	 */
	private static final String REG_ID_CARD_NO = "^(\\d{15}|\\d{18}|\\d{17}[x|X])$";
	/**
	 * 车牌号
	 */
	private static final String REG_CAR_NO = "^[\u4e00-\u9fa5]{1}[A-Z]{1}[A-Z_0-9]{5}$";
	/**
	 * QQ
	 */
	private static final String REG_QQ = "^[1-9]?[1-9][0-9]{4,13}$";
	/**
	 * 邮箱
	 */
	private static final String REG_EMAIL = "^([a-zA-Z0-9_\\.\\-\\+])+\\@(([a-zA-Z0-9\\-])+\\.)+([a-zA-Z0-9]{2,4})+$";
	/**
	 * 金额
	 */
	private static final String REG_MONEY = "^(([1-9]\\d*)|0)(\\.\\d{1,2})?$";
	/**
	 * 银行卡号
	 */
	private static final String REG_BANK_CARD_NO = "^[1-9]?[1-9][0-9]{1,18}$";
	/**
	 * 网址
	 */
	private static final String REG_WEB_URL = "^(\\w+:{0,1}\\w*@)?(\\S+)(:[0-9]+)?(\\/|\\/([\\w#!:.?+=&%@!\\-\\/]))?$";
	/**
	 * IP地址
	 */
	private static final String REG_IPV4 = "^(25[0-5]|2[0-4]\\d|[0-1]?\\d?\\d)(\\.(25[0-5]|2[0-4]\\d|[0-1]?\\d?\\d)){3}$";
	private static final String REG_IPV6_STD = "^(?:[0-9a-fA-F]{1,4}:){7}[0-9a-fA-F]{1,4}$";
	private static final String REG_IPV6_HEX_COMPRESSED = "^((?:[0-9A-Fa-f]{1,4}(?::[0-9A-Fa-f]{1,4})*)?)::((?:[0-9A-Fa-f]{1,4}(?::[0-9A-Fa-f]{1,4})*)?)$";

	
	/**
	 * 普通数字(非负整数)
	 */
	private static final String REG_COMMON_DIGIT = "^([1-9]\\d*)|0$";
	
	/**
	 * 正整数
	 */
	
	private static final String REG_POSITIVE_INTEGERS = "^[1-9]\\d*$";
	/**
	 * 英文字母和数字
	 */
	private static final String REG_LETTER_NUMBER = "^[A-Za-z0-9]+$";
	/**
	 * 小数
	 */
	private static final String REG_DECIMAL = "^(-?(([1-9]\\d*)|0))(\\.\\d+)?$";
	/**
	 * 汉字
	 * TODO: 中文字符 -- [\\u0041-\\u0056]
	 */
	private static final String REG_CHINESE_CHARACTER = "^[\u4e00-\u9fa5]{0,}$";
	
	/**
	 * 日期判断
	 */
	private static final String REG_DATE = "^((\\d{2}(([02468][048])|([13579][26]))[\\-\\/\\s]?((((0?[13578])|(1[02]))[\\-\\/\\s]?((0?[1-9])|([1-2][0-9])|(3[01])))|(((0?[469])|(11))[\\-\\/\\s]?((0?[1-9])|([1-2][0-9])|(30)))|(0?2[\\-\\/\\s]?((0?[1-9])|([1-2][0-9])))))|(\\d{2}(([02468][1235679])|([13579][01345789]))[\\-\\/\\s]?((((0?[13578])|(1[02]))[\\-\\/\\s]?((0?[1-9])|([1-2][0-9])|(3[01])))|(((0?[469])|(11))[\\-\\/\\s]?((0?[1-9])|([1-2][0-9])|(30)))|(0?2[\\-\\/\\s]?((0?[1-9])|(1[0-9])|(2[0-8]))))))";
	
		
	/**
	 * @Description:验证手机号
	 */
	public static boolean validateMobile(String content) {
		return validate(content,REG_MOBILE);
	}
	
	/**
	 * @Description:验证身份证号
	 */
	public static boolean validateIdCardNo(String content) {
		return validate(content,REG_ID_CARD_NO);
	}
	
	/**
	 * @Description:验证QQ
	 */
	public static boolean validateQQ(String content) {
		return validate(content,REG_QQ);
	}
	
	/**
	 * @Description:验证邮箱
	 */
	public static boolean validateEmail(String content) {
		return validate(content,REG_EMAIL);
	}
	
	/**
	 * @Description:验证车牌号
	 */
	public static boolean validateCarNo(String content) {
		return validate(content,REG_CAR_NO);
	}

	/**
	 * @Description:验证金额
	 */
	public static boolean validateMoney(String content) {
		return validate(content,REG_MONEY);
	}
	
	/**
	 * @Description:验证银行卡号
	 */
	public static boolean validateBankCardNo(String content) {
		return validate(content,REG_BANK_CARD_NO);
	}
	
	/**
	 * @Description:验证网址
	 */
	public static boolean validateWebUrl(String content) {
		return validate(content,REG_WEB_URL);
	}
	
	/**
	 * @Description:验证IP地址
	 */
	public static boolean validateIP(String content) {
		if(content == null || content.isEmpty()){
			return false;
		}
		return content.matches(REG_IPV4) || content.matches(REG_IPV6_STD) 
				|| content.matches(REG_IPV6_HEX_COMPRESSED);
	}
	
	/**
	 * @Description:验证普通数字(非负整数)
	 */
	public static boolean validateCommonDigit(String content) {
		return validate(content,REG_COMMON_DIGIT);
	}
	
	/**
	 * @Description:验证英文字母和数字
	 */
	public static boolean validateLetterAndNumber(String content) {
		return validate(content,REG_LETTER_NUMBER);
	}
	
	/**
	 * @Description:验证小数
	 */
	public static boolean validateDecimal(String content) {
		return validate(content,REG_DECIMAL);
	}
	
	/**
	 * @Description:验证汉字
	 */
	public static boolean validateChineseCharacter(String content) {
		return validate(content,REG_CHINESE_CHARACTER);
	}
	
	/**
	 * 验证正整数(可判断不为空，且大于0的数true)(用于Long\Integer)
	 */
	public static boolean validPosiDigit(Object content) {
		return validate(content+"",REG_POSITIVE_INTEGERS);
	}
	
	/**
	 * 验证非正整数(空、小于等于0=true)(用于Long\Integer)
	 */
	public static boolean validNotPosiDigit(Object content) {
		try {
			return !validate(content+"",REG_POSITIVE_INTEGERS);
		} catch (Exception e) {
			return false;
		}
	}
	
	/**
	 * @Description:验证日期
	 */
	public static boolean validateDate(String content) {
		return validate(content,REG_DATE);
	}
	
	private static boolean validate(String content,String reg) {
		if (content == null || content.isEmpty()) {
			return false;
		}
		return content.matches(reg);
	}
}
