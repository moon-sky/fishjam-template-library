#include "StdAfx.h"
#include "UPnPDeviceTree.h"

#include <uhttp/util/AutoLock.h>
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
        //AutoLock<CyberLink::ControlPoint> lockObj(m_pControlPoint);

        DeviceList* pDeviceList = m_pControlPoint->getDeviceList();
            int nDeviceCount = pDeviceList->size();
        HTREEITEM hPrev = NULL;
        for (int i = 0; i < nDeviceCount; i++)
        {
            Device* pDevice = pDeviceList->getDevice(i);
            hPrev = _InsertDevice(pDevice, hPrev);
        }
    }
}

HTREEITEM CUPnPDeviceTree::_InsertDevice(Device* pDevice, HTREEITEM hPrev)
{
    TVITEM tvi = { 0 };             // TreeView Item
    TVINSERTSTRUCT tvins = { 0 };   // TreeView Insert Struct

    tvi.mask = TVIF_TEXT | TVIF_PARAM;
    tvi.cChildren = 1;
    tvi.mask |= TVIF_CHILDREN;

    std::string DeviceInfo = pDevice->getFriendlyName();
    DeviceInfo += "|";
    DeviceInfo += pDevice->getDeviceType();
    FTL::CFConversion conv;
    tvi.pszText = (LPTSTR)conv.MBCS_TO_TCHAR(DeviceInfo.c_str());
    
    tvi.cchTextMax = MAX_PATH;
    tvi.lParam = (LPARAM)pDevice;

    tvins.hParent = NULL;
    tvins.hInsertAfter = hPrev;
    tvins.item = tvi;

    CTreeItem item = this->InsertItem(&tvins);
    ServiceList* pServices = pDevice->getServiceList();
    int nServiceCount = pServices->size();

    hPrev = NULL;
    for (int i = 0; i < nServiceCount; i++)
    {
        Service* pService = pServices->getService(i);
        hPrev = _InsertService(pService, item.m_hTreeItem, hPrev);
    }
    return item.m_hTreeItem;
}

HTREEITEM CUPnPDeviceTree::_InsertService(CyberLink::Service* pService, HTREEITEM hParent, HTREEITEM hPrev){
    TVITEM tvi = { 0 };             // TreeView Item
    TVINSERTSTRUCT tvins = { 0 };   // TreeView Insert Struct

    tvi.mask = TVIF_TEXT | TVIF_PARAM;
    tvi.cChildren = 1;
    tvi.mask |= TVIF_CHILDREN;

    std::string ServiceInfo = pService->getServiceID();
    ServiceInfo += "|";
    ServiceInfo += pService->getServiceType();
    FTL::CFConversion conv;
    tvi.pszText = (LPTSTR)conv.MBCS_TO_TCHAR(ServiceInfo.c_str());

    tvi.cchTextMax = MAX_PATH;
    tvi.lParam = (LPARAM)pService;

    tvins.hParent = hParent;
    tvins.hInsertAfter = hPrev;
    tvins.item = tvi;

    CTreeItem item = this->InsertItem(&tvins);
    ActionList* pActionList = pService->getActionList();
    int actionCount = pActionList->size();
    hPrev = NULL;
    for (int i = 0; i < actionCount; i++)
    {
        Action* pAction = pActionList->getAction(i);
        hPrev = _InsertAction(pAction, item.m_hTreeItem, hPrev);
    }
    return item.m_hTreeItem;
}

HTREEITEM CUPnPDeviceTree::_InsertAction(CyberLink::Action* pAction, HTREEITEM hParent, HTREEITEM hPrev){
    TVITEM tvi = { 0 };             // TreeView Item
    TVINSERTSTRUCT tvins = { 0 };   // TreeView Insert Struct

    tvi.mask = TVIF_TEXT | TVIF_PARAM;
    tvi.cChildren = 1;
    tvi.mask |= TVIF_CHILDREN;

    std::string ActionInfo = pAction->getName();
    ActionInfo += "(";
    std::ostringstream os;
    ArgumentList *pArgList = pAction->getArgumentList();
    int nArgCount = pArgList->size();
    for (int i = 0; i < nArgCount; i++)
    {
        Argument* pArgument = pArgList->getArgument(i);
        os << pArgument->getName();
        if (i < nArgCount - 1 )
        {
             os << ",";
        }
        else{
            os << ")";
        }
    }

    ActionInfo += os.str();
    FTL::CFConversion conv;
    tvi.pszText = (LPTSTR)conv.MBCS_TO_TCHAR(ActionInfo.c_str());

    tvi.cchTextMax = MAX_PATH;
    tvi.lParam = (LPARAM)pAction;

    tvins.hParent = hParent;
    tvins.hInsertAfter = hPrev;
    tvins.item = tvi;

    CTreeItem item = this->InsertItem(&tvins);
    return item.m_hTreeItem;
}