#pragma once

#include <cppunit/extensions/HelperMacros.h>

/**********************************************************************************************
* 提供了一个访问各类配置文件格式的接口，可以支持多种格式
*   Java-style的property文件 -- PropertyFileConfiguration.h
*   windows风格的.ini文件 -- IniFileConfiguration.h
*   XML -- XMLConfiguration.h
*
* Command Line Options --  处理与平台无关的命令行参数
*   options.addOption(
*      Option("长名字", "短名", "描述信息").required(false).repeatable(false)
*         .argument("value").binding("绑定到的配置属性"));
**********************************************************************************************/

class CPocoConfigurationTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CPocoConfigurationTester );
    CPPUNIT_TEST( test_container_holder );
    CPPUNIT_TEST_SUITE_END();

    DECLARE_DEFAULT_TEST_CLASS(CPocoConfigurationTester);
private:
    void test_container_holder();
};
