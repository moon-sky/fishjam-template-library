#pragma once

#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* deque -- 双向的queue?
* queue -- 先进先出的执的排列
* priority_queue -- 元素的次序是由作用于所存储的值对上的某种谓词决定的的一种队列
* 
* 当大部分插入和删除发生在序列的头或尾时可以选择deque这种数据结构
*
************************************************************************/

class CSTLQueueTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CSTLQueueTester );
    //CPPUNIT_TEST( test_constructor);

    CPPUNIT_TEST_SUITE_END();

    //void test_constructor();

    DECLARE_DEFAULT_TEST_CLASS(CSTLQueueTester);
};
