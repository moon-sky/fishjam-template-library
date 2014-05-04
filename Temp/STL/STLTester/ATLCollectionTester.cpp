#include "StdAfx.h"
#include "ATLCollectionTester.h"

void CATLCollectionTester::test_CAtlArray()
{
    //AtlArray 在扩展容量时,不会拷贝原来的数据 -- 性能比 vector 要高, 见 CPerformanceTester::test_container_holder
    DECLARE_MYTEST_DATA_COUNT_CHECKER(dataCountChecker, ctDataCount, 0, __FILE__, __LINE__);
    DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCountChecker, ctConstructCount, 4, __FILE__, __LINE__);
    typedef CAtlArray<CMyTestData, CElementTraits< CMyTestData > >  MyDataArray;
    MyDataArray dataArray;

    for (int i = 1; i<= 4; i++)
    {
        dataArray.Add(CMyTestData(1));
    }
}

void CATLCollectionTester::test_CAtlList()
{
	FTLASSERT(FALSE);
	//AddTail, Find, RemoveAt, GetHeadPosition, GetNext
}

void CATLCollectionTester::test_CAtlMap()
{
    DECLARE_MYTEST_DATA_COUNT_CHECKER(dataCountChecker,ctDataCount, 0, __FILE__, __LINE__);
    DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCountChecker, ctConstructCount, 4, __FILE__, __LINE__);
    DECLARE_MYTEST_DATA_COUNT_CHECKER(ctDestructCountChecker, ctDestructCount, 4, __FILE__, __LINE__);

    
    typedef CAtlMap<int, CMyTestData*>  MyDataMap;
    MyDataMap dataMap;

    for (int i = 1; i <= 4; i++)
    {
        dataMap.SetAt(i, new CMyTestData(i));
    }
    
    POSITION pos = dataMap.GetStartPosition();
    while (NULL != pos)
    {
        MyDataMap::CPair* pDataPair = dataMap.GetNext(pos);
        CMyTestData* pData = pDataPair->m_value;
        delete pData;
    }
    dataMap.RemoveAll();
}