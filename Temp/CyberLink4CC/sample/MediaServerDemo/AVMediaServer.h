#pragma once

#include <cybergarage/upnp/CyberLink.h>
#include <cybergarage/upnp/media/server/MediaServer.h>
#include <cybergarage/upnp/media/server/directory/file/FileDirectory.h>


class IAVMediaServerCallback{

};

class CAVMediaServer : public CyberLink::MediaServer
{
public:
	CAVMediaServer(IAVMediaServerCallback* pCallback);
	~CAVMediaServer(void);

	void DumpInfo();
private:
	IAVMediaServerCallback* m_pCallback;
};
