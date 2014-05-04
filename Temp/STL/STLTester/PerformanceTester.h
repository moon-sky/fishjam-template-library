#pragma once

#include <cppunit/extensions/HelperMacros.h>

/**********************************************************************************************
**********************************************************************************************/

class CPerformanceTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CPerformanceTester );
    CPPUNIT_TEST( test_container_holder );
    CPPUNIT_TEST_SUITE_END();

    DECLARE_DEFAULT_TEST_CLASS(CPerformanceTester);
private:
    void test_container_holder();
};
