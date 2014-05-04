#include "StdAfx.h"
#include "STLIteratorTester.h"

#include <iterator>
#include "STLTesterUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//如果bUseBackPlusIter为true的话，会使用 后置递增迭代器 的方式，否则使用 erase 的返回值更新迭代器


template<typename ContainerT>
void testSeriesContainer(ContainerT container, bool bUseBackPlusIter)
{
    for (int i = 10; i < 20; i++)
    {
        container.push_back(ContainerT::value_type(i));
    }
    CPPUNIT_ASSERT(container.size() == 10);

    int iCount = 0;
    typename ContainerT::iterator iter = container.begin();
    while (iter != container.end())
    {
        if (iCount % 2 == 0)
        {
            if (bUseBackPlusIter)
            {
                //使用后置递增的方式：
                container.erase(iter++);
            }
            else
            {
                //删除后返回下一个位置，不用再++
                iter = container.erase(iter);
            }
        }
        else
        {
            ++iter;
        }
        iCount++;
    }

    CPPUNIT_ASSERT(container.size() == 5);
    for (ContainerT::iterator iter = container.begin();
        iter != container.end(); ++iter)
    {
        CPPUNIT_ASSERT(*iter % 2 == 1);
    }
}

template<typename ContainerT>
void testSetContainer(ContainerT container, bool bUseBackPlusIter)
{
    for (int i = 10; i < 20; i++)
    {
        container.insert(ContainerT::value_type(i));
    }
    CPPUNIT_ASSERT(container.size() == 10);

    int iCount = 0;
    typename ContainerT::iterator iter = container.begin();
    while (iter != container.end())
    {
        if (iCount % 2 == 0)
        {
            if (bUseBackPlusIter)
            {
                //使用后置递增的方式：
                container.erase(iter++);
            }
            else
            {
                //删除后返回下一个位置，不用再++
                iter = container.erase(iter);
            }
        }
        else
        {
            ++iter;
        }
        iCount++;
    }

    CPPUNIT_ASSERT(container.size() == 5);
    for (ContainerT::iterator iter = container.begin();
        iter != container.end(); ++iter)
    {
        CPPUNIT_ASSERT(*iter % 2 == 1);
    }
}

template<typename ContainerT>
void testMapContainer(ContainerT container, bool bUseBackPlusIter)
{

    for (int i = 10; i < 20; i++)
    {
        container[ContainerT::key_type(i)] = ContainerT::mapped_type(i);
    }
    CPPUNIT_ASSERT(container.size() == 10);

    int iCount = 0;
    typename ContainerT::iterator iter = container.begin();
    while (iter != container.end())
    {
        if (iCount % 2 == 0)
        {
            if (bUseBackPlusIter)
            {
                //使用后置递增的方式：
                container.erase(iter++);
            }
            else
            {
                //删除后返回下一个位置，不用再++
                iter = container.erase(iter);
            }
        }
        else
        {
            ++iter;
        }
        iCount++;
    }

    CPPUNIT_ASSERT(container.size() == 5);
    for (ContainerT::iterator iter = container.begin();
        iter != container.end(); ++iter)
    {
        CPPUNIT_ASSERT((*iter).first % 2 == 1);
    }
}

void CSTLIteratorTester::test_deque_loopdelete()
{
    std::deque<int> intVector;
    CPPUNIT_ASSERT(intVector.empty());
    testSeriesContainer(intVector, false);
    CPPUNIT_ASSERT(intVector.empty());

#if 0
    //deque不能使用 后置递增迭代器
    testSeriesContainer(intVector, true);
#endif
}

void CSTLIteratorTester::test_vector_loopdelete()
{
    std::vector<int> intVector;
    CPPUNIT_ASSERT(intVector.empty());
    testSeriesContainer(intVector, false);
    CPPUNIT_ASSERT(intVector.empty());
#if 0
    //deque不能使用 后置递增迭代器
    testSeriesContainer(intVector, true);
#endif
}


void CSTLIteratorTester::test_list_loopdelete()
{
    std::list<int> intVector;
    CPPUNIT_ASSERT(intVector.empty());
    testSeriesContainer(intVector, false);
    CPPUNIT_ASSERT(intVector.empty());
    testSeriesContainer(intVector, true);
}

void CSTLIteratorTester::test_set_loopdelete()
{
    std::set<int> intVector;
    CPPUNIT_ASSERT(intVector.empty());
    testSetContainer(intVector, false);
    CPPUNIT_ASSERT(intVector.empty());
    testSetContainer(intVector, true);
}

