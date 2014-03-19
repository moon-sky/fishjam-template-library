package com.storeserver.service.impl;

import java.util.List;

import org.apache.log4j.Logger;
import org.springframework.mail.MailSender;
import org.springframework.mail.SimpleMailMessage;

import com.storeserver.action.base.BaseAction;
import com.storeserver.dao.StoreMapInfoDao;
import com.storeserver.dao.StoreDao;
import com.storeserver.domain.Store;
import com.storeserver.domain.StoreMapInfo;
import com.storeserver.service.StoreInfoManager;

public class StoreInfoManagerImpl implements StoreInfoManager
{
	static Logger log = Logger.getLogger(
			StoreInfoManagerImpl.class.getName());
	// 以下是该业务逻辑组件所依赖的DAO组件
	
	private StoreDao storeDao = new StoreDao();
	
	// 业务逻辑组件发送邮件所依赖的两个Bean
	private MailSender mailSender;
	private SimpleMailMessage message;
	
	// 为业务逻辑组件依赖注入DAO组件所需的setter方法
	public void setStoreDao(StoreDao storeDao)
	{
		this.storeDao = storeDao;
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
		storeDao.InitSqliteDB(baseAction.getServletContext());
		
		List<Store> stores = storeDao.getStoreList();
		return stores;
		/*
		List<StoreBean> resultBeans = new ArrayList<>();
		for (Iterator<Store> iterator = stores.iterator(); iterator.hasNext();) {
			StoreBean storeBean = new StoreBean();
			initStore(storeBean, iterator.next());
			resultBeans.add(storeBean);
		}
		return resultBeans;
		*/
	}

	@Override
	public List<StoreMapInfo> getStoreMaps(BaseAction baseAction, String strStoreMapDB) {
		StoreMapInfoDao mapInfoDao = new StoreMapInfoDao(strStoreMapDB);
		mapInfoDao.InitSqliteDB(baseAction.getServletContext());
		return mapInfoDao.getStoreMap();
	}

	@Override
	public int insertFloorInfo(BaseAction baseAction, String strStoreMapDB,
			StoreInfoManager.FloorInfo floorInfo) {

		StoreMapInfoDao mapInfoDao = new StoreMapInfoDao(strStoreMapDB);
		mapInfoDao.InitSqliteDB(baseAction.getServletContext());
		return mapInfoDao.insertFloorInfo(floorInfo);
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