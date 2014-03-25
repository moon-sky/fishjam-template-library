/******************************************************************
*
* uHTTP for C++
*
* Copyright (C) Satoshi Konno 2002-2013
*
* This is licensed under BSD-style license, see file COPYING.
*
******************************************************************/

#ifndef _CNET_URI_H_
#define _CNET_URI_H_

#include <string>

namespace uHTTP {
const int URI_KNKOWN_PORT = -1;

class URI {
protected:

  std::string m_uriStr;
  std::string m_target;
  std::string m_protocol;
  std::string m_user;
  std::string m_password;
  std::string m_host;
  int m_port;
  std::string m_path;
  std::string m_query;
  std::string m_fragment;

public:
  
  static const char *HTTP;
  static const char *HTTPS;
  
  static int HTTP_PORT;
  static int HTTPS_PORT;
  
 public:
  static const char *PROTOCOL_DELIM;
  static const char *USER_DELIM;
  static const char *COLON_DELIM;
  static const char *SLASH_DELIM;
  static const char *SBLACET_DELIM;
  static const char *EBLACET_DELIM;
  static const char *SHARP_DELIM;
  static const char *QUESTION_DELIM;
  
 public:
  URI();
  URI(const std::string &uriStr);

  bool IsAbsoluteURI();

  void setString(const std::string &value);

  const char *getSting() {
    return m_uriStr.c_str();
  }
  const char *getTarget(){
      return m_target.c_str();
  }
  void setProtocol(const std::string &value) {
    m_protocol = value;
  }
  
  const char *getProtocol() {
    return m_protocol.c_str();
  }

  bool hasProtocol() {
    return (0 < m_protocol.length());
  }
  
  bool isProtocol(const std::string &protocolPrefix) {
    return (m_protocol.compare(protocolPrefix) == 0) ? true : false;
  }

  bool isHTTP() {
    return isProtocol(HTTP);
  }

  bool isHTTPS() {
    return isProtocol(HTTPS);
  }
  
  void setUser(const std::string &value) {
    m_user = value;
  }
  
  const char *getUser() {
    return m_user.c_str();
  }

  void setPassword(const std::string &value) {
    m_password = value;
  }
  
  const char *getPassword() {
    return m_password.c_str();
  }

  void setHost(const std::string &value) {
    m_host = value;
  }
  
  const char *getHost() {
    return m_host.c_str();
  }

  void setPort(int value) {
    m_port = value;
  }

  int getPort() {
    return m_port;
  }

  void setPath(const std::string &value) {
    m_path = value;
  }
  
  const char *getPath() {
    return m_path.c_str();
  }

  bool hasPath() {
    return (0 < m_path.length());
  }
  
  void setQuery(const std::string &value) {
    m_query = value;
  }
  
  const char *getQuery() {
    return m_query.c_str();
  }

  void setFragment(const std::string &value) {
    m_fragment = value;
  }
  
  const char *getFragment() {
    return m_fragment.c_str();
  }

  bool isValid();
  
  void print();
};

}

#endif
