#include "StdAfx.h"
#include "PocoThreadTester.h"

#include "Poco/Activity.h"
#include "Poco/ActiveMethod.h"
#include "Poco/ActiveResult.h"

#include "Poco/ScopedLock.h"
#include "Poco/Mutex.h"
#include "Poco/RWLock.h"
#include "Poco/Thread.h"
#include "Poco/ThreadPool.h"
#include "poco/ThreadLocal.h"
#include "Poco/Timer.h"

void CPocoThreadTester::test_Locker()
{
    Poco::FastMutex m_mutex;    //定义成成员变量，本处使用局部变量代替(仅是示例)

    //两种声明范围锁的方式
    Poco::ScopedLock<Poco::FastMutex> locker(m_mutex);
    Poco::FastMutex::ScopedLock anotherLocker(m_mutex);  
}

//线程的处理逻辑，一般来说其子类会包含Thread变量，并可以支持 start/stop 等操作来控制工作线程的逻辑
class CTestPocoThreadRunnable : public Poco::Runnable
{
public:
    virtual void run()
    {
        //Poco::Logger logger;
        Poco::Int32 iIndex = 5;
        while (iIndex-- > 0)
        {
            std::ostringstream os;
            os << "CTestPocoThreadRunnable " << iIndex;
        }
    }
};

void CPocoThreadTester::test_Thread()
{
    Poco::Thread thread;
    CTestPocoThreadRunnable testRunnable;
    thread.start(testRunnable);
    thread.join();
}

void CPocoThreadTester::test_ThreadPool()
{
    Poco::ThreadPool threadPool;
}

class TimerExample  
{  
public:  
    void onTimer(Poco::Timer& timer)  
    {  
        std::ostringstream os;
        os << "onTimer called." << std::endl;
    }
};

void CPocoThreadTester::test_Timer()
{
    TimerExample te;  
    Poco::Timer timer(250, 500); // fire after 250ms, repeat every 500ms  
    timer.start(Poco::TimerCallback<TimerExample>(te, &TimerExample::onTimer));  
    Poco::Thread::sleep(5000);  
    timer.stop();  
}


class ActivityExample  
{  
public:  
    ActivityExample(): _activity(this,   
        &ActivityExample::runActivity)  
    {}  
    void start()  
    {  
        _activity.start();  
    }  
    void stop()  
    {  
        _activity.stop(); // request stop  
        _activity.wait(); // wait until activity actually stops  
    }  
protected:  
    void runActivity()  
    {  
        while (!_activity.isStopped())  
        {  
            std::cout << "Activity running." << std::endl;  
            Poco::Thread::sleep(200);  
        }  
    }  
private:  
    Poco::Activity<ActivityExample> _activity;  
};  

void CPocoThreadTester::test_Activity()
{
    ActivityExample example;  
    example.start();  
    Poco::Thread::sleep(2000);  
    example.stop();  
}

class ActiveAdder  
{  
public:  
    //ActiveAdder拥有一个ActiveMethod对象，在构造时对add进行实例化，传入了业务调用对象实例的指针，调用对象函数的入口地址
    ActiveAdder():  
      add(this, &ActiveAdder::addImpl)  
      {}  
      Poco::ActiveMethod<int, std::pair<int, int>, ActiveAdder> add;  
private:  
    int addImpl(const std::pair<int, int>& args)  
    {  
        return args.first + args.second;  
    }  
};  
void CPocoThreadTester::test_ActiveMethod()
{
    ActiveAdder adder;  

    //本行代码立刻返回的，但sum此时并没有得到真实的运行结果, 线程在后面异步执行
    Poco::ActiveResult<int> sum = adder.add(std::make_pair(1, 2));  

    //sum.wait()返回，真实的运算结果才被放置于sum中
    sum.wait();  

    std::cout << sum.data() << std::endl;  
}