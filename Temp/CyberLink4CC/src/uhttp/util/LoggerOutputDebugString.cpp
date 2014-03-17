#include <uhttp/util/LoggerOutputDebugString.h>

#if defined(WIN32) && !defined(ITRON)

uHTTP::LoggerOutputDebugString::LoggerOutputDebugString() {
  setFD(NULL);
  setMask(
      LoggerTarget::TRACE  |
      LoggerTarget::DBG    |
      LoggerTarget::INFO
      );
}

uHTTP::LoggerOutputDebugString::~LoggerOutputDebugString() {

}

bool uHTTP::LoggerOutputDebugString::outputMessage(const char *lineMessage) {
    OutputDebugStringA(lineMessage);
  return true;
}

#endif //