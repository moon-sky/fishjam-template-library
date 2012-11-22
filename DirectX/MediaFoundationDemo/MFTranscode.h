#pragma once

//#include <codecapi.h>

class ITranscodeCallback
{
public:

private:
	DWORD_PTR	m_param;
};

//struct H264ProfileInfo
//{
//	UINT32  profile;
//	MFRatio fps;
//	MFRatio frame_size;
//	UINT32  bitrate;
//};

//H264ProfileInfo h264_profiles[] = 
//{
//	{ eAVEncH264VProfile_Base, { 15, 1 },       { 176, 144 },   128000 },
//	{ eAVEncH264VProfile_Base, { 15, 1 },       { 352, 288 },   384000 },
//	{ eAVEncH264VProfile_Base, { 30, 1 },       { 352, 288 },   384000 },
//	{ eAVEncH264VProfile_Base, { 29970, 1000 }, { 320, 240 },   528560 },
//	{ eAVEncH264VProfile_Base, { 15, 1 },       { 720, 576 },  4000000 },
//	{ eAVEncH264VProfile_Main, { 25, 1 },       { 720, 576 }, 10000000 },
//	{ eAVEncH264VProfile_Main, { 30, 1 },       { 352, 288 }, 10000000 },
//};

//struct AACProfileInfo
//{
//	UINT32  samplesPerSec;
//	UINT32  numChannels;
//	UINT32  bitsPerSample;
//	UINT32  bytesPerSec;
//	UINT32  aacProfile;
//};
//
//AACProfileInfo aac_profiles[] = 
//{
//	{ 96000, 2, 16, 24000, 0x29}, 
//	{ 48000, 2, 16, 24000, 0x29}, 
//	{ 44100, 2, 16, 16000, 0x29}, 
//	{ 44100, 2, 16, 12000, 0x29}, 
//};

class CMFTranscode
{
public:
	CMFTranscode(void);
	virtual ~CMFTranscode(void);
public:
	HRESULT OpenSrcFile(LPCTSTR pszSrcURL);
	HRESULT EncodeToFile(LPCTSTR pszTargetURL, ITranscodeCallback* pCallback, DWORD_PTR param);
	//TODO: Set Param
protected:

	HRESULT ConfigureAudioOutput();
	HRESULT ConfigureVideoOutput();
	HRESULT ConfigureContainer();

private:
	CComPtr<IMFMediaSession>        m_spMediaSession;
	CComPtr<IMFMediaSource>         m_spMediaSource;
	CComPtr<IMFTopology>            m_spTopology;
	CComPtr<IMFTranscodeProfile>    m_spTranscodeProfile;
};
