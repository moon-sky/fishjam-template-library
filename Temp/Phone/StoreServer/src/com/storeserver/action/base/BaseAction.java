package com.storeserver.action.base;

import javax.servlet.ServletConfig;
import javax.servlet.ServletContext;

import org.apache.struts2.util.ServletContextAware;
import org.springframework.web.context.ServletConfigAware;

import com.opensymphony.xwork2.ActionSupport;
import com.storeserver.service.StoreInfoManager;

public class BaseAction extends ActionSupport implements ServletContextAware {
	protected StoreInfoManager mManager;
	protected ServletContext mServletContext;
	
	@Override
	public void setServletContext(ServletContext servletContext) {
			mServletContext = servletContext;
	}
	
	public ServletContext getServletContext() {
		return mServletContext;
	}

	public void setMgr(StoreInfoManager mgr)
	{
		this.mManager = mgr;
	}
	
	
}
