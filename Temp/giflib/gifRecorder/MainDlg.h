// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CGifMaker;

class CMainDlg : public CDialogImpl<CMainDlg>
{
public:
	enum { IDD = IDD_MAINDLG };
    CMainDlg();
    ~CMainDlg();

    BEGIN_DDX_MAP(CMainDlg)
        DDX_TEXT(IDC_STATIC_SAVE_PATH, m_strSavePath)
        DDX_INT(IDC_EDIT_RECORD_LEFT, m_nLeft)
        DDX_INT(IDC_EDIT_RECORD_TOP, m_nTop)
        DDX_INT(IDC_EDIT_WIDTH, m_nLeft)
        DDX_INT(IDC_EDIT_RECORD_LEFT, m_nLeft)
        DDX_INT(IDC_EDIT_RECORD_LEFT, m_nLeft)
    END_DDX_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
        
        MSG_WM_DESTROY(OnDestroy)
        COMMAND_ID_HANDLER_EX(IDC_BTN_CHOOSE_SAVE_PATH, OnBtnChooseSavePath)
        COMMAND_ID_HANDLER_EX(IDC_BTN_START_RECORD, OnBtnStartRecord)
        COMMAND_ID_HANDLER_EX(IDC_BTN_STOP_RECORD, OnBtnStopRecord)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);


    void OnBtnChooseSavePath(UINT uNotifyCode, int nID, CWindow wndCtl);
    void OnBtnStartRecord(UINT uNotifyCode, int nID, CWindow wndCtl);
    void OnBtnStopRecord(UINT uNotifyCode, int nID, CWindow wndCtl);
    void OnDestroy();
private:
    CGifMaker*  m_pGifMaker;
    CString m_strSavePath;
    int m_nLeft, m_nTop, m_nWidth, m_nHeight;
    int m_nFps;
};
