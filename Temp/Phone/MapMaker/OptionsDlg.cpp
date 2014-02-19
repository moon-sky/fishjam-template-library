#include "StdAfx.h"
#include "OptionsDlg.h"

COptionsDlg::COptionsDlg(void)
{
    m_nTileWidth = 0;
    m_nTileHeight = 0;
    m_nTranspant = 0;
}

COptionsDlg::~COptionsDlg(void)
{
}

BOOL COptionsDlg::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
{
    DoDataExchange(FALSE);
    return TRUE;
}

void COptionsDlg::OnOk(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    DoDataExchange(TRUE);
    EndDialog(IDOK);
}

void COptionsDlg::OnCancel(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    EndDialog(IDCANCEL);
}


