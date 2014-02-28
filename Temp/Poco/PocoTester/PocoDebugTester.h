#pragma once

#include <cppunit/extensions/HelperMacros.h>

/**********************************************************************************************
* Poco 采用异常的方式来处理业务逻辑，
*   Poco::Exception -- 基类，继承自std::exception
*   +-LogicException -- 该类及其子类负责处理程序错误(一般都是代码逻辑错误)，包括AssertionViolation、NullPointer等
*   +-ApplicationException -- 负责处理应用程序相关的错误
*   +-PosoRuntimeException -- 负责处理程序运行时的错误
*   相关方法:
*     displayText() -- 同时返回异常名字和消息字符串，中间使用": "分隔
*     nested() -- 如果存在嵌套异常的话，返回指向嵌套异常的指针，否则返回NULL
*
* 自定义异常
*   POCO_DECLARE_EXCEPTION -- 申明异常的宏
*   POCO_IMPLEMENT_EXCEPTION -- 定义异常宏的执行体
*   如：
*      .h --   POCO_DECLARE_EXCEPTION(MyLib_API, MyException, Poco::Exception) 
*      .cpp -- POCO_IMPLEMENT_EXCEPTION(MyException, Poco::Exception, "Something really bad happened") 
**********************************************************************************************/

/**********************************************************************************************
* 断言 -- POCO库中提供了一些断言的宏来进行运行时检查，这些断言能够提供出错代码的行号和文件信息。
*   断言类在debug调试模式下(比如在Visual C++)中时，会触发一个breakpoint
* 
*   Debugger
*     _assert(cond) -- 如果 !cond 时，抛出一个AssertionViolationException异常。
*     enter("提示信息") -- 调试版本下进入断言(如VC 使用 DebugBreak API 函数)
*   Bugcheck -- 调试帮助
*     全局辅助函数
*       poco_bugcheck_msg(string) -- 抛出BugcheckException异常, 等价于 enter ?
*       poco_assert_dbg(cond) -- 同poco_assert类似，但是只在debug模式下起作用
*       poco_check_ptr(ptr) -- 如果ptr为空，则抛出NullPointerException异常
*     
**********************************************************************************************/

/**********************************************************************************************
* NDC(Nested Diagnostic Context) -- 为了多线程诊断而设计的(使用TLS为每个线程产生各自的日志)
*   Poco::NestedDiagnosticContext -- 维护一个NDC对象，其中包括了上下文的栈信息，有函数方法名，源文件代码文件名，行号.
*   宏 poco_ndc(func)|poco_ndc_dbg(func) -- 声明了一个 NDCScope 对象，由其完成上下文的入栈、出栈工作。
*
*   1.DiagnosticLogger -- 分离日志和程序其他模块
*   2.TransactionalBuckets -- 事务桶，为事务单独建立日志
*   3.TypedDiagnostics -- 类型化诊断，为所有的诊断信息提供统一的展现
**********************************************************************************************/

class CPocoDebugTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CPocoDebugTester );
    CPPUNIT_TEST( test_NDC );
    CPPUNIT_TEST_SUITE_END();

    DECLARE_DEFAULT_TEST_CLASS(CPocoDebugTester);
private:
    void test_NDC();
};
