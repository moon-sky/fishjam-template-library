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
		//DUMP_WINDOWS_MSG(__FILE__LINE__, DEFAULT_DUMP_FILTER_MESSAGE, uMsg, wParam, lParam)
		COMMAND_HANDLER_EX(ID_CONTEXT_MENU_COPY, BN_CLICKED, _OnMenuCopyClicked)
		COMMAND_HANDLER_EX(IDC_BTN_RESET_RUNNING_INFO, BN_CLICKED, _OnBtnResetRunningInfoClicked)
		MSG_WM_COMMAND(_OnCommand)
		MSG_WM_CONTEXTMENU(_OnContextMenu)
	END_MSG_MAP()

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
	//BOOL                OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	HRESULT             _DisplayPinInfo(IPin* pPin, HWND hwndListbox, HWND hwndInfo, int& nCurIndex);
	HRESULT				_ClearDisplayPinInfo(HWND hwndListbox);

	void _OnContextMenu(CWindow wnd, CPoint point);
	void _OnCommand(UINT uNotifyCode, int nID, CWindow wndCtl);
	void _OnBtnResetRunningInfoClicked(UINT uNotifyCode, int nID, CWindow wndCtl);
	void _OnMenuCopyClicked(UINT uNotifyCode, int nID, CWindow wndCtl);

	HRESULT _CheckAndReconnectUseMediaType();
	HRESULT _ReConnectUseMediaType(AM_MEDIA_TYPE* pMediaType);

};
