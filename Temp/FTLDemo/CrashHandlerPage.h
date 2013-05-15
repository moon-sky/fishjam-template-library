#pragma once


// CCrashHandlerPage 对话框

class CCrashHandlerPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CCrashHandlerPage)

public:
	CCrashHandlerPage();
	virtual ~CCrashHandlerPage();

// 对话框数据
	enum { IDD = IDD_PAGE_CRASH_HANDLER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnCrash();
    afx_msg void OnBnClickedBtnDisResourcelessDlg();
};
