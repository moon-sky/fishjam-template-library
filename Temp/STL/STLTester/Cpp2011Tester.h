#pragma once
#include <cppunit/extensions/HelperMacros.h>

/*****************************************************************************************************
* C++11(也叫C++0x,2011年出的新标准)-- 核心语言的领域将被大幅改善，包括多线程支持、 泛型编程、统一的初始化，以及表现的加强
*   运行期表现强化 -- 
*     右值引用(typename &&) -- 可以被接受为非const值，从而允许改变其值，语法为 typename &&；
*       当与模板变参相结合时，可以创建一些能自动调用具有相应参数构造器的工厂函数(用于支持 tuple )
*       std::move(xxx) -- 可以使得变量被当作右值。意义?
*     转移语义(转移构造器?) --
*     初始化列表构造器(std::initializer_list) -- 
*   建构期表现强化 -- 
*     允许构造函数调用其他构造(类似Java)，注意：任一构造函数都可能在已经完成构造的对象上自行，需要小心重复初始化？
*     外部模板(extern, 用于优化编译速度) -- 如: extern template class std::vector<MyClass>; 告诉编译器不要在这个编译单元实例化此模板
*     统一的初始化 -- 一种能作用于任何对象的完全统一的类型初始化形式, CMyClass myObj{ 每一个共有变量的值列表 }； //定义myObj变量，并初始化其共有变量
*       注意：统一的初始化形式不会取代构造器语法
*   可用性强化
*     std::initializer_list -- 初始化列表，允许构造器及其他函数接受初始化列表作为其参数
*     以范围为基础的 for 循环(for each) -- 
*   
* Lambda -- Lambda为C++提供lambda表达式及无名函数。在使用标准库算法时特别好用(如 sort 和 find 等)，
*   Lambda允许函数在呼叫点创建，避免了创建多个小的函数对象。Lambda函数是一些类型取决于编译器的函数对象。其类型只对编译器开放。
*   使用lambdas意味着更少的代码，在哪需要就在哪写，这比分散在代码各处的函数对象更清晰、更好维护。
*   特点：在算法函数调用的触发点附近定义谓词函数，
*   语法：[可选的闭包变量列表](参数列表)->返回值类型{ 实现体 } -- 如实现体不返回，则返回值类型可省略
*     
* 新的关键字
*   auto -- 编译时通过类型推定来创建与初始化器类型相同的变量。主要是为了解决 模板编程时很难推导出具体类型 的问题，并可减少代码冗余
*   constexpr -- 常量表达式，使用户能保证某个函数或构造器在编译期产生常量, 
*   decltype -- 在编译期根据其他的变量等确定某个表达式的类型
*   enum class -- 强类型枚举，使用时必须显式指定作用域，(标准C++中枚举就是整数)
*   nullptr -- 空指针，将常数0和指针分开，这样可以区分 foo(char*) 和 foo(int) 的调用
* 新的函数或类
*   std::reference_closure
* 新的特性
*   约束(concept/requires) -- 提供了一种将模板参数所必需满足的接口代码化的机制(最初的目的是改进模板编译错误信息的质量)
*   
*****************************************************************************************************/

//VS2010 以后的 VisualStudio 支持新的 2011 C++ 标准(c++0x)
#if _MSC_VER >= 1600  //VS2010
#	define SUPPORT_CPP2011	1
#else
#  define SUPPORT_CPP2011	0
#endif //_MSC_VER

class CCpp2011Tester : public CPPUNIT_NS::TestFixture
{

public:
	CPPUNIT_TEST_SUITE( CCpp2011Tester );
#if SUPPORT_CPP2011
	CPPUNIT_TEST( test_auto );
	CPPUNIT_TEST( test_constexpr );
	CPPUNIT_TEST( test_decltype );
	CPPUNIT_TEST( test_initializer_list );
    CPPUNIT_TEST( test_lambda );
    CPPUNIT_TEST( test_requires );
	CPPUNIT_TEST( test_right_reference );
	CPPUNIT_TEST( test_scope_for );
#endif  //SUPPORT_CPP2011
	CPPUNIT_TEST_SUITE_END();

	DECLARE_DEFAULT_TEST_CLASS(CCpp2011Tester);
private:
#if SUPPORT_CPP2011
	void test_auto();
	void test_constexpr();
	void test_decltype();

	void test_initializer_list();

    void test_lambda();

    //测试约束
    void test_requires();

	//测试右值引用 -- 英文是否正确?
	void test_right_reference();

	//以范围为基础的 for 循环
	void test_scope_for();
#endif  //SUPPORT_CPP2011
};
