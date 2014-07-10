#pragma once
#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* 
************************************************************************/

class CUuidFinderTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CUuidFinderTester );

    CPPUNIT_TEST( test_UuidFinder );

    CPPUNIT_TEST_SUITE_END();

    void test_UuidFinder();

    DECLARE_DEFAULT_TEST_CLASS(CUuidFinderTester);
};
