package com.java.test;

import static org.junit.Assert.*;

import java.io.UnsupportedEncodingException;
import java.math.BigDecimal;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.List;
import java.util.concurrent.Callable;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.junit.Test;
//import org.omg.CORBA.PRIVATE_MEMBER;
import org.omg.CORBA.PRIVATE_MEMBER;


/***************************************************************************************************************
 * 书籍:  
 *    疯狂Java -- P327, Math
*     ThinkingInJava -- P34
*     
 * TODO
 *   1.容器中迭代 : SomeType [] values = xxx; for(SomeType v : values) { v.Xxx(); }
 *   2.类的生命周期: 加载 -> 验证 -> 准备 -> 解析 -> 初始化
 *     对象的生命周期: 
 *   3.访问控制级别由小到大: private->default(相同包下的类)->protected(相同包下的类+其他包的子类)->public
 *   4.import 可以导入指定包下某个类或全部类，1.5以后可以通过静态导入的语法来导入 指定类的静态属性值或方法(import static)
 *   
 * 闭包(Closure) -- 一种能被调用的对象，它保存了创建它的作用域的信息。Java不能显式的支持闭包，但可以把非静态内部类当成面向对象领域的闭包
 *   ( 内部类记录了其外部类的详细信息，有对外部类对象的引用，能直接调用其 private 成员 ).
 *   TODO: 通过闭包可以实现: 
 * 
 ***************************************************************************************************************/
