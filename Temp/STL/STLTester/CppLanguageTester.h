#pragma once
#include <cppunit/extensions/HelperMacros.h>

/**********************************************************************************************
* 测试Cpp语言在使用时的一些注意事项
*   C语言为支持printf等变参函数，需要自左至右入栈，且由调用者出栈，即最后要调用 add esp N 来调整 esp -- 调用方才知道传了多少参数
*      可变长参数(...)的缺点是：缺乏类型安全性，参数数量未知等。可考虑使用 boost::format 替换
*   C++(Pascal)：有被调用函数出栈(ret)，节省了 add esp N 的调用（时间、空间都节省了），但无法实现可变参函数的调用
*   
* 私有继承意味着用XX来实现（利用其中存在的代码），应尽可能换成组合（除非要改写虚函数，访问保护成员等）
* 要想获得多态性，需要通过指针或引用的方式，传值方式无法获得多态 -- 切割问题。
*   通常将基类的拷贝构造函数设为保护，并增加一个存虚拟的clone函数(virtual MyClass* clone() const = 0 {return new MyClass(*this);}
* 总是按成员变量声明的顺序编写成员初始化语句，尽量不要让一个成员的初始化依赖于其他成员更好。
* 避免在构造、析构中直接、间接调用虚拟函数 -- 使用后构造：1说明(如 Init);2在任一成员函数中判断并调用;3用工厂函数
*
* POD(Plain Old Data) -- 标准C++语言中，满足一些特定规则的结构体将产生兼容于C的对象布局，可以按照C的方式来处理（运算、拷贝等）。
*
* 泛型设置参数
*   SetParameter(var_list argptr)
*   {
*      INT myVar = va_arg(argptr, INT); //取出整形参数;
*      BOOL bVar = va_arg(argptr, BOOL); //
*   }
*  
* XXX* operator[](int nIndex) -- 类中定义根据下标返回指定元素的方法
**********************************************************************************************/
#pragma TODO(va_arg test)

class CCppLanguageTester : public CPPUNIT_NS::TestFixture
{
public:
    CCppLanguageTester();
    ~CCppLanguageTester();

    CPPUNIT_TEST_SUITE( CCppLanguageTester );
    CPPUNIT_TEST( test_bad_struct_pointer_assign );
    CPPUNIT_TEST( test_construct_init_list );
    CPPUNIT_TEST( test_exception_lifecycle );
    CPPUNIT_TEST( test_good_struct_pointer_assign );
    CPPUNIT_TEST( test_good_struct_object_assign );
    CPPUNIT_TEST( test_cpp_alloc_fail );
    CPPUNIT_TEST( test_cpp_alloc_physical_memory );
    CPPUNIT_TEST( test_anonymous_temp_objcet_Life );
    CPPUNIT_TEST( test_static_var_life );
    CPPUNIT_TEST( test_enum );
    CPPUNIT_TEST_SUITE_END();

    void test_bad_struct_pointer_assign();
    
    //测试构造函数中使用初始化列表和直接赋值得区别
    void test_construct_init_list();

    //测试异常的生命周期 -- 建议：通过值抛出异常，通过非常量的引用捕获异常（如果用值捕获，将在捕获处导致切割问题）
    //  未捕获得异常发生时，回依次调用 terminate->abort，可通过 set_unexpected 和 set_terminate 进行处理
    void test_exception_lifecycle();

    void test_good_struct_pointer_assign();

    void test_good_struct_object_assign();

    //VC2008 下测试失败！！！
    //C++ new失败后默认情况下是抛出 std::bad_alloc 异常，可以通过 new(std::nothrow)，使其返回NULL
    //  gcc 中的 -fno-exceptions 可以统一处理成不抛出异常（注意：可能造成不能抛出其他异常）
    //  注意：微软的VC中 new 失败不会抛出 std::bad_alloc 异常，http://support.microsoft.com/default.aspx?scid=kb;EN-US;q167733
    void test_cpp_alloc_fail();

    // 测试new以后物理内存
    void test_cpp_alloc_physical_memory();

    //测试隐式临时对象的生命周期：
    //  VC下立即删除（和定义局部变量不同，可能造成范围锁的Bug）
    //  FORTEC++在退出函数范围时删除（和定义局部变量相同）
    void test_anonymous_temp_objcet_Life();

    //测试静态变量的生存期
    void test_static_var_life();
    //DECLARE_DEFAULT_TEST_CLASS(CCppLanguageTester);

    void test_enum();
private:
    BOOL m_bCallGetMyTestDataInstance;
};