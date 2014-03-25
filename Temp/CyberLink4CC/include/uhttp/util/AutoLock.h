
#ifndef _CUTIL_AUTOLOCK_H_
#define _CUTIL_AUTOLOCK_H_

namespace uHTTP {
template <typename T>
class AutoLock{
public:
    AutoLock(T* pLocker){
        m_pLocker = pLocker;
        m_pLocker->lock();
    }
    ~AutoLock(){
        m_pLocker->unlock();
    }
private:
    T*  m_pLocker;
};

}

#endif
