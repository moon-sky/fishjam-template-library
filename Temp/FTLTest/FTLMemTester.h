#pragma once
#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* 
************************************************************************/

class CFTLMemTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CFTLMemTester );

    CPPUNIT_TEST( test_GlobalFunctions );
    CPPUNIT_TEST( test_MemLeakBase );
    CPPUNIT_TEST( test_CreateStreamOnHGlobal );

    CPPUNIT_TEST_SUITE_END();

    void test_GlobalFunctions();

    void test_MemLeakBase();
    void test_CreateStreamOnHGlobal();

    DECLARE_DEFAULT_TEST_CLASS(CFTLMemTester);

private:
    std::vector<HGLOBAL>    m_AllocMemories;
    void _AllocLargeMeomories(int nBLockSize, int nBlockCount, UINT nFlags);
    void _ClearAllocMeomories();
};
