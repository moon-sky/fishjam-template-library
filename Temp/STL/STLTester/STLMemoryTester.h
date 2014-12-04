#pragma once

#include <cppunit/extensions/HelperMacros.h>

/*********************************************************************************************************
* STL库中提供了智能指针auto_ptr，以解决“抛出异常时发生的内存泄漏”问题
* auto_ptr 在拷贝时，其所指向对象的所有权会转移，被拷贝对象的指针会被设为NULL。可通过 const auto_ptr 来禁止拷贝
* auto_ptr 拥有它所指向的对象指针。当auto_ptr被析构时，它所指向的对象也将被析构。
* 限制：
*   auto_ptr不能共享所有权。
*   auto_ptr不能作为容器的成员。 
*   auto_ptr不能指向数组。
*   不能通过赋值操作来初始化auto_ptr 。
* 
* std::tr1::shared_ptr -- 新版本中加入了SharePtr
*   智能指针的类型转换
*     std::tr1::static_pointer_cast
*     std::tr1::dynamic_pointer_cast
*
* 允许对this使用 shared_ptr -- 从 enable_share_from_this<> 继承 + shared_from_this();
*********************************************************************************************************/
class CSTLMemoryTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CSTLMemoryTester );
    CPPUNIT_TEST( test_auto_ptr );
	CPPUNIT_TEST( test_contain_assign );
	CPPUNIT_TEST( test_shared_ptr );
    CPPUNIT_TEST_SUITE_END();

    void test_auto_ptr();

    //测试容器对象直接赋值(=)、assign、copy 函数等三种方式，结果相同
    void test_contain_assign();
	void test_shared_ptr();

    DECLARE_DEFAULT_TEST_CLASS(CSTLMemoryTester);
};
