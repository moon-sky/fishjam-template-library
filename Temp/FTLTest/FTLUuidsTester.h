#pragma once

#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* 
************************************************************************/

class CFTLUuidsTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CFTLUuidsTester );
    
    CPPUNIT_TEST( test_GetUuidName );
    
    CPPUNIT_TEST_SUITE_END();

    void test_GetUuidName();
    

    DECLARE_DEFAULT_TEST_CLASS(CFTLUuidsTester);
};
