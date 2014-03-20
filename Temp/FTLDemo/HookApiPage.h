#pragma once

#if defined(_M_IX86)
//#  define HOOK_API_DETOURS
#endif 

//#define HOOK_API_EASYHOOK
//#define HOOK_API_MINHOOK

struct _HOOK_TRACE_INFO_;
typedef  struct _HOOK_TRACE_INFO_ HOOK_TRACE_INFO;

typedef int (WINAPI *MESSAGEBOXW)(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType);

class CHookApiPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CHookApiPage)

public:
	CHookApiPage();
	virtual ~CHookApiPage();

// Dialog Data
	enum { IDD = IDD_PAGE_HOOKAPI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnCallMessageBoxW();
	afx_msg void OnBnClickedBtnHookApiDetoursInstall();
	afx_msg void OnBnClickedBtnHookApiDetoursUnInstall();
	afx_msg void OnBnClickedBtnHookApiEasyHookInstall();
	afx_msg void OnBnClickedBtnHookApiEasyHookUnInstall();

	afx_msg void OnBnClickedBtnHookApiMinHookInstall();
	afx_msg void OnBnClickedBtnHookApiMinHookUnInstall();

	MESSAGEBOXW m_pOrigMessageBoxW;
	BOOL	m_bDetourIsHooked;
	BOOL	m_bEasyHookIsHooked;
	BOOL	m_bMinHookIsHooked;

#ifdef HOOK_API_EASYHOOK
	HOOK_TRACE_INFO*		m_pHookMessageBoxW;
	ULONG					m_HookCreateFileW_ACLEntries[1];
#endif 
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnHookApiTestJmp();
};
