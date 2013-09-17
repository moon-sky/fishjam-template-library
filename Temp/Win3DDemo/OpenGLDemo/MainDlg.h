// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "DialogResizeEx.h"
#include "OpenGLDemoTabCtrls.h"

class CMainDlg : public CDialogImpl<CMainDlg>,
    public CDialogResizeEx<CMainDlg>,
    public CWinDataExchange<CMainDlg>
{
public:
	enum { IDD = IDD_MAINDLG };

    BEGIN_DLGRESIZE_MAP(CMainDlg)
        DLGRESIZE_CONTROL(IDOK, DLSZ_MOVE_X|DLSZ_MOVE_Y)
        //DLGRESIZE_CONTROL(IDCANCEL, DLSZ_MOVE_X)
        //DLGRESIZE_CONTROL(IDC_DIALOG, DLSZ_MOVE_X)
        //DLGRESIZE_CONTROL(ID_APP_ABOUT, DLSZ_MOVE_X|DLSZ_MOVE_Y)
        DLGRESIZE_CONTROL(IDC_TAB_DEMOES, DLSZ_SIZE_X|DLSZ_SIZE_Y)
    END_DLGRESIZE_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
        MSG_WM_DESTROY(OnDestroy)
        //CHAIN_MSG_MAP(CTrayIconImpl<CMainDlg>)
        CHAIN_MSG_MAP(CDialogResizeEx<CMainDlg>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnDestroy(void);
private:
    COpenGLDemoTabCtrls m_tabControls;
    BEGIN_DDX_MAP(CMainDlg)
        DDX_CONTROL(IDC_TAB_DEMOES, m_tabControls)
    END_DDX_MAP()

};
