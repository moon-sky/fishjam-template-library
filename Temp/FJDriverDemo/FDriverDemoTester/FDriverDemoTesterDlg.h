
// FDriverDemoTesterDlg.h : header file
//

#pragma once

#include "FDriverDemoController.h"
 
// CFDriverDemoTesterDlg dialog
class CFDriverDemoTesterDlg : public CDialog
{
// Construction
public:
	CFDriverDemoTesterDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_FDRIVERDEMOTESTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnInstallService();

private:
	CFDriverDemoController	m_DriverController;
	CString m_strDriverPath;
	CDC         m_MemoryDC;
    CRect       m_rcClient;
    HBITMAP     m_hBmpWindow;
    HBITMAP     m_hOldBmp;
    HANDLE      m_hFileMapping;
    //CAtlFileMapping m_bmpFileMap;
    
	//CBitmap	m_bmpWindow;
	//CBitmap* m_pOldBmp;
	BOOL _RefreshMemoryDC();
public:
	afx_msg void OnBnClickedBtnChooseDriverFile();
	afx_msg void OnBnClickedBtnOpenDriver();
	afx_msg void OnBnClickedBtnCloseDriver();
	afx_msg void OnBnClickedBtnUninstallService();
	afx_msg void OnBnClickedBtnInstallHook();
	afx_msg void OnBnClickedBtnUninstallHook();
	afx_msg void OnBnClickedBtnDoBitblt();
	afx_msg void OnBnClickedBtnFilterDesktop();
    afx_msg void OnBnClickedBtnDoTextout();
	afx_msg void OnBnClickedBtnTestDesktop();
};
