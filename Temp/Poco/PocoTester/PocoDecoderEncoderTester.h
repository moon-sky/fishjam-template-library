#pragma once

#include <cppunit/extensions/HelperMacros.h>

/**********************************************************************************************
* ”–∂‡÷÷ Decoder/Encoder£∫
*   Base64Decoder -- 
*   HexBinaryDecoder
*   QuotedPrintableDecoder -- 
**********************************************************************************************/
//#include "Poco/Base64Decoder.h"

class CPocoDecoderEncoderTester : public CPPUNIT_NS::TestFixture
{
public:
    CPocoDecoderEncoderTester();
public:
    CPPUNIT_TEST_SUITE( CPocoDecoderEncoderTester );
    CPPUNIT_TEST( test_Base64Decoder );
    CPPUNIT_TEST( test_Base64Encoder );
    CPPUNIT_TEST( test_QuotedPrintableDecoder );
    CPPUNIT_TEST( test_QuotedPrintableEncoder );
    CPPUNIT_TEST_SUITE_END();

    //DECLARE_DEFAULT_TEST_CLASS(CPocoDecoderEncoderTester);
private:
    void test_Base64Decoder();
	void test_Base64Encoder();
    void test_QuotedPrintableDecoder();
    void test_QuotedPrintableEncoder();

private:
    std::wstring m_strUnicodeOriginal;
    std::string m_strUtf8Original;
    std::string m_strBase64String;

    std::string m_strQuotedPrintableString;
};
