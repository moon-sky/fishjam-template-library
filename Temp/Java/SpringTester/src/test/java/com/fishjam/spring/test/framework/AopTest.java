package com.fishjam.spring.test.framework;

import junit.framework.TestCase;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

/**************************************************************************************************************************************
 * TODO： 其本质就是Hook？
 * 
 * AOP(Aspect-Oriented Programming) -- 面向切面编程，通过在编译期间、装载期间或运行期间实现在不修改源代码的情况下给程序动态添加功能的一种技术。
 *   即把可重用的功能(横切关注点crosss-cutting concerns -- 如 安全、日志记录、事务管理 等)提取出来，然后将这些通用功能在合适的时候织入到应用程序中。
 *   AOP允许把遍布应用各处的功能分离出来形成可重用的组件，然后包裹核心业务层并提供服务，甚至核心应用根本不知道其存在。
 *   注意：继承与委托是最常见的实现重用通用功能的面向对象技术。但是，继承会导致一个脆弱的对象体系(无法更改框架？)，委托可能需要对委托对象进行复杂的调用。
 *            切面提供了取代继承和委托的另一种选择 -- 通过声明的方式定义通用功能以何种方式在何处应用，而无需修改受影响的类。
 *
 * 术语: 
 *    Advice(通知) -- 定义了切面是什么以及何时使用。有五种类型的通知: Before, After(执行后), After-returning(成功执行后), 
 *                          After-throwing(抛出异常后), Around(被通知的方法调用前和调用后)
 *    Joinpoint(连接点) -- 在应用执行过程中能够插入切面的一个时机点，切面代码可以利用这些点插入到应用的正常流程之中，并添加新的行为。
 *    Pointcut(切点) -- 切点有助于缩小切面所通知连接点的范围，其定义会匹配通知所要织入的一个或多个连接点，可使用明确的类和方法名称 或 正则表达式等指定。
 *                            通常是目标对象的特定方法。有些AOP框架允许创建动态的切点，可根据运行时的决策(如方法参数值)来决定是否应用通知。
 *    Aspect(切面) -- 通知和切点的结合，通知和切点共同定义了关于切面的全部内容 -- 是什么，在何时何何处完成其功能。
 *    Introduction(引入) -- 允许我们向现有的类添加新方法或属性，可以在无需修改现有类的情况下，使其具有新的行为和状态。
 *    Weaving(织入) -- 将切面应用到目标对象来创建新的代理对象的过程，在目标对象的生命周期里有多个点可以进行织入。
 *       编译期：   需要特殊的编译器，如 AspectJ
 *       类加载期：目标类加载到JVM时，需要特殊的类加载器(ClassLoader)，可以在目标类被引入应用之前增强该目标类的字节码。如 AspectJ 5 的LTW(load-time weaving)。
 *       运行期：   AOP容器为目标对象动态地创建一个代理对象，在应用运行的时候织入，拦截被通知的方法的调用，再将调用转发给真正的目标Bean。如 Spring AOP。
 *       
 * AOP框架
 *   AspectJ -- 除支持方法外，还支持属性、构造器拦截等更强大和细粒度的控制，但需要特有的AOP语言，在编译时织入
 *   JBoss AOP
 *   Spring AOP -- 提供了4种AOP支持方式，前三种都是基于代理的AOP变体，只支持方法连接点，使用标准的Java语言，在运行时织入。
 *      1.基于代理的经典AOP(使用 ProxyFactoryBean, 已被淘汰？)
 *      2.@AspectJ 注解驱动的切面
 *      3.纯 POJO 切面
 *      4.注入式 AspectJ 切面(适合Spring各版本) -- 可支持构造器或属性拦截等 高级功能，利用DI把Bean注入到AspectJ切面中实现。
 *   
 * 使用方式(Spring AOP 为例)：
 *    1.使用 AspectJ 的切点表达式语言定义切点 -- Spring AOP 只支持AspectJ切点指示器(pointcut designator)的一个子集。
 *      execution -- 用于匹配是连接点的执行方法
 *      例子: execution(* com.fishjam.Instrument.play(..)) and within(com.fishjam.test.*)   
 *         <== 表示当test包下的Instrument实现类的play() 方法执行时触发通知(返回值任意， 参数任意) 
 *    2. 编写通知
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * Spring 的AOP配置元素
 *    <aop:config>				-- 顶层的AOP配置
 *       <aop:aspect ref="othBean">				-- 定义切面
 *          <aop:advisor>				-- 定义AOP通知器
 *          <aop:after pointcut="execution(...)" method="xxx" arg-name="yyy" />-- 定义AOP后置通知(不管被通知的方法是否执行成功)， 
 *          <aop:after-returning pointcut-ref="myPointcut">	-- 定义成功后通知
 *          <aop:after-throwing>	-- 定义抛出异常后通知
 *          <aop:around>				-- 定义环绕通知(调用前和调用后)，其方法有 ProceedingJoinPoint 类型的参数，调用前 => joinpoint.proceed(); => 调用后
 *          <aop:aspectj-autoproxy> -- 启用@AspectJ注解驱动的切面
 *          <aop:before>				-- 定义AOP前置通知 
 *          <aop:declare-parents> -- 声明了此切面所通知的Bean在它的对象层次结构中拥有新的父类型。
 *          <aop:pointcut id="myPointcut" expression="execution(yyy)" /> -- 定义切点，然后其他的同质元素中可以重用
 *   
**************************************************************************************************************************************/

public class AopTest extends TestCase{
	
	public void testAop(){
		assertEquals( "AopTest", 1, 0);
	}
	
}