/***************************************************************************************************************
 * 关键字
 *    assert
 *    native <== 修饰方法，通常采用C语言实现，通常在需要利用平台相关特性或 访问系统硬件时使用。
 *    instanceof <== 判断实例是否是指定的类型或接口(进行类型转换), if(objStr instanceof String){ ... } 
 *    strictfp <== 精确浮点(FP-strict)， 用于类、接口或方法。指定对浮点数运算时按照浮点规范 IEEE-754 来执行
 *    synchronized <==
 *    throw
 *    throws 
 *    transient <== 修饰成员属性
 *    volatile   <== 修饰成员属性
 *    
 * 子类 extends 父类  implements 接口1,接口2
 * 内部类：成员内部类(类中定义)，局部内部类(方法里定义)，匿名内部类
 * 
 * Java的访问控制分为： public > protected(同一package中其他class也可以访问) >
 * package/friendly(同一个包内可以访问，其他包内同 private) > private
 * 
 * Java 是单根继承，所有类有共同的父类(Object)，有共同的接口 -- GC(垃圾回收机制)实现更容易；完全通过引用操作对象；都有RTTI，易于异常处理
 * 子类调用父类的方法：super.方法名(被覆盖的是实例方法) 或 父类类名.方法名(被覆盖的是类方法)；如果是构造函数中调用父类构造，则是 super(参数)
 * 初始化块 -- 其中可以包含任何可执行性语句，在构造函数前隐式执行。可以使用 static 修饰符对类 进行静态初始化。类定义中可以有多个(一般一个即可)，多个之间有顺序，先定义的先执行。
 * 
 * C++成员函数缺省是非virtual的(前期绑定)，而java的成员函数缺省是virtual的(后期绑定),子类覆盖实现时，最好加上 @Override，可以发现名字不匹配的问题。
 * abstract -- 定义抽象类(有部分实现，但不可实例化)，并指定其中的抽象成员函数 
 * interface -- 只有函数定义，不能有任何实现和成员变量，是接口和实现分离的完美境界
 * 
 * 对象相等的测试： == 和 != 判断对象引用是否相同，
 * equals 判断内容是否相同（Object缺省的实现是比较引用），可以override，标准库中很多类（如基本类型的Wrapper类、String等）都进行了override
 *    注意：override 后一般也需要同时重写 hashCode 方法，来保证 equals 为true 的两个对象其 hashCode 也相同
 *    public boolean equals(Object obj) --  
 *    实现时一般: 
 *      1.先判断比较对象是否是 this(总相等); 
 *      2. 判断是否为 null(总不等)，且是否是指定类型 -- 使用 instance 的话会认为子类实例也满足条件，因此推荐使用 obj.getClass() == Xxx.class  
 *      3.强制转换后判断具体的值
 *    
 * 位运算符 -- 操作基本整数类型中个别的位(bits)，&(与)、|(或)、^(异或)、~(取反)，
 * 如~0x1234(4660)=0xFFFFEDCB(-4661) 位移运算符 -- <<、>> 只能用于基本整数，左移(补0) 、右移(符号位扩展) >>>、右移(无论原值正负，一律在较高处补0)
 * Java 不允许程序员实现自定义的运算符重载 -- Java库唯一实现了String的+重载？
 * Java没有sizeof运算符 -- 在所有的机器上，每种数据类型有相同的容量大小，可以直接计算出来，易于移植
 * 
 * 容器 -- 迭代器 -- 选择容器中的元素，将容器实现细节和对容器进行访问动作之程序代码分离开来，可轻易更改底层结构(如容器类型)，而不干扰应用程序代码.
 * 模版(哪个版本后实现？) -- 参数化类型，降低向下转型的需求及可能导致的错误（模版之前使用Object保存对象，需要转型）
 * 
 * Java 在程序语言中内置了线程，线程功能在对象层次上提供(C/C++是通过函数提供的)，一个线程由一个对象表示，也提供有限
 * 的资源锁定功能，可锁定任何对象所有的内存(synchronized) -- 通常做法是产生一个对象代表欲锁定的资源，所有线程在访问
 * 这份资源之前都必须加以检测。
 * 
 * 持久性 -- Java提供轻量级持久性(light weight persistence)，可轻松地将对象存储于磁盘，并于日后取出
 * 
 * 类定义中，成员变量可在定义时初始化或构造中初始化，如没有，则由Java设置初始值(0或null)
 * Java源文件中最多只能有一个public类，该类的类名必须和文件名相同
 * 基本类型(primitive type)用自动变量(automatic)方式解决效率问题；基本类型有所谓的包装类(Wrapper classes)，可用new创建，并可通过字符串格式的构造参数进行初始化
 *    1.5以后JDK提供了自动装箱(Autoboxing)和自动拆箱(AutoUnboxing)功能，可直接赋值。包装类可通过 parseXxx(String) 静态方法将字符串转换成基本类型变量。
 *    对应的 String.valueOf(xxx) 可将基本类型变量转换成字符串
 * 高精度计算(如精确金融)使用 -- BigInteger 和 BigDecimal(任意精度的定点数)
 * 
 * Java的applet使用sandbox(砂盒)的方式，applet无法写、删除文件，如果数字签名了的话可以不受限。 
 *   可以使用SecurityManager--设定Java的安全策略 
 *   Java在服务器端，编写servlets及其衍生物JSP，可消除因浏览器能力不同造成的问题
 * 
 * 嵌套类可以直接访问嵌套它的类的成员，包括private成员，但是，嵌套类的成员不能被嵌套它的类直接访问(必须显示 使用具体的内部类对象来访问)。
 *   在内部类对象保存了一个对外部类对象的引用，当内部类的成员方法中访问某一变量时，如果在该方法和内部类中都没有定义过这个变量，
 *   内部类中对this的引用会被传递给那个外部类对象的引用 (即 this.变量 不存在时会传递 Outer类.this.变量)。
 *   如果内部类被定义为static，则不能访问外部类的变量(少用static内部类)
 *   因为非静态内部类的对象必须寄存在外部类的对象里，因此在外部类以外的地方创建的语法为： OuterInstance.new InnerConstructor();
 *      如: Out.In in = new Out().new In("内部类实例") -- 即通过外部类的实例来创建
 *   匿名内部类必须继承一个父类或实现一个接口，在创建匿名内部类时立即创建一个该类的实例，其定义立即消失，不能重复使用。
 *      匿名内部类不能定义构造函数，但可以通过实例初始化代码来进行初始化。
 *      如果匿名内部类需要访问外部类的局部变量，必须使用 final 来修饰外部类的局部变量 
 * 
 * Java不具备条件编译的能力 -- 可以用package来模拟(参考 C#?) -- 编译不同的实现，并放在不同的package中。 
 * 可以使用 this在构造函数中调用另一个构造函数或private的初始化函数（以免编写重复代码，注意：由于Java默认是虚拟函数，
 *   因此在构造函数 中不能调用protected/public等可以被override的函数），C++不允许(调用一个共同的初始化函数)。
 * 
 * Java的数组在定义时不能指定大小，在new或初始化时才能指定，如果数组成员是对象，初始化前都是null。Java的多维数组的维数可以不同，是真正的数组的数组。
 * 
 * Java初始化顺序： 
 *   装载.class文件->产生Class对象->查找父类->初始化static变量->new出变量->在heap上分配存储空间->
 *   将其中的值全部初始化为0 ->按定义顺序执行数据定义处的初始化(或初始化block)->构造函数(如有this调用，则调用对应的重载构造)
 * 
 * 每个Class都可以有一个public的main -- 可以使每个Class的单元测试更容易？
 * 
 * final -- 用于不能被改变处(类似C++中的const, 或C#中的 sealed )。可用于：
 *   data -- 编译器常量(static)；或执行期被构造函数初始化后不能再更改。如果作用于对象引用，表示指向的目标不变，但目标的内容可变；
 *   argument -- 参数中声明为常量，函数实体中不可更改 
 *   method -- 1.锁住函数，使子类无法override(即成为非虚函数)；
 *                   2.效率，可被编译器转化为内联函数。
 *                   Class中所有的private函数自动是final的 class -- 不能被继承，主要基于设计上的考虑（如不想变动）或为了安全、保密
 *   class -- 使得类不可被继承(如 java.lang.String )
 *   
 * 不可变类(immutable) -- 创建类实例后，其属性不可改变。如: 基本数据的包装类和 String(其 value/offset/count 等都是 private final 的)。规则：
 *    1.使用 private + final 来修饰该类的属性;
 *    2.提供带参数的构造函数，根据传入参数来初始化类里的属性;
 *    3.仅为该类的属性提供 getter 方法，不要提供 setter 方法;
 *    4.如有必要，重写 hashCode 和 equals 方法，在 equals 方法中根据关键属性来作为两个对象相等的标准。且需要保证用 equals 方法判断为相等的对象的 hashCode 也相等
 *    5.需要特别注意其引用类型的属性，如其是可变的，必须采用必要的措施(如 初始化时从入参构造副本；返回属性引用时创建临时的匿名副本对象 )来保护该属性所引用的对象不会被修改
 *    
 * 强制类型转换：((子类名)对象).方法
 * 
 * innerClass(内嵌类) -- 除有代码隐藏功能外，更可和外部类沟通（典型使用：Swing中的event机制） 
 *   private内嵌类 使设计者完全避免任何"与型别相依的程序代码"，并得以完全隐藏实现细节
 *   innerClass还可置于函数之内(实现某接口并返回匿名对象)或任意程序范畴内(语法为： return new 接口名() {方法定义和接口实现};)
 * 
 * 匿名类不能有构造函数(没有类名)，可用括号括起来进行实体初始化，通常会调用super。匿名内部类如想调用外部的对象(如参数、外部类变量)则该对象必须是final的。
 *   可访问外部对象的所有成员(即使是private的)，编译器会编译成 "外部类名.this.变量" 
 *   内部类在创建时，一定要关联至某个外部类对象（C++没有这个限制，因此内部类不能访问外部类的变量），
 *   外部创建时的语法为： 外部类名.内部类名 内部变量名 = 外部变量.new 内部类名。
 *   使用innerClass的理由：
 *     1.每个内嵌类都能够各自继承某一实现类或接口，不受限于外部类是否已继承自某一实现类。
 *     2.内部类机制才能有效而实际地允许"多实现继承" -- 在单一的外部类中，可以拥有多个内嵌类，每个都实现相同的interface或以不同的方式继承同一个class
 * 
 * Java泛型(Generic) -- 1.5引入的新特性，有 泛型类、泛型接口、泛型方法，可以做到编译时类型检查
 *   1.泛型的类型参数只能是类类型（包括自定义类），不能是简单类型，即默认 <T extends Object> 
 *      实例： public interface List<E> extends Collection<E> { ... void add(E x);  } -- 形参名为E，然后在定义内可作为类型来使用
 *   2.泛型的参数类型可以使用extends语句，例如<T extends superclass>。 习惯上称为"有界类型"
 *   3.为了解决类型被限制死了不能动态根据实例来确定的缺点，引入了"通配符泛型(问号 "?" 表示通配符，可匹配任何类型 )"，可同时指定必须实现的接口 (通过 "& 接口" 的语法指定)。 
 *     例如 MyStack<? extends Collection> typeStack = new ...
 *            public void drawAllShape(List<?  extends Shape> shapes) { ... }, 表示参数可接收所有 Shape 泛型List实例，如 List<Shape>, List<Circle>, List<Rectange> 等多种实例
 *            若 函数为 drawAllShape(List<Shape> shapes), 则只能接收 List<Shape> 类型的实参实例。
 *     3.1 如果只指定了<?>，而没有 extends，则默认是允许Object及其下的任何Java类了。也就是任意类。
 *     3.2 通配符泛型不单可以向下限制,还可以向上限制，如<? super Double>，表示类型只能接受Double及其上层父类类型
 *         示例如 : TreeSet(Comparator<? super E> comparator){ ... }
 *     3.3. TODO: ★ 向下限制(extends)通常用于定义函数的参数，读数据； 向上限制(super) 通常用于作为保存对象的容器(写数据)
 *   4.泛型方法--只需将泛型参数列表置于返回值前(类型形参声明)，如 public <T> void fun(T x)
 *   5.因为类的静态变量、静态方法等会在所有的实例间共享，因此不能在静态方法、静态初始化或静态变量的声明和初始化中使用类型形参。
 * 
 * 远程调用(RMI--Remote Method Invocation)，通过存活于远程机器上的对象，发送消息并执行。
 *   interface->本地Stub->网络 远端接口必须继承自rmi.Remote，而且远端接口的函数都必须声明可抛出RemoteException，
 *   RMISecurityManager rm = new RMISecurityManager(); //产生并安装支持RMI的安全管理器
 *   //产生一个或一个以上的远端对象实体 Naming.bind(name, obj); //向远端对象注册点注册远端对象
 *   注意：需要一个服务--rmiregistry
 * 
 * CORBA(Common Object Request Broker Architecture)--使用其他语言撰写执行于其它架构上的程序代码，
 *   是一种OMG定义的一套和语言无关的分布式"对象交换标准框架"，提供了Java对象和非Java对象的 远端程序调用(RPC)能力。
 * 
 * Java中检测密码(必须有数字、大、小写、特殊符号等，长度8-30)的正则表达式:
 *   "((?=.*\\d)(?=.*[a-z])(?=.*[A-Z]).{8,30})"
 * 
 * JNI调用库？： system.loadLibrary("MyLib);
 *****************************************************************************************************************/

