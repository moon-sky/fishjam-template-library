/******************************************************************
*
*	MediaServer for CyberLink
*
*	Copyright (C) Satoshi Konno 2003
*
*	File : MediaServer.cpp
*
*	04/03/04
*		- first revision.
*	05/19/04
*		- Changed the header include order for Cygwin.
*	02/24/06
*		- Added setName() and getName().
*
******************************************************************/

#include <uhttp/util/Debug.h>
#include <cybergarage/upnp/media/server/MediaServer.h>

#include <string>

using namespace std;
using namespace CyberLink;
using namespace uHTTP;

////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////
	
const char *MediaServer::DEVICE_TYPE = "urn:schemas-upnp-org:device:MediaServer:1";

const int MediaServer::DEFAULT_HTTP_PORT = 38520;

const char *MediaServer::DESCRIPTION = 
"<?xml version=\"1.0\" encoding=\"utf-8\"?> \n"
"<root xmlns=\"urn:schemas-upnp-org:device-1-0\"> \n"
"   <specVersion> \n"
"      <major>1</major> \n"
"      <minor>0</minor> \n"
"   </specVersion> \n"
"   <device> \n"
"      <deviceType>urn:schemas-upnp-org:device:MediaServer:1</deviceType> \n"
"      <friendlyName>Cyber Garage Media Server</friendlyName> \n"
"      <manufacturer>Cyber Garage</manufacturer> \n"
"      <manufacturerURL>http://www.cybergarage.org</manufacturerURL> \n"
"      <modelDescription>CyberGarage</modelDescription> \n"
"      <modelName>Cyber Garage Media Server</modelName> \n"
"      <modelNumber>1.0</modelNumber> \n"
"      <modelURL>http://www.cybergarage.org</modelURL> \n"
"      <serviceList> \n"
"         <service> \n"
"            <serviceType>urn:schemas-upnp-org:service:ContentDirectory:1</serviceType> \n"
"            <serviceId>urn:upnp-org:serviceId:urn:schemas-upnp-org:service:ContentDirectory</serviceId> \n"
"            <SCPDURL>/service/ContentDirectory1.xml</SCPDURL> \n"
"            <controlURL>/service/ContentDirectory_control</controlURL> \n"
"            <eventSubURL>/service/ContentDirectory_event</eventSubURL> \n"
"         </service> \n"
"         <service> \n"
"            <serviceType>urn:schemas-upnp-org:service:ConnectionManager:1</serviceType> \n"
"            <serviceId>urn:upnp-org:serviceId:urn:schemas-upnp-org:service:ConnectionManager</serviceId> \n"
"            <SCPDURL>/service/ConnectionManager1.xml</SCPDURL> \n"
"            <controlURL>/service/ConnectionManager_control</controlURL> \n"
"            <eventSubURL>/service/ConnectionManager_event</eventSubURL> \n"
"         </service> \n"
"      </serviceList> \n"
"   </device> \n"
"</root> \n";
	
////////////////////////////////////////////////
// Constructor
////////////////////////////////////////////////
	
	
MediaServer::MediaServer() : Device()
{
	// Device Configuration
	loadDescription(MediaServer::DESCRIPTION);

	// Netwroking initialization		
	UPnP::SetEnable(UPnP::USE_ONLY_IPV4_ADDR);
	string firstIf;
	GetHostAddress(0, firstIf);
	setInterfaceAddress(firstIf.c_str());
	setHTTPPort(DEFAULT_HTTP_PORT);

	// Servce Configuration
	m_pConnectionMgr = new ConnectionManager(this);
	m_pContentDir = new ContentDirectory(this);
		
	Service *pServConnectionMgr = getService(ConnectionManager::SERVICE_TYPE);
	pServConnectionMgr->loadSCPD(ConnectionManager::DESCRIPTION);
	pServConnectionMgr->setActionListener(getConnectionManager());
	pServConnectionMgr->setQueryListener(getConnectionManager());

	Service *pServContentDir = getService(ContentDirectory::SERVICE_TYPE);
	pServContentDir->loadSCPD(ContentDirectory::DESCRIPTION);
	pServContentDir->setActionListener(getContentDirectory());
	pServContentDir->setQueryListener(getContentDirectory());
}

MediaServer::~MediaServer()
{
	delete m_pConnectionMgr;
	delete m_pContentDir;
	stop();		
}

////////////////////////////////////////////////
// HttpRequestListner (Overridded)
////////////////////////////////////////////////
	
uHTTP::HTTP::StatusCode MediaServer::httpRequestRecieved(uHTTP::HTTPRequest *httpReq)
{
    if (Debug::isOn())
    {
        std::string strHttpReq;
        httpReq->toString(strHttpReq);
        LogInfo("MediaServer::httpRequestRecieved, httpReq=%s\n", strHttpReq.c_str());
    }
    
    string uri;
    httpReq->getURI(uri);
	if (uri.find(ContentDirectory::CONTENT_EXPORT_URI) != string::npos) {
		getContentDirectory()->contentExportRequestRecieved(httpReq);

        return httpReq->returnOK();
	}
			 
	return Device::httpRequestRecieved(httpReq);
}
