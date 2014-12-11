package com.java.test;

import static org.junit.Assert.*;

import java.lang.ref.PhantomReference;
import java.lang.ref.ReferenceQueue;
import java.lang.ref.SoftReference;
import java.lang.ref.WeakReference;

import org.junit.Test;

/****************************************************************************************
 * TODO
 *   1.并发问题
****************************************************************************************/

/****************************************************************************************
 * Java性能优化：
 *   1.尽量减少临时对象的使用
 *   2.对象不用时最好显式置为Null
 *   3.尽量使用StringBuffer,而不用String来累加字符串
 *   4.能用基本类型如Int,Long,就不用Integer,Long对象
 *   5.尽量少用静态对象变量--静态变量属于全局变量,不会被GC回收,它们会一直占用内存
 *   6.分散对象创建或删除的时间
 * 
 * 对象在内存中的状态(根据被引用变量所引用的状态来划分)
 *    1.激活状态 -- 对象被创建后，有一个以上的引用变量引用他
 *    2.去活状态 -- 程序中某个对象不再有任何引用变量引用他。该状态下,GC准备回收该对象所占用的内存，将调用其 finalize 方法
 *    3.死亡状态 -- 对象与所有引用变量的关联都被切断，且调用 finalize 方法后改对象依然没有变成激活状态，则该对象将永久性地失去引用。
 *                       之后系统才会真正回收该对象所占有的资源
 * 
 * 垃圾回收(Garbage Collector，GC)
 *   1.只能回收new出来的内存，且释放顺序是随机的(不是说越早成为垃圾越早释放)
 *   2.GC会在回收内存的同时，重排heap中的所有对象(包括正在使用的)，使其紧密排列，避免出现C/C++中的内存碎片问题.
 *   3.Java 中有 protected void finalize 方法，会在第一次垃圾回收时自动调用，进行资源清理(如 释放句柄、Socket等)，然后在第二次垃圾回收时回收内存。
 *     第一次GC -> finalize -> 下一次GC释放内存， TODO: finalize方法可能使该对象重新复活(让一个应用变量重新引用该对象)，从而取消回收
 *     ★如果override了finalize方法，千万记得调用 super.finalize ，否则base类的finalize将不会被调用。★
 *     由于是保护型的，不能被直接调用, 被调用时机具有不确定性(甚至不调用)。若调用该方法时出现异常，垃圾回收机制不会报告，而是继续执行。
 *     注意：★垃圾回收不等于析构★，最好手动执行必要的释放函数(finalize)来关闭相关的资源(文件句柄的)
 *     若要保证打开的资源被清理，需要 TODO: 
 *   4.两个条件会触发主GC：
 *     a.应用程序空闲时；
 *     b.堆内存不足时，
 *     若三次GC后内存仍然不足，则JVM将报“out of memory”的错误,Java应用将停止。
 *     通知系统进行垃圾回收： System.gc(); 或 Runtime.getRuntime().gc();
 *     强制垃圾回收机制调用系统中去活对象的finalize方法(会立即执行) -- System.runFinalization() 或 Runtime.getRuntime().runFinalization()  
 *   5.GC的算法不采用引用计数的方式(会出现有交叉引用的垃圾无法释放的情况)，而采用从stack或静态开始找所有能找到的对象，并将没有被找到的对象作为垃圾清除
 *
 * ref中的类在持有大量内存对象时格外有用--可以在保持引用的同时进行垃圾回收(类似Cache？但再次访问怎么办？) 
 * PhantomReference --
 * SoftReference -- 多用来实现cache机制 WeakReference -- 一般用来防止内存泄漏，要保证内存被VM回收?
 *   
 * Java 执行效能的改善(执行速度慢) 
 *   1.JDK1.3引入了 hotspot 技术，可以大幅改善执行效能 
 *   2.JIT(Just-In-Time)编译器 -- 在运行时按需编译，可以减少编译时间和运行时的程序大小，但初次运行时较慢?
 *   3.GC时，将回收回来的内存进行重排(Compact)，同时更改原来引用指向的地址；使得以后再次分配内存时，能简单的连续分配；
****************************************************************************************/

/****************************************************************************************
 * java.lang.ref -- 下的引用类(通过 get() 方法获取被引用的对象 )
 *   强引用(没有对应的类) -- 最常见的引用方式，创建对象并赋值给一个引用变量。当变量有引用变量时，处于激活状态，不会被回收
 *   软引用(SoftReference) -- 对于只有软引用的对象，当系统内存空间足够时，不会被系统回收。当内存空间不足时，将会回收。通用用于对内存敏感的程序中。
 *   弱引用(WeakReference) -- 对于只有弱引用的对象，当系统垃圾回收机制运行时，不管系统内存是否足够，总会回收其对应的内存。
 *   虚引用(PhantomReference) -- 完全类似于没有引用，主要用于跟踪对象被垃圾回收的状态。必须和引用队列(ReferenceQueue)联合使用。无法获取它引用的对象。
 * 
 * 引用队列(ReferenceQueue) -- 保存被回收后对象的引用，
****************************************************************************************/

/****************************************************************************************
 * 性能分析工具
 *   Memory profiler -- 在用例开始前和在用例结束后运行垃圾回收并且记录堆的快照，可看到用例的内存影响及该用例
 *     在内存中留下的具体对象列表。通常可检测内存泄露。
 *     基于Web的应用中，需要确认对象是基于每个请求创建还是创建后可被缓存重用，从而避免大用户时的问题。
 *   Code profiler
 *   Coverage profiler
 *   
****************************************************************************************/

public class PerformanceTest {

	@Test
	public void test() {
		//fail("Not yet implemented");
		assertEquals(1, 1);
	}

	
	@Test
	public void testReference(){
		//测试弱引用 
		String strWeakReference = new String("WeakReference");
		WeakReference<String> wReference = new WeakReference<String>(strWeakReference);
		strWeakReference = null;		//切断 string(强引用对象) 和对应字符串之间的关系，因此该字符串只有一个 弱引用 对象指向它
		assertNotNull(wReference.get());
		assertEquals(13, wReference.get().length());		//此时仍然可以使用该变量 

		//测试虚引用
		String strPhantomReference = new String("PhantomReference");
		ReferenceQueue<String> rQueue = new ReferenceQueue<String>();
		PhantomReference<String> phantomReference = new PhantomReference<String>(strPhantomReference, rQueue);
		assertTrue(rQueue.poll() == null);				
		assertNull(phantomReference.get());			//不能通过虚引用引用被引用的对象，因此为 null
		strPhantomReference = null;			//切断关联
		
		System.gc();		//强制垃圾回收
		System.runFinalization();
		
		assertNull(wReference.get());			//此时弱引用对应的对象会被释放
		assertTrue(rQueue.poll() == phantomReference); 		//对象被回收后，其对应引用会被添加到关联的引用队列中
		
	}
}
