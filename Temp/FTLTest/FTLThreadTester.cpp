#include "StdAfx.h"
#include "FTLThreadTester.h"
#include <string>

CString g_strAllocOrder;
CString g_strFreeOrder;
static const int g_pData[] = {7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0};
//static const int g_pData[] = {7,0,1,2,3,4,5,6,7,8,9,0,5,3,2,1,9,7,5,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0};

void CFTLThreadTester::test_CFAutoLock()
{
    {
        CFCriticalSection section;
        {
            CFAutoLock<CFCriticalSection> lock(&section);
        }
    }
    {
        CFEvent evt(TRUE,FALSE);
        {
            CFAutoLock<CFEvent> lock(&evt);
        }
    }
    
}

DWORD CFTLThreadTester::TestThreadProc(void* pParam)
{
    CFTLThreadTester* pThis = static_cast<CFTLThreadTester*>(pParam);
    DWORD dwCount = 0;
    FTL::FTLThreadWaitType waitType = FTL::ftwtTimeOut;
    while (waitType == FTL::ftwtTimeOut && dwCount < 10)
    {
        dwCount++;
        waitType = pThis->m_TestThread.SleepAndCheckStop(100);
        FTLTRACE(TEXT("In CFTLThreadTester::TestThreadProc , dwCount = %d\n"),dwCount);
    }
    FTLTRACE(TEXT("Thread CFTLThreadTester::TestThreadProc End.\n"));

    return 0;
}
void CFTLThreadTester::test_CFThread()
{
    BOOL bRet = FALSE;

    API_VERIFY(m_TestThread.Start(TestThreadProc,this));
    
    API_VERIFY(m_TestThread.Wait(INFINITE,TRUE));
    API_VERIFY(m_TestThread.Stop());
}

static LONG g_ThreadPoolCount = 0;

class CMyJob : public FTL::CFJobBase<DWORD_PTR>
{
public:
    virtual BOOL Run()
    {
        for (DWORD_PTR i = 0; i < m_JobParam ; i++)
        {
            InterlockedIncrement(&g_ThreadPoolCount);
            Sleep(10);
        }
		return TRUE;
    }
    virtual VOID OnFinalize(IN BOOL isWaiting)
    {
        delete this;
    }
};

void CFTLThreadTester::test_CFThreadPool()
{
    const DWORD_PTR JobCount = 10;
    const DWORD_PTR IncCount = 5;

    {
        g_ThreadPoolCount = 0;
        CFThreadPool<DWORD_PTR> threadPool(NULL);
        threadPool.Start(3, 6);

        for (DWORD_PTR i = 0; i< JobCount; i ++)
        {
			CMyJob* pMyJob = new CMyJob();
			pMyJob->m_JobParam = IncCount;
            threadPool.SubmitJob(pMyJob, NULL);
        }

        Sleep(1000); //测试等待线程自动 SubtractThread
        threadPool.StopAndWait(FTL_MAX_THREAD_DEADLINE_CHECK);

        CPPUNIT_ASSERT(g_ThreadPoolCount == IncCount * JobCount); //JobCount 个作业，每个作业加 IncCount 次
    }
    {
        g_ThreadPoolCount = 0;
        CFThreadPool<DWORD_PTR> threadPool(NULL);
        threadPool.Start(1,4);

        for (DWORD_PTR i = 0; i< JobCount; i ++)
        {
			CMyJob* pMyJob = new CMyJob();
			pMyJob->m_JobParam = IncCount;

            threadPool.SubmitJob(pMyJob, NULL);
        }

        Sleep(50);  //测试提前退出
        threadPool.StopAndWait(FTL_MAX_THREAD_DEADLINE_CHECK);

        CPPUNIT_ASSERT(g_ThreadPoolCount <= IncCount * JobCount); //JobCount 个作业，每个作业加 IncCount 次

    }
}


DWORD CFTLThreadTester::ProducerThreadFun(void* pParam)
{
    CFTLThreadTester* pThis = static_cast<CFTLThreadTester*>(pParam);
    for (LONG index = 0; index < 5; index++)
    {
        CMyTestData* pData = new CMyTestData(index,FALSE);
        FTLTRACE(TEXT("ProducerThread Write pData->Index = %d, CurrentTotalCount = %d\n"), 
            pData->GetCurrentIndex(),CMyTestData::GetTestDataCount());
        if (pThis->m_MyTestDataQueue.Append(pData,INFINITE) == ftwtContinue)
        {

        }
        else
        {
            delete pData;
        }
        pThis->m_ThreadProducer.SleepAndCheckStop(200);
    }

	for (LONG index = 6; index < 10; index++)
	{
		CMyTestData* pData = new CMyTestData(index,FALSE);
		FTLTRACE(TEXT("ProducerThread Write pData->Index = %d, CurrentTotalCount = %d\n"), 
			pData->GetCurrentIndex(),CMyTestData::GetTestDataCount());
		{
			CFAutoQueueOperator<CMyTestData*> queueOperator(&pThis->m_MyTestDataQueue,pData,TRUE,INFINITE);
            if (queueOperator.HaveGotQueue())  //成功获取，并能够加入
            {
                //Donothing
            }
            else
            {
                delete pData;
            }
		}
		pThis->m_ThreadProducer.SleepAndCheckStop(200);
	}

    //传入要求退出的元素
    CMyTestData *pQuitData = new CMyTestData(-1,FALSE);
    if (pThis->m_MyTestDataQueue.Append(pQuitData,INFINITE) != ftwtContinue)
    {
        delete pQuitData;
    }
    
    FTLTRACE(TEXT("Now Producer Thread End.\n"));
    return 0;
}

