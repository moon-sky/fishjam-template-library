package com.fishjam.springtester.domain;

import org.springframework.beans.BeansException;
import org.springframework.beans.factory.BeanFactory;
import org.springframework.beans.factory.BeanFactoryAware;
import org.springframework.beans.factory.BeanNameAware;
import org.springframework.beans.factory.DisposableBean;
import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.config.BeanPostProcessor;
import org.springframework.context.ApplicationContext;
import org.springframework.context.ApplicationContextAware;

/**********************************************************************************************************************************
 * Bean的生命周期 -- 可利用Spring提供的扩展点来自定义Bean的创建过程
 **********************************************************************************************************************************/

public class BeanLifeCycleDemo implements 
	BeanNameAware, 
	BeanFactoryAware, 
	ApplicationContextAware,
	BeanPostProcessor,
	InitializingBean,
	DisposableBean
	{
	public BeanLifeCycleDemo(){
		logInfo("create BeanLifeCycleDemo");
	}
	
	public void destroy() throws Exception {
		logInfo("BeanLifeCycleDemo destroy");
	}
	public void afterPropertiesSet() throws Exception {
		logInfo("BeanLifeCycleDemo afterPropertiesSet");
	}
	public Object postProcessBeforeInitialization(Object bean, String beanName)
			throws BeansException {
		logInfo("BeanLifeCycleDemo postProcessBeforeInitialization: beanName=" + beanName);
		return this;
	}
	public Object postProcessAfterInitialization(Object bean, String beanName)
			throws BeansException {
		logInfo("BeanLifeCycleDemo postProcessAfterInitialization: beanName=" + beanName);
		return this;
	}
	public void setApplicationContext(ApplicationContext applicationContext)
			throws BeansException {
		logInfo("BeanLifeCycleDemo setApplicationContext");
		
	}
	public void setBeanFactory(BeanFactory beanFactory) throws BeansException {
		logInfo("BeanLifeCycleDemo setBeanFactory");
		
	}
	public void setBeanName(String name) {
		logInfo("BeanLifeCycleDemo setBeanName, name=" + name);
	}

	private void logInfo(String strInfo){
		System.out.println(strInfo);
	}
}
