#pragma once

#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* valarray -- 
************************************************************************/

class CSTLValarrayTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CSTLValarrayTester );

    CPPUNIT_TEST( test_1 );

    CPPUNIT_TEST_SUITE_END();

    void test_1();

    DECLARE_DEFAULT_TEST_CLASS(CSTLValarrayTester);
};
