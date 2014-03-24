#ifndef _CXML_ASYNC_PARSER_H_
#define _CXML_ASYNC_PARSER_H_

#include <stdio.h>
#include <string>
#include <uhttp/util/Thread.h>
#include <cybergarage/xml/Parser.h>

namespace CyberXML {

class AsyncParser : public uHTTP::Thread  {
 public:
  ////////////////////////////////////////////////
  //  Constructor
  ////////////////////////////////////////////////

  AsyncParser();
  ~AsyncParser();

  void AddParserUrl(uHTTP::URL *url);
};

}

#endif
