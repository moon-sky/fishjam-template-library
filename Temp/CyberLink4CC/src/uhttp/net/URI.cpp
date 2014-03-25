/******************************************************************
*
* uHTTP for C++
*
* Copyright (C) Satoshi Konno 2002-2013
*
* This is licensed under BSD-style license, see file COPYING.
*
******************************************************************/


#include <uhttp/net/URI.h>
#include <uhttp/util/StringUtil.h>
#include <uhttp/util/Log.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>

const char *uHTTP::URI::HTTP = "http";
const char *uHTTP::URI::HTTPS = "https";

int uHTTP::URI::HTTP_PORT = 80;
int uHTTP::URI::HTTPS_PORT = 443;


const char *uHTTP::URI::PROTOCOL_DELIM = "://";
const char *uHTTP::URI::USER_DELIM = "@";
const char *uHTTP::URI::COLON_DELIM = ":";
const char *uHTTP::URI::SLASH_DELIM = "/";
const char *uHTTP::URI::SBLACET_DELIM = "[";
const char *uHTTP::URI::EBLACET_DELIM = "]";
const char *uHTTP::URI::SHARP_DELIM = "#";
const char *uHTTP::URI::QUESTION_DELIM = "?";

////////////////////////////////////////////////
//  uHTTP::URI::URI
////////////////////////////////////////////////

uHTTP::URI::URI() {
    m_port = URI_KNKOWN_PORT;
}

uHTTP::URI::URI(const std::string &value) {
  m_port = URI_KNKOWN_PORT;
  setString(value);
}

////////////////////////////////////////////////
// setString
////////////////////////////////////////////////

void uHTTP::URI::setString(const std::string &value) {
  m_uriStr = value;

  // Protocol
  size_t idx = m_uriStr.find(PROTOCOL_DELIM);
  if (idx != std::string::npos) {
    size_t protocolStrLen = strlen(PROTOCOL_DELIM);
    // Thanks for Jay Deen (03/26/04)
    m_protocol = m_uriStr.substr(0, idx/* + protocolStrLen*/);
    idx += protocolStrLen;
  }
  else
    idx = 0;

  // User (Password)
  size_t atIdx = m_uriStr.find(USER_DELIM, idx);
  if (atIdx != (int)std::string::npos) {
    std::string userPassStr = m_uriStr.substr(idx, atIdx - idx);
    size_t colonIdx = userPassStr.find(COLON_DELIM);
    if (colonIdx != std::string::npos) {
      m_user = userPassStr.substr(0, colonIdx);
      m_password = userPassStr.substr(colonIdx + 1, userPassStr.length() - colonIdx -1);
    }
    else
      m_user = userPassStr;
    idx = atIdx + 1;
  }

  // Host (Port)
  size_t shashIdx = m_uriStr.find(SLASH_DELIM, idx);
  if (shashIdx != std::string::npos)
    m_host = m_uriStr.substr(idx, shashIdx - idx);
  else
    m_host = m_uriStr.substr(idx, m_uriStr.length() - idx);
  size_t colonIdx = m_host.rfind(COLON_DELIM);
  size_t eblacketIdx = m_host.rfind(EBLACET_DELIM);
  if (colonIdx != std::string::npos && ((eblacketIdx == std::string::npos) || (eblacketIdx < colonIdx))) {
    std::string hostStr = m_host;
    m_host = hostStr.substr(0, colonIdx);
    if (0 < m_host.length()) {
      if (m_host.at(0) == '[' && m_host.at(m_host.length()-1) == ']')
        m_host = m_host.substr(1, colonIdx-2);
    }
    std::string portStr = hostStr.substr(colonIdx + 1, hostStr.length() - colonIdx -1);
    m_port = atoi(portStr.c_str());
  }
  else {
    m_port = URI_KNKOWN_PORT;
    if (isHTTP())
      m_port = HTTP_PORT;
    else if (isHTTPS())
      m_port = HTTPS_PORT;
  }
  
  if (shashIdx == (int)std::string::npos)
    return;
  idx = shashIdx/* + 1*/;

  // Path (Query/Fragment)
  m_path = m_uriStr.substr(idx, m_uriStr.length() - idx);
  m_target = m_path;
  size_t sharpIdx = m_path.find(SHARP_DELIM);
  if (sharpIdx != std::string::npos) {
    std::string pathStr = m_path;
    m_path = pathStr.substr(0, sharpIdx);
    m_fragment = pathStr.substr(sharpIdx + 1, pathStr.length() - sharpIdx -1);
  }
  size_t questionIdx = m_path.find(QUESTION_DELIM);
  if (questionIdx != std::string::npos) {
    std::string pathStr = m_path;
    m_path = pathStr.substr(0, questionIdx);
    m_query = pathStr.substr(questionIdx + 1, pathStr.length() - questionIdx -1);
  }
  
}

////////////////////////////////////////////////
// IsAbsoluteURI
////////////////////////////////////////////////

bool uHTTP::URI::IsAbsoluteURI() {
  if (0 < m_protocol.length())
    return true;
  return false;
}

////////////////////////////////////////////////
// print
////////////////////////////////////////////////

void uHTTP::URI::print() {
    std::ostringstream os;
    os << "URI=" << m_uriStr
        << ",protocol=" << m_protocol
        << ",user=" << m_user
        << ",password=" << m_password
        << ",host=" << m_host
        << ",port=" << m_port
        << ",path" << m_path
        << ",query" << m_query
        << ",fragment=" << m_fragment;

    LogInfo("%s\n", os.str());
}

