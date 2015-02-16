#pragma once
#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* 
************************************************************************/

class CFTLShellTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CFTLShellTester );
    CPPUNIT_TEST( test_ITEMIDLIST );
    CPPUNIT_TEST( test_LaunchIE );
    CPPUNIT_TEST( test_GetFileVersionInfo );
    CPPUNIT_TEST_SUITE_END();

    void test_ITEMIDLIST();
    void test_LaunchIE();
    void test_GetFileVersionInfo();

    DECLARE_DEFAULT_TEST_CLASS(CFTLShellTester);
};
