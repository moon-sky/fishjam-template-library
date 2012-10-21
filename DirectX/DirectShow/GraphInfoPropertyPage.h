#pragma once

#include "resource.h"

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
	//virtual INT_PTR OnReceiveMessage(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

public:
	BEGIN_DDX_MAP(CGraphInfoPropertyPage)
		DDX_CONTROL_HANDLE(IDC_LIST_FILTERS, m_listFilters)
	END_DDX_MAP()

	BEGIN_MSG_MAP_EX(CGraphInfoPropertyPage)
		//MSG_WM_COMMAND(OnCommand)
	END_MSG_MAP()

	HWND GetDlgItem(int nID) const throw()
	{
		ATLASSERT(::IsWindow(m_hwnd));
		return ::GetDlgItem(m_hwnd, nID);
	}
private:
	CGraphInfoPropertyPage(IUnknown * pUnk, HRESULT * phr);
	void SetDirty(BOOL bDirty);
	BOOL                OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	CComPtr<IBaseFilter>	m_spFilter;
	CComPtr<IFilterGraph>	m_spFilterGraph;
	CListBox				m_listFilters;
};
