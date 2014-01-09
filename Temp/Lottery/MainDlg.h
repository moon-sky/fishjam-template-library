// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "LotteryMgr.h"
#include <ftlWindow.h>
#include "ButtonST.h"

class CMainDlg : public CDialogImpl<CMainDlg>
    , public CWinDataExchange<CMainDlg>
    , public CDialogResize<CMainDlg>
{
public:
	enum { IDD = IDD_MAINDLG };
    CMainDlg();
    ~CMainDlg();

    BEGIN_DLGRESIZE_MAP(CMainDlg)
        //DLGRESIZE_CONTROL(IDC_STATIC_LOTTERY, DLSZ_SIZE_X)
        DLGRESIZE_CONTROL(IDC_BTN_START, DLSZ_MOVE_X)
        DLGRESIZE_CONTROL(IDC_BTN_INIT, DLSZ_MOVE_X| DLSZ_MOVE_Y)
        DLGRESIZE_CONTROL(IDC_BTN_RESET, DLSZ_MOVE_X| DLSZ_MOVE_Y)
        DLGRESIZE_CONTROL(IDC_STATIC_PIC, DLSZ_SIZE_X | DLSZ_SIZE_Y)
    END_DLGRESIZE_MAP()


    BEGIN_DDX_MAP(CMainDlg)
        DDX_CONTROL_HANDLE(IDC_STATIC_PIC, m_StaicPic)
        DDX_UINT(IDC_STATIC_IMAGE_COUNT, m_dwImageCount)
        DDX_CONTROL(IDC_BTN_START, m_btnStart)
    END_DDX_MAP()

	BEGIN_MSG_MAP(CMainDlg)
        //DUMP_WINDOWS_MSG(__FILE__LINE__, NULL, 0, uMsg, wParam, lParam)

		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		//COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
        MSG_WM_DESTROY(OnDestroy)
        MSG_WM_KEYDOWN(OnKeyDown)
        MSG_WM_ERASEBKGND(OnEraseBkgnd)

        COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
        COMMAND_ID_HANDLER_EX(IDC_BTN_INIT, OnBtnInitClick)
        COMMAND_ID_HANDLER_EX(IDC_BTN_START, OnBtnStartClick)
        COMMAND_ID_HANDLER_EX(IDC_BTN_RESET, OnBtnResetClick)
        COMMAND_ID_HANDLER_EX(IDC_BTN_CONFIG, OnBtnConfigClick)
        
        MESSAGE_HANDLER_EX(UM_ADD_LOTTERY_INFO, OnAddLotteryInfo)
        MESSAGE_HANDLER_EX(UM_INIT_LOTTERY_COMPLETE, OnInitLotteryComplete)
        MESSAGE_HANDLER_EX(UM_UPDATE_LOTTERY_INFO, OnUpdateLotteryInfo)

        CHAIN_MSG_MAP(CDialogResize<CMainDlg>)
        REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    void OnDestroy();
    void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    BOOL OnEraseBkgnd(CDCHandle dc);

    void OnBtnInitClick(UINT uNotifyCode, int nID, CWindow wndCtl);
    void OnBtnStartClick(UINT uNotifyCode, int nID, CWindow wndCtl);
    void OnBtnResetClick(UINT uNotifyCode, int nID, CWindow wndCtl);
    void OnBtnConfigClick(UINT uNotifyCode, int nID, CWindow wndCtl);
    
    LRESULT OnAddLotteryInfo(UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT OnInitLotteryComplete(UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT OnUpdateLotteryInfo(UINT uMsg, WPARAM wParam, LPARAM lParam);

    void _SetButtonStatus(BOOL bIniting, BOOL bStarted, BOOL bPaused);
private:
    CSize       m_szThumbnail;
    CLotteryMgr m_lotteryMgr;
    CAtlString  m_strInitPath;
    CStatic     m_StaicPic;
    CFCalcRect* m_pCalcRect;
    UINT        m_dwImageCount;
    CButtonST   m_btnStart;

    //CConfigDlg dlg;
};
