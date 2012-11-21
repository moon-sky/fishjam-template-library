#pragma once

class ITranscodeCallback
{
public:

private:
	DWORD_PTR	m_param;
};


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
