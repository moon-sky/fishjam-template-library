// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "LotteryMgr.h"

class CMainDlg : public CDialogImpl<CMainDlg>
    , public CWinDataExchange<CMainDlg>
{
public:
	enum { IDD = IDD_MAINDLG };
    CMainDlg();
    ~CMainDlg();

    BEGIN_DDX_MAP(CMainDlg)
        DDX_CONTROL_HANDLE(IDC_STATIC_PIC, m_StaicPic)
        DDX_UINT(IDC_STATIC_IMAGE_COUNT, m_dwImageCount)
    END_DDX_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
        MSG_WM_DESTROY(OnDestroy)

        COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
        COMMAND_ID_HANDLER_EX(IDC_BTN_INIT, OnBtnInitClick)
        COMMAND_ID_HANDLER_EX(IDC_BTN_START, OnBtnStartClick)
        COMMAND_ID_HANDLER_EX(IDC_BTN_PAUSE_RESUME, OnBtnPauseResumeClick)
        COMMAND_ID_HANDLER_EX(IDC_BTN_RESET, OnBtnResetClick)
        
        MESSAGE_HANDLER_EX(UM_ADD_LOTTERY_INFO, OnAddLotteryInfo)
        MESSAGE_HANDLER_EX(UM_UPDATE_LOTTERY_INFO, OnUpdateLotteryInfo)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    void OnDestroy();

    void OnBtnInitClick(UINT uNotifyCode, int nID, CWindow wndCtl);
    void OnBtnStartClick(UINT uNotifyCode, int nID, CWindow wndCtl);
    void OnBtnPauseResumeClick(UINT uNotifyCode, int nID, CWindow wndCtl);
    void OnBtnResetClick(UINT uNotifyCode, int nID, CWindow wndCtl);
    
    LRESULT OnAddLotteryInfo(UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT OnUpdateLotteryInfo(UINT uMsg, WPARAM wParam, LPARAM lParam);

    void _SetButtonStatus(BOOL bInited, BOOL bStarted, BOOL bPaused);
private:
    CSize       m_szThumbnail;
    CLotteryMgr m_lotteryMgr;
    CAtlString  m_strInitPath;
    CStatic     m_StaicPic;
    CFCalcRect* m_pCalcRect;
    UINT        m_dwImageCount;
};
