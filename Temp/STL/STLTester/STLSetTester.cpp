#include "StdAfx.h"
#include "STLSetTester.h"
#include "STLTesterUtil.h"

#include <streambuf>
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CSTLSetTester::test_set_order()
{
    std::set<char>   int_set;

    //插入 1到10之间的奇数
    for (int i = 1; i < 10; i+= 2)
    {
        int_set.insert(i + '0');
    }

    //插入 2到10之间的偶数
    for(int i = 2; i < 10; i+= 2)
    {
        int_set.insert(i + '0');
    }
    std::string strBuf;
    //挨着顺序将set中的值输出到string中，必然是顺序的
    std::copy(int_set.begin(),int_set.end(),std::back_inserter<std::string>(strBuf));
    CPPUNIT_ASSERT(strBuf == "123456789");
}

void CSTLSetTester::test_set_find()
{
    DECLARE_MYTEST_DATA_COUNT_CHECKER(myDataCountChecker,ctDataCount, 0, __FILE__, __LINE__);
    {
        std::set<CMyTestData*,less_CMyTestData_Pointer> myDataSet;
        {
            //由于保存的是指针，因此不会生成临时变量
            DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCountChecker,ctConstructCount, 5, __FILE__, __LINE__);
            DECLARE_MYTEST_DATA_COUNT_CHECKER(copyConstructCountChecker,ctCopyConstructCount, 0, __FILE__, __LINE__);
            DECLARE_MYTEST_DATA_COUNT_CHECKER(destructCountChecker,ctDestructCount, 0, __FILE__, __LINE__);
            for (int i = 5; i > 0; i--)
            {
                myDataSet.insert(new CMyTestData(i));
            }
        }

        //总共5个，按照 CurrentIndex 由小到大的顺序排列(1~5)
        CPPUNIT_ASSERT(myDataSet.size() == 5);
        CPPUNIT_ASSERT(((CMyTestData*)(*myDataSet.begin()))->GetCurrentIndex() == 1 );
        CPPUNIT_ASSERT(((CMyTestData*)(*myDataSet.rbegin()))->GetCurrentIndex() == 5 );

        //能找到中间的3，但不能找到不在set中的 8;
        CMyTestData tempData3(3);
        CPPUNIT_ASSERT(myDataSet.find(&tempData3) != myDataSet.end());

        CMyTestData tempData8(8);
        CPPUNIT_ASSERT(myDataSet.find(&tempData8) == myDataSet.end());

        std::for_each(myDataSet.begin(),myDataSet.end(),ObjecteDeleter<CMyTestData*>());
        myDataSet.clear();
    }

    {
#ifdef NEED_OPERATOR_FUNC
        std::set<CMyTestData> myDataSet;
        for( int i = 0; i < 100; i++)
        {
            myDataSet.insert(CMyTestData(i, FALSE));
        }
        {
            //使用成员函数查找
            DECLARE_MYTEST_DATA_COUNT_CHECKER(compareLessCountChecker,ctCompareEqualCount, 0, __FILE__, __LINE__);
            LONG lOldCompareLessCount = CMyTestData::GetCompareLessCount();
            std::set<CMyTestData>::iterator iter = myDataSet.end();
            {
                //在查找时不会再生成中间的临时变量了，此处的1是 CMyTestData(80, FALSE)
                DECLARE_MYTEST_DATA_COUNT_CHECKER(destrucyCountChecker,ctDestructCount, 1, __FILE__, __LINE__);
                iter = myDataSet.find(CMyTestData(80, FALSE));
            }
            CPPUNIT_ASSERT(iter != myDataSet.end() );
            CPPUNIT_ASSERT(iter->GetCurrentIndex() == 80 );

            LONG lUseCompareLessCount = CMyTestData::GetCompareLessCount() - lOldCompareLessCount;
            TRACE(" Find CMyTestData(80) in [0,100) use Compare Less %d Count\n", lUseCompareLessCount);
            //查找次数远小于遍历方式查找的80 -- 测试结果为 15
            CPPUNIT_ASSERT( lUseCompareLessCount < 80);
        }

        {
            //使用全局的find方法查找 -- 会使用线性的方式，使用 operator== 依次查找
            DECLARE_MYTEST_DATA_COUNT_CHECKER(compareEqualCountChecker,ctCompareEqualCount, 81, __FILE__, __LINE__);
            DECLARE_MYTEST_DATA_COUNT_CHECKER(compareLessCountChecker,ctCompareLessCount, 0, __FILE__, __LINE__);

            std::set<CMyTestData>::iterator iter = std::find(myDataSet.begin(), myDataSet.end(), CMyTestData(80, FALSE));
            CPPUNIT_ASSERT(iter != myDataSet.end());
            CPPUNIT_ASSERT(iter->GetCurrentIndex() == 80);
        }
#endif
    }
}

void CSTLSetTester::test_set_insert()
{
#ifdef NEED_OPERATOR_FUNC
    DECLARE_MYTEST_DATA_COUNT_CHECKER(myDataCountChecker,ctDataCount, 0, __FILE__, __LINE__);
    {
        const int MY_DATA_COUNT = 5;
        //会按照默认的 < 操作符 排序，依次是 0~4
        typedef std::set<CMyTestData> MyDataSet;
        typedef std::pair<MyDataSet::iterator, bool> MyDataSetInsertResult;

        MyDataSet myDataSet;
        {
            DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCountChecker, ctConstructCount, MY_DATA_COUNT, __FILE__, __LINE__); //会调用LONG参数的构造
            
            //由于保存的是元素，因此会通过拷贝构造生成 MY_DATA_COUNT 个临时变量，然后又释放
            DECLARE_MYTEST_DATA_COUNT_CHECKER(copyConstructCountChecker,ctCopyConstructCount, MY_DATA_COUNT, __FILE__, __LINE__);
            DECLARE_MYTEST_DATA_COUNT_CHECKER(destructCountChecker, ctDestructCount, MY_DATA_COUNT, __FILE__, __LINE__);
            DECLARE_MYTEST_DATA_COUNT_CHECKER(compareLessCountChecker, ctCompareLessCount, 16, __FILE__, __LINE__); // 3 + 4 + 4 + 5

            //set 使用 < 和 !< 进行比较
            DECLARE_MYTEST_DATA_COUNT_CHECKER(compareEqualCountChecker,ctCompareEqualCount, 0, __FILE__, __LINE__);
            for (int i = 0; i < MY_DATA_COUNT; i++)
            {
                myDataSet.insert(CMyTestData(i));
            }
        }

        CMyTestData& oldOneData = *myDataSet.begin();
        CPPUNIT_ASSERT(oldOneData.GetCurrentIndex() == 0);
        LONG lOldOtherInfo = oldOneData.GetOtherInfo();

        MyDataSetInsertResult insertResult = myDataSet.insert(CMyTestData(0));
        //由于已经有相同的元素了，因此插入失败,但会返回查找到相同元素的迭代器
        CPPUNIT_ASSERT(insertResult.first == myDataSet.find(CMyTestData(0)));
        CPPUNIT_ASSERT(insertResult.second == false);

        //原来的元素不会产生变化
        CPPUNIT_ASSERT(lOldOtherInfo == (*myDataSet.begin()).GetOtherInfo());
    }
#endif
}