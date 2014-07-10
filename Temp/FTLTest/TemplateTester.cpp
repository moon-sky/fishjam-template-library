#include "StdAfx.h"
#include "TemplateTester.h"
#include <algorithm>

enum callType
{
    ctUnknownFun,
    ctNoneTemplateFunInt,
    ctTemplateFun2Pointer,
    ctTemplateFun2ConstChar,
    ctTemplateFun2,
    ctTemplateFun3
};

callType    g_CallType = ctUnknownFun;

//传回两个int中的较大者
int const& myMax(int const& a, int const& b)
{
    g_CallType = ctNoneTemplateFunInt;
    return a < b ? b : a;
}

//传回两个指针所指对象的较大值
template <typename T>
T* const& myMax(T* const& a, T* const& b)
{
    g_CallType = ctTemplateFun2Pointer;
    return *a < *b ? b : a;
}

const char* myMax(const char*& a, const char*& b)
{
    g_CallType = ctTemplateFun2ConstChar;
    return std::strcmp(a,b) < 0 ? b : a;
}
//传回两个任意类型的数值中的较大者
template <typename T>
T const& myMax(T const& a, T const& b)
{
    g_CallType = ctTemplateFun2;
    return a < b ? b : a;
}

//传回三个任意类型的数值中的最大者
template <typename T>
const T& myMax(const T &a, const T &b, const T &c) 
{
    g_CallType = ctTemplateFun3;  
    //return ::myMax(::myMax(a,b), c);//注意：由于为了设置 g_CallType，因此不能使用调用2个参数的 myMax 重载
    return (a < b) ? ( b < c ? c : b) : ( a < c ? c : a);
}

void CTemplateTester::test_fun_template_overload()
{
    myMax(7,42,68);
    CPPUNIT_ASSERT(g_CallType == ctTemplateFun3);

    myMax(7.0,42.0);
    CPPUNIT_ASSERT(g_CallType == ctTemplateFun2);  //myMax<double>

    myMax('a','b');
    CPPUNIT_ASSERT(g_CallType == ctTemplateFun2);  //myMax<char>

    myMax(7,42);
    CPPUNIT_ASSERT(g_CallType == ctNoneTemplateFunInt);  //myMax(int)，其他条件都相同时，优先选择非模板函数

    myMax<>(7,42);          
    CPPUNIT_ASSERT(g_CallType == ctTemplateFun2);    //max<int>   

    myMax<double>(7,42);
    CPPUNIT_ASSERT(g_CallType == ctTemplateFun2);   //max<douoble> -- 不需要参数推导

#pragma warning(disable : 4244)
    myMax('a',42.7); //会有一个警告：C4244: “参数”: 从“double”转换到“const int”，可能丢失数据
#pragma warning(default : 4244)
    CPPUNIT_ASSERT(g_CallType == ctNoneTemplateFunInt); //注意：模板中不考虑自动类型转换，因此会调用“接受两个int参数”的非模板函数

    int a = 7;
    int b = 42;
    myMax(a,b);
    CPPUNIT_ASSERT(g_CallType == ctNoneTemplateFunInt);

    int *pa = &a;
    int *pb = &b;
    myMax(pa,pb);
    CPPUNIT_ASSERT(g_CallType == ctTemplateFun2Pointer);

    std::string s1 = "hello";
    std::string s2 = " world!";
    myMax(s1,s2);
    CPPUNIT_ASSERT(g_CallType == ctTemplateFun2);  //myMax<std::string>

    const char* cc1 = "hello";
    const char* cc2 = " world!";
    myMax(cc1,cc2);
    CPPUNIT_ASSERT(g_CallType == ctTemplateFun2ConstChar); //myMax(const char*,const char*)

    char* c1 = "hello";
    char* c2 = " world!";
    myMax(c1,c2);
    CPPUNIT_ASSERT(g_CallType == ctTemplateFun2Pointer); //myMax<char*>
}

class SortTracer
{
private:
    int m_value;            //用来排序的整数
    int m_generation;       //此追踪器的生成个数
    static long s_created;  //构造函数被调用的次数；
    static long s_destroyed;//析构函数被调用的次处；
    static long s_assigned; //赋值次数；
    static long s_compared; //比较次数；
    static long s_max_live; //同一时间最多存在几个Objects
    
    //重新计算“同一时间最多存在几个Objects”
    static void Update_Max_live()
    {
        if (s_created - s_destroyed > s_max_live)
        {
            s_max_live = s_created - s_destroyed;
        }
    }
public:
    static long creations(){ return s_created; }
    static long destructions(){ return s_destroyed; }
    static long assignments(){ return s_assigned; }
    static long comparisions(){ return s_compared; }
    static long max_live(){ return s_max_live; }
public:
    //构造函数
    SortTracer(int v = 0) : m_value(v),m_generation(1)
    {
        ++s_created;
        Update_Max_live();
        TRACE(TEXT("SortTracer Constructor: value=%d, generation = %d, total = %d\n"),m_value,m_generation, s_created - s_destroyed);
    }
    //拷贝构造
    SortTracer(const SortTracer& oth)
        :m_value(oth.m_value),m_generation(oth.m_generation)
    {
        ++s_created;
        Update_Max_live();
        TRACE(TEXT("SortTracer copy constructor: value=%d, generation = %d, total = %d\n"),m_value,m_generation, s_created - s_destroyed);
    }
    //析构函数
    ~SortTracer()
    {
        ++s_destroyed;
        Update_Max_live();
        TRACE(TEXT("SortTracer destroyed: value=%d, generation = %d, total = %d\n"),m_value,m_generation, s_created - s_destroyed);
    }
    //赋值操作符
    SortTracer& operator = (const SortTracer& oth)
    {
        ASSERT(this != &oth); //测试时不能对自己赋值
        ++s_assigned;
        TRACE(TEXT("SortTracer assigned: value=%d, generation = %d, total = %d\n"),m_value,m_generation, s_created - s_destroyed);
        this->m_value = oth.m_value;
        return *this;
    }
    //比较(comparision)
    friend bool operator < (const SortTracer& lft, const SortTracer& rht )
    {
        ++s_compared;
        TRACE(TEXT("SortTracer comparision: left value=%d,right value = %d\n"),lft.m_value,rht.m_value);
        return lft.m_value < rht.m_value;
    }
    int val() const
    {
        return m_value;
    }
};

long SortTracer::s_created = 0;
long SortTracer::s_destroyed = 0;
long SortTracer::s_max_live = 0;
long SortTracer::s_assigned = 0;
long SortTracer::s_compared = 0;

void CTemplateTester::test_tracer()
{
    SortTracer input[] = {7,3,5,6,4,2,0,1,9,8};
    std::sort<>(&input[0], &input[9] + 1);

    //CPPUNIT_ASSERT(SortTracer::creations() == 15);
    //CPPUNIT_ASSERT(SortTracer::max_live() == 12);
    //CPPUNIT_ASSERT(SortTracer::assignments() == 32);
    //CPPUNIT_ASSERT(SortTracer::comparisions() == 27);
    
}