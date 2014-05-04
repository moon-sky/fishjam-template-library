#include "StdAfx.h"
#include "STLAlgorithmTester.h"
#include "MyTestData.h"
#include "STLTesterUtil.h"
#include <functional>
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 返回第二个元素是否是第一个元素的两倍
bool twice (int elem1, int elem2 )
{
    return elem1 * 2 == elem2;
}

CSTLAlgorithmTester::CSTLAlgorithmTester()
{

}

CSTLAlgorithmTester::~CSTLAlgorithmTester()
{

}

void CSTLAlgorithmTester::setUp()
{
    m_string2MyTestDataMap.insert(String2MyTestDataPair("one", new CMyTestData(1)));
    m_string2MyTestDataMap.insert(String2MyTestDataPair("two", new CMyTestData(2)));
    m_string2MyTestDataMap.insert(String2MyTestDataPair("three", new CMyTestData(3)));

    m_string2MyTestDataMap.insert(String2MyTestDataPair("ONE", new CMyTestData(1)));
    m_string2MyTestDataMap.insert(String2MyTestDataPair("TWO", new CMyTestData(2)));
    m_string2MyTestDataMap.insert(String2MyTestDataPair("THREE", new CMyTestData(3)));


    m_MyTestDataList.push_back(new CMyTestData(8));
    m_MyTestDataList.push_back(new CMyTestData(1));
    m_MyTestDataList.push_back(new CMyTestData(3));
    m_MyTestDataList.push_back(new CMyTestData(2));
    m_MyTestDataList.push_back(new CMyTestData(4));

    m_MyTestDataList.push_back(new CMyTestData(-1));
    m_MyTestDataList.push_back(new CMyTestData(-3));
    m_MyTestDataList.push_back(new CMyTestData(-2));
    m_MyTestDataList.push_back(new CMyTestData(-4));

}

void CSTLAlgorithmTester::tearDown()
{
    for_each(m_string2MyTestDataMap.begin(), m_string2MyTestDataMap.end(),
        PairDeleter<std::string, CMyTestData*>());

    m_string2MyTestDataMap.clear();

    for_each(m_MyTestDataList.begin(), m_MyTestDataList.end(),ObjecteDeleter<CMyTestData*>() );
    m_MyTestDataList.clear();
}

void CSTLAlgorithmTester::test_adjacent_find()
{
    //std::adjacent_find(m_MyTestDataList.begin(), m_MyTestDataList.end() , )
    m_MyTestDataList.sort(less_CMyTestData_Pointer());

    std::list <int> L;
    std::list <int>::iterator Iter;
    std::list <int>::iterator result1, result2;
    L.push_back( 50 );
    L.push_back( 10 );
    L.push_back( 20 );
    L.push_back( 40 );
    L.push_back( 20 );
    
    L.sort();

    result1 = std::adjacent_find( L.begin( ), L.end( ) );
    //搜索到具有相同值的两个临近的20
    CPPUNIT_ASSERT(*result1 == 20);
    
    result2 = std::adjacent_find( L.begin( ), L.end( ), twice );
    //搜索到后一个数是第一个数2倍的临近元素(分别是 10 & 20)
    CPPUNIT_ASSERT(*result2 == 10); 
    CPPUNIT_ASSERT(*(++result2) == 20);
}

void CSTLAlgorithmTester::test_adjacent_difference()
{
    //std::adjacent_difference()
}

void CSTLAlgorithmTester::test_binary_search()
{
    std::list <int> L;
    std::list <int>::iterator Iter;
    bool b1;

    L.push_back( 50 );
    L.push_back( 10 );
    L.push_back( 30 );
    L.push_back( 20 );
    L.push_back( 25 );
    L.push_back( 5 );
    L.push_back( -10 );
    L.push_back( -15 );
    L.sort( );  //排序（默认是从小到大 -- less<int>）
    
    b1 = binary_search( L.begin( ), L.end( ), 10 );
    CPPUNIT_ASSERT(true == b1); //找到了10

    b1 = binary_search( L.begin( ), L.end( ), 15 );
    CPPUNIT_ASSERT(false == b1); //没有找到15

    L.sort(std::greater<int>()); //按照从大到小的顺序排序

    b1 = binary_search( L.begin(), L.end(), 10, std::greater<int>());
    CPPUNIT_ASSERT( true == b1);

    //b1 = std::binary_search(L.begin(), L.end(), 10, std::binder2nd(std::ptr_fun(abs)),-15);
    //RIter = find_if( v1.begin( ), v1.end( ),
    //    not1 ( bind2nd (ptr_fun ( strcmp ), "pearly" ) ) );

}

