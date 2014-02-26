#pragma once

#include <cppunit/extensions/HelperMacros.h>

/**********************************************************************************************
* Poco
*   ::Logger -- 日志框架,并且该类是一个用于产生 Message的特殊Channel，是整个链的头，通过 setChannel 设置后续的责任链
*      m_logger(Poco::Logger::get("default"))
*   日志由三部分组成:
*     Channel -- 子类(FileChannel,ConsoleChannel,StreamChannel, AsyncChannel,NullChannel,FormattingChannel 等)
*       setProperty("path", path); 
*       setChannel(Channel* pChannel) -- 设置后续处理Channel(责任链式模式?)
*     Formater -- 子类(PatternFormatter)
*       setProperty("times", "local");
*       setProperty("pattern", "%Y-%m-%d %H:%M:%S [%p] %T %t %U(%u)");
*     Message --
*   流程：继承自Channel的子类实例,通过Formater实例对Message进行格式化，然后传递到目的地。
*   
**********************************************************************************************/

class CPocoLoggerTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CPocoLoggerTester );
    CPPUNIT_TEST( test_container_holder );
    CPPUNIT_TEST_SUITE_END();

    DECLARE_DEFAULT_TEST_CLASS(CPocoLoggerTester);
private:
    void test_container_holder();
};
