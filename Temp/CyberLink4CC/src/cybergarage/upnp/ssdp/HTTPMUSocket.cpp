/******************************************************************
*
*  CyberLink for C++
*
*  Copyright (C) Satoshi Konno 2002-2003
*
*  File: HTTPMUSocket.cpp
*
*  Revision;
*
*  07/01/03
*    - first revision
*  11/19/04
*    - Theo Beisch <theo.beisch@gmx.de>
*    - Changed send() to set the TTL as 4.
*
******************************************************************/

#include <time.h>
#include <uhttp/net/HostInterface.h>
#include <cybergarage/upnp/ssdp/HTTPMUSocket.h>

using namespace CyberLink;
using namespace uHTTP;

////////////////////////////////////////////////
//  Constructor
////////////////////////////////////////////////
  
HTTPMUSocket::HTTPMUSocket() {
}

HTTPMUSocket::HTTPMUSocket(const std::string &addr, int port, const std::string &bindAddr) {
  open(addr, port, bindAddr);
}

////////////////////////////////////////////////
//  Destructor
////////////////////////////////////////////////

HTTPMUSocket::~HTTPMUSocket() {
  close();
}

////////////////////////////////////////////////
//  open/close
////////////////////////////////////////////////

bool HTTPMUSocket::open(const std::string &addr, int port, const std::string &bindAddr) {
  m_ssdpMultiGroup.setAddress(addr);
  m_ssdpMultiGroup.setPort(port);
  //ssdpMultiIf.setAddress(bindAddr);

  const char *msockBindAddr = bindAddr.c_str();
  if (m_ssdpMultiSock.bind(port, msockBindAddr) == false)
    return false;
  if (m_ssdpMultiSock.joinGroup(addr, bindAddr) == false) {
    m_ssdpMultiSock.close();
    return false;
  }    

  return true;
}

bool HTTPMUSocket::close() {
  if (m_ssdpMultiSock.close() == false)
    return false;
  return true;
}

////////////////////////////////////////////////
//  reveive
////////////////////////////////////////////////

bool HTTPMUSocket::send(const std::string &msg, const std::string &bindAddr, int bindPort) {
  MulticastSocket msock;
  if ((0 < bindAddr.length()) && (0 < bindPort))
      msock.bind(bindPort, bindAddr);
  DatagramPacket dgmPacket(msg, &m_ssdpMultiGroup);
  // Thnaks for Tho Beisch (11/09/04)
  msock.setTimeToLive(4);
  msock.send(&dgmPacket);
  return true;
}

////////////////////////////////////////////////
//  reveive
////////////////////////////////////////////////

SSDPPacket *HTTPMUSocket::receive() {
  m_recvPacket.setLocalAddress(getLocalAddress());
  size_t nRecv = m_ssdpMultiSock.receive(m_recvPacket.getDatagramPacket());
  if (nRecv == 0)
    return NULL;
  m_recvPacket.setTimeStamp(time(NULL));
  return &m_recvPacket;
}
