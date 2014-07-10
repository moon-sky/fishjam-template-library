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
    CPPUNIT_TEST_SUITE_END();

    void test_ITEMIDLIST();
    DECLARE_DEFAULT_TEST_CLASS(CFTLShellTester);
};
