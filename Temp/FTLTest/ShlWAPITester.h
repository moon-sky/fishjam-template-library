#pragma once

#include <cppunit/extensions/HelperMacros.h>

/*******************************************************************************************************
* 注意：以下测试都是在中文系统上
*   StrFromTimeInterval -- 将毫秒数转换为和OS相关的时间字符串，注意字符前面会有一个空格，
*     感觉由于要设置有效位数，实际上转换效果不好（有效位数应该设置成多少才合适 -- 有没有自适应的？）
*******************************************************************************************************/

class CShlWAPITester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CShlWAPITester );

    CPPUNIT_TEST( test_StrFormatByteSizeXX );
    CPPUNIT_TEST( test_StrFromTimeInterval );

    CPPUNIT_TEST_SUITE_END();

    void test_StrFormatByteSizeXX();
    void test_StrFromTimeInterval();

    DECLARE_DEFAULT_TEST_CLASS(CShlWAPITester);
};
