#include "StdAfx.h"
#include "PocoDecoderEncoderTester.h"

void CPocoDecoderEncoderTester::test_base64_decoder()
{
	/*
	std::istringstream	in(base64String);
	std::ostringstream	out;
	Poco::Base64Decoder	b64in(in);

	std::copy(	std::istreambuf_iterator<char>(b64in),
		std::istreambuf_iterator<char>(),
		std::ostreambuf_iterator<char>(out));
	
	decodedString = out.str();
	*/
}

void CPocoDecoderEncoderTester::test_base64_encoder()
{
	std::istringstream in("some original string");
	std::ostringstream out;
	Poco::Base64Encoder b64out(out);

	std::copy(std::istreambuf_iterator<char>(in),
		std::istreambuf_iterator<char>(),
		std::ostreambuf_iterator<char>(b64out));
	b64out.close();
}