public class JavaLanguageTest {

	@Test
	public void testBasicGrammar(){
		//long wroingValue = 999999999999;  //系统不会自动把大的值当成 long类型处理，会超出 int 的范围，从而引起编译错误
		long rightValue = 999999999999L;   //使用 "L" 后缀，强制使用 long 类型 
		assertEquals(rightValue, 999999999999L);
		
		int octalValue = 013; 	//八进制
		int hexValue = 0xB;	//十六进制
		assertTrue((octalValue == hexValue) && (octalValue == 11));
		
		char zhong = '中';		//Java 中的 char 类型是UNICODE的(16位)
		assertEquals(zhong, '中');
		
		BigDecimal	exactFloat = new BigDecimal("3.1415926"); 	//float 和 double在要求的精度很高时(通常不存在这种情况)可能不能精确保存其值，此时用 BigDecimal
		
		float f = 0.5f;   	//Java语言浮点型默认是 double 型，需要加后缀 "f|F" 指定为 float
		
		assertTrue(Double.POSITIVE_INFINITY == 10.0 / 0.0); 	//正无穷大， 但如果是 10/0 ， 则会抛出 " / by zero" 异常
		assertTrue(Float.NaN !=  (0.0f / 0.0f));						//非数, NaN不与任何数(即使是NaN)相等
		
		int intValue = 233;                          
		byte byteValue = (byte)intValue;     //二进制(0xE9) : 11101001 => 1(负数) 1101001(补码) =(减1再取反)> -(0010111) 即 -23
		assertEquals(-23, byteValue);			//强制类型转换，可能造成数据丢失
		
		//按位的 与或非, 移位 等运算
		assertEquals( (5 & 9) , 1);  // 101 & 1001 = 1
		assertEquals( (5  | 9 ) , 13);    // 101 |   1001 = 1101
		assertEquals( (5 ^ 9) , 12);   // 101 ^  1001 = 1100
		assertEquals( (5 << 2) , 20);
		
		assertEquals(-20, (-5 << 2));						//左移
		assertEquals(-2 , (-5 >> 2));						//带符号右移
		assertEquals(1073741822, -5 >>> 2);			//无符号右移
		
		byte bAdd = 5;
		bAdd+=5;
		//bAdd = bAdd + 5;    //编译错误  --  +时会转换为 int, 计算完以后无法赋值给 byte
		assertEquals(10, bAdd);
		
		String str1 = "Hello world";
		String str2 = "Hello ";
		str2 += "world";
		String str3 = "Hello " + "world";  
		assertTrue(str1 instanceof Comparable);		//String 类实现了 Comparable 接口
		assertTrue(str1.equals(str2));						//TODO: 会通过 Comparable 比较内容?
		assertTrue(str1.hashCode() == str2.hashCode());	 	
		assertTrue( str1 != str2);			//引用类型的变量，只有指向相同的实例时 == 才是 true
		
		assertTrue(str1 == str3);			//通过直接量直接赋值的String进行缓存，因此指向同一个变量，类似的有 包装类自动装箱的cache数组
		
		
		assertTrue(new Integer(2) != new Integer(2));		//基本类型的包装类直接比较的话比较的是“是否指向同一对象"
		assertTrue(new Integer(2) == 2); 
		
		assertTrue( true && (1 > 0)); 		//前后两个操作数都是 true 时才返回 true

		//注意： & 和 | 不会短路，需要进行全部的逻辑比较，这样可能造成代码错误， 如 if( obj != null & obj->xxx()); 可能访问到空指针
		int aLogical = 5;
		int bLogical = 10;
		boolean bResult = (aLogical < 3)  && (  bLogical++ < 15);			// && 会短路
		assertTrue((bResult == false) && (bLogical==10 ));
		bResult = (aLogical < 3)  & (  bLogical++ < 15);							// & 不会短路
		assertTrue((bResult == false) && (bLogical==11 ));
	
	
		//通过在 break|continue 后跟一个标识外层循环的标签，可以直接结束外层循环 -- 示例: 
		String strBreakResult = "";
		outer:  //定义标签，必须在 break 语句所在的循环之前定义
		for( int i = 1; i < 10; i++)
		{
			for( int j = 1; j < 10; j++)
			{
				if( i * j == 10){
					strBreakResult = "i="+i + ";j=" + j;
					break outer;  //continue outer;
				}
			}
		}
		assertTrue(strBreakResult.equals("i=2;j=5"));
	}
	
