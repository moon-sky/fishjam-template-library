package com.java.test;

/***************************************************************************************************************************
 * 容器类(JDK1.4加入了模版功能，但实现技术和C++不同)
 *   BitSet -- 用于压缩存储大量Bool信息(节省空间，但速度稍慢)，缺省是64bits
 *   
 * 元素排序： 
 *   1.被比较类实现 Comparable 接口(注意不要简单写为 return r1-r2;--正负号溢出, 而应该是  if(>){}else if(<){}else{...} 分别比较的方式 ； 
 *   2.创建新类，实现 Comparator 接口（如 Collection.reverseOrder--系统提供的反向排序）
 *   3.自定义类的hashCode(快速找到位置)和equals(判断唯一性)必须同时实现--作为key或者放入Set中。
 *     若要放入TreeSet/TreeMap，元素必须实现 Comparable 接口 或 创建容器时指定一个定制的 Comparator 对象实现，
 *     Set不允许持有重复元素 equals实现时必须调用(obj instanceof 类名)先判断对象的类型 
 * 
 * HasMap的性能分析：Capacity(buckets数量)、loadfactor(负载因子)；size/capacity 越大说明负载越高，越容易出现冲突，当达到指定值时(默认0.75)可自动扩充
 * UnModifialbeXXX -- 获得只读版本的容器，如果有更改方法调用，则会抛出异常 
 * Collections.synchronzedXXX(new YYY) -- 生成可自动同步的容器
 * 
 * Enumeration(不再使用) -- 迭代器的老版本
 * Iterator() -- 迭代器的新版本
 * foreach -- 
***************************************************************************************************************************/
 
import static org.junit.Assert.*;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.*;

import org.junit.Test;

