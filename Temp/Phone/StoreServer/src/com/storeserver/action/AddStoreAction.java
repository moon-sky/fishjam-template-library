package com.storeserver.action;

import com.storeserver.action.base.BaseAction;

public class AddStoreAction extends BaseAction {

	/**
	 * @return
	 */
	public String execute() {
		mManager.getStores();
		return SUCCESS;
	}
}