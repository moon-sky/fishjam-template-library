package com.fishjam.utility.net.servlet;

import java.io.IOException;
import java.util.Collection;
import java.util.Enumeration;
import java.util.Map;

import javax.servlet.ServletException;
import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;
import javax.servlet.http.Part;

import com.fishjam.utility.dumper.BaseInfoDumper;

public class ServletInfoDumper {

	//当访问如下地址时： http://localhost:8888/StoreServer/Pos/12345678?name=abc&pass=123
	public static String RequestToString(HttpServletRequest request, String strDivide) {
		if (request != null) {
			StringBuilder sb = new StringBuilder();

			sb.append("CharacterEncoding=" + request.getCharacterEncoding() + strDivide);
			
			//TODO: java.net.URLEncoder.encode/URLDecoder -- 对QueryString 进行编解码
			//查询字符串，通过 split("&") 拆分出参数列表;  split("="); 拆分出键值对
			sb.append("QueryString=" + request.getQueryString() + strDivide);		//-- name=abc&pass=123
			
			sb.append("getContextPath=" + request.getContextPath() + strDivide);	//-- /StoreServer"
			//获取客户请求的页面，在服务器上以 "/" 开始的地址
			sb.append("getServletPath=" + request.getServletPath() + strDivide);			//-- /Pos/12345678
			sb.append("getRequestURI=" + request.getRequestURI()+ strDivide);		//-- /StoreServer/Pos/12345678
			sb.append("getRequestURL=" + request.getRequestURL()+ strDivide);		//-- http://localhost:8888/StoreServer/Pos/12345678
			
			//获取所有的请求参数(input)， getParameterValues 根据名字获取多值类型的值(如 CheckBox 选择的多个值)
			Map<String, String[]> parmeterMap = request.getParameterMap();
			sb.append("ParameterMap:" + parmeterMap + strDivide);
			
			sb.append("getParameterMap = { "  + BaseInfoDumper.MapToString(parmeterMap) + " }" + strDivide);
			
			//获取所有的参数
			Enumeration<String> paramNames= request.getParameterNames();
			sb.append("ParameterNames:" + paramNames + strDivide);
			while (paramNames.hasMoreElements()) {
				String param = paramNames.nextElement();
				sb.append("  " + param + "-->" + request.getParameter(param) + strDivide);
			}

			//获取属性
			Enumeration<String> attributeNames = request.getAttributeNames();
			sb.append("AttributeNames:" + attributeNames + strDivide);
			while (attributeNames.hasMoreElements()) {
				final String strKey = attributeNames.nextElement();
				sb.append(strKey + "=" + request.getAttribute(strKey) + strDivide);
			}

			//获取所有请求头的名称 -- 注意：如果是 submit 的结果，则其 referer 会指向提交表单的URL
			Enumeration<String> headerNames =request.getHeaderNames();
			sb.append("headerNames:" + headerNames + strDivide);
			while (headerNames.hasMoreElements()) {
				 String  headerName = headerNames.nextElement();
				 sb.append("  " + headerName + "=" + request.getHeader(headerName) + strDivide);
			}
			
			final Cookie[] cookies = request.getCookies();
			if(cookies != null){
				sb.append("Cookies: size=" + (cookies != null ? cookies.length : 0 ) + strDivide);
				//for (int i = 0; i < cookies.length; i++) 
				for (Cookie cookie : cookies) {
					sb.append("  " + cookie.getName() + "["+ cookie.getValue() + "]" + strDivide);
				}
			}
			
			Collection<Part> parts;
			try {
				parts = request.getParts();
				sb.append("parts: " + parts);
				if (parts != null) {
					for (Part part : parts) {
						sb.append("File: " +  part.getName() + ", size=" + part.getSize() + strDivide);
						//实际上真实存储时，应该通过 java.util.UUID 工具类生成唯一文件名
						//part.write(request.getServletContext().getRealPath("/uploadFiles") + "/" + part.getName());
					}
				}
			} catch (IllegalStateException | IOException | ServletException e) {
				e.printStackTrace();
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
