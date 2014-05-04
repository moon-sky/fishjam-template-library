#pragma once

#include <cppunit/extensions/HelperMacros.h>

/*******************************************************************************************************
* set -- 由节点组成的红黑树，每个节点都包含着一个元素，节点之间以某种作用于元素对的谓词排列，
*        没有两个不同的元素能够拥有相同的次序
*        set按照Key进行排序存储 -- 即枚举时默认会以 less<key> 的方式输出，可以自定义排序的依据(必须是 
*          operator < 操作 -- 不能有等于，否则在 find 等操作时会抛出异常，
*          如果把 test_set_find 中 less_CMyTestData_Pointer 的实现改为 <= 即可发现异常)
* multiset -- 允许存在两个次序相等的元素的集合
*******************************************************************************************************/
class CSTLSetTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CSTLSetTester );
    CPPUNIT_TEST( test_set_order );
    CPPUNIT_TEST( test_set_find );
    CPPUNIT_TEST( test_set_insert );
    CPPUNIT_TEST_SUITE_END();

    //测试set按照顺序存储
    void test_set_order();
    
    //测试自定义的排序依据和查找
    void test_set_find();

    //测试插入相同的数据
    void test_set_insert();
    DECLARE_DEFAULT_TEST_CLASS(CSTLSetTester);
};
