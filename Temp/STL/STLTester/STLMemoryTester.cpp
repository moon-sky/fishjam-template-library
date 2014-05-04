#include "StdAfx.h"
#include "STLMemoryTester.h"
#include <memory>
#include <iterator>
using std::tr1::shared_ptr;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CSTLMemoryTester::test_auto_ptr()
{
    DECLARE_MYTEST_DATA_COUNT_CHECKER(ConstructCountChecker,ctDataCount, 0, __FILE__, __LINE__);
    {
        //测试超出范围后自动释放
        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataCountChecker,ctDataCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCountChecker,ctConstructCount, 1, __FILE__, __LINE__);
        {
            std::auto_ptr<CMyTestData> pMyTestData(new CMyTestData(1));
            CPPUNIT_ASSERT(pMyTestData.get()->GetCurrentIndex() == 1);
        }//此处以后自动释放
    }

    {
        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataCountChecker,ctDataCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCountChecker,ctConstructCount, 1, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(copyConstructChecker,ctCopyConstructCount, 0, __FILE__, __LINE__); //只有指针的拷贝，不会有拷贝构造函数

        //测试auto_ptr进行赋值时的所有权转移 -- 因为赋值时所有权转移的问题，所以不能用于标准容器
        std::auto_ptr<CMyTestData> pMyData(new CMyTestData(1));
        CPPUNIT_ASSERT(pMyData->GetCurrentIndex() == 1);

        std::auto_ptr<CMyTestData> pMyDataAssign = pMyData;
        CPPUNIT_ASSERT(NULL == pMyData.get()); //所有权转移后，原来的智能指针变为NULL
    }

    {
        //测试reset方法
        LONG lCount = CMyTestData::GetTestDataCount();
        {
            std::auto_ptr<CMyTestData> pMyTestData(new CMyTestData(1));
            CPPUNIT_ASSERT(CMyTestData::GetTestDataCount() == lCount + 1);
            CPPUNIT_ASSERT(pMyTestData.get()->GetCurrentIndex() == 1);

            //使用reset方法后，以前的会释放掉，产生新的
            pMyTestData.reset(new CMyTestData(2));
            CPPUNIT_ASSERT(CMyTestData::GetTestDataCount() == lCount + 1);
            CPPUNIT_ASSERT(pMyTestData.get()->GetCurrentIndex() == 2);

            //使用reset方法，同时不带参数的话，会释放
            pMyTestData.reset();

            CPPUNIT_ASSERT(CMyTestData::GetTestDataCount() == lCount);
            CPPUNIT_ASSERT(pMyTestData.get() == NULL);

        }
        CPPUNIT_ASSERT(CMyTestData::GetTestDataCount() == lCount);
    }

    {
        //测试手工使用 release 释放 -- 不好的使用方式
        LONG lCount = CMyTestData::GetTestDataCount();

        std::auto_ptr<CMyTestData> pMyTestData(new CMyTestData(1));
        CPPUNIT_ASSERT(NULL != pMyTestData.get());
        CPPUNIT_ASSERT(CMyTestData::GetTestDataCount() == lCount + 1);
        CPPUNIT_ASSERT(pMyTestData.get()->GetCurrentIndex() == 1);

        //由于release函数返回以前的指针，因此必须手工释放
        CMyTestData* pTestData = pMyTestData.release();
        CPPUNIT_ASSERT(NULL == pMyTestData.get());
        CPPUNIT_ASSERT(NULL != pTestData);
        CPPUNIT_ASSERT(pTestData->GetCurrentIndex() == 1);
        delete pTestData;  //必须手工释放

        CPPUNIT_ASSERT(CMyTestData::GetTestDataCount() == lCount);
    }
}

void CSTLMemoryTester::test_contain_assign()
{
#ifdef NEED_OPERATOR_FUNC
    typedef std::vector<CMyTestData> DataContainer;
    DataContainer srcDataContainer;
    
    //除了一开始的时候是构造函数，其他时候都是调用拷贝构造
    DECLARE_MYTEST_DATA_COUNT_CHECKER(ConstructCheck,ctConstructCount, 5, __FILE__, __LINE__);
    {
        for (long i = 0; i < 5; i++)
        {
            srcDataContainer.push_back(CMyTestData(i, 0));
        }

        {
            DECLARE_MYTEST_DATA_COUNT_CHECKER(ConstructCheck,ctConstructCount, 0, __FILE__, __LINE__);
            //赋值时会对容器中的每一个元素调用拷贝构造函数 -- 
            DECLARE_MYTEST_DATA_COUNT_CHECKER(CopyConstructCheck,ctCopyConstructCount, 5, __FILE__, __LINE__);
            //超出dstDataContainer的生存周期后释放
            DECLARE_MYTEST_DATA_COUNT_CHECKER(DestructCheck,ctDestructCount, 5, __FILE__, __LINE__);
            DataContainer dstDataContainer;
            dstDataContainer = srcDataContainer;
        }

        {
            DECLARE_MYTEST_DATA_COUNT_CHECKER(ConstructCheck,ctConstructCount, 0, __FILE__, __LINE__);
            //assign时会对容器中的每一个元素调用拷贝构造函数 -- 
            DECLARE_MYTEST_DATA_COUNT_CHECKER(CopyConstructCheck,ctCopyConstructCount, 5, __FILE__, __LINE__);
            //超出dstDataContainer的生存周期后释放
            DECLARE_MYTEST_DATA_COUNT_CHECKER(DestructCheck,ctDestructCount, 5, __FILE__, __LINE__);
            DataContainer dstDataContainer;
            dstDataContainer.assign(srcDataContainer.begin(), srcDataContainer.end());
        }

        {
            DECLARE_MYTEST_DATA_COUNT_CHECKER(ConstructCheck,ctConstructCount, 0, __FILE__, __LINE__);
            DECLARE_MYTEST_DATA_COUNT_CHECKER(CopyConstructCheck,ctCopyConstructCount, 5, __FILE__, __LINE__);
            DECLARE_MYTEST_DATA_COUNT_CHECKER(DestructCheck,ctDestructCount, 5, __FILE__, __LINE__);

            DataContainer dstDataContainer;
            dstDataContainer.reserve(srcDataContainer.size());
            std::copy(srcDataContainer.begin(), srcDataContainer.end(), 
                std::back_insert_iterator<DataContainer>(dstDataContainer));
        }
    }
#endif
}

void CSTLMemoryTester::test_shared_ptr()
{
	//可以将 shared_ptr 的指针放入容器，并且在容器超出范围或 clear 后会自动释放
	DECLARE_MYTEST_DATA_COUNT_CHECKER(dataCountChecker,ctDataCount, 0, __FILE__, __LINE__);
	DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCountChecker,ctConstructCount, 4, __FILE__, __LINE__);
	{
		typedef std::vector<std::tr1::shared_ptr<CMyTestData> >		CMyTestDataSharePtrContainer;
		CMyTestDataSharePtrContainer myTestDataSharedPtrs;
		for (int i = 0; i < 4; i++)
		{
			myTestDataSharedPtrs.push_back(shared_ptr<CMyTestData>(new CMyTestData(i)));
		}
	}//此处以后自动释放

}