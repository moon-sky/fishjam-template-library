#pragma once

#include <cppunit/extensions/HelperMacros.h>

/**********************************************************************************************
* 多线程封装
*   Thread -- 可以设置线程名字
*     使用静态函数 Thread::current() 可以获取到每个线程对象Thread的指针(实现方式为TLS)
*   ThreadPool(线程池) -- 功能不强
*     1.无法独立控制任务
*     2.向ThreadPool中增加任务,当没有空闲线程时，会抛出 NoThreadAvailableException 异常，而不是放入队列等待
*  
*   thread-local 存储 -- Poco::ThreadLocal + ThreadLocalStorage + TLSSlot
*   同步原子(Poco::Mutex, Poco::ScopedLock, Poco::Event,  Poco::Semaphore, Poco::RWLock), 
*     Mutex，Semaphore，Event，Scopelock，ReadWriteLock
*   Active Object(主动对象) -- 使用自己线程运行自己的成员函数,可以共享线程，Poco支持两种主动成员函数。
*      Activity -- 用在业务不需要返回值和无参数的成员函数时
*      ActiveMethod + ActiveResult(异步返回结果) -- 用在业务需要返回值和参数的成员函数时
*   Timer(定时器) -- 
**********************************************************************************************/

class CPocoThreadTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CPocoThreadTester );
    CPPUNIT_TEST( test_Locker );
    CPPUNIT_TEST( test_Thread );
    CPPUNIT_TEST( test_ThreadPool );
    CPPUNIT_TEST( test_Timer );
    CPPUNIT_TEST( test_Activity );
    CPPUNIT_TEST( test_ActiveMethod );
    CPPUNIT_TEST_SUITE_END();

    DECLARE_DEFAULT_TEST_CLASS(CPocoThreadTester);
private:
    void test_Locker();
    void test_Thread();
    void test_ThreadPool();
    void test_Timer();
    void test_Activity();
    void test_ActiveMethod();
};
