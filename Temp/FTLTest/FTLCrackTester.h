#pragma once

#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* 
************************************************************************/

class CFTLCrackTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CFTLCrackTester );

    CPPUNIT_TEST( test_GetCrackString );
    CPPUNIT_TEST( test_WinVerifyTrust );

    CPPUNIT_TEST_SUITE_END();

    void test_GetCrackString();
    void test_WinVerifyTrust();

    DECLARE_DEFAULT_TEST_CLASS(CFTLCrackTester);
};
