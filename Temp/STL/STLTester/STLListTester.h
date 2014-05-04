#pragma once

#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* list -- 由节点组成的双向链表，每个结点包含着一个元素, list容器仅提供双向的 iterator
* 
* 当很频繁地对序列中部进行插入和删除时应该用list(双向列表), slist(单向列表)
* list的成员函数
*   push_back()把一个对象放到一个list的后面，
*   push_front()把对象放到前面  -- 可用于设置特殊的顺序
*   end() 返回指向容器的最后一个位置的iterator -- ★有效位置的后面★
*   sort() -- 排序一个list，有两种方法：使用通用的 sort 算法和 list 中的成员函数sort -- 提高性能
*     通用算法仅能为那些提供随机存取里面元素 的容器排序，而由于list是作为一个连接的链表实现的，
*     它不支持对它里面的元素随机存取。所以就需要一个特殊的 sort()成员函数来排序list
*   insert -- 把对象插入到list中的任何地方,可以加入一个对象，一个对象的若干份拷贝，或者一个范围以内的对象
*   pop_front()删掉list中的第一个元素
*   pop_back()删掉最后一个元素
*   erase()删掉由一个iterator指出的元素或指定一个范围的元素 -- 由迭代器指出删除
*   remove 删除元素 -- 根据指定的元素内容
*   splice 在源list中删除元素，并放到目的list中 -- 经过测试，不会再调用移动元素的拷贝构造(直接移过去，保证了效率)
************************************************************************/

class CSTLListTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CSTLListTester );

    CPPUNIT_TEST( test_constructor);
    CPPUNIT_TEST( testMyTestDataList );
    CPPUNIT_TEST( test_sort );
    CPPUNIT_TEST( test_splice );
    CPPUNIT_TEST( test_unique );
    CPPUNIT_TEST( test_remove );
    CPPUNIT_TEST_SUITE_END();

    void test_constructor();
    void testMyTestDataList();
    void test_sort();

    //目的.splice( 目的的插入位置, 源,[源的起始，源的结束])
    void test_splice();

    //真的删除邻近的重复值 -- 改变大小
    void test_unique();

    //真的删除被remove的值 -- 改变大小
    void test_remove();

    DECLARE_DEFAULT_TEST_CLASS(CSTLListTester);
};
