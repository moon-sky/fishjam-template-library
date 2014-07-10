#pragma once

#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* 
************************************************************************/

class CFTLComTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CFTLComTester );

    CPPUNIT_TEST( test_RegisterComponent );
    CPPUNIT_TEST( test_SbsCreateInstance );

    CPPUNIT_TEST_SUITE_END();

    void test_RegisterComponent();
    
    //SideBySide
    void test_SbsCreateInstance();

    DECLARE_DEFAULT_TEST_CLASS(CFTLComTester);
};