void CSTLAlgorithmTester::test_copy_if()
{
    //从文件流中读取数据到内存变量中？
    //std::copy(std::istream_iterator<Pointer2D>(infile), std::istream_iterator<Pointer2D>(),
    //    std::back_inserter(points));

    const int BIGGER_NUMBER = 8;
    std::vector<int> intVector1,intVector2;

    intVector1.reserve(10);
    for (int i = 0; i<= 10; i++)
    {
        intVector1.push_back(i);
    }

    CPPUNIT_ASSERT(intVector2.size() == 0);  //开始时没有数据
    FTL::copy_if(intVector1.begin(),intVector1.end(),back_inserter(intVector2) ,
        bind2nd(std::greater<int>(), BIGGER_NUMBER));
    CPPUNIT_ASSERT(intVector2.size() == 2);  //拷贝大于8的2个数据(9和10)
    
    CPPUNIT_ASSERT(*intVector2.begin() > BIGGER_NUMBER); 
}

void CSTLAlgorithmTester::test_for_each()
{
    std::list<std::string> FruitAndVegetables;
    FruitAndVegetables.push_back("carrot");
    FruitAndVegetables.push_back("pumpkin");
    FruitAndVegetables.push_back("potato");
    FruitAndVegetables.push_front("apple");
    FruitAndVegetables.push_front("pineapple");
    
    g_forEachCount = 0;

    for_each(FruitAndVegetables.begin(), FruitAndVegetables.end(), PrintStringItem);
    
    CPPUNIT_ASSERT(g_forEachCount == FruitAndVegetables.size());
}


