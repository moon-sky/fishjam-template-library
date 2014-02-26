#pragma once

#include <cppunit/extensions/HelperMacros.h>

/**********************************************************************************************
* ”–∂‡÷÷ Converter£∫
*   LineEndingConverter --
*   StreamConverter --
*   TextConverter -- 
*   UnicodeConverter -- UTF8(std::strings) <==> UTF16(std::wstrings)
**********************************************************************************************/
#include "Poco/Crypto/Crypto.h"

class CPocoConverterTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CPocoConverterTester );
    CPPUNIT_TEST( test_container_holder );
    CPPUNIT_TEST_SUITE_END();

    DECLARE_DEFAULT_TEST_CLASS(CPocoConverterTester);
private:
    void test_container_holder();
};
