#pragma once

#include <cppunit/extensions/HelperMacros.h>

/**********************************************************************************************
* Poco
*   ::Net -- 
*     ::NetCore -- 为其它网络应用提供核心功能组件,比如 network address,DNS 类
*     ::NetSSL -- NetSSL为Net库中的网络类提供SSL支持
*     ::Socket 
*     ::Reactor 
**********************************************************************************************/

class CPocoNetTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CPocoNetTester );
    CPPUNIT_TEST( test_container_holder );
    CPPUNIT_TEST_SUITE_END();

    DECLARE_DEFAULT_TEST_CLASS(CPocoNetTester);
private:
    void test_container_holder();
};
