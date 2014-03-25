#pragma once

#include <cybergarage/upnp/CyberLink.h>

class CUPnPDeviceTree:public CWindowImpl<CUPnPDeviceTree, CTreeViewCtrlEx>
{
    typedef CWindowImpl<CUPnPDeviceTree, CTreeViewCtrlEx> BaseClass;
public:
    CUPnPDeviceTree(void);
    ~CUPnPDeviceTree(void);
    
public:
    void SetControlPoint(CyberLink::ControlPoint* pControlPoint);
    void Refresh();

    //void AddDevice(CyberLink::Device* pDevice);
    //void RemoveDevice(CyberLink::Device* pDevice);

    BEGIN_MSG_MAP(CUPnPDeviceTree)
        //CHAIN_MSG_MAP( BaseClass )
    END_MSG_MAP()

private:
    CyberLink::ControlPoint* m_pControlPoint;
    HTREEITEM _InsertDevice(CyberLink::Device* pDevice, HTREEITEM hPrev);
    HTREEITEM _InsertService(CyberLink::Service* pService, HTREEITEM hParent, HTREEITEM hPrev);
    HTREEITEM _InsertAction(CyberLink::Action* pAction, HTREEITEM hParent, HTREEITEM hPrev);
};
