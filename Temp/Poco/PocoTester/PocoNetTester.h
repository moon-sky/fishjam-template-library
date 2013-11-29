#pragma once

#include <cppunit/extensions/HelperMacros.h>

/**********************************************************************************************
* Poco
*   ::Net -- 
*     ::NetCore -- 为其它网络应用提供核心功能组件,比如 network address,DNS 类
*     ::NetSSL -- NetSSL为Net库中的网络类提供SSL支持
*     ::Socket 
*     ::Reactor 
*
* 基于socket 类提供两个框架创建TCP服务器:
*   1.多线程服务器(每个连接一个线程, 从线程池获得)
*   2.基于Acceptor-Reactor模式的服务器
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
