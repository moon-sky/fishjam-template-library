#pragma once

#include <cppunit/extensions/HelperMacros.h>
/************************************************************************
* array -- STL新增容器类，用于存储一个固定大小的数组。数组大小与数据元素类型一同由模版参数指出。
*   其指定的大小数值必须是一个常量运行时的 -- ★不能★增加或减少容器大小
*   引入的目的是 为了和其它STL容器进行无缝整合 -- 将 vector/list 等替换成普通数组时，可以访问 begin/end 等方法
************************************************************************/
class CSTLArrayTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CSTLArrayTester );
    CPPUNIT_TEST( test_array );
    CPPUNIT_TEST_SUITE_END();

	void test_array();

    DECLARE_DEFAULT_TEST_CLASS(CSTLArrayTester);
};