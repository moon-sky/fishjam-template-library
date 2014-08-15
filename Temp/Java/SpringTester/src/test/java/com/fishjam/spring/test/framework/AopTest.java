package com.fishjam.spring.test.framework;

import org.aspectj.lang.ProceedingJoinPoint;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Pointcut;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.AbstractJUnit4SpringContextTests;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import com.fishjam.springtester.domain.Student;
import com.fishjam.springtester.tools.PerformanceCheck;

import junit.framework.TestCase;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

/**************************************************************************************************************************************
 * AOP 是面向对象编程的一个强大补充。
 * 
 * TODO： 
 *   1.AOP的本质就是Hook？ (代理类型的)切面只是实现了它们所包装Bean的相同接口的代理。
 *   2.实现AOP功能的对象中，怎么访问激活连接点的对象？
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
 * AspectJ切点表达式语言(Spring AOP中使用) -- 注意：Spring仅支持AspectJ切点指示器(pointcut designator)的一个子集
 *   TODO: maven 中需要怎么引入依赖？ <org.aspectj><aspectjweaver> ?
 *   args(String) -- 限制连接点匹配参数为指定类型的执行方法, 如 args(param1)指定将param1作为方法的参数, 
 *      然后可在通知(advice)的设置中通过 arg-names="param1" 标识该参数必须传递给对应的方法 
 *   @args() -- 限制连接点匹配参数由指定注解标注的执行方法
 *   bean() -- 运行使用Bean的Id来标识Bean，限制切点只匹配特定的Bean
 *   execution() -- 用于匹配是连接点的执行方法
 *   this() -- 限制连接点匹配AOP代理的Bean引用为指定类型的类
 *   target() -- 限制连接点匹配目标对象为指定类型的类
 *   @target() -- 限制连接点匹配特定的执行对象，这些对象对应的类要具备指定类型的注解
 *   within() -- 限制连接点匹配指定的类型
 *   @within() -- 限制连接点匹配指定注解所标注的类型
 *   逻辑符号(用于注解时?)和对应的字面字符(用于XML中时): &&(and), ||(or), !(not)
 * 
 * Spring 的AOP配置元素： 
 *   <aop:config>  -- 顶层的AOP匹配元素，大多数 <aop:*> 元素必须包含在 <aop:config> 元素内
 *     <aop:aspect ref="beanId"> -- 定义切面，一般通过 ref="beanId" 的方式引用实现切面功能(如 记录日志等)的对象
 *       <aop:pointcut id="myPointcut" expression="execution(yyy)" /> -- 定义切点，定义切点，其 expression 属性的值就是
 *          AspectJ切点表达式语法 所定义的切点，可通过id属性指定其名字，然后通知元素中可以直接引用
 *       <aop:advisor> -- 定义AOP通知器
 *       <aop:after pointcut="execution(...)" method="xxx" arg-name="yyy" />-- 定义AOP后置通知(不管被通知的方法是否执行成功)， 
 *       <aop:after-returning pointcut-ref="myPointcut">	-- 定义成功后通知
 *       <aop:after-throwing> -- 定义抛出异常后通知
 *       <aop:around> -- 定义环绕通知(调用前和调用后)，其方法有 ProceedingJoinPoint 类型的参数，调用前 => joinpoint.proceed(); => 调用后
 *       <aop:before> -- 定义AOP前置通知 
 *       <aop:declare-parents> -- 声明了此切面所通知的Bean在它的对象层次结构中拥有新的父类型。即为被通知的对象引入额外的接口，并透明地实现。
 * 
 * 为通知传参：利用 args(xxx) 和 advice 中的 arg-names
 * 通过切面引入(introduction)新功能 -- 引入新的接口，当其方法被调用时，代理将此调用委托给实现了该新接口的某个其他对象，即Bean的实现被拆分到了多个类
 *      
 * 示例(参见 DemoBeans.xml 中的 mylog):
 *    1.<aop:pointcut id="doAction" expression="execution(* *.DoAction(..))" />   --  and within(com.fishjam.*)
 *       定义了一个id为 "doAction"的切点， 然后可被 通知(advice) 等引用;
 *       execution 表示 在方法执行时触发;
 *       (* *.DoAction(..))  -- 表示切点为： 返回任意类型的，任意包、类中的 DoAction 方法， 其参数可为任意值
 *       within(com.fishjam.*) -- 表示切点仅匹配 com.fishjam.* 包中的内容
 *   2.  为 Xxx 实现新的接口，
 *       <aop:declare-parents types-matching="com.fishjam.springtester.domain.Student" 
 *           implement-interface="com.fishjam.springtester.tools.PerformanceCheck" 
 *           default-impl="com.fishjam.springtester.tools.DefaultPerformanceCheck"/>
 *       类型匹配 Student 的所有Bean(注意：是Bean而不是类 ?)会实现 PerformanceCheck 接口，
 *       其对应的实现为 DefaultPerformanceCheck类，也可以用 delegate-ref 引用Bean作为引入的委托
**************************************************************************************************************************************/ 

