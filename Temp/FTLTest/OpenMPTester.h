#pragma once

#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* OpenMP
************************************************************************/

class COpenMPTester : public CPPUNIT_NS::TestFixture
{
public:
    COpenMPTester();
    virtual ~COpenMPTester();

    CPPUNIT_TEST_SUITE( COpenMPTester );

	CPPUNIT_TEST( test_parallel );
    CPPUNIT_TEST( test_parallel_for );
    CPPUNIT_TEST( test_parallel_cancel );

    CPPUNIT_TEST( test_variable );

    CPPUNIT_TEST_SUITE_END();

    void test_parallel();
	void test_parallel_for();
    void test_parallel_cancel();

    void test_variable();

    //DECLARE_DEFAULT_TEST_CLASS(COpenMPTester);

private:
    int m_nOpenMPMaxThreadCount;
    int m_nProcessorsCount;
};
