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
 * TODO:
 *   1.支持 @PostConstruct 和 @PreDestroy
 *   
 * Bean的生命周期 -- 可利用Spring提供的扩展点来自定义Bean的创建过程
 *   TODO:尚未进行测试确认
 *   1.new Bean;
 *   2.bean.setXxx
 *   3.BeanNameAware.setBeanName
 *   4.BeanFactoryAware.setBeanFactory
 *   5.ApplicationContextAware.setApplicationContext
 *   6.BeanPostProcessor.postProcessBeforeInitialization
 *   7.InitializingBean.afterPropertiesSet
 *   8.BeanPostProcessor.postProcessAfterInitialization
 *   ...
 *   99.DisposableBean.destroy
 *   
 * 所有的Spring Bean默认都是单例，当容器分配一个Bean时，总是返回Bean的同一个实例，可通过 scope 属性改变默认的单例配置
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
		logInfo("BeanLifeCycleDemo create," + this.toString());
	}
	
	public void destroy() throws Exception {
		logInfo("BeanLifeCycleDemo destroy," + this.toString());
	}
	public void afterPropertiesSet() throws Exception {
		logInfo("BeanLifeCycleDemo afterPropertiesSet");
	}
	public Object postProcessBeforeInitialization(Object bean, String beanName)
			throws BeansException {
		logInfo("BeanLifeCycleDemo postProcessBeforeInitialization: beanName=" + beanName);
		return bean;
	}
	public Object postProcessAfterInitialization(Object bean, String beanName)
			throws BeansException {
		logInfo("BeanLifeCycleDemo postProcessAfterInitialization: beanName=" + beanName);
		return bean;
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
