#ifndef _CUTIL_THREAD_POOL_H_
#define _CUTIL_THREAD_POOL_H_

#include <uhttp/util/Mutex.h>
#include <uhttp/util/Thread.h>
#include <list>

namespace uHTTP {

class PoolJob{
public:
    virtual void jobRun() = 0;
};

class ThreadPool {
public:
    ThreadPool(int nThreadCount);
    ~ThreadPool();

private:
    Thread*                 m_pThreads;
    std::list<PoolJob*>     m_JobsList;
    Mutex                   m_mutex;
};

}

#endif
