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
    CPPUNIT_TEST_SUITE( CPocoDecoderEncoderTester );
    CPPUNIT_TEST( test_base64_decoder );
    CPPUNIT_TEST( test_base64_encoder );
    CPPUNIT_TEST_SUITE_END();

    DECLARE_DEFAULT_TEST_CLASS(CPocoDecoderEncoderTester);
private:
    void test_base64_decoder();
	void test_base64_encoder();
};
