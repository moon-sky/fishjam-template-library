package com.storeserver.action;

import java.util.List;

import com.storeserver.action.base.BaseAction;
import com.storeserver.domain.Store;

public class StoreListAction extends BaseAction {
	
	private List<Store> stores;
	
	public List<Store> getStores() {
		return stores;
	}

	public void setStores(List<Store> stores) {
		this.stores = stores;
	}

	@Override
	public String execute() throws Exception {
		stores =  mManager.getStores(this);

		return SUCCESS;
	}
}
