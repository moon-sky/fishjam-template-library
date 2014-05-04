#pragma once

#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* 迭代器的类型 -- 可以将迭代器认为是依次的继承关系，下面的继承自上面
*   输入迭代器(Input Iterator) <== 只读
*   输出迭代器(Output Iterator) <== 只写，通常用于将数据从一个位置拷贝到另一个位置
*   前向迭代器(Forward Iterator) <== ++ (向前推进迭代器),读写操作
*   双向迭代器(Bidirectional Interator) <== ++、--(能向前和向后操作)
*   随机存取迭代器() <== +n、-n、[]
*   
* 使用各种类型的迭代器 -- <iterator>( 注意：唯美主义，不一定合适)。如
* //Sample 代码： 从命令行输入数字，排序后重新输出。
*   typedef vector<int> int_vector;
*   typedef istream_iterator<int> istream_itr;
*   typedef ostream_iterator<int> ostream_itr;
*   typedef back_insert_iterator< int_vector > back_ins_itr;
*  
*   // 从标准输入设备读入整数，直到输入的是非整型数据为止
*   // istream_itr是适配器(adaptor)，对 cin 进行包装，产生迭代器的临时对象 -- 算法只认迭代器
*   // 注意：不能写成 copy(istream_iterator<int>(cin), istream_iterator<int>(), num.begin()); 
*   //   原因是：数组越界 -- 此时 copy 不会自动增加容量。需要使用适配器 back_insert_iterator
*   copy(istream_itr(cin), istream_itr(), back_ins_itr(num));  //其中 int_vector num;
*   
*   // 排序
*   sort(num.begin(), num.end());
*   
*   // 将排序结果输出到标准输出设备
*   // ostream_iterator(cout, "\n") -- 产生一个处理输出数据流的迭待器对象，其位置指向数据流的起始处，并且以"\n"作为分割符
*   copy(num.begin(), num.end(), ostream_itr(cout, "\n"));
*    
* 迭代器失效
*   当使用一个容器的insert或者erase函数通过迭代器插入或删除元素“可能”会导致迭代器失效(win下断言，linux下不确定)
*   一般需要获取insert或者erase返回的迭代器，以便用重新获取新的有效的迭代器进行正确的操作
************************************************************************/

class CSTLIteratorTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CSTLIteratorTester );
    CPPUNIT_TEST( test_deque_loopdelete );
    CPPUNIT_TEST( test_vector_loopdelete );
    CPPUNIT_TEST( test_list_loopdelete );
    CPPUNIT_TEST( test_set_loopdelete );
    CPPUNIT_TEST( test_map_loopdelete );

    CPPUNIT_TEST( test_bad_loopdelete );
    CPPUNIT_TEST( test_better_loopdelete );

    CPPUNIT_TEST( test_ostream_iterator );
    CPPUNIT_TEST( test_front_inserter );
    CPPUNIT_TEST_SUITE_END();

    //insert/erase后迭代器的更新有几种方式：
    //  1.顺序式容器，使用 insert/erase 的返回值更新迭代器 -- iter = intCon.erase(iter)
    //    Win下所有的容器都可以使用该方式，但Linux下vector/deque/list才可用
    //  2.关联容器，后置递增迭代器 -- intCon.erase(iter++);
    //    set/map  可以 使用
    //
    //注意：一般不使用for循环 -- 每次都会进行++，在删除后会又进行++的话会跳过一个元素
    //for (IntContainer::iterator iter = intCon.begin();
    //    iter != intCon.end(); ++iter)

    void test_deque_loopdelete();
    void test_vector_loopdelete();
    void test_list_loopdelete();
    void test_set_loopdelete();
    void test_map_loopdelete();

    void test_bad_loopdelete();
    //更好的删除方法
    void test_better_loopdelete();

    void test_ostream_iterator();

    // front_inserter -- 将对象插入到数据集的前面
    // back_inserter -- 将对象插入到集合的尾部
    // inserter -- 插入到容器任何对象的前面，可用于 remove_copy_if
    // 注意：使用插入迭代器可能导致容器中的其他对象移动位置，因而使得现存的迭代器非法 -- 插入到vector中将导致动态扩展
    //      一般来说，插入到链表(list)类型的结构中更为有效
    void test_front_inserter();
    DECLARE_DEFAULT_TEST_CLASS(CSTLIteratorTester);

    typedef int TestType;
};
