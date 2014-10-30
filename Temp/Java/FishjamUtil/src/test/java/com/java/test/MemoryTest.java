package com.java.test;

/***********************************************************************************************************************************************
 * http://www.iteye.com/topic/802573
 * http://www.iteye.com/topic/802638
 * http://wenku.baidu.com/view/11d2305c3b3567ec102d8a9c.html?re=view
 * 
 * 最佳实践(未确认):
 *   1.设置 -Xms 与 -Xmx 相同,以避免每次垃圾回收完成后JVM重新分配内存
 *   2.设置 -XX:+HeapDumpOnOutOfMemoryError ， JVM会在发生OutOfMemoryError错误时自动dump当时的内存转储文件
 *     设置 -XX:+HeapDumpOnCtrlBreak 可通过快捷键生成内存转储
 *     TODO:生成的文件名和位置?
 *   3.设置 -verbose:gc -xloggc:gc.log -XX:+PrintGCDetails -XX:+PrintGCTimeStamps 可以在进行GC时打印出详细的日志，方便分析内存泄露
 *
 * 配置注意:
 *   1.-noclassgc -- 告诉JVM不要跑到持久空间去执行垃圾收集释放其中已经装载的类文件(.class),
 *     但当持久空间满时会持续的调用GC而无法得到足够的内存空间(死循环).
***********************************************************************************************************************************************/

/***********************************************************************************************************************************************
 * GC分两种:
 *   Copy Collection(Minor GC) -- 复制算法,小幅度的调整收集，将可用内存划分为两块，每次只使用其中的一块，当半区内存用完了，仅将还存活的对象复制到另外一块上面
 *   Mark-Sweep Collection -- 标记-清除算法，大幅度的调整收集，停止所有在堆中运行的线程，并进行标记清除动作
 * Sun JVM 的内存管理方式 -- 在一个地方创建对象，在其长期占据空间之前给它多次死亡的机会，针对各个年代的特点采用最合适的收集算法
 *   新生代(Young generation) -- 包括 Eden(创建位置) + 2个Survivor Space，通常采用 复制算法
 *   老年代(Old generation) -- Tenured Space
 *   永久代(Permanent generation) -- 可通过 MaxPermSize 配置，JVM存放第三方jar和应用的类信息
 *   ? 判断内存泄漏的方式: 查看老一代(Tenured Space) 是否增加
 * IBM JVM的内存管理方式 -- 不是运行在一个巨大的继承HEAP中，它仅在一个单一的地区维护了所有的对象同时随着堆的增长来释放内存。
 *   开始运行时很小，随着对象实例不断的填充，在需要执行垃圾收集的地方清除掉无效的对象同时把所有有效的对象紧凑的放置到堆的底部
 *   判断内存泄漏的方式: 观察堆中所有的动作，确认堆的使用率是否在提高
***********************************************************************************************************************************************/


/***********************************************************************************************************************************************
 * Java程序 + 虚拟机(JVM) + 操作系统(OS) 三个层次
 *
 * Java内存(未确认是否是这样, 另一种分发是 方法区 + 堆 + 栈):
 *   heap(-Xmx 和 -XX:MaxPermSize=Xxxm) -- 
 *   native heap -- used by JVM and OS , (作用? 加载类，保存结构、类静态成员等?)
 *
 * Java内存泄露，造成 OutOfMemoryError,分两类
 *   PermGen space -- 
 *   Java heap space -- 
 *
 * jvm的内存控制 -- 整个堆大小=年轻代大小+年老代大小+持久代大小(一般固定为64M)
 *   -Xms -- 设置JVM初始内存, 默认为物理内存的 1/64,内存剩余超过70%时，减小到该值。
 *   -Xmx -- 设置JVM最大可用内存，默认为物理内存的 1/4, 内存不足40%时，增大到该值.
 *   -Xmn -- 设置年轻代大小, 增大年轻代后,将会减小年老代大小，此值对系统性能影响较大。推荐配置为整个堆的 3/8
 *   -Xss(Stack) -- 设置每个线程的堆栈大小。在相同物理内存下,减小这个值能生成更多的线程。
 *   示例: -Xms256m -Xmx256m
 *
 * Java使用有向图的方式进行管理，可以消除引用循环的问题
 *   根元素集合 -- 被虚拟机直接引 用的对象，从这些对象开始查找引用。如
 *     a.正在运行的线程对象
 *     b.系统调用栈里面的对象
 *     c.(★)被 System Class Loader 所加载的那些对象 -- 
 *
 * 分析工具: 
 *   JConsole -- 
 *   jmap -- jdk自带的转储(dump)java内存堆数据到文件中，然后可通过 MAT 进行分析,主要的文件格式有Sun的hrpof和IBM的PHD。
 *     jmap -dump:format=b,file=heap.bin <pid> -- 将进程号为<pid>的Java进程的object转储到 heap.bin 文件
 *     TODO: jmap -heap <pid> -- 有这个命令吗?(苹果下的，其他平台呢)
 *     
 *   jstack -l <pid> -- 列出<pid>指定进程的调用堆栈
 *   jProfile -- 进行java内存实时监控，观看哪些对象在增长，哪些对象的数据没有释放
 *   Eclipse Memory Analyzer Tool(MAT) -- http://www.eclipse.org/mat/downloads.php
 *     java堆转储文件分析工具，可以生成内存分析报告，包括内存泄露(Leak Suspects)，可独立运行也可集成到 Eclipse IDE 中。
 *     运行配置:
 *       修改启动参数或MemoryAnalyzer.ini,添加 -vmargs -Xmx4g ,允许使用较大的内存
 *     分析项:
 *       Leak Suspects -- 内存泄露分析报告
 *         Shortest Paths To the Accumulation Point -- 查看从GC根元素到内存消耗聚集点的最短路径，可以查看整个引用链
 *         Accumulated Objects in Dominator Tree -- 
 *       Top Consumers -- 
 *     高级功能:
 *       OQL(Object Query Language) -- 支持对 heap dump 中的对象进行查询
 *   VisualVM -- 
 *
 * 常见泄露的场景及分析:
 *   1.
 *
 * 常见的工具类
 *   Runtime -- 可获取JVM的内存等信息(totalMemory/freeMemory/maxMemory)
 ***********************************************************************************************************************************************/

public class MemoryTest {

}
