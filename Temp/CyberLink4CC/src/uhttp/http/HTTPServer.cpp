/******************************************************************
*
* uHTTP for C++
*
* Copyright (C) Satoshi Konno 2002-2013
*
* This is licensed under BSD-style license, see file COPYING.
*
******************************************************************/

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#if defined(HAVE_UNAME) || defined(__APPLE__)
#include <sys/utsname.h>
#endif

#include <uhttp/http/HTTPServer.h>
#include <uhttp/http/HTTPServerThread.h>

#include <sstream>

using namespace std;
using namespace uHTTP;

////////////////////////////////////////////////
//  Constructor
////////////////////////////////////////////////
  
HTTPServer::HTTPServer() {
  m_pServerSock = NULL;
}

HTTPServer::~HTTPServer() {
  stop();
  close();
}

////////////////////////////////////////////////
//  ServerSocket
////////////////////////////////////////////////

bool HTTPServer::bind(int port, const std::string &addr) {
  if (m_pServerSock != NULL)
    return true;
    
  m_pServerSock = new ServerSocket();
  if (m_pServerSock->bind(port, addr) == false) {
    delete m_pServerSock;
    m_pServerSock = NULL;
    return false;
  }
  if (m_pServerSock->listen() == false) {
    delete m_pServerSock;
    m_pServerSock = NULL;
    return false;
  }
  return true;
}

bool HTTPServer::open(int port, const std::string &addr) {
  return bind(port, addr);
}

bool HTTPServer::close() {
  if (m_pServerSock == NULL)
    return true;
  m_pServerSock->close();
  m_pServerSock = NULL;
  return true;
}

bool HTTPServer::accept(uHTTP::Socket *socket) {
  if (m_pServerSock == NULL)
    return false;
  if (m_pServerSock->accept(socket) == false)
    return false;
  m_pServerSock->setTimeout(HTTP::DEFAULT_TIMEOUT * 1000);
  return true;
}

bool HTTPServer::isOpened() {
  return (m_pServerSock != NULL) ? true : false;
}

////////////////////////////////////////////////
//  run
////////////////////////////////////////////////

void HTTPServer::run() {
  if (isOpened() == false)
    return;

  while (isRunnable() == true) {
    Socket *sock = new Socket();
    if (accept(sock) == false) {
      delete sock;
      continue;
    }
    ServerSocket *m_pServerSock = getServerSock();
    if (m_pServerSock == NULL) {
      delete sock;
      continue;
    }

    HTTPServerThread *httpServThread = new HTTPServerThread(this, sock);
    httpServThread->start();
  }
}

////////////////////////////////////////////////
//  start
////////////////////////////////////////////////

bool HTTPServer::start() {
  return Thread::start();
}

////////////////////////////////////////////////
//  stop
////////////////////////////////////////////////

bool HTTPServer::stop() {
	close();
	return Thread::stop();
}

////////////////////////////////////////////////
//  Server Functions
////////////////////////////////////////////////

const char *uHTTP::GetServerName(string &buf) {
  buf = "";
  string osName = "Platform";
  string osVer = "1.0";
#if defined(WIN32) && !defined(ITRON)
  OSVERSIONINFO verInfo;
  ZeroMemory(&verInfo, sizeof(OSVERSIONINFO));
  verInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  if (GetVersionEx(&verInfo)) {
    osName = "Windows";
    ostringstream verBuf;
    verBuf << verInfo.dwMajorVersion << "." << verInfo.dwMinorVersion;
    osVer = verBuf.str();
  }
#elif defined(BTRON)
  T_VER verInfo;
  if (get_ver(&verInfo) == 0) {
    osName = "BTRON";
    char verStr[32];
    sprintf(verStr, "%hd", verInfo.spver);
    osVer = verStr;
  }
#elif defined(ITRON)
  osName = "uITRON";
  osVer = "4.0";
#elif defined(TENGINE)
  osName = "T-Engine";
  osVer = "1.0";
#elif defined(HAVE_UNAME) || defined(__APPLE__)
  struct utsname unameBuf;
  if (uname(&unameBuf) == 0) {
    osName = unameBuf.sysname;
    osVer = unameBuf.release;
  }
#endif

  buf = osName;
  buf += "/";
  buf += osVer;
  buf += " ";
  buf += uHTTP::LIBRARY_NAME;
  buf += "/";
  buf += uHTTP::LIBRARY_VERSION;
  
  return buf.c_str();
}
