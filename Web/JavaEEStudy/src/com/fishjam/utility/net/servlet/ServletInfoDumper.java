package com.fishjam.utility.net.servlet;

import java.util.Enumeration;
import java.util.Map;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;

import com.fishjam.utility.dumper.BaseInfoDumper;

public class ServletInfoDumper {

	public static String RequestToString(HttpServletRequest request, String strDivide) {
		if (request != null) {
			StringBuilder sb = new StringBuilder();
			
			sb.append("CharacterEncoding=" + request.getCharacterEncoding() + strDivide);
			
			//TODO: java.net.URLEncoder.encode/URLDecoder -- 对QueryString 进行编解码
			//查询字符串，通过 split("&") 拆分出参数列表;  split("="); 拆分出键值对
			sb.append("QueryString=" + request.getQueryString() + strDivide);
			
			//获取客户请求的页面，在服务器上以 "/" 开始的地址
			sb.append("getServletPath=" + request.getServletPath());	
			sb.append("getRequestURI=" + request.getRequestURI());
			sb.append("getRequestURL=" + request.getRequestURL());
			
			//获取所有的请求参数(input)， getParameterValues 根据名字获取多值类型的值(如 CheckBox 选择的多个值)
			Map<String, String[]> parmeterMap = request.getParameterMap();
			String parmeterMapString = BaseInfoDumper.MapToString(parmeterMap);
			sb.append("getParameterMap = { "  + parmeterMapString + " }" + strDivide);
			
			//获取所有的参数
			Enumeration<String> paramNames= request.getParameterNames();
			while (paramNames.hasMoreElements()) {
				String headerName = paramNames.nextElement();
				sb.append(headerName + "-->" + request.getHeader(headerName) + strDivide);
			}

			//获取属性
			Enumeration<String> attributeNames = request.getAttributeNames();
			while (attributeNames.hasMoreElements()) {
				final String strKey = attributeNames.nextElement();
				sb.append(strKey + "=" + request.getAttribute(strKey) + strDivide);
			}

			//获取所有请求头的名称 -- 注意：如果是 submit 的结果，则其 referer 会指向提交表单的URL
			Enumeration<String> headerNames =request.getHeaderNames();
			while (headerNames.hasMoreElements()) {
				 String  headerName = headerNames.nextElement();
				 sb.append(headerName + "=" + request.getHeader(headerName) + strDivide);
			}
			
			final Cookie[] cookies = request.getCookies();
			sb.append("Cookies: size=" + (cookies != null ? cookies.length : 0 ) + strDivide);
			//for (int i = 0; i < cookies.length; i++) 
			for (Cookie cookie : cookies) {
				sb.append(cookie.getName() + "["+ cookie.getValue() + "]" + strDivide);
			}

			return sb.toString();
		}
		return "";
	}
	
	public static String SessionToString(HttpSession session, String strDivide) {
		if (session != null) {
			
		}
		return "";
	}
}
