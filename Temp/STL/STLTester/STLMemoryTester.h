#pragma once

#include <cppunit/extensions/HelperMacros.h>

/*********************************************************************************************************
* C++引用计数方案可以分为两类
*   侵入式(如 CComPtr) -- 要求资源对象本身维护引用计数，同时提供增减引用计数的管理接口
*   非侵入式(如 shared_ptr) -- 对资源对象本身没有任何要求,完全借助非侵入式引用计数智能指针在资源对象外部维护独立的引用计数
*    
* STL库中提供了智能指针auto_ptr，以解决“抛出异常时发生的内存泄漏”问题
* auto_ptr 在拷贝时，其所指向对象的所有权会转移，被拷贝对象的指针会被设为NULL。可通过 const auto_ptr 来禁止拷贝
* auto_ptr 拥有它所指向的对象指针。当auto_ptr被析构时，它所指向的对象也将被析构。
* 限制：
*   auto_ptr不能共享所有权。
*   auto_ptr不能作为容器的成员。 
*   auto_ptr不能指向数组。
*   不能通过赋值操作来初始化auto_ptr 。
* 
* std::tr1::shared_ptr -- 新版本中加入了SharePtr，引用计数管理接口是私有的，无法从shared_ptr之外操控
*   优点:
*     1.可以自定义 deleter/allocator 等
*   限制：
*     1.一旦开始对资源对象使用shared_ptr，就必须一直使用
*     2.必须使用最初的shared_ptr实例拷贝来维系资源对象生存期
*     3.如果资源对象的成员方法需要获取一个指向自己的shared_ptr，
*       需要从 enable_shared_from_this<CMyClass> 继承(★成为侵入式的了★) + shared_from_this() 方法返回指向自身的 shared_ptr
*   智能指针的类型转换
*     std::tr1::static_pointer_cast
*     std::tr1::dynamic_pointer_cast
*
* 允许对this使用 shared_ptr -- 
* TODO: weak_ptr
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
