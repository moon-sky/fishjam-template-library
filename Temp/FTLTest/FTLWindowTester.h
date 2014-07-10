#pragma once

#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* 
************************************************************************/

class CFTLWindowTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CFTLWindowTester );

    CPPUNIT_TEST( test_MessageBoxHook );

    CPPUNIT_TEST_SUITE_END();

    void test_MessageBoxHook();

    DECLARE_DEFAULT_TEST_CLASS(CFTLWindowTester);
};