	/**
	 * 
	 */
	@Test
	public void testArray(){
		int total = 0;
		//int[] dynamicArray -- 定义了一个引用类型的数组变量，此时尚未指向任何有效内存，且定义时不能指定数组的长度
		//new int[5] -- 为数组元素分配内存空间，此时每个数组元素尚未赋值，其值是默认值(如 int 的0， 引用类型的 null)
		//数组的初始化：
		//1.静态初始化 -- 程序员显式指定每个数组元素的初始值，由系统决定需要的数组长度，如 staticArray
		//2.动态初始化 -- 初始化时程序员只指定数组长度，由系统为数组元素分配初始值, 如 dynamicArray

		int[]  staticArray = { 1, 2, 3, 4, 5 }; //也可以: = new int[]{ 1, 2, 3, 4, 5};
		assertEquals(staticArray.length, 5);

		int[] dynamicArray = new int[5];   //int dynamicArray[] = new int[5];	Java 同时支持两种语法定义数组，推荐第一种
		for(int i = 0; i < dynamicArray.length; i++){		//1~5
			dynamicArray[i] = i + 1;
			total +=dynamicArray[i]; 
		}
		assertEquals(total, 15);
		
		total = 0;
		for (int i : dynamicArray) {//通过 foreach 循环遍历 集合、数组 的元素
			//注意：此时的迭代变量(i)是 集合元素的副本，因此无法通过更改迭代变量来改变集合、数组中的值(引用类型)
			total += i;
		}
		assertEquals(total, 15);

		//dynamicArray[0] = 99;
		assertArrayEquals(dynamicArray, staticArray);  				//数组进行了重载? 比较时比较长度和每一个元素?
		assertTrue(Arrays.equals(dynamicArray, staticArray));	//两个数组的长度相等，并且每个元素依次相等
		assertTrue(dynamicArray != staticArray);						//两个数组的变量不是指向同一个数组对象
		
		//多维数组
		int multiArrayIndex = 1;
		int [][] multiArray = new int[3][4];
		for(int i = 0; i  < multiArray.length; i++){
			for (int j = 0; j < multiArray[i].length; j++) {
				multiArray[i][j]=  multiArrayIndex++;
			}
		}
		int multiArraySum = 0;
		for (int[] is : multiArray) {
			for (int i : is) {
				multiArraySum += i;
			}
		}
		assertEquals(78, multiArraySum);  // 1 + 2 + ... + 12
		
		//操作数组的 工具类 Arrays, 其中定义了一些可以直接操作数组数据的静态方法
		Arrays.sort(staticArray);
		int nBinaraySearchIndex = Arrays.binarySearch(staticArray, 4);		//二分法查找 -- 必须已经按升序排好序
		assertEquals(3, nBinaraySearchIndex);
		assertTrue(Arrays.binarySearch(staticArray, 10)  < 0);					//找不到的话，返回值为 负数(不一定是 -1)
		
		int[] copyResult = Arrays.copyOf(staticArray, 3);
		assertEquals(3, copyResult.length);
		
		String strArraysToString = Arrays.toString(copyResult);	//按顺序把多个数组元素连接在一起，多个数组元素使用英文逗号(,) 和 空格 隔开
		String strDirectToString = copyResult.toString();			//默认的 toString() 返回 "类名@hashCode" 格式的字符串，如果要重写的话，一般返回 "类名[属性1=值1, ...]" 格式的字符串
		assertTrue( strArraysToString != strDirectToString);	    //★两种方法的结果不一样★
		assertEquals("[1, 2, 3]", strArraysToString);
		//assertEquals("[I@182f0db", strDirectToString);				//实际上因为每次运行的地址可能不同，本assert可能不正确
	}
	
