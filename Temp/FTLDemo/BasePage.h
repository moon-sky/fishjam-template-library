#pragma once

/*************************************************************************************************
* 测试了 CriticalSection 和 Mutex 的性能差别（100万次循环）
* 1.单线程方式下：CS只要约80毫秒，而Mutex需要约2000毫秒
* 2.双线程交互方式下：CS只要约170毫秒，和Spin值无关，而Mutex需要约4000毫秒；
*   但是，在公司使用的双核机器上，CS需要 1000~5000 毫秒？而且如果设置Spin值，将大大减少所需时间
**************************************************************************************************/


// CBasePage 对话框

class CBasePage : public CPropertyPage
{
	DECLARE_DYNAMIC(CBasePage)

public:
	CBasePage();
	virtual ~CBasePage();

// 对话框数据
	enum { IDD = IDD_PAGE_BASE };
private:
    UINT m_performanceSpinCountNumber;
    BOOL m_nPerformanceType;
    CFElapseCounter m_ElapseCounter;
    CRITICAL_SECTION        m_perCS;
    HANDLE                  m_perMutex;
    static DWORD WINAPI TestPerformanceProc(LPVOID pParam);
    static void loopForMutex(HANDLE hMutex);
    static void loopForCS(LPCRITICAL_SECTION pCS);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
    void SetElapseButtonStatus(FTL::RunningStatus status, BOOL bEnabled);
public:
    afx_msg void OnBnClickedBtnStart();
    afx_msg void OnBnClickedBtnPauseResume();
    afx_msg void OnBnClickedBtnGetElapsetime();
    afx_msg void OnBnClickedBtnStop();
    afx_msg void OnBnClickedBtnExeCounter();
    afx_msg void OnBnClickedBtnPerformanceStart();
    afx_msg void OnBnClickedCheckCsSpincount();
    afx_msg void OnBnClickedRadioPerformanceCs();
    afx_msg void OnBnClickedRadioPerformanceMutex();
	afx_msg void OnBnClickedBtnApiVerify();
};
