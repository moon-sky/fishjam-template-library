#pragma once

#include <cppunit/extensions/HelperMacros.h>

/**********************************************************************************************************
* 包括几个在序列上面进行简单数学运算的模板函数，包括加法和乘法在序列上的一些操作
**********************************************************************************************************/
class CSTLNumericTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CSTLNumericTester );
    CPPUNIT_TEST( test_accumulate);

    CPPUNIT_TEST_SUITE_END();

    //区间统计：统计指定区间（和 或 指定函数的操作结果）中所有值的总和 -- 也可以使用 for_each 自己统计
    //  注意：传给accumulate的函数中不能有副作用
    //        这样的值不一定是简单的类型，通过重载operator+()，也可以是类对象
    void test_accumulate();

    void test_adjacent_difference();
    void test_inner_product();
    void test_partial_sum();

    DECLARE_DEFAULT_TEST_CLASS(CSTLNumericTester);
};
