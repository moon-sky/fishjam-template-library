package com.fishjam.utility.web.spring;

import com.fishjam.utility.dumper.BaseInfoDumper;
import org.springframework.context.ApplicationContext;

public class ApplicationContextDumper extends BaseInfoDumper {

	public static String ApplicationContextToString(ApplicationContext applicationContext, String strDivide){
		if (applicationContext != null) {
			StringBuilder sBuilder = new StringBuilder();
			sBuilder.append("{" + strDivide) ;
			
			sBuilder.append("applicationName=" +  applicationContext.getApplicationName() + strDivide);
			sBuilder.append("displayName=" + applicationContext.getDisplayName() + strDivide);
			sBuilder.append("beanCount=" +  applicationContext.getBeanDefinitionCount() + strDivide);
			
			String[] beanNames = applicationContext.getBeanDefinitionNames();
			int idxOfBean = 1;
			for (String strBeanName : beanNames) {
				Object objBean = applicationContext.getBean(strBeanName);
				sBuilder.append("\t[" + idxOfBean + "]=" + objBean.getClass().getCanonicalName() + "; " + objBean.toString() + strDivide);
				idxOfBean++;
			}

			sBuilder.append("Environment=" + strDivide + 
					EnvironmentDumper.EnvironmentToString(applicationContext.getEnvironment(), strDivide));
			
			sBuilder.append("}" + strDivide) ;
			return sBuilder.toString();
		}
		return "null";
	}
}
