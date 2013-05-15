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


// CNetPage 对话框

class CNetPage 
	: public CPropertyPage
	//, public FTL::IFThreadPoolCallBack<FTransferJobInfoPtr>
	, public FTL::IInternetTransferCallBack
	
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

	//virtual BOOL OnPromptSaveFile(__in LONG nJobIndex, __inout CFJobBase<FTransferJobInfoPtr>* pJob , 
	//	__in LONGLONG nTotalFileSize, __inout LPTSTR pszFileName, __in DWORD nBufferLenght)
	//{
	//	CString strFile;
	//	strFile.Format(TEXT("Name=%s, Size=%lld"), pszFileName, nTotalFileSize);
	//	AfxMessageBox(strFile);
	//	return TRUE;
	//}

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
