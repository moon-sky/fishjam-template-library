#pragma once
#include <cppunit/extensions/HelperMacros.h>
#include <atlcoll.h>

/**********************************************************************************************

**********************************************************************************************/

class CATLCollectionTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CATLCollectionTester );
	CPPUNIT_TEST( test_CAtlArray );
	CPPUNIT_TEST( test_CAtlList );
    CPPUNIT_TEST( test_CAtlMap );
    CPPUNIT_TEST_SUITE_END();

    DECLARE_DEFAULT_TEST_CLASS(CATLCollectionTester);
private:
    void test_CAtlArray();
	void test_CAtlList();
	void test_CAtlMap();
};
