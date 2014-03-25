#include <uhttp/util/ThreadPool.h>
#include <ftlBase.h>

using namespace uHTTP;

class JobThread : public Thread{
    ThreadPool* m_pPool;
public:
    JobThread(ThreadPool* pPool){
        m_pPool = pPool;
    }
    virtual void run(){
        FUNCTION_BLOCK_TRACE(0);
        PoolJob* pJob = NULL;
        while (m_pPool->getJob(&pJob)){
            pJob->jobRun();
            pJob->release();
        }
    }
};

ThreadPool::ThreadPool(int nThreadCount, int nMaxWaitJobCount){
    m_nThreadCount = nThreadCount;
	m_nMaxWaitJobCount = nMaxWaitJobCount;
    m_isRunning = false;
    m_pThreads = NULL;
    m_hEventStop = CreateEvent(NULL, TRUE, FALSE, NULL);
    m_hSemaphoreJobToDo = CreateSemaphore(NULL, 0, nMaxWaitJobCount, NULL);
	m_hSemaphoreWaitJobSlot = CreateSemaphore(NULL, nMaxWaitJobCount, nMaxWaitJobCount, NULL);
}

ThreadPool::~ThreadPool(){
    stop();
    close();
}

void ThreadPool::close(){
    if (m_pThreads){
        for (int i = 0; i < m_nThreadCount; i++){
            delete m_pThreads[i];
        }
        delete[] m_pThreads;
        m_pThreads = NULL;
    }
	if (m_hSemaphoreWaitJobSlot)
	{
		CloseHandle(m_hSemaphoreWaitJobSlot);
		m_hSemaphoreWaitJobSlot = NULL;
	}
	
    if (m_hSemaphoreJobToDo){
        CloseHandle(m_hSemaphoreJobToDo);
        m_hSemaphoreJobToDo = NULL;
    }
    if (m_hEventStop){
        CloseHandle(m_hEventStop);
        m_hEventStop = NULL;
    }
}
bool ThreadPool::start(){
    if(m_isRunning){
        return true;
    }
    ResetEvent(m_hEventStop);
    m_pThreads = new Thread*[m_nThreadCount];
    for (int i = 0; i < m_nThreadCount; i++)
    {
        m_pThreads[i] = new JobThread(this);
        m_pThreads[i]->start();
    }
    m_isRunning = true;
    return true;
}

bool ThreadPool::stop(){
    m_isRunning = false;
    SetEvent(m_hEventStop);
    if (m_pThreads){
        for (int i = 0; i < m_nThreadCount; i++){
            m_pThreads[i]->stop();
        }
        m_pThreads = NULL;
    }
    return true;
}

bool ThreadPool::addJob(PoolJob* pJob){
	HANDLE hWaitHandles[] = 
	{
		m_hEventStop,                 //user stop thread pool
		m_hSemaphoreWaitJobSlot,      //there are empty slot
	};
	DWORD dwResult = WaitForMultipleObjects(sizeof(hWaitHandles)/sizeof(hWaitHandles[0]), 
		hWaitHandles, FALSE, INFINITE);
	switch(dwResult)
	{
	case WAIT_OBJECT_0:				//m_hEventStop
		return false;
	case WAIT_OBJECT_0 + 1:			//m_hSemaphoreWaitJobSlot
		break;
	default:
		return false;
	}

    AutoLock lockObj(&m_mutex);
    m_JobsList.push_back(pJob);
    ReleaseSemaphore(m_hSemaphoreJobToDo, 1, NULL);
    return true;
}

bool ThreadPool::getJob(PoolJob** ppOutJob){
    bool bGet = false;
    HANDLE hWaitHandles[] = 
    {
        m_hEventStop,                 //user stop thread pool
        m_hSemaphoreJobToDo,          //there are waiting jobs
    };
    DWORD dwResult = WaitForMultipleObjects(sizeof(hWaitHandles)/sizeof(hWaitHandles[0]), 
        hWaitHandles, FALSE, INFINITE);

    switch(dwResult)
    {
    case WAIT_OBJECT_0:				//m_hEventStop
        return false;
    case WAIT_OBJECT_0 + 1:			//m_hSemaphoreJobToDo
        break;
    default:
        return false;
    }

    AutoLock lockObj(&m_mutex);
    JobContainer::iterator iterBegin = m_JobsList.begin();
    *ppOutJob = *iterBegin;
    m_JobsList.erase(iterBegin);
	ReleaseSemaphore(m_hSemaphoreWaitJobSlot, 1, NULL);
    return true;
}