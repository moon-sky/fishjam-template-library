#pragma once


// CFilePage dialog

class CFilePage : public CPropertyPage
{
	DECLARE_DYNAMIC(CFilePage)

public:
	CFilePage();
	virtual ~CFilePage();

// Dialog Data
	enum { IDD = IDD_PAGE_FILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnFileChooseCopySrcDir();
    afx_msg void OnBnClickedBtnFileChooseCopyDstDir();
    afx_msg void OnBnClickedBtnFileCopyDir();
    virtual BOOL OnInitDialog();
protected:
    CString     m_strCopySrcDir;
    CString     m_strCopyDstDir;
};
