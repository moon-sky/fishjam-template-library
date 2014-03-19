package com.storeserver.service;

import java.io.InputStream;
import java.sql.Struct;
import java.util.List;

import com.storeserver.action.base.BaseAction;
import com.storeserver.domain.Store;
import com.storeserver.domain.StoreMapInfo;

public interface StoreInfoManager
{
	class FloorInfo
	{
		public int storeId;
		public int floor;
		public InputStream mapDataStream;
		public InputStream pathDataStream;
	}
	
	List<Store> getStores(BaseAction baseAction);

	List<StoreMapInfo> getStoreMaps(BaseAction baseAction, String strStoreMapDB);
	
	int insertFloorInfo(BaseAction baseAction, String strStoreMapDB,  StoreInfoManager.FloorInfo floorInfo);
}