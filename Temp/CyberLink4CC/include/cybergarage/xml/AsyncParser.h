#ifndef _CXML_ASYNC_PARSER_H_
#define _CXML_ASYNC_PARSER_H_

#include <stdio.h>
#include <string>
#include <uhttp/util/Threadpool.h>
#include <cybergarage/xml/Parser.h>
#include <cybergarage/upnp/ssdp/SSDPPacket.h>
#include <cybergarage/xml/AsyncParserListener.h>

namespace CyberXML {

class AsyncParser : public uHTTP::PoolJob{
    AsyncParseListener* m_pListener;
    uHTTP::URL*     m_pUrl;
    Parser m_Parser;
    CyberLink::SSDPPacket m_ssdpPacket;
 public:
  ////////////////////////////////////////////////
  //  Constructor
  ////////////////////////////////////////////////
  AsyncParser(AsyncParseListener* pListener);
  ~AsyncParser();

  void setTarget(uHTTP::URL* url);   //url alloc by new
  void backupSSDPPacket(CyberLink::SSDPPacket* pPacket){
      m_ssdpPacket = *pPacket;
  }
  //PoolJob
  virtual void jobRun();
  virtual void release();
};

}

#endif
