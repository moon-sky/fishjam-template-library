#pragma once

#include <cppunit/extensions/HelperMacros.h>

/*************************************************************************************************************
* 标准仿函数（比如less<T>）和仿函数适配器（比如not1、bind2nd）
*   仿函数(函数对象)，是一个重载了"()"函数运算符的struct或class，极大的拓展了算法的功能。
*   仿函数(通常)的调用方式：在算法处定义一个无名的对象 ( 如 less<int>() )
* 
* 为了最大限度的使得仿函数类可适配(仿函数能够与其它仿函数适配在一起实现新的功能)，
*    最好从 unary_functon 或 binary_function 继承（参见《Effective STL》40）
*    std::unary_function(带一个实参)， 通过 typedef 定义了 argument_type 和 result_type
*    std::binary_function(带两个实参)，通过 typedef 定义了 first_argument_type、second_argument_type、result_type
*       .
*   一般来说，通用函数也可以作为仿函数参数传递给算法，但其区别在于“通用函数不具有可适配性”。
*
* STL里的一个普遍习惯：函数和函数对象总使用用于非成员函数的语法形式调用（如全局函数或静态成员函数，否则不能编译），
* STL提供了把 通用函数、类成员函数 转换为仿函数的功能，在转换成员函数时，有引用和指针两个版本：
*   mem_fun -- 通过对象指针调用成员函数，可用于多态的虚函数，只能生成不带参数，或者是仅带一个参数的函数对象
*     boost进行了扩展，能对任意数量参数的成员函数生成functor(boost::bind和boost::mem_fn)
*     for_each(lpw.begin(), lpw.end(),mem_fun(&Shape::draw));  //list<Shape*> lpw;draw 为无参的，如果有参的？
*   mem_fun_ref -- 通过对象调用成员函数 
*     for_each(lw.begin(), lw.end(),mem_fun_ref(&Shape::test)); //list<Shape> lw; 
*   ptr_fun -- 通过对象调用普通函数（加不加不影响性能和是否可编译）
*     pointer_to_binary_function/pointer_to_unary_function -- 为通用函数提供适配功能，但很少直接使用(应该用 ptr_fun )。
*     在仿函数对象构造时把通用函数作为参数传入。
*   
* 函数对象适配器（绑定器函数对象）： not1、not2、bind1st和bind2nd ，都需要存在某些typedef ，如果没有时，使用 ptr_fun
*   一个绑定器 将一个函数对象和一个参数捆绑到为另一个函数对象，并将该对象作为算法的参数进行调用(算法要求只有一个参数)
*   bind1st和bind2nd函数用于将一个二元算子（binary functor，bf）转换成一元算子，可以提供额外的参数
*   bind1st 表示参数是第一个操作数，bind2nd 表示参数是第二个操作数
*     v.erase(remove_if( v.begin(), v.end(),bind1st(less<int>(), 100)),v.end()); //移除所有大于100的元素 100 < v.value
*     v.erase(remove_if( v.begin(), v.end(),bind2nd(less<int>(), 100)),v.end()); //移除所有小于100的元素 v.value < 100
*   not1否定单参数的函数对象，not2否定双参数的函数对象
*   not2(ptr_fun(ciCharCompare))); //int ciCharCompare(char c1, char c2)	
*
*
*************************************************************************************************************/
class CSTLFunctionalTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CSTLFunctionalTester );
    CPPUNIT_TEST ( test_bind1st_this );
    CPPUNIT_TEST ( test_another_bind1st_this );
    CPPUNIT_TEST ( test_bind2nd );
    CPPUNIT_TEST ( test_compose1 );
    CPPUNIT_TEST ( test_equal_to ); //equal_to,notequal_to,greater,greater_equal,less_equal,
    CPPUNIT_TEST ( test_not );      
    CPPUNIT_TEST ( test_logical_not );
    CPPUNIT_TEST ( test_negate );
    CPPUNIT_TEST ( test_plus );  //类似的有 minus(减),multiples(乘),divieds(除),modulus(模?)
    CPPUNIT_TEST ( test_ptr_fun );
    CPPUNIT_TEST ( test_pointer_to_unary_function );
    CPPUNIT_TEST ( test_my_triple_funcion );
    CPPUNIT_TEST_SUITE_END();

    //通过for_each使用成员函数来遍历一个容器，并且更新this中的变量
    void test_bind1st_this();
    void test_another_bind1st_this(); //另外一种方法

    void test_bind2nd();

    void test_compose1();

    //! 使用 operator == 判断两个元素是否相等, 对应的还有 notequal_to
    void test_equal_to();

    //否定函数对象： not1 和 not2
    void test_not();

    //对操作数逻辑取反
    void test_logical_not();   // 对应的还有 logical_or, test_logical_and

    //对操作数值取反
    void test_negate();

    void test_plus();
    void test_ptr_fun();

    //供双参函数使用的adapter
    void test_pointer_to_unary_function();

    //测试自定义的一个三元运算符的适配器
    void test_my_triple_funcion();

    DECLARE_DEFAULT_TEST_CLASS(CSTLFunctionalTester);
private:
    std::vector<std::string>    m_UpperVec;
    
    template <typename Fn>
    bool HandleRecordByEvent(std::string eventName, Fn f);
    void UpperAndInsert2This(std::string str);
};
