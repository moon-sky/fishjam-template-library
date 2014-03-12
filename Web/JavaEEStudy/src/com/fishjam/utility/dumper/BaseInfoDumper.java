package com.fishjam.utility.dumper;

import java.util.Collection;
import java.util.Enumeration;
import java.util.List;
import java.util.Map;

public class BaseInfoDumper {

	//public final static <T> String ArrayToString(T [] arr){  //String[] arr
	//  for(String item : arr) { item.toString(); }
	//	return "";
	//}
	
	public final static <T> String CollectionToString(Collection<T> cv) {
		return "";
	}
	
	public final static <T> String EnumerationToString(Enumeration<T> ev) {
		
		return "";
	}

	public final static <T> String ListToString(List<T> list){
		return "";
	}
	
	public final static <K, V> String MapToString(Map<K, V> map){
		if (map != null) {
			StringBuilder sBuilder = new StringBuilder();
			
			return sBuilder.toString();
		}
		return "";
	}
}
