#include "StdAfx.h"
#include "STLListTester.h"
#include "MyTestData.h"
#include "STLTesterUtil.h"

#include <list>
#include <string>
#include <algorithm>

#include <ftlString.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CSTLListTester::test_constructor()
{
    // define a list of 10 elements and initialise them all to 0 
    std::list<int> Fred(10, 0); //定义一个list，并同时初始化它的元素
    // list now contains 0,0,0,0,0,0,0,0,0,0 
    CPPUNIT_ASSERT(Fred.size() == 10);

    //定义一个list并用另一个STL容器的一个范围来初始化它，这个STL容器不一定是一个list， 仅仅需要是元素类型相同的的容器就可以

    int IntArray[2] = {1,2};
    std::list<int> Harry(&IntArray[0], &IntArray[2]);  //注意，这里使用的是不存在的下标2

    CPPUNIT_ASSERT(Harry.size() == 2);
    CPPUNIT_ASSERT( 1 == (*Harry.begin()));
    CPPUNIT_ASSERT( 2 == (*Harry.rbegin()));
}

void CSTLListTester::testMyTestDataList()
{
    DECLARE_MYTEST_DATA_COUNT_CHECKER(dataCountChecker,ctDataCount, 0, __FILE__, __LINE__);
    {
        //由于List 中保存的是类实例（非指针），因此在 push_back 和 pus_front 的时候会调用拷贝构造（性能问题）

        std::list<CMyTestData> myList;

        //用empty来代替检查size()是否为0
        //对于所有的标准容器，empty是一个常数时间的操作，但对于一些list实现，size花费线性时间--需要遍历(和 splice 的实现常数时间冲突)
        CPPUNIT_ASSERT( myList.empty() );
#if 0
        CPPUNIT_ASSERT( 0 == myList.size() ); //不好！
#endif

        myList.push_back(CMyTestData(2));
        myList.push_back(CMyTestData(3));
        myList.push_front(CMyTestData(1));

        //在指定位置的前面插入（即 0 在最前面）
        myList.insert(myList.begin(),CMyTestData(0));
        myList.insert(myList.end(),CMyTestData(4));
        
#if 0
        //也可以插入一个范围内的数据（可以使用数组或其他容器）
        //int IntArray[2] = {11,12}; 
        //list1.insert(list1.end(), &IntArray[0], &IntArray[2]); 
#endif

        CPPUNIT_ASSERT(myList.size() == 5);
        CPPUNIT_ASSERT(!myList.empty());
        CPPUNIT_ASSERT( (*myList.begin()).GetCurrentIndex() == 0);//0 在最前面
        CPPUNIT_ASSERT( (*myList.rbegin()).GetCurrentIndex() == 4);//4 在最后面

        std::list<CMyTestData>::iterator iter = myList.begin();
        while (iter != myList.end())
        {
            //此时没有增加引用计数，不调用 operator = 和 拷贝构造
            TRACE(_T("myList Index is %d \n"),iter->GetCurrentIndex());
            ++iter;
        }
        myList.pop_back();  //4
        myList.pop_front(); //0

        CPPUNIT_ASSERT(3 == myList.size());
        CPPUNIT_ASSERT(1 == (*myList.begin()).GetCurrentIndex());

        myList.erase(myList.begin()); //1
        CPPUNIT_ASSERT(2 == myList.size());
#if NEED_OPERATOR_FUNC
        //会调用 operator == 进行比较 -- 注意 和保存指针时的区别
        myList.remove(CMyTestData(3));
#endif
        TRACE(_T("Before Call List::clear\n"));
        myList.clear();
        //调 clear 的时候，会清空内部的 CMyTestData 对象
        //-- 由于目前保存的是对象，因此会自动调用析构函数，但如果是指针的话，需要手工delete

        //clear 等价于如下语句
        //myList.erase(myList.begin(),myList.endif());
        TRACE(_T("After Call List::clear\n"));

        CPPUNIT_ASSERT(myList.empty());
    }
}

void CSTLListTester::test_sort()
{
    std::list<std::string> Staff; 
    std::list<std::string>::iterator PeopleIterator; 

    Staff.push_back("John"); 
    Staff.push_back("Bill"); 
    Staff.push_back("Tony"); 
    Staff.push_back("Fidel"); 
    Staff.push_back("Nelson"); 

    //未排序的时候
    CPPUNIT_ASSERT( (*Staff.begin()) == "John"); 

    Staff.sort();
    //排完序后
    CPPUNIT_ASSERT( (*Staff.begin()) == "Bill");  //第一个位置
    CPPUNIT_ASSERT( (*Staff.rbegin()) == "Tony"); //最后一个位置


    
    std::list<unsigned char> numbers;  //4, 6, 
    int i = 0;
    for (i = 4; i >= 0; i--)
    {
        numbers.push_back('0' + i);
        numbers.push_back('0' + (9 - i));
    }

    std::string strBuf;
    std::copy(numbers.begin(),numbers.end(),std::back_inserter<std::string>(strBuf));
    CPPUNIT_ASSERT(strBuf == "4536271809");

    numbers.sort();
    strBuf = "";
    std::copy(numbers.begin(),numbers.end(),std::back_inserter<std::string>(strBuf));
    CPPUNIT_ASSERT(strBuf == "0123456789");

    //使用指定的 greater 函数进行反向排序
    numbers.sort(std::greater<unsigned char>());
    strBuf = "";
    std::copy(numbers.begin(),numbers.end(),std::back_inserter<std::string>(strBuf));
    CPPUNIT_ASSERT(strBuf == "9876543210");

}


void CSTLListTester::test_remove()
{

}

void CSTLListTester::test_splice()
{
    std::list<CMyTestData> numbers;
    int i = 0;
    for (i = 1; i < 10; i++)
    {
        numbers.push_back(CMyTestData(i));
    }

    i = 1;
    std::list<CMyTestData>::iterator iter = numbers.begin();
    while (i <= 5 && iter != numbers.end())
    {
        i++;
        iter++;
    }

    std::list<CMyTestData> numberFirst5;
    std::list<CMyTestData> numberLast;

    CPPUNIT_ASSERT( numbers.size() == 9);
    CPPUNIT_ASSERT(numberFirst5.size() == 0);

    numberFirst5.splice(numberFirst5.begin(),numbers,numbers.begin(),iter);
    TRACE(_T("list splice, first 5: \n"));

    CPPUNIT_ASSERT(numbers.size() == 4);// splice了5个，还剩下 4个
    CPPUNIT_ASSERT(numberFirst5.size() == 5);// splice了5个，还剩下 4个

    for_each(numberFirst5.begin(),numberFirst5.end(),PrintMyTestData);

    //Flags.splice(Flags.begin(), CmdLineParameters, CmdLineParameters.begin(), StartOfFiles);
    numberLast.splice(numberLast.begin(),numbers);
    CPPUNIT_ASSERT(numberLast.size() == 4);//把 number 中剩下的（4个）全放到 numberLast 中
    CPPUNIT_ASSERT(numbers.size() == 0);// 原有的number已经不存在了。

    TRACE(_T("\n list splice, the last: \n"));
    for_each(numberLast.begin(),numberLast.end(),PrintMyTestData);
}


void CSTLListTester::test_unique()
{

}