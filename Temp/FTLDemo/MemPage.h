#pragma once

#include <ftlMem.h>
// CMemPage dialog

class CMemPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CMemPage)

public:
	CMemPage();
	virtual ~CMemPage();

// Dialog Data
	enum { IDD = IDD_PAGE_MEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
    void SucceedCheckAllocAndFree();
    void FaildCheckAllocAndFree();
    void CheckErrorMemoryFree();
public:
    afx_msg void OnBnClickedBtnMemCheckMemLeak();
    afx_msg void OnBnClickedBtnCheckMultiThreadMemLeak();
    afx_msg void OnBnClickedBtnCheckErrorFreeMemory();
    afx_msg void OnBnClickedBtnUseMemoryPool();
};
