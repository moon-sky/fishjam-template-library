#pragma once

#include <cppunit/extensions/HelperMacros.h>

/**********************************************************************************************
* 多线程封装
*   Poco::Thread
*   同步原子(Poco::Mutex, Poco::ScopedLock, Poco::Event,  Poco::Semaphore, Poco::RWLock), 
*   Poco::ThreadPool 类及支持thread-local 存储, 
*   高级别的活动对象(active object) -- 拥有运行于自有线程中的方法的类
*      ActiveMethod + ActiveResult
**********************************************************************************************/

class CPocoThreadTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CPocoThreadTester );
    CPPUNIT_TEST( test_container_holder );
    CPPUNIT_TEST_SUITE_END();

    DECLARE_DEFAULT_TEST_CLASS(CPocoThreadTester);
private:
    void test_container_holder();
};
