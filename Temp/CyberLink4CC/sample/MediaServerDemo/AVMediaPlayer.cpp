#include "StdAfx.h"
#include "AVMediaPlayer.h"

using namespace CyberLink;
using namespace uHTTP;

CAVMediaPlayer::CAVMediaPlayer(IAVMediaPlayerCallback* pCallback)
{
	m_pCallback = pCallback;

	addNotifyListener(this);
	addSearchResponseListener(this);
	addEventListener(this);
    addDeviceChangeListener(this);
}

CAVMediaPlayer::~CAVMediaPlayer(void)
{
}

void CAVMediaPlayer::DumpInfo()
{
	DeviceList *devList = this->getDeviceList();
	int devCnt = devList->size();
	int mediaServerCnt = 0;
	for (int n=0; n<devCnt; n++) {
		Device *dev = devList->getDevice(n);
		if (dev->isDeviceType(MediaServer::DEVICE_TYPE) == false)
			continue;
		FTLTRACEA("[%d],Name=%s,LeaseTime=%d,ElpaseTime=%d", n, dev->getFriendlyName(), dev->getLeaseTime() , dev->getElapsedTime());
		//PrintContentDirectory(mplayer, dev);
		mediaServerCnt++;
	}		
	if (mediaServerCnt == 0) {
		FTLTRACE(TEXT("MediaServer is not found\n"));
	}

}

//HTTP::StatusCode CAVMediaPlayer::httpRequestRecieved( HTTPRequest *httpReq )
//{
//	throw std::exception("The method or operation is not implemented.");
//}
//

void CAVMediaPlayer::deviceNotifyReceived( SSDPPacket *ssdpPacket )
{
	ssdpPacket->print("CAVMediaPlayer::deviceNotifyReceived\n");
}

void CAVMediaPlayer::eventNotifyReceived( const std::string &uuid, long seq, const std::string &varName, const std::string &value )
{
	FTLTRACEA("eventNotifyReceived:uuid=%s, seq=%ld, varName=%s, value=%s\n", uuid.c_str(), seq, varName.c_str(), value.c_str());
}

void CAVMediaPlayer::deviceSearchResponseReceived( SSDPPacket *ssdpPacket )
{
	ssdpPacket->print("CAVMediaPlayer::deviceSearchResponseReceived\n");
}

void CAVMediaPlayer::deviceAdded(Device *dev){
   m_pCallback->OnDeviceAdded(dev);
}
void CAVMediaPlayer::deviceRemoved(Device *dev)
{
    m_pCallback->OnDeviceRemoved(dev);
}