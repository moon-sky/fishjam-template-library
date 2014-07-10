#pragma once

#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* 
************************************************************************/

class CFTLPipelineTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CFTLPipelineTester );

    CPPUNIT_TEST( test_Pipeline );

    CPPUNIT_TEST_SUITE_END();

    void test_Pipeline();

    DECLARE_DEFAULT_TEST_CLASS(CFTLPipelineTester);
};
