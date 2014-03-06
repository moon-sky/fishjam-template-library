package com.fishjam.utility.app;

/**
 * 异常(不是编码错误--数组越界，除0等 是编码错误而不是异常)-- try...catch...finally
 *   使用 throws 异常类型列表 表明函数中可能发生异常，调用者必须进行 try...catch 处理 或声明为throws继续向上传递
 *   使用 throw new 异常类型 主动抛出异常
 *   一个方法被覆盖时，覆盖它的方法必须抛出相同的异常或异常的子类--子类能抛出的异常只能比父类的少
 *   类继承关系：Throwable -> Exception -> 自定义异常类
 *
 * 异常处理后有两种模式：
 *   1.终止当前执行逻辑，无法回到异常发生点；
 *   2.异常处理程序有能力修正当前错误，然后重新执行发生错误的函数，并成功执行(否则会死循环)
 *   Java使用的是终止模型，但可用while的方式模拟修正模型
 *   
 *
 * throws 异常列表 -- 说明异常规格(Exception Specification)，如是throwable表可抛出任何异常，
 * Java使用异常类名区分异常(最重要的就是Class名)，一般只需从特定异常类继承一个新类即可。  
 *
 * 重新抛出异常：
 *   catch(Exception e) 
 *   {System.err.println(“An exception wan thrown”);throw e;} 或 抛出新的异常类型
 *   重抛异常会把异常抛给上一级环境中的异常处理程序。同一个try块的后续catch子句将被忽略。
 *   此外，异常对象的所有信息都得以保持，所以高一级环境中捕获此异常的处理程序可以从这个异常对象中得到所有信息。
 *   如果你只是把当前异常对象重新抛出，那么printStackTrace()方法显示的将是原来异常抛出点的调用栈信息，而并非重新抛出点的信息。
 *   要想更新这个信息，你可以调用fillInStackTrace()方法，这将返回一个Throwable对象，
 *   它将通过把当前调用栈信息填入原来那个异常对象而建立的。
 * 
 * 异常链
 *   你常常会想要在捕获一个异常后抛出另一个异常，并且希望把原始异常的信息保存下来，这被称为“异常链”(Exception chaining)。
 *   现在所有Throwable的子类在构造器中都可以接受一个cause对象作为参数。这个cause就用来表示原始异常，
 *   这样通过把原始异常传递给新的异常，使得即使你在当前位置创建并抛出了新的异常，你也能通过这个异常链追踪到异常最初发生的位置。
 * 
 * Java标准异常
 *    Throwable这个Java类被用来表示任何可以作为异常被抛出的类。Throwable对象可分为两种类型（指从Throwable继承而得到的类型）：
 *      a.Error 编译期和系统错误（除了特殊情况）；
 *      b.Exception 是可以被抛出的基本类型，在Java类库、用户方法以及运行时故障中都可能被抛出Exception型异常。
 *   所以Java程序员关心的主要是Exception。
 */

public class ClassExceptionHandler {

}