DWORD CFTLThreadTester::ResumerThreadFun(void* pParam)
{
    CFTLThreadTester* pThis = static_cast<CFTLThreadTester*>(pParam);
    CMyTestData* pData = NULL;
    while (ftwtContinue == pThis->m_MyTestDataQueue.Remove(pData,INFINITE))
    {
        LONG dwCurrentIndex = pData->GetCurrentIndex();
        if (dwCurrentIndex == -1) //Quit
        {
            delete pData;
            break;
        }
        pThis->m_ThreadResumer.SleepAndCheckStop(500);
        delete pData;

        FTLTRACE(TEXT("ResumerThread  Read  pData->Index = %d, CurrentTotalCount = %d\n"),
            dwCurrentIndex,CMyTestData::GetTestDataCount());
    }
	{
		CFAutoQueueOperator<CMyTestData*> queueOperator(&pThis->m_MyTestDataQueue,pData,FALSE,100);
	}

    //清除
    while (pThis->m_MyTestDataQueue.RemoveAfterStop(pData))
    {
        delete pData;
    }

    FTLTRACE(TEXT("Now Resumer Thread End, CurrentTotalCount = %d\n"),CMyTestData::GetTestDataCount());
    return 0;
}

void CFTLThreadTester::test_CFProducerResumerQueue()
{
    m_ThreadProducer.Start(ProducerThreadFun,this);
    m_ThreadResumer.Start(ResumerThreadFun,this);

    m_ThreadProducer.Wait(INFINITE,TRUE);
    m_ThreadResumer.Wait(INFINITE,TRUE);
	

	m_ThreadProducer.Start(ProducerThreadFun,this);
	Sleep(1000);
    m_ThreadResumer.Start(ResumerThreadFun,this);
    Sleep(10);
    m_MyTestDataQueue.Stop();  //停止Queue

	m_ThreadProducer.Wait(INFINITE,TRUE);
	m_ThreadResumer.Wait(INFINITE,TRUE);

}

void CFTLThreadTester::test_CFProducerResumerQueue_ResolveRelease()
{
    //测试保留Slot值 -- 
    CPPUNIT_ASSERT(m_MyTestDataQueue.GetCapability() == MAX_QUEUE_COUNT);
    CPPUNIT_ASSERT(m_MyTestDataQueue.GetElementCount() == 0);  //Queue中没有元素
    CPPUNIT_ASSERT(m_MyTestDataQueue.GetReserveSlotCount() == 1);   //初始值是1

    CPPUNIT_ASSERT(m_MyTestDataQueue.ReserveSlot(1,INFINITE) == ftwtContinue);   //可以保留成功

    DWORD dwUseSlot = m_MyTestDataQueue.GetCapability() - m_MyTestDataQueue.GetReserveSlotCount();

    CPPUNIT_ASSERT(dwUseSlot == 0);  //都保留了，可用Slot为0，将不能再加入Element
    CMyTestData data(999);
    CPPUNIT_ASSERT(m_MyTestDataQueue.Append(&data,100) == ftwtTimeOut);      //

    CPPUNIT_ASSERT(m_MyTestDataQueue.ReleaseSlot(MAX_QUEUE_COUNT));     //全部释放
    CPPUNIT_ASSERT(m_MyTestDataQueue.GetReserveSlotCount() == 0);

    CPPUNIT_ASSERT(m_MyTestDataQueue.Append(&data,1000) == ftwtContinue);      //此时能成功加入
    CMyTestData* pTempData = NULL;
    CPPUNIT_ASSERT(m_MyTestDataQueue.Remove(pTempData,1000) == ftwtContinue); //能成功取出
    CPPUNIT_ASSERT(pTempData == &data);         //取出的就是之前放入的

    CPPUNIT_ASSERT(m_MyTestDataQueue.ReserveSlot(1,1000) == ftwtContinue);       //恢复保留Slot为1
}

