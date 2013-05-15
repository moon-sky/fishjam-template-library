#pragma once

#include <ftlNLS.h>

// CNLSPage 对话框

class CNLSPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CNLSPage)

public:
	CNLSPage();
	virtual ~CNLSPage();

// 对话框数据
	enum { IDD = IDD_PAGE_NLS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
    void SetButtonStatus(BOOL bServerEnabled);
public:
};
