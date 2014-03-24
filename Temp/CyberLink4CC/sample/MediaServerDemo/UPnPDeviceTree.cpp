#include "StdAfx.h"
#include "UPnPDeviceTree.h"

#include <uhttp/util/StringConverter.h>

using namespace CyberLink;
using namespace uHTTP;

CUPnPDeviceTree::CUPnPDeviceTree(void)
{
    m_pControlPoint = NULL;
}

CUPnPDeviceTree::~CUPnPDeviceTree(void)
{
}

void CUPnPDeviceTree::SetControlPoint(ControlPoint* pControlPoint)
{
    m_pControlPoint = pControlPoint;
}

void CUPnPDeviceTree::Refresh()
{
    BOOL bRet = FALSE;
    API_VERIFY(DeleteAllItems());

    if (m_pControlPoint)
    {
        DeviceList* pDeviceList = m_pControlPoint->getDeviceList();
            int nDeviceCount = pDeviceList->size();
        for (int i = 0; i < nDeviceCount; i++)
        {
            Device* pDevice = pDeviceList->getDevice(i);
            _InsertDevice(pDevice);
        }
    }
}

void CUPnPDeviceTree::_InsertDevice(CyberLink::Device* pDevice)
{
    TVITEM tvi = { 0 };             // TreeView Item
    TVINSERTSTRUCT tvins = { 0 };   // TreeView Insert Struct
    HTREEITEM hPrev = NULL;         // Previous Item Added

    tvi.mask = TVIF_TEXT | TVIF_PARAM;
    tvi.cChildren = 1;
    tvi.mask |= TVIF_CHILDREN;

    std::wstring wstrFriendName;
    StringConverter::LocalToUTF16(pDevice->getFriendlyName(), wstrFriendName, CP_ACP);
    tvi.pszText = (LPTSTR)wstrFriendName.c_str();
    tvi.cchTextMax = MAX_PATH;
    tvi.lParam = (LPARAM)pDevice;

    tvins.hParent = NULL;
    tvins.hInsertAfter = NULL;
    tvins.item = tvi;

    CTreeItem item = this->InsertItem(&tvins);

}