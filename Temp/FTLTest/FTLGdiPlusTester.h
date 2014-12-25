#pragma once

#include <cppunit/extensions/HelperMacros.h>
#include <ftlGdiPlus.h>

/************************************************************************
* 
************************************************************************/

class CFTLGdiPlusTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CFTLGdiPlusTester );
    CPPUNIT_TEST( test_Bitmap );

    CPPUNIT_TEST_SUITE_END();

    void test_Bitmap();

    DECLARE_DEFAULT_TEST_CLASS(CFTLGdiPlusTester);
private:
};
