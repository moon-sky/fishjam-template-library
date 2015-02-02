#pragma once

#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* 
************************************************************************/

class CFTLNetTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CFTLNetTester );
    
    CPPUNIT_TEST( test_FNetServerT );
    CPPUNIT_TEST( test_FSocketAddress );
    CPPUNIT_TEST( test_GetNetInfo );
    CPPUNIT_TEST( test_IpV6 );
    CPPUNIT_TEST( test_UrlEscape );
    CPPUNIT_TEST( test_WinSocket );
    CPPUNIT_TEST( test_UrlMonFunctions );

    CPPUNIT_TEST_SUITE_END();

    void test_FNetServerT();
    void test_FSocketAddress();
    void test_GetNetInfo();
    void test_IpV6();
    void test_UrlEscape();
    void test_WinSocket();

    void test_UrlMonFunctions();

    DECLARE_DEFAULT_TEST_CLASS(CFTLNetTester);
};