	//形参长度可变的方法，多个参数被当成数组传入 -- 操作上等价于数组
	//区别:
	//  1.调用时可变形参更方便，
	//    String... ss -- 逗号分开的列表 或 new String[]{初始化列表}
	//    String[]  ss -- new String[]{初始化列表}
	//  2.数组形参可位于任何位置，可有多个；可变形参只能位于最后，且最多一个
	private String myContacStrings(String... ss)  
	{
		StringBuilder sBuilder = new StringBuilder("[");
		boolean bAppened = false;
		boolean bMoreThanOne = (ss.length > 1);
		for (String string : ss) {
			if (bAppened ) {
				if (bMoreThanOne) {
					sBuilder.append(", ");
				}
			}
			else {
				bAppened = true;
			}
			sBuilder.append(string);
		}
		sBuilder.append("]");
		return sBuilder.toString();
	}
	
	@Test
	public void testArrayParam() {
		String testString = myContacStrings("one", "two", "three");
		//System.out.println(testString);
		
		assertEquals(testString, "[one, two, three]");
		//fail("Not yet implemented");
	}
	
	//测试字符串
	@Test
	public void testString() throws UnsupportedEncodingException{
		String strChinese = "中华人民共和国";
		char[] strCharArray = strChinese.toCharArray();
		assertEquals(7, strCharArray.length);
		
		byte[] rawBytes = strChinese.getBytes("UTF-8");
		String strUTF8 = new String(rawBytes, "UTF-8");	//使用UTF-8字符集将指定的byte数组解码成String对象
		assertEquals(strChinese, strUTF8);
		
		{
			//String.split
			String splitString = "one, two, three";
			String strs[] = splitString.split(",");
			assertEquals(strs.length, 3);
			
			assertEquals(strs[0].trim(), "one");
			assertEquals(strs[1].trim(), "two");
			assertEquals(strs[2].trim(), "three");
		}
		
		{
			assertTrue("abcdefg".endsWith("efg"));			  //是否以指定字符串结尾
			assertTrue("abcdefg".equalsIgnoreCase("AbcdeFG"));//忽略大小写的比较
		}
	}
	@Test
	public void testStringBufferAndBuilder(){
		//String        -- 是不可变类，每次尝试更改内容都会生成一个新的实例
		//StringBuffer  -- 线程安全, 代表一个字符序列可变的字符串，可通过 append/insert/reverse 等方法改变字符串对象的字符序列
		//StringBuilder -- 1.5后新增的类(非线程安全,但性能高)

		StringBuilder sBuilder = new StringBuilder();
		sBuilder.append("hello");
		sBuilder.append(" world");
		assertEquals("hello world", sBuilder.toString());
		
		assertEquals(11, sBuilder.length());
		assertTrue(sBuilder.length() <= sBuilder.capacity());
		
		sBuilder.setLength(5);
		assertEquals("hello", sBuilder.toString());
		
		sBuilder.replace(0, 1, "H");
		assertEquals("Hello", sBuilder.toString());
		
		sBuilder.reverse();
		assertEquals("olleH", sBuilder.toString());
	}
	
