package com.storeserver.service.impl;

import org.apache.log4j.Logger;

import java.util.*;

import org.springframework.mail.MailSender;
import org.springframework.mail.SimpleMailMessage;

import com.storeserver.business.StoreBean;
import com.storeserver.dao.StoreDao;
import com.storeserver.domain.Store;
import com.storeserver.exception.StoreServerException;
import com.storeserver.service.StoreInfoManager;

public class StoreInfoManagerImpl implements StoreInfoManager
{
	static Logger log = Logger.getLogger(
			StoreInfoManagerImpl.class.getName());
	// 以下是该业务逻辑组件所依赖的DAO组件
	
	private StoreDao storeDao;

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
	public List<StoreBean> getStores() {
		List<Store> items = storeDao.getStoreList();
		List<StoreBean> resultBeans = new ArrayList<>();
		for (Iterator<Store> iterator = items.iterator(); iterator.hasNext();) {
			StoreBean storeBean = new StoreBean();
			initStore(storeBean, iterator.next());
			resultBeans.add(storeBean);
		}
		return resultBeans;
	}
	
	private void initStore(StoreBean storeBean, Store store) {
		storeBean.setId(store.getId());
		storeBean.setName(store.getName());
		storeBean.setPosition(store.getPosition());
		storeBean.setDbName(store.getName());
	}

}