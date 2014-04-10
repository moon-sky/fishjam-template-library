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
    m_pDLNACasting->Init(this);
    return TRUE;
}

void CDlgDLNATester::OnDestroy()
{
    m_pDLNACasting->Release();
    m_pDLNACasting = NULL;
}

void CDlgDLNATester::OnBtnRefreshDeviceClick(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    CTreeItem selItem = m_TreeDlna.GetSelectedItem();
    if (selItem != NULL)
    {
        m_pDLNACasting->PlayMediaObjectOnDevice(NULL, (INDevice*)selItem.GetData());
    }
    
}

void CDlgDLNATester::OnDeviceAdd(INDevice* pDevice)
{
    FTL::CFConversion conv;
    CTreeItem item = m_TreeDlna.InsertItem(conv.UTF8_TO_TCHAR(pDevice->GetFriendlyName()), NULL, NULL);
    if (item != NULL)
    {
        item.SetData(DWORD_PTR(pDevice));
    }
}

void CDlgDLNATester::OnDeviceRemove(INDevice* pDevice)
{

}
