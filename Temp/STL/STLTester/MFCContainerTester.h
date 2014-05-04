#pragma once

#include <cppunit/extensions/HelperMacros.h>
#include <atlcoll.h>

/**********************************************************************************************
* CPtrList 不是模版，保存的是 void*
**********************************************************************************************/

class CMFCContainerTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CMFCContainerTester );
    CPPUNIT_TEST( test_CPtrList );
    CPPUNIT_TEST_SUITE_END();

    DECLARE_DEFAULT_TEST_CLASS(CMFCContainerTester);
private:
    void test_CPtrList();
};