#ifndef _CUTIL_THREAD_POOL_H_
#define _CUTIL_THREAD_POOL_H_

#include <uhttp/util/Mutex.h>
#include <uhttp/util/Thread.h>
#include <list>

namespace uHTTP {

class PoolJob{
public:
    virtual void jobRun() = 0;
    virtual void release() = 0;
};

class ThreadPool {
public:
    ThreadPool(int nThreadCount, int nMaxWaitJobCount);
    ~ThreadPool();

    bool start();
    bool stop();
    void close();
public:
    bool addJob(PoolJob* pJob);

//thie is call from JobThread
    bool getJob(PoolJob** ppOutJob);
private:
    int                     m_nThreadCount;
	int						m_nMaxWaitJobCount;
    bool                    m_isRunning;
    Thread**                m_pThreads;

    typedef std::list<PoolJob*> JobContainer;
    JobContainer            m_JobsList;
    Mutex                   m_mutex;
    
    void*                   m_hEventStop;
    void*                   m_hSemaphoreJobToDo;
	void*					m_hSemaphoreWaitJobSlot;
};

}

#endif
