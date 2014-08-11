package com.fishjam.springtester.domain;

import java.util.ArrayList;
import java.util.List;

import org.springframework.beans.BeansException;
import org.springframework.beans.factory.BeanFactory;
import org.springframework.beans.factory.BeanFactoryAware;
import org.springframework.beans.factory.BeanNameAware;
import org.springframework.beans.factory.DisposableBean;
import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.config.BeanPostProcessor;
import org.springframework.context.ApplicationContext;
import org.springframework.context.ApplicationContextAware;

import com.fishjam.springtester.tools.MyLog;

/**********************************************************************************************************************************
 * TODO:
 *   1.支持 @PostConstruct 和 @PreDestroy
 *   
 * Bean的生命周期 -- 可利用Spring提供的扩展点( 实现指定接口或通过 init-method/destroy-method 配置 )来自定义Bean的创建过程
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
 *   singletone[默认] -- 一个Spring容器中，一个Bean定义只有一个对象实例
 *   prototype -- 每次调用时生成实例
 *   request -- 一次HTTP请求中，每个Bean定义对应一个实例，该作用域仅在基于Web的Spring上下文中才有效
 *   session -- 在一个HTTP Session中，每个Bean定义对应一个实例，该作用域仅在基于Web的Spring上下文中才有效
 *   global-session -- 在一个全局HTTP Session中，每个Bean定义对应一个实例，该作用域仅在Portlet上下文中才有效
 *   
 * 配置
 *   <bean init-method="xxx"> -- 在初始化Bean时要调用的方法
 *   <bean destroy-method=xxx"> -- Bean从容器移除之前要调用的方法
 *   <beans default-init-method="xxx"> -- 配置所有本处定义的bean的初始化方法
 **********************************************************************************************************************************/

//实现指定的生存期接口 -- 缺点：和Spring的API产生了耦合
public class BeanLifeCycleDemo implements 
	BeanNameAware, 
	BeanFactoryAware, 
	ApplicationContextAware,
	BeanPostProcessor,
	InitializingBean,
	DisposableBean
	{

	private List<String> listLogInfo = new ArrayList<String>();
	private static final String[] standardLifeCycleCalls = {
		"BeanLifeCycleDemo create",
		"BeanNameAware.setBeanName",
		"BeanFactoryAware.setBeanFactory",
		"ApplicationContextAware.setApplicationContext",
		"InitializingBean.afterPropertiesSet",
		"init-method",
		
		
	};
	
	public boolean checkLifeCycleCalls(){
		boolean bRet = listLogInfo.size() == standardLifeCycleCalls.length;
		if (bRet) {
			for (int i = 0; i < standardLifeCycleCalls.length; i++) {
				if (listLogInfo.get(i) != standardLifeCycleCalls[i]) {
					System.err.println("BeanLifeCycleDemo diff [" + i + "]" + ", log=" + listLogInfo.get(i) + ", standard=" + standardLifeCycleCalls[i]);
					bRet = false;
					break;
				}
			}
		}
		
		return bRet;
	}
	
	
	public BeanLifeCycleDemo(){
		logInfo("BeanLifeCycleDemo create, this=" + this.toString());
	}

	public void setBeanName(String name) {
		logInfo("BeanNameAware.setBeanName, name=" + name);
	}
	public void setBeanFactory(BeanFactory beanFactory) throws BeansException {
		logInfo("BeanFactoryAware.setBeanFactory");
	}
	public void setApplicationContext(ApplicationContext applicationContext)
			throws BeansException {
		logInfo("ApplicationContextAware.setApplicationContext");
		
	}
	public Object postProcessBeforeInitialization(Object bean, String beanName)
			throws BeansException {
		logInfo("BeanPostProcessor.postProcessBeforeInitialization: beanName=" + beanName);
		return bean;
	}
	public Object postProcessAfterInitialization(Object bean, String beanName)
			throws BeansException {
		logInfo("BeanPostProcessor.postProcessAfterInitialization: beanName=" + beanName);
		return bean;
	}
	public void afterPropertiesSet() throws Exception {
		//初始化方法
		logInfo("InitializingBean.afterPropertiesSet");
	}
	public void destroy() throws Exception {
		//从应用上下文移除时被调用
		logInfo("DisposableBean.destroy");
	}

	private void logInfo(String strInfo){
		System.out.println(strInfo);
		listLogInfo.add(strInfo);
	}
	
	public void callForInitMethod(){
		logInfo("init-method");
	}
	
	public void callForDestroyMethod(){
		logInfo("destroy-method");
	}
}
