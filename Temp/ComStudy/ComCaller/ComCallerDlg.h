// ComCallerDlg.h : header file
//

#pragma once
#include "..\ATLCom\ATLCom.h"
#include "TimerSink.h"

// CComCallerDlg dialog
class CComCallerDlg : public CDialog , IMyMathCallBack
{
// Construction
public:
	CComCallerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_COMCALLER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
    CComPtr<IMyMath2>    m_spMyMath2;
    CComPtr<IMyTimer>   m_spMyTimer;
    CComQIPtr< IConnectionPoint > m_spCPTimer;       //连接点
    CTimerSink          m_TimerSink;
    LONG                m_dwCallbackCookie;
    DWORD               m_dwCPCookie;

    // STDMETHOD(xx) 是宏，等价于 long __stdcall xx
//IUnknown
    STDMETHOD_(ULONG,AddRef)(void);
    STDMETHOD_(ULONG,Release)(void);
    STDMETHOD(QueryInterface)(const struct _GUID &iid,void ** ppv);

//IMyMathCallBack
    STDMETHOD(Fire_Result)(LONG nResult);
public:
    afx_msg void OnClose();

    afx_msg void OnBnClickedBtnViewStorageFile();
    afx_msg void OnBnClickedBtnDrawPic();
    afx_msg void OnBnClickedBtnCallServiceExeMulti();
    afx_msg void OnBnClickedBtnCallMymathAdd();
    afx_msg void OnBnClickedBtnCallMymath2Mul();
    afx_msg void OnBnClickedBtnViewDispatch();
    afx_msg void OnBnClickedBtnImportUpper();
    afx_msg void OnBnClickedBtnDispatchApi();
    afx_msg void OnBnClickedBtnUseCcomdispatchdriver();
    afx_msg void OnBnClickedBtnErrorHandle();
    afx_msg void OnBnClickedBtnSubAndCallback();
    afx_msg void OnBnClickedBtnTimerConnectpoint();
    afx_msg void OnBnClickedBtnPersist();
    afx_msg void OnBnClickedBtnDemoMarshal();
    afx_msg void OnDestroy();
    afx_msg void OnBnClickedBtnSumArray();
    afx_msg void OnBnClickedBtnGetComponentInfo();
    afx_msg void OnBnClickedBtnDetectInterface();
    afx_msg void OnBnClickedBtnMoniker();
    afx_msg void OnBnClickedBtnWia();
    afx_msg void OnBnClickedBtnLengthyComOperation();
    afx_msg void OnBnClickedBtnEventHook();
};
