#pragma once

#include <cppunit/extensions/HelperMacros.h>

/**********************************************************************************************
* Xml解析器是对 Expat(http://www.libexpat.org) 解析库的包装, 支持工业标准SAX及DOM接口
* 
*   SAX -- 定义了基于事件的XML读接口，不需要将整个XML文档装载到内存中
*   DOM -- 让应用程序访问整个XML文档, 使用树形的对象层级
**********************************************************************************************/

class CPocoXMLTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CPocoXMLTester );
    CPPUNIT_TEST( test_container_holder );
    CPPUNIT_TEST_SUITE_END();

    DECLARE_DEFAULT_TEST_CLASS(CPocoXMLTester);
private:
    void test_container_holder();
};