class CSampleItemCache : public CFItemCacheT<int,int*,CSampleItemCache>
{
public:
    CSampleItemCache():CFItemCacheT<int,int*,CSampleItemCache>(4)
    {

    }
    //覆盖父类的函数
    BOOL GetRealItem(int key,int*& value)
    {
        value = new int;
        *value = key;
        g_strAllocOrder.AppendFormat(_T("%d"),key);
        //TRACE(_T("inGetRealItem key = %d, *value=%d\n"),key,*value);

        return TRUE;
    }
    void FreeItem(int key,int*& value)
    {
        g_strFreeOrder.AppendFormat(_T("%d"),key);
        //TRACE(_T("In FreeItem,key=%d,*value=%d\n"),key,*value); 
        delete value;
        value = NULL;
    }
};

class CSampleStringItemCache : public CFItemCacheT<std::string,int,CSampleStringItemCache>
{
public:
    CSampleStringItemCache():CFItemCacheT<std::string,int,CSampleStringItemCache>(4)
    {

    }
    //覆盖父类的函数
    BOOL GetRealItem(std::string key,int& value)
    {
        value = (int)key.size();
        g_strAllocOrder.AppendFormat(_T("%s"),key.c_str());
        //TRACE(_T("inGetRealItem key = %d, *value=%d\n"),key,*value);
        return TRUE;
    }
    void FreeItem(std::string key,int& value)
    {
        UNREFERENCED_PARAMETER(value);
        g_strFreeOrder.AppendFormat(_T("%s"),key.c_str());
    }
};

void CFTLThreadTester::test_ItemCacheByFIFO()
{
    g_strAllocOrder = _T("");
    g_strFreeOrder = _T("");

    {
        CSampleItemCache cache;
        cache.SetClearMode(CSampleItemCache::clearByFIFO);
        int *value = 0;

        for (int index = 0; index< _countof(g_pData); index++)
        {
            cache.GetItemByKey(g_pData[index],value);
        }
    }

    //{7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0}
    TRACE(_T("In ClearByFIFO: AllocOrder=%s,FreeOrder=%s\n"),g_strAllocOrder,g_strFreeOrder);
    CPPUNIT_ASSERT(_T("7012340127") == g_strAllocOrder);
    CPPUNIT_ASSERT(_T("7012340127") == g_strFreeOrder);

    {
        CSampleStringItemCache cache;
        cache.SetClearMode(CSampleStringItemCache::clearByFIFO);
        int value = 0;
        cache.GetItemByKey("fishjam",value);
        cache.GetItemByKey("study",value);
        cache.GetItemByKey("stl",value);
    }

}

void CFTLThreadTester::test_ItemCacheByLRU()
{
    g_strAllocOrder = _T("");
    g_strFreeOrder = _T("");

    {
        CSampleItemCache cache;
        cache.SetClearMode(CSampleItemCache::clearByLRU);
        int *value = 0;
        for (int index = 0; index< _countof(g_pData); index++)
        {
            cache.GetItemByKey(g_pData[index],value);
        }
    }

    //{7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0}
    TRACE(_T("In ClearByLRU: AllocOrder=%s,FreeOrder=%s\n"),g_strAllocOrder,g_strFreeOrder);
    CPPUNIT_ASSERT(_T("70123417") == g_strAllocOrder);
    CPPUNIT_ASSERT(_T("71430127") == g_strFreeOrder);
}

void CFTLThreadTester::test_ItemCacheByRandom()
{
    g_strAllocOrder = _T("");
    g_strFreeOrder = _T("");

    {
        CSampleItemCache cache;
        cache.SetClearMode(CSampleItemCache::clearByRandom);
        int *value = 0;
        for (int index = 0; index< _countof(g_pData); index++)
        {
            cache.GetItemByKey(g_pData[index],value);
        }
    }

    //{7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0}
    TRACE(_T("In testItemCacheByRandom: AllocOrder=%s,FreeOrder=%s\n"),g_strAllocOrder,g_strFreeOrder);
    //In testItemCacheByRandom: AllocOrder=70123040212010,FreeOrder=01020212010347
    CPPUNIT_ASSERT(g_strAllocOrder.GetLength() == g_strFreeOrder.GetLength());//两者长度相同（分配的都进行了释放）
}

