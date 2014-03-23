#include "StdAfx.h"
#include "AVMediaServer.h"

CAVMediaServer::CAVMediaServer(IAVMediaServerCallback* pCallback)
{
	m_pCallback = pCallback;
}

CAVMediaServer::~CAVMediaServer(void)
{
}

void CAVMediaServer::DumpInfo()
{
	FTLTRACEA("MediaServer DumpInfo\n");
}
