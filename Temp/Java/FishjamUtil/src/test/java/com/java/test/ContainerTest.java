package com.java.test;

/***************************************************************************************************************************
 * java.util.Collection; -- list, set ?
 * java.util.Map;
 * java.util.Properties;  -- Map的一种特例？要求键和值都必须为String类型
 * 
 * 容器类(JDK1.4加入了模版功能，但实现技术和C++不同)
 *   ArrayList(替代Vector)
 *   BitSet -- 用于压缩存储大量Bool信息(节省空间，但速度稍慢)，缺省是64bits
 *   Collection 
 *   HashMap(替代HashTable)、
 *   HashSet(查找快)
 *   Map(key-value对) -- 可返回一个由key形成的set或一个由value形成的Collection
 *   TreeMap(红黑树，下层排序，可通过SubMap返回tree中的部分)、 如要遍历元素，建议用 entrySet取代 keySet，这样性能更高。
 *     问题代码： Iterator set = symbolMap.keySet().iterator(); while(set.hasNext()){...}
 *     正确代码： for(Entry<Icon,Integer> set:symbolMap.entrySet()) { set.getValue() }
 *   TreeSet(有序Set)
 *   WeakHashMap(持有WeakReference，节省空间)
 *   
 * 元素排序： 
 *   1.被比较类实现 Comparable 接口(注意不能简单写为 return r1-r2;--正负号溢出)； 
 *   2.创建新类，实现 Comparator 接口（如 Collection.reverseOrder--系统提供的反向排序）
 *   3.自定义类的hashCode(快速找到位置)和equals(判断唯一性)必须同时实现--作为key或者放入Set中。
 *     若要放入TreeSet/TreeMap，必须实现Comparable接口。Set不允许持有重复元素 equals实现时必须调用(obj instanceof 类名)先判断对象的类型 
 * 
 * HasMap的性能分析：Capacity(buckets数量)、loadfactor(负载因子)；size/capacity 越大说明负载越高，越容易出现冲突，当达到指定值时(默认0.75)可自动扩充
 * UnModifialbeXXX -- 获得只读版本的容器，如果有更改方法调用，则会抛出异常 
 * Collections.synchronzedXXX(new YYY) -- 生成可自动同步的容器
***************************************************************************************************************************/
 
import static org.junit.Assert.*;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;
import org.junit.Test;

public class ContainerTest {

	@Test
	public void testHashMap(){
		Map<Integer, String> map = new HashMap<Integer, String>();
		map.put(1,  "one");
		map.put(2, "two");
		map.put(3, "three");
		
		assertEquals(map.size(), 3);
	}

	@Test
	public void testProperties(){
		Properties	properties = new Properties();
		properties.put("one", "一");
		
	}
}
