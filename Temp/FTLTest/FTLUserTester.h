#pragma once

#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* 
************************************************************************/

class CFTLUserTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CFTLUserTester );
    CPPUNIT_TEST( dump_TokenInfo );
    CPPUNIT_TEST_SUITE_END();

    void dump_TokenInfo();

    DECLARE_DEFAULT_TEST_CLASS(CFTLUserTester);
};
