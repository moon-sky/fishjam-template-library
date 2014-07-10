#pragma once

#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* 
************************************************************************/

class CFTLSystemTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CFTLSystemTester );
    CPPUNIT_TEST( test_IntelIsLittleSystem );

    CPPUNIT_TEST_SUITE_END();

    void test_IntelIsLittleSystem();

    DECLARE_DEFAULT_TEST_CLASS(CFTLSystemTester);
};
