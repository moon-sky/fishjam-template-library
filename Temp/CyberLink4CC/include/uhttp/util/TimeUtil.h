/******************************************************************
*
* uHTTP for C++
*
* Copyright (C) Satoshi Konno 2002-2013
*
* This is licensed under BSD-style license, see file COPYING.
*
******************************************************************/


#ifndef _CUTIL_TIMERUTIL_H_
#define _CUTIL_TIMERUTIL_H_

namespace uHTTP {
    long GetCurrentSystemTime();
    float Random();
    
    class CTimeUtil{
    public:
        CTimeUtil();
        ~CTimeUtil();
        void Reset();
        void Stop();

        bool Wait(long mtime);
        bool WaitRandom(long mtime);
    private:
#if defined(WIN32) && !defined(ITRON)
        void* m_hEventStop;
#endif 
    };
}

#endif
