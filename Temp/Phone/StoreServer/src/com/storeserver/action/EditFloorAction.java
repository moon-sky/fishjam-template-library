package com.storeserver.action;

import javax.servlet.http.HttpServletRequest;

import org.apache.struts2.interceptor.ServletRequestAware;

import com.fishjam.utility.net.servlet.ServletInfoDumper;
import com.opensymphony.xwork2.ActionContext;
import com.storeserver.action.base.BaseAction;
import com.storeserver.domain.StoreMapInfo;

public class EditFloorAction extends BaseAction implements ServletRequestAware {
	private HttpServletRequest mRequest;
	private StoreMapInfo mStoreMapInfo;
	
	public StoreMapInfo getStoreMapInfo() {
		return mStoreMapInfo;
	}

	@Override
	public void setServletRequest(HttpServletRequest request) {
		this.mRequest = request;
	}

	@Override
	public String execute() throws Exception {
		System.out.println(ServletInfoDumper.RequestToString(mRequest, "\n"));
		
		//
		return SUCCESS;
	}
	public String edit() throws Exception{
		System.out.println(ServletInfoDumper.RequestToString(mRequest, "\n"));
		String strMapId = mRequest.getParameter("mapId");
		String strStoreDbName = (String)ActionContext.getContext().getSession().get("stroreDbName");
		if (strMapId != null && strStoreDbName != null) {
			int nMapId = Integer.parseInt(strMapId);
			
			mStoreMapInfo = mManager.getStoreMapById(this, strStoreDbName, nMapId);
			return SUCCESS;
		}
		return INPUT;
	}
}
