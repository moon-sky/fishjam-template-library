package com.fishjam.utility.web.spring;

import org.springframework.context.ApplicationContext;
import org.springframework.core.env.Environment;

import com.fishjam.utility.dumper.BaseInfoDumper;

public class EnvironmentDumper extends BaseInfoDumper{
	public static String EnvironmentToString(Environment environment, String strDivide){
		if (environment != null) {
			StringBuilder sBuilder = new StringBuilder();
			sBuilder.append("{" + strDivide);
			
			String[] defaultProfiles = environment.getDefaultProfiles();
			if (defaultProfiles != null) {
				sBuilder.append("defaultProfiles count is " + defaultProfiles.length + strDivide);
				for (int i = 0; i < defaultProfiles.length; i++) {
					String strProfile = defaultProfiles[i];
					sBuilder.append("\t[" + (i+1) + "]" + strProfile + "=" + environment.getProperty(strProfile) + strDivide);
				}
			}
			
			String[] activeProfiles = environment.getActiveProfiles();
			if (activeProfiles != null) {
				sBuilder.append("activeProfiles count is " + activeProfiles.length + strDivide);
				for (int i = 0; i < activeProfiles.length; i++) {
					String strActive= activeProfiles[i];
					sBuilder.append("\t[" + (i+1) + "]=" + strActive + strDivide);
				}
			}
			
			sBuilder.append("}" + strDivide) ;
			return sBuilder.toString();
		}
		return "null";
	}
}
