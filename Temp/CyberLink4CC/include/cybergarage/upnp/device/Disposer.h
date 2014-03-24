/******************************************************************
*
*  CyberLink for C++
*
*  Copyright (C) Satoshi Konno 2002-2003
*
*  File: Disposer.h
*
*  Revision;
*
*  01/06/04
*    - first revision
*
******************************************************************/

#ifndef _CLINK_DISPOSER_H_
#define _CLINK_DISPOSER_H_

#include <uhttp/util/Thread.h>
#include <uhttp/util/TimeUtil.h>

namespace CyberLink {
class ControlPoint;

class Disposer : public uHTTP::Thread {
  ////////////////////////////////////////////////
  //  Constructor
  ////////////////////////////////////////////////

 public:
  Disposer(ControlPoint *ctrlp) {
    setControlPoint(ctrlp);
  }
  
  ////////////////////////////////////////////////
  //  Member
  ////////////////////////////////////////////////

private:
  
  ControlPoint *controlPoint;
  uHTTP::CTimeUtil     m_timeUtil;
 public:
  void setControlPoint(ControlPoint *ctrlp) {
    controlPoint = ctrlp;
  }
  
  ControlPoint *getControlPoint() {
    return controlPoint;
  }

  ////////////////////////////////////////////////
  //  Thread
  ////////////////////////////////////////////////
  
 public:
   virtual void run();
   virtual bool stop();
};

}

#endif

