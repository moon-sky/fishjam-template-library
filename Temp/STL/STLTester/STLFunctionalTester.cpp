#include "StdAfx.h"
#include "STLFunctionalTester.h"
#include <list>
#include <string>
//#include <functional> 这个例子测试的是 Algorithm ？
#include <algorithm>
#include <cctype>
#include "STLTesterUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TRIPLE_ARG(Operation, Type) Operation::Type
//使用3个参数的仿函数的函数原型 --可以作为STL的一种扩展(原理)
template<class Arg1, class Arg2, class Arg3, class Result>
struct triple_funcion
{
    //保证可配接性的类型定义
    typedef Arg1 first_argument_type;
    typedef Arg2 second_argument_type;
    typedef Arg3 third_argument_type;
    typedef Result result_type;
};

//三元函数的适配器，把第3个参数固定为特定值
template <class Operation>
class binder3rd : public std::binary_function<typename TRIPLE_ARG(Operation, first_argument_type),
    typename TRIPLE_ARG(Operation, second_argument_type), typename TRIPLE_ARG(Operation, result_type)>
{
protected:
    Operation m_op;
    typename Operation::third_argument_type value;
public:
    binder3rd(const Operation& x, const typename Operation::third_argument_type y):m_op(x), value(y){}
    //通过固定第三个参数，把函数转换为binary_function
    typename Operation::result_type operator()(typename const Operation::first_argument_type& x,
        typename const Operation::second_argument_type& y) const
    {
        return m_op(x, y, value);
    }
};

//上层使用的包装类
template<class Operation, class Arg>
inline binder3rd<Operation> bind3rd(const Operation& fn, const Arg& x)
{
    typedef Operation::third_argument_type third_argument_type;
    return binder3rd<Operation>(fn, third_argument_type(x));
}

char char_toupper(char c)
{
    return std::toupper(c);
}

std::string string_toupper(std::string& s)
{
    std::string strUpper(s);
    std::transform(strUpper.begin(), strUpper.end(), strUpper.begin(), char_toupper);
    return strUpper;
}

void CSTLFunctionalTester::UpperAndInsert2This(std::string str)
{
    std::string strUpper = string_toupper(str);
    this->m_UpperVec.push_back(strUpper);    
}

template<typename Fn>
bool CSTLFunctionalTester::HandleRecordByEvent(std::string eventName, Fn f)
{
    std::vector<std::string> vec;
    vec.push_back(eventName);
    
    std::for_each(vec.begin(),vec.end(),
        std::bind1st(mem_fun(f), this));
    return true;
}

void CSTLFunctionalTester::test_another_bind1st_this()
{
    CPPUNIT_ASSERT(m_UpperVec.size() == 0);
    HandleRecordByEvent("another_bind1st_this", &CSTLFunctionalTester::UpperAndInsert2This);
    CPPUNIT_ASSERT(m_UpperVec.size() == 1);
    CPPUNIT_ASSERT( (*m_UpperVec.begin()) == "ANOTHER_BIND1ST_THIS");
    m_UpperVec.clear();
}

void CSTLFunctionalTester::test_bind1st_this()
{
    CPPUNIT_ASSERT(m_UpperVec.size() == 0);
    std::vector<std::string> vec;
    vec.push_back("one");
    vec.push_back("two");
    vec.push_back("three");

    //或写为：std::for_each(vec.begin(),vec.end(),boost::bind(&CSTLFunctionalTester::UpperAndInsert2This,this,_1));
    std::for_each(vec.begin(),vec.end(),
        std::bind1st(mem_fun(&CSTLFunctionalTester::UpperAndInsert2This), this));

    CPPUNIT_ASSERT(m_UpperVec.size() == vec.size());
    CPPUNIT_ASSERT( (*m_UpperVec.begin()) == "ONE");
    CPPUNIT_ASSERT( (*m_UpperVec.rbegin()) == "THREE");
    m_UpperVec.clear();
}


void CSTLFunctionalTester::test_bind2nd()
{
    std::list<std::string> coll;
    coll.push_back("hello");
    coll.push_back("world");
    coll.push_back("StlTester");

    std::list<std::string>::iterator pos;

    //搜寻第一个大于"q"的元素   value > q
    pos = std::find_if(coll.begin(),coll.end(),std::bind2nd(std::greater<std::string>(),"q"));
    CPPUNIT_ASSERT(*pos == "world");
}

void CSTLFunctionalTester::test_compose1()
{
    //transform(first, last, first,
    //    compose1(negate<double>, ptr_fun(fabs)));

}

void CSTLFunctionalTester::test_equal_to()
{
    std::string strInfo = "dir1;dir2;dir3;dir4";
    
    //把字符串中的 分号(;)换成冒号(:)
    std::replace_if(strInfo.begin(), strInfo.end(), 
        std::bind2nd(std::equal_to<char>(), ';'), 
        ':');
    CPPUNIT_ASSERT( strInfo == "dir1:dir2:dir3:dir4");
}

