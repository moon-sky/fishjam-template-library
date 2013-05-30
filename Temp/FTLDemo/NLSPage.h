#pragma once

#include <ftlNLS.h>


class CNLSPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CNLSPage)

public:
	CNLSPage();
	virtual ~CNLSPage();

	enum { IDD = IDD_PAGE_NLS };
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
private:
	BOOL _InitControls();
	BOOL _RefreshCodePages(DWORD dwFlags);
	BOOL _TestConverion();
public:
	CComboBox	m_cmbCodePages;
	static BOOL CALLBACK _InnerCodePageEnumProc(LPTSTR lpCodePageString);
private:
public:
	afx_msg void OnBnClickedRadioCodepageSupported();
	afx_msg void OnBnClickedRadioCodepageInstalled();
};
