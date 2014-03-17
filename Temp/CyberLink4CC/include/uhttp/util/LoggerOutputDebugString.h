
#ifndef _UHTTP_LOGGER_OUTPUTDEBUGSTRING_H_
#define _UHTTP_LOGGER_OUTPUTDEBUGSTRING_H_

#include <uhttp/util/LoggerTarget.h>

#if defined(WIN32) && !defined(ITRON)

namespace uHTTP {
class LoggerOutputDebugString : public LoggerTarget {
 public:
  LoggerOutputDebugString();
  ~LoggerOutputDebugString();

  virtual bool outputMessage(const char *lineMessage);

};

}
#endif 

#endif  //_UHTTP_LOGGER_OUTPUTDEBUGSTRING_H_
