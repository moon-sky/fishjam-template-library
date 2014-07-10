#pragma once

#include <cppunit/extensions/HelperMacros.h>
#include <ftlThread.h>
#include <ftlThreadPool.h>
/************************************************************************
* 
************************************************************************/

#define MAX_QUEUE_COUNT 2
class CFTLThreadTester : public CPPUNIT_NS::TestFixture
{
public:
    CFTLThreadTester():
      m_MyTestDataQueue(MAX_QUEUE_COUNT,1)
    {

    }
    ~CFTLThreadTester()
    {

    }

    virtual void setUp(){};
    virtual void tearDown(){};

public:
    CPPUNIT_TEST_SUITE( CFTLThreadTester );

    CPPUNIT_TEST( test_CFAutoLock );
    CPPUNIT_TEST( test_CFThread );
    CPPUNIT_TEST( test_CFThreadPool );
    CPPUNIT_TEST( test_CFProducerResumerQueue );
    CPPUNIT_TEST( test_CFProducerResumerQueue_ResolveRelease );
    CPPUNIT_TEST( test_FMemCacheT );
    CPPUNIT_TEST( test_ItemCacheByFIFO );
    CPPUNIT_TEST( test_ItemCacheByLRU );
    CPPUNIT_TEST( test_ItemCacheByRandom );

    CPPUNIT_TEST( test_threadSafeWrapper );

    CPPUNIT_TEST_SUITE_END();

    void test_CFAutoLock();
    void test_CFProducerResumerQueue();
    void test_CFProducerResumerQueue_ResolveRelease();

    void test_FMemCacheT();

    void test_CFThread();
    void test_CFThreadPool();

    void test_ItemCacheByFIFO();
    void test_ItemCacheByLRU();
    void test_ItemCacheByRandom();

    void test_threadSafeWrapper();

    //DECLARE_DEFAULT_TEST_CLASS(CFTLThreadTester);
private:
    FTL::CFThread<>      m_TestThread;
    static DWORD __stdcall TestThreadProc(void* pParam);
private:
    //测试生产者/消费者线程队列
    FTL::CFProducerResumerQueue<CMyTestData*> m_MyTestDataQueue;
    FTL::CFThread<FTL::CFWin32ThreadTraits>      m_ThreadProducer;
    FTL::CFThread<FTL::CFCRTThreadTraits>      m_ThreadResumer;
    static DWORD __stdcall ProducerThreadFun(void* pParam);
    static DWORD __stdcall ResumerThreadFun(void* pParam);
};