	@Test
	public void testPrimitive(){
		//通过包装类的 parseXxx(String) 可将特定字符串转换成基本类型变量
		assertEquals(Integer.parseInt("123"), 123);
		assertEquals(Integer.parseInt("0123"),  123);
		assertEquals(Integer.parseInt("0123", 10), 123);
		assertEquals(Integer.parseInt("0123", 8), 83);

		assertEquals("123",  String.valueOf(123));
		
		Integer intNew3 = new Integer(3);
		Integer intValueOf3_1 = Integer.valueOf(3);			
		Integer intValueOf3_2 = Integer.valueOf(3);
		assertTrue(intNew3 != intValueOf3_1);				//new 出来的肯定返回新的对象
		assertTrue(intValueOf3_1  == intValueOf3_2);	//Integer类会缓存  [-128, 127] 的数据，因此使用 valueOf 会返回相同的对象
		
		boolean bThrowException = false;
		try {
			int a = Integer.parseInt("A0123B", 10);	//解析字符串时，会抛出异常
		} catch (NumberFormatException e) {
			bThrowException  = true;
			//e.printStackTrace();
		}
		assertTrue(bThrowException);
	}
	

	@Test
	public void testSingleInstance(){
		/*
		 * 单实例：处于线程安全考虑，getInstance() 使用了一种被称为 "initialization on demand holder" 的技术来创建单例类的实例
		class Stage{
			private Stage() {}
		}
		private static class StageSingletonHolder{
			static Stage instance = new Stage();
		}
		public static Stage getInstance() {
			return StageSingletonHolder.instance;
		}	
		*/
	}
	
