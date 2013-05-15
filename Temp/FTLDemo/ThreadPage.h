#pragma once

// CThreadPage 对话框

#include <ftlThread.h>
#include "afxcmn.h"

class CThreadPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CThreadPage)

public:
	CThreadPage();
	virtual ~CThreadPage();

// 对话框数据
	enum { IDD = IDD_PAGE_THREAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
protected:
    static unsigned int __stdcall RWLockReadThreadProc(void* pParam);
    static unsigned int __stdcall RWLockWriteThreadProc(void* pParam);

    CFRWLocker*                         m_pRWLocker;
    std::vector<HANDLE>                 m_hReadThreads;
    std::vector<HANDLE>                 m_hWriteThreads;

    FTL::CFThread<>     m_TestThread;
    static DWORD __stdcall TestThreadProc(void* pParam);
    void SetReadWriteLockButtonStatus(BOOL bStarted);
    void SetThreadButtonStatus(BOOL bStarted, BOOL bPaused);
    void SetThreadPoolButtonStatus(BOOL bStarted, BOOL bPaused);
    LRESULT AFX_MSG_CALL OnThreadUpdateProgress(WPARAM wParam, LPARAM lParam);
protected:
    CProgressCtrl m_ProgressOfThread;
public:
    afx_msg void OnBnClickedBtnThreadStart();
    afx_msg void OnBnClickedBtnThreadStop();
    afx_msg void OnBnClickedBtnThreadPauseResume();
    afx_msg void OnDestroy();
    afx_msg void OnBnClickedBtnRwlockStart();
    afx_msg void OnBnClickedBtnRwlockAddRead();
    afx_msg void OnBnClickedBtnRwlockAddWrite();
    afx_msg void OnBnClickedBtnRwlockStop();
};
