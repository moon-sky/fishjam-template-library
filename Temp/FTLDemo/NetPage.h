#pragma once

#include <ftlNet.h>
#include <ftlSocket.h>

class CMyNetServer : public CFNetServerT<DWORD>
{
public:
    CMyNetServer(FTL::FSocketType socketType)
        :CFNetServerT<DWORD>(socketType)
    {

    }
};

class CNetPage 
	: public CPropertyPage
	, public FTL::IFThreadPoolCallBack<FTransferJobInfoPtr>
	
{
	DECLARE_DYNAMIC(CNetPage)

public:
	CNetPage();
	virtual ~CNetPage();

// 对话框数据
	enum { IDD = IDD_PAGE_NET };

public:
	//IFThreadPoolCallBack<FTransferJobInfoPtr>
	//virtual void OnJobBegin(LONG nJobIndex, CFJobBase<FTransferJobInfoPtr>* pJob)
	virtual void OnJobEnd(LONG nJobIndex, CFJobBase<FTransferJobInfoPtr>* pJob);
	//virtual void OnJobCancel(LONG nJobIndex, CFJobBase<FTransferJobInfoPtr>* pJob)
	virtual void OnJobProgress(LONG nJobIndex, CFJobBase<FTransferJobInfoPtr>* pJob, LONGLONG nCurPos, LONGLONG nTotalSize);
	//virtual void OnJobError(LONG nJobIndex, CFJobBase<FTransferJobInfoPtr>* pJob, DWORD dwError, LPCTSTR pszDescription){}; 
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnStartServer();
    afx_msg void OnBnClickedBtnStopServer();
	afx_msg void OnBnClickedBtnWininetConnect();
	afx_msg void OnBnClickedBtnWinhttpConnect();

	afx_msg void OnBnClickedBtnInternetTransferAddDownload();
	afx_msg void OnBnClickedBtnInternetTransferAddUpload();
	afx_msg void OnBnClickedBtnInternetTransferCancelJob();
	afx_msg void OnBnClickedBtnInternetTransferStop();

    afx_msg void OnBnClickedBtnSocketDumpOption();
    afx_msg void OnBnClickedBtnTransmitFile();
private:
	// ServerHost
	CString m_strServerHost;
	CMyNetServer*    m_pMyServer;
	void SetButtonStatus(BOOL bServerEnabled);

	HINTERNET	m_hSession;
	HINTERNET	m_hConnect;
	HINTERNET	m_hRequest;
public:


	virtual BOOL OnSetActive();
	FTL::CFInternetTransfer*	m_pInternetTranser;
	INT m_nJobIndex;
};
