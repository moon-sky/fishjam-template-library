#pragma once

#include <cppunit/extensions/HelperMacros.h>
#include "Poco/Types.h"     //基本数据结构的统一，包括不同平台int、long等数据结构的定义


/**********************************************************************************************
* 基本核心库，包含底层平台的封装层，也包含经常使用的工具类和函数
*   
* 主要部分:
*   Core -- 建立跨平台库的基础头文件，封装了原子计数的基类(AtomicCounter), 垃圾收集(AutoPtr，SharedPtr)
*   Cache -- 内存Cache
*   Crypt -- 数字摘要
*   Filesystem -- 文件系统，主要是对文件本身的操作，如移动，拷贝文件等
*   Hashing -- Hash表
*   Logging -- 日志系统
*   Notifications -- 通知
*   Processes -- 进程通讯
*   RegularExpression -- 正则表达式，依赖于PCRE库.(http://www.pcre.org)
*   SharedLibrary -- 运行时动态加载/卸载共享库，方便以平台无关的方式编写应用程序插件。
*   Streams -- 流
*   Tasks -- 任务
*   Text -- 文本转换
*   Threading -- 多线程
*   URI -- 
*   UUID -- UUID生成和操作
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
*
a.Timestamp 微秒级精度，对gettimeofday()封装,参考Timestamp.h   
b.Stopwatch 计时器,参考Stopwatch.h   
c.DateTime  日历时间,参考DateTime.h    
d.日历时间的格式转换DateTimeFormat,DateTimeFormatter,DateTimeParser等   
e.日历时区Timezone   
f.本地时间LocalDateTime

**********************************************************************************************/


/**********************************************************************************************
* 导出插件类 -- TODO: 同一个文件中导出多个 Plugin 而不是一个目录下有多个文件，各自导出自己的Plugin?
*   1.定义 class AbstractPlugin { virtual SomeFunc() = 0; }
*   2.实现多个具体的插件类 class PluginA: public AbstractPlugin { ... }
*   3.使用Poco预定义的宏进行导出(编译时实现多态)
*     POCO_BEGIN_MANIFEST(AbstractPlugin)
*        POCO_EXPORT_CLASS(PluginA)
*     POCO_END_MANIFEST
*   4.实现初始化和清除函数(可选) -- 名字固定?
*     void pocoInitializeLibrary() { ... } 和 void pocoUninitializeLibrary() { ... } 
**********************************************************************************************/


class CPocoFoundationTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CPocoFoundationTester );
    CPPUNIT_TEST( test_DateTime );
    CPPUNIT_TEST( test_PluginLoader );
    CPPUNIT_TEST( test_SharedLibrary );
    CPPUNIT_TEST_SUITE_END();

    DECLARE_DEFAULT_TEST_CLASS(CPocoFoundationTester);
private:
    void test_DateTime();
    void test_PluginLoader();
    void test_SharedLibrary();


    void test_DateTimeFormat();
    void test_Stopwatch();
};
