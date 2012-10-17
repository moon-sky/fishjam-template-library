// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once


#include "VideoRecord/NRecordGraph.h"

class CMainDlg : public CDialogImpl<CMainDlg>
	, public IRecordGraphNotify
{
public:
	CMainDlg();
	virtual ~CMainDlg();
public:
	enum { IDD = IDD_MAINDLG };

	BEGIN_MSG_MAP_EX(CMainDlg)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_TIMER(OnTimer)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_ID_HANDLER_EX(IDC_BTN_INIT, OnInitCapture)
		COMMAND_ID_HANDLER_EX(IDC_BTN_STOPWHENREADY, OnStopWhenReady)
		COMMAND_ID_HANDLER_EX(IDC_BTN_START, OnStartCapture)
		COMMAND_ID_HANDLER_EX(IDC_BTN_PAUSE_RESUME, OnPauseResumeCapture)
		COMMAND_ID_HANDLER_EX(IDC_BTN_STOP, OnStopCapture)
		COMMAND_ID_HANDLER_EX(IDC_BTN_UNINIT, OnUnInitCapture)
	END_MSG_MAP()

	//IRecordGraphNotify
	virtual int OnNotifyEvent(long EventCode, LONG_PTR EventParam1,LONG_PTR EventParam2);

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	void OnDestroy();
	BOOL OnEraseBkgnd(CDCHandle dc);
	void OnTimer(UINT_PTR nIDEvent);

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);


	void OnInitCapture(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnStopWhenReady(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnStartCapture(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnPauseResumeCapture(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnStopCapture(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnUnInitCapture(UINT uNotifyCode, int nID, CWindow wndCtl);
protected:
	VOID _SetButtonStatus(RecordStatus	status);
private:
	FTL::CFAutoGdiplus*			m_pAutoGdiPlus;
	CNRecordGraph*				m_pNRecordGraph;
};
