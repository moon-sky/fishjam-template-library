package com.storeserver.action;

import java.util.ArrayList;
import java.util.List;




import com.storeserver.action.base.BaseAction;
import com.storeserver.business.StoreBean;

public class StoreListAction extends BaseAction {
	
	private List<StoreBean> stores;
	
	public List<StoreBean> getStores() {
		return stores;
	}

	public void setStores(List<StoreBean> stores) {
		this.stores = stores;
	}

	@Override
	public String execute() throws Exception {
		System.out.println("enter StoreListAction execute");
		
		stores = new ArrayList<StoreBean>();
		for (int i = 1; i <= 5; i++) {
			StoreBean sBean = new StoreBean();
			sBean.setId(i);
			sBean.setName("StoreName" + i);
			sBean.setDbName("StoreDBName" + i);
			sBean.setPosition("Position" + i);
			stores.add(sBean);
		}

		return SUCCESS;
	}
}
