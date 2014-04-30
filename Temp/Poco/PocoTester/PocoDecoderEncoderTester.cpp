#include "StdAfx.h"
#include "PocoDecoderEncoderTester.h"
#include <poco/Base64Decoder.h>
#include <Poco/Base64Encoder.h>
#include <Poco/Net/QuotedPrintableDecoder.h>
#include <Poco/Net/QuotedPrintableEncoder.h>
#include <Poco/UnicodeConverter.h>
#include <ftlConversion.h>

CPocoDecoderEncoderTester::CPocoDecoderEncoderTester()
{
    m_strUnicodeOriginal = L"some Info:!@#$%^&*()-=_+[]中文";
    m_strUtf8Original = FTL::CFConversion().UTF16_TO_UTF8(m_strUnicodeOriginal.c_str());
    //注意：Poco中所有的字符串处理都必须以 UTF8 为基础
    
    m_strBase64String = "c29tZSBJbmZvOiFAIyQlXiYqKCktPV8rW13kuK3mloc=";
    m_strQuotedPrintableString = "some Info:!@#$%^&*()-=3D_+[]=E4=B8=AD=E6=96=87";
}

void CPocoDecoderEncoderTester::test_Base64Decoder()
{
	std::istringstream	in(m_strBase64String);
	std::ostringstream	out;
	Poco::Base64Decoder	b64in(in);

	std::copy(	std::istreambuf_iterator<char>(b64in),
		std::istreambuf_iterator<char>(),
		std::ostreambuf_iterator<char>(out));
	
    std::string base64DecodedString = out.str();
    CPPUNIT_ASSERT_EQUAL(m_strUtf8Original, base64DecodedString);
}

void CPocoDecoderEncoderTester::test_Base64Encoder()
{
	std::istringstream in(m_strUtf8Original);
	std::ostringstream out;
	Poco::Base64Encoder b64out(out);

	std::copy(std::istreambuf_iterator<char>(in),
		std::istreambuf_iterator<char>(),
		std::ostreambuf_iterator<char>(b64out));
	b64out.close();

    std::string base64EncoderString = out.str();
    CPPUNIT_ASSERT_EQUAL(m_strBase64String, base64EncoderString);
}

void CPocoDecoderEncoderTester::test_QuotedPrintableDecoder()
{

}

void CPocoDecoderEncoderTester::test_QuotedPrintableEncoder()
{
    std::istringstream in(m_strUtf8Original);
    std::ostringstream out;
    Poco::Net::QuotedPrintableEncoder encoder(out);

    std::copy(std::istreambuf_iterator<char>(in),
        std::istreambuf_iterator<char>(),
        std::ostreambuf_iterator<char>(encoder));
    encoder.close();

    std::string quotedPrintableString = out.str();
    CPPUNIT_ASSERT_EQUAL(m_strQuotedPrintableString, quotedPrintableString);
}