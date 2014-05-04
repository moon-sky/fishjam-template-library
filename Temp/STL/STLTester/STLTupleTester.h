#pragma once

#include <cppunit/extensions/HelperMacros.h>
/************************************************************************
* tuple -- 类似 pair,但允许将2至10个元素包装在一起
*   make_pair用于初始化pair,类似的VS2010有可传入2至10个参数的重载函数make_tuple
************************************************************************/
class CSTLTupleTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CSTLTupleTester );
    CPPUNIT_TEST( test_tuple_init );
    CPPUNIT_TEST_SUITE_END();

	void test_tuple_init();

    DECLARE_DEFAULT_TEST_CLASS(CSTLTupleTester);
};