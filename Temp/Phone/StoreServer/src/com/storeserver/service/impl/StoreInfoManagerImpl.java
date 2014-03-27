package com.storeserver.service.impl;

import java.util.HashMap;
import java.util.List;

import javax.servlet.ServletContext;

import org.apache.log4j.Logger;
import org.openid4java.message.ax.StoreResponse;
import org.springframework.mail.MailSender;
import org.springframework.mail.SimpleMailMessage;

import com.storeserver.action.base.BaseAction;
import com.storeserver.dao.StoreMapInfoDao;
import com.storeserver.dao.StoreDao;
import com.storeserver.domain.PositionInfo;
import com.storeserver.domain.Store;
import com.storeserver.domain.StoreMapInfo;
import com.storeserver.service.StoreInfoManager;

public class StoreInfoManagerImpl implements StoreInfoManager
{
	static Logger log = Logger.getLogger(
			StoreInfoManagerImpl.class.getName());
	// 以下是该业务逻辑组件所依赖的DAO组件
	
	private StoreDao mStoreDao = new StoreDao();
	private HashMap<String, StoreMapInfoDao>mDb2StoreMapInfoDaoMap = new HashMap<>();
	
	// 业务逻辑组件发送邮件所依赖的两个Bean
	private MailSender mailSender;
	private SimpleMailMessage message;
	
	// 为业务逻辑组件依赖注入DAO组件所需的setter方法
	public void setStoreDao(StoreDao storeDao)
	{
		this.mStoreDao = storeDao;
	}
	
	// 为业务逻辑组件注入两个邮件发送Bean的setter方法
	public void setMailSender(MailSender mailSender)
	{
		this.mailSender = mailSender;
	}
	public void setMessage(SimpleMailMessage message)
	{
		this.message = message;
	}

	@Override
	public List<Store> getStores(BaseAction baseAction) {
		mStoreDao.InitSqliteDB(baseAction.getServletContext());
		
		List<Store> stores = mStoreDao.getStoreList();
		return stores;
	}

	@Override
	public List<StoreMapInfo> getStoreMaps(BaseAction baseAction, String strStoreMapDB) {
		StoreMapInfoDao mapInfoDao = getStoreMapInfoDaoByDBName(baseAction.getServletContext(), strStoreMapDB);
		return mapInfoDao.getStoreMap();
	}

	@Override
	public StoreMapInfo getStoreMapById(BaseAction baseAction, String strStoreMapDB, int mapId){
		StoreMapInfoDao mapInfoDao = getStoreMapInfoDaoByDBName(baseAction.getServletContext(), strStoreMapDB);
		return mapInfoDao.getStoreMapInfo(mapId);
	}
	@Override
	public PositionInfo getPositionInfo(BaseAction baseAction, String strPosition) {
		mStoreDao.InitSqliteDB(baseAction.getServletContext());
		
		if (strPosition != null) {
			String[] posInfos = strPosition.split("-");
			if (posInfos.length == 2) {
				int storeId = Integer.parseInt(posInfos[0]);
				Store store = mStoreDao.getStoreByStoreId(storeId);
				if (store != null) {
					StoreMapInfoDao mapInfoDao = getStoreMapInfoDaoByDBName(baseAction.getServletContext(), store.getDbName());
					if (mapInfoDao != null) {
						return mapInfoDao.getPositionInfo(posInfos[1]);
					}
				}
			}
		}
		
		return null;
	}
	
	@Override
	public int insertFloorInfo(BaseAction baseAction, String strStoreMapDB,
			StoreInfoManager.FloorInfo floorInfo) {
		StoreMapInfoDao mapInfoDao = getStoreMapInfoDaoByDBName(baseAction.getServletContext(), strStoreMapDB);
		return mapInfoDao.insertFloorInfo(floorInfo);
	}
	
	private StoreMapInfoDao getStoreMapInfoDaoByDBName(ServletContext servletContext, String strStoreMapDB){
		StoreMapInfoDao mapInfoDao = mDb2StoreMapInfoDaoMap.get( strStoreMapDB );
		if (mapInfoDao == null) {
			mapInfoDao = new StoreMapInfoDao(strStoreMapDB);
			mapInfoDao.InitSqliteDB(servletContext);
			mDb2StoreMapInfoDaoMap.put(strStoreMapDB, mapInfoDao);
		}
		return mapInfoDao;
	}
	/*
	private void initStore(StoreBean storeBean, Store store) {
		storeBean.setId(store.getId());
		storeBean.setName(store.getName());
		storeBean.setPosition(store.getPosition());
		storeBean.setDbName(store.getDbName());
	}
	*/
}