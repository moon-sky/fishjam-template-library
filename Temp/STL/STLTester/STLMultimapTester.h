#pragma once

#include <cppunit/extensions/HelperMacros.h>

/*******************************************************************************************************
* multimap 可以支持多个相同的 key, 而每一个key可以映射到一个value,其内部仍然使用红黑树排序
* 通过 find 方法,如果查找到的话,会返回第一个的迭代器,否则返回 end
* 通过 equal_range 可以返回一个
*******************************************************************************************************/

class CSTLMultimapTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CSTLMultimapTester );
    CPPUNIT_TEST( test_multimap );
    CPPUNIT_TEST_SUITE_END();

    void test_multimap();

    DECLARE_DEFAULT_TEST_CLASS(CSTLMultimapTester);
};
