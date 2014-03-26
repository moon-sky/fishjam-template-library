#pragma once

#include <cybergarage/upnp/CyberLink.h>
#include <cybergarage/upnp/media/server/MediaServer.h>
#include <cybergarage/upnp/media/server/directory/file/FileDirectory.h>


class IAVMediaServerCallback{

};

class CAVMediaServer : public CyberLink::MediaServer
{
public:
    static const char *DEFAULT_PREFERENCE_FILENAME;
    static const char *PREFERENCE_CYBERGARAGE;
    static const char *PREFERENCE_MEDIASERVER;
    static const char *PREFERENCE_CONTENT_DIRECTORY;
    static const char *PREFERENCE_DIRECTORY_LIST;
    static const char *PREFERENCE_DIRECTORY;
    static const char *PREFERENCE_NAME;
    static const char *PREFERENCE_PATH;

public:
	CAVMediaServer(IAVMediaServerCallback* pCallback);
	~CAVMediaServer(void);

	void DumpInfo();
public:
    bool loadPreferences(const char *fname);
    bool savePreferences(const char *fname);
    bool AddDirectory(const char* name, const char* path);
private:
	IAVMediaServerCallback* m_pCallback;
};
