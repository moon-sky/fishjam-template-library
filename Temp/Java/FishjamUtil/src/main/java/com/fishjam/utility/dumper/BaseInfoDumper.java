package com.fishjam.utility.dumper;

/****************************************************************************************
 * 日志：
 *   1.import org.apache.log4j.Logger;
 *   2.static Logger log = Logger.getLogger(BaseInfoDumper.class.getName());
 *   3.log.debug(e.getMessage());
 *   
 * 
 * Java集合框架
 * 
 * +-Array(C) -- 数组对应的Java类，“元素类型固定，数组长度固定时”性能最佳，
 * +-Arrays(C)/Collections(C) --  提供了大量的静态的算法(如 二分查找、排序 等)，
 *       集合转换(如 unmodifiableXxx, synchronizedXXX)
 * 
 * +-Iterable(I)
 *     +-Collection(I)
 *       +-List(I) -- 可包含null、重复数据的有序列表，能精确控制每个元素插入的位置，能使用索引( [index]? )定位
 *           +-ArrayList(C) -- 基于数组，非线程同步的(性能较好，但需要自己同步)
 *           +-LinkedList(C) -- 基于单向链表，
 *           +-Vector(C) -- 基于数组，线程安全的，
 *       +-Set(I) -- 不包含重复元素的无序Collection，在HashMap的基础上实现
 *           +-HashSet(C)
 *             +-LinkedHashSet(C)
 *           +-SortedSet(I) -- 有序的
 *             +-NavigableSet(I)
 *               +-TreeSet(S)
 *
 * +-Map(I)
 *   +-SortedMap(I)
 *     +-NavigableMap(I)
 *       +-TreeMap(C) -- 对键按序存放，有 firstKey 等方法，可指定一个范围以取得其子Map
 *   +-HashMap(C) -- 使用哈希码的算法，快速查找，元素可为空 
 *   +-Hashtable(C) -- 线程安全，元素不可为空
 ****************************************************************************************/

import java.lang.reflect.Array;
import java.util.Collection;
import java.util.Collections;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.List;
import java.util.Map;

public class BaseInfoDumper {
	
	public final static <T> String CollectionToString(Collection<T> cv) {
		if (cv != null) {
			return cv.toString();
		}
		return "";
		
//		StringBuilder sBuilder = new StringBuilder();
//		for (T t : cv) {
//			sBuilder.append(t.toString() + ",");
//		}
//		if (sBuilder.length() > 0) {
//			sBuilder.deleteCharAt(sBuilder.length() - 1);
//		}
//
//		return sBuilder.toString();
	}

	public final static <T> String EnumerationToString(Enumeration<T> ev) {
		if (ev != null) {
			return ev.toString();
		}
		return "";
	}

	public final static <T> String ListToString(List<T> list) {
		if (list != null) {
			return list.toString();
		}
		return "";
	}

	public final static <K, V> String MapToString(Map<K, V> map) {
		if (map != null) {
			//StringBuilder sBuilder = new StringBuilder();
			//return sBuilder.toString();
			return map.toString();
		}
		return "";
	}
}
