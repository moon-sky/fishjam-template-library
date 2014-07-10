#pragma once
#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* 
************************************************************************/

class CFTLMemTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CFTLMemTester );

    CPPUNIT_TEST( test_MemLeakBase );

    CPPUNIT_TEST_SUITE_END();

    void test_MemLeakBase();

    DECLARE_DEFAULT_TEST_CLASS(CFTLMemTester);
};
