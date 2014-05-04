#pragma once

#include <cppunit/extensions/HelperMacros.h>
/************************************************************************
* thread
************************************************************************/
class CSTLThreadTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CSTLThreadTester );
    //CPPUNIT_TEST( test_ifstream );
    CPPUNIT_TEST_SUITE_END();

    //void test_ifstream();

    DECLARE_DEFAULT_TEST_CLASS(CSTLThreadTester);
};