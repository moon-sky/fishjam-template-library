#include "StdAfx.h"
#include "STLUtilityTester.h"
#include "STLTesterUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CSTLUtilityTester::test_advance()
{
    typedef std::deque<int> IntDeque;
    typedef IntDeque::iterator Iter;
    typedef IntDeque::const_iterator ConstIter;

    IntDeque  d;
    for(int i = 0; i< 10; i++)
    {
        d.push_back(i);
    }
    ConstIter ci = d.end();         // 让ci指向d

    //要得到与const_iterator指向同一位置的iterator，首先将iterator指向容器的起始位置，
    //然后把它向前移到和const_iterator距离容器起始位置的偏移量一样的位置即可
    Iter i(d.begin());				// 初始化i为d.begin()
    //distance返回两个指向同一个容器的iterator之间的距离，advance则用于将一个iterator移动指定的距离
    //通过 ConstIter显式的指明distance调用的模板参数类型，从而避免编译器自己得出它们的类型(否则无法编译)
    advance(i, std::distance<ConstIter>(i, ci));	// 把i移到指向ci位置

    //advance(p, 2);  // same as p = p + 2;

    //TODO: 这样的单元测试是否能说明问题
    CPPUNIT_ASSERT(i == ci);        // 已经把const_iterator转化成iterator
}

void CSTLUtilityTester::test_distance()
{
    std::vector<int> intVector;
    for (int i = 1; i <= 9; i++)
    {
        intVector.push_back(i);
    }

    std::vector<int>::iterator iter = std::find(intVector.begin(), intVector.end(), 7);
    //找到元素7的迭代器
    CPPUNIT_ASSERT(7 == *iter);

    //从元素7的迭代器到结尾，需要递增 3 次，依次为 8,9,end()
    std::vector<int>::difference_type diff = std::distance(iter, intVector.end());
    CPPUNIT_ASSERT(3 == diff );

    //递增3次以后，就指向那结束符
    std::advance(iter, diff);
    CPPUNIT_ASSERT(intVector.end() == iter);
}

void CSTLUtilityTester::test_delete()
{
    DECLARE_MYTEST_DATA_COUNT_CHECKER(dataCountChecker,ctDataCount, 0, __FILE__, __LINE__);

    std::vector<CMyTestData*> myTestArray;
    for (int i = 0; i< 10; i++)
    {
        myTestArray.push_back(new CMyTestData(i));
    }

    //dosomething

    {
        DECLARE_MYTEST_DATA_COUNT_CHECKER(destructCountChecker,ctDestructCount, 10, __FILE__, __LINE__);
        std::for_each(myTestArray.begin(),myTestArray.end(), ObjecteDeleter<CMyTestData*>());
    }
}

void CSTLUtilityTester::test_boost_delete()
{
    DECLARE_MYTEST_DATA_COUNT_CHECKER(dataCountChecker,ctDataCount, 0, __FILE__, __LINE__);
    {
#ifdef BOOST
        //注意：不能使用标准的 auto_ptr 职能指针 -- 参见 Effective STL 条款8
        //  即不能： vector<auto_ptr<CMyTestData> > MyTestDatas;	

        typedef boost::shared_ptr<CMyTestData> SPD;	//SPD = "shared_ptr to Widget"
        vector< SPD > vwp;
        for (int i = 0; i < 10; ++i)
        {
            vwp.push_back(SPD(new CMyTestData));	// 从一个Widget建立SPD,然后进行一次push_back
        }
#endif
    }
    //超出生存周期，自动释放(没有内存泄漏，甚至在上面代码中抛出异常)
}

void CSTLUtilityTester::test_StringPtrSet()
{
    {
        StringPtrSet strPtrSet;
        //set<string*> strPtrSet;
        strPtrSet.insert(new std::string("Banana"));
        strPtrSet.insert(new std::string("Orange"));
        strPtrSet.insert(new std::string("Apple"));

#if 0
        // 通过解引用“转换”ssp中的每个元素，把结果写入cout
        transform(strPtrSet.begin(), strPtrSet.end(),ostream_iterator<string>(cout, "\n"),Dereference());
#endif

        //按照字母顺序对插入的string指针排序
        CPPUNIT_ASSERT((**strPtrSet.begin()) == "Apple"); 
        CPPUNIT_ASSERT((**strPtrSet.rbegin()) == "Orange");

        for_each(strPtrSet.begin(),strPtrSet.end(),ObjecteDeleter<std::string*>());

    }

    //使用模版
    {
        std::set<std::string*,DereferenceLess> strPtrSet2;
        strPtrSet2.insert(new std::string("Banana"));
        strPtrSet2.insert(new std::string("Orange"));
        strPtrSet2.insert(new std::string("Apple"));

        //按照字母顺序对插入的string指针排序
        CPPUNIT_ASSERT((**strPtrSet2.begin()) == "Apple"); 
        CPPUNIT_ASSERT((**strPtrSet2.rbegin()) == "Orange");

        std::for_each(strPtrSet2.begin(),strPtrSet2.end(),ObjecteDeleter<std::string*>());
    }
}


void CSTLUtilityTester::test_swap()
{
    std::vector<int> intVector;
    CPPUNIT_ASSERT(intVector.capacity() == 0);

    for (int i = 0; i< 100; i++)
    {
        intVector.push_back(i);
    }

    CPPUNIT_ASSERT(intVector.capacity() >= 100);

    intVector.erase(intVector.begin() + 20,intVector.end());

    CPPUNIT_ASSERT(intVector.capacity() >= 100);

    //
    std::vector<int>(intVector).swap(intVector);

    CPPUNIT_ASSERT(intVector.capacity() < 100 );
    CPPUNIT_ASSERT(intVector.capacity() >= 20 ); //还剩20个
}

template<typename T>
class CMyBPFCImpl1 : public CBPFCImpl<T>
{
public:
    virtual void operator()(const T& val) const
    {
        TRACE("CMyBPFCImpl1 CurrentIndex = %d\n",m_MyData.GetCurrentIndex());
    }
};

void CSTLUtilityTester::test_BPFC()
{
    std::list<CMyTestData> myTestDataList;
    for (int i = 0; i< 5; i++)
    {
        myTestDataList.push_back(CMyTestData(i));
    }
    //CPPUNIT_ASSERT(FALSE); 
    //怎么编译、测试？ 头文件也不正确。
    //for_each(myTestDataList.begin(),myTestDataList.end(),CBPFC<CMyTestData>(CBPFCImpl<CMyTestData>()));
}