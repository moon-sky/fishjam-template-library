package com.storeserver.service;

import java.util.List;

import com.storeserver.domain.Store;
import com.storeserver.business.StoreBean;
import com.storeserver.exception.StoreServerException;

public interface StoreInfoManager
{
	List<StoreBean> getStores();
}