#pragma once

#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* 
************************************************************************/

class CFTLNlsTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CFTLNlsTester );
    
	CPPUNIT_TEST( test_ATLBase64 );
	CPPUNIT_TEST( test_ATLBase64_ForGuid );
    CPPUNIT_TEST( test_CFConversion );
	CPPUNIT_TEST( test_MultiByteToWideChar );
	CPPUNIT_TEST( test_WideCharToMultiByte );

    CPPUNIT_TEST_SUITE_END();

	void test_ATLBase64();
    void test_ATLBase64_ForGuid();
    void test_CFConversion();
    void test_MultiByteToWideChar();
	void test_WideCharToMultiByte();

    DECLARE_DEFAULT_TEST_CLASS(CFTLNlsTester);
};
