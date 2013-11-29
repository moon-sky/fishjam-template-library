#pragma once

#include <cppunit/extensions/HelperMacros.h>

/**********************************************************************************************
* Poco 采用异常的方式来处理业务逻辑
**********************************************************************************************/
#include "Poco/Bugcheck.h"      //调试帮助

class CPocoDebugTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CPocoDebugTester );
    CPPUNIT_TEST( test_container_holder );
    CPPUNIT_TEST_SUITE_END();

    DECLARE_DEFAULT_TEST_CLASS(CPocoDebugTester);
private:
    void test_container_holder();
};
