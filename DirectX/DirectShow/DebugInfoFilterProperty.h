#pragma once

#include "resource.h"
#include "DirectShowDemo_h.h"

class CDebugInfoFilterProperty 
	: public CBasePropertyPage
	, public CWinDataExchange<CDebugInfoFilterProperty>
{
public:
	static CUnknown * WINAPI CreateInstance(LPUNKNOWN lpunk, HRESULT *phr);

	virtual HRESULT OnConnect(IUnknown *pUnknown);
	virtual HRESULT OnDisconnect();
	virtual HRESULT OnActivate();
	virtual HRESULT OnDeactivate();
	virtual HRESULT OnApplyChanges();
	virtual INT_PTR OnReceiveMessage(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

public:
	BEGIN_DDX_MAP(CDebugInfoFilterProperty)
		DDX_CONTROL_HANDLE(IDC_LIST_INPUT, m_listInput)
		DDX_CONTROL_HANDLE(IDC_LIST_OUTPUT, m_listOutput)
		DDX_CONTROL_HANDLE(IDC_INPUT_PIN_INFO, m_wndInputInfo)
		DDX_CONTROL_HANDLE(IDC_OUTPUT_INFO, m_wndOutputInfo)
	END_DDX_MAP()

	BEGIN_MSG_MAP_EX(CDebugInfoFilterProperty)
		MSG_WM_COMMAND(OnCommand)
	END_MSG_MAP()

	void OnCommand(UINT uNotifyCode, int nID, CWindow wndCtl);

	HWND GetDlgItem(int nID) const throw()
	{
		ATLASSERT(::IsWindow(m_hwnd));
		return ::GetDlgItem(m_hwnd, nID);
	}

private:
	CDebugInfoFilterProperty(IUnknown * pUnk, HRESULT * phr);
	void SetDirty(BOOL bDirty);
private:
	int                 m_nInputIndex;
	int                 m_nOutputIndex;
	IFilterDebugInfo*   m_pFilterInfo;
	IPin*               m_pInputConnectedPin;
	IPin*               m_pOutputConnectedPin;
	CListBox			m_listInput;
	CListBox			m_listOutput;
	CWindow				m_wndInputInfo;
	CWindow				m_wndOutputInfo;
	FilterDebugParam    m_FilterDebugParam;
	BOOL                OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	HRESULT				_UpdateListboxHorizontalExtent(HWND hwndListbox);
	HRESULT             _DisplayPinInfo(IPin* pPin, HWND hwndListbox, HWND hwndInfo, int& nCurIndex);
	HRESULT				_ClearDisplayPinInfo(HWND hwndListbox);
};
