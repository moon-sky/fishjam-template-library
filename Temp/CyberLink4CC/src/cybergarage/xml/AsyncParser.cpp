#include <stdio.h>
#include <string.h>
#include <string>

#include <cybergarage/xml/AsyncParser.h>

using namespace std;
using namespace CyberXML;
using namespace uHTTP;
using namespace uHTTP;

AsyncParser::AsyncParser(AsyncParseListener* pListener){
    m_pListener = pListener;
    m_pUrl = NULL;
}

AsyncParser::~AsyncParser(){
    if (m_pUrl){
        delete m_pUrl;
        m_pUrl = NULL;
    }
    
}
void AsyncParser::setTarget(URL* url){
    if (m_pUrl){
        delete m_pUrl;
    }
    m_pUrl = url;
}


void AsyncParser::jobRun(){
    if (m_pUrl){
        Node *pNode = m_Parser.parse(m_pUrl);
        if (m_pListener){
            m_pListener->OnAsyncParseResult(pNode, &m_ssdpPacket);
        }
    }
}

void AsyncParser::release(){
    delete this;
}