package com.storeserver.action;
import java.util.List;

import javax.servlet.http.HttpServletRequest;

import org.apache.struts2.interceptor.ServletRequestAware;

import com.fishjam.utility.net.servlet.ServletInfoDumper;
import com.opensymphony.xwork2.ActionContext;
import com.storeserver.action.base.BaseAction;
import com.storeserver.domain.Store;
import com.storeserver.domain.StoreMapInfo;

public class MapInfoAction extends BaseAction implements ServletRequestAware{
	HttpServletRequest mRequest;
	private List<StoreMapInfo> storeMaps;
	
	public List<StoreMapInfo> getStoreMaps() {
		return storeMaps;
	}

	public void setStoreMaps(List<StoreMapInfo> storeMaps) {
		this.storeMaps = storeMaps;
	}

	/**
	 * @return
	 */
	public String execute() {
		String strStoreDbName = mRequest.getParameter("stroreDbName");
		//int storeId = 0;
		if (strStoreDbName != null) {
			//storeId = Integer.parseInt(strStoreId, 10);
			storeMaps = mManager.getStoreMaps(this, strStoreDbName);
			ActionContext.getContext().getSession().put("stroreDbName", strStoreDbName);
			return SUCCESS;
		}
		return INPUT;
		//String strRequest = ServletInfoDumper.RequestToString(mRequest, "\n");
		//System.out.println(strRequest);
	}

	@Override
	public void setServletRequest(HttpServletRequest request) {
		mRequest = request;
	}
}