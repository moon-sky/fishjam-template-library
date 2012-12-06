#pragma once

#include "resource.h"
#include "ftlWindow.h"

class CGraphInfoPropertyPage 
	: public CBasePropertyPage
	, public CWinDataExchange<CGraphInfoPropertyPage>
{
public:
	static CUnknown * WINAPI CreateInstance(LPUNKNOWN lpunk, HRESULT *phr);

	virtual HRESULT OnConnect(IUnknown *pUnknown);
	virtual HRESULT OnDisconnect();
	virtual HRESULT OnActivate();
	virtual HRESULT OnDeactivate();
	//virtual HRESULT OnApplyChanges();
	virtual INT_PTR OnReceiveMessage(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
public:
	BEGIN_DDX_MAP(CGraphInfoPropertyPage)
		DDX_CONTROL_HANDLE(IDC_STATIC_INTERFACES, m_stInterfaces)
		DDX_CONTROL_HANDLE(IDC_STATIC_MEDIATYPES, m_stMediaTypes)
		DDX_CONTROL_HANDLE(IDC_STATIC_DETAIL, m_stDetail)
		DDX_CONTROL_HANDLE(IDC_LIST_FILTERS, m_listFilters)
		DDX_CONTROL_HANDLE(IDC_LIST_PINS, m_listPins)
		DDX_CONTROL_HANDLE(IDC_LIST_MEDIATYPES, m_listMediaTypes)
		DDX_CONTROL_HANDLE(IDC_LIST_INTERFACES, m_listInterfaces)
		DDX_CONTROL_HANDLE(IDC_EDIT_DETAIL, m_edtDetail)
	END_DDX_MAP()

	BEGIN_MSG_MAP_EX(CGraphInfoPropertyPage)
	//DUMP_WINDOWS_MSG(TEXT("CGraphInfoPropertyPage"), DEFAULT_DUMP_FILTER_MESSAGE, uMsg, wParam, lParam)
		MSG_WM_CONTEXTMENU(_OnContextMenu)
		COMMAND_HANDLER_EX(ID_CONTEXT_MENU_COPY, BN_CLICKED, _OnMenuCopyClicked)
		COMMAND_HANDLER_EX(IDC_LIST_FILTERS, LBN_SELCHANGE, OnListFilterSelChange)
		COMMAND_HANDLER_EX(IDC_LIST_PINS, LBN_SELCHANGE, OnListPinSelChange)
	//MSG_WM_COMMAND(OnCommand)
	END_MSG_MAP()

	HWND GetDlgItem(int nID) const throw()
	{
		ATLASSERT(::IsWindow(m_hwnd));
		return ::GetDlgItem(m_hwnd, nID);
	}
protected:
	HRESULT _GetFilterList();
	HRESULT _ClearFilterList();
	
	HRESULT _GetPinList(IBaseFilter* pFilter);
	HRESULT _ClearPinList();
	HRESULT _GetFilterDetailInfo(IBaseFilter* pFilter);

	HRESULT _GetMediaTypeList(IPin* pPin);
	HRESULT _ClearMediaTypeList();
	HRESULT _GetPinDetailInfo(IPin* pPin);

	HRESULT _GetInterfaceList(IUnknown* pUnknown);
	HRESULT _ClearInterfaceList();

	void OnListFilterSelChange(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnListPinSelChange(UINT uNotifyCode, int nID, CWindow wndCtl);
	void _OnContextMenu(CWindow wnd, CPoint point);
	void _OnMenuCopyClicked(UINT uNotifyCode, int nID, CWindow wndCtl);

	//call back for interface detect
	static HRESULT OnDetectInterfaceCallBack(DWORD_PTR pParam, IUnknown* pUnknwon, 
		DWORD dwInterfaceCount,	REFIID checkedRIIF, LPCTSTR pszInterfaceName);
private:
	CGraphInfoPropertyPage(IUnknown * pUnk, HRESULT * phr);
	void SetDirty(BOOL bDirty);
	BOOL                OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	CComPtr<IBaseFilter>	m_spFilter;
	CComPtr<IFilterGraph>	m_spFilterGraph;
	CStatic					m_stInterfaces;
	CStatic					m_stMediaTypes;
	CStatic					m_stDetail;
	CListBox				m_listFilters;
	CListBox				m_listPins;
	CListBox				m_listMediaTypes;
	CListBox				m_listInterfaces;
	CEdit					m_edtDetail;
	//CButton					m_btnClearLog;
};
