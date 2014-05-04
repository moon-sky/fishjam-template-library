#pragma once

#include <cppunit/extensions/HelperMacros.h>

/**********************************************************************************************
**********************************************************************************************/

class CAtlEncTester : public CPPUNIT_NS::TestFixture
{
public:
    CAtlEncTester();

public:
    CPPUNIT_TEST_SUITE( CAtlEncTester );
    CPPUNIT_TEST( test_AtlCanonicalizeUrl );
    CPPUNIT_TEST( test_AtlCombineUrl );
    CPPUNIT_TEST( test_AtlEscapeUrl );
    CPPUNIT_TEST( test_AtlHexEncode );
    CPPUNIT_TEST( test_AtlUnicodeToUTF8 );
    CPPUNIT_TEST( test_Base64Encode );
    CPPUNIT_TEST( test_BEncode );
    CPPUNIT_TEST( test_QEncode );
    CPPUNIT_TEST( test_QPEncode );
    CPPUNIT_TEST( test_UUDecode );
    CPPUNIT_TEST( test_UrlEscape );
    CPPUNIT_TEST_SUITE_END();

    //DECLARE_DEFAULT_TEST_CLASS(CAtlEncTester);
private:
    void test_AtlCanonicalizeUrl();
    void test_AtlCombineUrl();
    void test_AtlEscapeUrl();
    void test_AtlHexEncode();
    void test_AtlUnicodeToUTF8();
    void test_Base64Encode();
    void test_BEncode();
    void test_QEncode();
    void test_QPEncode();
    void test_UUDecode();
    void test_UrlEscape();
private:
    CAtlString m_strOriginalURL;
    CAtlString m_strUrlEscaped;
};
