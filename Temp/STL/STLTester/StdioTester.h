#pragma once

#include <cppunit/extensions/HelperMacros.h>

/****************************************************************************************
* 
****************************************************************************************/
class CStdioTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CStdioTester );
    CPPUNIT_TEST( test_string_func );
    CPPUNIT_TEST_SUITE_END();

    void test_string_func();

    DECLARE_DEFAULT_TEST_CLASS(CStdioTester);
};