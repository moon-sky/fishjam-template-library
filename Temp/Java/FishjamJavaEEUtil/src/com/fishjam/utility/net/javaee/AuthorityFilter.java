package com.fishjam.utility.net.javaee;

import java.io.IOException;

import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.annotation.WebFilter;
import javax.servlet.annotation.WebInitParam;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;

/*
@WebFilter(filterName="authority", urlPatterns={"/*"},
	initParams = {
		@WebInitParam(name="encoding", value="UTF-8"),
		@WebInitParam(name="loginPage", value="/login.jsp"),
		@WebInitParam(name="proLogin", value="/proLogin.jsp")
	}
)
*/

public class AuthorityFilter implements Filter {

	private FilterConfig mConfig;
	@Override
	public void init(FilterConfig config) throws ServletException {
		mConfig = config;
	}

	@Override
	public void doFilter(ServletRequest request, ServletResponse response,
			FilterChain chain) throws IOException, ServletException {
		//获取配置参数
		String encoding = mConfig.getInitParameter("encoding");
		String loginPage = mConfig.getInitParameter("loginPage");
		String proLogin = mConfig.getInitParameter("proLogin");
		
		//统一设置 request 编码用的字符集
		request.setCharacterEncoding(encoding);
		
		HttpServletRequest hRequest = (HttpServletRequest)request;
		HttpSession session = hRequest.getSession();
		//获取客户请求的页面，在服务器上以 "/" 开始的地址
		String requestPath = hRequest.getServletPath();
		
		//如果session范围的user为null，即表明没有登录；
		//如果用户请求的既不是登陆页面，也不是处理登录的页面
		if (session.getAttribute("user") == null
				&& !requestPath.endsWith(loginPage)
				&& !requestPath.endsWith(proLogin)) {
			request.setAttribute("tip", "尚未登录");
			//forward到登录页面
			request.getRequestDispatcher(loginPage).forward(request, response);
		}
		else {
			//放行默认的请求
			chain.doFilter(request, response);
		}
	}

	@Override
	public void destroy() {
		this.mConfig = null;		
	}


}
