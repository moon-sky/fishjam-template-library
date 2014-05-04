#pragma once

#include <cppunit/extensions/HelperMacros.h>

/****************************************************************************************
* deque -- (采用链表？)存储的指向不同元素的指针所组成的数组，允许快速的随机定位和在头尾插入和删除元素。
*   如果在中间删除元素，会造成元素内存拷贝。
****************************************************************************************/
class CSTLDequeTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CSTLDequeTester );
    CPPUNIT_TEST( test_deque_memory );
    CPPUNIT_TEST_SUITE_END();

    //测试deque使用内存的模式 -- 证明增加元素时不会出现内存拷贝的情况，但如果在中间删除元素
    //  会出现内存拷贝.
    void test_deque_memory();
    
    DECLARE_DEFAULT_TEST_CLASS(CSTLDequeTester);
};