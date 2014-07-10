#pragma once

#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* NLS -- National Language Support，国际化支持
************************************************************************/

class CWinNLSTester : public CPPUNIT_NS::TestFixture
{
public:
    CWinNLSTester(){}
    virtual ~CWinNLSTester(){}
    virtual void setUp();
    virtual void tearDown();


    CPPUNIT_TEST_SUITE( CWinNLSTester );

	CPPUNIT_TEST( test_GetDateFormat );
	CPPUNIT_TEST( test_GetNumberFormat );

    CPPUNIT_TEST_SUITE_END();

    void test_GetDateFormat();

	void test_GetNumberFormat();
    //DECLARE_DEFAULT_TEST_CLASS(CWinNLSTester);

private:
    SYSTEMTIME  m_systemtime;
};
