package com.fishjam.study.java;

/**************************************************************************************
* RTTI(Run-time Type Information, 运行时类型信息) -- 需要.class
*   1.通过一个指向基类的引用找出其所指的确切类型－－编译时
*   2.Reflection(反射)机制，允许在 运行时 找出和class相关的信息(如函数名称)，主要是为支持RMI和Java Beans
*   
* Class对象内含与class相关的各种信息,在产生该class的对象时，JVM检查该类型的Class对象
*   是否已被加载，如果尚未加载，就会根据名称找到.class文件并加载它 -- Java程序启动时并
*   不会将整个程序都加载(可通过Class的 static Block 进行验证)。
*   
*   获得Class对象：Class.forName("类名")=类名.class=对象实例.getClass()，
*     然后可以通过 .newInstance() 创建新实例 -- 注意必须有缺省构造函数
*   强制类型转型：运用RTTI检查转型动作是否正确，如果发生错误，抛出ClassCastException
*   if(对象 instanceof 类名) <== 判断某个对象是否为某个特定类型的一个实例(instance)，
*     或者使用 Class对象.isInstance(实例对象) 进行判断   
**************************************************************************************/

public class FRttiStudy
{
     
}
