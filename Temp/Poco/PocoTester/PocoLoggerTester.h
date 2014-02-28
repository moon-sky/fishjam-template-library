#pragma once

#include <cppunit/extensions/HelperMacros.h>
#include "Poco/Logger.h"

/**********************************************************************************************
* ::Logger -- 日志框架,并且该类是一个用于产生 Message的特殊Channel，是整个链的头，通过 setChannel 设置后续的责任链
*    get() -- POCO库在内部管理了一个全局的日志map,用户可通过get获取到和指定名字关联的logger对象引用
*      m_logger(Poco::Logger::get("default")) -- 定义一个消息源名字为 "default" 的Logger
*    setLevel -- ? 可以设置过滤消息的优先级，只有消息的优先级比设置的高才会被传递
* 
* 日志由三部分组成: 
*   各个部分可以通过 setProperty() 方法设置键值对格式的属性控制动作，具体能设置的属性可以参见头文件
*   Channel -- 通道对象，负责传递消息，并最终把消息送到目的地，通过引用计数来管理生存期
*     setChannel(Channel* pChannel) -- 设置后续处理Channel(责任链式模式?)
*     +-AsyncChannel -- 允许在另外一个分离的线程中去记录通道的日志。通过一个先进先出的消息队列进行中转。
*     +-ConsoleChannel -- 满足大多数的控制台输出,只是简单的把消息内容写入了标准输出流(std::clog)
*     +-EventLogChannel -- 事件日志通道，写到Windows事件日志中。使用和查看时依赖于 PocoFoundation.dll 文件。
*     +-FileChannel -- 提供了完整的日志支持，支持按文件大小和时间间隔对日志进行循环覆盖，支持自动归档(使用不同的文件命名策略),
*         支持压缩(GZIP)和清除(根据已归档文件的日期或数量)归档日志文件。
*     +-FormattingChannel -- 会把它接受到的每一个消息通过 Formatter 传递给下一个的输出通道
*     +-NullChannel -- 抛弃所有发向它的消息，并且忽略所有setProperty()函数设置的属性
*     +-RemoteSyslogChannel -- 可以通过基于UDP的系统日志协议(Syslog protoco)把日志输出到远程的Linux主机日志守护程序上
*     +-SimpleFileChannel -- 向日志文件输出的简单功能。对于每一个消息，其内容都会被添加到文件中，并使用一个新行输出。
*         支持两个(secondaryPath)日志文件的循环(rotation="10M")
*     +-SplitterChannel -- 可以把消息发送给一个或者多个(通过 addChannel 增加)其他的通道，即输出日志在多个目标中
*     +-StreamChannel
*     +-SyslogChannel -- 把日志输出到Linux系统的本地系统日志守护程序
*     +-WindowsConsoleChannel -- 向windows控制台输出,支持UTF-8编码
*   Formater -- 消息输出时对消息进行格式化
*     +-PatternFormatter -- 可以根据打印格式来格式化消息
*   Message -- 所有的消息都被存储并通过类Poco::Message传递,其中包括 优先级、消息源、内容、时间戳、线程标记、键值对格式的消息参数等
* 流程：继承自Channel的子类实例,通过Formater实例对Message进行格式化，然后传递到目的地。
* 
* LogStream -- 提供了一个日志的输出流接口。可以在日志流中，格式化输出日志记录消息。日志消息必须以std::endl(或CR和LF字符)结尾
**********************************************************************************************/

/**********************************************************************************************
* 日志效率
*   1.消息通常情况下是通过引用的方式传递给下一个通道。例外的情况是，FormattingChannel和AsyncChannel类。它们会生成消息的一个副本
*   2.尽可能的避免频繁的调用Logger::get()函数，更好的方法是在通过函数获得日志(logger)对象引用后，保存并使用
*   3.记录和输出日志的效率取决于日志输出的通道。通道的效率非常依赖于操作系统的实现。
**********************************************************************************************/

class CPocoLoggerTester : public CPPUNIT_NS::TestFixture
{
public:
    CPocoLoggerTester();
    virtual ~CPocoLoggerTester();
    virtual void setUp();
    virtual void tearDown();

public:
    CPPUNIT_TEST_SUITE( CPocoLoggerTester );
    CPPUNIT_TEST( test_AsyncLogger );
    CPPUNIT_TEST( test_Logger );
    CPPUNIT_TEST( test_SplitterLogger );
    CPPUNIT_TEST( test_LogStream );
    CPPUNIT_TEST_SUITE_END();

    //DECLARE_DEFAULT_TEST_CLASS(CPocoLoggerTester);
    DISABLE_COPY_AND_ASSIGNMENT(CPocoLoggerTester);
private:
    void test_AsyncLogger();
    void test_Logger();
    void test_SplitterLogger();
    void test_LogStream();
    Poco::Logger& m_logger;
};