void CSTLFunctionalTester::test_not()
{
    //生成 [1-10]
    std::vector<int> intVector(10);
    std::generate(intVector.begin(), intVector.end(), sequence_generator<int>(1,1));

    int count = count_if(intVector.begin(), intVector.end(), std::bind1st(std::less<int>(), 8));  //8 < value
    CPPUNIT_ASSERT(2 == count);   //9, 10

    count = count_if(intVector.begin(), intVector.end(), std::not1(std::bind1st(std::less<int>(), 8))); // !(8 < value) 等价于 8 >= value
    CPPUNIT_ASSERT(8 == count);   // 1 - 8

    count = count_if(intVector.begin(), intVector.end(), std::bind1st(std::greater_equal<int>(), 8));  // 8 >= value
    CPPUNIT_ASSERT(8 == count);
}

void CSTLFunctionalTester::test_logical_not()
{
    std::string strInfo = "this is \t logical_and demo";

    //const char* wptr = find_if(str, str + MAXLEN,
    //    compose2(not2(logical_or<bool>()),
    //    bind2nd(equal_to<char>(), ' '),
    //    bind2nd(equal_to<char>(), '\n')));
}

void CSTLFunctionalTester::test_negate()
{
    std::list<int> intSrc;
    // { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 } 
    std::generate_n(std::inserter(intSrc, intSrc.end()), 10, sequence_generator<int>(1,1) );

    std::list<int> intResult;
    std::transform(intSrc.begin(), intSrc.end(), std::inserter(intResult, intResult.end()), std::negate<int>());

    int intResultArray[] = {-1, -2, -3, -4, -5, -6, -7, -8, -9, -10};
    STLTESTER_ASSERT_CONTAINER_ELEMENT_EQUAL(intResultArray, ARRAY_PAST_THE_END(intResultArray),
        intResult.begin(), intResult.end());
}

void CSTLFunctionalTester::test_plus()
{
    //生成 [1-10]
    std::vector<int> intVector(10);
    std::generate(intVector.begin(), intVector.end(), sequence_generator<int>(1,1));
    //在每一个数字上加 1
    std::for_each(intVector.begin(), intVector.end(), std::bind2nd(std::plus<int>(), 1));

#pragma TODO(why cannot plus)
    //比较容器的值
    //int checkArray[] = {2,3,4,5,6,7,8,9,10,333};
    //STLTESTER_ASSERT_CONTAINER_ELEMENT_EQUAL(checkArray, ARRAY_PAST_THE_END(checkArray), intVector.begin(), intVector.end());
}

void CSTLFunctionalTester::test_ptr_fun()
{
    std::vector <char*> v1;
    std::vector <char*>::iterator Iter1, RIter;

    v1.push_back ( "Open" );
    v1.push_back ( "up" );
    v1.push_back ( "the" );
    v1.push_back ( "pearly" );
    v1.push_back ( "gates" );

    //使用全局函数 To search the sequence for "pearly" use a pointer_to_function conversion
    RIter = find_if( v1.begin( ), v1.end( ), std::not1 ( std::bind2nd (std::ptr_fun ( strcmp ), "pearly" ) ) );
    CPPUNIT_ASSERT(RIter != v1.end());
    CPPUNIT_ASSERT( *RIter == "pearly" );
}

unsigned int RandInt(const unsigned int n)
{
    return rand() % n;
}

void CSTLFunctionalTester::test_pointer_to_unary_function()
{
    int checkArray[] = {1,2,3,4,5,6,7,8,9,10};
    std::vector<int> intVector;
    std::generate_n( back_inserter(intVector), 10, sequence_generator<int>(1,1));
    CPPUNIT_ASSERT(10 == intVector.size());
    STLTESTER_ASSERT_CONTAINER_ELEMENT_EQUAL(checkArray, ARRAY_PAST_THE_END(checkArray),
        intVector.begin(), intVector.end());

    //使用STL的单目函数模板定义了一个变量ptr_RandInt，并将地址初始化到我们的函数RandInt()。单目函数接受一个参数，并返回一个值。
    std::pointer_to_unary_function<unsigned int, unsigned int> ptr_RandInt = std::ptr_fun(RandInt);  // Pointer to RandInt()//注意这行
    
    //两种都行，有什么区别？

    random_shuffle(intVector.begin(), intVector.end(), ptr_RandInt);
    random_shuffle(intVector.begin(), intVector.end(), RandInt);
    
    std::ostringstream os;
    std::copy(intVector.begin(), intVector.end(), std::ostream_iterator<int>(os, ","));
    const std::string& strInfo = os.str();

    TRACE("after random_shuffle ptr_RandInt: %s\n", strInfo.c_str());

}

struct Student
{
    Student(int math,int physics)
        :math(math),physics(physics){}
    int math;       //数学成绩
    int physics;    //物理成绩
};

//定义一个三元仿函数，用于查找数学和物理两科成绩符合条件的学生。
class Qualified : public triple_funcion<Student, int, int, bool>
{
public:
    bool operator()(const Student& s, int math, int physics) const
    {
        return s.math >= math && s.physics >= physics;
    }
};


void CSTLFunctionalTester::test_my_triple_funcion()
{
    std::vector<Student> students;
    students.push_back(Student(70,70));
    students.push_back(Student(90,80));
    students.push_back(Student(50,40));
    students.push_back(Student(65,55));
    //统计 数学>=60，物理>=60 的人数(即都几个的人数)
    std::vector<Student>::difference_type count = std::count_if(students.begin(),students.end(),
        std::bind2nd(bind3rd(Qualified(), 60), 60));
    CPPUNIT_ASSERT(count == 2);
}