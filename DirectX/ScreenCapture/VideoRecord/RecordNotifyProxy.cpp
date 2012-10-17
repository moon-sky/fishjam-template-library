#include "stdafx.h"
#include <evcode.h>
#include "RecordNotifyProxy.h"

CRecordNotifyProxy::CRecordNotifyProxy(/*CNCaptureGraphBuilder* pNCaptureBuilder, */IMediaEvent* pMediaEvent)
{
	//m_pNCaptureBuilder = pNCaptureBuilder;
	m_pNotify = NULL;
	m_hEventNotify = NULL;

	m_spMediaEvent = pMediaEvent;
	HRESULT hr = E_FAIL;

	DX_VERIFY(m_spMediaEvent->GetEventHandle((OAEVENT*)&m_hEventNotify));

	BOOL bRet = FALSE;
	m_pThreadNotifyHandler = new FTL::CFThread<DefaultThreadTraits>(NULL, m_hEventNotify);
	API_VERIFY(m_pThreadNotifyHandler->Start(s_NotifyHandlerProc, this, TRUE));
}

CRecordNotifyProxy::~CRecordNotifyProxy()
{
	FTLTRACE(TEXT("Enter CRecordNotifyProxy Destroy\n"));
	
	if (m_pThreadNotifyHandler)
	{
		m_pThreadNotifyHandler->StopAndWait();
		SAFE_DELETE(m_pThreadNotifyHandler);
	}
	m_spMediaEvent.Release();

	FTLTRACE(TEXT("Leave CRecordNotifyProxy Destroy\n"));
}

DWORD CRecordNotifyProxy::s_NotifyHandlerProc(LPVOID pParam)
{
	CRecordNotifyProxy* pThis = static_cast<CRecordNotifyProxy*>(pParam);
	ATLASSERT(pThis);
	DWORD dwResult = pThis->_InnerNotifyHandlerProc();

	return dwResult;
}

DWORD CRecordNotifyProxy::_InnerNotifyHandlerProc()
{
	FTLTRACE(TEXT("Enter _InnerNotifyHandlerProc\n"));
	HRESULT hr = E_FAIL;

	BOOL bQuit = FALSE;
	while (!bQuit && FTL::ftwtContinue == m_pThreadNotifyHandler->GetThreadWaitType(INFINITE))
	{
		bQuit = _HandleEvent();
	}

	FTLTRACE(TEXT("Leave _InnerNotifyHandlerProc\n"));
	return 0;
}

BOOL CRecordNotifyProxy::_HandleEvent()
{
	HRESULT hr = E_FAIL;
	LONG lEvent = 0;
	LONG_PTR lParam1 = 0;
	LONG_PTR lParam2 = 0;
	BOOL bWillQuit = FALSE;

	FTLTRACE(TEXT("Enter CRecordNotifyProxy::_HandleEvent\n"));
	while (hr =  m_spMediaEvent->GetEvent(&lEvent, &lParam1, &lParam2, 1000),  SUCCEEDED(hr))
	{
		FTLTRACE(TEXT("  _HandleEvent, hr=0x%x, Event=%s(%d), lParam1=0x%x, lParam2=0x%x\n"),
			hr, FTL::CFDirectShowEventInfo(lEvent, lParam1, lParam2).GetConvertedInfo(),
			lEvent, lParam1, lParam2);

		m_spMediaEvent->FreeEventParams(lEvent, lParam1, lParam2);

		switch (lEvent)
		{
		case EC_COMPLETE:
			bWillQuit = TRUE;
			break;
		default:
			break;
		}

	}

	FTLTRACE(TEXT("Leave CRecordNotifyProxy::_HandleEvent, bWillQuit=%d\n"), bWillQuit);
	return bWillQuit;
}