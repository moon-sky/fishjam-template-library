#ifndef _CXML_ASYNC_PARSER_LISTENER_H_
#define _CXML_ASYNC_PARSER_LISTENER_H_

#include <cybergarage/xml/Parser.h>
#include <cybergarage/upnp/ssdp/SSDPPacket.h>

namespace CyberXML {

class AsyncParseListener{
public:
    virtual void OnAsyncParseResult(Node* pNode, CyberLink::SSDPPacket* pPacket) = 0;
};

}

#endif
