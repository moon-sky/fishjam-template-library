#pragma once
#include "VideoRecordInclude.h"
#include "NRecordGraph.h"

//class CNCaptureGraphBuilder;

class CRecordNotifyProxy
{
public:
	CRecordNotifyProxy(IMediaEvent* pMediaEvent);
	~CRecordNotifyProxy();

private:
	static			DWORD WINAPI s_NotifyHandlerProc(LPVOID pParam);
	DWORD			_InnerNotifyHandlerProc();
	BOOL			_HandleEvent();
private:
	//CNCaptureGraphBuilder*	m_pNCaptureBuilder;
	IRecordGraphNotify*				m_pNotify;
	HANDLE							m_hEventNotify;
	CComPtr<IMediaEvent>			m_spMediaEvent;

	FTL::CFThread<DefaultThreadTraits>*	m_pThreadNotifyHandler;
};