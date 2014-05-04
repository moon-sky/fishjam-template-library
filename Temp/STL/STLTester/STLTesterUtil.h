#pragma once


#include <cppunit/extensions/HelperMacros.h>

#include <string>
#include <set>
#include <algorithm>

extern LONG g_forEachCount;

//! 使用 for_each 打印每一个元素
void PrintStringItem (std::string& StringToPrint);
void PrintIntItem (int& intToPrint);
void PrintMyTestData(CMyTestData & myDataToPrint);

//! 统计不及格的个数
bool CountFailure(int value);



//判断命令行传入的是否是选项
class IsAFlag { 
public: 
    bool operator () (std::string& PossibleFlag) { 
        return PossibleFlag.substr(0,1)=="-"; 
    } 
}; 

//定义函数对象类IsFailure，该类能判断成绩是否不及格。
//如果不及格的话，函数调用operator()返回一个true，否则返回false
class IsFailure
{
public:  
    IsFailure()
    {
        TRACE(_T("IsFailure::Constructor\n"));
    }
    IsFailure(const IsFailure& copy)
    {
        TRACE(_T("IsFailure::Copy Constructor\n"));
    }
    ~IsFailure()
    {
        TRACE(_T("IsFailure::Destructor\n"));
    }
    bool operator() ( int value ) 
    {
        return (value < 60);
    }
private:
    IsFailure& operator = (const IsFailure& copy)
    {
        TRACE(_T("IsFailure::operator = \n"));
        if (this == &copy)
        {
            return *this;
        }
        return *this;
    }
};

//! 使用结构来作为函数对象，并通过构造函数传递参数 -- 统计特定成绩段中的人数
struct IsClassX
{
public:
    IsClassX(TCHAR type, int lowScore, int highScore)
    {
        m_type = type;
        m_lowScore = lowScore;
        m_highScore = highScore;

        TRACE(_T("IsClassX::Constructor ,type='%c', score between [%d,%d)\n"),m_type,m_lowScore,m_highScore);
    }
    IsClassX(const IsClassX& copy)
    {
        m_type = copy.m_type;
        m_lowScore = copy.m_lowScore;
        m_highScore = copy.m_highScore;
        TRACE(_T("IsClassX::Copy Constructor,type='%c', score between [%d,%d)\n"), m_type,m_lowScore,m_highScore);
    }
    ~IsClassX()
    {
        TRACE(_T("IsClassX::Destructor, type = %c, score between [%d,%d)\n"),m_type,m_lowScore,m_highScore);
    }

    bool operator() ( int value )
    {
        //TODO:100 如何处理？
        if (value >= m_lowScore && value < m_highScore)
        {
            return true;
        }
        return false;
    }
    TCHAR GetClassType()
    {
        return m_type;
    }
private:
    TCHAR m_type;
    int m_lowScore;
    int m_highScore;
    IsClassX& operator = (const IsClassX& copy);
};

//在set<string*> 中保证 set 按照字母顺序保存string -- 参见 Effective STL 中的条款20
struct StringPtrLess: public std::binary_function<const std::string*, const std::string*,bool>
{
    bool operator()(const std::string *ps1, const std::string *ps2) const
    {
        return *ps1 < *ps2;
    }
};

typedef std::set<std::string*, StringPtrLess> StringPtrSet;

//可以在容器保存指针时使用的 仿函数模板
struct DereferenceLess 
{
    template <typename PtrType>
    bool operator()(PtrType pT1, PtrType pT2) const		// 参数是值传递的，因为我们希望它们是（或行为像）指针
    {
        return *pT1 < *pT2;
    }
};


//泛型的解引用仿函数类 -- 当本类型的仿函数被传入一个T*时，它们返回一个const T&
struct Dereference 
{
    template <typename T>
    const T& operator()(const T *ptr) const
    {
        return *ptr;
    }
};


#if 0
    //想建立 使用大的、支持虚函数的仿函数对象(实际上是不行的) -- 见 Effective STL 38
    template<typename T>
    class CBPFC:	public unary_function<T, void>      // BPFC = “Big Polymorphic Functor Class”
    {
    private:
        CMyTestData m_MyData;					    // 这个类有很多数据，所以用值传递会影响效率
        int m_otherInfo;
    public:
        virtual void operator()(const T& val) const;// 这是一个虚函数，所以切割时会出问题
    };
#endif 

//建立一个包含一个指向实现类的指针的小而单态的类，然后把所有数据和虚函数放到实现类：
template<typename T>
class CBPFC;

template<typename T>            // 用于修改的BPFC的新实现类
class CBPFCImpl :public std::unary_function<T, void> 
{
private:
    CMyTestData m_MyData;					// 以前在 CBPFC 里的所有数据现在在这里
    int m_otherInfo;
public:
    virtual ~CBPFCImpl(){};				    // 多态类需要虚析构函数

    virtual void operator()(const T& val) const
    {
    
    }
    friend class CBPFC<T>;				    // 让 CBPFC 可以访问这些数据
};

//仿函数类（CBPFC）是小而单态的，但可以访问大量状态而且行为多态
//必须支持合理方式的拷贝 -- 保证拷贝构造函数对指向的BPFCImpl对象做了合理的事情。也许最简单的合理的东西是引用计数
template<typename T>
class CBPFC : public std::unary_function<T, void>	// 小的，单态版的BPFC
{
private:
    CBPFCImpl<T> *m_pImpl;				    // 这是CBPFC唯一的数据
public:
    CBPFC(CBPFCImpl<T>* pImpl)
    {
        m_pImpl = pImpl;
    }
    void operator()(const T& val) const		// 现在非虚；
    {						
        pImpl->operator() (val);            // 调用BPFCImpl的
    }
private:
    CBPFC(const CBPFC<T>& other);
};


//判断值是否在指定值之间的仿函数类
//使用：
//  vector<int>::iterator i = find_if(v.begin(), v.end(),BetweenValues<int>(x, y));
template<typename T>
class BetweenValues: public std::unary_function<T, bool> 
{
public: 
    BetweenValues(const T& lowValue, const T& highValue)			// 使构造函数保存上下界
        :lowVal(lowValue), highVal(highValue){} 
    bool operator()(const T& val) const	//返回val是否在保存的值之间 
    {
        return (val > lowVal) && (val < highVal);
    }
private: 
    T lowVal; 
    T highVal; 
}; 

//判断两个容器中的元素相同的函数
template<typename InIter1, typename InIter2>
void container_elem_equal_assert(InIter1 first1, InIter1 last1, InIter2 first2, InIter2 last2)
{
    std::ostringstream os;
    while (first1 != last1 && first2 != last2 )
    {
        os.str("");
        os << *first1 << " == " << *first2;

        if (*first1 != *first2)
        {
            //采用 if 内放置 ASSERT 断言的方法，可以方便设置断点，发现出错的具体位置
            CPPUNIT_ASSERT_MESSAGE(os.str(), *first1 == *first2);
        }
        ++first1;
        ++first2;
    }

    //两个容器的个数相等 -- 即退出循环后都应该到最后
    CPPUNIT_ASSERT(std::distance(first1, last1) == std::distance(first2, last2));
    CPPUNIT_ASSERT(first1 == last1);
    CPPUNIT_ASSERT(first2 == last2);
}

#define STLTESTER_ASSERT_CONTAINER_ELEMENT_EQUAL    container_elem_equal_assert

