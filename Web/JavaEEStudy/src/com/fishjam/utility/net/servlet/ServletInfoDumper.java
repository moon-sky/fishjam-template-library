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
			sb.append("QueryString=" + request.getQueryString() + strDivide);
			//TODO: URLEncoder/URLDecoder -- 对QueryString 进行编解码
			
			
			//获取所有的请求参数(input)
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

			//获取所有请求头的名称
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
