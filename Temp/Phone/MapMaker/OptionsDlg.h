#pragma once

#include "resource.h"
class COptionsDlg : public CDialogImpl<COptionsDlg>, 
    public CWinDataExchange<COptionsDlg>
{
public:
    int m_nTileWidth;
    int m_nTileHeight;
    BEGIN_DDX_MAP(COptionsDlg)
        DDX_INT(IDC_EDIT_TILE_HEIGHT, m_nTileHeight)
        DDX_INT(IDC_EDIT_TILE_WIDTH, m_nTileWidth)
    END_DDX_MAP()
public:
    enum { IDD = IDD_OPTIONS };

    COptionsDlg(void);
    ~COptionsDlg(void);

    BEGIN_MSG_MAP(COptionsDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
        COMMAND_ID_HANDLER_EX(IDOK, OnOk)
        COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
    END_MSG_MAP()

protected:
     BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
     void OnOk(UINT uNotifyCode, int nID, CWindow wndCtl);
     void OnCancel(UINT uNotifyCode, int nID, CWindow wndCtl);
};