/***********************************************************************************************************************************************
 * Java集合体系 -- 1.5以前集合对象当成 Object 类型处理，1.5通过泛型保存了数据类型
 *   容器中的元素比较是否相等时总是通过 equals(而非 ==), 有的时候还会比较 hashCode().
 *   ★一般来说，需要重写元素的 equals 和 hashCode 方法，且保证两个方法的判断标准一致。否则可能导致通过 == 判断是否相等，而出现错误 
 * 
 *   <Iterator> <== 迭代器，提供了遍历集合元素的统一编程接口， while(it.hasNext()){ Object obj = it.next();  }
 *      迭代过程中，不可修改集合元素(如 Collection.add/Collection.remove 等)，否则会抛异常(ConcurrentModificationException)，多线程时若不加锁则容易发生 
 *      如果要修改，只能通过 Iterator.remove() 的方式删除上一次 next 返回的元素
 *   foreach 循环遍历 <== 
 *   
 *   <Collection> <== 容器，可通过 iterator() 方法返回 Iterator 对象，用于遍历集合里的元素; 通过 toArray() 方法把集合转换成一个数组。
 *      所有Collection集合实现类都重写了 toString() 方法，该方法可以一次性输出集合中的所有元素。
 *      <Queue> -- 1.5后增加，队列集合, 先进先出(FIFO)的容器。新元素从队列尾部插入(offer/add),从头部取出(peek/poll)
 *        <Deque> <== 双向队列
 *          [LinkedList] <== 同时实现 List + Deque 接口(详细介绍见后)
 *        [PriorityQueue] -- 按队列元素的大小自动排序，当 peek/poll 取出元素时，是取出队列的最小元素。
 *      <Set> <== 无序，不可重复的集合。如添加(add)相同元素，会失败(返回fasle)。判断两个对象相同是使用 equals
 *        [EnumSet] <== 专为枚举类设计的集合类，其值都必须是指定枚举类型的枚举值，通过静态方法构造实例。内部以位向量的形式存储，在比较时非常高效
 *        <SortedSet>
 *          [TreeSet] <== 集合元素处于排序状态(红黑树), 集合元素可实现 Comparable 接口，通过其 compareTo()方法来比较元素之间的大小关系，
 *                           并按升序排序。判断两个对象相等依据为: equals + compareTo 返回0。
 *                           如果构造TreeSet时传入定制的 Comparator实现，可实现定制排序，且集合元素可以不实现 Comparable 接口。
 *        [HashSet] <== 相等依据: equals + hashCode. 保存时: 按hash算法来存储集合中的元素，有很好的存取和查找性能。不保证元素的排列顺序。
 *                          注意：如果向HashSet中添加一个可变对象后，并且后面程序修改了该可变对象的属性，可能导致它与集合中其他元素相同，
 *                                   会导致HashSet中包含两个相同的对象 -- 
 *                                   因此进行 equals 和 hashCode 计算的属性值最好是不可变的(比如 Id 等)
 *          [LinkedHashSet] -- 根据hashCode值决定元素存储位置(方便查找)，但同时使用链表维护元素的次序，当遍历时会按元素的添加顺序来访问，此时速度快
 *      <List> -- 有序、可重复的集合。可以根据索引来操作集合元素，并额外提供了 listIterator() 返回可向前遍历、并可在当前位置插入新元素 的迭代器
 *        [ArrayList] <== 替代Vector。基于数组实现的List。添加大量元素时，可使用 ensureCapacity 提高性能。
 *        [LinkedList] <== 基于链表实现的 List + Deque,  功能很强大，可以模拟很多种类型的容器。且对顺序访问集合中的元素进行了优化，插入、删除元素时速度快
 *        [Arrays.ArrayList] <== 固定长度的List集合，通过Array.asList()将一个数组或指定个数的对象转换 ，只能遍历访问，不能增删 -- 会抛出不支持的异常
 *        [Vector] <== 旧的实现，推荐使用 ArrayList 代替
 *          [Stack]
 *
 *   <Map> -- 有映射关系(key-value 对)的集合。可返回一个由key形成的set或一个由value形成的Collection
 *   	[EnumMap] <== 其中的所有key都必须是单个枚举类的枚举值，内部以数组形式保存(紧凑高效)。根据枚举的定义顺序来维护 key-value 对的次序
 *   	[IdentityHashMap]
 *   	[HashMap] <== 替代Hashtable
 *   		[LinkedHashMap] <== 使用双向链表来维护 key-value 对的次序，其顺序与数据插入顺序保持一致
 *   	[Hashtable] <== 旧实现，不再使用
 *   		[Properties] <== 处理属性文件中的键值对，要求键和值都必须为String类型。可通过 load/store 读写属性文件的 Stream
 *   	<SortedMap>
 *   		[TreeMap] <== 红黑树，下层排序(Comparable 接口)，可通过SubMap返回tree中的部分、 如要遍历元素，建议用 entrySet取代 keySet，这样性能更高。
 *                                  问题代码： Iterator set = symbolMap.keySet().iterator(); while(set.hasNext()){...}
 *                                  正确代码： for(Entry<Icon,Integer> set:symbolMap.entrySet()) { set.getValue() }
 *   	[WeakHashMap] <== 其key是实际对象的 WeakReference(弱引用)，如其引用的对象没有被其它强引用变量所引用，则可以被垃圾回收。
 *   									 可节约内存。当垃圾回收了其对应的实际对象后，容器会自动删除该key对应的 key-value 对。
 *   									 使用其中的对象时，需要检测是否存在，如果不存在，需要加载，相当于一个 Cache 模型。
 *   									 //TODO: 有指向value的强引用对容器的逻辑没有影响 ? 
 *      [IdentityHashMap] <== 比较两个key相等时，当且仅当两个key严格相等(key1 == key2)时，才认为相等 -- 即同一个对象
 ***********************************************************************************************************************************************/

/***********************************************************************************************************************************************
 *   
 * Collections工具类
 *   1.提供了大量方法对集合元素进行排序、查询和修改等操作;
 *   2.可将对象设置为不可变;
 *   3.synchronizedXxx 包装容器集合即可成为线程安全的类 
 *     SortedSet<String> s = Collections.synchronizedSortedSet(new TreeSet<String>());
 *   4.返回不可变集合()只读对象：
 *     emptyXxx()--空的不可变集合对象; 
 *     singletonXxx --只包含指定对象(一个或一项元素); 
 *     unmodifiableXxx(指定集合对象的不可变视图)
 *
 ***********************************************************************************************************************************************/
public class ContainerTest {