void CSTLIteratorTester::test_map_loopdelete()
{
    std::map<TestType,TestType> intVector;
    CPPUNIT_ASSERT(intVector.empty());
    testMapContainer(intVector, false);
    CPPUNIT_ASSERT(intVector.empty());
    testMapContainer(intVector, true);
}

void CSTLIteratorTester::test_bad_loopdelete()
{
    typedef std::list<TestType> TypeContainer;
    TypeContainer container;
    for (int i = 10; i < 20; i++)
    {
        container.push_back(i);
    }
    CPPUNIT_ASSERT(container.size() == 10);

#if 0
    for (TypeContainer::iterator iter = container.begin();
        iter != container.end(); ++iter)
    {
        if (*iter % 2 == 0)
        {
            //不能使用这种方式，否则迭代器会出现错误
            container.erase(iter);
        }
    }
    CPPUNIT_ASSERT(container.size() == 5);
#endif
}

void CSTLIteratorTester::test_better_loopdelete()
{
    typedef std::vector<TestType> VecContainer;
    VecContainer container;
    for (int i = 0; i < 10; i++)
    {
        container.push_back(TestType(i));
    }
    CPPUNIT_ASSERT( container.size()==10 );

    //注意：因为 remove_if 以后，不要的元素被移到了最后，因此还需要 erase,
    //通常 使用 erase(remove_if(xxx), end() ) 的方式一步执行完全部逻辑

    //删除 2 < x 的元素，剩下的是 0~2
    VecContainer::iterator iterReturn = std::remove_if(container.begin(),container.end(), bind1st(std::less<int>(),2));
    CPPUNIT_ASSERT(container.size() == 10);

    container.erase(iterReturn,container.end());

    CPPUNIT_ASSERT( container.size() == 3 );

	{
		DECLARE_MYTEST_DATA_COUNT_CHECKER(countChecker,ctDataCount, 0, __FILE__, __LINE__);
		DECLARE_MYTEST_DATA_COUNT_CHECKER(constructChecker,ctConstructCount, 10, __FILE__, __LINE__);
		DECLARE_MYTEST_DATA_COUNT_CHECKER(destructChecker,ctDestructCount, 10, __FILE__, __LINE__);

		//删除map中的元素
		typedef std::map<int , CMyTestData*> MapDataContainer;
		MapDataContainer mapDataContainer;
		for (LONG index = 0; index < 10; index++)
		{
			mapDataContainer[index] = new CMyTestData(index);
		}

		//保留需要删除的部分 -- 
		//MapDataContainer tmpMapContainer;
		//std::remove_copy_if(mapDataContainer.begin(), mapDataContainer.end(), 
		//	inserter(tmpMapContainer, tmpMapContainer.end()),
		//	std::bind1st(std::less<LONG>(), 3));
		
		for_each(mapDataContainer.begin(), mapDataContainer.end(),
			PairDeleter<LONG, CMyTestData*>());

	}

    //std::remove_copy_if(mapContainer.begin(), mapContainer.end(),
    // inserter(tmpMapContainer.begin(), tmpMapContainer.end()),
    // equal_to())
}

void CSTLIteratorTester::test_ostream_iterator()
{
    std::ostringstream os;
    std::vector<int>    intVector;
    for (int i = 1; i < 10; i++)
    {
        intVector.push_back(i);
    }

    std::copy(intVector.begin(), intVector.end(), std::ostream_iterator<int>(os, ","));
    const std::string& strInfo = os.str();

    //有什么方法可以不让最后一个 "," 输出？
    CPPUNIT_ASSERT( strInfo == "1,2,3,4,5,6,7,8,9,");
}

void CSTLIteratorTester::test_front_inserter()
{
    std::vector<int>    intVector(10);
    std::list<int>      intList;

    std::generate(intVector.begin(), intVector.end(), rand );
    CPPUNIT_ASSERT(intVector.size() == 10);

    //通过使用front_inserter迭代器对象，将vector中的元素依次插入到链表前面 -- 相当于 vector 元素的倒序
    std::copy(intVector.begin(), intVector.end(), std::front_inserter(intList));
    CPPUNIT_ASSERT(intList.size() == intVector.size());

    STLTESTER_ASSERT_CONTAINER_ELEMENT_EQUAL(intVector.begin(), intVector.end(), intList.rbegin(), intList.rend());
}