void CSTLAlgorithmTester::test_generate()
{
    int checkArray[] = {1,2,3,4,5,6,7,8,9,10};
    
    std::vector<int>  intVect(10);
    //使用 generate 在已知大小的 vector 中生成 [1,10]
    std::generate(intVect.begin(), intVect.end(), sequence_generator<int>(1,1));
    STLTESTER_ASSERT_CONTAINER_ELEMENT_EQUAL(checkArray, ARRAY_PAST_THE_END(checkArray), intVect.begin(), intVect.end());

    std::list<int> intList;
    //使用 generate_n 在list中生成指定个数的 [1,10]
    std::generate_n(back_inserter(intList), _countof(checkArray), sequence_generator<int>(1,1));
    STLTESTER_ASSERT_CONTAINER_ELEMENT_EQUAL(checkArray, ARRAY_PAST_THE_END(checkArray), intList.begin(), intList.end());

    {
#ifdef NEED_OPERATOR_FUNC
        //生成 set 中的 CMyTestData -- 注意因为 拷贝问题，有效率问题
        DECLARE_MYTEST_DATA_COUNT_CHECKER(countChecker,ctDataCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(constructChecker,ctConstructCount, 5, __FILE__, __LINE__);

         //sequence_generator_ex 的返回 + 放到 set 中
        DECLARE_MYTEST_DATA_COUNT_CHECKER(copyChecker,ctCopyConstructCount, 5 * 2, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(assignmentChecker,ctAssignmentCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(destrucChecker,ctDestructCount, 5 * 3, __FILE__, __LINE__);

        std::set<CMyTestData>   dataSet;
        std::generate_n(
            inserter(dataSet, dataSet.begin()), 
            5, sequence_generator_ex<int,CMyTestData, CMyTestData>(1,1));
        CPPUNIT_ASSERT(dataSet.size() == 5);
#endif
    }

    {
        std::set<CMyTestData*>   dataPointerSet;

    }

    {
        typedef std::map<int, CMyTestData*> Int2DataPointerMap;
        typedef Int2DataPointerMap::value_type Int2DataPointerMapValue;
        Int2DataPointerMap int2DataPtrMap;
        //std::generate_n(
        //    inserter(int2DataPtrMap, int2DataPtrMap.begin()), 
        //    5, sequence_generator_ex<int,Int2DataPointerMapValue, Int2DataPointerMapValue(1,1));
    }
}

void CSTLAlgorithmTester::test_includes()
{

}

void CSTLAlgorithmTester::test_count()
{
    std::list<int> studentScroe;
    studentScroe.push_back(100); 
    studentScroe.push_back(80);
    studentScroe.push_back(45);
    studentScroe.push_back(75);
    studentScroe.push_back(99); 
    studentScroe.push_back(100);
    studentScroe.push_back(59);
    studentScroe.push_back(60);

    //统计满足指定值的个数
    std::list<int>::iterator::difference_type NumberOf100Scores = count(studentScroe.begin(), studentScroe.end(),100);
    CPPUNIT_ASSERT(2 == NumberOf100Scores);


    //使用静态函数指针 -- 统计不及格(低于60的个数)
    std::list<int>::iterator::difference_type NumberFailure = count_if(
        studentScroe.begin(),studentScroe.end(),
        std::bind2nd(std::less<int>(), 60));   // value < 60
        //CountFailure);
    CPPUNIT_ASSERT(2 == NumberFailure);

    //使用函数对象(由它的构造函数 "IsFailure()" 临时构造的一个函数对象),count_if()将对该容器的每个对象调用这个函数。
    //注意：★会调用函数对象的拷贝构造★ -- 不能使用带很多成员变量的普通类
    NumberFailure = std::count_if(studentScroe.begin(),studentScroe.end(),IsFailure());
    CPPUNIT_ASSERT(2 == NumberFailure);


    //使用传递参数的函数对象 -- 注意会调用 一次构造函数(声明变量时)， 两次拷贝构造函数 (传递给 count_if 时 和 在 count_if 内部处理时)
    TRACE(_T("\nNow Count Class A:\n"));
    {
        IsClassX classA(_T('A'),80,101); //Notice: not 100
        std::list<int>::iterator::difference_type NumberOfClassA = count_if(studentScroe.begin(),studentScroe.end(),classA);
        CPPUNIT_ASSERT(4 == NumberOfClassA );
    }

    TRACE(_T("\nNow Count Class B:\n"));
    {
        IsClassX classB(_T('B'),60,80);
        std::list<int>::iterator::difference_type NumberOfClassB = count_if(studentScroe.begin(),studentScroe.end(),classB);
        CPPUNIT_ASSERT(2 == NumberOfClassB );
    }
}

void CSTLAlgorithmTester::test_equal()
{

}

void CSTLAlgorithmTester::test_fill()
{

}

void CSTLAlgorithmTester::test_find()
{
    std::list<std::string> FruitAndVegetables;
    FruitAndVegetables.push_back("carrot");
    FruitAndVegetables.push_back("pumpkin");
    FruitAndVegetables.push_back("potato");
    FruitAndVegetables.push_front("apple");
    FruitAndVegetables.push_front("pineapple");

    std::list<std::string>::iterator iter = std::find(FruitAndVegetables.begin(),FruitAndVegetables.end(),"apple");
    CPPUNIT_ASSERT(iter != FruitAndVegetables.end());
    CPPUNIT_ASSERT((*iter) == "apple");

    iter = std::find(FruitAndVegetables.begin(),FruitAndVegetables.end(),"orange");
    CPPUNIT_ASSERT(FruitAndVegetables.end() == iter);


    //在数组中使用指针进行查找
    int intArray[10] = {0};
    for (int i = 0; i < _countof(intArray); i++)
    {
        intArray[i] = i;
    }
    //类似 end()
    int *pPastTheEnd = intArray + _countof(intArray);

    int *pFound = std::find(intArray,pPastTheEnd , 5);
    CPPUNIT_ASSERT( pPastTheEnd != pFound);
    CPPUNIT_ASSERT( 5 == *pFound);

    //没有找到，返回最后的 end() -- 注意：不能和 NULL 比较
    int *pNotFound = std::find(intArray, pPastTheEnd, 100);
    CPPUNIT_ASSERT( pPastTheEnd == pNotFound);
}

void CSTLAlgorithmTester::test_find_if()
{
    std::list<int> studentScroe;
    studentScroe.push_back(100); 
    studentScroe.push_back(80);
    studentScroe.push_back(45);
    studentScroe.push_back(75);
    studentScroe.push_back(99); 
    studentScroe.push_back(100);
    studentScroe.push_back(59);
    studentScroe.push_back(60);

    int count = 0;
    std::list<int>::iterator iter = std::find_if(studentScroe.begin(),studentScroe.end(),IsClassX('A',80,101));
    CPPUNIT_ASSERT(studentScroe.end() != iter);
    CPPUNIT_ASSERT(*iter = 100);  //返回找到的第一个满足条件的迭代器位置

    //通过更改查找的起始位置，来遍历所有满足条件的
    while (iter != studentScroe.end())
    {
        count++;
        iter = std::find_if( ++iter, studentScroe.end(), IsClassX('A', 80, 101));
    }

    //有4个等级为A[80,100) 的
    CPPUNIT_ASSERT(4 == count );

    //没有分数在 40 分以下的
    iter = find_if(studentScroe.begin(),studentScroe.end(),IsClassX('E',0,40));
    CPPUNIT_ASSERT(studentScroe.end() == iter) ;
}

void CSTLAlgorithmTester::test_find_end()
{

}

void CSTLAlgorithmTester::test_find_first_of()
{

}

void CSTLAlgorithmTester::test_search()
{
    std::vector <int> sourceVector; //要找的序列
    std::vector <int> targetVector; //可以找到的序列
    std::vector <int> nonTargetVector; //找不到的序列

    //sourceVecotor -- { 0 5 10 15 20 25 0 5 10 15 20 25 }
    int i;
    for ( i = 0 ; i <= 5 ; i++ )
    {
        sourceVector.push_back( 5 * i );
    }
    for ( i = 0 ; i <= 5 ; i++ )
    {
        sourceVector.push_back( 5 * i );
    }

    //targetVector -- { 20 25 }
    int ii;
    for ( ii = 4 ; ii <= 5 ; ii++ )
    {
        targetVector.push_back( 5 * ii );
    }

    //nonTargetVector -- { 20 30 40 }
    int iii;
    for ( iii = 2 ; iii <= 4 ; iii++ )
    {
        nonTargetVector.push_back( 10 * iii );
    }

    std::vector<int>::iterator iter;
    iter = std::search(sourceVector.begin(),sourceVector.end(),targetVector.begin(),targetVector.end());
    CPPUNIT_ASSERT(4 == iter - sourceVector.begin() ); //第一次找到的位置在 4

    iter = std::search(sourceVector.begin(),sourceVector.end(),nonTargetVector.begin(),nonTargetVector.end());
    CPPUNIT_ASSERT(sourceVector.end() == iter);


    //说明使用 operator == 进行比较
#if NEED_OPERATOR_FUNC
    std::vector<CMyTestData> sourceTestDataVector1;
    std::vector<CMyTestData> targetTestDataVector1;
    //由于 CMyTestData 中 operator == 为私有函数，因此本语句无法编译通过
    search(sourceTestDataVector1.begin(),sourceTestDataVector1.end(),targetTestDataVector1.begin(),targetTestDataVector1.end());
#endif

    std::vector<CMyTestData*> sourceTestDataVector2;
    std::vector<CMyTestData*> targetTestDataVector2;
    
    sourceTestDataVector2.push_back(new CMyTestData(1));
    sourceTestDataVector2.push_back(new CMyTestData(2));
    sourceTestDataVector2.push_back(new CMyTestData(3));
    
    targetTestDataVector2.push_back(new CMyTestData(2));
    targetTestDataVector2.push_back(new CMyTestData(3));

    //由于vector中保存的是地址，search会比较地址，可以编译通过。但无法正确找到 CMyTestData 数值相同的序列位置
    std::vector<CMyTestData*>::iterator iterMyTestData = std::search(sourceTestDataVector2.begin(),sourceTestDataVector2.end(),targetTestDataVector2.begin(),targetTestDataVector2.end());
    //CPPUNIT_ASSERT(1 == iterMyTestData - sourceTestDataVector2.begin()); //此语句会失败，找不到
    CPPUNIT_ASSERT(sourceTestDataVector2.end() == iterMyTestData); //虽然数据相同，但由于地址不同，因此找不到

    
    //清除内存
    for (iterMyTestData = sourceTestDataVector2.begin();iterMyTestData!= sourceTestDataVector2.end();
        ++iterMyTestData)
    {
        delete *iterMyTestData;
    }
    sourceTestDataVector2.clear();

    for (iterMyTestData = targetTestDataVector2.begin();iterMyTestData!= targetTestDataVector2.end();
        ++iterMyTestData)
    {
        delete *iterMyTestData;
    }
    targetTestDataVector2.clear();
}

void CSTLAlgorithmTester::test_set_difference()
{

}

void CSTLAlgorithmTester::test_set_intersection()
{

}

void CSTLAlgorithmTester::test_random_shuffle()
{
    std::vector <int> intVector;
    for ( int i = 1 ; i <= 9 ; i++ )
    {
        intVector.push_back( i );
    }

    std::random_shuffle(intVector.begin(), intVector.end());
    std::ostringstream os;
    std::copy(intVector.begin(), intVector.end(), std::ostream_iterator<int>(os, ","));
    const std::string& strInfo = os.str();
    TRACE("after random_shuffle: %s\n",strInfo.c_str());

    for (std::vector<int>::iterator iter = intVector.begin();
        iter != intVector.end();
        ++iter)
    {
        int iNumber = *iter;
        CPPUNIT_ASSERT(iNumber >= 1 && iNumber <= 9);
    }
    
    //再排序，则是原来的 1-9 顺序(数字不变)
    std::sort(intVector.begin(), intVector.end());
    for (int i = 1; i <= 9; i++)
    {
        CPPUNIT_ASSERT(i == intVector[i-1]);
    }
}

void CSTLAlgorithmTester::test_remove()
{
    std::vector<int> NumberArray;

    for (int count = 0; count < 2; count++)
    {
        for (int i = 0; i< 5; i++)
        {
            NumberArray.push_back(i);
        }
    }
    std::vector<int>::size_type oldSize = NumberArray.size();
    std::vector<int>::iterator iterRemoveLast;

    TRACE(_T("Just Insert the date\n"));
    //{ 0,1,2,3,4,0,1,2,3,4 }
    for_each(NumberArray.begin(),NumberArray.end(),PrintIntItem);

    CPPUNIT_ASSERT(4 == (*NumberArray.rbegin()));   //现在最后一个是 4

    iterRemoveLast = std::remove(NumberArray.begin(),NumberArray.end(),4); //删除掉 NumberArray 中的所有的 4
    std::vector<int>::size_type removeCount = NumberArray.end() - iterRemoveLast;

    TRACE(_T("\nremove all 4\n"));

    //{ 0,1,2,3,0,1,2,3,3,4 } //其中最后的 3, 4 是应为移除了2个4以后，保留下来的。
    for_each(NumberArray.begin(),NumberArray.end(),PrintIntItem);

    std::vector<int>::size_type newSize = NumberArray.size();
    
    CPPUNIT_ASSERT( oldSize == newSize );   //remove 方法不更改容器的大小
    CPPUNIT_ASSERT( 2 == removeCount );     //remove 了2个4
    CPPUNIT_ASSERT(3 == (*iterRemoveLast));   //remove返回的位置是移除掉特定数据后的新数据结尾 -- 此处指向最后一个 3
    CPPUNIT_ASSERT(4 == (*NumberArray.rbegin()));   //现在最后一个还是4 (通用的 remove 算法不删除被移除的)

    NumberArray.erase(iterRemoveLast,NumberArray.end());
    
    TRACE(_T("\nNow after NumberArray.erase\n"));
    for_each(NumberArray.begin(),NumberArray.end(),PrintIntItem);
    TRACE(_T("\n"));

    CPPUNIT_ASSERT(NumberArray.size() == oldSize - 2); //删除了 2 个

    //注意 list::remove 方法删除数据同时改变大小
    std::list<int> NumberList;
    for (int count = 0; count < 2; count++)
    {
        for (int i = 0; i< 5; i++)
        {
            NumberList.push_back(i);
        }
    }
    CPPUNIT_ASSERT(NumberList.size() == 10);
    NumberList.remove(4);       //返回值为 void, 不像算法 remove 返回新的逻辑终点
    CPPUNIT_ASSERT(NumberList.size() == 10 - 2); //list::remove时同时更改了大小

    //remove容器中保存的是指针时的情况 -- Effective STL 33
#if 0
    void delAndNullifyUncertified(Widget*& pWidget)
    {
        if (!pWidget->isCertified()) {                  //如果*pWidget是一个未通过检验Widget，
            delete pWidget;				                // 删除指针并且设置它为空
            pWidget = 0;
        }
    }
    for_each(v.begin(), v.end(),delAndNullifyUncertified);//把所有指向未通过检验Widget的指针删除并且设置为空
    // 从v中除去空指针，0必须映射到一个指针，让C++可以正确地推出remove的第三个参数的类型
    v.erase(remove(v.begin(), v.end(),static_cast<Widget*>(0)),v.end());
#endif
}

void CSTLAlgorithmTester::test_remove_copy()
{

}

void CSTLAlgorithmTester::test_remove_copy_if()
{
    //临时变量，用于保存不会被删除的

}

void CSTLAlgorithmTester::test_set_union()
{
    std::set<int>  intSet1, intSet2, intSetResult;
    std::generate_n( std::inserter(intSet1, intSet1.begin()), 5, sequence_generator<int>(1,3));  // 1,4,7,10,13

    std::generate_n( std::inserter(intSet2, intSet2.begin()), 5, sequence_generator<int>(2, 2));  // 2,4,6,8,10

    std::set_union(intSet1.begin(), intSet1.end(), intSet2.begin(), intSet2.end(), inserter(intSetResult, intSetResult.begin()));
    
    //两个排好序的 set 中数据的集合
    int checkResultArray[] = {1,2,4,6,7,8,10,13};
    STLTESTER_ASSERT_CONTAINER_ELEMENT_EQUAL(checkResultArray, ARRAY_PAST_THE_END(checkResultArray), 
        intSetResult.begin(), intSetResult.end());

}

void CSTLAlgorithmTester::test_stable_sort()
{
    
}

void CSTLAlgorithmTester::test_swap()
{

}

void CSTLAlgorithmTester::test_swap_range()
{

}

void CSTLAlgorithmTester::test_stable_partition()
{
    std::list<int> numbers;
    int i;
    for ( i = 0 ; i <= 10 ; i++ )
    {
        numbers.push_back( i );
        numbers.push_back( 10 - i);
    }
    TRACE(_T("\n原始数据：\n"));
    //{ 0,10,1,9,2,8,3,7,4,6,5,5,6,4,7,3,8,2,9,1,10,0 }
    for_each(numbers.begin(),numbers.end(),PrintIntItem);
    
    //TODO: 注意此处使用了 binder1st 和 less
    std::list<int>::iterator iter = std::stable_partition(numbers.begin(),numbers.end(),
        std::binder1st<std::less<int> >(std::less<int>(), 5));
    
    CPPUNIT_ASSERT(*iter <= 5);

    TRACE(_T("\nNow is the first part: greater than 5\n"));
    for_each(numbers.begin(),iter,PrintIntItem);

    TRACE(_T("\nnow is the second parts(less or equal than 5)\n"));
    for_each(iter,numbers.end(),PrintIntItem);

    TRACE(_T("\n"));
}

void CSTLAlgorithmTester::test_unique()
{
    std::list<int> myList;
    for (int i = 1; i< 10; i++)
    {
        myList.push_back(i);
    }
    myList.push_back(5);
    CPPUNIT_ASSERT(myList.size() == 10);

    myList.sort();
    myList.erase(std::unique(myList.begin(),myList.end()), myList.end());
    CPPUNIT_ASSERT(myList.size() == 9); //not ok
}

void CSTLAlgorithmTester::test_permutation()
{

}

void CSTLAlgorithmTester::test_inner_product()
{

}

void CSTLAlgorithmTester::test_inner_merge()
{

}

void CSTLAlgorithmTester::test_iter_swap()
{

}

void CSTLAlgorithmTester::test_lexicographical_compare()
{

}

void CSTLAlgorithmTester::test_nth_element()
{
    //nth_element(widgets.begin(),			// 把最好的20个元素
    //    widgets.begin() + 19,		// 放在widgets前端，
    //    widgets.end(),			// 但不用担心
    //    qualityCompare);			// 它们的顺序

}
void CSTLAlgorithmTester::test_partition()
{
    //vector<Widget>::iterator goodEnd =			// 把所有满足hasAcceptableQuality
    //    partition(widgets.begin(),			// 的widgets移动到widgets前端，
    //    widgets.end(),		// 并且返回一个指向第一个
    //    hasAcceptableQuality);	// 不满足的widget的迭代器


    //vector<Widget>::size_type goalOffset =		// 指出兴趣的Widget
    //    0.25 * widgets.size();			// 离开始有多远
    //nth_element(begin, begin + goalOffset, end,		// 找到质量值为
    //    qualityCompare);		// 75%的Widget
    //...						// begin + goalOffset现在指向
    //    // 质量等级为75%的Widget


}

void CSTLAlgorithmTester::test_partial_sort()
{
    //partial_sort(widgets.begin(),				// 把最好的20个元素
    //    widgets.begin() + 20,		// （按顺序）放在widgets的前端
    //    widgets.end(),
    //    qualityCompare);

}

void CSTLAlgorithmTester::test_partial_sum()
{

}

void CSTLAlgorithmTester::test_sort()
{
    std::vector<int> intVector;
    for (int i = 1; i< 10; i+= 2)
    {
        intVector.push_back(i);
    }
    for (int i = 0; i< 10; i+=2)
    {
        intVector.push_back(i);
    }
    for_each(intVector.begin(),intVector.end(),PrintIntItem);
    CPPUNIT_ASSERT(*intVector.begin() == 1);  //插入的第一个是1
    CPPUNIT_ASSERT(*intVector.rbegin() == 8); //插入的最后一个是8

    sort(intVector.begin(),intVector.end());  //使用vector中默认的 less 方法排序(升序)
    CPPUNIT_ASSERT(*intVector.begin() == 0);  //第一个是0
    CPPUNIT_ASSERT(*intVector.rbegin() == 9); //最后一个是9

    sort(intVector.begin(),intVector.end(),std::greater<int>());//使用 greater 进行排序（降序）
    CPPUNIT_ASSERT(*intVector.begin() == 9);  //第一个是9
    CPPUNIT_ASSERT(*intVector.rbegin() == 0); //最后一个是0

    sort(intVector.rbegin(),intVector.rend(),std::greater<int>());// 使用 greater 从最后开始排序 (升序)
    CPPUNIT_ASSERT(*intVector.begin() == 0);  //第一个是0
    CPPUNIT_ASSERT(*intVector.rbegin() == 9); //最后一个是9

#if 0
    std::set<int> intSet;
    //编译错误 -- set 和 map 是采用红黑树，自动排好序的
    //sort(intSet.begin(), intSet.end(), std::less<int>());

    std::map<int,std::string> mapSet;
    sort(mapSet.begin(), mapSet.end(), std::less<int>());
#endif
}

void CSTLAlgorithmTester::test_unique_copy()
{

}

void CSTLAlgorithmTester::test_min_max_element()
{
    std::vector<int> intVector(10);
    //1 ~ 10
    std::generate(intVector.begin(), intVector.end(), sequence_generator<int>(1,1));
    
    //随机打乱顺序
    std::random_shuffle(intVector.begin(), intVector.end());

    std::vector<int>::iterator iterMin = std::min_element(intVector.begin(), intVector.end());
    std::vector<int>::iterator iterMax = std::max_element(intVector.begin(), intVector.end());

    CPPUNIT_ASSERT(1 == *iterMin);
    CPPUNIT_ASSERT(10 == *iterMax);
}

void CSTLAlgorithmTester::test_mismatch()
{

}

void CSTLAlgorithmTester::test_transform()
{
    //把 V1 + V2 => V3
    //transform(V1.begin(), V1.end(), V2.begin(), V3.begin(),plus<double>());

    // 把所有元素增加上41，从data拷贝到d的前端
    //double data[numDoubles];
    //deque<double> d;	
    //transform(data, data + numDoubles,inserter(d, d.begin()),bind2nd(plus<double>(), 41));
    //transform(V1.begin(), V1.end(), V2.begin(), V3.begin(),plus<double>());

    DECLARE_MYTEST_DATA_COUNT_CHECKER(dataChecker,ctDataCount, 0, __FILE__, __LINE__);

    MyTestDataList  tmpTestDataList;
    
    std::transform(m_MyTestDataList.begin(), m_MyTestDataList.end(), 
        std::back_insert_iterator<MyTestDataList>(tmpTestDataList), duplicate_abs_CMytestData());

    //转换以后个数相同，但转换后的结果中 index 都为正
    CPPUNIT_ASSERT_EQUAL(m_MyTestDataList.size(), tmpTestDataList.size());

    std::for_each(tmpTestDataList.begin(), tmpTestDataList.end(), ObjecteDeleter<CMyTestData*>());
    tmpTestDataList.clear();

}

void CSTLAlgorithmTester::test_replace_if()
{
    std::vector<int> intVector1;

    intVector1.reserve(10);
    for (int i = 0; i< 10; i++)
    {
        intVector1.push_back(i);
    }
    CPPUNIT_ASSERT(intVector1[8] == 8);
    CPPUNIT_ASSERT(intVector1[9] == 9);

    //替换大于8的数据(9)为0
    replace_if(intVector1.begin(),intVector1.end(), bind2nd(std::greater<int>(), 8), 0);
    
    CPPUNIT_ASSERT(intVector1.size() == 10); //个数还是10
    for (std::vector<int>::iterator iter = intVector1.begin();iter != intVector1.end();++iter)
    {
        CPPUNIT_ASSERT( *iter <= 8 );   //目前没有大于8的元素了
    }
    CPPUNIT_ASSERT(intVector1[8] == 8);
    CPPUNIT_ASSERT(intVector1[9] == 0);
}

void CSTLAlgorithmTester::test_reverse()
{
    //生成10个随机数
    std::vector<int>    intVector(10);
    std::generate(intVector.begin(), intVector.end(), rand );

    //备份原来的容器
    std::vector<int>    intBackupVector(intVector.begin(), intVector.end());
    
    //对容器进行倒置
    std::reverse(intVector.begin(), intVector.end());

    std::vector<int>::iterator iterIntVector = intVector.begin();
    std::vector<int>::reverse_iterator iterBackVector = intBackupVector.rbegin();
    for (; iterIntVector != intVector.end() 
        && iterBackVector != intBackupVector.rend();
        ++iterIntVector, ++iterBackVector)
    {
        CPPUNIT_ASSERT(*iterIntVector == *iterBackVector);
    }
}

void CSTLAlgorithmTester::test_rotate()
{

}

void CSTLAlgorithmTester::test_equal_range()
{

}

void CSTLAlgorithmTester::test_lower_bound()
{

}

void CSTLAlgorithmTester::test_upper_bound()
{
    //使用 upper_bound 在有序 list 中保持顺序插入新的数据

    //list<CMyTestData> lp;
    //CMyTestData newData;
    //// 在lp中排在newPerson之前或者等价的最后一个对象后面插入newPerson
    //lp.insert(upper_bound(lp.begin(),lp.end(),newData),newData);

}

void CSTLAlgorithmTester::test_merge()
{

}

void CSTLAlgorithmTester::test_heap()
{

}