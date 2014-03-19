package com.storeserver.action.base;

import com.opensymphony.xwork2.ActionSupport;
import com.storeserver.service.StoreInfoManager;

public class BaseAction extends ActionSupport {
	
	protected StoreInfoManager mManager;

	public void setMgr(StoreInfoManager mgr)
	{
		this.mManager = mgr;
	}
}