	class Base
	{
		public int     checkLength;
		public String checkValue;
		public int setCheckValue() { 
			checkValue = "Base Call";
			return 0;
		}
		public Base(){
			checkLength = setCheckValue();
		}
	}
	class Sub extends Base{
		public String childValue;
		
		Sub(String value){
			childValue = value;
		}
		@Override
		public  int setCheckValue() {
			checkValue = "Sub Call";
			if(childValue == null){
				//父类的构造函数中调用该重写函数时，因为子类尚未完全初始化，因此会进到这里 -- ★逻辑错误甚至空指针异常★
				return -1;
			}
			else{
				return childValue.length();
			}
			
		}
	}
	
	
	//测试Java中在构造函数中调用子类会重载的函数 -- 语法上可以调用，但很危险(子类尚未完全初始化好)，比如：不要再构造中调用 toString() 等 ?
	@Test
	public void testCallChildMethodInConstructor(){
		Base base = new Sub("child");
		assertEquals("Sub Call", base.checkValue);   //Java可以在构造函数中调用子类会重载的方法，但这样可能会造成错误：子类尚未实例化好 -- 注意和 C++(不能调用) 的区别
		assertEquals(-1, base.checkLength);				//子类的构造函数尚未实例化好，因此 childValue 为 null, 子类的方法无法正确执行
		
		Sub sub = (Sub)base;
		assertNotNull(sub.childValue);						//子类完全构造好
		assertEquals("child", sub.childValue);
		assertEquals(5, sub.setCheckValue());			
	}
	
