#include "StdAfx.h"
#include "DlgDLNATester.h"
#include "../Hanlders/DLNAHandler/DLNAHandler.h"

CDlgDLNATester::CDlgDLNATester(void)
:m_pDLNACasting(NULL)
{
}

CDlgDLNATester::~CDlgDLNATester(void)
{
}


BOOL CDlgDLNATester::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
{
    DoDataExchange();
    DlgResize_Init(FALSE);

    m_pDLNACasting = CreateDLNAHandlerInstance();//CDLNAHandler::CreateInstance();
    m_pDLNACasting->Init();
    return TRUE;
}

void CDlgDLNATester::OnDestroy()
{
    m_pDLNACasting->Release();
    m_pDLNACasting = NULL;
}

void CDlgDLNATester::OnBtnRefreshDeviceClick(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    m_pDLNACasting->RefreshDevice();
}