/**************************************************************************************************************************************
 * <aop:aspectj-autoproxy /> -- 启用@AspectJ 注解驱动的切面(Spring内部使用名为 AnnotationAwareAspectJAutoProxyCreator 的自动代理创建类)
 * 注解切面的支持 (AspectJ 5 以后的特性 -- @AspectJ) -- 可以不再使用XML进行配置
 *   @Aspect -- 标注类为一个切面
 *   @Pointcut(value("execution(* *.DoAction(..))"))   //args(param1) 
 *      public void myPointCutName(ParType1 param1) {} -- 定义一个可以在切面内重用的切点，
 *      方法名为切点名称，可在通知等地方进行重用，方法内容不重要(只是一个标识，一般为空)
 *   @Before("myPointCutName(param1)") public void xxx(){ .... } -- 定义前置通知时需要执行的方法内容
 *   @Around("myPointCutName(param1)")  -- 注解环绕通知
 *   @DeclareParents(value="com.fishjam.springtester.domain.Student+", defaultImpl="DefaultPerformanceCheck.class")
 *      public static PerformanceCheck performanceCheck;      -- 标注引入新功能
 *    
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * 使用传统的AspectJ切面(有 特殊的语法，需要 Xxx 的支持 ?) -- 可以支持属性、构造器拦截等更强大和细粒度的控制
 *    public aspect JudgeAspect{										//定义裁判 Aspect
 *      public JudgeAspect() { } 		
 *    	pointcut performance() : execution( * perform(..));		//定义切点，匹配 perform() 方法
 *    	after() returning() :  performance() {							//在表演(perform) 结束后为表演发表评论
 *    		System.out.println(...);
 *    	}
 *    } 
 * 
 * 在配置AspectJ相关的Bean时，需要指定 factory-method="aspectOf" -- AspectJ切面由AspectJ在运行期创建的，而不是由Spring容器初始化，
 *   因此由静态的 aspectOf() 方法返回切面的单例
 *   <bean id="xxxAspect" class="xxxAspect" factory-method="aspectOf"> ... </bean>
**************************************************************************************************************************************/

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations={"classpath:DemoBeans.xml"})
public class AopTest extends AbstractJUnit4SpringContextTests {
	
	@Autowired
	private Student student;
	
	@Test
	public void testAopntroduction(){
		//<aop:declare-parents> 给 Student 引入了新的 PerformanceCheck 接口
		//此处 getCallCount 返回固定的 99
		PerformanceCheck performanceCheck = (PerformanceCheck)student;
		assertEquals(performanceCheck.getCallCount(), 99);	
	}
	
	/**********************************************************************************************************
	 * AOP(切面编程) -- 确保POJO保持简单
	 *   通常用于将系统服务(如日志、安全控制、事务管理等)模块化，并从业务对象中分离出来，以声明的方式应用进去
	 *
	 * 示例：
	 *    在 Student.DoAction 的前后自动调用 MyLog 对应的方法记录日志，在Console中会看到 System.out 打印出的日志
	 *    TODO:  怎么给 logBeforeAction 传递参数（如 方法名)
	 *    
	 * 使用方法（参见 DemoBeans.xml）-- 
	 *    1.将对应的服务类 (MyLog) 声明为一个Bean;
	 *    2.在 <aop:aspect> 中通过 ref 引用该Bean;
	 *    3.通过 <aop:pointcut> 定义切入点和表达式(expression)属性来选择所应用的通知，其语法为 AspectJ 的切点表达式语言
	 *    4.声明需要调用的切面方法和时机 <aop:before>, <aop:after>, <aop:异常> 等
	 *    
	 * 常见错误
	 *    1.NoClassDefFoundError: org/aspectj/weaver/reflect/ReflectionWorld$ReflectionWorld
	 *      使用了AOP 和 AspectJ 语法，但没有加入依赖包 <org.aspectj><aspectjweaver>
	 ***********************************************************************************************************/
	@Test
	public void testStudentDoActionWithLog(){
		student.DoAction();
	}
}