void CFTLThreadTester::test_FMemCacheT()
{
    LONG oldCount = CMyTestData::GetTestDataCount();
    {
        CFMemCacheT<CMyTestData>   memCache(5,10);
        std::vector<CMyTestData*>  allDataPtrArray;

        CPPUNIT_ASSERT(oldCount + 5 == CMyTestData::GetTestDataCount());

        //先获取3个
        for (int i = 0; i < 3; i++)
        {
            CMyTestData* pData = memCache.Get();
            allDataPtrArray.push_back(pData);
        }
        CPPUNIT_ASSERT(oldCount + 5 == CMyTestData::GetTestDataCount());


        //再获取7个
        for (int i = 0; i < 7; i++)
        {
            CMyTestData* pData = memCache.Get();
            allDataPtrArray.push_back(pData);
        }
        CPPUNIT_ASSERT(oldCount + 10 == CMyTestData::GetTestDataCount());

        CMyTestData* pNewData = memCache.Get();
        CPPUNIT_ASSERT(pNewData == NULL);
        CPPUNIT_ASSERT(oldCount + 10 == CMyTestData::GetTestDataCount());

        //释放回去
        for (std::vector<CMyTestData*>::iterator iter = allDataPtrArray.begin();
            iter != allDataPtrArray.end(); ++iter)
        {
            memCache.Release(*iter);
        }
        CPPUNIT_ASSERT(oldCount + 10 == CMyTestData::GetTestDataCount());
    }
    //超出Cache的生存期，全部释放
    CPPUNIT_ASSERT(oldCount  == CMyTestData::GetTestDataCount());

}

typedef CFThreadSafeWrapper<CMyTestData, CFCriticalSection> ThreadSafeTestData;

//线程安全的调用方式
DWORD __stdcall ThreadSafeTestDataIncProc( LPVOID lpThreadParameter)
{
    ThreadSafeTestData& testData = *(ThreadSafeTestData*)lpThreadParameter;
    for (int i = 0; i < 10; i++)
    {
        testData->LongTimeIncreatment(10);
        Sleep(20);
    }
    return 0;
}

DWORD __stdcall ThreadSafeTestDataDecProc( LPVOID lpThreadParameter)
{
    ThreadSafeTestData& testData = *(ThreadSafeTestData*)lpThreadParameter;
    for (int i = 0; i < 10; i++)
    {
        testData->LongTimeDecreatment(20);
        Sleep(10);
    }
    return 0;
}

//原生方式调用 -- 非线程安全
DWORD __stdcall NavtiveTestDataIncProc( LPVOID lpThreadParameter)
{
    CMyTestData& testData = *(CMyTestData*)lpThreadParameter;
    for (int i = 0; i < 10; i++)
    {
        testData.LongTimeIncreatment(10);
        Sleep(10);
    }
    return 0;
}

DWORD __stdcall NavtiveTestDataDecProc( LPVOID lpThreadParameter)
{
    CMyTestData& testData = *(CMyTestData*)lpThreadParameter;
    for (int i = 0; i < 10; i++)
    {
        testData.LongTimeDecreatment(20);
        Sleep(20);
    }
    return 0;
}

void CFTLThreadTester::test_threadSafeWrapper()
{
    {
        //使用封装后线程安全的方式调用 -- 通过日志可以看到 Enter <-> Leave 是匹配的
		DECLARE_MYTEST_DATA_COUNT_CHECKER(countChecker, ctDataCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(constructChecker, ctConstructCount, 1, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(destructChecker, ctDestructCount, 1, __FILE__, __LINE__);

        ThreadSafeTestData testData(new CMyTestData(100), TRUE);
        CPPUNIT_ASSERT( testData->GetCurrentIndex() == 100 );

        CFThread<> threadInc, threadDec;
        threadInc.Start(ThreadSafeTestDataIncProc, (void*)&testData, TRUE);
        threadDec.Start(ThreadSafeTestDataDecProc, (void*)&testData, TRUE);

        threadInc.Wait();
        threadDec.Wait();
        CPPUNIT_ASSERT( testData->GetCurrentIndex() == 100 );

#if 0
        CFAutoLock<CFCriticalSection> lockObj(&getLockObject(testData));
        getImpObject<CMyTestData, CFCriticalSection>(testData)->LongTimeIncreatment(100);
        CPPUNIT_ASSERT( testData->GetCurrentIndex() == 101 );
#endif 
    }

    {
        //字节使用原始的方式调用（非线程安全） -- 通过日志可以看到 Enter/Leave 不匹配
		DECLARE_MYTEST_DATA_COUNT_CHECKER(countChecker, ctDataCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(constructChecker, ctConstructCount, 1, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(destructChecker, ctDestructCount, 1, __FILE__, __LINE__);

        CMyTestData testData(100);
        CPPUNIT_ASSERT( testData.GetCurrentIndex() == 100 );

        CFThread<> threadInc, threadDec;
        threadInc.Start(NavtiveTestDataIncProc, (void*)&testData, TRUE);
        threadDec.Start(NavtiveTestDataDecProc, (void*)&testData, TRUE);

        threadInc.Wait();
        threadDec.Wait();

        FTLTRACE(TEXT("After Navtive Test Data Change ,Index=%d\n"), testData.GetCurrentIndex());
        //最后的结果往往不相等 -- 极少的情况下也会相等
        CPPUNIT_ASSERT( testData.GetCurrentIndex() != 100 );
    }
}
