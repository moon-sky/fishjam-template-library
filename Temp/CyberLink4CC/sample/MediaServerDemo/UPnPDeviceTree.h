#pragma once

#include <cybergarage/upnp/CyberLink.h>

enum ItemType{
    itDevice,
    itService,
    itAction
};

struct ItemParam{
    ItemParam(ItemType type, void* pObject){
        this->type = type;
        this->pUPnPObject = pObject;
    }
    ItemType    type;
    void*       pUPnPObject;
};

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
        //DUMP_WINDOWS_MSG(__FILE__LINE__, NULL, 0, uMsg, wParam, lParam)
        REFLECTED_NOTIFY_CODE_HANDLER_EX(TVN_ITEMEXPANDING, OnItemExpanding)
        REFLECTED_NOTIFY_CODE_HANDLER_EX(NM_RCLICK, OnRClick)
        DEFAULT_REFLECTION_HANDLER()
        //CHAIN_MSG_MAP( BaseClass )
    END_MSG_MAP()

private:
    CyberLink::ControlPoint* m_pControlPoint;
    typedef std::list<ItemParam*>   ItemParamList;
    ItemParamList                   m_ItemParams;
    HTREEITEM _InsertDevice(CyberLink::Device* pDevice, HTREEITEM hPrev);
    HTREEITEM _InsertService(CyberLink::Service* pService, HTREEITEM hParent, HTREEITEM hPrev);
    HTREEITEM _InsertAction(CyberLink::Action* pAction, HTREEITEM hParent, HTREEITEM hPrev);


    LRESULT OnItemExpanding(LPNMHDR pnmh);
    LRESULT OnRClick(LPNMHDR pnmh);
};
