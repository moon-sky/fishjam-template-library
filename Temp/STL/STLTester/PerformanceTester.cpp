#include "StdAfx.h"
#include "PerformanceTester.h"
#include <atlcoll.h>

void CPerformanceTester::test_container_holder()
{
    {
#ifdef NEED_OPERATOR_FUNC
        //在容器中直接保存变量,在加入变量/复制等时 实际上是进行了拷贝构造的
        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataCountChecker,ctDataCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCountChecker,ctConstructCount, 5, __FILE__, __LINE__);

        //因为vector没有设置初始容量,push_back 时进行扩展,实际上的 拷贝构造函数达到15此 -- 如果设置了促使容量,应该只有5次
        DECLARE_MYTEST_DATA_COUNT_CHECKER(copyConstructCounterChecker,ctCopyConstructCount, 15, __FILE__, __LINE__); 
        DECLARE_MYTEST_DATA_COUNT_CHECKER(destructCountChecker,ctDestructCount, 20, __FILE__, __LINE__);

        typedef std::vector<CMyTestData>  MyDataVector;
        MyDataVector dataVector;

        for (int i = 0; i< 5; i++)
        {
            dataVector.push_back(CMyTestData(i, FALSE));
        }
#endif
    }

    {
#ifdef NEED_OPERATOR_FUNC
        //在容器中直接保存变量,在加入变量/复制等时 实际上是进行了拷贝构造的
        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataCountChecker,ctDataCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCountChecker,ctConstructCount, 5, __FILE__, __LINE__);

        DECLARE_MYTEST_DATA_COUNT_CHECKER(copyConstructCounterChecker,ctCopyConstructCount, 5, __FILE__, __LINE__); 
        DECLARE_MYTEST_DATA_COUNT_CHECKER(destructCountChecker,ctDestructCount, 10, __FILE__, __LINE__);

        typedef std::vector<CMyTestData>  MyDataVector;
        MyDataVector dataVector;
        dataVector.reserve(10);    //设置初始容量,避免移动

        for (int i = 0; i< 5; i++)
        {
            dataVector.push_back(CMyTestData(i, FALSE));
        }
#endif
    }

    {
        //采用指针的方式,则只会加入实际生成的变量,其他时候都是指针处理,节约空间/时间
        //但需要手工释放,否则会造成内存泄漏 -- 或者使用 shareptr 
        typedef std::vector<CMyTestData*>   MyDataPointVector;
        MyDataPointVector   dataPointVector;

        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataCountChecker,ctDataCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCountChecker,ctConstructCount, 5, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(copyConstructCounterChecker,ctCopyConstructCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(destructCountChecker,ctDestructCount, 5, __FILE__, __LINE__);

        for (int i = 0; i < 5; i++)
        {
            dataPointVector.push_back(new CMyTestData(i));
        }

        for_each(dataPointVector.begin(), dataPointVector.end(), FTL::ObjecteDeleter<CMyTestData*>() );
        dataPointVector.clear();
    }

    {
#ifdef NEED_OPERATOR_FUNC
        const int DATA_COUNT = 10;

        //在容器中直接保存变量,在加入变量/复制等时 实际上是进行了拷贝构造的
        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataCountChecker,ctDataCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCountChecker,ctConstructCount, DATA_COUNT, __FILE__, __LINE__);

        DECLARE_MYTEST_DATA_COUNT_CHECKER(copyConstructCounterChecker,ctCopyConstructCount, DATA_COUNT, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(destructCountChecker,ctDestructCount, 2*DATA_COUNT, __FILE__, __LINE__);

        //注意:AtlArray 在扩展容量时,不会拷贝原来的数据 -- 性能比 vector 要高
        typedef CAtlArray<CMyTestData>  MyDataArray;
        MyDataArray dataArray;
        
        for (int i = 0; i< DATA_COUNT; i++)
        {
            dataArray.Add(CMyTestData(i));
        }
#endif

    }
}
