#pragma once
#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* 
************************************************************************/

class CFTLStringTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CFTLStringTester );
    CPPUNIT_TEST( test_IsMachMask );
    CPPUNIT_TEST_SUITE_END();

    void test_IsMachMask();
    DECLARE_DEFAULT_TEST_CLASS(CFTLStringTester);
};
