#include "StdAfx.h"
#include "UPnPDeviceTree.h"

#include <uhttp/util/AutoLock.h>
#include <uhttp/util/StringConverter.h>
#include <cybergarage/upnp/media/server/action/BrowseAction.h>
#include <ftlFunctional.h>

using namespace CyberLink;
using namespace uHTTP;
using namespace FTL;

CUPnPDeviceTree::CUPnPDeviceTree(void)
{
    m_pControlPoint = NULL;
}

CUPnPDeviceTree::~CUPnPDeviceTree(void)
{
    //BOOL bRet = FALSE;
    for_each(m_ItemParams.begin(), m_ItemParams.end(), ObjecteDeleter<ItemParam*>());
    m_ItemParams.clear();
    //API_VERIFY(DeleteAllItems());
}


void CUPnPDeviceTree::SetControlPoint(ControlPoint* pControlPoint)
{
    m_pControlPoint = pControlPoint;
}

void CUPnPDeviceTree::Refresh()
{
    BOOL bRet = FALSE;
    for_each(m_ItemParams.begin(), m_ItemParams.end(), ObjecteDeleter<ItemParam*>());
    m_ItemParams.clear();
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
    ItemParam* pItemParm = new ItemParam(itDevice, pDevice);
    m_ItemParams.push_back(pItemParm);
    tvi.lParam = (LPARAM)pItemParm;

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
    ItemParam* pItemParm = new ItemParam(itService, pService);
    m_ItemParams.push_back(pItemParm);
    tvi.lParam = (LPARAM)pItemParm;

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
    ItemParam* pItemParm = new ItemParam(itAction, pAction);
    m_ItemParams.push_back(pItemParm);
    tvi.lParam = (LPARAM)pItemParm;
    
    tvins.hParent = hParent;
    tvins.hInsertAfter = hPrev;
    tvins.item = tvi;

    CTreeItem item = this->InsertItem(&tvins);
    return item.m_hTreeItem;
}

LRESULT CUPnPDeviceTree::OnItemExpanding(LPNMHDR pnmh)
{
    FTLTRACE(TEXT("CUPnPDeviceTree::OnItemExpanding\n"));

    return 0;
}

LRESULT CUPnPDeviceTree::OnRClick(LPNMHDR pnmh){
    CPoint ptMouse;
    GetCursorPos(&ptMouse);
    ScreenToClient(&ptMouse);
    UINT uFlags=TVHT_ONITEM;
    HTREEITEM hItem = HitTest(ptMouse, &uFlags);
    if ((hItem != NULL) && (TVHT_ONITEM & uFlags))
    {   
        ItemParam* pItemPara = (ItemParam*)GetItemData(hItem);
        switch (pItemPara->type)
        {
        case itAction:
            {
                Action* pAction = (Action*)pItemPara->pUPnPObject;
                BrowseAction browseAction(pAction);
                browseAction.setObjectID("0");
                browseAction.setBrowseFlag(BrowseAction::BROWSE_METADATA);
                browseAction.setStartingIndex(0);
                browseAction.setRequestedCount(0);
                browseAction.setFilter("*");
                browseAction.setSortCriteria("");
                if (browseAction.postControlAction() == false)
                    return NULL;

                Argument *resultArg = browseAction.getArgument(BrowseAction::RESULT);
                if (resultArg == NULL)
                    return NULL;

                const char *resultStr = resultArg->getValue();
                if (resultStr)
                {
                    std::wstring wstrResult;
                    StringConverter::toUTF16(resultStr, wstrResult);
                    FTLTRACE(TEXT("%s\n"), wstrResult.c_str());
                }
            }
            break;
        default:
            break;
        }
    }
    return 0;
}