	@Test
	public void testCollections(){
		List<String>		listStrings = new ArrayList<String>();
		for (int i = 1; i <= 5; i++) {
			listStrings.add("" + i);
		}
		assertEquals("[1, 2, 3, 4, 5]", listStrings.toString());
		Collections.reverse(listStrings);		//反转集合中的元素
		assertEquals("[5, 4, 3, 2, 1]", listStrings.toString());
		assertEquals("5", Collections.max(listStrings));
		
		Collections.shuffle(listStrings);			//随机排序(模拟洗牌)
		String strShuffle1 = listStrings.toString();
		Collections.shuffle(listStrings);
		String strShuffle2 = listStrings.toString();
		assertTrue( !strShuffle1.equals(strShuffle2)); 
		
		Collections.sort(listStrings);			//根据元素的自然顺序按升序排序
		assertEquals("[1, 2, 3, 4, 5]", listStrings.toString());
		
		//二分法查找(必须是按升序排序的)，
		int nSearchResult = Collections.binarySearch(listStrings, "4");	//若找到，则返回找到的位置索引
		assertEquals(3, nSearchResult);
		nSearchResult = Collections.binarySearch(listStrings, "8");
		assertEquals(-6, nSearchResult);			//找不到的时候，为 -(size + 1)
		
		Collections.sort(listStrings, Collections.reverseOrder());		//反向排序
		assertEquals("[5, 4, 3, 2, 1]", listStrings.toString());
		
		Collections.swap(listStrings, 1, 3); 		//将第 1 处元素 和第 3 处元素进行交换 -- 0 基址
		assertEquals("[5, 2, 3, 4, 1]", listStrings.toString());
		
		//生成只读对象
		List<String> listReadOnly = Collections.unmodifiableList(listStrings);
		assertEquals(listStrings.size(), listReadOnly.size());
		//listReadOnly.add("will throw exception");	//UnsupportedOperationException
		
		//生成线程安全的包装对象 
		SortedSet<String> syncSet = Collections.synchronizedSortedSet(new TreeSet<String>());
		syncSet.add(new String("thread1 value"));
	}
	
	@Test
	public void testIdentityHashMap(){
		IdentityHashMap<String, String>	identityHashMap = new IdentityHashMap<String, String>();
		identityHashMap.put("1", "one");
		identityHashMap.put("1", "ONE");		//由于采用编译时常量，因此两次的 key 完全相等(==), 无法再次加入
		assertEquals(1, identityHashMap.size());
		
		identityHashMap.clear();
		
		String strKey1 = new String("1");
		identityHashMap.put(strKey1, "one");
		identityHashMap.put(new String("1"), "ONE");		//由于采用new 出来的key，两次 key 的值相同(equals),但不完全相等(==)，可以加入
		String strCheck = identityHashMap.toString(); 
		assertTrue( strCheck.equals("{1=ONE, 1=one}") || strCheck.equals("{1=one, 1=ONE}") );		//顺序不确定(因为采用的是 hash )
		assertEquals(2, identityHashMap.size());
		assertEquals("one", identityHashMap.get(strKey1));			//使用之前保存的引用来查找, 能找到
		assertNull(identityHashMap.get(new String("1")));				//使用 new 出来的一个新key进行查找，虽然值一样，但找不到
		
	}
	
	@Test
	public void testList(){
		//测试 Arrays.ArrayList
		String[] books = new String[] {"Java", "C++", "C#" };
		List<String> readOnlyList = Arrays.asList(books);			//这种方式返回的是 Arrays.ArrayList , 是一个只读 List 
		assertEquals(3, readOnlyList.size());
		assertEquals("java.util.Arrays$ArrayList", readOnlyList.getClass().getName());
		//readOnlyList.add("JavaScript");		//以下代码会抛出异常  (UnsupportedOperationException)
	}
	
	@Test
	public void testLinkedList(){
		//LinkedList 同时实现了 List + Deque 接口，因此不仅可以 根据索引来随机访问(List), 而且可进行双向队列的操作(Deque)
		LinkedList<String> linkedList = new LinkedList<String>();

		linkedList.add("三");			// [三 ]
		linkedList.offer("四");			// [三, 四 ]	<== 加入尾部，当使用容量有限制的队列时，比add好(TODO: why?)
		
		linkedList.offerFirst("一"); 	// [一, 三, 四 ]
		linkedList.push("零"); 			// [零, 一, 三, 四 ]    -- 利用堆栈的方式插入数据(开头)
		linkedList.add(2, "二");			// [零, 一, 二, 三, 四 ] <== 指定位置插入
		
		linkedList.addLast("五");		// [零, 一, 二, 三, 四, 五 ]  插入双向队列的末尾
		
		
		assertEquals("零", linkedList.getFirst());			//获取但不删除第一个元素
		assertEquals("五", linkedList.peekLast()); 		//获取 但不删除 第一个元素,，如队列为空，返回 null
		assertEquals("五", linkedList.pollLast());			//获取 并删除    第一个元素,，如队列为空，返回 null
		assertEquals("[零, 一, 二, 三, 四]", linkedList.toString());
		
		linkedList.descendingIterator(); 		//返回 以逆向顺序进行遍历的迭代器
		
		//System.currentTimeMillis();
	}
	
