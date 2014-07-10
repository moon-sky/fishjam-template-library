package com.fishjam.study.java;

import java.lang.annotation.*;
import java.lang.reflect.Method;
/********************************************************************************************************************
 * Annotation -- 类似修饰符，可以给包、类型、方法、变量、参数 等通过键值对的方式提供额外的元数据，
 *     然后可以通过反射API(java.lang.reflect.AnnotatedElement 等)对其信息进行访问(返回一个实现了该Annotation类型接口的对象，然后可以访问其成员值)
 *     注意：Annotation不影响程序代码的执行，Java语言解释器会忽略这些信息，需要特别的工具(apt)或API才能对其进行访问和处理
 *     
 * 
 * Annotation(修饰)功能由几部分组成：
 *   定义Annotation类型的语法
 *   描述Annotation声明的语法
 *   读取Annotation 的API
 *   使用Annotation修饰的class文件
 *   Annotation处理工具(apt)
 * 
 * 使用方法
 *    1.声明Annotation类型: (通常由系统或第三方库提供) -- public @interface MyAnnotation { 每个无参数方法声明定义了一个Annotation的类型成员 }
 *      在声明自定义Annotation时可以使用系统提供的Meta-Annotation进行修饰
 *      Annotation成员必须是非空的编译时常量表达式。可用的成员类型为：primitive类型、, String, Class, enumerated类型, annotation类型, 和前面类型的数组
 *    2.使用: 修饰方法或类(程序员使用) -- @MyAnnotation( 成员名=值 列表 ), 其值必须是编译时常量  
 *       当声明 MyAnnotation 时，如只有唯一的一个成员，则应命名为 value()； 使用时可忽略成员名和赋值号
 * 
 * Meta Annotation -- 声明annotation类型的annotation
 *    @Documented -- 描述其它类型的annotation应该被作为被标注的程序成员的公共API，可被Javadoc等工具文档化
 *    @Inherited -- 阐述了某个被标注的类型是被继承的
 *    @Retention -- 保留的时间长短
 *       SOURCE -- 源码中，被编译器丢弃
 *       [CLASS] -- 编译在class中，运行时会被虚拟机忽略(不能被反射访问)
 *       RUNTIME -- 运行时加载,可通过反射读取, 然后可通过 Method.isAnnotationPresent 等判断是否存在指定Annotation
 *   @Target -- 修饰的程序成员的类型, 如METHOD(修饰方法声明), TYPE(修饰类型)
 * 内建Annotation
 *    @Deprecated -- 修饰已经过时的类型或方法
 *    @Override  -- ?? 修饰此方法覆盖了父类的方法（而非重载）??
 *    @SuppressWarnings -- 通知Java编译器禁止特定的编译警告， 如 @SuppressWarnings({"deprecation","unchecked"}) 
 *   
 * Annotation运行存在几种方式(通过 @Retention 声明): ？
 *   RetentionPolicy.SOURCE -- 
 *   编译时(RetentionPolicy.CLASS, 缺省) -- 需要使用 apt
 *   运行时(RetentionPolicy.RUNTIME)
 * 
 * 
 * TODO:
 *   1.自定义annotation生成javabean映射xml文件
 *
 *********************************************************************************************************************/

//简单的 Annotation 类型声明示例
@Retention(RetentionPolicy.RUNTIME)
@Target({ElementType.METHOD, ElementType.TYPE})
@interface MyAnnotation{			//表明从 java.lang.annotation.Annotation 继承，而非 interface 
	//Annotation成员 以无参数的方法的形式被声明，其方法名和返回值定义了该成员的名字和类型
	int id();
	String author()  default "fujie";
	String date()    default "[unimplemented]";
}

@MyAnnotation(id = 1, author="fujie")
public class JavaAnnotationStudy {
	int a;
}
