package com.java.test;

/***************************************************************************************************************************
 * TODO:
 *   1.如果向HashSet中添加一个可变对象后，并且后面程序修改了该可变对象的属性，可能导致它与集合中其他元素相同，
 *     可能导致HashSet中包含两个相同的对象 -- 因此进行 equals 和 hashCode 计算的属性值最好是不可变的(比如 Id 等)
 *   2.LinkedHashSet -- 根据hashCode值决定元素存储位置(方便查找)，但同时使用链表维护元素的次序，当遍历时会按元素的添加顺序来访问，此时速度快
 *   3.TreeSet -- 集合元素处于排序状态(红黑树), 集合元素必须实现 Comparable 接口，通过其 compareTo()方法来比较元素之间的大小关系，
 *     并按升序排序。判断两个对象相等依据为: equals + compareTo 返回0。
 *     如果构造TreeSet时传入定制的Comparator实现，可实现定制排序，且集合元素可以不实现 Comparable 接口。
 *   4.EnumSet -- 专为枚举类设计的集合类，其值都必须是指定枚举类型的枚举值，通过静态方法构造实例。内部以位向量的形式存储，在比较时非常高效
 *   5.List -- 可以根据索引来操作集合元素，并额外提供了 listIterator() 返回可向前遍历、并可在当前位置插入新元素 的迭代器
 *   6.ArrayList(推荐)/Vector(旧的) -- 基于数组实现的List。添加大量元素时，可使用 ensureCapacity 提高性能。
 *   7.LinkedList -- 基于链表实现的List+Deque，且对顺序访问集合中的元素进行了优化，插入、删除元素时速度快
 *   8.Arrays.ArrayList -- 固定长度的List集合，通过Array.asList()将一个数组或指定个数的对象转换 ，只能遍历访问，不能增删 -- 会抛出不支持的异常
 *   n.Collections 工具类 -- synchronizedSortedSet 包装Set集合即可成为线程安全的类?
 *     SortedSet s = Collections.synchronizedSortedSet(new TreeSet(...));
 *   
 *   
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