	@Test
	public void testMap(){
		Map<Integer, String> map = new HashMap<Integer, String>();
		map.put(1,  "one");
		map.put(2, "two");
		map.put(3, "three");
		
		assertEquals(map.size(), 3);
		assertEquals("two", map.get(2));
		assertNull(map.get(4));
		
		//推荐使用的遍历方式
		for (Map.Entry<Integer, String> element: map.entrySet()) {
			assertTrue(map.containsKey(element.getKey()));
			assertEquals(element.getValue(), map.get(element.getKey()));
		}
	}
	
	@Test
	public void testPriorityQueue(){
		PriorityQueue<Integer>	priorityQueue = new PriorityQueue<Integer>(10);
		for (int i = 5; i > 0; i--) {
			priorityQueue.add(new Integer(i)); 		//按 5, 4, 3, 2, 1 的顺序插入数据
		}
		
		//注意：toString 返回的结果不是 [1, 2, 3, 4, 5] , 
		//原因：	Java的优先队列是一种堆Heap结构， toString 返回的是堆的存储顺序，而非排序的顺序
		List<Integer> listCheckOrder = new LinkedList<Integer>();
		Integer orderValue;
		assertEquals("[1, 2, 4, 5, 3]",  priorityQueue.toString());  

		//通过 poll 取出数据进行验证，确认是按排序的顺序取出的
		while( (orderValue = priorityQueue.poll()) != null){
			listCheckOrder.add(orderValue);
		}
		assertEquals("[1, 2, 3, 4, 5]",  listCheckOrder.toString());
	}
	@Test
	public void testProperties() throws IOException{
		Properties	props = new Properties();
		props.setProperty("username", "fishjam");
		props.setProperty("password", "12345678");
		assertEquals(2, props.size());
		
		try {
			//写入配置文件 -- ini 格式
			//TODO: 怎么读写 XML 等其他格式的配置属性?
			props.save(new FileOutputStream("testProperties.ini"), "Properties test");
			props.clear();
			assertEquals(0, props.size());
			
			props.load(new FileInputStream("testProperties.ini"));
		
			assertEquals("fishjam", props.getProperty("username"));
			assertEquals("12345678", props.getProperty("password"));
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
	}
	
	@Test
	public void testSet(){
		Set<String> strSet = new HashSet<String>();
		boolean bRet = strSet.add("testSet");
		assertTrue(bRet);
		assertEquals(1, strSet.size());
		
		bRet = strSet.add(new String("testSet"));
		assertFalse(bRet);				//两次的值 equals(对象不同)， 不能加入
		assertEquals(1, strSet.size());
	}
	
	@Test
	public void testWeakHashMap(){
		WeakHashMap<String, String> weakHashMap = new WeakHashMap<String, String>();
		weakHashMap.put(new String("largeImage1.png"), new String("image1的图片数据"));
		weakHashMap.put(new String("largeImage2.png"), new String("image2的图片数据"));
		weakHashMap.put(new String("largeImage3.png"), new String("image3的图片数据"));
		String strReloadImageData = weakHashMap.get("largeImage1.png");
		assertNotNull(strReloadImageData);
		
		assertEquals(3, weakHashMap.size());
		assertTrue( weakHashMap.containsKey("largeImage1.png") );
		strReloadImageData = null;
		
		//强制进行垃圾回收
		System.gc();
		System.runFinalization();
		
		assertEquals(0, weakHashMap.size());
		strReloadImageData = weakHashMap.get("largeImage1.png");
		assertNull(strReloadImageData);
		
		//再次使用时，需要先检查是否还存在，如果不存在，需要重新加载
		if (null == strReloadImageData) {
			strReloadImageData = new String("重新加载图片的数据");
			weakHashMap.put("largeImage1.png", strReloadImageData);
		}
		//使用 strReloadImageData 的数据
		strReloadImageData = null; 		//切断强引用，使得只在 WeakHashMap 中有弱引用指向，从而允许系统释放空间
	}
	

	
	/*
	@Test
	public void testCollectionsInThread(){
		Runnable runnable = new Runnable() {
			public void run() {
				
			}
		};
		runnable.run();
	}
	*/
	
}