	/***
	 * 枚举类 
	 *    1.5以后增加 enum 关键字进行定义，继承自 java.lang.Enum 类，可通过 values() 方法遍历所有的枚举值
	 *    
	 */
	
	enum SeasonEnum { 
		//定义枚举类的枚举值(实际就是该类所有可能的实例)，系统自动添加 public static final 修饰
		//等价于 : public static final SeasonEnum SPRING = new SeasonEnum("春天");
		SPRING("春天"), 
		SUMMER("夏天"), 
		FALL("秋天"), 
		WINTER("冬天"); 		//最后一个以分号结尾
		
		//一些扩展的用法 -- 定义属性、构造等，提供更详细的信息, 在方法体内可通过 	switch (this) { case SPRING: xxx; } 的方式进行判断处理
		private SeasonEnum(String desc){
			this.desc = desc;
		}
		private String desc;
		//public String getDesc(){
		//	return desc;
		//}
	}
	@Test
	public void testEnumClass(){
		SeasonEnum spring = SeasonEnum.SPRING;
		assertEquals(0, spring.ordinal()); 		//在枚举类中的索引值
		assertEquals("SPRING", spring.toString());		//toString 返回更用户友好的名称
		assertEquals("SPRING", spring.name());
		assertEquals("春天", spring.desc);					//TODO: 为啥能访问到 private 的变量? 枚举中会自动更正为 public 的?
		SeasonEnum winter = SeasonEnum.valueOf("WINTER");  //注意：大小写必须一样，否则会抛出异常(如 "winter" 错误)
									// 等价写法： Enum.valueOf(SeasonEnum.class, "WINTER");
		assertTrue(winter == SeasonEnum.WINTER);
		
		//for (SeasonEnum season : SeasonEnum.values()) {
		//	System.out.println(season);
		//}
	}
	
	//正则表达式 -- String类增加了正则表达式支持
	@Test
	public void testRegex(){
		Pattern	pattern = null;
		Matcher matcher = null;
		
	}
	
	//国际化支持 -- 使用Locale对象封装一个国家、语言环境，使用 ResourceBundle 根据 Locale 加载语言资源包
	@Test
	public void testLocale(){
		
	}
	
}
