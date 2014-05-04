#pragma once

#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* 包括了贯穿使用在STL中的几个模板的声明
* 
************************************************************************/

class CSTLUtilityTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CSTLUtilityTester );
    CPPUNIT_TEST( test_advance );
    CPPUNIT_TEST( test_distance );
    CPPUNIT_TEST( test_delete );
    CPPUNIT_TEST( test_boost_delete );
    CPPUNIT_TEST( test_StringPtrSet );
    CPPUNIT_TEST( test_swap );
    CPPUNIT_TEST( test_BPFC );
    CPPUNIT_TEST_SUITE_END();

    //用distance和advance把const_iterator转化成iterator -- 见 Effective STL 条款27
    //  advance -- 按指定的数目增减迭代器，对于前推迭代器，该值必须为正，而对于双向迭代器和随机访问迭代器，该值可以为负
    void test_advance();

    //返回到达一个迭代器所需（递增）操作的数目
    void test_distance();


    //使用 for_each 删除容器中new出来的对象指针--防止内存泄漏的同时让代码安全、易读 -- 但需要注意删除的对象有 virtual 析构函数
    void test_delete();
    void test_boost_delete();
    void test_StringPtrSet();

    //使用“交换技巧”来修整过剩容量 -- Effective STL 规则17
    void test_swap();

    //测试使用大数据，多态的仿函数类
    void test_BPFC();
    DECLARE_DEFAULT_TEST_CLASS(CSTLUtilityTester);
};