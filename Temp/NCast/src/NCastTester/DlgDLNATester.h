#pragma once

#include "DialogResizeEx.h"
#include "resource.h"
#include "../Hanlders/CastingHandler.h"

class CDlgDLNATester : 
    public CDialogImpl<CDlgDLNATester>,
    public CWinDataExchange<CDlgDLNATester>,
    public CDialogResizeEx<CDlgDLNATester>,
    public INCastingEventCallback
{
public:
    CDlgDLNATester(void);
    ~CDlgDLNATester(void);

    enum { IDD = IDD_DLG_DLNA };

    BEGIN_MSG_MAP(CDlgDLNATester)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_DESTROY(OnDestroy)

        COMMAND_ID_HANDLER_EX(IDC_BTN_REFRESH_DEVICE, OnBtnRefreshDeviceClick)
        CHAIN_MSG_MAP(CDialogResize<CDlgDLNATester>)
        REFLECT_NOTIFICATIONS()
    END_MSG_MAP(CDlgDLNATester)

    BEGIN_DDX_MAP(CDlgDLNATester)
        //DDX_CONTROL(IDC_BTN_ST, m_BtnSt)
        //DDX_CONTROL(IDC_BTN_SHADE, m_BtnShade)
        //DDX_CONTROL(IDC_STATIC_HYPER_LINK, m_HyperLink)
        DDX_CONTROL_HANDLE(IDC_TREE_DLNA, m_TreeDlna)
    END_DDX_MAP()

    BEGIN_DLGRESIZE_MAP(CDlgDLNATester)
        //DLGRESIZE_CONTROL(IDOK, DLSZ_MOVE_X)
        //DLGRESIZE_CONTROL(IDCANCEL, DLSZ_MOVE_X)
        //DLGRESIZE_CONTROL(IDC_BTN_ST, DLSZ_MOVE_X)
        //DLGRESIZE_CONTROL(IDC_BTN_SHADE, DLSZ_MOVE_X)
    END_DLGRESIZE_MAP()

public:
    //INCastingEventCallback
    virtual void OnDeviceAdd(INDevice* pDevice);
    virtual void OnDeviceRemove(INDevice* pDevice);

private:
    BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
    void OnDestroy();
    void OnBtnRefreshDeviceClick(UINT uNotifyCode, int nID, CWindow wndCtl);
private:
    INCastingHandler*    m_pDLNACasting;
    CTreeViewCtrlEx      m_TreeDlna;
};
