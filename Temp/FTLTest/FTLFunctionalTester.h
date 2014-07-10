#pragma once
#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* 
************************************************************************/

class CFTLFunctionalTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CFTLFunctionalTester );
    CPPUNIT_TEST( test_PairDeleter );
    CPPUNIT_TEST( test_Split );
    CPPUNIT_TEST( test_find_nearest );
    CPPUNIT_TEST( test_binarystream );
    CPPUNIT_TEST_SUITE_END();

    void test_PairDeleter();
    void test_Split();
    void test_find_nearest();
    void test_binarystream();

    DECLARE_DEFAULT_TEST_CLASS(CFTLFunctionalTester);
private:
    void test_find_nearest_set_int();
    void test_find_nearest_set_struct();
    void test_find_nearest_map_int();
    void test_find_nearest_empty();

};
