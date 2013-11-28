#pragma once

#include <cppunit/extensions/HelperMacros.h>

/**********************************************************************************************
* 基本核心库，包含底层平台的封装层，也包含经常使用的工具类和函数
*
**********************************************************************************************/

/**********************************************************************************************
* 智能指针(smart pointer)和共享指针(shared pointer) -- AutoPtr.h,SharedPtr.h 
* 引用计数器的垃圾回收机制 AtomicCounter.h,refCountedObject.h
*
* BinaryReader/BinaryWriter -- 二进制流的读写，自动、透明地处理字节顺序问题
* 
* Logging -- 可扩展的日志框架，该框架支持过滤、路由到不同的通道以及格式化日志信息等。
*   可以输出到多个目的地(console,log files,Unix Syslog,Windows Event Log,remote logging)
*
* NotificationCenter/NotificationQueue -- 异步通知?
*   source.theEvent += Delegate<Target, int>(&target,&Target::onEvent);
*
* SharedLibrary -- 运行时装载及卸载的共享库，方便以平台无关的方式编写应用程序插件。
*
a.Timestamp 微秒级精度，对gettimeofday()封装,参考Timestamp.h   
b.Stopwatch 计时器,参考Stopwatch.h   
c.DateTime  日历时间,参考DateTime.h    
d.日历时间的格式转换DateTimeFormat,DateTimeFormatter,DateTimeParser等   
e.日历时区Timezone   
f.本地时间LocalDateTime



**********************************************************************************************/

class CPocoFoundationTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CPocoFoundationTester );
    CPPUNIT_TEST( test_DateTime );
    CPPUNIT_TEST_SUITE_END();

    DECLARE_DEFAULT_TEST_CLASS(CPocoFoundationTester);
private:
    void test_DateTime();
    void test_DateTimeFormat();
    void test_Stopwatch();
};
